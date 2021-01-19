#ifndef __D_H__
#define __D_H__

#include "mito/Actor.h"
#include "mito/Port.h"

#include "mito/Event.h"

class D : public Actor
  {
private:
    int tInitial (Event *event);
    int tRequest (Event *event);

public:
    D ();
    virtual ~D ();

    Port *port_a;
    Port *port_b;
    Port *port_c;
    Port *port_d;
  };

#endif
