using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Forms.Design;
using System.Windows.Input;

namespace NeuralNetworkUI
{

    public class MainViewVM : BaseViewModel
    {
        public ObservableCollection<string> ModuleItems { get; set; }
        public ObservableCollection<string> ActivationItems { get; set; }

        public ObservableCollection<string> Data { get; set; }
        public MainViewVM()
        {
            ModuleItems = new ObservableCollection<string>() {new string("Сверточный слой") , new string("Слой пуллинга"), new string("Полносвязный слой") };
            ActivationItems = new ObservableCollection<string>() {new string("relu"), new string("sigmoid") };
            Data = new ObservableCollection<string>() {};
        }
        private string _mySelectedItem;
        private DelegateCommand _AddButton;
        public DelegateCommand AddButton { get; set; }
        public DelegateCommand CannelButton { get; set; }
        public string MySelectedItem { 
            get {
                return _mySelectedItem; } 
            set {
                
                _mySelectedItem = value;
                Data.Add(_mySelectedItem);
            }
        }

        public Visibility myVisibility
        {
            get {
                if (!String.IsNullOrEmpty(MySelectedItem))
                {
                    return Visibility.Hidden;
                }
                return Visibility.Visible;
            }
        }

    }

}
