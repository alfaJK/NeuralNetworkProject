using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NeuralNetworkUI
{
    public class ActiveL_ViewModel : BaseViewModel
    {
        private ModuleSetting _ActiveLSetting;
        public ObservableCollection<string> ActivationItems { get; set; }

        public ActiveL_ViewModel()
        {
            ActivationItems = new ObservableCollection<string>() { new string("relu"), new string("sigmoid") };
        }
        public ModuleSetting ActiveLSetting
        {
            get { return _ActiveLSetting; }
            set { _ActiveLSetting = value; }
        }

        public string SelectActivationItem
        {
            set
            {
                _ActiveLSetting.FuncActivationName = value;
            }
        }
    }
}
