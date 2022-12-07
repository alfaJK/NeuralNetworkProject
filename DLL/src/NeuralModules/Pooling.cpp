#include "NeuralModules/Pooling.h"
#include <fstream>


PoolingLayer::PoolingLayer(Tensor_size InputSize, int scale) : Mask(InputSize) {
    tt =0;
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
   // Tensor OutTensor2(OutputSize); 
    Tensorsize SizeI;
    Tensorsize SizeF;
	Tensorsize SizeO;

	SizeI.width = X.size.width;
	SizeI.height = X.size.height;
	SizeI.depth = X.size.depth;

    SizeF.width = Mask.size.width;
	SizeF.height = Mask.size.height;
	SizeF.depth = Mask.size.depth;

	SizeO.width = OutputSize.width;
	SizeO.height = OutputSize.height;
	SizeO.depth = OutputSize.depth;

  /*  std::vector<double> allmask;
		for (int i = 0; i < InputSize.width; i++) 
			for (int j = 0; j < InputSize.height; j++)
				for (int c = 0; c < InputSize.depth; c++)
					allmask.push_back(Mask.Tensor_Values[InputSize.width * InputSize.depth * i + j *InputSize.depth + c]);*/
    // МОЖЕТ БЫТЬ ПРОБЛЕМА В MASK
    CudaPoolingDirectDist(X.Tensor_Values, Mask.Tensor_Values, OutTensor.Tensor_Values, SizeI, SizeF, SizeO, scale );
    

   /*for (int d = 0; d < InputSize.depth; d++) {
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

    std::ofstream fout("test\\Pfilename" + std::to_string(tt) + ".txt");
	std::ofstream fout2("2test\\PfilenameCUDA" + std::to_string(tt) + ".txt");
	tt++;
	for (int i = 0; i < InputSize.width; i++){ 
			for (int j = 0; j < InputSize.height; j++){
				for (int c = 0; c < InputSize.depth; c++){
				fout << Mask(i, j , c) << " ";
				fout2 << allmask[InputSize.width * InputSize.depth * i + j *InputSize.depth + c] << " ";
			}
			fout << std::endl;
			fout2 << std::endl;
		}
		fout << std::endl;
		fout2 << std::endl;
	}
    fout.close();
	fout2.close();*/


    return OutTensor; 



}




Tensor PoolingLayer::Back_dist(const Tensor& delta_out, const Tensor& X) {

    Tensor dX(InputSize); 

    Tensorsize SizeDel;
    Tensorsize SizeF;
	Tensorsize SizeDx;

	SizeDel.width = delta_out.size.width;
	SizeDel.height = delta_out.size.height;
	SizeDel.depth = delta_out.size.depth;

    SizeF.width = Mask.size.width;
	SizeF.height = Mask.size.height;
	SizeF.depth = Mask.size.depth;

	SizeDx.width = InputSize.width;
	SizeDx.height = InputSize.height;
	SizeDx.depth = InputSize.depth;

    CudaPoolingBackDist(delta_out.Tensor_Values , Mask.Tensor_Values , dX.Tensor_Values , SizeDel,SizeF,SizeDx, scale);



    /*for (int d = 0; d < InputSize.depth; d++)
        for (int i = 0; i < InputSize.height; i++)
            for (int j = 0; j < InputSize.width; j++)
               dX(i, j, d) = delta_out(i / scale, j / scale, d) * Mask(i, j, d); */

    return dX; 


}