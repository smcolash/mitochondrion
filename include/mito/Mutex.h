#ifndef __MUTEX_H__
#define __MUTEX_H__

//
// incluse system headers
//
#include <iostream>

//
// include platform-specific headers
//
#ifdef WIN32
//
// windows
//
#include <windows.h>
#include <process.h>
#else
#ifdef VXWORKS
//
// vxworks
//
#include <vxWorks.h>
#include <semLib.h>
#else
//
// other, POSIX compliant
//
#include <pthread.h>
#endif
#endif

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

class __DLL__ Mutex
  {
private:
#ifdef WIN32
    HANDLE _mutex;
#else
#ifdef VXWORKS
    SEM_ID _mutex;
#else
    int _taken;
    pthread_mutex_t _mutex;
#endif
#endif

public:
    typedef enum Timeout
      {
        TIMEOUT_INFINITE = -1,
        TIMEOUT_IMMEDIATE = 0
      } _Timeout;

public:
    Mutex ();
    ~Mutex ();

    int lock (int timeout = Mutex::TIMEOUT_INFINITE);
    int unlock ();
  };

#endif

