#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent):QSyntaxHighlighter (parent)
{

}

void Highlighter::highlightBlock(const QString &text){
    QTextCharFormat font;
    font.setFontWeight(QFont::Bold);
    font.setForeground(Qt::green);
    QString pattern="\\bchar\b";//要匹配的字符，这里是“char”单词
    QRegExp expression(pattern);//创建正则表达式
    int index=text.indexOf(expression);//从0位置开始匹配字符串
    while (index>=0) {
        int length=expression.matchedLength();//要匹配的字符串长度
        setFormat(index,length,font);//对要匹配的字符串设置格式
        index=text.indexOf(expression,index+length);
    }
}
