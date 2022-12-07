using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NeuralNetworkUI
{
    public class Fullc_ViewModel : BaseViewModel
    {
        private ModuleSetting _FullCSetting;
        public ObservableCollection<string> ActivationItems { get; set; }

        public Fullc_ViewModel()
        {
            ActivationItems = new ObservableCollection<string>() { new string("relu"), new string("sigmoid") };
        }
        public ModuleSetting FullCSetting{
            get { return _FullCSetting; }
            set { _FullCSetting = value; }
        }

        public string SelectActivationItem
        {
            set
            {
                _FullCSetting.FuncActivationName = value;
            }
        }

        public string OutSize
        {
            set
            {
                int ignore;
                if (int.TryParse(value, out ignore))
                {
                    _FullCSetting.OutputSize = int.Parse(value);
                    OnPropertyChanged("OutSize");
                }
               
            }
        }
    }
}
