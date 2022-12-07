using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace NeuralNetworkUI
{
    /// <summary>
    /// Логика взаимодействия для ActiveL_View.xaml
    /// </summary>
    
    public partial class ActiveL_View : UserControl
    {

        public ActiveL_ViewModel AViewModel;
        public ActiveL_View()
        {
            AViewModel = new ActiveL_ViewModel();
            InitializeComponent();
            DataContext = AViewModel;
        }
        public void SetModule(ModuleSetting setting)
        {
            AViewModel.ActiveLSetting = setting;
        }
        public ModuleSetting GetModule()
        {
            return AViewModel.ActiveLSetting;
        }
    }
}
