#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pixmap=new QPixmap(100,100);
    pixmap->fill(Qt::gray);
    QPainter painter(pixmap);
    painter.drawRect(10, 10, 80, 80);
    painter.drawText(20, 30, "Hello World");
        // 使用 pixmap

    ui->drawLab->setPixmap(*pixmap);
    qDebug()<<ui->drawLab->size();
}

MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::paintEvent(QPaintEvent *) {
//    //    QPainter painter(this); // this 是 PandoraWidget 的指针
//    //    painter.setPen(Qt::gray);
//    //    painter.setBrush(Qt::green);
//    //    painter.drawRect(10, 10, 50, 50);
//}
