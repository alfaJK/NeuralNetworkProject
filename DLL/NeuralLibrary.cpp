#include "NeuralLibrary.h"

Neural* Create_NeuralControl(){
   return new Neural("config.txt");

}
double GetProgressData(Neural* NeurPtr){
   return NeurPtr->GetProgressData();
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