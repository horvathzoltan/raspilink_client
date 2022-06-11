#ifndef DOWORK_H
#define DOWORK_H

#include <QCoreApplication>
#include "models/mainviewmodel.h"
#include "helpers/httphelper.h"


class DoWork: public QObject
{
    Q_OBJECT

public:
    explicit DoWork(QObject *parent = nullptr);

    struct Params{
    public:
        QString inFile;
        QString outFile;
        bool isBackup;
        bool isTest = false;

        bool IsValid();

        static Params Parse(const QCoreApplication& app);
    };


    bool init(Params p);
    MainViewModel::DoWorkRModel Work1(MainViewModel::DoWorkModel m);
    QString GetCheckin();
private:
    bool _isInited = false;
    //bool _isEventLoopNeeded = false;
    Params params;
    //QMap<QString, Wcode> _wcodes;
    HttpHelper _h;
    //Result doWork2();

signals:
    void ResponseOkAction2(QString);

private slots:
    void ResponseOkAction(QByteArray s);
};

#endif // DOWORK_H
