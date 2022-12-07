using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace NeuralNetworkUI
{
    public class Pool_ViewModel : BaseViewModel
    {
        private ModuleSetting _PoolSetting;
        public ObservableCollection<string> Poolingitems { get; set; }

        public Pool_ViewModel()
        {
            Poolingitems = new ObservableCollection<string>() { new string("maxpooling"), new string("meanpooling") };
        }
        public ModuleSetting PoolSetting { get { return _PoolSetting; } set { _PoolSetting = value;} }

        public string SelectPoolType
        {
            set{ _PoolSetting.PoolingType = value;}
        }
        
        public string PoolSize
        {
            set
            {
                int ignore;
                if (int.TryParse(value, out ignore))
                {
                    _PoolSetting.MatrixSize = int.Parse(value);
                    OnPropertyChanged("PoolSize");
                }
            }
        }

        public string PoolScale
        {
            set
            {
                int ignore;
                if (int.TryParse(value, out ignore))
                {
                    _PoolSetting.PoolScale = int.Parse(value);
                    OnPropertyChanged("PoolScale");
                }
            }
        }
    }
}
