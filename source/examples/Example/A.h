#ifndef __A_H__
#define __A_H__

#include "mito/Actor.h"
#include "mito/Port.h"
#include "mito/TimerPort.h"

#include "mito/Event.h"
#include "mito/TimerEvent.h"
#include "mito/Statistics.h"
#include "mito/MitoLog.h"

class A : public Actor
  {
private:
    int tInitial (Event *event);
    int tRequest (Event *event);
    int tTimeout (Event *event);
    int unhandled (Event *event);

public:
    int status_callback (WebServer *server, const std::string &command, 
        const std::string &url);
    int control_callback (WebServer *server, const std::string &command, 
        const std::string &url);

private:
    int _count;
    mito::Statistics _statistics;
    MitoLogRecord::ALARM_TYPE _alarm;

public:
    A ();
    virtual ~A ();

    Port *port_a;
    Port *port_b;
    Port *port_c;
    TimerPort *timer;
  };

#endif
