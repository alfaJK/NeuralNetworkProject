#include "NeuralModules/Conv_Layer.h"

ConvL::ConvL(Tensor_size size, int size_f, int count_f, int depth_f, int Padding) : distribution(0.0, sqrt(2.0 / (size_f * size_f * size.depth))) {
	this->size_f = size_f;
	this->count_f = count_f;
	this->depth_f = depth_f;
	this->Padding = Padding;
	Filers = std::vector<Tensor>(count_f, Tensor(size_f, size_f, depth_f));
	d_Filers = std::vector<Tensor>(count_f, Tensor(size_f, size_f, depth_f));
	Bias = std::vector<double>(count_f, 0);
	d_Bias = std::vector<double>(count_f, 0);
	InputSize.width = size.width;
	InputSize.height = size.height;
	InputSize.depth = size.depth;

	OutputSize.width = ((size.width - size_f + 2 * Padding) / Step)+ 1;
	OutputSize.height = ((size.height - size_f + 2 * Padding) / Step) + 1;
	OutputSize.depth = count_f;

	InitWeights();


}


void ConvL::UploadWeight(const char* filename) {

	std::ofstream fout(filename);
	for (int c = 0; c < count_f; c++) {
		for (int i = 0; i < size_f; i++) {
			for (int j = 0; j < size_f; j++) {
				for (int d = 0; d < depth_f; d++) {

					fout<< Filers[c](i, j, d) << " ";

				}
				
			}
			fout << std::endl;
		}
		//fout << std::endl;
	}


	for (int c = 0; c < count_f; c++) {
			fout << Bias[c] << " ";
	}
	fout.close();

}

void ConvL::LoadWeight(const char* filename) {
	std::ifstream fout(filename);
	for (int c = 0; c < count_f; c++) {
		for (int i = 0; i < size_f; i++) {
			for (int j = 0; j < size_f; j++) {
				for (int d = 0; d < depth_f; d++) {
					fout >> Filers[c](i, j, d);
				}
			}
		}
	}
	for (int c = 0; c < count_f; c++) 
		fout >> Bias[c] ;
	fout.close();
}

void ConvL::InitWeights() {
	for (int c = 0; c < count_f; c++) {
		for (int i = 0; i < size_f; i++) {
			for (int j = 0; j < size_f; j++) {
				for (int d = 0; d < depth_f; d++) {
				
					Filers[c](i, j, d) = distribution(generator);

				}
			}
		}
		Bias[c] = 0.1;
	}
}

Tensor ConvL::Direct_dist(const Tensor& X) {

	Tensor output(OutputSize); 

	Tensorsize SizeI;
	Tensorsize SizeF;
	Tensorsize SizeO;

	SizeI.width = X.size.width;
	SizeI.height = X.size.height;
	SizeI.depth = X.size.depth;

	SizeF.width = Filers[0].size.width;
	SizeF.height = Filers[0].size.height;
	SizeF.depth = Filers[0].size.depth;

	SizeO.width = output.size.width;
	SizeO.height = output.size.height;
	SizeO.depth = 1;

	for (int f = 0; f < count_f; f++) {

		float* I = (float*)malloc(X.size.height * X.size.width * X.size.depth * sizeof(float));
		float* F = (float*)malloc(Filers[f].size.height * Filers[f].size.width * Filers[f].size.depth * sizeof(float));
		float* O = (float*)malloc(output.size.height * output.size.width * sizeof(float));

		for(int i=0; i<X.size.height * X.size.width * X.size.depth; i++ ) I[i] = X.Tensor_Values[i];

		for(int i=0; i<Filers[f].size.height * Filers[f].size.width * Filers[f].size.depth; i++ ) F[i] = Filers[f].Tensor_Values[i];

		for(int i=0; i<output.size.height * output.size.width; i++ ) O[i] = 0;

		CudaConvDirectDist(I , F, O , SizeI , SizeF , SizeO);

		for (int y = 0; y < OutputSize.height; y++) {
			for (int x = 0; x < OutputSize.width; x++) {
				output(y, x , f) = O[y * OutputSize.width + x];
			}
		}
		
		free(I);
		free(F);
		free(O);
	}
	
/*	for (int f = 0; f < count_f; f++) {
		for (int y = 0; y < OutputSize.height; y++) {
			for (int x = 0; x < OutputSize.width; x++) {
				double sum = Bias[f]; 
				for (int i = 0; i < size_f; i++) {
					for (int j = 0; j < size_f; j++) {
						int i0 = Step * y + i - Padding;
						int j0 = Step * x + j - Padding;
						if (i0 < 0 || i0 >= InputSize.height || j0 < 0 || j0 >= InputSize.width)
							continue;
						for (int c = 0; c < depth_f; c++)
							sum += X(i0, j0 , c) * Filers[f](i, j ,c);
					}
				}
				output(y, x , f) = sum; 
			}
		}
	}*/
	return output;
}

Tensor ConvL::Back_dist(const Tensor& delta_out, const Tensor& X) {

	Tensor_size size;

	size.height = Step * (OutputSize.height - 1) + 1;
	size.width = Step * (OutputSize.width - 1) + 1;
	size.depth = OutputSize.depth;

	Tensor deltas(size);

	for (int d = 0; d < size.depth; d++)
		for (int i = 0; i < OutputSize.height; i++)
			for (int j = 0; j < OutputSize.width; j++)
				deltas(i * Step, j * Step,d) = delta_out(i, j,d);

	
	for (int f = 0; f < count_f; f++) {
		for (int y = 0; y < size.height; y++) {
			for (int x = 0; x < size.width; x++) {
				double delta = deltas(y, x,f);

				for (int i = 0; i < size_f; i++) {
					for (int j = 0; j < size_f; j++) {
						int i0 = i + y - Padding;
						int j0 = j + x - Padding;

						
						if (i0 < 0 || i0 >= InputSize.height || j0 < 0 || j0 >= InputSize.width)
							continue;

						
						for (int c = 0; c < depth_f; c++)
							d_Filers[f](i, j,c) += delta * X(i0, j0,c);
					}
				}

				d_Bias[f] += delta; 
			}
		}
	}

	int pad = size_f - 1 - Padding;
	Tensor dX(InputSize);

	
	for (int y = 0; y < InputSize.height; y++) {
		for (int x = 0; x < InputSize.width; x++) {
			for (int c = 0; c < depth_f; c++) {
				double sum = 0;

				
				for (int i = 0; i < size_f; i++) {
					for (int j = 0; j < size_f; j++) {
						int i0 = y + i - pad;
						int j0 = x + j - pad;

						
						if (i0 < 0 || i0 >= size.height || j0 < 0 || j0 >= size.width)
							continue;

						
						for (int f = 0; f < count_f; f++)
							sum += Filers[f](size_f - 1 - i, size_f - 1 - j, c) * deltas(i0, j0, f); 
					}
				}

				dX(y, x, c) = sum; 
			}
		}
	}

	return dX;


}

void ConvL::UpdateWeights(double LearnRate) {


	for (int c = 0; c < count_f; c++) {
		for (int y = 0; y < size_f; y++) {

			for (int x = 0; x < size_f; x++) {


				for (int d = 0; d < depth_f; d++) {
					Filers[c](y, x, d) -= LearnRate * d_Filers[c](y, x, d);
					d_Filers[c](y, x, d) = 0;
				}
			}

		}
		Bias[c] -= LearnRate * d_Bias[c];
		d_Bias[c] = 0;

	}


}