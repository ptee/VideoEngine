#ifndef IMAGEDEFS_H
#define IMAGEDEFS_H

#include <QString>
#include <QSet>

namespace oscv
{

/**
 * @brief The ImageDefs class Image definitions
 */
class ImageDefs
{
public:

    /**
     * Default image extension.
     * We use ".jpg" to append the image file extension.
    */
    static const QString DEFAULT_IMG_EXTENSION;

    /**
     * Image format given as "JPG" or "PNG"
     */
    static const QString DEFAULT_IMG_FORMAT;

    /**
     * @brief NUMBER_OF_IMAGESEQ_DIGITS Max is 9999
     */
    static const int NUMBER_OF_IMAGESEQ_DIGITS = 4;


    /** Default frame rate for video image player or image player
     * This must be set inside the player implementation if framerate function is called.
    */
     static const int DEFAULT_IMAGE_FRAME_RATE = 25;

    /**
     * @brief getSupportFormats list of image formats (extensions of file)
     * @return set of strings for identify the image format such as
     *         "bmp", "png", "tif", "tiff", "jpeg", "jpg", "jp2", "ppm",
     *         "pbm", "ppm", "sr" and "ras".
     */
    static QSet<QString> getSupportFormats();

    /**
     * @brief isSupportFormat
     * @param filename filename with file extension
     * @return true if the given image file name is supported.
     */

    static bool isSupportFormat(QString filename);

};


}

#endif // IMAGEDEFS_H
