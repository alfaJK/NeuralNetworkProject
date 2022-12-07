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
        public ModuleSetting _ModuleSetting;
        private DelegateCommand _ConvLViewCommand;
        private DelegateCommand _PoolViewCommand;
        private DelegateCommand _ActiveLViewCommand;
        private DelegateCommand _FullCViewCommand;
        private UserControl _currentView;
        private UserControl _ConvLView;
        private UserControl _ActiveLView;
        private UserControl _PoolView;
        private UserControl _FullCView;

        public ObservableCollection<string> ModuleItems { get; set; }
        public ObservableCollection<string> Data { get; set; }
        public MainViewVM()
        {
            _ModuleSetting = new ModuleSetting();
            _ConvLView = new ConvL_View();
            _ActiveLView = new ActiveL_View();
            _PoolView = new Pool_View();
            _FullCView = new FullC_View();
            CurrentView = _ConvLView;
            ModuleItems = new ObservableCollection<string>() {new string("Сверточный слой") , new string("Активационный слой"),  new string("Слой пуллинга"), new string("Полносвязный слой") };
            Data = new ObservableCollection<string>() {};
        }
        private string _mySelectedItem;
        public DelegateCommand AddButton { get; set; }
        public DelegateCommand CannelButton { get; set; }

        public DelegateCommand ConvLViewCommand
        {
            get
            {
                return _ConvLViewCommand ?? (_ConvLViewCommand = new DelegateCommand(
                   x =>
                   {
                       GotoConvLView();
                   }));
            }
        }
        public DelegateCommand ActiveLViewCommand
        {
            get
            {
                return _ActiveLViewCommand ?? (_ActiveLViewCommand = new DelegateCommand(
                   x =>
                   {
                       GotoActiveLView();
                   }));
            }
        }
        public DelegateCommand PoolViewCommand
        {
            get
            {
                return _PoolViewCommand ?? (_PoolViewCommand = new DelegateCommand(
                   x =>
                   {
                       GotoPoolView();
                   }));
            }
        }
        public DelegateCommand FullCViewCommand
        {
            get
            {
                return _FullCViewCommand ?? (_FullCViewCommand = new DelegateCommand(
                   x =>
                   {
                       GotoFullCView();
                   }));
            }
        }
        private void GotoConvLView()
        {
            CurrentView = _ConvLView;
        }
        private void GotoActiveLView()
        {
            CurrentView = _ActiveLView;
        }
        private void GotoPoolView()
        {
            CurrentView = _PoolView;
        }
        private void GotoFullCView()
        {
            CurrentView = _FullCView;
        }

        public UserControl CurrentView
        {
            get { return _currentView; }
            set
            {
                _currentView = value;
                OnPropertyChanged("CurrentView");
            }
        }
        public string MySelectedItem { 
            get {
                return _mySelectedItem; } 
            set {    
                _mySelectedItem = value;

                switch(_mySelectedItem)
                {
                    case "Сверточный слой":
                        _ModuleSetting.ModuleName = "Conv_L";
                        _ModuleSetting.ModuleId = 1;
                        ((ConvL_View)_ConvLView).SetModule(_ModuleSetting);
                        CurrentView = _ConvLView;
                        break;
                    case "Активационный слой":
                        _ModuleSetting.ModuleName = "Active_L";
                        _ModuleSetting.ModuleId = 2;
                        ((ActiveL_View)_ActiveLView).SetModule(_ModuleSetting);
                        CurrentView = _ActiveLView;
                        break;
                    case "Слой пуллинга":
                        _ModuleSetting.ModuleName = "Pool_L";
                        _ModuleSetting.ModuleId = 3;
                        ((Pool_View)_PoolView).SetModule(_ModuleSetting);
                        CurrentView = _PoolView;
                        break;
                    case "Полносвязный слой":
                        _ModuleSetting.ModuleName = "FC_L";
                        _ModuleSetting.ModuleId = 4;
                        ((FullC_View)_FullCView).SetModule(_ModuleSetting);
                        CurrentView = _FullCView;
                        break;
                }
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

        public void GetSettings()
        {
            if(CurrentView != null)
            {
                switch (_mySelectedItem)
                {
                    case "Сверточный слой": 
                        _ModuleSetting = ((ConvL_View)_ConvLView).GetModule();
                        break;
                    case "Активационный слой":
                        _ModuleSetting = ((ActiveL_View)_ActiveLView).GetModule();
                        break;
                    case "Слой пуллинга":
                        _ModuleSetting = ((Pool_View)_PoolView).GetModule();
                        break;
                    case "Полносвязный слой":
                        _ModuleSetting = ((FullC_View)_FullCView).GetModule();
                        break;
                }
            }
            
        }

    }

    public interface IModule
    {
        /// <summary>
        /// Название выводимое в меню
        /// </summary>
        string Name { get; }
        /// <summary>
        /// Ссылка на вьюшку
        /// </summary>
        UserControl UserInterface { get; }
        /// <summary>
        /// Отключение текущей вьюшки, для вьюмоделей с реализацией IDisposable
        /// </summary>
        void Deactivate();
    }



}
