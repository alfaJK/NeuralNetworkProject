#pragma once
#include <vector>
#include <iostream>
struct Tensor_size {
	int width;
	int height;
	int depth;
};


class Tensor {
public:
	Tensor_size size;
	std::vector<double> Tensor_Values;
	Tensor(const Tensor& tensorCOPY);
	Tensor(int w , int h , int d);
	Tensor(Tensor_size size);
	~Tensor();
	double& operator()(int h_i, int w_j, int d);
	double operator()(int h_i, int w_j, int d) const;
	friend std::ostream& operator<<(std::ostream& os, const Tensor& tensor);
};
