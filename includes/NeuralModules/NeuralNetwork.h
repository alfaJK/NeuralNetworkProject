#pragma once
#include <vector>
#include <random>
#include "IOData/Tensor.h"
#include "IOData/Matrix.h"
#include "NeuralModules/Neural_Module.h"
#include "iostream"
#include <fstream>

class FullyConnectedLayer : public NeuralModule {
    enum class ActivationType {
        None, 
        Sigmoid, 
        Tanh,
        ReLU,
        LeakyReLU, 
        ELU 
    };
    Tensor_size inputSize; 
    Tensor_size outputSize;

    std::default_random_engine generator;
    std::normal_distribution<double> distribution;

    int inputs; 
    int outputs;

    ActivationType activationType;
    Tensor df; 

    Matrix Weight;
    Matrix d_Weight;

    std::vector<double> Bias; 
    std::vector<double> d_Bias; 

    ActivationType GetActivationType(const std::string& activationType) const;

    void InitWeights() override;
    void Activate(Tensor& output);

public:
    FullyConnectedLayer(Tensor_size size, int outputs, const std::string& activationType = "none");

    Tensor Direct_dist(const Tensor& X) override;
    Tensor Back_dist(const Tensor& dout, const Tensor& X) override;
    void UpdateWeights(double learningRate) override;
    void LoadWeight(const char* filename) override;
    void UploadWeight(const char* filename) override;
};











