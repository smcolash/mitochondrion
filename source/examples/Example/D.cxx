//
// include system headers
//
#include <sstream>

//
// include application headers
//
#include "D.h"
#include "mito/UserEvents.h"
#include "mito/Constants.h"

//
// initial transition code
//
int D::tInitial (Event *event)
  {
    //
    // done with the event
    //
    return (FALSE);
  }

//
// transition code for generic event receive
//
int D::tRequest (Event *event)
  {
    //
    // done with the event
    //
    return (FALSE);
  }

//
// constructor creates and initializes object behavior
//
D::D ()
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
    port_c = new Port (this, "c");
    port_d = new Port (this, "d");

    //
    // create the states
    //
    State *sTop = new State ("top");
    State *sS1 = new State ("S1");

    //
    // connect the states
    //
    sTop->compose (sS1);

    //
    // create transition and entry/exit behavior
    //
    sTop->initial (sS1, "tInitial",
        (METHOD) &D::tInitial);
    sS1->transition (eRequest, "eRequest",
        port_a, sS1, "tRequest", (METHOD) &D::tRequest);
    sS1->transition (eRequest, "eRequest",
        port_b, sS1, "tRequest", (METHOD) &D::tRequest);
    sS1->transition (eRequest, "eRequest",
        port_c, sS1, "tRequest", (METHOD) &D::tRequest);
    sS1->transition (eRequest, "eRequest",
        port_d, sS1, "tRequest", (METHOD) &D::tRequest);

    //
    // activate the state machine for use
    //
    behavior (sTop);

    //
    // ready for use in the model
    //
    return;
  }

//
// destructor deletes any derived-class-specific data
//
D::~D ()
  {
    delete port_a;
    delete port_b;
    delete port_c;
    delete port_d;
  }
