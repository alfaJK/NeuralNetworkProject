#include "NeuralModules/ActivationLayer.h"
#include <math.h>


ActivationLayer::ActivationLayer(Tensor_size size) {
	this->size = size;
}

Tensor ActivationLayer::Direct_dist(const Tensor& X) {
	Tensor output(size); 

	for (int i = 0; i < size.height; i++)
		for (int j = 0; j < size.width; j++)
			for (int k = 0; k < size.depth; k++)
				output(i, j, k) = X(i, j, k) > 0 ? X(i, j, k) : 0;

	return output; 
}

Tensor ActivationLayer::Back_dist(const Tensor& dout, const Tensor& X) {
	Tensor dX(size); 

	
	for (int i = 0; i < size.height; i++)
		for (int j = 0; j < size.width; j++)
			for (int k = 0; k < size.depth; k++)
				dX(i, j, k) = dout(i, j, k) * (X(i, j, k) > 0 ? 1 : 0); 
	return dX; 
}


