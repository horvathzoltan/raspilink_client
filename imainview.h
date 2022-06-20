#ifndef IMAINVIEW_H
#define IMAINVIEW_H

#include "models/viewmodel.h"


class IMainView
{
public:
    virtual void set_ConnectionView(const ViewModel::ConnectionR& m) = 0;
    virtual void set_ApiverView(const ViewModel::Apiver& m) = 0;
    virtual void set_FeatureRequestView(const ViewModel::Features& m) = 0;
    virtual void set_MediaView(const ViewModel::Media& m) = 0;
    virtual void set_DeviceView(const ViewModel::Device& m) = 0;
    virtual void set_CallsView(const ViewModel::Calls& m) = 0;
    virtual void set_CurrentWeatherView(const ViewModel::CurrentWeather& m) = 0;

public:
    virtual void GetConnectionActionTriggered(IMainView *sender) = 0;
    virtual void GetApiverActionTriggered(IMainView *sender) = 0;
    virtual void GetFeatureRequestActionTriggered(IMainView *sender) =0;
    virtual void MediaRefreshActionTriggered(IMainView *sender) =0;
    virtual void DeviceRefreshActionTriggered(IMainView *sender) =0;
    virtual void CallsRefreshActionTriggered(IMainView *sender) =0;
    virtual void GetCurrentWeatherActionTriggered(IMainView *sender) =0;
};

#endif // IMAINVIEW_H
