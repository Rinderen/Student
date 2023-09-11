#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>// 语法高亮

class Highlighter : public QSyntaxHighlighter
{
public:
    explicit Highlighter(QTextDocument *parent=nullptr);
protected:
    void highlightBlock(const QString &text);
};

#endif // HIGHLIGHTER_H
