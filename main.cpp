#include "logger.h"
#include "mainwindow.h"
#include "mainpresenter.h"
#include "macro.h"
#include "models/commandlineargs.h"
#include <QApplication>
#include <models/settings.h>
#include "logger.h"

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

    CommadLineArgs params = CommadLineArgs::Parse(a);

    Settings settings = Settings::Load(params.working_dir);
    if(!settings.isValid())
    {
        zInfo("cannot load settings");
        return 1;
    }

    MainWindow w;
    MainPresenter p;
    p.init({settings});
    p.appendView(&w);
    w.show();
    p.initView(&w);

    return a.exec();
}
