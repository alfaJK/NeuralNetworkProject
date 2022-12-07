#include "CUDA/CudaModuleCalc.h"
#include "stdio.h"
#include <time.h> 
#include <iostream>

#define PAD 1
// calc conv module
#if __CUDA_ARCH__ < 600
__device__ double _atomicAdd(double* address, double val)
{
 unsigned long long int* address_as_ull =
 (unsigned long long int*)address;
 unsigned long long int old = *address_as_ull, assumed;

 do {
 assumed = old;
 old = atomicCAS(address_as_ull, assumed,
 __double_as_longlong(val +
 __longlong_as_double(assumed)));

 // Note: uses integer comparison to avoid hang in case of NaN (since NaN != NaN)
 } while (assumed != old);

 return __longlong_as_double(old);
}
#endif

__device__ double d_ConvDirectDist(int index_x, int index_y ,int index_z , CudaTensor *input , CudaTensor* filter  ,CudaTensor* CBias){
    double sum = CBias->GetElement(0 , 0 , index_z);
    int Padding = PAD; //(filter->Size.width - 1) / 2;
    for (int i = 0; i < filter->Size.height; i++) {
        for (int j = 0; j < filter->Size.width; j++) {
            int i0 = index_y + i - Padding;
            int j0 = index_x + j - Padding;
            if (i0 < 0 || i0 >= input->Size.height || j0 < 0 || j0 >= input->Size.width)
                continue;
            for (int c = 0; c < input->Size.depth; c++)
                sum += input->GetElement(j0, i0 , c) * filter->Data[index_z * (filter->Size.height * filter->Size.width *input->Size.depth) + (filter->Size.width * input->Size.depth * i + j *input->Size.depth + c)];//GetElement(i, j , index_z + c);
        }
    }
    return sum;
}

__global__ void ConvDirectDist_global(CudaTensor* InputMatrix , CudaTensor* Filter , CudaTensor* OutputMatrix ,CudaTensor* CBias){

    int index_x = blockIdx.x * blockDim.x + threadIdx.x;
    int index_y = blockIdx.y * blockDim.y + threadIdx.y;
    int index_z = blockIdx.z * blockDim.z + threadIdx.z;

    double sum = d_ConvDirectDist(index_x, index_y,index_z , InputMatrix, Filter ,CBias);

    __syncthreads();
      OutputMatrix->SetElement(index_x, index_y,index_z ,  sum);
    __syncthreads();


}

int CudaConvDirectDist(const std::vector<double> InputMatrix , std::vector<double> Filter , std::vector<double> &OutputMatrix,std::vector<double> Bias , Tensorsize I, Tensorsize F,Tensorsize O , int CountF){
    dim3 gridSize , blockSize;
   /* if(O.width < BLOCK_SIZE || O.height < BLOCK_SIZE ){

        gridSize = dim3(O.width, O.height, CountF);
        blockSize = dim3(1, 1, 1);
    }else{
        gridSize = dim3(O.width / BLOCK_SIZE, O.height / BLOCK_SIZE, CountF);
        blockSize = dim3(BLOCK_SIZE,BLOCK_SIZE, 1);
    }*/
     //gridSize = dim3(O.width, O.height, CountF);
    // blockSize = dim3(1, 1, 1);
    gridSize = dim3(O.width, O.height , CountF);
    blockSize = dim3(1,1, 1);
    CudaTensor* CInputMatrix = new CudaTensor(I.width, I.height , I.depth , InputMatrix);
    CudaTensor* CFilter = new CudaTensor(F.width, F.height , F.depth * CountF  , Filter);
    CudaTensor* COutputMatrix = new CudaTensor(O.width, O.height , O.depth , OutputMatrix);
    CudaTensor* CBias = new CudaTensor( 1 , 1 , CountF , Bias);
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    ConvDirectDist_global<<< gridSize , blockSize>>>(CInputMatrix, CFilter, COutputMatrix , CBias);

    cudaEventRecord(stop);
    cudaDeviceSynchronize(); 
    cudaGetLastError();
    thrust::copy(COutputMatrix->d_vec.begin(), COutputMatrix->d_vec.end(), OutputMatrix.begin());
    delete CInputMatrix;
    delete CFilter;
    delete COutputMatrix;
    delete CBias;
    return 1;

}

__global__ void  ConvCalcDeltaFilters_global(CudaTensor* InputMatrix , CudaTensor* d_Filers , CudaTensor* d_Bias, CudaTensor* deltas ,  Tensorsize Isize ,int CountF){
    int index_x = blockIdx.x * blockDim.x + threadIdx.x;
    int index_y = blockIdx.y * blockDim.y + threadIdx.y;
    int index_z = blockIdx.z * blockDim.z + threadIdx.z;
        int Padding = PAD;
    	double delta = deltas->GetElement(index_x, index_y,index_z);
        for (int i = 0; i < d_Filers->Size.height; i++) {
            for (int j = 0; j < d_Filers->Size.width; j++) {
                int i0 = index_y + i - Padding;
                int j0 = index_x + j - Padding;
                if (i0 < 0 || i0 >= Isize.height || j0 < 0 || j0 >= Isize.width) continue;
                for (int c = 0; c < Isize.depth; c++){
                    int e = index_z * (d_Filers->Size.height * d_Filers->Size.width *Isize.depth) + (d_Filers->Size.width * Isize.depth * i + j *Isize.depth + c);
                    _atomicAdd(&(d_Filers->Data[e]) , (delta * InputMatrix->GetElement(j0, i0,c)));
                    
                }
            }
        }

        

        __syncthreads();
        _atomicAdd(&d_Bias->Data[index_z] ,  delta );
        __syncthreads();
}

 int CudaConvCalcDFilters(const std::vector<double> InputMatrix ,std::vector<double> &d_Filers , std::vector<double> &d_Bias ,std::vector<double> deltas , Tensorsize I, Tensorsize dF, Tensorsize _size , Tensorsize InputSize, int CountF){
    dim3 gridSize , blockSize;
  /*  if(_size.width < BLOCK_SIZE || _size.height < BLOCK_SIZE ){
        gridSize = dim3(_size.width, _size.height , CountF);
        blockSize = dim3(1,1, 1);
    }else{
        gridSize = dim3(_size.width / BLOCK_SIZE, _size.height / BLOCK_SIZE,  CountF);
        blockSize = dim3(BLOCK_SIZE,BLOCK_SIZE, 1);
    }*/
    gridSize = dim3(_size.width, _size.height , CountF);
    blockSize = dim3(1,1, 1);
    CudaTensor* CInputMatrix = new CudaTensor(I.width, I.height , I.depth , InputMatrix);
    CudaTensor* CdFilter = new CudaTensor(dF.width, dF.height , dF.depth * CountF  , d_Filers);
    CudaTensor* CdBias = new CudaTensor(1, 1 , CountF , d_Bias);
    CudaTensor* Cdeltas = new CudaTensor(_size.width, _size.height , _size.depth  , deltas);
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);
    ConvCalcDeltaFilters_global<<< gridSize , blockSize>>>(CInputMatrix , CdFilter , CdBias ,Cdeltas ,InputSize , CountF);
    cudaEventRecord(stop);
    cudaDeviceSynchronize(); 
    cudaGetLastError();
    thrust::copy(CdFilter->d_vec.begin(), CdFilter->d_vec.end(), d_Filers.begin());
    thrust::copy(CdBias->d_vec.begin(), CdBias->d_vec.end(), d_Bias.begin());
    delete CInputMatrix;
    delete CdFilter;
    delete CdBias;
    delete Cdeltas;
    return 1;

 }

__global__ void  ConvCalcDx_global(CudaTensor* Filter , CudaTensor* dX , CudaTensor* deltas ,Tensorsize _size ,int CountF){
    int index_x = blockIdx.x * blockDim.x + threadIdx.x;
    int index_y = blockIdx.y * blockDim.y + threadIdx.y;
    int index_z = blockIdx.z * blockDim.z + threadIdx.z;
    int Padding = PAD;
	int pad = Filter->Size.width - 1 - Padding;
    //double sum = d_ConvDirectDist(index_x, index_y,index_z , InputMatrix, Filter ,CBias);
    double sum = 0;
		for (int i = 0; i < Filter->Size.width; i++) {
			for (int j = 0; j < Filter->Size.height; j++) {
				int i0 = index_y + i - pad; // height
				int j0 = index_x + j - pad; // width
				if (i0 < 0 || i0 >= _size.height || j0 < 0 || j0 >= _size.width)
					continue;
				for (int f = 0; f < CountF; f++){
                    //solved
                    int i_f =  Filter->Size.width - 1 - i;
                    int j_f =  Filter->Size.width - 1 - j;
                    int e = f * (Filter->Size.height * Filter->Size.width *dX->Size.depth) + (Filter->Size.width * dX->Size.depth * i_f + j_f *dX->Size.depth + index_z);
                    sum += Filter->Data[e] * deltas->GetElement(j0, i0, f); 
                }
			}
		}

    __syncthreads();
      dX->SetElement(index_x, index_y,index_z ,  sum);
    __syncthreads();


}

int CudaConvCalcDx(std::vector<double> Filter , std::vector<double> &dX,std::vector<double> deltas , Tensorsize F, Tensorsize del,Tensorsize Sdx ,Tensorsize _size, int CountF){

    dim3 gridSize , blockSize;
   /* if(Sdx.width < BLOCK_SIZE || Sdx.height < BLOCK_SIZE ){

        gridSize = dim3(Sdx.width, Sdx.height, CountF);
        blockSize = dim3(1, 1, 1);
    }else{
        gridSize = dim3(Sdx.width / BLOCK_SIZE, Sdx.height / BLOCK_SIZE, CountF);
        blockSize = dim3(BLOCK_SIZE,BLOCK_SIZE, 1);
    }*/
    gridSize = dim3(Sdx.width, Sdx.height , Sdx.depth);
    blockSize = dim3(1,1, 1);
    CudaTensor* CdX = new CudaTensor(Sdx.width, Sdx.height , Sdx.depth , dX);
    CudaTensor* CFilter = new CudaTensor(F.width, F.height , F.depth * CountF  , Filter);
    CudaTensor* Cdeltas = new CudaTensor(_size.width, _size.height , _size.depth , deltas);
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    ConvCalcDx_global<<< gridSize , blockSize>>>(CFilter, CdX, Cdeltas ,_size,CountF);

    cudaEventRecord(stop);
    cudaDeviceSynchronize(); 
    cudaGetLastError();
    thrust::copy(CdX->d_vec.begin(), CdX->d_vec.end(), dX.begin());
    delete CdX;
    delete CFilter;
    delete Cdeltas;
    return 1;
}
__global__ void  ActivationDirectDist_global(CudaTensor* InputMatrix , CudaTensor* OutputMatrix){
    int index_x = blockIdx.x * blockDim.x + threadIdx.x;
    int index_y = blockIdx.y * blockDim.y + threadIdx.y;
    int index_z = blockIdx.z * blockDim.z + threadIdx.z;
    __syncthreads();
        if(InputMatrix->GetElement(index_x, index_y, index_z) > 0)
            OutputMatrix->SetElement(index_x, index_y, index_z , InputMatrix->GetElement(index_x, index_y, index_z));
        else OutputMatrix->SetElement(index_x, index_y, index_z , 0);
    __syncthreads();

}
int CudaActivationDirectDist(std::vector<double> InputMatrix , std::vector<double> &OutputMatrix , Tensorsize I ,Tensorsize O){
    dim3 gridSize , blockSize;
 /*   if(O.width < BLOCK_SIZE || O.height < BLOCK_SIZE ){
        gridSize = dim3(O.width, O.height, O.depth);
        blockSize = dim3(1, 1, 1);
    }else{
        gridSize = dim3(O.width / BLOCK_SIZE, O.height / BLOCK_SIZE,  O.depth);
        blockSize = dim3(BLOCK_SIZE,BLOCK_SIZE, 1);
    }*/
    gridSize = dim3(O.width, O.height, O.depth);
    blockSize = dim3(1, 1, 1);
    //gridSize = dim3(O.width / BLOCK_SIZE, O.height / BLOCK_SIZE,  O.depth);
    //blockSize = dim3(BLOCK_SIZE,BLOCK_SIZE, 1);

    CudaTensor* CInputMatrix = new CudaTensor(I.width, I.height , I.depth , InputMatrix);
    CudaTensor* COutputMatrix = new CudaTensor(O.width, O.height , O.depth , OutputMatrix);
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    ActivationDirectDist_global<<< gridSize , blockSize>>>(CInputMatrix, COutputMatrix);

    cudaEventRecord(stop);
    cudaDeviceSynchronize(); 
    cudaGetLastError();
    thrust::copy(COutputMatrix->d_vec.begin(), COutputMatrix->d_vec.end(), OutputMatrix.begin());
    delete CInputMatrix;
    delete COutputMatrix;
    return 1;
}

__global__ void  ActivationBackDist_global(CudaTensor* InputMatrix , CudaTensor* DeltaMatrix , CudaTensor* OutputMatrix){
    int index_x = blockIdx.x * blockDim.x + threadIdx.x;
    int index_y = blockIdx.y * blockDim.y + threadIdx.y;
    int index_z = blockIdx.z * blockDim.z + threadIdx.z;
    __syncthreads();
    //dX(i, j, k) = dout(i, j, k) * (X(i, j, k) > 0 ? 1 : 0); 
        if(InputMatrix->GetElement(index_x, index_y, index_z) > 0)
            OutputMatrix->SetElement(index_x, index_y, index_z , DeltaMatrix->GetElement(index_x, index_y, index_z));
        else OutputMatrix->SetElement(index_x, index_y, index_z , 0);
    __syncthreads();


}

int CudaActivationBackDist(std::vector<double> InputMatrix , std::vector<double> DeltaMatrix , std::vector<double> &OutputMatrix , Tensorsize I , Tensorsize D ,Tensorsize O){
    dim3 gridSize , blockSize;
  /*  if(O.width < BLOCK_SIZE || O.height < BLOCK_SIZE ){
        gridSize = dim3(O.width, O.height, O.depth);
        blockSize = dim3(1, 1, 1);
    }else{
        gridSize = dim3(O.width / BLOCK_SIZE, O.height / BLOCK_SIZE,  O.depth);
        blockSize = dim3(BLOCK_SIZE,BLOCK_SIZE, 1);
    }*/
    gridSize = dim3(O.width, O.height, O.depth);
    blockSize = dim3(1, 1, 1);
    //gridSize = dim3(O.width / BLOCK_SIZE, O.height / BLOCK_SIZE,  O.depth);
    //blockSize = dim3(BLOCK_SIZE,BLOCK_SIZE, 1);
    CudaTensor* CInputMatrix = new CudaTensor(I.width, I.height , I.depth , InputMatrix);
    CudaTensor* CDeltaMatrix = new CudaTensor(D.width, D.height , D.depth , DeltaMatrix);
    CudaTensor* COutputMatrix = new CudaTensor(O.width, O.height , O.depth , OutputMatrix);
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    ActivationBackDist_global<<< gridSize , blockSize>>>(CInputMatrix,CDeltaMatrix, COutputMatrix);

    cudaEventRecord(stop);
    cudaDeviceSynchronize(); 
    cudaGetLastError();
    thrust::copy(COutputMatrix->d_vec.begin(), COutputMatrix->d_vec.end(), OutputMatrix.begin());
    delete CInputMatrix;
    delete CDeltaMatrix;
    delete COutputMatrix;
    return 1;
}


__global__ void  PoolingDirectDist_global(CudaTensor* InputMatrix ,CudaTensor* Filter , CudaTensor* OutputMatrix, int scale){
    int index_x = blockIdx.x * blockDim.x + threadIdx.x;
    int index_y = blockIdx.y * blockDim.y + threadIdx.y;
    int index_z = blockIdx.z * blockDim.z + threadIdx.z;
    __syncthreads();

    if(index_x % scale == 0 && index_y % scale == 0 ){
        int imax = index_y;
        int jmax = index_x; 
        double max = InputMatrix->GetElement(index_y, index_x, index_z);
        for (int y = index_y; y < index_y + scale; y++) {
            for (int x = index_x; x < index_x + scale; x++) {
                double value = InputMatrix->GetElement(y, x, index_z);
                
                Filter->SetElement(y, x, index_z , 0);
                if (value > max) {
                    max = value;
                    imax = y;
                    jmax = x;
                }
            }
        }
        __syncthreads();
        OutputMatrix->SetElement(index_y / scale, index_x / scale, index_z , max);
        Filter->SetElement(imax, jmax, index_z , 1);
    }
    __syncthreads();
}

int CudaPoolingDirectDist(const std::vector<double> InputMatrix , std::vector<double> &Filter , std::vector<double> &OutputMatrix, Tensorsize I, Tensorsize F,Tensorsize O , int scale){
    dim3 gridSize , blockSize;
   /* if(I.width < BLOCK_SIZE || I.height < BLOCK_SIZE ){
        gridSize = dim3(I.width, I.height, I.depth);
        blockSize = dim3(1, 1, 1);
    }else{
        gridSize = dim3(I.width / BLOCK_SIZE, I.height / BLOCK_SIZE,  I.depth);
        blockSize = dim3(BLOCK_SIZE,BLOCK_SIZE, 1);
    }*/
    gridSize = dim3(I.width, I.height, I.depth);
    blockSize = dim3(1, 1, 1);
    //gridSize = dim3(I.width / BLOCK_SIZE, I.height / BLOCK_SIZE,  I.depth);
    //blockSize = dim3(BLOCK_SIZE,BLOCK_SIZE, 1);
    CudaTensor* CInputMatrix = new CudaTensor(I.width, I.height , I.depth , InputMatrix);
    CudaTensor* CFilter = new CudaTensor(F.width, F.height , F.depth , Filter);
    CudaTensor* COutputMatrix = new CudaTensor(O.width, O.height , O.depth , OutputMatrix);
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    PoolingDirectDist_global<<< gridSize , blockSize>>>(CInputMatrix, CFilter , COutputMatrix,scale);

    cudaEventRecord(stop);
    cudaDeviceSynchronize(); 
    cudaGetLastError();
    thrust::copy(COutputMatrix->d_vec.begin(), COutputMatrix->d_vec.end(), OutputMatrix.begin());
    thrust::copy(CFilter->d_vec.begin(), CFilter->d_vec.end(), Filter.begin());
    delete CInputMatrix;
    delete CFilter;
    delete COutputMatrix;
    return 1;
}
/* + */
__global__ void  PoolingBackDist_global(CudaTensor* DeltaMatrix , CudaTensor* FilterMatrix , CudaTensor* dXMatrix, int scale){
    int index_x = blockIdx.x * blockDim.x + threadIdx.x;
    int index_y = blockIdx.y * blockDim.y + threadIdx.y;
    int index_z = blockIdx.z * blockDim.z + threadIdx.z;
    __syncthreads();
        dXMatrix->SetElement(index_y, index_x, index_z , DeltaMatrix->GetElement(index_y / scale, index_x / scale, index_z) * FilterMatrix->GetElement(index_y, index_x, index_z)); 
    __syncthreads();
}

int CudaPoolingBackDist(std::vector<double> deltas , std::vector<double> Filter , std::vector<double> &dX , Tensorsize del, Tensorsize F,Tensorsize Sdx, int scale){
    dim3 gridSize , blockSize;
   /* if(Sdx.width < BLOCK_SIZE || Sdx.height < BLOCK_SIZE ){
        gridSize = dim3(Sdx.width, Sdx.height, Sdx.depth);
        blockSize = dim3(1, 1, 1);
    }else{
        gridSize = dim3(Sdx.width / BLOCK_SIZE, Sdx.height / BLOCK_SIZE,  Sdx.depth);
        blockSize = dim3(BLOCK_SIZE,BLOCK_SIZE, 1);
    }*/
    gridSize = dim3(Sdx.width, Sdx.height, Sdx.depth);
    blockSize = dim3(1, 1, 1);
    //gridSize = dim3(Sdx.width / BLOCK_SIZE, Sdx.height / BLOCK_SIZE,  Sdx.depth);
    //blockSize = dim3(BLOCK_SIZE,BLOCK_SIZE, 1);
    CudaTensor* CDeltaMatrix = new CudaTensor(del.width, del.height , del.depth , deltas);
    CudaTensor* CFilter = new CudaTensor(F.width, F.height , F.depth , Filter);
    CudaTensor* CdXMatrix = new CudaTensor(Sdx.width, Sdx.height , Sdx.depth , dX);
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    PoolingBackDist_global<<< gridSize , blockSize>>>(CDeltaMatrix, CFilter , CdXMatrix,scale);

    cudaEventRecord(stop);
    cudaDeviceSynchronize(); 
    cudaGetLastError();
    thrust::copy(CdXMatrix->d_vec.begin(), CdXMatrix->d_vec.end(), dX.begin());
    delete CDeltaMatrix;
    delete CFilter;
    delete CdXMatrix;
    return 1;
}


