#ifndef IMAINVIEW_H
#define IMAINVIEW_H

#include "models/mainviewmodel.h"


class IMainView
{
public:
    virtual MainViewModel::DoWorkModel get_DoWorkModel() =0;
    virtual void set_PhoneView(const MainViewModel::PhoneViewModelR& m) = 0;
    virtual void set_ConnectionView(const MainViewModel::ConnectionViewModelR& m) = 0;
    virtual void set_ApiverView(const MainViewModel::ApiverViewModelR& m) = 0;
    virtual void set_FeatureRequestView(const MainViewModel::FeatureRequestViewModelR& m) = 0;

public: // signals
    virtual void PushButtonActionTriggered(IMainView *sender) = 0;
    virtual void GetConnectionActionTriggered(IMainView *sender) = 0;
    virtual void GetApiverActionTriggered(IMainView *sender) = 0;
    virtual void GetFeatureRequestActionTriggered(IMainView *sender) =0;
};

#endif // IMAINVIEW_H
