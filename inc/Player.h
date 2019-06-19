#ifndef PLAYER_H
#define PLAYER_H

/** ***********************************************************************************************
 * @file Player.h
 * @brief Interface class for the video and image player
 * @author Pattreeya Tanisaro
 */

#include <QThread>
#include <QImage>
#include <QObject>

#include <opencv2/core/core.hpp>


/** \mainpage A library to handle video and image sequence which separate the GUI from the main process. \n 
 * The library depends on Qt to handle the thread and OpenCV2 to handle the video. 
 * We, computer vision people, usually use OpenCV to manipulate the video and images. However, we want to have the full control of the GUI in order to add more elaborated features. Therefore, instead of relying on Highgui in OpenCV, which has several limitations, Qt is preferred.\n
 *
 *  The library allows user to have full control of the video such as jump to the a specific frame, slide on the video frames and changing speed of the video.
 *
 *  @see doc/readme.md <br>
 *  <p>
 *  @see doxygen 1.7.6.
 *  </p>
 */


Q_DECLARE_METATYPE(cv::Mat)
Q_DECLARE_METATYPE(const cv::Mat*)


namespace oscv {

enum class PlayerType { Unknown=0, Video=1, VideoImage, Image};

/**
 * @brief The IPlayer class Base class for all types of Players (Video, VideoImage, Image, ...)
 */
class IPlayer
{

public:
    /**
     * @brief The Speed enum Speed of the @see PlayerType
     */
    enum class Speed { SpeedDn_2x=4000, SpeedDn_1x=2000, Normal=1000, SpeedUp_1x=500, SpeedUp_2x=250, SpeedUp_3x=125, SpeedUp_4x=75, SpeedUp_5x=50, SpeedUp_6x=25, Fast = 0 };

    virtual ~IPlayer() {}

    //! Load a video from memory
    /**
     * @brief open
     * @param filename
     * @return true if open the file successfully.
     */
    virtual bool open(QString filename) = 0;

    /**
     * @brief name
     * @return
     */
    virtual QString name() const = 0;


    /** Play the video from the start (if stop) or continue if (pause)
     *
     */
    virtual void play() = 0;

    /** Stop the video but not close
     */
    virtual void stop(bool stop=true) = 0;

    /** check if the player has been stopped
     */
    virtual bool isStopped() const = 0;

    /** Close the video
     */
    virtual void close() = 0;

    /**
     * @brief go wraps around go forward if relativeFrame is positive
     *        and go backward if relativeFrame is negative
     * @param relativeFrame
     * @return
     */
    virtual bool go(int relativeFrame=1) = 0;

    /**
     * @brief setSpeed set speed of the player  when playing @sa {IPlayer::Speed}
     * @param speed
     */
    virtual void setSpeed(Speed speed) = 0;


    /** Set specific frame number.
     @see getCurrentFrame()
     @see go(int relativeFrame=1)
    */
    virtual bool setCurrentFrame( int frameNumber) = 0;

    /** Frame rate
     */
    virtual int getFrameRate() const = 0;

    /** Get index of current frame
     * If the image does not start from 0000, but has an offset
     * the first reading image wil has an offset set and it is marked as frame-00000.
    */
    virtual int getCurrentFrame() const = 0;

    /** Number of frames in the video file !!!BUG in OpencV!!!
     *  Number of images from the first reading to the end of indexing number
     * Image extracted from the video will be written from video-file-name_00001
     * Maximum index of image reading is 99999 (100,000 images) or about 4000 sec or 1 hour.
    */
    virtual int getNumberOfFrames() const = 0;


};

}

#endif // PLAYER_H
/////////////////////////////////////////////////END OF FILE///////////////////////////////////////
