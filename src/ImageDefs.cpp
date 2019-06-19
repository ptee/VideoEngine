
#include "ImageDefs.h"
#include <QFileInfo>

using namespace oscv;


const QString ImageDefs::DEFAULT_IMG_EXTENSION = ".jpg";

const QString ImageDefs::DEFAULT_IMG_FORMAT = "JPG";

QSet<QString> ImageDefs::getSupportFormats()
{
    QSet<QString> formats;
    formats.insert("bmp");
    formats.insert("pbm");
    formats.insert("pgm");
    formats.insert("ppm");
    formats.insert("sr");
    formats.insert("ras");
    formats.insert("jpg");
    formats.insert("jpeg");
    formats.insert("jpe");
    formats.insert("jp2");
    formats.insert("tiff");
    formats.insert("tif");
    formats.insert("png");
    return formats;
}


bool ImageDefs::isSupportFormat(QString filename)
{
     QFileInfo fileinfo(filename);
     QString ext = fileinfo.suffix();
     QSet<QString> formats=getSupportFormats();
     if ( formats.contains(ext) ) {
         return true;
     }
     return false;
}
