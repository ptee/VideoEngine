#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <QString>


namespace oscv
{

    /**
    * @brief beautifyNumberToString convert & beautify number to string
    * @param num[in] number
    * @param digits[in] number format of '0'
    * @param str[out]
    * @return
    */
    inline void beautifyNumberToString(int num, int digits, QString& str )
    {
        str = QString::number(num).rightJustified(digits,'0');
    }




}
#endif // STRINGUTILS_H
