//
// include application headers
//
#include "mito/FdPort.h"
#include "mito/FdEvent.h"

//
// basic constructor
//
FdEvent::FdEvent (unsigned long fd, Port *port, Event *data)
  {
    _fd = fd;
    from (port);
    to (port);
    event (Constants::Events::eFile);
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
FdEvent::~FdEvent ()
  {
  }

//
// class-specific copy function
//
Event *FdEvent::copy ()
  {
    FdEvent *event = new FdEvent (this->fd (), this->from (), this->_data);
    return ((Event *) event);
  }

//
// timeout accessor
//
unsigned long FdEvent::fd ()
  {
    return (_fd);
  }

//
// output routine
//
ostream& operator<< (ostream &os, FdEvent &event)
  {
    os << (Event &) event;
    os << ", fd = " << event.fd ();
    return (os);
  }

