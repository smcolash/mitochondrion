//
// include system headers
//
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#include <mmsystem.h>
#else
#include <time.h>
#include <sys/time.h>
#endif

#include <string>

//
// include application headers
//
#include "mito/MitoLog.h"

//
// static method to return the current system time
//
double timestamp ()
  {
    double temp = 0.0;

    //
    // get the (hopefully precise) system time
    //
#ifdef WIN32
    LARGE_INTEGER frequency;
    LARGE_INTEGER counter;

    QueryPerformanceCounter (&counter);
    QueryPerformanceFrequency (&frequency);
    if (frequency.QuadPart == 0)
      {
        timeBeginPeriod (1);
        temp = timeGetTime ();
        timeEndPeriod (1);
      }
    else
      {
        temp = (double) counter.QuadPart / (double) frequency.QuadPart;
      }
#else
    struct timespec ts;
    memset ((void *) &ts, 0x00, sizeof (ts));

    int status = clock_gettime (CLOCK_MONOTONIC, &ts);
    if (status != 0)
      {
        status = clock_gettime (CLOCK_REALTIME, &ts);
        if (status != 0)
          {
            memset ((void *) &ts, 0x00, sizeof (ts));
          }
      }

    temp = ts.tv_sec + (ts.tv_nsec / 1.0e9);
#endif

    return (temp);
  }

//
// initialization method
//
void MitoLogRecord::clear (void)
  {
    timestamp = 0.0;
    type = INVALID;
    memset (label, 0x00, sizeof (label));
    memset (actor, 0x00, sizeof (actor));
    memset ((char *) &data, 0x00, sizeof (data));

    return;
  }

//
// constructor
//
MitoLogRecord::MitoLogRecord ()
  {
    clear ();
  }

MitoLogRecord::MitoLogRecord (MitoLogRecord::EVENT_TYPE event_type,
    const std::string &context)
  {
    clear ();

    timestamp = ::timestamp ();
    type = event_type;
    strncpy (actor, context.c_str (), sizeof (actor) - 1);

    return;
  }

//
// destructor
//
MitoLogRecord::~MitoLogRecord ()
  {
  }

//
// copy method
//
int MitoLogRecord::copy (const MitoLogRecord &record)
  {
    timestamp = record.timestamp;
    type = record.type;
    strncpy (label, record.label, sizeof (label));
    strncpy (actor, record.actor, sizeof (actor));
    memcpy ((char *) &data, (char *) &record.data, sizeof (data));

    return (0);
  }
