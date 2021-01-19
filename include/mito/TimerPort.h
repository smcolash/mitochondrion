#ifndef __TIMER_H__
#define __TIMER_H__

#include <string>

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

#include "mito/Constants.h"
#include "mito/Port.h"
#include "mito/MitoLog.h"

class __DLL__ TimerPort : public Port
  {
private:
    MitoLog log;

public:
    TimerPort (Actor *, const std::string &id);
    virtual ~TimerPort ();

    virtual int set (int timeout, Event *data = NULL);
    virtual int cancel (int);

    static double timestamp ();
    static unsigned long int msec ();
    static unsigned long int usec ();

    static unsigned long clear ();
    static unsigned long delta ();
  };

#endif

