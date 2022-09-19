using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace NeuralNetworkUI
{

    public class NeuraSettinglView : BaseViewModel
    {
        public StackPanel root;
        private string _neuralname;
        public string _learnfilename;
        public string _testfilename;
        public string Learnfilename
        {
            get { return _learnfilename; }
            set { 
                _learnfilename = value;

                OnPropertyChanged("Learnfilename");
            }
        }

        public string Testfilename
        {
            get { return _testfilename; }
            set
            {
                _testfilename = value;
                OnPropertyChanged("Testfilename");
            }
        }
        [Required]
        public string NeuralName
        {
            get { return _neuralname; }
            set
            {
                _neuralname = value;

                bool test = true;
                root.Children.OfType<TextBox>().ToList()
                    .ForEach(item =>
                    {
                        if (item.Text == null || item.Text == "")
                        {
                            test = false;

                          //  MessageBox.Show(test.ToString());
                            
                        }
                        
                    });

                // MessageBox.Show(test.ToString());
                IsValid = test;

                OnPropertyChanged("IsValid");
                OnPropertyChanged("NeuralName");
               
            }
        }

        public DelegateCommand AddButton { get; set; }
        public DelegateCommand CannelButton { get; set; }
        public DelegateCommand AddLearnFile { get; set; }
        public DelegateCommand AddTestFile { get; set; }

        public NeuraSettinglView()
        {
            AddLearnFile = new DelegateCommand((obj) => _AddLearFile());
            AddTestFile = new DelegateCommand((obj) => _AddTestFile());
        }

        private void _AddLearFile()
        {
                Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();
                dlg.DefaultExt = ".txt";
                dlg.Filter = "TXT Files (*.txt)|*.txt";
                Nullable<bool> result = dlg.ShowDialog();
                if (result == true) { Learnfilename = dlg.FileName;}
       

        }
        private void _AddTestFile()
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();
            dlg.DefaultExt = ".txt";
            dlg.Filter = "TXT Files (*.txt)|*.txt";
            Nullable<bool> result = dlg.ShowDialog();
            if (result == true) { Testfilename = dlg.FileName; }

        }
       
        public bool IsValid { get; private set; }

    }
}
