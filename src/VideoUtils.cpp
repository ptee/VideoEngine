#include "VideoUtils.h"
// opencv
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
// Qt
#include <QApplication>
#include <QVector>
// oscv
#include "ProgressBar.h"
#include "Definitions.h"
#include "VideoDefs.h"
#include "ImageDefs.h"
#include "FileUtils.h"
#include "StringUtils.h"

using namespace oscv;
using namespace cv;

bool VideoUtils::VidToImg(const QString& vidFile,
                          const QString& imgDir,
                          const QString& imgPrefix,
                          IProgressBar* progress)
{

    VideoCapture* capture  =  new VideoCapture( vidFile.toStdString() );
    if ( ! capture->isOpened() ) {
        return false;
    }

    double numberOfFrame = getNumberOfFramesWithLimit( capture );
    if (progress) {
        progress->setMaximum(numberOfFrame);
    }

    QString filename(imgDir);
    getPathWithSeparator(filename);

    QVector<int> params;
    //params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    //params.push_back(16);
    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(95);


    Mat frame;
    bool ok = true;
    for( uint i=0;i<numberOfFrame;i++)
    {
        QString num;
        beautifyNumberToString(i, 4, num); // allow for 4 digits number, max is 9999
        num.prepend(imgPrefix);
        QString str(filename);
        str.append(num);
        str.append(ImageDefs::DEFAULT_IMG_EXTENSION);

        if (progress) {
            progress->setValue(i);
        }

        // Allow GUI to be able to perform update or redraw
        QApplication::processEvents();

        // Reading video..
        if( capture->read(frame) && frame.data ) {
            imwrite(str.toStdString(), frame, params.toStdVector());
        }
        if (progress) {
            if ( progress->wasCanceled() ) {
                ok = false;
                break;
            }
        }
    } // end for loop

    // Ok, done. Release the video capture
    capture->release();
    delete capture;
    return ok;
}
