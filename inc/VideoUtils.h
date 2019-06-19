#ifndef VIDEOUTILS_H
#define VIDEOUTILS_H

#include <QString>

/*! Video to Image, Image to Video conversion
 * Utilities to convert between video and image files.
*/

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>


#include "VideoDefs.h"

namespace oscv
{
class IProgressBar;

/**
 * @brief The VideoUtils class Video utilities functions
 */

class VideoUtils
{
public:


    ~VideoUtils() {}

    /*! Convert video to image files..
     * \param[in] vidFile video file
     * \param[in] imgDir directory to store images (as output)
     * \param[in] imgPrefix prefix of the images
     * \param[in] progress progress dialog for the user interface. It is set to NULL if the caller
     *            does not care for the progress but only want to attain output. The caller is
     *            the owner of the object and has the responsibility to delete the IProgressBar object.
    */
    static bool VidToImg(const QString& vidFile,
                         const QString& imgDir,
                         const QString& imgPrefix,
                         IProgressBar* progress=NULL);

    /*! Get frame rate per second (e.g.PAL = 25fps)
     */
    static inline double getFrameRate(cv::VideoCapture* capture);

    /*! Get index of current frame.
     * \brief If the image does not start from 0000, but has an offset
     *          the first reading image wil has an offset set and it is marked as frame-00000.
    */
    static inline double getCurrentFrame(cv::VideoCapture* capture);

    /*! Number of frames in the video file !!!BUG in OpencV!!!
     * \brief Number of images from the first reading to the end of indexing number
    */
    static inline double getNumberOfFrames(cv::VideoCapture* capture);

    /*!
     * \brief getNumberOfFramesWithLimit this is mainly used for writing images. With limit, it will not
     * overflow the directory with the thousands of image files.
     * Image extracted from the video will be written from e.g.image_00001
     * Maximum index of image reading is 9999 (10,000 images) or about 400 sec or 5-6 mins.
     * \param capture cannot be NULL
     * \param limit maximum number of frames to be read out
     * \return total number of frames if not beyond the limit else return the limit
     */
    static inline int getNumberOfFramesWithLimit(cv::VideoCapture* capture, int limit=VideoDefs::MAX_NUMBER_OF_FRAMES );


    /*! Set specific frame number
        \see getCurrentFrame()
        \return false if go to the specific given frame number failed.
    */
    static inline bool setCurrentFrame( cv::VideoCapture* capture, double frameNumber );


    /*! Get CODEC Type- Int form
     All the video codecs have a unique short name of maximum four characters.
     */
    static inline int getCodec(cv::VideoCapture* capture);

    /*! Get CODEC name
    */
    static inline QString getCodecName(cv::VideoCapture* capture);

};

///////////////////////////////////////INLINE IMPLEMENTATION//////////////////////////////////////////


double VideoUtils::getFrameRate(cv::VideoCapture *capture)
{
    if ( capture == NULL)
        return VideoDefs::NULL_VIDEO_CAPTURE;

    return (int) capture->get(CV_CAP_PROP_FPS);
}


double VideoUtils::getCurrentFrame(cv::VideoCapture* capture)
{
    if ( capture == NULL)
        return VideoDefs::NULL_VIDEO_CAPTURE;

    return capture->get(CV_CAP_PROP_POS_FRAMES);
}

double VideoUtils::getNumberOfFrames(cv::VideoCapture* capture)
{
    if ( capture == NULL)
        return VideoDefs::NULL_VIDEO_CAPTURE;

    // Some say.. this might return the wrong frame count.... BUG in CV
    return capture->get(CV_CAP_PROP_FRAME_COUNT);
}

int VideoUtils::getNumberOfFramesWithLimit(cv::VideoCapture* capture, int limit)
{
    if ( capture == NULL)
        return VideoDefs::NULL_VIDEO_CAPTURE;

    double numberOfFrame = capture->get(CV_CAP_PROP_FRAME_COUNT);
    if ( numberOfFrame > limit ) {
        numberOfFrame = limit;
    }
    return (int) numberOfFrame;
}


bool VideoUtils::setCurrentFrame( cv::VideoCapture* capture, double frameNumber )
{

    if ( frameNumber <= VideoDefs::INVALID_FRAME_NUMBER  || frameNumber > VideoUtils::getNumberOfFrames(capture) ) {
        return false;
    }
    if ( capture == NULL )
        return false;

    return capture->set(CV_CAP_PROP_POS_FRAMES, frameNumber);

}

int VideoUtils::getCodec(cv::VideoCapture* capture)
{
    if ( capture == NULL )
        return VideoDefs::NULL_VIDEO_CAPTURE;

    // Get Codec Type- Int form
    // All the video codecs have a unique short name of maximum four characters.
    // e.g. XVID, DIVX or H264 names.
    // Four characters are four bytes, meaning 32 bits.
    // These four characters are coded in the lower 32 bits of the double.
    // A simple way to throw away the upper 32 bits would be to just convert this value to int:
    return  static_cast<int>(capture->get(CV_CAP_PROP_FOURCC));
}



QString VideoUtils::getCodecName(cv::VideoCapture* capture)
{
    QString codec("");
    int ex = getCodec(capture);
    if ( ex == VideoDefs::NULL_VIDEO_CAPTURE )
        return codec;

    // Transform from int to char via Bitwise operators
    char EXT[] = {(char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0};
    codec = EXT;
    return codec;
}

} // end namespace
#endif // VIDEOUTILS_H
