using OxyPlot;
using OxyPlot.Axes;
using OxyPlot.Series;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Animation;

namespace NeuralNetworkUI
{


    public class DataGraph
    {
        OxyPlot.Series.LineSeries GrPoints;
        public PlotModel MyDataModel { get; private set; }
        public DataGraph()
        {
            GrPoints = new OxyPlot.Series.LineSeries();
            this.MyDataModel = new PlotModel { Title = "Матрица ошибок" };

            this.MyDataModel.Axes.Add(new CategoryAxis
            {
                Position = AxisPosition.Bottom,

                // Key used for specifying this axis in the HeatMapSeries
                Key = "data1",

                // Array of Categories (see above), mapped to one of the coordinates of the 2D-data array
                ItemsSource = new[]
                    {
                            "cl 1",
                             "cl 2",
                             "cl 3",
                             "cl 4",
                             "cl 5",
                             "cl 6",
                             "cl 7"
                    }
                });
            this.MyDataModel.Axes.Add(new CategoryAxis
            {
                    Position = AxisPosition.Left,
                    Key = "data2",
                    ItemsSource = new[]
                        {
                                   "cl 1",
                             "cl 2",
                             "cl 3",
                             "cl 4",
                             "cl 5",
                             "cl 6",
                             "cl 7"
                        }
            });

            this.MyDataModel.Axes.Add(new LinearColorAxis
            {
                Palette = OxyPalettes.Hot(200)
            });

            var rand = new Random();
            var data = new double[7,7];
            for (int x = 0; x < 7; ++x)
            {
                for (int y = 0; y < 7; ++y)
                {
                    data[y, x] = rand.Next(100, 200);
                    if (x == y)
                    {
                        data[y, x] = 20;
                    }
                   
                }
            }

            var heatMapSeries = new HeatMapSeries
            {
                X0 = 0,
                X1 = 6,
                Y0 = 0,
                Y1 = 4,
                XAxisKey = "data1",
                YAxisKey = "data2",
                RenderMethod = HeatMapRenderMethod.Rectangles,
                LabelFontSize = 0.2, // neccessary to display the label
                Data = data

            };

            this.MyDataModel.Axes.Add(new OxyPlot.Axes.LinearAxis { Position = OxyPlot.Axes.AxisPosition.Left, Title = "Класс данных" });
            this.MyDataModel.Axes.Add(new OxyPlot.Axes.LinearAxis { Position = OxyPlot.Axes.AxisPosition.Bottom, Title = "Класс данных" });
            this.MyDataModel.Series.Add(heatMapSeries);
        }
    }

    internal class Graph
    {
        OxyPlot.Series.LineSeries GrPoints;
        public PlotModel MyModel { get; private set; }
        public Graph()
        {
            GrPoints = new OxyPlot.Series.LineSeries();
            this.MyModel = new PlotModel { Title = "Обучение нейронной сети" };
            /* 
             //   var model = new PlotModel { Title = "ContourSeries" };

                double x0 = -3.1;
                double x1 = 3.1;
                double y0 = -3;
                double y1 = 3;

                //generate values
                this.MyModel.Axes.Add(new LinearColorAxis
                {
                    Palette = OxyPalettes.Hot(100)
                });
                var data = new double[100, 100];
                for (int x = 0; x < 100; ++x)
                {
                    for (int y = 0; y < 100; ++y)
                    {
                        data[y, x] = x * Math.Pow(y , 0.1);
                    }
                }
                var heatMapSeries = new HeatMapSeries
                {
                    X0 = 0,
                    X1 = 99,
                    Y0 = 0,
                    Y1 = 99,
                    Interpolate = true,
                    RenderMethod = HeatMapRenderMethod.Bitmap,
                    Data = data
                };
                this.MyModel.Series.Add(heatMapSeries);
            */

            this.MyModel.Axes.Add(new CategoryAxis());
            this.MyModel.Series.Add(GrPoints);
            this.MyModel.DefaultFontSize = 20;
            this.MyModel.Axes.Add(new OxyPlot.Axes.LinearAxis { Position = OxyPlot.Axes.AxisPosition.Left, Title = "Значение ошибки" });
            this.MyModel.Axes.Add(new OxyPlot.Axes.LinearAxis { Position = OxyPlot.Axes.AxisPosition.Bottom, Title = "Количество эпох" });
            this.GrPoints.Points.Add(new OxyPlot.DataPoint(0, 1));
        }
        
    }
}
