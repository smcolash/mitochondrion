#ifndef __TIMER_EVENT_H__
#define __TIMER_EVENT_H__

//
// include system headers
//
#include <iostream>

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

//
// include application headers
//
#include "mito/Event.h"

//
// define the timeout event for the timer class
//
class __DLL__ TimerEvent : public Event
  {
public:
    unsigned long _timeout;
    Event *_data;

public:
    TimerEvent (unsigned long, Port *port, Event *data = NULL);
    virtual ~TimerEvent ();

    virtual Event *copy ();

    virtual unsigned long timeout ();

    friend ostream& operator<< (ostream&, TimerEvent *);
  };

#endif

