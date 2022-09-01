#include "IOData/Images.h"
#include "CImg/CImg.h"



/*Image::Image(Tensor_size size, std::string filename) :NewImageBits(size) {
	//Palitra = new BYTE(sizeN);
	std::ifstream file(filename, std::ios::binary);
	std::vector<char> Im_buffer((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	ImBuffer = Im_buffer;
	BitmapHeader = PBITMAPFILEHEADER(&ImBuffer[0]);
	BitmapInfo = PBITMAPINFOHEADER(&BitmapHeader[0] + 1);
	ImageFmt = PBITMAPINFO(BitmapInfo);
	ImageBits = ((BYTE*)BitmapHeader) + BitmapHeader->bfOffBits;
	int p = 120;
	int I = 0;
	w_line = (BitmapHeader->bfSize - BitmapHeader->bfOffBits) / BitmapInfo->biHeight;
	newl = BitmapInfo->biWidth * 3;
	for (int i = 27, y = 0; i >= 0; i--) {
		for (int j = 27; j >= 0; j--) {

			I = 0.3 * ImageBits[i * w_line + j * bit + 2] + 0.59 * ImageBits[i * w_line + j * bit + 1] + 0.11 * ImageBits[i * w_line + j * bit];
			NewImageBits(y, j, 0) = I / 255;
		}
		y++;
	}

}

*/

Image::Image(Tensor_size size, const char* filename):NewImageBits(size) {
	cimg_library::CImg<unsigned char> src(filename);
	int width = src.width();
	int height = src.height();
	//std::cout << width << "  " << height << std::endl;
	unsigned char* p = src.data(0, 0);
	//  cout << width << "x" << height << endl;
	for (int h = 0, y = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			//int I = 0.3 * (int)src(c, r, 0, 0) + 0.59 * (int)src(c, r, 0, 1) + 0.11 * (int)src(c, r, 0, 2);
		   //if ((int)src(h, w, 0, 0) < 1) {
		  //      NewImageBits(w, h, 0) = 0.0;
		  //  }

			NewImageBits(h, w, 0) = ((int)p[h * (width * 1) + w * 1 + 1])/255.0;
			//	std::cout << (int)p[h * (width * 1) + w * 1 + 1];//(int)src(h, w);// / 255.0;

		}

	}

}