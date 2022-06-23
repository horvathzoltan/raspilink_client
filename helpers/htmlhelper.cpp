#include "htmlhelper.h"
#include "logger.h"

#include <QRegularExpression>

const QString HTMLHelper::divregexp = QStringLiteral(R"(<div.*%1[^-][^>]*>([^<]*)<\/div>)");
const QString HTMLHelper::divregexp2 = QStringLiteral(R"(<div.*%1(?:.*\W.*)>)");
const QString HTMLHelper::divregexp3 = QStringLiteral(R"(<img[\w\W]*src=\"([^\"]*)\"[^>]*>)");
const QString HTMLHelper::timeregexp = QStringLiteral(R"((\d?\d?:\d\d?))");
//(\d?\d?:\d\d?)
//<img[\w\W]*src="([^"]*)"[^>]*>
// <div.*sunrise(?:.*\n.*)>(.*)<\/div>
//<div.*current-weather-short-desc(?:.*\n*.*)>(.*)<\/div>
///current-weather-short-desc

QString HTMLHelper::GetContent(const QString &txt, const QString& regextxt)
{
       QRegularExpression rx(regextxt);
       auto m = rx.match(txt);
       if(!m.hasMatch())return {};
       return m.captured(1);
}

QString HTMLHelper::GetDivContent(const QString &txt, const QString& token)
{
    return GetContent(txt, divregexp.arg(token));
}

QString HTMLHelper::GetImgSrc(const QString &txt)
{
        return GetContent(txt, divregexp3);
}

QTime HTMLHelper::GetTime(const QString &txt)
{
    QString txt3 = GetContent(txt, timeregexp);
    QTime t = QTime::fromString(txt3, "h:m");
    return t;
}

QString HTMLHelper::GetNestedDivContent(const QString &txt, const QString& token)
{
       QRegularExpression rx(divregexp2.arg(token));
       auto m = rx.match(txt);
       if(!m.hasMatch())return {};

       int ix1 = m.capturedStart();
       //auto txt3=txt.mid(ix1,50);
       int i=0;
       //int j=1;
       int ixd=ix1+3;
       while(ixd!=-1){
           ixd=txt.indexOf("div",ixd);
           if(ixd==-1) break;
           //j++;
//           auto txt5=txt.mid(ixd,50);
//           if(j>170){

//               zInfo("hutty");

//           }
           auto c = txt[ixd-1];
           if(c=='<')i++;
           else if(c=='/') i--;
           if(i==0) break;
           ixd+=3;
       }
       //auto txt4=txt.mid(ixd,50);
       auto txt2=txt.mid(ix1,ixd-ix1);
       return txt2;
}
