using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NeuralNetworkUI
{
    public class NeuraSettinglView : BaseViewModel
    {
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
        public string NeuralName
        {
            get { return _neuralname; }
            set
            {
                _neuralname = value;
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

    }
}
