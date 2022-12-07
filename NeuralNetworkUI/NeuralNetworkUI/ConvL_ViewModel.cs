using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NeuralNetworkUI
{
    public class ConvL_ViewModel : BaseViewModel
    {
        private ModuleSetting _ConvLsetting;
        public ObservableCollection<string> ActivationItems { get; set; }
        public ModuleSetting ConvLsetting
        {
            get { return _ConvLsetting; }
            set { _ConvLsetting = value; }
        }

        public string MatrixSize
        {
            set
            {
                int ignore;
                if (int.TryParse(value, out ignore))
                {
                    _ConvLsetting.MatrixSize = int.Parse(value);
                    OnPropertyChanged("MatrixSize");
                }
            }
        }
        public string PaddingSize
        {
            set
            {
                int ignore;
                if (int.TryParse(value, out ignore))
                {
                    _ConvLsetting.Padding = int.Parse(value);
                    OnPropertyChanged("PaddingSize");
                }
            }
        }

        public string FilterCount
        {
            set
            {
                int ignore;
                if (int.TryParse(value, out ignore))
                {
                    _ConvLsetting.FilterCount = int.Parse(value);
                    OnPropertyChanged("FilterCount");
                }
            }
        }

        public string FilterDepth
        {
            set
            {
                int ignore;
                if (int.TryParse(value, out ignore))
                {
                    _ConvLsetting.FilterDepth = int.Parse(value);
                    OnPropertyChanged("FilterDepth");
                }
            }
        }

    }
}
