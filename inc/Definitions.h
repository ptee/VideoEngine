#ifndef DEFINITIONS_H
#define DEFINITIONS_H

namespace oscv
{
#if defined (__linux__)  || defined (__unix__)

  #define FILE_SEPARATOR "/"
  #define NEWLINE_ENDING "\n"
  //#pragma message("*Nix platform detected")

#elif defined (__WINDOWS__) || defined (_WIN32) || defined (_WIN64)

  #define FILE_SEPARATOR "\\"
  #define NEWLINE_ENDING "\r\n"
  #pragma message("Windows platform detected")

#else
  #error "!!PLATFORM NOT SUPPORT!! FILE_SEPARATOR Not defined!!!"
#endif

typedef unsigned char uint8;
typedef unsigned char uchar;
typedef unsigned int  uint;

}

#endif // DEFINITIONS_H
