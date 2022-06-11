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
    void processGetConnectionAction(IMainView *sender);
    void onResponseConnectionAction(CheckinResponseModel);
    void processGetApiverAction(IMainView *sender);
    void onResponseGetApiverAction(QString);
    void processGetFeatureRequestAction(IMainView *sender);
    void onResponseGetFeatureRequestAction(QString);

private:
    bool _isInited = false;
    DoWork _dowork;
//    IMainView *_connectionActionSender = nullptr;
//    IMainView *_apiverActionSender = nullptr;
//    IMainView *_featureRequestActionSender = nullptr;

    QMap<QUuid, IMainView*> _senders;
};

#endif // MAINPRESENTER_H
