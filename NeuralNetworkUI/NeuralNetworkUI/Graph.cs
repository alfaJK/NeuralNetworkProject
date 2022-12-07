using OxyPlot;
using OxyPlot.Axes;
using OxyPlot.Legends;
using OxyPlot.Series;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics.Metrics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Markup;
using System.Windows.Media.Animation;

namespace NeuralNetworkUI
{


    public class DataGraph : BaseViewModel
    {
        OxyPlot.Series.LineSeries GrPoints;
        public int countclasses;
        public PlotModel MyDataModel { get; private set; }
        public DataGraph()
        {
            GrPoints = new OxyPlot.Series.LineSeries();
            this.MyDataModel = new PlotModel { Title = "Матрица ошибок" };
            countclasses = 2;
            SetUpModel();
        }
        private void SetUpModel()
        {
          /*  this.MyDataModel.Axes.Add(new CategoryAxis
            {
                Position = AxisPosition.Bottom,
                Key = "data1"
            });*/
      

            this.MyDataModel.Axes.Add(new LinearColorAxis
            {
                Palette = OxyPalettes.Jet(200)
            });

            var data = new double[countclasses, countclasses];
            for (int x = 0; x < countclasses; ++x)
            {
                for (int y = 0; y < countclasses; ++y)
                {
                    data[x, y] = 0;
                }
            }
            data[0, 0] = 282;
            data[0, 1] = 81;
            data[1, 0] = 18;
            data[1, 1] = 172;

            var categoryAxis = new CategoryAxis()
            {
                Position = AxisPosition.Left,
                Key = "data1"
            };
            categoryAxis.FontSize = 20;
            categoryAxis.ActualLabels.Add("Positive");
            categoryAxis.ActualLabels.Add("Negative");
          
            var categoryAxis2 = new CategoryAxis()
            {
                Position = AxisPosition.Bottom,
                Key = "data2"
            };
            categoryAxis2.FontSize = 20;
            categoryAxis2.ActualLabels.Add("Positive");
            categoryAxis2.ActualLabels.Add("Negative");



            this.MyDataModel.Axes.Add(categoryAxis);
            this.MyDataModel.Axes.Add(categoryAxis2);
            // this.MyDataModel.Axes.Add(categoryAxis);

            var heatMapSeries = new HeatMapSeries
            {
                X0 = 0,
                X1 = 1,
                Y0 = 0,
                Y1 = 1,
                XAxisKey = "data1",
                YAxisKey = "data2",
                RenderMethod = HeatMapRenderMethod.Rectangles,
                LabelFontSize = 0.2, // neccessary to display the label
                Data = data

            };
            //     this.MyDataModel.Axes.Add(new OxyPlot.Axes.LinearAxis { Position = OxyPlot.Axes.AxisPosition.Left, Title = "Класс данных" });
            //    this.MyDataModel.Axes.Add(new OxyPlot.Axes.LinearAxis { Position = OxyPlot.Axes.AxisPosition.Bottom, Title = "Класс данных" });
            this.MyDataModel.Series.Add(heatMapSeries);

        }

        public void UpdatePoints(int[] inputdata)
        {
            var data = new double[countclasses, countclasses];
            for (int x = 0; x < countclasses; x++)
            {
                for (int y = 0; y < countclasses; y++)
                {
                    data[x, y] = inputdata[x * countclasses + y];
                }
            }

            var heatMapSeries = new HeatMapSeries
            {
                X0 = 0,
                X1 = 1,
                Y0 = 0,
                Y1 = 1,
                XAxisKey = "data1",
                YAxisKey = "data2",
                RenderMethod = HeatMapRenderMethod.Rectangles,
                LabelFontSize = 0.2,
                Data = data

            };

  
            this.MyDataModel.Series.Add(heatMapSeries);
            this.MyDataModel.InvalidatePlot(true);
        }
    }

    internal class Graph : BaseViewModel
    {
        private double xt = 0;
        private double yt = 1;
        private OxyPlot.Series.LineSeries GrPoints;
        private OxyPlot.Series.LineSeries GrPoints2;
        private PlotModel _MyModel;

        public PlotModel MyModel
        {
            get { return _MyModel; }
            set { _MyModel = value; OnPropertyChanged("PlotModel");  }
        }
        public Graph()
        {
            GrPoints = new OxyPlot.Series.LineSeries() { Color = OxyColors.Blue, Title = "топология 1" };
            //GrPoints2 = new OxyPlot.Series.LineSeries() { Color = OxyColors.Red, Title = "топология 2" };
            MyModel = new PlotModel()
            {
                IsLegendVisible = true};

            SetUpModel();
        }

        private void SetUpModel()
        {
            this.MyModel = new PlotModel { Title = "Обучение нейронной сети" };
           // this.MyModel.Axes.Add(new CategoryAxis());
            this.MyModel.DefaultFontSize = 20;
            MyModel.Legends.Add(new Legend()
            {
                LegendPosition = LegendPosition.RightTop,
            });
            this.MyModel.Series.Add(GrPoints);
            //this.MyModel.Series.Add(GrPoints2);
            this.MyModel.Axes.Add(new OxyPlot.Axes.LinearAxis { Position = OxyPlot.Axes.AxisPosition.Left, Title = "Значение ошибки" });
            this.MyModel.Axes.Add(new OxyPlot.Axes.LinearAxis { Position = OxyPlot.Axes.AxisPosition.Bottom, Title = "Количество эпох" });
          //  ty==this.GrPoints.Points.Add(new OxyPlot.DataPoint(0, 1));
            //StreamReader f = new StreamReader("D:\\GIT_RPS\\Just-repository\\Neural Interface\\NeuralNetwork\\NeuralNetwork\\bin\\x64\\Debug\\test.txt");
            StreamReader f = new StreamReader("D:\\GIT_RPS\\AnyProjects\\NeuralNetworkProject\\NeuralNetworkUI\\NeuralNetworkUI\\bin\\Debug\\net6.0-windows\\Weight\\graph_data2.txt");

            double xt = 0;
            double yt = 1;
            UpdatePoints(xt, yt);
               while (!f.EndOfStream)
               {
                   string s = f.ReadLine().ToString();

                   UpdatePoints(xt, yt);
                   xt += 1;
                   yt =  Convert.ToDouble(s);
                   // что-нибудь делаем с прочитанной строкой s
                   //       MessageBox.Show(s);
               }
               f.Close();

          /*  StreamReader f2 = new StreamReader("D:\\GIT_RPS\\AnyProjects\\NeuralNetworkProject\\NeuralNetworkUI\\NeuralNetworkUI\\bin\\Debug\\net6.0-windows\\Weight2\\graph_data2.txt");

            double xt2 = 0;
            double yt2 = 1;
            UpdatePoints2(xt2, yt2);
            while (!f2.EndOfStream)
            {
                string s = f2.ReadLine().ToString();

                UpdatePoints2(xt2, yt2);
                xt2 += 1;
                yt2 = Convert.ToDouble(s);
                // что-нибудь делаем с прочитанной строкой s
                //       MessageBox.Show(s);
            }
            f.Close();*/
            this.MyModel.InvalidatePlot(true);
        }



        public void UpdatePoints(double x , double y)
        {
            //this.MyModel.Series.Clear();//DataSeries.Clear();
            this.GrPoints.Points.Add(new OxyPlot.DataPoint(x, y));
            //this.MyModel.Series.Add(GrPoints);
        //    this.MyModel.InvalidatePlot(true);
        }
        public void UpdatePoints2(double x, double y)
        {
            //this.MyModel.Series.Clear();//DataSeries.Clear();
            this.GrPoints2.Points.Add(new OxyPlot.DataPoint(x, y));
           // this.MyModel.Series.Add(GrPoints2);
    //        this.MyModel.InvalidatePlot(true);
        }


    }

    internal class ROCGraph : BaseViewModel
    {
        private double xt = 0;
        private double yt = 1;
        private OxyPlot.Series.LineSeries GrPoints;
        private OxyPlot.Series.LineSeries GrPoints2;
        private PlotModel _MyModel;

        public PlotModel MyModel
        {
            get { return _MyModel; }
            set { _MyModel = value; OnPropertyChanged("PlotModel"); }
        }
        public ROCGraph()
        {
            GrPoints = new OxyPlot.Series.LineSeries() { Color = OxyColors.Blue , Title = "топология 1"  } ;
         //   GrPoints2 = new OxyPlot.Series.LineSeries() { Color = OxyColors.Red, Title = "топология 2" };
            MyModel = new PlotModel();
            SetUpModel();
        }

        private void SetUpModel()
        {
            this.MyModel = new PlotModel { Title = "ROC-кривая " };
            // this.MyModel.Axes.Add(new CategoryAxis());
            this.MyModel.DefaultFontSize = 20;
            MyModel.Legends.Add(new Legend()
            {
                LegendPosition = LegendPosition.RightBottom,
            });
            this.MyModel.Series.Add(GrPoints);
          //  this.MyModel.Series.Add(GrPoints2);
            this.MyModel.Axes.Add(new OxyPlot.Axes.LinearAxis { Position = OxyPlot.Axes.AxisPosition.Left, Title = "Чувствительность" });
            this.MyModel.Axes.Add(new OxyPlot.Axes.LinearAxis { Position = OxyPlot.Axes.AxisPosition.Bottom, Title = "Специфичность" });
            //  this.GrPoints.Points.Add(new OxyPlot.DataPoint(0, 1));
            StreamReader f = new StreamReader("D:\\GIT_RPS\\AnyProjects\\NeuralNetworkProject\\NeuralNetworkUI\\NeuralNetworkUI\\bin\\Debug\\net6.0-windows\\Weight\\graph_dataspec.txt");
            StreamReader f2 = new StreamReader("D:\\GIT_RPS\\AnyProjects\\NeuralNetworkProject\\NeuralNetworkUI\\NeuralNetworkUI\\bin\\Debug\\net6.0-windows\\Weight\\graph_datasens.txt");
            double xt = 0;
            double yt = 0;
            UpdatePoints(xt, yt);
              while (!f.EndOfStream)
              {
                  string s = f.ReadLine().ToString();
                  string s2 = f2.ReadLine().ToString();
                  UpdatePoints(xt, yt);
                //  xt = Convert.ToDouble(s);
                 // yt = Convert.ToDouble(s2);
                  // что-нибудь делаем с прочитанной строкой s
                  //       MessageBox.Show(s);
              }
            UpdatePoints(1, yt);
            f.Close();
              f2.Close();

        /*    StreamReader f3 = new StreamReader("D:\\GIT_RPS\\AnyProjects\\NeuralNetworkProject\\NeuralNetworkUI\\NeuralNetworkUI\\bin\\Debug\\net6.0-windows\\Weight2\\graph_dataspec.txt");
            StreamReader f4 = new StreamReader("D:\\GIT_RPS\\AnyProjects\\NeuralNetworkProject\\NeuralNetworkUI\\NeuralNetworkUI\\bin\\Debug\\net6.0-windows\\Weight2\\graph_datasens.txt");
            double xt2 = 0;
            double yt2 = 0;
            UpdatePoints2(xt2, yt2);
            while (!f4.EndOfStream)
            {
                string s = f3.ReadLine().ToString();
                string s2 = f4.ReadLine().ToString();
                UpdatePoints2(xt2, yt2);
                xt2 = Convert.ToDouble(s);
                yt2 = Convert.ToDouble(s2);
                // что-нибудь делаем с прочитанной строкой s
                //       MessageBox.Show(s);
            }
            UpdatePoints2(1, yt2);
            f3.Close();
            f4.Close();*/

        }



        public void UpdatePoints(double x, double y)
        {
            //this.MyModel.Series.Clear();//DataSeries.Clear();
            this.GrPoints.Points.Add(new OxyPlot.DataPoint(x, y));
            //this.MyModel.Series.Add(GrPoints);
            //    this.MyModel.InvalidatePlot(true);
        }
        public void UpdatePoints2(double x, double y)
        {
            //this.MyModel.Series.Clear();//DataSeries.Clear();
            this.GrPoints2.Points.Add(new OxyPlot.DataPoint(x, y));
            // this.MyModel.Series.Add(GrPoints2);
            //        this.MyModel.InvalidatePlot(true);
        }


    }

}
