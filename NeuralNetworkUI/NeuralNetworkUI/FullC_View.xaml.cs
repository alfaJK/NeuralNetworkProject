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
    /// Логика взаимодействия для FullC_View.xaml
    /// </summary>
    public partial class FullC_View : UserControl
    {
        public Fullc_ViewModel FCViewModel;
        public FullC_View()
        {
            FCViewModel = new Fullc_ViewModel();
            InitializeComponent();
            DataContext = FCViewModel;
        }
        public void SetModule(ModuleSetting setting)
        {
            FCViewModel.FullCSetting = setting;
        }
        public ModuleSetting GetModule()
        {
            return FCViewModel.FullCSetting;
        }
    }
}
