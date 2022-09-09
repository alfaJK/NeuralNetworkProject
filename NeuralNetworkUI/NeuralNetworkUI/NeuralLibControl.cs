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

namespace NeuralNetworkUI
{

        struct NeuralSetting
        {
            int cout_learn;
            int cout_training;
            int count_TypeSet;
            int learn;
            int filter_size;
            int Padding;
            int Count_Module;
            int[] Module;
            double rate;
            string path;
            string f_activation;
            string file_name;
            string training_path;
            string data_path;
            string weight_path;
            // Tensor_size SizeImage;
        }
        class NeuralLibControl
        {
            public bool IsInit;

            [DllImport("SSD.dll")]
            static private extern IntPtr Create_NeuralControl();

            [DllImport("SSD.dll", CharSet = CharSet.Ansi)]
            static private extern double GetProgressData(IntPtr NeurPtr);

            [DllImport("SSD.dll", CharSet = CharSet.Ansi)]
            static private extern int GetTestData(IntPtr NeurPtr);

            [DllImport("SSD.dll", CharSet = CharSet.Ansi)]
            static private extern void StartLearn(IntPtr NeurPtr);

            [DllImport("SSD.dll", CharSet = CharSet.Ansi)]
            static private extern bool IsInitModel(IntPtr NeurPtr);

        private IntPtr NeuralPtr;
            //private NeuralSetting Setting;
            public NeuralLibControl()
            {
                IsInit = false;
                Thread thread1 = new Thread(() =>
                {
                    try{InitNeural();}
                    finally{StartLearning();}
                });
                thread1.Start();
        }

            public void InitNeural()
            {
                NeuralPtr = Create_NeuralControl();
            }


            public int GetTestData()
            {
                return GetTestData(NeuralPtr);
            }
            public double GetProgressData()
            {
                return GetProgressData(NeuralPtr);
            }

            public void StartLearning()
            {
            IsInit = IsInitModel(NeuralPtr);
            if (IsInit) { MessageBox.Show("Удалось создать модель"); StartLearn(NeuralPtr); }
            else MessageBox.Show("Не удалось создать модель");
            }

    }
    
}
