#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QString>
#include "Definitions.h"
#include "Player.h"
#include "VideoDefs.h"
#include "ImageDefs.h"
#include <QFileInfo>
#include <QDir>

#include <vector>
#include <string>

namespace oscv
{


    //enum class PlayerType;
    enum class FilePlayerType { Unknown=0, Video=1, Image};


    /** Get path of the given path with file separator if no ending with file separator
     * @param[in/out] filepath path to directory
    */
    inline void getPathWithSeparator(QString& filepath)
    {
        if ( ! filepath.endsWith(FILE_SEPARATOR) ) {
            filepath.append(FILE_SEPARATOR);
        }
    }

    /**
     * @brief playerType get the player type @see PlayerType.
     * @param filename file name with extension
     * @return @see PlayerType
     */
    inline FilePlayerType filePlayerType(const QString& filename)
    {
        if (VideoDefs::isVideoExtension(filename)) {
            return FilePlayerType::Video;
        }
        else if (ImageDefs::isSupportFormat(filename)) {
            return FilePlayerType::Image;
        }
        else {
            return FilePlayerType::Unknown;
        }
    }

    //////////////////////////////////////////////// CLASS //////////////////////////////////////////////

    /**
     * @brief The FileUtils class File utilities to handle anything about files.
     */
    class FileUtils
    {

    public:
    /**
     * @brief getNumberOfFiles count number of files with the same prefix in the same folder.
     *        For example, given: /home/me/images/pic_0000.png, the last images is pic_0199.png
     *        , the folder contain 200 images. 200 is to be returned from this function.
     *        It is used to get number of images/frames during playing images in the "play" mode.
     *
     * @param filename file with path name. If the given filename started with non-zero, eg.
     *        pic_0100.png is given, number of files return will be the last images subtracted with 100.
     *
     * @param digit Digit is the number of digits extension in the images. For example if we have pic_0000.png
     *        the given digit is number of 0  which is 4 (the default) here.
     *
     * @param all true if count all images regardless of starting image sequence.
     *            false if count number of images from the given image to the last in the sequence.
     * @return number of files in the given folder with similar prefix and suffix
     */
    static int getNumberOfSequences(const QString& filename, int digits = 4, bool all=true)
    {
        QFileInfo file(filename);
        QDir dir = file.absoluteDir();
        QStringList filters;
        QString number;
        QString prefix = getNamePrefix(filename, number, digits);
        QString ext = getExtension(filename);
        filters << ext.prepend(prefix.append("*"));
        dir.setNameFilters(filters);
        int total = dir.count();
        if ( ! all ) {
            total -= number.toInt(); // if filename does not start with sequence 0000
        }
        return total;
    }

    /**
     * @brief getExtension filename extension
     * @param filename file
     * @return filename extension
     */
    static QString getExtension(const QString& filename)
    {
          QFileInfo file(filename);
          return file.suffix();
    }

    /**
     * @brief getNamePrefix get prefix of the given filename e.g. img_0001.png
     *        the prefix of file with 4 digits number is "img_"
     * @param filename[in]
     * @param numChars[in] if it is 0, the prefix is the same as the given filename
     * @param number[out] string of the number. From the example, the number return as "0001"
     * @return prefix of the given filename
     */
    static QString getNamePrefix(const QString& filename, QString& number, unsigned int numLastChars=4)
    {
        QFileInfo file(filename);
        QString name = file.fileName();
        QString prefix(name);
        if ( numLastChars != 0 )
        {
           int idx = name.indexOf(".");
           prefix = name.left(idx-numLastChars);
           number = name.mid((idx-numLastChars), numLastChars);
        }
        return prefix;
    }

    static QString getNamePrefix(const QString& filename, unsigned int numLastChars=4)
    {
        QFileInfo file(filename);
        QString name = file.fileName();
        QString prefix(name);
        if ( numLastChars != 0 )
        {
           int idx = name.indexOf(".");
           prefix = name.left(idx-numLastChars);
        }
        return prefix;
    }


    /**
     * @brief getFilePathWithSeparator extract absolute filepath from filename and end with separator
     * @param filename filename
     * @return
     */
    static QString getFilePathWithSeparator(const QString& filename)
    {
          QFileInfo file(filename);
          QString filepath(file.absolutePath());
          getPathWithSeparator( filepath );
          return filepath;
    }


    static std::vector<std::string> getFileNames(const QString& dir, const QStringList& filters)
    {
        std::vector<std::string> fileNames;
        QDir path(dir);
        QFileInfoList files = path.entryInfoList(filters);
        for (QFileInfo f: files ) {
            fileNames.push_back( f.filePath().toStdString() );
        }
        return fileNames;
    }

    }; //end class


} // end namespace
#endif // FILEUTILS_H
