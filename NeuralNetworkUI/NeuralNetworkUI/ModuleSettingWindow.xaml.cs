using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
using System.Windows.Shapes;

namespace NeuralNetworkUI
{
    /// <summary>
    /// Логика взаимодействия для ModuleSettingWindow.xaml
    /// </summary>
    public partial class ModuleSettingWindow : Window
    {
        public MainViewVM mv;
        public ModuleSettingWindow()
        {
            InitializeComponent();
            mv = new MainViewVM();
            mv.AddButton = new DelegateCommand((obj) => this.DialogResult = true);
            mv.CannelButton = new DelegateCommand((obj) => this.DialogResult = false);
            this.DataContext = mv;

        }

 
    }
}
