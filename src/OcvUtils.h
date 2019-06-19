#ifndef OCVUTILS_H
#define OCVUTILS_H

// CV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>


namespace oscv
{
    class OcvUtils
    {
    public:
        /**
         * @brief ShiftDft Rearrange the quadrants of Fourier Mat image so that the viewing in frequency domain becomes more meaningful
         *       The bottom-left of the image will be swapped between quater 0 and 3 and between 1 and 2.
         *
         * @param src as in/out
         */
        static inline void ShiftDft(cv::Mat& src);


        /**
         * @brief convolveDFT
         * @param inA
         * @param inB
         * @param out
         */
        static void convolveDFT(cv::Mat& inA, cv::Mat& inB, cv::Mat& out);

        /**
         * @brief cosine Cos of cv::Mat angle
         * @param angle angle as radian if angle is degree, the isDegree must be set.
         * @param out Cos of angle in cv::Mat
         * @param isDegree default is false (radian)
         */

        template<typename T>
        static void cosine(const cv::Mat_<T>& angle, cv::Mat_<T>& out, bool isDegree=false);

        /**
         * @brief sine Sin of cv::Mat angle
         * @param angle
         * @param out
         * @param isDegree
         */
         template<typename T>
        static void sine(const cv::Mat_<T>& angle, cv::Mat_<T>& out, bool isDegree=false);

        /**
         * @brief chopEdgeOff Chop the edge of the image in x and y dimension and reset to the given value
         *          If the given image is in RGB mode, all 3 channels will be reset.
         * @param img
         * @param percent percent of the image width & height to be chop off
         * @param value value to be reset, if this is an RGB, all 3 channels will be reset with the same value.
         */
        static void chopEdgeOff( cv::Mat& img, int percent=2, uchar value=0);


        /**
         * @brief matTypeAsString Get readable cv::Mat's type as string.
         *       The output will be e.g. 8UC3 64FC1 and so on.
         * @param type Type of Mat acquired from Mat.type() which given integer.
         * @param typeAsString Output of type as string
         */
        static inline void matTypeAsString(int type, std::string& typeAsString );

        /**
         * @brief stretchImage Similar to equalizeHist but this function can perform on any Matrix not
         *              confined only to input 8UC1.
         * @param img
         * @param out
         * @param a_min
         * @param a_max
         * @param filterMin percent of  minimum threshold value to remove noise
         * @param filterMax percent of maximum threshould valule to remove noise
         */
        template<typename T>
        static bool stretchImage(const cv::Mat_<T>& img, cv::Mat_<uchar>& out, int a_min=0, int a_max=255, double filterMin=10, double filterMax=10);


    };

     /////////////////////////////INLINE

    void OcvUtils::ShiftDft(cv::Mat& src)
    {
        int cx = src.cols/2;
        int cy = src.rows/2;

        // rearrange the quadrants of Fourier image
        // so that the origin is at the image center
        cv::Mat tmp;
        // Rect started at bottom-left as (x,y), whereas cx, cy is its widtht and height.
        cv::Mat q0(src, cv::Rect(0, 0, cx, cy));
        cv::Mat q1(src, cv::Rect(cx, 0, cx, cy));
        cv::Mat q2(src, cv::Rect(0, cy, cx, cy));
        cv::Mat q3(src, cv::Rect(cx, cy, cx, cy));

        q0.copyTo(tmp);
        q3.copyTo(q0);
        tmp.copyTo(q3);

        q1.copyTo(tmp);
        q2.copyTo(q1);
        tmp.copyTo(q2);
    }

    void OcvUtils::matTypeAsString(int type, std::string& typeAsString )
    {
        // see types_c.h
        uchar depth = type & CV_MAT_DEPTH_MASK;
        uchar chans = 1 + (type >> CV_CN_SHIFT);
        switch ( depth ) {
            case CV_8U:  typeAsString = "8U"; break;  // 0
            case CV_8S:  typeAsString = "8S"; break;  // 1
            case CV_16U: typeAsString = "16U"; break; // 2
            case CV_16S: typeAsString = "16S"; break; // 3
            case CV_32S: typeAsString = "32S"; break; // 4
            case CV_32F: typeAsString = "32F"; break; // 5
            case CV_64F: typeAsString = "64F"; break; // 6
            default:     typeAsString = "User"; break;
          }
          typeAsString += "C";
          typeAsString += (chans+'0');
    }


    template<typename T>
    bool OcvUtils::stretchImage(const cv::Mat_<T>& img, cv::Mat_<uchar>& out, int a_min, int a_max, double filterMin, double filterMax)
    {
        if ( a_min < 0 || a_max > 255 || filterMin < 1.0 || filterMax > 254.0 )
            return false;

        double minV,maxV;
        cv::minMaxLoc(img, &minV, &maxV);
        double factor = (a_max - a_min)/(maxV - minV);
        double minVal = minV+(factor*filterMin/100);
        double maxVal = maxV-(factor*filterMax/100);
        for(int i=0; i< img.rows; ++i) {
           for(int j=0; j< img.cols; ++j) {
               double v = factor*img(i,j);
                v = v < minVal? a_min : v > maxVal? a_max: v;
               out(i,j) = static_cast<uchar>(  round(v) );
           }
        }
        return true;
    }

    template<typename T>
    void OcvUtils::cosine(const cv::Mat_<T>& angle, cv::Mat_<T>& out, bool isDegree)
    {
        for(int i=0; i< angle.rows; ++i) {
           for(int j=0; j<angle.cols; ++j) {
               out(i,j) = (isDegree)? cos(angle(i,j)*M_PI/180.0) : cos(angle(i,j));
           }
        }

    }

    template<typename T>
    void OcvUtils::sine(const cv::Mat_<T>& angle, cv::Mat_<T>& out, bool isDegree)
    {
        for(int i=0; i< angle.rows; ++i) {
           for(int j=0; j<angle.cols; ++j) {
               out(i,j) = (isDegree)? sin(angle(i,j)*M_PI/180.0) : sin(angle(i,j));
           }
        }

    }


} // end namespace

#endif
/////////////////////////////////////////////END OF FILE //////////////////////////////////////////////////
