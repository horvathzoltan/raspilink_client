#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QString>


namespace ViewModel
{
    struct FeatureRequestR{
        QString msg;
    };

    struct ConnectionR{
        enum Page{main, caonnection, media, calls};
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
