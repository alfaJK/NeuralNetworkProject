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
using Microsoft.VisualBasic.Devices;
using Newtonsoft.Json;
using OxyPlot.Series;

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
           
      
          /*  switch (ModuleName)
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

            }*/
                ItemName = ModuleName;
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
        public double MCC;
        public double TPR;
        public double FPR;
        private List<ModuleSetting> _ModuleSettings;
        public ObservableCollection<NeuralSetting> SetCollection;
        NeuralLibControl neuralLibControl;
        Graph graph;
        ROCGraph graph2;
        double x;
        double error;
        DataGraph dataGraph;
        System.Windows.Threading.DispatcherTimer dispatcherTimer;
        public ObservableCollection<CustomTreeItem> categoriesList { get; set; }
        public MainWindow()
        {
            x = 0;
            error = 0;
            TPR = 0;
            FPR = 0;
            dataGraph = new DataGraph();
            graph  = new Graph();
            graph2 = new ROCGraph();
            InitializeComponent();
            this.SizeChanged += OnWindowSizeChanged;
            SetCollection = new ObservableCollection<NeuralSetting>();
            _ModuleSettings = new List<ModuleSetting>();
            dispatcherTimer = new System.Windows.Threading.DispatcherTimer();
            dispatcherTimer.Tick += new EventHandler(dispatcherTimer_Tick);
            dispatcherTimer.Interval = new TimeSpan(0, 0, 10);
            dispatcherTimer.Start();
            categoriesList = new ObservableCollection<CustomTreeItem>() { };
            this.Resources.Add("mods", graph.MyModel);
            this.Resources.Add("mods2", graph2.MyModel);
            this.Resources.Add("Datamods", dataGraph.MyDataModel);
            this.DataContext = this;
            ProgressLearn.Maximum = 400;
        }


        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            if(neuralLibControl != null)
            {
                if (neuralLibControl.IsInit)
                {

                    textbox1.Clear();

                    textbox1.AppendText(neuralLibControl.GetProgressData().ToString());
                    ProgressLearn.Value = neuralLibControl.GetProgressData();
                    if (x != neuralLibControl._GetEpoh())
                    {
                        x = neuralLibControl._GetEpoh();
                       // TextBox2.Clear();
                        error = neuralLibControl._GetError();
                        //TextBox2.AppendText(error.ToString());

                        graph.UpdatePoints(x, error);
                        graph_2.InvalidatePlot(true);
                        
                        neuralLibControl._GetConfMatrix();
                     /*   try{ TPR = (double)neuralLibControl.ConfMatrixData[0] / (neuralLibControl.ConfMatrixData[0] + neuralLibControl.ConfMatrixData[1]); }
                        catch (DivideByZeroException) { TPR = 0;}
                        try { FPR = (double)neuralLibControl.ConfMatrixData[2] / (neuralLibControl.ConfMatrixData[2] + neuralLibControl.ConfMatrixData[3]); }
                        catch (DivideByZeroException) { FPR = 0; }*/
                        graph2.UpdatePoints(neuralLibControl._GetSpec(), neuralLibControl._GetSens());
                        graph_3.InvalidatePlot(true);

                        dataGraph.UpdatePoints(neuralLibControl.ConfMatrixData);
                        graph_1.InvalidatePlot(true);

                    }
                    


                }

            }
         
        }

        protected void OnWindowSizeChanged(object sender, SizeChangedEventArgs e)
        {
            treeView.Height = (e.NewSize.Height - 25)/2;
            treeView1.Height = (e.NewSize.Height - 25)/2;
            panel_1.Width = (e.NewSize.Width - leftbar.Width) / 2;
            panel_2.Width = (e.NewSize.Width - leftbar.Width) / 2.2;
            graph_1.Width = panel_2.Width;
            graph_1.Height = panel_2.Width/2;

            graph_2.Width = panel_2.Width;

            graph_3.Width = panel_1.Width;
            graph_3.Height = panel_1.Width;

        }

        private void MenuItem_Add(object sender, RoutedEventArgs e)
        {
            string modName = "";
            ModuleSettingWindow moduleSettingWindow = new ModuleSettingWindow();
            //  moduleSettingWindow.Show();
            bool? diaRes = moduleSettingWindow.ShowDialog();
            if (diaRes.HasValue && diaRes.Value) {
                moduleSettingWindow.mv.GetSettings();
                modName += moduleSettingWindow.mv.MySelectedItem;
                _ModuleSettings.Add(moduleSettingWindow.mv._ModuleSetting);
                CustomTreeItem item = ((sender as MenuItem).DataContext) as CustomTreeItem;
                
                item.Items.Add(new CustomTreeItem() { ItemName = moduleSettingWindow.mv._ModuleSetting.ModuleName });
            }

        }

        private void NeuralSetting_Add(object sender, RoutedEventArgs e)
        {
            NeuralSettingWindow neuralsettingwindow = new NeuralSettingWindow();
            bool? diaRes = neuralsettingwindow.ShowDialog();
            if (diaRes.HasValue && diaRes.Value)
            {
                SetCollection.Clear();
                SetCollection.Add(neuralsettingwindow.ns.NeuralSettingStruct);
                categoriesList.Add(new CustomTreeItem() { ItemName = SetCollection[0].NeuralName, Items = new ObservableCollection<CustomTreeItem>() });

            }
        }


        private void StartLearn(object sender, RoutedEventArgs e)
        {
            if(SetCollection.Count != 0) {
                neuralLibControl = new NeuralLibControl(SetCollection[0] , _ModuleSettings.ToArray());
            }
            else MessageBox.Show("Настройки нейронной сети не установлены");
        }


        private void SaveSetting(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.FolderBrowserDialog dlg = new FolderBrowserDialog();
            DialogResult result = dlg.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK && !string.IsNullOrWhiteSpace(dlg.SelectedPath))
            {
                using (StreamWriter file = File.CreateText(dlg.SelectedPath + "\\NeuralSetting.json"))
                {
                    JsonSerializer serializer = new JsonSerializer();
                    serializer.Serialize(file, SetCollection[0]);
                }
                using (StreamWriter file = File.CreateText(dlg.SelectedPath + "\\ModSetting.json"))
                {
                    JsonSerializer serializer = new JsonSerializer();
                    serializer.Serialize(file, _ModuleSettings);
                }
 
            }
        }

        private void LoadSetting(object sender, RoutedEventArgs e)
        {

            categoriesList.Clear();

            System.Windows.Forms.FolderBrowserDialog dlg = new FolderBrowserDialog();
            DialogResult result = dlg.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK && !string.IsNullOrWhiteSpace(dlg.SelectedPath))
            {
                using (StreamReader file = File.OpenText(dlg.SelectedPath + "\\NeuralSetting.json"))
                {
                    JsonSerializer serializer = new JsonSerializer();
                    NeuralSetting NeuralJson = (NeuralSetting)serializer.Deserialize(file, typeof(NeuralSetting));
                    SetCollection.Clear();
                    SetCollection.Add(NeuralJson);
                }
                ObservableCollection<CustomTreeItem> ModCollection = new ObservableCollection<CustomTreeItem>();
                _ModuleSettings.Clear();
                _ModuleSettings = JsonConvert.DeserializeObject<List<ModuleSetting>>(File.ReadAllText(dlg.SelectedPath + "\\ModSetting.json"));
                for(int i = 0; i< _ModuleSettings.Count; i++)  
                    ModCollection.Add(new CustomTreeItem(_ModuleSettings[i].ModuleName));
                 categoriesList.Add(new CustomTreeItem() { ItemName = SetCollection[0].NeuralName, Items = ModCollection });
            }
        }
    }
}
