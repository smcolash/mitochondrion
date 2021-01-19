#ifndef __FILEACTIVITY_H__
#define __FILEACTIVITY_H__

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
// Resolve forward references.
//
class Port;
class Event;

//
// define the timeout event for the timer class
//
class __DLL__ FdEvent : public Event
  {
public:
    unsigned long _fd;
    Event *_data;

public:
    FdEvent (unsigned long fd, Port *port, Event *data = NULL);
    virtual ~FdEvent ();

    virtual Event *copy ();

    virtual unsigned long fd ();

    friend ostream& operator<< (ostream&, FdEvent *);
  };

#endif

