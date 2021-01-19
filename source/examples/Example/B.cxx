#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <io.h>
#endif

//
// include application headers
//
#include "mito/TimerEvent.h"

#include "B.h"

//
// initial transition code
//
int B::tInitial (Event *event)
  {
    return (FALSE);
  }

//
// transition code for generic event receive
//
int B::tRequest (Event *event)
  {
    Event response (eResponse, Event::Priority::NORMAL);
    //port_a->send (&response);

    Event message (eRequest, Event::Priority::NORMAL);
    port_b->send (&message);

    return (FALSE);
  }

//
// constructor creates and initializes object behavior
//
B::B ()
  {
    //
    // name the derived Actor type
    //
    type (__FUNCTION__);
    name (__FUNCTION__);

    //
    // create the interface ports
    //
    port_a = new Port (this, "a");
    port_b = new Port (this, "b");

    //
    // create the states
    //
    State *sTop = new State ("top");
    State *sS = new State ("S");

    //
    // connect the states
    //
    sTop->compose (sS);

    //
    // create transition and entry/exit behavior
    //
    sTop->initial (sS, "tInitial",
        (METHOD) &B::tInitial);
    sS->transition (eRequest, "eRequest", port_a, sS, "tRequest",
        (METHOD) &B::tRequest);

    //
    // create an interface to the central log
    //
    MitoLog log;

    //
    // register event enumerations
    //
    log.eventname ("eRequest", 1);
    log.eventname ("eResponse", 2);

    //
    // activate the state machine for use
    //
    behavior (sTop);
  }

//
// destructor deletes any derived-class-specific data
//
B::~B ()
  {
    delete port_a;
    delete port_b;
  }
