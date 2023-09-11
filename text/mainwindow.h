#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "highlighter.h"

namespace Ui {
class MainWindow;
}

class QLineEdit;
class QDialog;
class Highlighter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showTextFrame();
    void showTextBlock();
    void setTextFont(bool checked);

    void insertTable();
    void insertList();
    void insertImage();

    void textFind();
    void findNext();

    void timerUpdate();
protected:
    void dragEnterEvent(QDragEnterEvent *event);// 拖入进入事件
    void dropEvent(QDropEvent*event);// 放下事件
    void timerEvent(QTimerEvent *event);

private:
    QLineEdit *lineEdit;
    QDialog *findDialog;
    Highlighter *highlighter;
    int id1,id2,id3,id4;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
