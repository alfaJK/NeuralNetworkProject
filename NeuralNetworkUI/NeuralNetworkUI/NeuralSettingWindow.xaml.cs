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
using System.Windows.Shapes;

namespace NeuralNetworkUI
{
    /// <summary>
    /// Логика взаимодействия для NeuralSettingWindow.xaml
    /// </summary>
    public partial class NeuralSettingWindow : Window
    {
        public NeuraSettinglView ns;
        public NeuralSettingWindow()
        {
            InitializeComponent();
            ns = new NeuraSettinglView();
            ns.AddButton = new DelegateCommand((obj) => this.DialogResult = true);
            ns.CannelButton = new DelegateCommand((obj) => this.DialogResult = false);
            this.DataContext = ns;

        }
    }
}
