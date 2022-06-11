#ifndef MAINPRESENTER_H
#define MAINPRESENTER_H

#include <QObject>
#include "imainview.h"
#include "dowork.h"

class IMainView;

class MainPresenter : public QObject
{
    Q_OBJECT

public:
    explicit MainPresenter(QObject *parent = nullptr);
    void appendView(IMainView *w);
    void initView(IMainView *w) const;
    bool init(DoWork::Params params);
private:
    QList<IMainView*> _views;

private slots:
    void processPushButtonAction(IMainView *sender);
    void processConnectionAction(IMainView *sender);
    void onResponseOkAction2(QString);

private:
    bool _isInited = false;
    DoWork _dowork;
    IMainView *_connectionActionSender = nullptr;

};

#endif // MAINPRESENTER_H
