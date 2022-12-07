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
    /// Логика взаимодействия для Pool_View.xaml
    /// </summary>
    public partial class Pool_View : UserControl
    {
        public Pool_ViewModel PViewModel;
        public Pool_View()
        {
            PViewModel = new Pool_ViewModel();
            InitializeComponent();
            DataContext = PViewModel;
        }
        public void SetModule(ModuleSetting setting)
        {
            PViewModel.PoolSetting = setting;
        }
        public ModuleSetting GetModule()
        {
            return PViewModel.PoolSetting;
        }
    }
}
