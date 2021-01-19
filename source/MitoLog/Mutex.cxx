#include "mito/Mutex.h"

Mutex::Mutex ()
  {
    //
    // initialize the mutex
    //
#ifdef WIN32
    // some flavor of 32-bit Windows
    _mutex = CreateMutex (NULL, FALSE, NULL);
#else
#ifdef VXWORKS
    // VxWorks
    _mutex = semMCreate (SEM_Q_PRIORITY | SEM_INVERSION_SAFE | SEM_DELETE_SAFE);
#else
    _taken = 0;
    pthread_mutex_init (&_mutex, NULL);
#endif
#endif
  }

Mutex::~Mutex ()
  {
    //
    // release the mutex
    //
#ifdef WIN32
    CloseHandle (_mutex);
#else
#ifdef VXWORKS
    VXWORKS DELETE MUTEX... (_mutex);
#else
    pthread_mutex_destroy (&_mutex);
#endif
#endif
  }

int Mutex::lock (int timeout)
  {
    int status = 1;

#ifdef WIN32
    DWORD temp;
    DWORD duration;

    duration = timeout;
    if (timeout == Mutex::TIMEOUT_INFINITE)
      {
        duration = INFINITE;
      }
    else if (timeout == Mutex::Timeout::TIMEOUT_IMMEDIATE)
      {
        duration = 0;
      }
    temp = WaitForSingleObject (_mutex, duration);
    if (temp == WAIT_OBJECT_0)
      {
        status = 0;
      }
#else
#ifdef VXWORKS
#else
    status = 0;
    pthread_mutex_lock (&_mutex);
#endif
#endif
    return (status);
  }

int Mutex::unlock ()
  {
#ifdef WIN32
    ReleaseMutex (_mutex);
#else
#ifdef VXWORKS
    semGive (_mutex);
#else
    pthread_mutex_unlock (&_mutex);
#endif
#endif
    return (0);
  }

