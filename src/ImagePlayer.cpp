#include "ImagePlayer.h"

// Qt
#include <QApplication>
#include <QMutexLocker>

// oscv
#include "VideoDefs.h"
#include "FileUtils.h"
#include "StringUtils.h"
#include "TimeUtils.h"
#include "ImageUtils.h"
// cv
#ifdef OPENCV_3
#include <opencv2/highgui.hpp> //imread
#else
#include <opencv2/highgui/highgui.hpp>
#endif

using namespace oscv;

ImagePlayer::ImagePlayer(QObject *parent)
    : QThread(parent)
    , m_stop(true)
    , m_name("")
    , m_filepath("")
    , m_prefix("")
    , m_fileExt("")
    , m_frameNumber(0)
    , m_totalFrames(0)
    , m_speed(Speed::Fast)
{
      qRegisterMetaType<cv::Mat>("cv::Mat");
}

ImagePlayer::~ImagePlayer()
{
    close();
}

void ImagePlayer::close()
{
    m_mutex.lock();
    m_stop = true;
    m_frameNumber = 0;
    init("");
    m_mutex.unlock();
    m_waitCondition.wakeOne();
    //condition.wakeAll();
    wait();
}



bool ImagePlayer::open(QString filename)
{
    bool ok = init(filename);
    ok = ok && readFrame();
    if ( ok )
    {
        m_variant.setValue( m_frame );
        emit newFrame( m_variant, m_frameNumber );
    }
    return ok;
}

void ImagePlayer::play()
{
    if ( ! isRunning() )
     {
         {
            QMutexLocker locker(&m_mutex);
            if ( isStopped() ) {
              m_stop = false;
            }
         }
         start(LowPriority);
     }
}




bool ImagePlayer::go(int relativeFrame)
{
  m_mutex.lock();
  int frameNumber = m_frameNumber +relativeFrame;
  m_mutex.unlock();

  bool ok = setCurrentFrame(frameNumber);
  if ( ok )
  {
        m_mutex.lock();
        ok = readFrame();
        m_mutex.unlock();
        if (ok )
        {
            m_mutex.lock();
            m_variant.setValue( m_frame );
            m_mutex.unlock();
            emit newFrame( m_variant, m_frameNumber );
        }
  }
  return ok;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Protected
///

void ImagePlayer::run()
{
    int delay = static_cast<int>(m_speed)/getFrameRate();
    bool ok = true;
    while( !m_stop )
    {
        m_mutex.lock();
        m_frameNumber++;
        ok = readFrame( );
        m_mutex.unlock();
        if ( !ok ) {
              m_mutex.lock();
              m_stop = true;
              m_mutex.unlock();
             emit donePlay(m_stop);

        }
        if ( ! m_stop ) {
             m_mutex.lock();
             delay = static_cast<int>(m_speed)/getFrameRate();
             m_variant.setValue( m_frame );
             m_mutex.unlock();
             emit newFrame( m_variant, m_frameNumber );
        }
        sleepMiliSecond(delay);
    }

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////PRIVATE/////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

// filename must be exist before calling this function
bool ImagePlayer::init(QString filename)
{
    m_name = filename;
    m_filepath = FileUtils::getFilePathWithSeparator(filename);
    QString frameNumber;
    m_prefix = FileUtils::getNamePrefix(filename, frameNumber);
    m_frameNumber = frameNumber.toInt();
    m_totalFrames = FileUtils::getNumberOfSequences(filename);
    m_fileExt = FileUtils::getExtension(filename);

    return ( !m_name.compare("")? false: true );
}


bool ImagePlayer::readFrame()
{

    if ( m_frameNumber > m_totalFrames ) {
        return false;
    }
   QString num;
   beautifyNumberToString(m_frameNumber, ImageDefs::NUMBER_OF_IMAGESEQ_DIGITS, num);
   m_name = m_filepath;
   m_name.append(m_prefix).append(num).append(".").append(m_fileExt);
   m_frame = cv::imread(m_name.toStdString());
   if ( m_frame.data == 0 || m_frame.data == nullptr ) {
       return false;
   }

   return true;
}


///////////////////////////////////////////////////END OF FILE////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
