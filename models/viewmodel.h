#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include "model.h"

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

    struct State{
        enum ConnectionState {unknown, unchanged, created, changed, deleted};

        ConnectionState deviceState = unknown;
        QString deviceMsg;
        ConnectionState mediaState = unknown;
        QString mediaMsg;
        ConnectionState callsState = unknown;
        QString callsMsg;
    };

    struct ConnectionR{
        Page page;
        State state;
        //QString deviceMsg;
        //QString mediaMsg;
        //QString callsMsg;
    };

    struct ApiverViewR{
        QString msg;
    };

    struct Media{
        Model::Media media;
    };

    struct Device{
        Model::Device device;
    };

    struct Calls{
        Model::Calls calls;
    };
};

#endif // VIEWMODEL_H
