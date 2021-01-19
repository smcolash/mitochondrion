#ifndef __B_H__
#define __B_H__

#include "mito/Constants.h"

#include "mito/Actor.h"
#include "mito/Port.h"
#include "mito/Event.h"

#include "mito/UserEvents.h"

class B : public Actor
  {
private:
    int tInitial (Event *event);
    int tRequest (Event *event);

public:
    B ();
    virtual ~B ();

    Port *port_a;
    Port *port_b;
  };

#endif
