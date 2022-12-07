#include "NeuralLibrary.h"

Neural* Create_NeuralControl(){
   return new Neural();
}

void InitNeural(Neural* NeurPtr){
   NeurPtr->InitNeural();
}

double GetProgressData(Neural* NeurPtr){
   return NeurPtr->GetProgressData();
}

double GetError(Neural* NeurPtr){
   return NeurPtr->error;
}
double GetSens(Neural* NeurPtr){
   return NeurPtr->sens;
}
double GetSpec(Neural* NeurPtr){
   return NeurPtr->spec;
}
int GetEpoh(Neural* NeurPtr){
   return NeurPtr->epoh;
}
int* GetConfMatrix(Neural* NeurPtr){
   return NeurPtr->ConfMatrix;
}


int GetTestData(Neural* NeurPtr){
   return NeurPtr->GetTestData();
}
void StartLearn(Neural* NeurPtr){
   NeurPtr->Start();
}

bool IsInitModel(Neural* NeurPtr){
   return NeurPtr->IsInitModel();
}
int GetSettingStruct(Neural* NeurPtr , SettingStruct* st){
   NeurPtr->Neural_setting->CopyStruct(st);
   return  NeurPtr->Neural_setting->cout_module;
}

int GetModuleSettingStruct(Neural* NeurPtr , ModuleSetting* st){
   NeurPtr->Neural_setting->CopyModuleStruct(st);
   return NeurPtr->Neural_setting->ModuleData[0]->ModuleId;
}