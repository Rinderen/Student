#ifndef DRAWVIEW_H
#define DRAWVIEW_H

#include <QWidget>
#include <QImage>
#include <QColor>
#include <QPoint>

namespace Ui {
class Widget;
}

class DrawView : public QWidget
{
    Q_OBJECT
public:
    explicit DrawView(QWidget *parent = nullptr);
    ~DrawView() override;


    void mousePressEvent(QMouseEvent *event) override; // 鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override; // 鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event) override; // 鼠标松开事件
    void paintEvent(QPaintEvent *event) override; // 系统开始时调用绘图事件影响绘图区域的每一步操作调用绘图事件
    void resizeEvent(QResizeEvent *event) override;// 在应用程序启动时调整绘制区域，调整事件

    QColor penColor() const {return mPenColor;}
    int penWidth() const {return mPenWidth;}
    QString mouseCoordinateValue();// 坐标值转换成字符串 x,y

signals:
    void sendMouseCoordinateValue(); // 鼠标移动事件，给父窗口发送鼠标

public slots:

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);

    bool mDrawing;// 按下绘图变量
    int mPenWidth;// 笔画宽度
    bool modified;// 涂鸦区域中显示的图像是否有未保存的更改
    QColor mPenColor;
    QImage mImage;
    QPoint mLastPoint;// 在上次按下鼠标或鼠标移动事件时保持光标的位置
    QString posText;
};

#endif // DRAWVIEW_H
