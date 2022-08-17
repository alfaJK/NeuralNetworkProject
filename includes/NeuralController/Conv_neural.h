#pragma once
#include <iostream>
#include <math.h>
#include <sstream>
#include <vector>
#include "IOData/Tensor.h"
#include "IOData/Images.h"
#include "NeuralModules/Conv_Layer.h"
#include "NeuralModules/ActivationLayer.h"
#include "NeuralModules/Pooling.h"
#include "NeuralModules/NeuralNetwork.h"
#include "NeuralController/Setting.h"

//#include "../../DLL/NeuralLib/NeuralLib/pch.h"


class Neural {
public:
	std::vector <NeuralModule*> Module;
	std::vector <Tensor*> ModuleResult;
	Setting* Neural_setting;
	Image* InitImage;
	int t;
	int Metrics[3];
	const char* setting_filename;
public:
	Neural(const char* setting_filename);
	~Neural();
	void Start();
	void Learn();
	void setcur(int x, int y);
	void LoadBar(int size, int count, int progress);
	void Active();
};

