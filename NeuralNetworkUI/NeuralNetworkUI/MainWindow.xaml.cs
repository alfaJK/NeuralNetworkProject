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
using System.Windows.Forms;
using OxyPlot.Wpf;
using OxyPlot;
using static System.Net.Mime.MediaTypeNames;
using System.Windows.Media.Animation;

namespace NeuralNetworkUI
{


    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 



    public partial class MainWindow : Window
    {
        NeuralLibControl neuralLibControl;
        System.Windows.Threading.DispatcherTimer dispatcherTimer;

        public MainWindow()
        {
            

            InitializeComponent();
            TreeViewItem newChild = new TreeViewItem();
            newChild.Header = "DADAD";
           // treeView1.Items.Add(newChild);



            neuralLibControl = new NeuralLibControl();
            dispatcherTimer = new System.Windows.Threading.DispatcherTimer();
            dispatcherTimer.Tick += new EventHandler(dispatcherTimer_Tick);
            dispatcherTimer.Interval = new TimeSpan(0, 0, 1);
            dispatcherTimer.Start();
    
            //TextBox1.GetBindingExpression(TextBox.TextProperty).UpdateSource();
        }

    

    private void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
        }
        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            if (neuralLibControl.IsInit)
            {
                //TextBox1.Clear();
               // TextBox1.AppendText(neuralLibControl.GetProgressData().ToString());
            }
        }
    }
}
