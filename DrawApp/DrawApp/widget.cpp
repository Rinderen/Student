#include "widget.h"
#include "ui_widget.h"
#include <QLabel>
#include <QDebug>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    posLabel =new QLabel(this);
    posLabel->setGeometry(0,0,200,20);



    QPalette palette;
    palette.setColor(QPalette::Window,Qt::green);
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    this->show();

    drawView = new DrawView(this);
    palette.setColor(QPalette::Window,Qt::red);
    drawView->setGeometry(50,50,200,200);
    drawView->setAutoFillBackground(true);
    drawView->setPalette(palette);
    drawView->show();
    connect(drawView,&DrawView::sendMouseCoordinateValue,this,&Widget::receivesMouseCoordinateValue);
}

void Widget::receivesMouseCoordinateValue(){// 接收鼠标坐标值
    posLabel->setText(drawView->mouseCoordinateValue());
}


Widget::~Widget()
{
    delete ui;
}

