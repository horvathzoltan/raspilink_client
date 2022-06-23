#ifndef HTMLHELPER_H
#define HTMLHELPER_H

#include <QString>
#include <QTime>

class HTMLHelper
{
private:
    static const QString divregexp;
    static const QString divregexp2;
    static const QString divregexp3;
    static const QString timeregexp;
    static QString GetContent(const QString &txt, const QString& regextxt);
public:
    static QString GetDivContent(const QString& txt, const QString& token);
    static QString GetImgSrc(const QString& txt);
    static QString GetNestedDivContent(const QString& txt, const QString& token);
    static QTime GetTime(const QString &txt);
};

#endif // HTMLHELPER_H
