#ifdef WIN32
#include <windows.h>
#include <process.h>
#else
#include <time.h>
#include <pthread.h>
//FIXME//int pthread_yield (void);
#endif

#include "mito/Constants.h"

#include "mito/TimerPort.h"
#include "mito/TimerEvent.h"
#include "mito/TimerQueue.h"

#include "mito/Queue.h"
#include "mito/Mutex.h"
#include "mito/Port.h"
#include "mito/MitoLog.h"

static int _running = FALSE;
static int _stopped = FALSE;

static Event *_queue[Constants::Length::List::TIMER];
int TimerQueue::size = Constants::Length::List::TIMER;
static int _size = TimerQueue::size;

#ifdef WIN32
void TimerQueue::loop (void)
#else
void TimerQueue::loop (void)
#endif
  {
    TimerQueue *queue = (TimerQueue *) this;

    //
    // set the running condition
    //
    queue->lock ();
    _running = TRUE;
    queue->unlock ();

    //
    // loop until done
    //
    while (_running)
      {
        int loop;

        //
        // lock the instance and check for events
        //
        queue->lock ();

        //
        // exit the loop if no longer running
        //
        if ((Queue::running () == false) || (_running == FALSE))
          {
            _running = FALSE;
            _stopped = TRUE;
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
                if (((TimerEvent *) _queue[loop])->timeout () < TimerPort::delta ())
                  {
                    //
                    // swap the endpoints and dispatch the timer event
                    //
                    MitoLog *log = queue->log ();
                    Port *temp = _queue[loop]->from ();

                    _queue[loop]->from (_queue[loop]->to ());
                    _queue[loop]->to (temp);

                    Queue::inject (_queue[loop]);
                    log->timeout (temp->actor ()->name (), temp->name ());

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
#else
	struct timespec delay;
	delay.tv_sec = 0;
	delay.tv_nsec = 1000;
	nanosleep(&delay, NULL);
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
#ifdef VXWORKS
    return;
#else
    pthread_exit (NULL);
#endif
#endif
  }

TimerQueue::TimerQueue ()
  {
    int loop;

    //
    // initialize from the globals
    //
    _size = size;

    _thread = 0;

    //
    // create the mutex
    //
    _mutex = new Mutex;

    //
    // create the interface port
    //
    port = new Port (this, "port");

    //
    // clear out the timer list
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

/**
  * Helper function to run the timer event loop on a seperate thread. The
  * exact signature of the thread function and the return type are selected as
  * appropriate by the ifdef statements.
  */
#ifdef WIN32
static unsigned int WINAPI launch (LPVOID handle)
#else
#ifdef VXWORKS
static int launch (void *handle)
#else
static void *launch (void *handle)
#endif
#endif
  {
    TimerQueue *server = (TimerQueue *) handle;

    server->loop ();

#ifdef WIN32
    return (FALSE);
#else
#ifdef VXWORKS
    return (FALSE);
#else
    return (NULL);
#endif
#endif
  }

TimerQueue::~TimerQueue ()
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
#else
#ifdef VXWORKS
#else
    //FIXME//pthread_yield ();
#endif
#endif
      }

    //
    // delete the interface port
    //
    delete port;
  }

int TimerQueue::inject (Event *event)
  {
    if (event->event () == Constants::Events::eInitial)
      {
#ifdef WIN32
    _beginthreadex (NULL, 0, launch, this, 0, &_thread);
#else
#ifdef VXWORKS
    _thread = taskSpawn ("timer", 100, 0x00, (64 * 1024), launch, (int) this,
        0, 0, 0, 0, 0, 0, 0, 0, 0);
#else
    pthread_attr_t attr;

    pthread_attr_init (&attr);
    pthread_attr_setdetachstate (&attr,
        PTHREAD_CREATE_DETACHED);

    pthread_create (&_thread, &attr, launch, (void *) this);
#endif
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

int TimerQueue::lock ()
  {
    return (_mutex->lock ());
  }

int TimerQueue::unlock ()
  {
    return (_mutex->unlock ());
  }

MitoLog *TimerQueue::log ()
  {
    return (&_log);
  }

