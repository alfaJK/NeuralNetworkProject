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
    /// Логика взаимодействия для ConvL_View.xaml
    /// </summary>
    public partial class ConvL_View : UserControl
    {
        public ConvL_ViewModel CnViewModel;
        public ConvL_View()
        {
            InitializeComponent();
            CnViewModel = new ConvL_ViewModel();
            DataContext = CnViewModel;
        }
        public void SetModule(ModuleSetting setting)
        {
            CnViewModel.ConvLsetting = setting;
        }
        public ModuleSetting GetModule()
        {
            return CnViewModel.ConvLsetting;
        }
    }
}
