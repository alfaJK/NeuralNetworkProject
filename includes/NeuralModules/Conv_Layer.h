#pragma once
#include "IOData/Tensor.h"
#include "NeuralModules/Neural_Module.h"
#include <random>
#include <fstream>
#include "CUDA/CudaTensor.h"
#include "CUDA/CudaModuleCalc.h"

class ConvL : public NeuralModule{
private:
	std::default_random_engine generator;
	std::normal_distribution<double> distribution;

	std::vector<Tensor> Filers;
	std::vector<Tensor> d_Filers;

	Tensor_size InputSize;
	Tensor_size OutputSize;

	std::vector<double> Bias; 
	std::vector<double> d_Bias;
	int size_f; 
	int count_f;  
	int depth_f; 
	int Padding; 
	int Step = 1;
public:
	ConvL(Tensor_size size ,int size_f , int count_f, int depth_f, int Padding);
	void InitWeights() override;
	Tensor Direct_dist(const Tensor &X) override;
	Tensor Back_dist(const Tensor& delta_out ,const Tensor& X) override;
	void UpdateWeights(double LearnRate) override;
	void LoadWeight(const char* filename) override;
	void UploadWeight(const char* filename) override;

};


/*void SetWeight(int index, int i, int j, int k, double weight) {
		Filers[index](i, j, k) = weight;
	}
	void SetBias(int index, double bias) {
		Bias[index] = bias;
	}*/