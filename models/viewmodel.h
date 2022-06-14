#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QString>


namespace ViewModel
{
    struct FeatureRequestR{
        QString msg;
    };

    struct ConnectionR{
        QString deviceMsg;
        QString mediaMsg;
    };

    struct ApiverViewR{
        QString msg;
    };

};

#endif // VIEWMODEL_H
