#pragma once
#include "InitCuda.h"
#include "stdio.h"

struct Tensorsize {
	int width;
	int height;
	int depth;
};

class Unified {
public:
    void* operator new(size_t len) {
        void* ptr;
        cudaMallocManaged(&ptr, len);
        return ptr;
    }
    void operator delete(void* ptr) {
        cudaFree(ptr);
    }
    void* operator new[](std::size_t size) {
        void* ptr;
        cudaMallocManaged(&ptr, size);
        return ptr;
    }
        void operator delete[](void* ptr) {
        cudaFree(ptr);
    }
};

class CudaTensor: public Unified {
public:
    double* Data;
    Tensorsize Size;
    thrust::device_vector<double> d_vec;
    __host__  __device__ CudaTensor(int w, int h, int d, std::vector<double> data){
        Size.width = w;
        Size.height = h;
        Size.depth = d;
        d_vec = thrust::device_vector<double> (data);
        Data = thrust::raw_pointer_cast(d_vec.data());
    }
    __host__  __device__ double GetElement(int x,int y, int d){return Data[Size.depth * Size.width * y + x * Size.depth + d];}
    __host__  __device__ double* GetSubMatrix(int x, int y){return &Data[Size.width * BLOCK_SIZE * y + BLOCK_SIZE * x];}
    __host__  __device__ void SetElement(int x, int y, int d , double data){Data[Size.depth * Size.width * y + x * Size.depth + d] = data;}

};

/*

#include "CUDA/CudaTensor.h"

__host__ __device__ double CudaTensor::GetElement(int x, int y) {
    return Data[width * y + x];
}

__host__  __device__ double* CudaTensor::GetSubMatrix(int x, int y) {

   // Matrix *Asub = new Matrix(BLOCK_SIZE, BLOCK_SIZE,&Data[width * BLOCK_SIZE * y + BLOCK_SIZE * x]);
    return &Data[width * BLOCK_SIZE * y + BLOCK_SIZE * x];
}

__host__ __device__ void CudaTensor::SetElement(int x, int y, int data) {
    Data[width * y + x] = data;
}



 */