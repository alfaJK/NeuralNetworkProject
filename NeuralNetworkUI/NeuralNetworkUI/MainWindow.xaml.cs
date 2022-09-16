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
using System.IO;
using OxyPlot;
using System.Collections.ObjectModel;
using static System.Net.Mime.MediaTypeNames;
using System.Windows.Media.Animation;
using MessageBox = System.Windows.Forms.MessageBox;
using DragEventArgs = System.Windows.Forms.DragEventArgs;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;
using System.Security.Cryptography;
using static System.Windows.Forms.LinkLabel;
using System.Reflection.PortableExecutable;

namespace NeuralNetworkUI
{
    
    public class CustomTreeItem
    {
        public string ItemName { get; set; }
        public CustomTreeItem Parent { get; set; }
        public ObservableCollection<CustomTreeItem> Items { get; set; }
        public CustomTreeItem() { }
        public CustomTreeItem(string ModuleName)
        {
           
      
            switch (ModuleName)
            {
                case "Conv_L":
                    ItemName = "Сверточный слой";
                    break;
                case "Pool_L":
                    ItemName = "Слой пуллинга";
                    break;
                case "FC_L":
                    ItemName = "Полносвязный слой";
                    break;

            }
             Items = new ObservableCollection<CustomTreeItem>();
        }
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
    public partial class MainWindow : System.Windows.Window
    {
        private string FileName;
        private ObservableCollection<string> ModuleName;
        NeuralLibControl neuralLibControl;
        Graph grap;
        DataGraph dataGraph;
        System.Windows.Threading.DispatcherTimer dispatcherTimer;
        public ObservableCollection<CustomTreeItem> categoriesList { get; set; }
        public MainWindow()
        {
            ModuleName = new ObservableCollection<string>();
            dataGraph = new DataGraph();
            grap = new Graph();
            InitializeComponent();
            this.SizeChanged += OnWindowSizeChanged;
            
            dispatcherTimer = new System.Windows.Threading.DispatcherTimer();
            dispatcherTimer.Tick += new EventHandler(dispatcherTimer_Tick);
            dispatcherTimer.Interval = new TimeSpan(0, 0, 1);
            dispatcherTimer.Start();
            categoriesList = new ObservableCollection<CustomTreeItem>() { };
            
            this.DataContext = this;
        }

        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            if(neuralLibControl != null)
            {
                if (neuralLibControl.IsInit)
                {
                    //TextBox1.Clear();
                    // TextBox1.AppendText(neuralLibControl.GetProgressData().ToString());
                }

            }
         
        }

        protected void OnWindowSizeChanged(object sender, SizeChangedEventArgs e)
        {
           treeView.Height = (e.NewSize.Height - 25)/2;
            treeView1.Height = (e.NewSize.Height - 25)/2;
        }

        private void MenuItem_Add(object sender, RoutedEventArgs e)
        {
            string modName = "";
            ModuleSettingWindow moduleSettingWindow = new ModuleSettingWindow();
            //  moduleSettingWindow.Show();
            bool? diaRes = moduleSettingWindow.ShowDialog();
            if (diaRes.HasValue && diaRes.Value) { 

                modName += moduleSettingWindow.mv.MySelectedItem;

                CustomTreeItem item = ((sender as MenuItem).DataContext) as CustomTreeItem;

                item.Items.Add(new CustomTreeItem() { ItemName = modName });



            }

        }

        private void NeuralSetting_Add(object sender, RoutedEventArgs e)
        {
            string NeurName = "";

            NeuralSettingWindow neuralsettingwindow = new NeuralSettingWindow();

            //  moduleSettingWindow.Show();
            bool? diaRes = neuralsettingwindow.ShowDialog();
            if (diaRes.HasValue && diaRes.Value)
            {
                NeurName += neuralsettingwindow.ns.NeuralName;

                //modName += neuralsettingwindow.nv.MySelectedItem;
                categoriesList.Add(new CustomTreeItem() { ItemName = NeurName, Items = new ObservableCollection<CustomTreeItem>() });
         //       categoriesList.Add(new CustomTreeItem(NeurName));

            }
        }


        private void StartLearn(object sender, RoutedEventArgs e)
        {
            if (FileName != null && categoriesList != null)
            {
                neuralLibControl = new NeuralLibControl();

            }
            else MessageBox.Show("Настройки нейронной сети не установлены");
        }
        private void button1_Click(object sender, RoutedEventArgs e)
        {
            // Create OpenFileDialog 
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();
            dlg.DefaultExt = ".txt";
            dlg.Filter = "TXT Files (*.txt)|*.txt";
            Nullable<bool> result = dlg.ShowDialog();
            if (result == true)
            {
                FileName = dlg.FileName;
                OpenFile(FileName);
                categoriesList.Clear();
                ObservableCollection<CustomTreeItem> ModCollection = new ObservableCollection<CustomTreeItem>();
             //   categoriesList.Add(new CustomTreeItem() { ItemName = "Neural Topology", Items = new ObservableCollection<CustomTreeItem>() });


                foreach (var item in ModuleName)
                {
                    if(item != "Active_L")
                        ModCollection.Add(new CustomTreeItem(item));
                   // categoriesList.Add(new CustomTreeItem(item));
                }
                  categoriesList.Add(new CustomTreeItem() { ItemName = "Neural Topology", Items = ModCollection });
            }
        }
        private void OpenFile(string filename)
        {
            string line = "";
            try
            {
                //Pass the file path and file name to the StreamReader constructor
                StreamReader sr = new StreamReader(filename);

                line = sr.ReadLine();
                while (line != null)
                {
                    if (line.Contains("neural-architecture"))
                    {
                        char[] separators = new char[] { '<', ',' };
                        string[] subs = line.Split(separators, StringSplitOptions.RemoveEmptyEntries);
                        for (var i = 1; i < subs.Length - 1; i++) ModuleName.Add(subs[i]);
                        line = sr.ReadLine();
                    }
                    line = sr.ReadLine();
                }
                //close the file
                sr.Close();
             
            }
            catch (Exception e)
            {
            }
            finally
            {
            }
        }



    }
}
