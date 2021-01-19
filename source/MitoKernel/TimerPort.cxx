#ifdef WIN32
#include <windows.h>
#include <mmsystem.h>
#else
#include <sys/time.h>
#endif

#include "mito/TimerPort.h"
#include "mito/TimerEvent.h"
#include "mito/TimerQueue.h"

//
// timer event queue processor
//
static TimerQueue *_processor = NULL;

//
// timer queue reference count
//
static int _count = 0;

//
// time that the system was initialized
//
static unsigned long _time = 0;

//
// constructor
//
TimerPort::TimerPort (Actor *actor, const std::string &id)
  {
    //
    // initialize the instance data
    //
    init (actor, id);

    //
    // perform (potentially) global/class manipulations
    //
    lock ();

    if (_time == 0)
      {
        TimerPort::clear ();
      }

    //
    // create the timer queue
    //
    if (_processor == NULL)
      {
        //
        // allocate a new queue
        //
        _processor = new TimerQueue;
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
TimerPort::~TimerPort ()
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
// static method to return the current system time
//
double TimerPort::timestamp ()
  {
    double temp = 0.0;

    //
    // get the (hopefully precise) system time
    //
#ifdef WIN32
    LARGE_INTEGER frequency;
    LARGE_INTEGER counter;

    QueryPerformanceCounter (&counter);
    QueryPerformanceFrequency (&frequency);
    if (frequency.QuadPart == 0)
      {
        timeBeginPeriod (1);
        temp = timeGetTime ();
        timeEndPeriod (1);
      }
    else
      {
        temp = (double) counter.QuadPart / (double) frequency.QuadPart;
      }
#else
    struct timeval tv;

    gettimeofday (&tv, NULL);
    temp = tv.tv_sec + (tv.tv_usec / 1.0e6);
#endif

    return (temp);
  }

//
// static method to return the current system time in milliseconds
//
unsigned long TimerPort::msec ()
  {
    unsigned long int temp;

    temp = (unsigned long int) (timestamp () * 1.0e3);

    return (temp);
  }

//
// static method to return the current system time in microseconds
//
unsigned long TimerPort::usec ()
  {
    unsigned long int temp;

    temp = (unsigned long int) (timestamp () * 1.0e6);

    return (temp);
  }

//
// return time since initialization
//
unsigned long int TimerPort::delta ()
  {
    unsigned long int value;

    value = msec () - _time;

    return (value);
  }

//
// set the time of the start of execution
//
unsigned long TimerPort::clear ()
  {
    _time = msec ();

    return (_time);
  }

//
// return the index of the timer entry if successful, else returns -1
//
int TimerPort::set (int timeout, Event *data)
  {
    //
    // create and insert timer event into the queue
    //
    TimerEvent event (TimerPort::delta () + timeout, this, data);

    //
    // log the start of the timer
    //
    log.timer (actor ()->name (), name (), timeout);

    return (_processor->port->inject (&event));
  }

//
//
//
int TimerPort::cancel (int)
  {
    //
    // log the cancellation of the timer
    //
    log.cancel (actor()->name (), name ());

    return (FALSE);
  }

