#pragma once

#include <list>
#include <vector>
#include<wincodec.h>
#include <fstream>
#include <math.h>
#include "IOData/Tensor.h"

/*
class Image {
public:
	const int sizeN = 4356;
	int I = 0;
	std::string Path;
	PBITMAPFILEHEADER BitmapHeader;
	PBITMAPINFOHEADER BitmapInfo;
	PBITMAPINFO ImageFmt;
	std::vector<char> ImBuffer;
	BYTE* ImageBits;
	Tensor NewImageBits;
	HBITMAP hBmp;
	tagBITMAP BitMap;
	int bit = 3;
	unsigned int w_line, newl;
public:
	Image(Tensor_size size, std::string);
	void ShowImage(HDC hdc, int pos_x, int pos_y);
	void Binaryzation(int value);
	void Enlight(int value);
	void Negative(int value);
	void SaveImage();
	void Canel();
};


*/

class Image {
public:
	Tensor NewImageBits;
public:
	Image(Tensor_size size , const char* filename);
	Image(std::string filename, Tensor *InputBits );
	void CreateImage(std::string filename);
};