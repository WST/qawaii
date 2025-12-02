#ifndef MARKDOWNPARSER_H
#define MARKDOWNPARSER_H

#include <QString>

class MarkdownParser
{
public:
    static QString toHtml(const QString &markdown);
    static QString toPlainText(const QString &markdown);
};

#endif // MARKDOWNPARSER_H

