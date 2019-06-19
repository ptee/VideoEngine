#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <QString>
#include <QTime>
#ifdef WIN64 // Port to Windows
#include <chrono>
#include <thread>
#endif

namespace oscv
{
    /**
     * @brief getFormattedTime Convert time from seconds to hours, minutes and seconds as string
     * @param timeInSeconds time in seconds
     * @return string in formatted text as hh:mm:ss or mm:ss
     */
    inline QString getFormattedTime(int timeInSeconds)
    {
        int seconds = (int) (timeInSeconds) % 60 ;
        int minutes = (int) ((timeInSeconds / 60) % 60);
        int hours   = (int) ((timeInSeconds / (60*60)) % 24);
        QTime t(hours, minutes, seconds);
        if (hours == 0 ) {
            return t.toString("mm:ss");
        }
        else {
            return t.toString("h:mm:ss");
        }
    }

    /**
     * @brief Sleep the thread in millisecond
     * @param ms time in miliseconds to let the thread to sleep
    */
    inline void sleepMiliSecond(int ms)
    {
#ifdef WIN64
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#else
        struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
        nanosleep(&ts, NULL);
#endif
    }

}

#endif // TIMEUTILS_H
