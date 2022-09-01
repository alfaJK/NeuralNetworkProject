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
    float* Data;
    Tensorsize Size;
    int height;
    int width;
    __host__  __device__ CudaTensor(int w, int h, int d, float* data){
        Size.width = h;
        Size.height = w;
        Size.depth = d;
        cudaMallocManaged(&Data, Size.width * Size.height *  Size.depth * sizeof(float));
        cudaMemcpy(Data, data, Size.width * Size.height * Size.depth * sizeof(float), cudaMemcpyHostToDevice);
    }
    __host__  __device__ float GetElement(int x,int y){return Data[width * y + x];}
    __host__  __device__ float* GetSubMatrix(int x, int y){return &Data[width * BLOCK_SIZE * y + BLOCK_SIZE * x];}
    __host__  __device__ void SetElement(int x, int y,int data){Data[width * y + x] = data;}
   // __host__  __device__ void PrintMatrix();
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