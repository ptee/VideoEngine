#ifndef DRAWING_H
#define DRAWING_H

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QVector>


namespace oscv
{
/**
 * @brief Contour_t is a lists(as vector) of several points which use to draw the contour around the object.
 */
typedef std::vector<cv::Point> Contour_t;
/**
 * @brief Hierarchy_t hierarchy of the contours
 */
typedef cv::Vec4i   Hierarchy_t;

typedef cv::Rect Rect_t;


/**
 * @brief The Drawing class This class might be called from different threads to draw their own objects.
 */
class Contours
{
public:
    /**
     * @brief Contours The only main computation.
     * @param binaryImg binary image after thresholding
     * @param minArea min number of pixel to be pass through the filter
     */
    Contours( const cv::Mat& binaryImg, int minArea=10 );

    inline const std::vector<Contour_t>& contours() const;

    inline const std::vector<Rect_t>& rectangles() const;

    inline const std::vector<Hierarchy_t>& hierarchies() const;

    inline const std::vector<cv::Moments>& moments() const;

    inline cv::Point2d massCenter( const cv::Moments& moment ) const;


private:
    std::vector<Contour_t> m_contours;
    std::vector<Rect_t> m_rects;

    /* hierarchy[idx][{0,1,2,3}]={next contour (same level),
               previous contour (same level),
               child contour, parent contour} */
    std::vector<Hierarchy_t> m_hierarchies;
    std::vector<cv::Moments>  m_moments;

};
////////////////////INLINE

const std::vector<Contour_t>& Contours::contours() const
{
    return m_contours;

}

const std::vector<Rect_t>& Contours::rectangles() const
{
    return m_rects;
}

const std::vector<Hierarchy_t>& Contours::hierarchies() const
{
    return m_hierarchies;
}

const std::vector<cv::Moments>& Contours::moments() const
{
    return m_moments;
}

cv::Point2d Contours::massCenter( const cv::Moments& m ) const
{
    return cv::Point2d(  (int)round(m.m10/m.m00), (int)round( m.m01/m.m00) );
}


} // end namespace
#endif //DRAWING_H
/////////////////////////////////////////////END OF FILE//////////////////////////////////////////////////////
