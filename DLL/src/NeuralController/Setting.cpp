#include "NeuralController/Setting.h"
#include <iostream>
#include <iomanip>
#include <vector>

void Setting::Load_Setting(const char* setting_filename) {
  /*  std::ifstream fout(setting_filename);
    char Str[256];

    char k;
    while (fout >> k) {
        if (k == 60) {
            for (int i = 0; i < 256; i++) {
                if (Str[i - 1] == '>') { Str[i - 1] = '\0'; break; }
                fout >> Str[i];
            }
            path = Str;

            break;
        }
    }
    while (fout >> k) {
        if (k == 60) {
            for (int i = 0; i < 256; i++) {
                if (Str[i - 1] == '>') { Str[i - 1] = '\0'; break; }
                fout >> Str[i];
            }
            training_path = Str;

            break;
        }
    }
    while (fout >> k) {
        if (k == 60) {
            for (int i = 0; i < 256; i++) {
                if (Str[i - 1] == '>') { Str[i - 1] = '\0'; break; }
                fout >> Str[i];
            }
            data_path = Str;

            break;
        }
    }
    while (fout >> k) {
        if (k == 60) {
            for (int i = 0; i < 256; i++) {
                if (Str[i - 1] == '>') { Str[i - 1] = '\0'; break; }
                fout >> Str[i];
            }
            weight_path = Str;

            break;
        }
    }
    while (fout >> k) {
        if (k == 60) {
            for (int i = 0; i < 256; i++) {
                if (Str[i - 1] == '>') { Str[i - 1] = '\0'; break; }
                fout >> Str[i];
            }
            name = Str;
            break;
        }
    }
    while (fout >> k) {
        if (k == 60) {
            for (int i = 0; i < 256; i++) {
                if (Str[i - 1] == '>') { Str[i - 1] = '\0'; break; }
                fout >> Str[i];
            }
            f_activation = Str;
            break;
        }
    }

    while (fout >> k) {
        if (k == 60) fout >> learn;
        if (k == '>')break;
    }
    while (fout >> k) {
        if (k == 60) fout >> count_TypeSet;
        if (k == '>')break;
    }
    while (fout >> k) {
        if (k == 60) fout >> cout_learn;
        if (k == '>')break;
    }
    while (fout >> k) {
        if (k == 60) fout >> cout_training;
        if (k == '>')break;
    }
    while (fout >> k) {
        if (k == 60) fout >> rate;
        if (k == '>')break;
    }
    while (fout >> k) {
        if (k == 60) fout >> filter_size;
        if (k == '>')break;
    }
    while (fout >> k) {
        if (k == 60) fout >> SizeImage.width;
        if (k == '>')break;
    }
    while (fout >> k) {
        if (k == 60) fout >> SizeImage.height;
        if (k == '>')break;
    }
    while (fout >> k) {
        if (k == 60) fout >> SizeImage.depth;
        if (k == '>')break;
    }
    while (fout >> k) {
        if (k == 60) fout >> Padding;
        if (k == '>')break;
    }
    while (fout >> k) {
        if (k == 60) fout >> Count_Module;
        if (k == '>')break;
    }
    Module.resize(Count_Module);
    int j = 0;
    memset(Str, 0, sizeof(Str));
    while (fout >> k) {
        if (k == 60) {
            for (int i = 0; i < 256; i++) {
                 fout >> Str[i];
                if (Str[i] == ',') {
                    Str[i] = '\0';
                    //for (int f = 0; f < i; f++)  
                     //   std::cout<<Str[f];
                   // std::cout << std::endl;
                 
                    if (strcmp(Str, "Conv_L") == 0) { Module[j] = 0; j++;   for (int f = 0; f < i; f++)  Str[f] = '\0'; i = -1; continue; }
                    if (strcmp(Str, "Active_L") == 0) { Module[j] = 1; j++;  for (int f = 0; f < i; f++)  Str[f] = '\0'; i = -1; continue; }
                    if (strcmp(Str, "Pool_L") == 0) { Module[j] = 2; j++;   for (int f = 0; f < i; f++)  Str[f] = '\0'; i = -1;  continue; }
                    if (strcmp(Str, "FC_L") == 0) { Module[j] = 3; j++;  for (int f = 0; f < i; f++)  Str[f] = '\0'; i = -1;  continue; }
                }
                if (Str[i] == '>') { Str[i] = '\0'; break; }
            }
            break;
        }
    }

    fout.close();*/

}

void Setting::CopyStruct(SettingStruct *str){
    NeuralName = (*str).NeuralName;
    training_path = (*str).training_path;
    learn_path = (*str).learn_path;
    weight_path = (*str).weight_path;
    file_name = (*str).file_name;
    rate = (*str).rate;
    cout_training = (*str).cout_training;
    cout_typeset = (*str).cout_typeset;
    cout_module = (*str).cout_module;
    SizeImage.height = (*str).size_image;
    SizeImage.width = (*str).size_image;
    SizeImage.depth = 1;
    learn = 1;
}

void Setting::CopyModuleStruct(ModuleSetting *Mstr){
    ModuleSetting *MS = new ModuleSetting;
    (*MS).FuncActivationName =  (*Mstr).FuncActivationName;
    (*MS).PoolingType =  (*Mstr).PoolingType;
    (*MS).MatrixSize = (*Mstr).MatrixSize;
    (*MS).ModuleName = (*Mstr).ModuleName;
    (*MS).OutputSize = (*Mstr).OutputSize;
    (*MS).Padding =  (*Mstr).Padding;
    (*MS).ModuleId =  (*Mstr).ModuleId;
    (*MS).PoolScale = (*Mstr).PoolScale;
    (*MS).FilterCount = (*Mstr).FilterCount;
    (*MS).FilterDepth = (*Mstr).FilterDepth;
    ModuleData.push_back(MS);
}