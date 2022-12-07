#pragma once
#include "IOData/Tensor.h"
#include "CUDA/CudaTensor.h"
#include "CUDA/CudaModuleCalc.h"
#include "Neural_Module.h"
#include <vector>

class PoolingLayer : public NeuralModule {
public:
	int scale;
	Tensor_size InputSize; 
	Tensor_size OutputSize;
	Tensor Mask;
	int tt;
	PoolingLayer(Tensor_size InputSize, int scale);
	Tensor Direct_dist(const Tensor& X) override;
	Tensor Back_dist(const Tensor& delta_out, const Tensor& X) override;
	void InitWeights() override {};
	void UpdateWeights(double learningRate) override {};
	void LoadWeight(const char* filename) override {};
	void UploadWeight(const char* filename) override {};
};