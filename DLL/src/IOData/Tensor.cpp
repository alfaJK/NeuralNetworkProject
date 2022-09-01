#include "IOData/Tensor.h"

Tensor::Tensor(int w, int h, int d) {
	size.width = w;//������ �������
	size.height = h;//������ �������
	size.depth = d; //������� �������
	Tensor_Values = std::vector<double>(w * h * d, 0);
}
Tensor::~Tensor() {
	Tensor_Values.clear();
}
Tensor::Tensor(const Tensor& tensorCOPY) {
	this->size.width = tensorCOPY.size.width;
	this->size.height = tensorCOPY.size.height;
	this->size.depth = tensorCOPY.size.depth;
	Tensor_Values = std::vector<double>(size.width * size.height * size.depth, 0);
	for (int y = 0; y < tensorCOPY.size.height; y++) {
		for (int x = 0; x < tensorCOPY.size.width; x++) {
			for (int d = 0; d < tensorCOPY.size.depth; d++) {
				this->Tensor_Values[y * size.depth * size.width + x * size.depth + d] = tensorCOPY(y,x,d);
			}

		}

	}
}

Tensor::Tensor(Tensor_size size) {
	this->size.width = size.width;
	this->size.height = size.height;
	this->size.depth = size.depth;
	Tensor_Values = std::vector<double>(size.width * size.height * size.depth, 0);
}

double& Tensor::operator()(int h_i, int w_j, int d) {
	return Tensor_Values[h_i * size.depth * size.width + w_j * size.depth + d];
}

double Tensor::operator()(int h_i, int w_j, int d) const {
	return Tensor_Values[h_i * size.depth * size.width + w_j * size.depth + d];
}

std::ostream& operator<<(std::ostream& os, const Tensor& tensor) {
	for (int d = 0; d < tensor.size.depth; d++) {
		for (int i = 0; i < tensor.size.height; i++) {
			for (int j = 0; j < tensor.size.width; j++)
				os << tensor.Tensor_Values[i * (tensor.size.depth * tensor.size.width) + j * tensor.size.depth + d] << " ";
	
		//	os << std::endl;
		}

	//	os << std::endl;
	}
	os << std::endl;
	return os;
}