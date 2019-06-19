#ifndef MATHUTILS_H
#define MATHUTILS_H

namespace oscv
{

    template <typename T> int sgn(T val)
    {
        return (T > 0) ? 1 : ((T < 0) ? -1 : 0);
    }
}




#endif // MATHUTILS_H
