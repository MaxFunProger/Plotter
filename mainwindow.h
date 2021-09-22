#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QTextItem>
#include <QListWidgetItem>
#include <string>
#include <string.h>
#include <stack>
#include <cmath>
#include <ctype.h>
#include "polis.h"
#include <algorithm>
#include <QColor>
#include <QBrush>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_f_list_itemChanged(QListWidgetItem *item);

private:
    Ui::MainWindow  *ui;
    QGraphicsScene *scene;
    void UpdateScene();
    void DrawGrid();
    void DrawDigits();
    void DrawFuncs();
    void DrawFunc(QString, QColor);
    std::string Poliz(std::string);
    int CalcPoliz(QString, int);
    double Normalize(double);
    int step = 25;
    int step_big = 125;
    double step_x = 0.01;
    int left_x = -1000;
    int right_x = 1000;
    int left_y = -1000;
    int right_y = 1000;

};

#endif // MAINWINDOW_H
