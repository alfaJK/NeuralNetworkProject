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
using System.Collections.ObjectModel;
using static System.Net.Mime.MediaTypeNames;
using System.Windows.Media.Animation;
using MessageBox = System.Windows.Forms.MessageBox;
using DragEventArgs = System.Windows.Forms.DragEventArgs;

namespace NeuralNetworkUI
{

    public class CustomTreeItem
    {
        public string ItemName { get; set; }
        public CustomTreeItem Parent { get; set; }
        public ObservableCollection<CustomTreeItem> Items { get; set; }

        public ObservableCollection<CustomTreeItem> FindDirectParent(ObservableCollection<CustomTreeItem> _items)
        {
            ObservableCollection<CustomTreeItem> ret = new ObservableCollection<CustomTreeItem>();
            if (_items.Contains(this) == true)
            {
                ret = _items;
            }
            else
            {
                foreach (CustomTreeItem item in _items)
                {
                    if (item.Items != null && item.Items.Contains(this) == true)
                        ret = FindDirectParent(item.Items);
                }
            }
            return ret;
        }
    }


    public partial class MainWindow : Window
    {
        NeuralLibControl neuralLibControl;
        System.Windows.Threading.DispatcherTimer dispatcherTimer;
        public ObservableCollection<CustomTreeItem> CategoriesList { get; set; }

        public MainWindow()
        {
            InitializeComponent();
            this.SizeChanged += OnWindowSizeChanged;
            neuralLibControl = new NeuralLibControl();
            dispatcherTimer = new System.Windows.Threading.DispatcherTimer();
            dispatcherTimer.Tick += new EventHandler(dispatcherTimer_Tick);
            dispatcherTimer.Interval = new TimeSpan(0, 0, 1);
            dispatcherTimer.Start();
            CategoriesList = new ObservableCollection<CustomTreeItem>() { };
            this.DataContext = this;


            MenuItem ee = new MenuItem();
            ee.Header = "1343";
            addmenu.Items.Add(ee);
        }

    



        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            if (neuralLibControl.IsInit)
            {
                //TextBox1.Clear();
               // TextBox1.AppendText(neuralLibControl.GetProgressData().ToString());
            }
        }

        protected void OnWindowSizeChanged(object sender, SizeChangedEventArgs e)
        {
           treeView.Height = (e.NewSize.Height - 25)/2;
            treeView1.Height = (e.NewSize.Height - 25) / 2;
        }



        private void MenuItem_Add(object sender, RoutedEventArgs e)
        {

            CategoriesList.Add(new CustomTreeItem()
            {
                ItemName = "Item1",
            });
        }



    }
}
