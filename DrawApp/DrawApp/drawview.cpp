#include "drawview.h"
#include <QLabel>
#include <QtWidgets> // 两个解决头文件相互包含

DrawView::DrawView(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(this->windowFlags()|Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    setAttribute(Qt::WA_StaticContents);
    this->mDrawing=false;
    this->modified=false;
    this->mPenColor=Qt::black;
    this->mPenWidth=2;

}

void DrawView::mousePressEvent(QMouseEvent *event)// 鼠标按下事件
{
    if(event->button()==Qt::LeftButton&&mDrawing==false){
        mLastPoint = event->pos();
        drawLineTo(event->pos());
        mDrawing=true;
    }
}
void DrawView::mouseMoveEvent(QMouseEvent *event) // 鼠标移动事件
{
    if((event->buttons()&Qt::LeftButton)&&mDrawing){
        drawLineTo(event->pos());
        posText=QString::number(event->pos().x())+","+QString::number(event->pos().y());
        emit sendMouseCoordinateValue();
    }
}
void DrawView::mouseReleaseEvent(QMouseEvent *event) // 鼠标松开事件
{
    if(event->button()&&mDrawing){
        mDrawing=false;
    }
}
// 系统开始时、放大缩小、影响绘图区域的每一步操作，调用绘图事件
void DrawView::paintEvent(QPaintEvent *event)// 绘图事件
{
    QPainter painter(this); // QPainter是执行绘图的一个类，接收绘图区域
    QRect dirtyRect = event->rect(); // 使用整数精度在平面上定义了一个矩形
    painter.drawImage(dirtyRect, mImage, dirtyRect); //
}
//界面初始化拉伸界面放大缩小会调用
void DrawView::resizeEvent(QResizeEvent *event)// 调整事件
{
    if (width() > mImage.width() || height() > mImage.height()) {
        int newWidth = qMax(width() + 128, mImage.width());
        int newHeight = qMax(height() + 128, mImage.height());
        resizeImage(&mImage, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void DrawView::drawLineTo(const QPoint &endPoint){// 画任一点
    QPainter painter(&mImage);
    painter.setPen(QPen(mPenColor, mPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(mLastPoint, endPoint);
    modified = true;

    int rad = (mPenWidth / 2) + 2;
    update(QRect(mLastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    mLastPoint = endPoint;
}

void DrawView::resizeImage(QImage *image, const QSize &newSize){
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

QString DrawView::mouseCoordinateValue(){// 坐标值转换成字符串 x,y
    return posText;
}



DrawView::~DrawView(){
//    delete ui;
}
