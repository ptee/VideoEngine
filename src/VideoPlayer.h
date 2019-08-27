#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H
/** ***********************************************************
 * @file VideoPlayer.h
 * @brief Video player interface.
 * @author Pattreeya Tanisaro
*/

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <QMetaType>
#include <QVariant>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Player.h"
#include "VideoUtils.h"


namespace oscv
{
/**
 * @brief The VideoPlayer class Player for video file
 */


class VideoPlayer : public QThread, public IPlayer
{

    Q_OBJECT

public:

     //! Constructor
     VideoPlayer(QObject *parent = 0);

     //! Destructor
     ~VideoPlayer();


     //! Load a video from memory
     bool open(QString filename);


     //! Play the video from the start (if stop) or continue if (pause)
     void play();

     //! Stop the video but not close
     void stop(bool stop);

     //! Close the video
     void close();

     bool next(int frame);

     bool previous(int frame);

     bool go(int relativeFrame);

     //! check if the player has been stopped
     inline bool isStopped() const;


     inline QString name() const;

     //! Set specific frame number in the video to be read next
     bool setCurrentFrame( int frameNumber);

    //! Frame rate of this video
     int getFrameRate() const;


     //! Get index of current frame
     int getCurrentFrame() const;
     

     //! Number of frames in the video file
     int getNumberOfFrames() const;
     
     //! Set speed of the player
     void setSpeed(Speed speed);

     //! Get current video/image frame
     inline const cv::Mat& getRawFrame() const {

          return m_frame;
      }

signals:

    void newFrame( const QVariant v, int frameNum );

    //! To application
    void donePlay(bool done);

protected:
    //! Override QThread
     void run();


private:
     //! Read frame
     bool readFrame();


private:
    /*! True if video is playing but suddenly receiving a stop signal. Or video is prepared for initialized.
        New video can be loaded with thread start (new thread)
     */
    bool m_stop;

    /*! True if video is "newly loaded", used this flag to set the frame view before user starts press "play".
        False if video is in playing, pause or stop/idle mode.
    */
    bool m_isNewVideoLoaded;

    //! Current video frame
    cv::Mat m_frame;

    //! Frame rate per second
    int m_frameRate;

    //! Image of current frame
    QImage m_img;

    //! Video file name
    QString m_name;

    //! Video capture
    cv::VideoCapture *m_Capture;

    //! CV Mat as RGB frame
    cv::Mat m_RGBframe;

    //! Mutex own by this thread
    QMutex m_mutex;

    //! Synchronization
    QWaitCondition m_waitCondition;

    Speed m_speed;

    QVariant m_variant;

};

///////////////////////////////////////IMPLEMENTATION///////////////////////////////////////


bool VideoPlayer::isStopped() const
{
    return m_stop;
}

QString VideoPlayer::name() const
{
    return m_name;
}



} // end namespace


#endif // VIDEOPLAYER_H
