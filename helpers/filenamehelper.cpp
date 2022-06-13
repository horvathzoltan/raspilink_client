#include "filenamehelper.h"

#include <QFileInfo>
#include <QDir>


QString FileNameHelper::GetAbsolutFn(const QString& fn){
    QFileInfo fi(fn);

    if(fi.isAbsolute())
        return fn;
    return QDir::homePath()+'/'+ fn;
}
