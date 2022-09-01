#include "InitCuda.h"
#include "CudaTensor.h"

__device__ float  d_ConvDirectDist(int index_x, int index_y , CudaTensor *input , CudaTensor* filter);

__global__ void  ConvDirectDist_global(CudaTensor* InputMatrix , CudaTensor* Filter , CudaTensor* OutputMatrix);

DECLSPEC float*  CudaConvDirectDist(float* InputMatrix , float* Filter , float* OutputMatrix, Tensorsize I, Tensorsize F,Tensorsize O);
