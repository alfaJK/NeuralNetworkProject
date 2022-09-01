#include "CUDA/CudaModuleCalc.h"
#include "stdio.h"
#include <time.h> 
#include <iostream>

// calc conv module

__device__ float d_ConvDirectDist(int index_x, int index_y , CudaTensor *input , CudaTensor* filter){
    float sum = 0;
    int Padding = (filter->Size.width - 1) / 2;
    for (int i = 0; i < filter->Size.height; i++) {
        for (int j = 0; j < filter->Size.width; j++) {
            int i0 = index_x + i - Padding;
            int j0 = index_y + j - Padding;
            if (i0 < 0 || i0 >= input->Size.height || j0 < 0 || j0 >= input->Size.width)
                continue;
            sum += input->GetElement(i0, j0) * filter->GetElement(i, j);
        }
    }
    return sum;
}

__global__ void ConvDirectDist_global(CudaTensor* InputMatrix , CudaTensor* Filter , CudaTensor* OutputMatrix){

    int index_x = blockIdx.x * blockDim.x + threadIdx.x;
    int index_y = blockIdx.y * blockDim.y + threadIdx.y;

    float sum = d_ConvDirectDist(index_x, index_y, InputMatrix, Filter);

    __syncthreads();
      OutputMatrix->SetElement(index_x, index_y, sum);
    __syncthreads();


}

float* CudaConvDirectDist(float* InputMatrix , float* Filter , float* OutputMatrix, Tensorsize I, Tensorsize F,Tensorsize O){
     dim3 gridSize , blockSize;
    if(I.width < BLOCK_SIZE || I.height < BLOCK_SIZE ){
        gridSize = dim3(I.width, I.height, 1);
        blockSize = dim3(1, 1, 1);
    }else{
        gridSize = dim3(I.width / BLOCK_SIZE, I.height / BLOCK_SIZE, 1);
        blockSize = dim3(BLOCK_SIZE, BLOCK_SIZE, 1);
    } 
    CudaTensor* CInputMatrix = new CudaTensor(I.width, I.height , I.depth , InputMatrix);
    CudaTensor* CFilter = new CudaTensor(F.width, F.height , F.depth , Filter);
    CudaTensor* COutputMatrix = new CudaTensor(O.width, O.height , O.depth , OutputMatrix);
    

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    ConvDirectDist_global<<< gridSize , blockSize>>>(CInputMatrix, CFilter, COutputMatrix);

    cudaEventRecord(stop);
    cudaDeviceSynchronize(); 
    cudaGetLastError();
    cudaMemcpy(OutputMatrix, COutputMatrix->Data, O.width * O.height * O.depth * sizeof(double), cudaMemcpyDeviceToHost);

    delete CInputMatrix;
    delete CFilter;
    delete COutputMatrix;

    return OutputMatrix;

}

//__device__ void d_ConvBackDist(CudaTensor& InputMatrix , CudaTensor& Filter , CudaTensor& OutputMatrix){}

//__global__ void ConvBackDist_global(CudaTensor& InputMatrix , CudaTensor& Filter , CudaTensor& OutputMatrix){}

//Tensor CudaConvBackDist(Tensor& InputMatrix , Tensor& Filter, Tensor& OutputMatrix){}