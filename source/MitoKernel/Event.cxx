#include <string.h>

#include "mito/Event.h"
#include "mito/Port.h"

Event::Event ()
  {
    //memset (this, 0x00, sizeof (*this));
  }

Event::Event (int code, Event::Priority::Level priority, Port *to, Port *from)
  {
    _event = code;
    _priority = priority;
    _from = from;
    _to = to;
  }

Event::Event (Event *other)
  {
    _event = other->event ();
    _priority = other->priority ();
    _from = other->from ();
    _to = other->to ();
  }

Event::~Event ()
  {
  }

Event::Priority::Level Event::priority ()
  {
    return (_priority);
  }

Event::Priority::Level Event::priority (Event::Priority::Level level)
  {
    _priority = level;
    return (priority ());
  }

int Event::event ()
  {
    return (_event);
  }

int Event::event (int value)
  {
    _event = value;

    return (event ());
  }

Port *Event::to ()
  {
    return (_to);
  }

Port *Event::to (Port *port)
  {
    _to = port;
    return (to ());
  }

Port *Event::from ()
  {
    return (_from);
  }

Port *Event::from (Port *port)
  {
    _from = port;
    return (from ());
  }

ostream& operator<< (ostream &os, Event &event)
  {
    os << "event: code = " << event.event ();
    os << ", priority = " << Event::Priority::name (event.priority ());
    if (event.from ())
      {
        os << ", from [" << (event.from ())->name () << "]";
      }
    if (event.to ())
      {
        os << ", to [" << (event.to ())->name () << "]";
      }
    return (os);
  }

Event *Event::copy ()
  {
    return (new Event (this));
  }

