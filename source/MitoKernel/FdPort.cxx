#ifdef WIN32
#include <windows.h>
#include <mmsystem.h>
#else
#include <sys/time.h>
#endif

#include "mito/FdPort.h"
#include "mito/FdEvent.h"
#include "mito/FdQueue.h"

//
// queue processor
//
static FdQueue *_processor = NULL;

//
// queue reference count
//
static int _count = 0;

//
// constructor
//
FdPort::FdPort (Actor *actor, const std::string &id)
  {
    //
    // initialize the instance data
    //
    init (actor, id);

    //
    // perform (potentially) global/class manipulations
    //
    lock ();

    //
    // create the event watcher
    //
    if (_processor == NULL)
      {
        //
        // allocate a new watcher
        //
        _processor = new FdQueue;
      }

    //
    // increase the reference count
    //
    _count++;

    unlock ();
  }

//
// destructor
//
FdPort::~FdPort ()
  {
    lock ();

    //
    // decrement reference count
    //
    _count--;

    //
    // destroy the timer queue if no longer needed
    //
    if (_count < 1)
      {
        delete _processor;
      }

    unlock ();
  }

//
// return the index of the timer entry if successful, else returns -1
//
int FdPort::set (int fd, Event *data)
  {
    //
    // create and insert timer event into the queue
    //
    FdEvent event (fd, this, data);

    return (_processor->port->inject (&event));
  }

//
//
//
int FdPort::cancel (int)
  {
    return (FALSE);
  }

