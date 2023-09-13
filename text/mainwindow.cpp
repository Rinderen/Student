#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QDebug>
#include<QTextDocument>
#include<QTextFrame>
#include<QTextBlock>
#include<QLineEdit>
#include<QDialog>
#include<QPushButton>
#include<QVBoxLayout>

#include<QDragEnterEvent>
#include<QUrl>
#include<QFile>
#include<QTextStream>
#include<QMimeData>

#include<QTimer>
#include<QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QAction *action_textFrame=new QAction(tr("框架"),this);
    connect(action_textFrame,&QAction::triggered,this,&MainWindow::showTextFrame);
    ui->mainToolBar->addAction(action_textFrame);

    QAction *action_textBlock=new QAction(tr("文本块"),this);
    connect(action_textBlock,&QAction::triggered,this,&MainWindow::showTextBlock);
    ui->mainToolBar->addAction(action_textBlock);

    QAction *action_font=new QAction(tr("字体"),this);
    action_font->setCheckable(true);
    connect(action_font,&QAction::triggered,this,&MainWindow::setTextFont);
    ui->mainToolBar->addAction(action_font);

    QAction *action_textTable=new QAction(tr("表格"),this);
    QAction *action_textList=new QAction(tr("列表"),this);
    QAction *action_textIamge=new QAction(tr("图片"),this);
    connect(action_textTable,&QAction::triggered,this,&MainWindow::insertTable);
    connect(action_textList,&QAction::triggered,this,&MainWindow::insertList);
    connect(action_textIamge,&QAction::triggered,this,&MainWindow::insertImage);
    ui->mainToolBar->addAction(action_textTable);
    ui->mainToolBar->addAction(action_textList);
    ui->mainToolBar->addAction(action_textIamge);

    QAction *action_textFind=new QAction(tr("查找"),this);
    connect(action_textFind,&QAction::triggered,this,&MainWindow::textFind);
    ui->mainToolBar->addAction(action_textFind);

    findDialog=new QDialog(this);       // 创建对话框
    lineEdit=new QLineEdit(findDialog); // 创建行编辑器
    QPushButton *btn=new QPushButton(findDialog); // 创建按钮
    btn->setText(tr("查找下一个"));
    connect(btn,&QPushButton::clicked,this,&MainWindow::findNext);
    QVBoxLayout *layout=new QVBoxLayout;  // 创建垂直布局管理器
    layout->addWidget(lineEdit);       // 添加部件
    layout->addWidget(btn);
    findDialog->setLayout(layout);

    highlighter =new Highlighter(ui->textEdit->document());
    ui->textEdit->append(tr("<h1><font color=red>使用HTML</font>"));

    setAcceptDrops(true);
    ui->dragtextEdit->setAcceptDrops(false);//关闭textEdit触发，使mainwindow触发dropEvent。

    id1=startTimer(1000);
    id2=startTimer(1500);
    id3=startTimer(2200);

    QTimer *timer=new QTimer(this);//创建一个新的定时器
    connect(timer,&QTimer::timeout,this,&MainWindow::timerUpdate);
    timer->start(1000);// 设置溢出时间为1s，并启动定时器

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    QTimer::singleShot(10000,this,&MainWindow::close);//自动关闭 函数运行10s
}

void MainWindow::showTextFrame(){
    QTextDocument *document=ui->textEdit->document();
    QTextFrame *frame=document->rootFrame();
    QTextFrame::iterator it;
    for (it=frame->begin();!(it.atEnd());++it) {
        QTextFrame *childFrame=it.currentFrame();
        QTextBlock childBlock=it.currentBlock();
        if(childFrame)
            qDebug()<<"frame";
        else if(childBlock.isValid())
            qDebug()<<"block:"<<childBlock.text();
    }
}

void MainWindow::showTextBlock(){
    QTextDocument *document=ui->textEdit->document();
    QTextBlock block=document->firstBlock();
    for (int i=0;i<document->blockCount();i++) {
        qDebug()<<tr("文本块%1，文本块首行行号为：%2，长度为：%3，内容为：")
                  .arg(i).arg(block.firstLineNumber()+1).arg(block.length())
               <<block.text();
        block=block.next();
    }
}

void MainWindow::setTextFont(bool checked){
    if(checked){
        QTextCursor cursor=ui->textEdit->textCursor();
        QTextBlockFormat blockFormat;
        blockFormat.setAlignment(Qt::AlignCenter);
        cursor.insertBlock(blockFormat);
        QTextCharFormat charFormat;
        charFormat.setBackground(Qt::lightGray);
        charFormat.setForeground(Qt::blue);
        charFormat.setFont(QFont(tr("宋体"),12,QFont::Bold,true));
        charFormat.setFontUnderline(true);
        cursor.setCharFormat(charFormat);
        cursor.insertText(tr("测试字体"));
    }
    else {

    }
}

void MainWindow::insertTable(){
    QTextCursor cursor=ui->textEdit->textCursor();
    QTextTableFormat format;
    format.setCellSpacing(2);
    format.setCellPadding(10);
    cursor.insertTable(2,2,format);
}
void MainWindow::insertList(){
   QTextListFormat format;
   format.setStyle(QTextListFormat::ListDecimal);
   ui->textEdit->textCursor().insertList(format);
}
void MainWindow::insertImage(){
    QTextImageFormat format;
    format.setName("../debug/nvwushen.PNG");

    ui->textEdit->textCursor().insertImage(format);
}

void MainWindow::textFind(){
    findDialog->show();
}
void MainWindow::findNext(){
    QString string=lineEdit->text();
    bool isfind=ui->textEdit->find(string,QTextDocument::FindBackward);
    if(isfind){
        qDebug()<<tr("行号：%1列号：%2")
                  .arg(ui->textEdit->textCursor().blockNumber())
                  .arg(ui->textEdit->textCursor().columnNumber());
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event){// 拖动进入事件
    if(event->mimeData()->hasUrls()){
        //数据中师傅包含URL
        qDebug()<<"dragEnterEvent";
        event->acceptProposedAction();// 如果是则接收动作
        const QMimeData *mimeData=event->mimeData();// 获取mime数据
        if(mimeData->hasUrls()){
            QList<QUrl>urlList=mimeData->urls();
            //将其中第一个URL表示为本地文件路径
            QString fileName=urlList.at(0).toLocalFile();
            if(!fileName.isEmpty()){
                QFile file(fileName);// 建立QFile对象并且以只读方式打开该文件
                if(!file.open(QIODevice::ReadOnly))return;
                QTextStream in(&file);// 建立文本流
                ui->dragtextEdit->setText(in.readAll());
            }
        }
    }
    else event->ignore();//否则忽略该事件
}


void MainWindow::dropEvent(QDropEvent *event){// 放下事件
    const QMimeData *mimeData=event->mimeData();
    if(mimeData->hasUrls()){
        QList<QUrl>urlList=mimeData->urls();
        //将其中第一个URL表示为本地文件路径
        QString fileName=urlList.at(0).toLocalFile();
        if(!fileName.isEmpty()){
            QFile file(fileName);
            if(!file.open(QIODevice::ReadOnly))return;
            QTextStream in(&file);
            ui->dragtextEdit->setText(in.readAll());
        }
    }
    qDebug()<<"拖放时间触发";
}

void MainWindow::timerEvent(QTimerEvent *event){
    if(event->timerId()==id1){
//        qDebug()<<"timer 1";
    }else if(event->timerId()==id2){
//        qDebug()<<"timer 2";
    }else if(event->timerId()==id3){
//        qDebug()<<"timer 3";
    }else{}
}

void MainWindow::timerUpdate(){//定时器溢出处理
    QTime time=QTime::currentTime();
    QString text=time.toString("hh:mm:ss");
//    qDebug()<<text.length();
    if((time.second()%2)==0){text[2]=' ';text[5]=' ';}// 每隔一秒就将“:”显示为空格
    ui->lcdNumber->display(text);

    int rand=qrand()%300;
    text=QString::number(rand);
    ui->randLcdNumber->display(text);
}

MainWindow::~MainWindow()
{
    delete ui;
}
