//
// include application headers
//
#include "C.h"

//
// initial transition code
//
int C::tInitial (Event *event)
  {
    //cout << "C::" << __FUNCTION__ << "\n";
    return (FALSE);
  }

//
// transition code for generic event receive
//
int C::tRequest (Event *event)
  {
    //cout << "C::" << __FUNCTION__ << "\n";
    Event message (eRequest, Event::Priority::NORMAL);
    port_b->send (&message);

    return (FALSE);
  }

//
// constructor creates and initializes object behavior
//
C::C ()
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
        (METHOD) &C::tInitial);
    sS->transition (eRequest, "eRequest", port_a, sS, "tRequest",
        (METHOD) &C::tRequest);

    //
    // activate the state machine for use
    //
    behavior (sTop);
  }

//
// destructor deletes any derived-class-specific data
//
C::~C ()
  {
    delete port_a;
    delete port_b;
  }
