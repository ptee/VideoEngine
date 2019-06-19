#ifndef COLORDEF_H
#define COLORDEF_H

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QVector>


namespace oscv
{
   enum class HueColors { Undefined = 0, White, Black, Gray,
                  /*  */  Red /* 0 +-30 */, Yellow /* 60 +-30 */, Green /* 120 +-30*/, Cyan /* 180 +-30*/, Blue /*240 +-30*/, Magenta /*300 +-30*/};

#define RGB_Black   (0,0,0)
#define RGB_White   (255,255,255)
#define RGB_Red     (255,0,0)
#define RGB_Green   (0,255,0)
#define RGB_Blue    (0,0,255)
#define RGB_Yellow  (255,255,0)
#define RGB_Cyan    (0,255,255)
#define RGB_Magenta (255,0,255)
#define RGB_Gray    (128, 128, 128)
#define RGB_Silver (192,192,193)



   class ColorDef
   {
    public:
       // Convert RGB to HSV color
       // H = 0..360, S = 0..1, V = 0..1
       static cv::Vec3f rgb2hsv(const cv::Vec3b& rgb)
       {
           float red = static_cast<float>(rgb[0])/255;
           float green = static_cast<float>(rgb[1])/255;
           float blue = static_cast<float>(rgb[2])/255;
           float minRGB = std::min(red, std::min(green, blue));
           float maxRGB = std::max(red, std::max(green, blue));
           if ( minRGB == maxRGB ) { // black/gray/white
               return cv::Vec3f(0,0, minRGB);
           }
           float delta = maxRGB - minRGB;
           float d =  (red==minRGB)?  (green - blue) : (blue== minRGB)? red-green: blue-red ;
           float h = (red==minRGB)? 3: ((blue==minRGB)? 1: 5);
           float hue = 60*(h-d/delta);
           float sat = delta/maxRGB;
           float val = maxRGB;
           return cv::Vec3f( hue, sat, val);
       }

       // Convert HSV to RGB
       static cv::Vec3b hsv2rgb(const cv::Vec3f hsv)
       {
          float hue = hsv[0];
          float sat = hsv[1];
          float val = hsv[2];

          float C = val*sat;
          float X = C*(1- fabs(static_cast<int>(hue/60)%2 -1));
          float m = val -C;
          cv::Vec3f rgb;
          if       ( hue>=0  && hue < 60 ) { // Red at 0
              rgb[0] = C; rgb[1] = X; rgb[2] = 0;
          }
          else  if ( hue>=60 && hue < 120 ) { // Yellow at 60
              rgb[0] = X; rgb[1] = C; rgb[2] = 0;
          }
          else  if ( hue>=120 && hue < 180 ) { // Green at 120
              rgb[0] = 0; rgb[1] = C; rgb[2] = X;
          }
          else  if ( hue>=180 && hue < 240 ) { // Cyan 180
              rgb[0] = 0; rgb[1] = X; rgb[2] = C;
          }
          else  if ( hue>=240 && hue < 300 ) { // Blue 240
              rgb[0] = X; rgb[1] = 0; rgb[2] = C;
          }
          else  if ( hue>=300 && hue < 360 ) { // Magenta 300
              rgb[0] = C; rgb[1] = 0; rgb[2] = X;
          }
          else  { // undefined!
            // print error!
          }
        return cv::Vec3b( static_cast<int>(rgb[0]+m), static_cast<int>(rgb[1]+m), static_cast<int>(rgb[2]+m) );

       }

      static HueColors estimateColorName(const cv::Vec3f& hsv)
      {
            float hue = hsv[0]; // 0..360
            float sat = hsv[1]; // 0..1
            float val = hsv[2]; // 0..1
            const float lowerBound = 0.25;
            const float higherBound = 0.75;

            // check for gray/white/black in the range of (0,0,val)
            if ( hue < lowerBound && sat < lowerBound ) {
                if (val < lowerBound) {
                    return HueColors::Black;
                }
                else if ( val > higherBound ) {
                    return HueColors::White;
                }
                else {
                    return HueColors::Gray;
                }
            }

            // Other colors, we check only the hue
            if  ( hue>=0  && hue < 30 || hue >= 330 && hue <= 360) { // Red at 0 +-30
                return HueColors::Red;
            }
            else  if ( hue>= 60-30 && hue < 60+30 ) { // Yellow at 60
                return HueColors::Yellow;
            }
            else  if ( hue>=120-30 && hue < 120+30 ) { // Green at 120
                return HueColors::Green;
            }
            else  if ( hue>=180-30 && hue < 180+30 ) { // Cyan 180
                return HueColors::Cyan;
            }
            else  if ( hue>=240-30 && hue < 240+30 ) { // Blue 240
                return HueColors::Blue;
            }
            else  if ( hue>=300-30 && hue < 300+30 ) { // Magenta 300
                return HueColors::Magenta;
            }
            else  { // undefined!
                return HueColors::Undefined;
            }
      }
   };



}
#endif // COLORDEF_H
