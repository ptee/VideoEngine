#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H
// Qt
#include <QImage>
#include <QString>

// CV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>


namespace oscv
{
    /**
     * @brief The ImageUtils class e.g. Image Conversion etc.
     */
    class ImageUtils
    {
    public:
         enum class IMG_COMPRESSION{PNG, JPG};


        /**
         * @brief MatToQImage convert OpenCV image to QImage
         *
         * This is only for the conversion of RGB 3 channels (RGB888) and gray (Indexed8) image only.
         * Since we don't use 4 channels, otherwise, this must be added in the function.
         * @param frame image in OpenCV as input
         * @param qImg QImage as output
         */

      static  void MatToQImage( const cv::Mat& frame, QImage& qImg);
      static QImage toQImage(const cv::Mat& frame);


      /**
       * @brief QImageToMat convert QImage to cv::Mat in OpenCV
       *
       * This function is currently support for RGB 3 channel and gray image only.
       * To have support for alpha channel, we have to add and test it.
       * @param img
       * @param frame
       */
      static void QImageToMat( const QImage& img, cv::Mat frame);
      static cv::Mat toMat( const QImage& img);

      /**
       * @brief printMat Output Mat data to the debug screen with qDebug()
       * @param mat Mat input
       * @param fromRow started row to be shown
       * @param toRow  last row to be shown
       * @param fromCol started column
       * @param toCol end column
       */
      static void printMat(const cv::Mat& mat, int fromRow, int toRow, int fromCol, int toCol);


      /**
       * @brief calHistogram calculate the histogram of the given image
       * @param img
       * @return image of histogram with the same width and height as image
       */
      static cv::Mat calHistogram( const cv::Mat& img );

      /**
       * @brief write
       * @param complete filename full path with filename and its extension
       * @param img
       * @param compression compression type should corresponding to given filename
       * @return
       */
      static bool write( const QString& filename, const cv::Mat& img, IMG_COMPRESSION compression);


      // return BRG color
      static cv::Vec3b averageColor( const cv::Mat& img, cv::Rect rect );


    };

}

#endif // IMAGEUTILS_H
////////////////////////////////// END OF FILE /////////////////////////////////
