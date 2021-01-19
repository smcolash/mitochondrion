#ifdef WIN32
#include <windows.h>
#include <process.h>
#include <io.h>
#endif

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "mito/Constants.h"

#include "mito/FdQueue.h"
#include "mito/FdEvent.h"

#include "mito/Queue.h"
#include "mito/Mutex.h"
#include "mito/Port.h"

static int _running = FALSE;
static int _stopped = FALSE;

static Event *_queue[Constants::Length::List::TIMER];
int FdQueue::size = Constants::Length::List::TIMER;
static int _size = FdQueue::size;

#if 0
#ifdef WIN32
static void _thread (void *instance)
#else
static int _thread (void *instance)
#endif
  {
    FdQueue *queue = (FdQueue *) instance;

    //
    // set the running condition
    //
    queue->lock ();
    _running = TRUE;
    queue->unlock ();

    //
    // loop until done
    //
    while (1)
      {
        int loop;

        //
        // lock the instance and check for events
        //
        queue->lock ();

        //
        // exit the loop if no longer running
        //
        if (_running == FALSE)
          {
            queue->unlock ();
            break;
          }

        //
        // check for events
        //
        for (loop = 0; loop < _size; loop++)
          {
            if (_queue[loop] != NULL)
              {
                FILE *temp = fdopen (((FdEvent *) _queue[loop])->fd (), "r");
                //if (!feof (((FdEvent *) _queue[loop])->fd ()))
                if (!feof (temp))
                  {
                    //
                    // swap the endpoints and dispatch the event
                    //
                    Port *temp = _queue[loop]->from ();
                    _queue[loop]->from (_queue[loop]->to ());
                    _queue[loop]->to (temp);

                    Queue::inject (_queue[loop]);

                    //
                    // clean up the queue entry
                    //
                    delete _queue[loop];
                    _queue[loop] = NULL;
                  }
              }
          }

        //
        // unlock the instance
        //
        queue->unlock ();

        //
        // release to the rest of the system
        //
#ifdef WIN32
        Sleep (1);
#endif
      }

    //
    // signal that the thread is stopping
    //
    queue->lock ();
    _stopped = TRUE;
    queue->unlock ();

#ifdef WIN32
    return;
#else
    return (0);
#endif
  }
#endif

FdQueue::FdQueue ()
  {
    int loop;

    //
    // initialize from the globals
    //
    _size = size;

    //
    // create the mutex
    //
    _mutex = new Mutex;

    //
    // create the interface port
    //
    port = new Port (this, "port");

    //
    // clear out the event list
    //
    for (loop = 0; loop < _size; loop++)
      {
        _queue[loop] = NULL;
      }

    //
    // notify the queue that the object is ready to start
    //
    Event event (Constants::Events::eInitial, Event::Priority::SYSTEM, port);
    Queue::inject (&event);
  }

FdQueue::~FdQueue ()
  {
    //
    // terminate the thread
    //
    _mutex->lock ();
    _running = FALSE;
    _mutex->unlock ();

    while (_stopped != TRUE)
      {
#ifdef WIN32
        Sleep (1);
#endif
      }

    //
    // delete the interface port
    //
    delete port;
  }

int FdQueue::inject (Event *event)
  {
    if (event->event () == Constants::Events::eInitial)
      {
#ifdef WIN32
        _beginthread (_thread, 0, (void *) this);
#endif
      }
    else
      {
        int loop;
        int status = TRUE;

        //
        // insert the event into the queue
        //
        _mutex->lock ();
        for (loop = 0; loop < _size; loop++)
          {
            if (_queue[loop] == NULL)
              {
                _queue[loop] = event->copy ();
                status = FALSE;
                break;
              }
          }

        if (status == TRUE)
          {
            cout << "error - failed to schedule the event\n" << flush;
          }

        _mutex->unlock ();
      }
    
    return (FALSE);
  }

int FdQueue::lock ()
  {
    return (_mutex->lock ());
  }

int FdQueue::unlock ()
  {
    return (_mutex->unlock ());
  }

