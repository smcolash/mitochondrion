//
// include application headers
//
#include "mito/TimerEvent.h"
#include "mito/TimerPort.h"

//
// basic constructor
//
TimerEvent::TimerEvent (unsigned long duration, Port *timer, Event *data)
  {
    _timeout = duration;
    from (timer);
    to (timer);
    event (Constants::Events::eTimer);
    priority (Event::Priority::NORMAL);

    _data = NULL;
    if (data)
      {
        _data = data->copy ();
      }
  }

//
// destructor
//
TimerEvent::~TimerEvent ()
  {
  }

//
// class-specific copy function
//
Event *TimerEvent::copy ()
  {
    TimerEvent *event = new TimerEvent (this->timeout (),
        this->from (), this->_data);
    return ((Event *) event);
  }

//
// timeout accessor
//
unsigned long TimerEvent::timeout ()
  {
    return (_timeout);
  }

//
// output routine
//
ostream& operator<< (ostream &os, TimerEvent &event)
  {
    os << (Event &) event;
    os << ", timeout = " << event.timeout ();
    return (os);
  }

