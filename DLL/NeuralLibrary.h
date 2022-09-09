#include "NeuralController/Conv_neural.h"
#include "IOData/Images.h"
#include "NeuralController/Setting.h"

#ifdef __DLL__
   #define DECLSPEC    __declspec(dllexport)
#else
   #define DECLSPEC    __declspec(dllimport)
#endif 

extern "C" {
	extern DECLSPEC Neural* Create_NeuralControl();
    extern DECLSPEC void StartLearn(Neural* NeurPtr);
	extern DECLSPEC double GetProgressData(Neural* NeurPtr);
    extern DECLSPEC int GetTestData(Neural* NeurPtr);
    extern DECLSPEC bool IsInitModel(Neural* NeurPtr);
}