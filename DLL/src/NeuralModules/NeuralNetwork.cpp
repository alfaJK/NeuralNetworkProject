#include "NeuralModules/NeuralNetwork.h"
#include <math.h>

FullyConnectedLayer::FullyConnectedLayer(Tensor_size size, int outputs, const std::string& activationType) : Weight(outputs, size.height* size.width* size.depth), d_Weight(outputs, size.height* size.width* size.depth), df(1, 1, outputs), distribution(0.0, sqrt(2.0 / (size.height * size.width * size.depth))) {
    // ���������� ������� ������
    inputSize.width = size.width;
    inputSize.height = size.height;
    inputSize.depth = size.depth;

    // ��������� �������� ������
    outputSize.width = 1;
    outputSize.height = 1;
    outputSize.depth = outputs;

    this->inputs = size.height * size.width * size.depth; // ���������� ����� ������� ��������
    this->outputs = outputs; // ���������� ����� �������� ��������

    this->activationType = GetActivationType(activationType); // �������� ������������� �������

    Bias = std::vector<double>(outputs); // ������ ������ ��������
    d_Bias = std::vector<double>(outputs); // ������ ������ ���������� �� ����� ��������

    InitWeights(); // �������������� ������� ������������
}

FullyConnectedLayer::ActivationType FullyConnectedLayer::GetActivationType(const std::string& activationType) const {
    if (activationType == "sigmoid")
        return ActivationType::Sigmoid;

    if (activationType == "tanh")
        return ActivationType::Tanh;

    if (activationType == "relu")
        return ActivationType::ReLU;

    if (activationType == "leakyrelu")
        return ActivationType::LeakyReLU;

    if (activationType == "none" || activationType == "")
        return ActivationType::None;

}


void FullyConnectedLayer::UploadWeight(const char* filename) {

    std::ofstream fout(filename);
  
    for (int i = 0; i < outputs; i++) {
        for (int j = 0; j < inputs; j++){
            fout << Weight(i, j) << " ";
        }
        fout << std::endl;
    }
    for (int i = 0; i < outputs; i++) {
    
        fout << Bias[i] << " ";
    }

    fout.close();

}

void FullyConnectedLayer::LoadWeight(const char* filename) {
    std::ifstream fout(filename);

    for (int i = 0; i < outputs; i++) {
        for (int j = 0; j < inputs; j++) {
            fout >> Weight(i, j) ;
        }
     
    }
    for (int i = 0; i < outputs; i++) {

        fout >> Bias[i];
    }

    fout.close();
}



void FullyConnectedLayer::InitWeights() {
    for (int i = 0; i < outputs; i++) {
        for (int j = 0; j < inputs; j++)
            Weight(i, j) = distribution(generator);

        Bias[i] = 0.01; 
    }
}

void FullyConnectedLayer::Activate(Tensor& output) {
    if (activationType == ActivationType::None) {
        for (int i = 0; i < outputs; i++) {
            df(0, 0, i) = 1;
        }
    }
    else if (activationType == ActivationType::Sigmoid) {
        for (int i = 0; i < outputs; i++) {
            output(0, 0, i) = 1 / (1 + exp(-output(0, 0, i)));
            df(0,0,i) = output(0, 0, i) * (1 - output(0, 0, i));
        }
    }
    else if (activationType == ActivationType::Tanh) {
        for (int i = 0; i < outputs; i++) {
            output(0, 0, i) = tanh(output(0, 0, i));
            df(0, 0, i) = 1 - output(0, 0, i) * output(0, 0, i);
        }
    }
    else if (activationType == ActivationType::ReLU) {
        for (int i = 0; i < outputs; i++) {
            if (output(0, 0, i) > 0) {
                df(0, 0, i) = 1;
            }
            else {
                output(0, 0, i) = 0;
                df(0, 0, i) = 0;
            }
        }
    }
    else if (activationType == ActivationType::LeakyReLU) {
        for (int i = 0; i < outputs; i++) {
            if (output(0, 0, i) > 0) {
                df(0, 0, i) = 1;
            }
            else {
                output(0, 0, i) *= 0.01;
                df(0, 0, i) = 0.01;
            }
        }
    }

}

Tensor FullyConnectedLayer::Direct_dist(const Tensor& X) {
    Tensor output(outputSize); 

    for (int i = 0; i < outputs; i++) {
        double sum = Bias[i]; 

        for (int y = 0; y < inputSize.height; y++) {
            for (int x = 0; x < inputSize.width; x++) {
                for (int d = 0; d < inputSize.depth; d++) {
                    sum += Weight(i, y * inputSize.depth * inputSize.width + x * inputSize.depth + d) * X(y, x, d);
                }
            }

        }
        output(0, 0, i) = sum;
    }

    Activate(output); 

    return output; 
}

Tensor FullyConnectedLayer::Back_dist(const Tensor& dout, const Tensor& X) {
   
    for (int i = 0; i < outputs; i++)
        df(0, 0, i) *= dout(0,0,i);
    for (int i = 0; i < outputs; i++) {
        for (int j = 0; j < inputs; j++)
            d_Weight(i, j) = df(0, 0, i) * X(0, 0, i);

        d_Bias[i] = df(0,0,i);
       
    }

    Tensor dX(inputSize);

   
    for (int j = 0; j < inputs; j++) {
        double sum = 0;

        for (int i = 0; i < outputs; i++)
            sum += Weight(i, j) * df(0, 0, i);

        dX(0, 0, j) = sum; 
    }

    return dX; 
}

void FullyConnectedLayer::UpdateWeights(double learningRate) {
    for (int i = 0; i < outputs; i++) {
        for (int j = 0; j < inputs; j++)
            Weight(i, j) -= learningRate * d_Weight(i, j);

        Bias[i] += learningRate * d_Bias[i];
    }
}