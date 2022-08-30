//#include "../../DLL/NeuralLib/NeuralLib/pch.h"
#include "NeuralController/Conv_neural.h"
#include <time.h> 
Neural::Neural(const char* setting_filename) {
    Neural_setting = new Setting();
    Neural_setting->Load_Setting(setting_filename);
    Module.resize(Neural_setting->Count_Module);
    ModuleResult.resize(Neural_setting->Count_Module+1);
    std::string road = Neural_setting->path + "0" + "\\" + Neural_setting->name + "0" + " (1).jpg";
    InitImage = new Image(Neural_setting->SizeImage ,  road.c_str());
    ModuleResult[0] = new Tensor(InitImage->NewImageBits);
    int rr = 1;
    for (int i = 0; i < Neural_setting->Count_Module; i++) {
        
        if (Neural_setting->Module[i] == 0) {
            if (rr == 1) {
                Module.at(i) = new ConvL(ModuleResult[i]->size, Neural_setting->filter_size, 16, 1, Neural_setting->Padding);
                clock_t start = clock();
                ModuleResult[i + 1] = new Tensor(Module[i]->Direct_dist(*ModuleResult[i]));
                clock_t end = clock();
                double seconds = (double)(end - start) / CLOCKS_PER_SEC;
                printf("The time: %f seconds\n", seconds);
                int ff;
                std::cin>>ff;
                rr++;
                continue;
            }
            if (rr == 2) {
                Module.at(i) = new ConvL(ModuleResult[i]->size, Neural_setting->filter_size, 32, 1, Neural_setting->Padding);
                ModuleResult[i + 1] = new Tensor(Module[i]->Direct_dist(*ModuleResult[i]));
                rr--;
                continue;
            }
        }
        if (Neural_setting->Module[i] == 1) {
            Module.at(i) = new ActivationLayer(ModuleResult[i]->size);
            ModuleResult[i + 1] = new Tensor(Module[i]->Direct_dist(*ModuleResult[i]));
            continue;
        }
        if (Neural_setting->Module[i] == 2) {
            Module.at(i) = new PoolingLayer(ModuleResult[i]->size, 2);
            ModuleResult[i + 1] = new Tensor(Module[i]->Direct_dist(*ModuleResult[i]));
            continue;
        }
        if (Neural_setting->Module[i] == 3) {
            Module.at(i) = new FullyConnectedLayer(ModuleResult[i]->size, Neural_setting->count_TypeSet, Neural_setting->f_activation);
            ModuleResult[i + 1] = new Tensor(Module[i]->Direct_dist(*ModuleResult[i]));
            continue;
        }
    }
}
void Neural::Start() {
    if (Neural_setting->learn == 1) Learn();

    if (Neural_setting->learn == 0) {
        for (int i = 0; i < Neural_setting->Count_Module; i++) {
            std::stringstream num; num << i;
            std::string road = Neural_setting->weight_path + "\\Mod_Weight" + num.str() + ".txt";
            Module[i]->LoadWeight(road.c_str());
        }
        Active();
    }
    
}


void Neural::setcur(int x, int y)//установка курсора на позицию  x y 
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
};

void Neural::LoadBar(int size , int count ,int progress ) {
    std::cout << "  [";
    for (int i = 1; i <= size; i++) {
        if (i <= ((double)progress / count)*size) { std::cout << (char)254; continue; }
        std::cout << " ";
    }
    std::cout << "] ";
}

void Neural::Learn() {

    int sw = 0;
    double error = 0;
    double spec = 0;
    double sens = 0;

    for (int u = 0; u < 200000; u++) {
        for (int c = 0; c < Neural_setting->cout_learn-1; c++) {
            for (int c_f = 0; c_f < Neural_setting->count_TypeSet; c_f++) {
                if (sw ==50) {
                 
                    for (int i = 0; i < Neural_setting->Count_Module; i++) {
                        std::stringstream num; num << i;
                        std::string road = Neural_setting->weight_path + "\\Mod_Weight" + num.str() + ".txt";
                        Module[i]->UploadWeight(road.c_str());
                    }
                    std::ofstream addMember1(Neural_setting->data_path + "\\graph_data1.txt", std::ios::app);
                    std::ofstream addMember2(Neural_setting->data_path + "\\graph_data2.txt", std::ios::app);
                    Active();
                    error = 1.0 - ((double)Metrics[0] / (Neural_setting->count_TypeSet * Neural_setting->cout_learn));
                 //   spec = ((double)Metrics[1] / Neural_setting->cout_learn) * 100;
                  //  sens = ((double)Metrics[2] / Neural_setting->cout_learn) * 100;
                    int epoh = u * Neural_setting->cout_learn + c;
                    addMember1 << epoh << std::endl;
                    addMember2 << error << std::endl;
              
                    addMember1.close();
                    addMember2.close();

                    sw = 0;

                }

                std::stringstream forden, cout_im;
                forden << c_f; cout_im << c + 1;
                std::string road = Neural_setting->path + forden.str() + "\\" + Neural_setting->name + forden.str() + " (" + cout_im.str() + ").jpg";
                Image ImageT(Neural_setting->SizeImage, road.c_str());
                *ModuleResult[0] = ImageT.NewImageBits;
             //   std::cout << *ModuleResult[0] << std::endl;
                for (int i = 0; i < Neural_setting->Count_Module; i++)
                    *ModuleResult[i + 1] = Module[i]->Direct_dist(*ModuleResult[i]);
                setcur(0, 0);
                LoadBar(80, Neural_setting->cout_learn, (c));
                double pr = ((double)(c) / Neural_setting->cout_learn) * 100;
                std::cout << pr << "%  "<< "rate: " << Neural_setting->rate<<"  "<<std::endl;
                std::cout << u << "/" << "200000  er1: " << error << "   " << Metrics[0] << " " ;
                //std::cout<< "spec " << spec << " sens " <<sens << std::endl;
                
              
                Tensor Dx = *ModuleResult[Neural_setting->Count_Module];

                for (int i = 0; i < Neural_setting->count_TypeSet; i++) {
                   if (i == c_f) { Dx.Tensor_Values[i] =  (Dx.Tensor_Values[i] - 1); }
                   else   Dx.Tensor_Values[i] =  (Dx.Tensor_Values[i] - 0);
                }

                // ОБУЧЕНИЕ НЕЙРОННОЙ СЕТИ
                for (int i = Neural_setting->Count_Module-1; i >= 0 ; i--) {
                    Dx = Module[i]->Back_dist(Dx, *ModuleResult[i]);
                    Module[i]->UpdateWeights(Neural_setting->rate);
                }
            }
            sw++;
        }
    }

    for (int i = 0; i < Neural_setting->Count_Module; i++) {
        std::stringstream num; num << i;
        std::string road = Neural_setting->weight_path + "\\Mod_Weight" + num.str() + ".txt";
        Module[i]->UploadWeight(road.c_str());
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void Neural::Active(){
 
        int False = 0;
        int tr[10] = {0,0,0,0,0,0,0,0,0,0};
        int fl[10] = {0,0,0,0,0,0,0,0,0,0};
        for (int i = 0; i < 3; i++) {
            Metrics[i] = 0;
        }
        for (int c = 1; c <= Neural_setting->cout_training - 1; c++) {
           for (int c_f = 0; c_f < Neural_setting->count_TypeSet; c_f++) {
                std::stringstream forden, cout_im;
                forden << c_f;
                cout_im << c;
                std::string road = Neural_setting->training_path + forden.str() + "\\" + Neural_setting->name + forden.str() + " (" + cout_im.str() + ").jpg";
               // std::string road = Neural_setting->training_path + Neural_setting->name + " (" + cout_im.str() + ").jpg";

                setcur(0, 2);
                std::cout << std::endl;
                for (int o = 0; o < Neural_setting->count_TypeSet; o++) {

                    std::cout << "TEST" << o << " True : " << tr[o] << " False : " << fl[o] << "  " << std::endl;
                }
                std::cout << std::endl;
                std::cout << "Test " << c_f << ":  " << *ModuleResult[Neural_setting->Count_Module] << "  " << std::endl;
                std::cout << std::endl;
                std::cout << " ALLTrue : " << Metrics[0] << " ALLFalse : " << False << "  " << std::endl;
                
                
                Image ImageT(Neural_setting->SizeImage, road.c_str());
                *ModuleResult[0] = ImageT.NewImageBits;
                for (int i = 0; i < Neural_setting->Count_Module; i++)
                    *ModuleResult[i + 1] = Module[i]->Direct_dist(*ModuleResult[i]);

             //   std::cout << "Test " <<"("<< c <<") :  " << *ModuleResult[Neural_setting->Count_Module] << std::endl;
                double answer = -1;
                int h = 0;
                for (int i = 0; i < Neural_setting->count_TypeSet; i++) {
                    if (ModuleResult[Neural_setting->Count_Module]->Tensor_Values[i] > answer) {
                        h = i;
                        answer = ModuleResult[Neural_setting->Count_Module]->Tensor_Values[i];
                    }
                }

                if (c_f == h) { 
                    tr[c_f]++; Metrics[0]++;
                    if (h == 0) {
                        Metrics[1]++;
                    }
                    if (h == 1) {
                        Metrics[2]++;
                    }
                }
                else { fl[c_f]++; False++; }

              
            }

        }
    }