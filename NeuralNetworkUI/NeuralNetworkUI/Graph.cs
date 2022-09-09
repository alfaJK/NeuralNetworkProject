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


    internal class Graph
    {
        OxyPlot.Series.LineSeries GrPoints;
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
        public PlotModel MyModel { get; private set; }
    }
}
