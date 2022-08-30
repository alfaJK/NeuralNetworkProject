#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#define BLOCK_SIZE 32

#ifdef __DLL__
   #define DECLSPEC    __declspec(dllexport)
#else
   #define DECLSPEC    __declspec(dllimport)
#endif 