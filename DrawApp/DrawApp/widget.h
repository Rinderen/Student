#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include "drawview.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
    void receivesMouseCoordinateValue();// 接收鼠标坐标值

private:
    Ui::Widget *ui;
    DrawView *drawView;
    QLabel *posLabel;
};

#endif // WIDGET_H
