using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Forms;
using static System.Net.Mime.MediaTypeNames;
using TextBox = System.Windows.Controls.TextBox;

namespace NeuralNetworkUI
{

    public class NeuraSettinglView : BaseViewModel
    {
        
        public StackPanel root;
        private string _neuralname;
        public NeuralSetting NeuralSettingStruct;
        public string Learnfilename
        {
            get { return NeuralSettingStruct.learn_path; }
            set {
                NeuralSettingStruct.learn_path = value.Replace("\\", "\\\\") + "\\\\";
                OnPropertyChanged("Learnfilename");
            }
        }

        public string Testfilename
        {
            get { return NeuralSettingStruct.training_path; }
            set
            {
                NeuralSettingStruct.training_path = value.Replace("\\" , "\\\\" ) + "\\\\";
                OnPropertyChanged("Testfilename");
            }
        }
        public string Weightfilename
        {
            get { return NeuralSettingStruct.training_path; }
            set
            {
                NeuralSettingStruct.weight_path = value.Replace("\\", "\\\\") + "\\\\";
                OnPropertyChanged("Weightfilename");
            }
        }
        public string NeuralName
        {
            set
            {
                OnPropertyChanged("NeuralName");
                IsValid = IsfillTextBox();
                OnPropertyChanged("IsValid");
             
                    NeuralSettingStruct.NeuralName = value;
            }
        }

        public string FileName
        {
            set
            {
                OnPropertyChanged("FileName");
                IsValid = IsfillTextBox();
                OnPropertyChanged("IsValid");

                NeuralSettingStruct.file_name = value;

            }
        }
        public string Rate
        {
            set
            {
                double ignore;
                if (double.TryParse(value ,out ignore))
                {
                    NeuralSettingStruct.rate = double.Parse(value);
                    IsValid = IsfillTextBox();
                    OnPropertyChanged("IsValid");
                    OnPropertyChanged("Rate");
                }                   
            }
        }
        public string ImageSize
        {
            set
            {
                int ignore;
                if (int.TryParse(value, out ignore))
                {
                    NeuralSettingStruct.ImageSize = int.Parse(value);
                    IsValid = IsfillTextBox();
                    OnPropertyChanged("IsValid");
                    OnPropertyChanged("ImageSize");
                }
            }
        }
        public string CountTraining
        {
            set
            {
                int ignore;
                if (int.TryParse(value, out ignore))
                {
                    NeuralSettingStruct.cout_training = int.Parse(value);
                    IsValid = IsfillTextBox();
                    OnPropertyChanged("IsValid");
                    OnPropertyChanged("CountTraining");
                }
            }
        }
        public string CountTypeset
        {
            set
            {
                int ignore;
                if (int.TryParse(value, out ignore))
                {
                    NeuralSettingStruct.cout_typeset = int.Parse(value);
                    IsValid = IsfillTextBox();
                    OnPropertyChanged("IsValid");
                    OnPropertyChanged("CountTypesset");
                }
            }
        }


        public DelegateCommand AddButton { get; set; }
        public DelegateCommand CannelButton { get; set; }
        public DelegateCommand AddLearnFile { get; set; }
        public DelegateCommand AddTestFile { get; set; }
        public DelegateCommand AddWeightFile { get; set; }

        public bool IsfillTextBox()
        {
            bool test = true;
            root.Children.OfType<TextBox>().ToList()
                   .ForEach(item =>
                   {
                       if (item.Text == null || item.Text == "")
                       {
                           test =  false;
                       }
                   });
            return test;
        }
        public NeuraSettinglView()
        {
            AddLearnFile = new DelegateCommand((obj) => _AddLearFile());
            AddTestFile = new DelegateCommand((obj) => _AddTestFile());
            AddWeightFile = new DelegateCommand((obj) => _AddWeightFile());
        }

        private void _AddLearFile()
        {


            System.Windows.Forms.FolderBrowserDialog dlg = new FolderBrowserDialog();
            DialogResult result = dlg.ShowDialog();
            if (result == DialogResult.OK && !string.IsNullOrWhiteSpace(dlg.SelectedPath))
            {
                Learnfilename = dlg.SelectedPath;
            }
  
       

        }
        private void _AddTestFile()
        {
            System.Windows.Forms.FolderBrowserDialog dlg = new FolderBrowserDialog();
            DialogResult result = dlg.ShowDialog();
            if (result == DialogResult.OK && !string.IsNullOrWhiteSpace(dlg.SelectedPath))
            {
                Testfilename = dlg.SelectedPath;
            }

        }

        private void _AddWeightFile()
        {
            System.Windows.Forms.FolderBrowserDialog dlg = new FolderBrowserDialog();
            DialogResult result = dlg.ShowDialog();
            if (result == DialogResult.OK && !string.IsNullOrWhiteSpace(dlg.SelectedPath))
            {
                Weightfilename = dlg.SelectedPath;
            }

        }

        public bool IsValid { get; private set; }

    }
}
