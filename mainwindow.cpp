#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "polis.h"
#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene();
    ui->gV->setScene(scene);
    UpdateScene();


}


void MainWindow::UpdateScene(){
    scene->clear();
    DrawGrid();

}

void MainWindow::DrawGrid(){
    for (int x = left_x; x <= right_x; x += step){
        scene->addLine(x, left_y, x, right_y, QPen(QColor(Qt::lightGray), 1));
    }
    for (int y = left_y; y <= right_y; y += step){
        scene->addLine(left_x, y, right_x, y, QPen(QColor(Qt::lightGray), 1));
    }

    for (int x = left_x; x <= right_x; x += step_big){
        scene->addLine(x, left_y, x, right_y, QPen(QColor(Qt::black), 1));
    }
    for (int y = left_y; y <= right_y; y += step_big){
        scene->addLine(left_x, y, right_x, y, QPen(QColor(Qt::black), 1));
    }

    scene->addLine(0, left_y, 0, right_y, QPen(QColor(Qt::black), 2));
    scene->addLine(left_x, 0, right_x, 0, QPen(QColor(Qt::black), 2));

    DrawDigits();

}

void MainWindow::DrawDigits(){
    for (int x = left_x; x < right_x; x += step_big){ // Ox
        QGraphicsTextItem *text;
        text = new QGraphicsTextItem(QString::number(x / step));
        text->setPos(x + 5, 2);
        QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(0);
        ellipse->setPos(text->pos());
        ellipse->setRect(text->boundingRect());
        ellipse->setBrush(QBrush(QColor(Qt::white)));
        ellipse->setPen((QPen(QColor(Qt::white))));
        scene->addItem(ellipse);
        scene->addItem(text);
    }
    QGraphicsTextItem *text;
    text = new QGraphicsTextItem(QString::number(right_x / step));
    text->setPos(right_x - 30, 2);
    QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(0);
    ellipse->setPos(text->pos());
    ellipse->setRect(text->boundingRect());
    ellipse->setBrush(QBrush(QColor(Qt::white)));
    ellipse->setPen((QPen(QColor(Qt::white))));
    scene->addItem(ellipse);
    scene->addItem(text);


    for (int y = left_y; y < right_y; y += step_big){  //Oy
        QGraphicsTextItem *text;
        text = new QGraphicsTextItem(QString::number(-y / step));
        text->setPos(5, y + 2);
        QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(0);
        ellipse->setPos(text->pos());
        ellipse->setRect(text->boundingRect());
        ellipse->setBrush(QBrush(QColor(Qt::white)));
        ellipse->setPen((QPen(QColor(Qt::white))));
        scene->addItem(ellipse);
        scene->addItem(text);
    }
    QGraphicsTextItem *text2;
    text2 = new QGraphicsTextItem(QString::number(-right_y / step));
    text2->setPos(5, right_y - 30);
    QGraphicsEllipseItem *ellipse2 = new QGraphicsEllipseItem(0);
    ellipse2->setPos(text2->pos());
    ellipse2->setRect(text2->boundingRect());
    ellipse2->setBrush(QBrush(QColor(Qt::white)));
    ellipse2->setPen((QPen(QColor(Qt::white))));
    scene->addItem(ellipse2);
    scene->addItem(text2);
}

void MainWindow::DrawFuncs(){
    for (int i = 0; i < ui->f_list->count(); ++i){
        QColor color = QColor(rand() % 256, rand() % 256, rand() % 256);
        //ui->f_list->item(i)->setBackground(QBrush(QColor(c1, c2, c3)));
        DrawFunc(ui->f_list->item(i)->text(), color);
    }
}

double MainWindow::Normalize(double a){
    /*if (a < left_y)
        return left_y * 1.0;
    if (a > right_y)
        return right_y * 1.0;*/
    return a;
}

void MainWindow::DrawFunc(QString s, QColor color){
    s = s.split(" ").join("");
    vs tmp = to_vector(s.toStdString());
    check(tmp);
    qs ans = to_polis(tmp);
    double prev = 1e9;
    double x = left_x / step;
    while (x <= right_x / step){
        double y;
        try {
            y = -calc(ans, x);
            if (y * step >= left_y && y * step <= right_y && prev * step >= left_y && prev * step <= right_y){
                if (prev != 1e9){
                    scene->addLine((x - step_x) * step, prev * step, x * step, y * step, QPen(color, 2));
                }
            }
            prev = y;
        } catch (...){
            prev = 1e9;
        }
        x += step_x;
    }
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_f_list_itemChanged(QListWidgetItem *item)
{
    QListWidgetItem *new_item = new QListWidgetItem();
    new_item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui->f_list->addItem(new_item);
    QColor color = QColor(rand() % 256, rand() % 256, rand() % 256);
    //item->setBackground(QBrush(color));
    DrawFunc(item->text(), color);
}

