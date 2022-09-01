#include "NeuralModules/Pooling.h"



PoolingLayer::PoolingLayer(Tensor_size InputSize, int scale) : Mask(InputSize) {

	this->InputSize.width = InputSize.width;
	this->InputSize.height = InputSize.height;
	this->InputSize.depth = InputSize.depth;

	OutputSize.width = InputSize.width / scale;
	OutputSize.height = InputSize.height / scale;
	OutputSize.depth = InputSize.depth;

    this->scale = scale;
}
Tensor PoolingLayer::Direct_dist(const Tensor& X) {
    Tensor OutTensor(OutputSize); 



    

    for (int d = 0; d < InputSize.depth; d++) {
        for (int i = 0; i < InputSize.height; i += scale) {
            for (int j = 0; j < InputSize.width; j += scale) {
                int imax = i;
                int jmax = j; 
                double max = X(i, j, d);

               
                for (int y = i; y < i + scale; y++) {
                    for (int x = j; x < j + scale; x++) {
                        double value = X(y, x, d);
                        Mask(y, x, d) = 0;

                        
                        if (value > max) {
                            max = value;
                            imax = y;
                            jmax = x;
                        }
                    }
                }

                OutTensor(i / scale, j / scale, d) = max;
                Mask(imax, jmax, d) = 1;
            }
        }
    }

    return OutTensor; 



}




Tensor PoolingLayer::Back_dist(const Tensor& delta_out, const Tensor& X) {

    Tensor dX(InputSize); 

    for (int d = 0; d < InputSize.depth; d++)
        for (int i = 0; i < InputSize.height; i++)
            for (int j = 0; j < InputSize.width; j++)
               dX(i, j, d) = delta_out(i / scale, j / scale, d) * Mask(i, j, d); 

    return dX; 


}