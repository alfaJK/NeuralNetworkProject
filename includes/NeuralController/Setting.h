#pragma once

#include <iostream>
#include <fstream>
#include "IOData/Tensor.h"

class Setting {
public:
    int cout_learn;
    int cout_training;
    int count_TypeSet;
    int learn;
    int filter_size;
    int Padding;
    int Count_Module;
    std::vector<int> Module;
    double rate;
    std::string path;
    std::string f_activation;
    std::string name;
    std::string training_path; 
    std::string data_path;
    std::string weight_path;
    Tensor_size SizeImage;
    void Load_Setting(const char* setting_filename);
};