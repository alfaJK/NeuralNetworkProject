//#include "../../DLL/NeuralLib/NeuralLib/pch.h"
#include "NeuralController/Conv_neural.h"
#include <time.h> 
#include <filesystem>

Neural::Neural() {
    isInit = false;
    pr = 0;
    testnum = 0;
    Neural_setting = new Setting();
}
void Neural::InitNeural() {
    ConfMatrix = new int[Neural_setting->cout_typeset * Neural_setting->cout_typeset];
    for (int i = 0; i < Neural_setting->cout_typeset; i++)
        for (int j = 0; j < Neural_setting->cout_typeset; j++)
            ConfMatrix[i * Neural_setting->cout_typeset + j] = 0;

    Module.resize(Neural_setting->cout_module);
    ModuleResult.resize(Neural_setting->cout_module+1);
  /*  std::string road =  Neural_setting->learn_path;
    std::filesystem::path FirstFilePath;
    for (const auto & entry : std::filesystem::directory_iterator(road)){
        FirstFilePath = entry.path();
        break;
    }*/
     std::string path_string = Neural_setting->learn_path  + "0" + "\\\\" + Neural_setting->file_name + "0" + " (1).jpg";
    //std::string path_string = "C:\\Users\\Даниил\\Desktop\\LEARNDATA\\0\\im_0 (1).jpg"; // {FirstFilePath.u8string() +  "\\" + Neural_setting->file_name + "0" + " (1).jpg "};
   // std::string road =  Neural_setting->learn_path + "0" + "\\" + Neural_setting->file_name + "0" + " (1).jpg";
    InitImage = new Image(Neural_setting->SizeImage ,  path_string.c_str());
  //  InitImage->CreateImage("test.jpg");
    //std::ofstream addMemberIM(Neural_setting->weight_path + "\\IM.txt", std::ios::app);

    //addMemberIM <<InitImage->NewImageBits << std::endl;
    //addMemberIM.close();
    ModuleResult[0] = new Tensor(InitImage->NewImageBits);
    for (int i = 0; i < Neural_setting->cout_module; i++) {
        if(Neural_setting->ModuleData[i]->ModuleId == 1){
                isInit = true;
                Module.at(i) = new ConvL(ModuleResult[i]->size, Neural_setting->ModuleData[i]->MatrixSize, Neural_setting->ModuleData[i]->FilterCount, Neural_setting->ModuleData[i]->FilterDepth, Neural_setting->ModuleData[i]->Padding);
                ModuleResult[i + 1] = new Tensor(Module[i]->Direct_dist(*ModuleResult[i]));
                num_buf.push_back(i);
                continue;
            }
        if(Neural_setting->ModuleData[i]->ModuleId == 2){
              //  Module.at(i) = new ActivationLayer(Neural_setting->ModuleData[i]->FuncActivationName,ModuleResult[i]->size);
                Module.at(i) = new ActivationLayer(ModuleResult[i]->size);
                ModuleResult[i + 1] = new Tensor(Module[i]->Direct_dist(*ModuleResult[i]));
                continue;
            }
        if(Neural_setting->ModuleData[i]->ModuleId == 3){
                Module.at(i) = new PoolingLayer(ModuleResult[i]->size, Neural_setting->ModuleData[i]->PoolScale);
                ModuleResult[i + 1] = new Tensor(Module[i]->Direct_dist(*ModuleResult[i]));
                continue;
            }
        if(Neural_setting->ModuleData[i]->ModuleId == 4){
                Module.at(i) = new FullyConnectedLayer(ModuleResult[i]->size, Neural_setting->ModuleData[i]->OutputSize, Neural_setting->ModuleData[i]->FuncActivationName);
                ModuleResult[i + 1] = new Tensor(Module[i]->Direct_dist(*ModuleResult[i]));
                continue;
           }
   /*     if (Neural_setting->Module[i] == 0) {
            if (rr == 1) {
                Module.at(i) = new ConvL(ModuleResult[i]->size, Neural_setting->filter_size, 16, 1, Neural_setting->Padding);
               // clock_t start = clock();
                ModuleResult[i + 1] = new Tensor(Module[i]->Direct_dist(*ModuleResult[i]));
               // clock_t end = clock();
               // double seconds = (double)(end - start) / CLOCKS_PER_SEC;
              //  printf("The time: %f seconds\n", seconds);
              //  int ff;
               // std::cin>>ff;
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
        }*/
    }
}
void Neural::Start() {
    if (Neural_setting->learn == 1) Learn();

    if (Neural_setting->learn == 0) {
        for (int i = 0; i < Neural_setting->cout_module; i++) {
            std::stringstream num; num << i;
            std::string road = Neural_setting->weight_path + "Mod_Weight" + num.str() + ".txt";
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
    for (int u = 0; u < 200000; u++) {
        for (int c = 0; c < Neural_setting->cout_training; c++) {
            for (int c_f = 0; c_f < Neural_setting->cout_typeset; c_f++) {
                std::stringstream forden, cout_im;
                forden << c_f; cout_im << c + 1;
              //  std::string road = Neural_setting->learn_path  + "0" + "\\\\" + Neural_setting->file_name + "0" + " (1).jpg";
                std::string road = Neural_setting->learn_path + forden.str() + "\\\\" + Neural_setting->file_name + forden.str() + " (" + cout_im.str() + ").jpg";
                Image ImageT(Neural_setting->SizeImage, road.c_str());
                *ModuleResult[0] = ImageT.NewImageBits;
             //   std::cout << *ModuleResult[0] << std::endl;
                for (int i = 0; i < Neural_setting->cout_module; i++)
                    *ModuleResult[i + 1] = Module[i]->Direct_dist(*ModuleResult[i]);
            //    setcur(0, 0);
          //      LoadBar(80, Neural_setting->cout_learn, (c));
               // pr = ((double)(c) / Neural_setting->cout_training) * 100;
              //  std::cout << pr << "%  "<< "rate: " << Neural_setting->rate<<"  "<<std::endl;
             //   std::cout << u << "/" << "200000  er1: " << error << "   " << Metrics[0] << " " ;
                //std::cout<< "spec " << spec << " sens " <<sens << std::endl;
                
                pr = ( ((u*c) + c) / (20 * (double)Neural_setting->cout_training ) )* 100;


                Tensor Dx = *ModuleResult[Neural_setting->cout_module];

                for (int i = 0; i < Neural_setting->cout_typeset; i++) {
                   if (i == c_f) { Dx.Tensor_Values[i] =  (Dx.Tensor_Values[i] - 1); }
                   else   Dx.Tensor_Values[i] =  (Dx.Tensor_Values[i] - 0);
                }

                // ОБУЧЕНИЕ НЕЙРОННОЙ СЕТИ
                for (int i = Neural_setting->cout_module-1; i >= 0 ; i--) {
                    Dx = Module[i]->Back_dist(Dx, *ModuleResult[i]);
                    Module[i]->UpdateWeights(Neural_setting->rate);
                }
            }
        }
                    std::ofstream addMember1(Neural_setting->weight_path + "\\graph_data1.txt", std::ios::app);
                    std::ofstream addMember2(Neural_setting->weight_path + "\\graph_data2.txt", std::ios::app);
                    std::ofstream addMembersens(Neural_setting->weight_path + "\\graph_datasens.txt", std::ios::app);
                    std::ofstream addMemberspec(Neural_setting->weight_path + "\\graph_dataspec.txt", std::ios::app);
                    Active();
                    error = 1.0 - ((double)Metrics[0] / (Neural_setting->cout_typeset * Neural_setting->cout_training));
                    spec = ((double)Metrics[1] / Neural_setting->cout_training);
                    sens = ((double)Metrics[2] / Neural_setting->cout_training);
                    epoh = u+1;
                    addMember1 << epoh << std::endl;
                    addMember2 << error << std::endl;
                    addMembersens << sens << std::endl;
                    addMemberspec << spec << std::endl;
                    addMember1.close();
                    addMember2.close();
                    addMembersens.close();
                    addMemberspec.close();
    }

  /*  for (int i = 0; i < Neural_setting->cout_module; i++) {
        std::stringstream num; num << i;
        std::string road = Neural_setting->weight_path + "\\Mod_Weight" + num.str() + ".txt";
        Module[i]->UploadWeight(road.c_str());
    }
    std::cout << std::endl;
    std::cout << std::endl;*/
}

void Neural::Active(){
    std::string path_string2 = Neural_setting->learn_path  + "1" + "\\\\" + Neural_setting->file_name + "1" + " (11).jpg";
    Image IImage(Neural_setting->SizeImage ,  path_string2.c_str());
    *ModuleResult[0] = IImage.NewImageBits;
        for (int i = 0; i < Neural_setting->cout_module; i++){
                *ModuleResult[i + 1] = Module[i]->Direct_dist(*ModuleResult[i]);
                }
        for ( int k =0; k <num_buf.size() ; k++){
                        std::stringstream num;
                        num << num_buf[k]+1;
                        std::string p = "test" + num.str();
                        Image InpImage(p ,ModuleResult[num_buf[k]+1]);
        }
        for (int i = 0; i < Neural_setting->cout_typeset; i++)
            for (int j = 0; j < Neural_setting->cout_typeset; j++)
                ConfMatrix[i * Neural_setting->cout_typeset + j] = 0;
 
        int False = 0;
        int tr[2] = {0,0};
        int fl[2] = {0,0};
        for (int i = 0; i < 3; i++) {
            Metrics[i] = 0;
        }
        std::ofstream addMember(Neural_setting->weight_path + "\\answer.txt", std::ios::app);
        for (int c = 1; c <= Neural_setting->cout_training - 1; c++) {
           for (int c_f = 0; c_f < Neural_setting->cout_typeset; c_f++) {
                std::stringstream forden, cout_im;
                forden << c_f;
                cout_im << c;
                std::string road = Neural_setting->training_path + forden.str() + "\\" + Neural_setting->file_name + forden.str() + " (" + cout_im.str() + ").jpg";
               // std::string road = Neural_setting->training_path + Neural_setting->name + " (" + cout_im.str() + ").jpg";

           /*     setcur(0, 2);
                std::cout << std::endl;
                for (int o = 0; o < Neural_setting->cout_typeset; o++) {

                    std::cout << "TEST" << o << " True : " << tr[o] << " False : " << fl[o] << "  " << std::endl;
                }
                std::cout << std::endl;
                std::cout << "Test " << c_f << ":  " << *ModuleResult[Neural_setting->cout_module] << "  " << std::endl;
                std::cout << std::endl;
                std::cout << " ALLTrue : " << Metrics[0] << " ALLFalse : " << False << "  " << std::endl;*/
                
                
                Image ImageT(Neural_setting->SizeImage, road.c_str());
                *ModuleResult[0] = ImageT.NewImageBits;
                for (int i = 0; i < Neural_setting->cout_module; i++)
                    *ModuleResult[i + 1] = Module[i]->Direct_dist(*ModuleResult[i]);
                

             //   std::cout << "Test " <<"("<< c <<") :  " << *ModuleResult[Neural_setting->Count_Module] << std::endl;
                double answer = -1;
                int h = 0;
                for (int i = 0; i < Neural_setting->cout_typeset; i++) {
                    addMember << ModuleResult[Neural_setting->cout_module]->Tensor_Values[i] <<" ";
                    if (ModuleResult[Neural_setting->cout_module]->Tensor_Values[i] > answer) {
                        h = i;
                        answer = ModuleResult[Neural_setting->cout_module]->Tensor_Values[i];
                    }
                }
                  addMember << std::endl;
                ConfMatrix[h * Neural_setting->cout_typeset + c_f] += 1;
       /*           for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            printf("%d %d: %d\n", i, j, a[i * n + j]);*/

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
        addMember.close();

        std::ofstream addMemberM(Neural_setting->weight_path + "\\Mdata1.txt", std::ios::app);

          for (int i = 0; i < Neural_setting->cout_typeset; i++){
            for (int j = 0; j < Neural_setting->cout_typeset; j++){
                addMemberM << ConfMatrix[i * Neural_setting->cout_typeset + j];
            }
             addMemberM << std::endl;
          }
        addMemberM.close();
    }