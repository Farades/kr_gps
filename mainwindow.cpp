#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Simulator.h"
#include "myVector.h"
#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Simulator* sim;
    double h = 0, t0 = 0, tk = 0 , iteration = 0;
    double alpha;
    Vector UserPos;

    h= 100;
    t0 = 0;
    tk = 20000;

    UserPos.add(1710671);
    UserPos.add(3046094);
    UserPos.add(5318708);
    UserPos.add(0);
    UserPos.add(0);
    UserPos.add(0);

    alpha = 25;

    sim = new Simulator(h, t0, tk, UserPos, alpha);
    sim->Run();

    int counter = 0;
    int i = 0;
    double t = t0;

    QPolygonF visible_time;
    QPolygonF visible_count;
    QPolygonF phase_vector_x;
    QPolygonF phase_vector_y;
    QPolygonF phase_vector_z;
    QPolygonF user_x;
    QPolygonF user_y;
    QPolygonF user_z;
    QPolygonF LSM_res_x;
    QPolygonF LSM_res_y;
    QPolygonF LSM_res_z;
    QPolygonF user_x_veri;
    QPolygonF user_y_veri;
    QPolygonF user_z_veri;

    while ( t <= tk)
    {
        for (int j = 0; j < 27; j++)
        {
            if (sim->user->VectorToVisibleSatellite[j][i].getValue(8)== 1)
                //видимость спутников
                visible_time << QPointF(sim->user->VectorToVisibleSatellite[j][i].getValue(7), sim->user->VectorToVisibleSatellite[j][i].getValue(6));
            if (sim->user->VectorToVisibleSatellite[j][i].getValue(8) == 1)
                counter++;
        }
        visible_count  << QPointF(t, counter);
        phase_vector_x << QPointF(t, sim->SatPtr[14]->GetResult(i).getValue(3));
        phase_vector_y << QPointF(t, sim->SatPtr[14]->GetResult(i).getValue(4));
        phase_vector_z << QPointF(t, sim->SatPtr[14]->GetResult(i).getValue(5));
        user_x << QPointF(t, sim->user->AllPos[i].getValue(0));
        user_y << QPointF(t, sim->user->AllPos[i].getValue(1));
        user_z << QPointF(t, sim->user->AllPos[i].getValue(2));
        counter = 0;
        t += h;
        i++;
    }

    for (int k = 0; k < 48; k++)
    {
       LSM_res_x << QPointF(k, sim->lsm->RESULT[k].getValue(0));
       if (abs(sim->lsm->RESULT[k].getValue(2) - 5318708) < 3 * sim->lsm->RESULT_MATRIX[k][2][2])
        user_x_veri << QPointF(k, (sim->lsm->RESULT[k].getValue(2) - 5318708));
       LSM_res_y << QPointF(k, sim->lsm->RESULT[k].getValue(1));
       user_y_veri << QPointF(k, -3 * sim->lsm->RESULT_MATRIX[k][2][2]);
       LSM_res_z << QPointF(k, sim->lsm->RESULT[k].getValue(2));
       user_z_veri << QPointF(k, 3 * sim->lsm->RESULT_MATRIX[k][2][2]);
    }

    ui->setupUi(this);

    MainWindow::init_graph();
    MainWindow::add_graph(user_y_veri, Qt::red, "");
    MainWindow::add_graph(user_z_veri, Qt::red, "");
    MainWindow::add_graph(user_x_veri, Qt::blue, "Discrepancy Z");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_graph()
{


    setCentralWidget(d_plot); // привязать поле к границам окна

    d_plot->setTitle( "Confidence interval(Z)" ); // заголовок
    d_plot->setCanvasBackground( Qt::white ); // цвет фона

    // Параметры осей координат
    d_plot->setAxisTitle(QwtPlot::yLeft, "Coordinate's [m]");
    d_plot->setAxisTitle(QwtPlot::xBottom, "iteration");
    d_plot->insertLegend( new QwtLegend() );

    // Включить сетку
    // #include <qwt_plot_grid.h>
    QwtPlotGrid *grid = new QwtPlotGrid(); //
    grid->setMajorPen(QPen( Qt::gray, 2 )); // цвет линий и толщина
    grid->attach( d_plot ); // добавить сетку к полю графика

    // Включить возможность приближения/удаления графика
    // #include <qwt_plot_magnifier.h>
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier(d_plot->canvas());
    // клавиша, активирующая приближение/удаление
    magnifier->setMouseButton(Qt::MidButton);

    // Включить возможность перемещения по графику
    // #include <qwt_plot_panner.h>
    QwtPlotPanner *d_panner = new QwtPlotPanner( d_plot->canvas() );
    // клавиша, активирующая перемещение
    d_panner->setMouseButton( Qt::RightButton );

    // Включить отображение координат курсора и двух перпендикулярных
    // линий в месте его отображения
    // #include <qwt_plot_picker.h>

    // настройка функций
    QwtPlotPicker *d_picker =
        new QwtPlotPicker(
            QwtPlot::xBottom, QwtPlot::yLeft, // ассоциация с осями
    QwtPlotPicker::CrossRubberBand, // стиль перпендикулярных линий
    QwtPicker::ActiveOnly, // включение/выключение
    d_plot->canvas() ); // ассоциация с полем

    // Цвет перпендикулярных линий
    d_picker->setRubberBandPen( QColor( Qt::red ) );

    // цвет координат положения указателя
    d_picker->setTrackerPen( QColor( Qt::black ) );

    // непосредственное включение вышеописанных функций
    d_picker->setStateMachine( new QwtPickerDragPointMachine() );
}

void MainWindow::add_graph(QPolygonF points, QColor color, QString graph_name)
{
    // Кривая
    //#include <qwt_plot_curve.h>
    QwtPlotCurve *curve = new QwtPlotCurve();
    curve->setTitle( graph_name );
    curve->setPen( color, 2 ); // цвет и толщина кривой
    curve->setStyle(QwtPlotCurve::Lines);
    curve->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // сглаживание

    /*// Маркеры кривой
    // #include <qwt_symbol.h>
    QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
        QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 8, 8 ) );
    curve->setSymbol( symbol );*/

    // Добавить точки на ранее созданную кривую


     curve->setSamples( points ); // ассоциировать набор точек с кривой

     curve->attach( d_plot ); // отобразить кривую на графике
}
