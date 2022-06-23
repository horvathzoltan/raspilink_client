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
    struct MainPresenterInit{
        Settings settings;
    };

    bool init(const MainPresenterInit& m);
private:
    QList<IMainView*> _views;

    ViewModel::Page GetActivePage(ViewModel::State state);

private slots:
    void processGetConnectionAction(IMainView *sender);
    void onResponseConnectionAction(ResponseModel::Checkin);
    void processGetApiverAction(IMainView *sender);
    void onResponseGetApiverAction(ResponseModel::GetApiVer);
    void processGetFeatureRequestAction(IMainView *sender);
    void onResponseGetFeatureRequestAction(ResponseModel::GetFeature);

    void processMediaRefreshAction(IMainView *sender);
    void processDeviceRefreshAction(IMainView *sender);
    void processCallsRefreshAction(IMainView *sender);
    void processGetCurrentWeatherAction(IMainView *sender);
    void onResponseGetCurrentWeatherRequestAction(ResponseModel::GetCurrentWeather);
    void onResponseGetCurrentWeatherIconRequestAction(ResponseModel::GetCurrentWeatherIcon);

private:
    bool _isInited = false;
    DoWork _dowork;
    QMap<QUuid, IMainView*> _senders;

    //Model::Data _data;
};

#endif // MAINPRESENTER_H
