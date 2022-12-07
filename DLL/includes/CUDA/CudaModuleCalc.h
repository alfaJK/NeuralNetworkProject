#include "InitCuda.h"
#include "CudaTensor.h"

__device__ double  d_ConvDirectDist(int index_x, int index_y ,int index_z ,CudaTensor *input , CudaTensor* filter  ,CudaTensor* CBias);

__global__ void  ConvDirectDist_global(CudaTensor* InputMatrix , CudaTensor* Filter , CudaTensor* OutputMatrix  ,CudaTensor* CBias);

DECLSPEC int CudaConvDirectDist(const std::vector<double> InputMatrix , std::vector<double> Filter , std::vector<double> &OutputMatrix,std::vector<double> Bias , Tensorsize I, Tensorsize F,Tensorsize O , int CountF);


DECLSPEC int CudaConvCalcDFilters(const std::vector<double> InputMatrix ,std::vector<double> &d_Filers , std::vector<double> &d_Bias ,std::vector<double> deltas , Tensorsize I, Tensorsize dF, Tensorsize _size , Tensorsize InputSize, int CountF);

__global__ void  ConvCalcDeltaFilters_global(CudaTensor* InputMatrix , CudaTensor* d_Filers , CudaTensor* d_Bias, CudaTensor* deltas , Tensorsize Isize , int CountF);

DECLSPEC int CudaConvCalcDx(std::vector<double> Filter , std::vector<double> &dX,std::vector<double> deltas , Tensorsize F, Tensorsize del,Tensorsize Sdx ,Tensorsize _size, int CountF);

__global__ void  ConvCalcDx_global(CudaTensor* Filter , CudaTensor* dX , CudaTensor* deltas,Tensorsize _size ,int CountF);


DECLSPEC int CudaActivationDirectDist(std::vector<double> InputMatrix , std::vector<double> &OutputMatrix , Tensorsize I ,Tensorsize O);

__global__ void  ActivationDirectDist_global(CudaTensor* InputMatrix , CudaTensor* OutputMatrix);


DECLSPEC int CudaActivationBackDist(std::vector<double> InputMatrix , std::vector<double> DeltaMatrix , std::vector<double> &OutputMatrix , Tensorsize I , Tensorsize D ,Tensorsize O);

__global__ void  ActivationBackDist_global(CudaTensor* InputMatrix , CudaTensor* DeltaMatrix , CudaTensor* OutputMatrix);


DECLSPEC int CudaPoolingDirectDist(const std::vector<double> InputMatrix , std::vector<double> &Filter , std::vector<double> &OutputMatrix, Tensorsize I, Tensorsize F,Tensorsize O, int scale);

__global__ void  PoolingDirectDist_global(CudaTensor* InputMatrix ,CudaTensor* Filter , CudaTensor* OutputMatrix , int scale);


DECLSPEC int CudaPoolingBackDist(std::vector<double> deltas , std::vector<double> Filter , std::vector<double> &dX , Tensorsize del, Tensorsize F,Tensorsize Sdx, int scale);

__global__ void  PoolingBackDist_global(CudaTensor* DeltaMatrix , CudaTensor* FilterMatrix , CudaTensor* dXMatrix, int scale);
