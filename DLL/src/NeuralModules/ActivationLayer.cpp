#include "NeuralModules/ActivationLayer.h"
#include <math.h>
#include <fstream>

ActivationLayer::ActivationLayer(Tensor_size size) {
	this->size = size;
	tt= 0;
}

Tensor ActivationLayer::Direct_dist(const Tensor& X) {
	Tensor output(size); 
	Tensorsize SizeI;
	Tensorsize SizeO;

	SizeI.width = X.size.width;
	SizeI.height = X.size.height;
	SizeI.depth = X.size.depth;

	SizeO.width = size.width;
	SizeO.height = size.height;
	SizeO.depth = size.depth;


	CudaActivationDirectDist(X.Tensor_Values , output.Tensor_Values , SizeI , SizeO);


	/*for (int i = 0; i < size.height; i++)
		for (int j = 0; j < size.width; j++)
			for (int k = 0; k < size.depth; k++)
				output(i, j, k) = X(i, j, k) > 0 ? X(i, j, k) : 0;

	std::ofstream fout("test\\Afilename" + std::to_string(tt) + ".txt");
	std::ofstream fout2("2test\\AfilenameCUDA" + std::to_string(tt) + ".txt");
	tt++;
	for (int f = 0; f < size.depth; f++) {
		for (int y = 0; y < size.height; y++) {
			for (int x = 0; x < size.width; x++) {
				fout << output(y, x , f) << " ";
				fout2 << output2(y, x , f) << " ";
			}
			fout << std::endl;
			fout2 << std::endl;
		}
		fout << std::endl;
		fout2 << std::endl;
	}*/

	return output; 
}

Tensor ActivationLayer::Back_dist(const Tensor& dout, const Tensor& X) {
	Tensor dX(size); 
	Tensorsize SizeI;
	Tensorsize SizeD;
	Tensorsize SizeO;

	SizeI.width = X.size.width;
	SizeI.height = X.size.height;
	SizeI.depth = X.size.depth;

	SizeD.width = dout.size.width;
	SizeD.height = dout.size.height;
	SizeD.depth = dout.size.depth;

	SizeO.width = size.width;
	SizeO.height = size.height;
	SizeO.depth = size.depth;

	CudaActivationBackDist(X.Tensor_Values , dout.Tensor_Values , dX.Tensor_Values ,SizeI , SizeD ,SizeO );
	/*for (int i = 0; i < size.height; i++)
		for (int j = 0; j < size.width; j++)
			for (int k = 0; k < size.depth; k++)
				dX(i, j, k) = dout(i, j, k) * (X(i, j, k) > 0 ? 1 : 0); */
	return dX; 
}


