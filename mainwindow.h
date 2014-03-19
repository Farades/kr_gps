#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <qwt_plot.h>
#include <qwt_plot_grid.h>

#include <qwt_legend.h>

#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

#include <qwt_plot_magnifier.h>

#include <qwt_plot_panner.h>

#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>

#include <qwt_plot_renderer.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void init_graph();
    void add_graph(QPolygonF points, QColor color, QString graph_name);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // Создать поле со шкалами для отображения графика
    // (this) - разместить поле на текущем окне
    // #include <qwt_plot.h>
    QwtPlot *d_plot = new QwtPlot( this );
};

#endif // MAINWINDOW_H
