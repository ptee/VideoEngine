#ifndef IMAGEPLAYER_H
#define IMAGEPLAYER_H


/** ***********************************************************************************************
 * @file ImagePlayer.h
 * @brief Interface class for playing a sequence of images in the given folder.
 * @author Pattreeya Tanisaro
 */

// cv
#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//

// Qt
#include <QObject>
#include <QThread>
#include <QString>
#include <QImage>
#include <QMutex>
#include <QWaitCondition>
#include <QMetaType>
#include <QVariant>

// oscv
#include "Player.h"
#include "VideoDefs.h"



namespace oscv
{

/**
 * @brief The ImagePlayer class Player for images, not relying on VideoCapture.
 */

class ImagePlayer : public QThread, public IPlayer
{
    Q_OBJECT

public:
    ImagePlayer(QObject *parent = 0);

    ~ImagePlayer();

    void close();

    bool open(QString filename);

    void play();

    bool go(int relativeFrame);

    inline void stop(bool stop);

    inline bool isStopped() const;

    inline QString name() const;

    inline bool setCurrentFrame( int frameNumber);

    inline int getFrameRate() const;

    inline int getCurrentFrame() const;

    inline int getNumberOfFrames() const;

    inline void setSpeed(Speed speed);


protected:

    void run();

signals:

    void newFrame( const QVariant v, int frameNum );

   void donePlay(bool done);


private:

   bool init(QString filename);

   bool readFrame();


   bool m_stop;
   cv::Mat m_frame;
   QImage m_img;
   QString m_name; // current image name with full path..
   QString m_filepath;
   QString m_prefix;
   QString m_fileExt;
   int m_frameRate;
   int m_frameNumber;
   int m_totalFrames;
   Speed m_speed;
   QMutex m_mutex;
   QWaitCondition m_waitCondition;
   QVariant m_variant;


};

///////////////////////////////////////////////IMPLEMENTATION/////////////////////////////////////////
///

bool ImagePlayer::isStopped() const
{
    return m_stop;
}

void ImagePlayer::stop(bool stop)
{
    m_stop = stop;
}

QString  ImagePlayer::name() const
{
     return m_name;
}

bool ImagePlayer::setCurrentFrame( int frameNumber)
{
    QMutexLocker locker(&m_mutex);
    if ( frameNumber <= VideoDefs::INVALID_FRAME_NUMBER || frameNumber > m_totalFrames ) {
        return false;
    }
    m_frameNumber = frameNumber;
    return true;
}

int ImagePlayer::getFrameRate() const
{
    return VideoDefs::DEFAULT_FRAME_RATE;
}

int ImagePlayer::getCurrentFrame() const
{
    return m_frameNumber;
}

int ImagePlayer::getNumberOfFrames() const
{
    return m_totalFrames;
}

void ImagePlayer::setSpeed(Speed speed)
{
    QMutexLocker locker(&m_mutex);
    m_speed = speed;

}


}
#endif // IMAGEPLAYER_H
