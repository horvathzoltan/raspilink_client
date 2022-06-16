#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QString>


namespace ViewModel
{
    struct FeatureRequestR{
        QString msg;
    };

    enum Page{main, connection, media, calls, noChange};
    static QString PageToPageName(Page page)
    {
        switch(page){
        case ViewModel::Page::main: return "tab_main";
        case ViewModel::Page::calls: return "tab_calls";
        case ViewModel::Page::connection: return "tab_connection";
        case ViewModel::Page::media: return "tab_media";
        default: return{};
        }
    }

    struct ConnectionR{

        Page page;
        QString deviceMsg;
        QString mediaMsg;
        QString callsMsg;
    };

    struct ApiverViewR{
        QString msg;
    };

};

#endif // VIEWMODEL_H
