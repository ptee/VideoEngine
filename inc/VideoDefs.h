#ifndef VIDEODEFS_H
#define VIDEODEFS_H

#include <QSet>

namespace oscv
{
/**
 * @brief The VideoDefs class Video definitions.
 */

class VideoDefs
{
public:
    /*! Maximum numbers of frame readable from video.
     * 10,000 frames is 4000 seconds for frame rate 25fps.
     * The video length should not exceed 1 hour.
    */
    static const unsigned int MAX_NUMBER_OF_FRAMES = 10000;

    static const unsigned int DEFAULT_FRAME_RATE = 25;

    static const int INVALID_FRAME_NUMBER = -1;

    static const int INVALID_FRAME_RATE = -1;

    static const int NULL_VIDEO_CAPTURE = -1;


    static QSet<QString> getVideoExtensions();

    /**
     * @brief isSupportFormat This is just simply a container.
     * For the video files the list is somehow longer and includes names such as XVID,
     * DIVX, H264 or LAGS (Lagarith Lossless Codec).
     * The codecs depend on what you have installed.
     * @param filename
     * @return
     */
    static bool isVideoExtension(const QString& filename);


};

}

#endif // VIDEODEFS_H
