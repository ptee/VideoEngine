
#include "VideoDefs.h"
#include <QSet>
#include <QString>
#include <QFileInfo>

using namespace oscv;


QSet<QString> VideoDefs::getVideoExtensions()
{
    QSet<QString> formats;
    formats.insert("avi");
    formats.insert("mpg");
    formats.insert("mp4");

    return formats;
}

bool VideoDefs::isVideoExtension(const QString& filename)
{

    QFileInfo fileinfo(filename);
    QString ext(fileinfo.suffix());
    QSet<QString> formats=getVideoExtensions();
    if ( formats.contains(ext) ) {
        return true;
    }
    return false;

}

