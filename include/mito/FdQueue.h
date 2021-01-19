#ifndef __FDWATCHER_H__
#define __FDWATCHER_H__

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

#include "mito/Actor.h"

//
// Reslove forward references.
//
class Port;
class Mutex;
class Timeout;
class Event;

class __DLL__ FdQueue : public Actor
  {
public:
    static int size;

private:
    Mutex *_mutex;
    Timeout *_schedule;
    int _timeout;
    int _size;

public:
    FdQueue ();
    virtual ~FdQueue ();

    Port *port;

    //
    // event injection method
    //
    virtual int inject (Event *event);

    //
    // mutex methods
    //
    int lock ();
    int unlock ();
  };

#endif

