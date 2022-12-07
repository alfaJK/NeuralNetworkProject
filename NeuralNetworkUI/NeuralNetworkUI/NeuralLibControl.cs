using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Runtime;
using System.Runtime.InteropServices;
using static System.Net.Mime.MediaTypeNames;
using System.Windows.Forms;
using System.Reflection.Metadata;
using System.CodeDom;
using System.IO;
using Newtonsoft.Json.Linq;
using Newtonsoft.Json;
using System.Windows.Markup;

namespace NeuralNetworkUI
{
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public struct ModuleSetting
        {
            public string ModuleName;
            public string FuncActivationName;
            public string PoolingType;
            public int MatrixSize;
            public int ModuleId;
            public int Padding;
            public int OutputSize;
            public int PoolScale;
            public int FilterCount;
            public int FilterDepth;
        }
            
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public struct NeuralSetting
        {
            public string NeuralName;
            public string training_path;
            public string learn_path;
            public string weight_path;
            public string file_name;
            public double rate;
            public int ImageSize;
            public int cout_typeset;
            public int cout_training;
            public int cout_module;
        }

    class NeuralLibControl
        {
            public int CountMod;
            public int[] ConfMatrixData;

            [DllImport("SSD.dll")]
            static private extern IntPtr Create_NeuralControl();

            [DllImport("SSD.dll")]
            static private extern IntPtr GetConfMatrix(IntPtr NeurPtr);

            [DllImport("SSD.dll", CharSet = CharSet.Ansi)]
            static private extern double GetProgressData(IntPtr NeurPtr);

            [DllImport("SSD.dll", CharSet = CharSet.Ansi)]
            static private extern int GetEpoh(IntPtr NeurPtr);

            [DllImport("SSD.dll", CharSet = CharSet.Ansi)]
            static private extern double GetError(IntPtr NeurPtr);

            [DllImport("SSD.dll", CharSet = CharSet.Ansi)]
            static private extern double GetSens(IntPtr NeurPtr);

            [DllImport("SSD.dll", CharSet = CharSet.Ansi)]
            static private extern double GetSpec(IntPtr NeurPtr);

            [DllImport("SSD.dll", CharSet = CharSet.Ansi)]
            static private extern int GetTestData(IntPtr NeurPtr);

            [DllImport("SSD.dll", CharSet = CharSet.Ansi)]
            static private extern void InitNeural(IntPtr NeurPtr);

            [DllImport("SSD.dll", CharSet = CharSet.Ansi)]
            static private extern void StartLearn(IntPtr NeurPtr);

            [DllImport("SSD.dll", CharSet = CharSet.Ansi)]
            static private extern bool IsInitModel(IntPtr NeurPtr);

            [DllImport("SSD.dll", CharSet = CharSet.Ansi)]
            static private extern int GetSettingStruct(IntPtr NeurPtr , ref NeuralSetting SettingStruct);

            [DllImport("SSD.dll", CharSet = CharSet.Ansi)]
            static private extern int GetModuleSettingStruct(IntPtr NeurPtr, ref ModuleSetting ModSettingStruct);

            public bool IsInit;
            private IntPtr NeuralPtr;
            private NeuralSetting Setting;
            private ModuleSetting[] mod_set;
            public NeuralLibControl(NeuralSetting setting , ModuleSetting[] mod_set)
            {
                IsInit = false;
                ConfMatrixData = new int[setting.cout_typeset * setting.cout_typeset];
                Setting = setting;
                Setting.cout_module = mod_set.Length;
                this.mod_set = mod_set;
                Thread thread1 = new Thread(() =>
                {
                    try{ CreateNeural();}
                    finally{StartLearning();}
                });
                thread1.Start();
            }

            public void CreateNeural()
            {
                NeuralPtr = Create_NeuralControl();
            }
            public int GetTestData()
            {
                return GetTestData(NeuralPtr);
            }
            public void _GetConfMatrix()
            {
            Marshal.Copy(GetConfMatrix(NeuralPtr), ConfMatrixData, 0, ConfMatrixData.Length);    // Copy result to array.
          //  Marshal.FreeCoTaskMem(a);
            }
           public int _GetEpoh()
            {
                return GetEpoh(NeuralPtr);
            }
            public double _GetError()
            {
                return GetError(NeuralPtr);
            }
            public double _GetSens()
            {
                return GetSens(NeuralPtr);
            }

            public double _GetSpec()
            {
                return GetSpec(NeuralPtr);
            }
            public double GetProgressData()
            {
                return GetProgressData(NeuralPtr);
            }

            public void StartLearning()
            {

            GetSettingStruct(NeuralPtr,ref Setting).ToString();
            for (int i = 0; i < mod_set.Length; i++)
            {
                GetModuleSettingStruct(NeuralPtr, ref mod_set[i]).ToString();
            }
                 
                InitNeural(NeuralPtr);
                IsInit = IsInitModel(NeuralPtr);
            if (IsInit)
            {
                MessageBox.Show("Удалось создать модель");
                StartLearn(NeuralPtr); 
            }
            else MessageBox.Show("Не удалось создать модель");

            }

    }
    
}
