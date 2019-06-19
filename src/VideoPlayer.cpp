/** ***********************************************************************************************
 * @file VideoPlayer.cpp
 * @brief
 * @author Pattreeya Tanisaro
 */


// Qt
#include <QThread>
#include <QVariant>
#include <QDebug>

// oscv
#include "VideoPlayer.h"
#include "VideoUtils.h"
#include "TimeUtils.h"
#include "ImageUtils.h"


using namespace oscv;
using namespace cv;



// Constructor
VideoPlayer::VideoPlayer(QObject *parent) :
    QThread(parent)
    , m_stop(true)
    , m_isNewVideoLoaded(false)
    , m_Capture(NULL)
    , m_name("")
    , m_speed(Speed::Fast)
{
     qRegisterMetaType<cv::Mat>("cv::Mat");

}


// Destructor
VideoPlayer::~VideoPlayer()
{
    m_mutex.lock();
    m_stop = true;
    if (m_Capture != NULL) {
        m_Capture->release();
        delete m_Capture;
    }

    m_waitCondition.wakeOne();
    //condition.wakeAll();
    m_mutex.unlock();
    wait();

}

// Load video to the memory
bool VideoPlayer::open(QString filename)
{
    if ( m_Capture ) {
        delete m_Capture;
    }
    m_Capture  =  new cv::VideoCapture( filename.toStdString() );
    if ( ! m_Capture->isOpened() ) {
        return false;
    }

    m_name = filename;
    m_frameRate = getFrameRate();
    m_isNewVideoLoaded = true;
    int initFrameNr = 0; // initial image frame to display a video content
    setCurrentFrame( initFrameNr );
    if ( readFrame() )
    {
        m_variant.setValue( m_frame );
        emit newFrame( m_variant, getCurrentFrame() );
        //setCurrentFrame( 1 );
        return true;
    }
    return false;

}



// Playing the video from the start or last pause
void VideoPlayer::play()
{
    if ( ! isRunning() )
    {
      {
        QMutexLocker locker(&m_mutex);
        if ( m_isNewVideoLoaded == true) {
            m_isNewVideoLoaded = false;
        }
        if ( isStopped() ) {
             m_stop = false;
        }
      }
      start(LowPriority);

    }
}

void VideoPlayer::close()
{
    m_mutex.lock();
    m_stop = true;
    m_isNewVideoLoaded = false;
    m_mutex.unlock();
    setCurrentFrame(0);
    //m_waitCondition.wakeOne();
    m_waitCondition.wakeAll();

    wait();

}


// Stop the video but not close
void VideoPlayer::stop(bool stop)
{
    QMutexLocker locker(&m_mutex);
    m_stop = stop;
}

// Overriding of QThread class
void VideoPlayer::run()
{

    bool ok = true;
    int delay = static_cast<int>(m_speed) /m_frameRate;
    while( !m_stop )
    {
        m_mutex.lock();
        ok = readFrame();
        m_mutex.unlock();

        if ( ! ok  ) {
            m_mutex.lock();
            m_stop = true;
            m_mutex.unlock();
            emit donePlay(m_stop);
        }
        if ( ! m_stop ) {
             m_mutex.lock();
             delay = static_cast<int>(m_speed)/m_frameRate;
             m_variant.setValue( m_frame );
             m_mutex.unlock();
             emit newFrame( m_variant, getCurrentFrame() );
        }
        sleepMiliSecond(delay);

    }
}


bool VideoPlayer::go(int relativeFrame)
{
    m_mutex.lock();
    m_stop=true;
    int frameNumber = getCurrentFrame()+relativeFrame;
    m_mutex.unlock();

    if ( frameNumber <= VideoDefs::INVALID_FRAME_NUMBER || frameNumber >= getNumberOfFrames() )
        return false;

    bool ok = setCurrentFrame(frameNumber);

    if ( ok && m_stop == true)
    {
        m_mutex.lock();
        ok = readFrame();
        m_mutex.unlock();
        if (ok )
        {
            m_mutex.lock();
            m_variant.setValue( m_frame );
            m_mutex.unlock();
            emit newFrame( m_variant, getCurrentFrame() );
        }
    }
    return ok;

}


bool VideoPlayer::setCurrentFrame( int frameNumber )
{
    QMutexLocker locker(&m_mutex);
    if (frameNumber <= VideoDefs::INVALID_FRAME_NUMBER || frameNumber >= getNumberOfFrames() ) {
        return false;
    }
    return VideoUtils::setCurrentFrame(m_Capture, frameNumber);
}


int VideoPlayer::getCurrentFrame() const
{
    return (int) VideoUtils::getCurrentFrame(m_Capture)-1;
}


int VideoPlayer::getNumberOfFrames() const
{
    return (int) (VideoUtils::getNumberOfFrames(m_Capture)-1);
}


int VideoPlayer::getFrameRate() const
{
    return (int) VideoUtils::getFrameRate(m_Capture);
}

void VideoPlayer::setSpeed(Speed speed)
{
    QMutexLocker locker(&m_mutex);
    m_speed = speed;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
///                              PRIVATE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////


// Read next frame
bool VideoPlayer::readFrame()
{
    if ( m_Capture == NULL ) {
        return false;
    }    
    if ( m_Capture->read(m_frame) && m_frame.data ) {
        return true;
    }
    return false;

}



/////////////////////////////////////////////////END OF FILE///////////////////////////////////////
