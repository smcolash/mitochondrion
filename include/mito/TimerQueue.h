#ifndef __TIMERWATCHER_H__
#define __TIMERWATCHER_H__

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

#include "mito/Actor.h"
#include "mito/MitoLog.h"

class Port;
class Mutex;
class Timeout;

class __DLL__ TimerQueue : public Actor
  {
public:
    static int size;

private:
    Mutex *_mutex;
    Timeout *_schedule;
#ifdef WIN32
    unsigned int _thread;
#else
#ifdef VXWORKS
    int _thread;
#else
    pthread_t _thread;
#endif
#endif
    int _timeout;
    int _size;
    MitoLog _log;

public:
    TimerQueue ();
    virtual ~TimerQueue ();

    Port *port;

    //
    // event injection method
    //
    virtual int inject (Event *event);

    MitoLog *log ();

    //
    // mutex methods
    //
    int lock ();
    int unlock ();
    void loop ();
  };

#endif

