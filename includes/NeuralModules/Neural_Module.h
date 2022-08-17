#pragma once

#include "IOData/Tensor.h"

class NeuralModule {
public:
	virtual void InitWeights() = 0;
	virtual Tensor Direct_dist(const Tensor&) = 0;
	virtual Tensor Back_dist(const Tensor& ,const Tensor& ) = 0;
	virtual void UpdateWeights(double) = 0;
	virtual void UploadWeight(const char*) = 0;
	virtual void LoadWeight(const char*) = 0;
};