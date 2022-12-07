#include "NeuralModules/Conv_Layer.h"

ConvL::ConvL(Tensor_size size, int size_f, int count_f, int depth_f, int Padding) : distribution(0.0, sqrt(2.0 / (size_f * size_f * size.depth))) {
	this->size_f = size_f;
	this->count_f = count_f;
	this->depth_f = size.depth;
	this->Padding = Padding;
	tt = 0;
	Filers = std::vector<Tensor>(count_f, Tensor(size_f, size_f, this->depth_f));
	d_Filers = std::vector<Tensor>(count_f, Tensor(size_f, size_f, this->depth_f));
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
		Bias[c] = 0.01;
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
	SizeO.depth = output.size.depth;
	std::vector<double> allfilter;
	for (int f = 0; f < count_f; f++) 
		for (int i = 0; i < size_f; i++) 
			for (int j = 0; j < size_f; j++)
				for (int c = 0; c < depth_f; c++)
					allfilter.push_back(Filers[f].Tensor_Values[size_f * depth_f * i + j *depth_f + c]);

	CudaConvDirectDist(X.Tensor_Values , allfilter, output.Tensor_Values ,Bias , SizeI , SizeF , SizeO , count_f);



/*	for (int i = 0; i < count_f; i++)
	{
		allfilter.insert(allfilter.end(), Filers[i].Tensor_Values.begin() ,Filers[i].Tensor_Values.end());
	}*/

	/*	// передача данных в выходной тензор
		for (int f = 0; f < count_f; f++) {
			for (int y = 0; y < OutputSize.height; y++) {
				for (int x = 0; x < OutputSize.width; x++) {
					output(y, x , f) = O[y * OutputSize.width + x];
				}
			}
		}
		free(I);
		free(F);
		free(O);*/

/*	for (int f = 0; f < count_f; f++) {

		float* I = (float*)malloc(X.size.height * X.size.width * X.size.depth * sizeof(float));

		float* F = (float*)malloc(Filers[0].size.height * Filers[0].size.width * Filers[0].size.depth * sizeof(float));

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
*/
 //================================================
	/*for (int f = 0; f < count_f; f++) {
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
	}
	tt++;
	std::ofstream fout("test\\filename" + std::to_string(tt) + ".txt");
	std::ofstream fout2("2test\\filenameCUDA" + std::to_string(tt) + ".txt");
	//fout << "depth_f " << std::to_string(Filers[0].size.depth) << "depth " << std::to_string(InputSize.depth)<< "count_f " <<std::to_string(count_f);
	
	for (int f = 0; f < count_f; f++) {
		for (int y = 0; y < OutputSize.height; y++) {
			for (int x = 0; x < OutputSize.width; x++) {
				fout << output(y, x , f) << " ";
				fout2 << output2(y, x , f) << " ";
			}
			fout << std::endl;
			fout2 << std::endl;
		}
		fout << std::endl;
		fout2 << std::endl;
	}
	fout.close();
	fout2.close();*/

/*	for (int f = 0; f < count_f; f++) {
		for (int i = 0; i < size_f; i++) {
			for (int j = 0; j < size_f; j++){
					for (int c = 0; c < depth_f; c++){
				fout << Filers[f](i, j ,c) << " ";
				fout2 << allfilter[f * (size_f * size_f *depth_f) + (size_f * depth_f * i + j *depth_f + c)]<< " ";
				//[count_f * (size_f * size_f *depth_f) + (size_f * depth_f * i + j *depth_f + c)]
			}
			fout << std::endl;
			fout2 << std::endl;
			}
				fout << std::endl;
				fout2 << std::endl;
		}
		fout << std::endl;
		fout2 << std::endl;
	}*/

//	fout.close();
	//fout2.close();

	return output;
}

Tensor ConvL::Back_dist(const Tensor& delta_out, const Tensor& X) {

	Tensor_size size;

	size.height = Step * (OutputSize.height - 1) + 1;
	size.width = Step * (OutputSize.width - 1) + 1;
	size.depth = OutputSize.depth;
	Tensorsize SizeI;
	Tensorsize Sizedel;
	Tensorsize SizeF;
	Tensorsize SizeDx;
	Tensorsize _size;

	SizeI.width = X.size.width;
	SizeI.height = X.size.height;
	SizeI.depth = X.size.depth;

	Sizedel.width = X.size.width;
	Sizedel.height = X.size.height;
	Sizedel.depth = X.size.depth;

	SizeF.width = Filers[0].size.width;
	SizeF.height = Filers[0].size.height;
	SizeF.depth = Filers[0].size.depth;

	SizeDx.width = InputSize.width;
	SizeDx.height = InputSize.height;
	SizeDx.depth = InputSize.depth;

	_size.height = Step * (OutputSize.height - 1) + 1;
	_size.width = Step * (OutputSize.width - 1) + 1;
	_size.depth = OutputSize.depth;

	std::vector<double> allfilter;
	std::vector<double> alld_filter;
	std::vector<double> alld_bias;
	for (int f = 0; f < count_f; f++) {
		for (int i = 0; i < size_f; i++) {
			for (int j = 0; j < size_f; j++){
				for (int c = 0; c < depth_f; c++){
						allfilter.push_back(Filers[f].Tensor_Values[size_f * depth_f * i + j *depth_f + c]);
						alld_filter.push_back(d_Filers[f].Tensor_Values[size_f * depth_f * i + j *depth_f + c]);
				}	
			}
		}
		alld_bias.push_back(d_Bias[f]);
	}

	Tensor deltas(size);
	
	for (int d = 0; d < size.depth; d++)
		for (int i = 0; i < OutputSize.height; i++)
			for (int j = 0; j < OutputSize.width; j++)
				deltas(i * Step, j * Step,d) = delta_out(i, j,d);

	CudaConvCalcDFilters(X.Tensor_Values ,alld_filter, alld_bias , deltas.Tensor_Values ,SizeI,SizeF,_size ,SizeDx ,count_f);


		for (int f = 0; f < count_f; f++) {
		for (int i = 0; i < size_f; i++) {
			for (int j = 0; j < size_f; j++){
				for (int c = 0; c < depth_f; c++){
					d_Filers[f].Tensor_Values[size_f * depth_f * i + j *depth_f + c] = alld_filter[f*(size_f * size_f * depth_f)+(size_f * depth_f * i + j *depth_f + c)]; 
				}	
			}
		}
		d_Bias[f] = alld_bias[f];
	}




// ============================
/*
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

						
						for (int c = 0; c < depth_f; c++){
							int e = f * (size_f * size_f *depth_f) + (size_f * depth_f * i + j *depth_f + c);
							d_Filers[f](i, j,c) += delta * X(i0, j0,c);
						}
					}
				}

				d_Bias[f] += delta; 
			}
		}
	}

	std::ofstream fout("test\\CFfilename" + std::to_string(tt) + ".txt");
	std::ofstream fout2("2test\\CFfilenameCUDA" + std::to_string(tt) + ".txt");
	tt++;

		for (int f = 0; f < count_f; f++) {
						fout <<d_Bias[f]<< " ";
						fout2 <<alld_bias[f]<< " ";
		}
		fout.close();
		fout2.close();

*/
	/*for (int f = 0; f < count_f; f++) {
		for (int c = 0; c < depth_f; c++){
			for (int i = 0; i < size_f; i++) {
				for (int j = 0; j < size_f; j++){
						fout2 << alld_filter[f*(size_f * size_f * depth_f)+(size_f * depth_f * i + j *depth_f + c)]<< " ";
						fout << d_Filers[f].Tensor_Values[size_f * depth_f * i + j *depth_f + c]<< " ";
				}
				fout << std::endl;
				fout2 << std::endl;	
			}
			fout << std::endl;
			fout2 << std::endl;
		}
		fout << std::endl;
		fout2 << std::endl;
	}
	fout.close();
	fout2.close();*/

//+++++++++++++++++
	int pad = size_f - 1 - Padding;
	Tensor dX(InputSize);
	//Tensor dX2(InputSize);
	CudaConvCalcDx(allfilter , dX.Tensor_Values ,deltas.Tensor_Values , SizeF ,Sizedel,SizeDx,_size,count_f);


	/*for (int c = 0; c < depth_f; c++) {
		for (int y = 0; y < InputSize.height; y++) {
			for (int x = 0; x < InputSize.width; x++) {
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

	std::ofstream fout("test\\CBfilename" + std::to_string(tt) + ".txt");
	std::ofstream fout2("2test\\CBfilenameCUDA" + std::to_string(tt) + ".txt");
	tt++;
	for (int f = 0; f < InputSize.depth; f++) {
		for (int y = 0; y < InputSize.height; y++) {
			for (int x = 0; x < InputSize.width; x++) {
				fout << dX(y, x , f) << " ";
				fout2 << dX2(y, x , f) << " ";
			}
			fout << std::endl;
			fout2 << std::endl;
		}
		fout << std::endl;
		fout2 << std::endl;
	}
	fout.close();
	fout2.close();
*/




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