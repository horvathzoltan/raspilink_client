#ifndef IMAINVIEW_H
#define IMAINVIEW_H

#include "models/viewmodel.h"


class IMainView
{
public:
    virtual void set_ConnectionView(const ViewModel::ConnectionR& m) = 0;
    virtual void set_ApiverView(const ViewModel::ApiverViewR& m) = 0;
    virtual void set_FeatureRequestView(const ViewModel::FeatureRequestR& m) = 0;

public:
    virtual void GetConnectionActionTriggered(IMainView *sender) = 0;
    virtual void GetApiverActionTriggered(IMainView *sender) = 0;
    virtual void GetFeatureRequestActionTriggered(IMainView *sender) =0;
};

#endif // IMAINVIEW_H
