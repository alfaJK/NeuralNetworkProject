#pragma once
#include "IOData/Tensor.h"
#include "NeuralModules/Neural_Module.h"

class ActivationLayer : public NeuralModule {
public:
    Tensor_size size;
	ActivationLayer(Tensor_size size);
    Tensor Direct_dist(const Tensor& X) override;
    Tensor Back_dist(const Tensor& dout, const Tensor& X) override;
	void InitWeights() override {};
	void UpdateWeights(double learningRate) override {};
	void LoadWeight(const char* filename) override {};
	void UploadWeight(const char* filename) override {};
};
