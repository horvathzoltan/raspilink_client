#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QString>


namespace ViewModel
{
    struct FeatureRequestR{
        QString msg;
    };

    enum Page{main, connection, media, calls};

    struct ConnectionR{

        Page page;
        QString deviceMsg;
        QString mediaMsg;
        QString callMsg;
    };

    struct ApiverViewR{
        QString msg;
    };

};

#endif // VIEWMODEL_H
