#include "logger.h"
#include "mainwindow.h"
#include "mainpresenter.h"
#include "macro.h"
#include <QApplication>


int main(int argc, char *argv[])
{
#if defined (STRING) && defined (TARGI)
    auto poj = STRING(TARGI);
    zInfo(QStringLiteral("started ")+poj);
#else
    auto poj=QStringLiteral("manymany");
#endif

    QApplication a(argc, argv);
    QCoreApplication::setApplicationName(poj);

    DoWork::Params params = DoWork::Params::Parse(a);

    MainWindow w;
    MainPresenter p;
    p.init(params);
    p.appendView(&w);
    w.show();
    p.initView(&w);

    return a.exec();
}
