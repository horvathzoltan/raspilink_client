#ifndef MAINPRESENTER_H
#define MAINPRESENTER_H

#include <QObject>
#include <models/responsemodel.h>
#include "imainview.h"
#include "models/commandlineargs.h"
#include "dowork.h"

class IMainView;

class MainPresenter : public QObject
{
    Q_OBJECT

public:
    explicit MainPresenter(QObject *parent = nullptr);
    void appendView(IMainView *w);
    void initView(IMainView *w) const;
    bool init(CommadLineArgs params);
private:
    QList<IMainView*> _views;

private slots:
    void processGetConnectionAction(IMainView *sender);
    void onResponseConnectionAction(ResponseModel::Checkin);
    void processGetApiverAction(IMainView *sender);
    void onResponseGetApiverAction(ResponseModel::GetApiVer);
    void processGetFeatureRequestAction(IMainView *sender);
    void onResponseGetFeatureRequestAction(ResponseModel::GetFeature);

private:
    bool _isInited = false;
    DoWork _dowork;
    QMap<QUuid, IMainView*> _senders;
};

#endif // MAINPRESENTER_H
