#pragma once

#include <iostream>
#include <fstream>
#include "IOData/Tensor.h"

struct ModuleSetting{
        char* ModuleName;
        char* FuncActivationName;
        char* PoolingType;
        int MatrixSize;
        int ModuleId;
        int Padding;
        int OutputSize;
        int PoolScale;
        int FilterCount;
        int FilterDepth;
        };

struct SettingStruct{
    char* NeuralName;
    char* training_path;
    char*learn_path;
    char* weight_path;
    char* file_name;
    double rate;
    int size_image;
    int cout_typeset;
    int cout_training;
    int cout_module;
};


class Setting {
public:
    std::string NeuralName;
    std::string  Filename;
    std::string  training_path;
    std::string learn_path;
    std::string  weight_path;
    std::string  file_name;
    double rate;
    int cout_typeset;
    int cout_training;
    int cout_module;
    int learn; // -
    Tensor_size SizeImage;
    std::vector<ModuleSetting*> ModuleData;
    
    void CopyStruct(SettingStruct *str);
    void Load_Setting(const char* setting_filename);
    void CopyModuleStruct(ModuleSetting *Mstr);
};