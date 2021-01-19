#ifndef __C_H__
#define __C_H__

#include "mito/Constants.h"

#include "mito/Actor.h"
#include "mito/Port.h"
#include "mito/Event.h"

#include "mito/UserEvents.h"

class C : public Actor
  {
private:
    int tInitial (Event *event);
    int tRequest (Event *event);

public:
    C ();
    virtual ~C ();

    Port *port_a;
    Port *port_b;
  };

#endif
