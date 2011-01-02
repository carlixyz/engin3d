#ifndef UTIL_H
#define	UTIL_H

#ifdef _WIN32
  #include <windows.h>
  #define GetMiliSeconds() timeGetTime()


#else
  #include <sys/time.h>
  float GetMiliSeconds(){
    timeval ltTime;
    gettimeofday(&ltTime, NULL);
    return ltTime.tv_usec/1000.0f;
  }
#endif

#endif // UTIL_H