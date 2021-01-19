#ifdef WIN32
#include "windows.h"
#include "winbase.h"
#else
#include <time.h>
#endif

#include <sys/timeb.h>

#include "mito/Queue.h"
#include "mito/Port.h"
#include "mito/Mutex.h"

//
// global status of queue processing
//
bool Queue::_running = false;

//
// local debug flags for event processing
//
static int _debug = Debug::NONE;

//
// local pointer to the system event queue
//
static Queue *_queue = NULL;
static Mutex *_mutex = NULL;

//
// generic event holder for use in an event queue
//
QueueEntry::QueueEntry ()
  {
    _event = NULL;
  }

QueueEntry::~QueueEntry ()
  {
  }

int QueueEntry::print ()
  {
    cout << "--------------------\n";
    cout << "event:    " << _event->event () << "\n";;
    cout << "priority: " << _event->priority () << "\n";;
    cout << "from:     " << _event->from () << "\n";;
    cout << "to:       " << _event->to () << "\n";;
    cout << "--------------------\n";

    return (FALSE);
  }

int QueueEntry::process ()
  {
    if (_event->event () == Constants::Events::eTerminate)
      {
        return (TRUE);
      }

    if (_event->to ())
      {
        Port *to = _event->to ();
        //cout << __LINE__ << " " << to << "\n";
        //to->print ();
        to->inject (_event);
      }

    delete _event;
    _event = NULL;

    return (FALSE);
  }

//
// generic event queue
//
MessageQueue::MessageQueue (Queue *mqueue, Event::Priority::Level priority, int size)
  {
    _container = mqueue;
    _priority = priority;
    _size = size;
    _queue = new QueueEntry[_size];
    _current = 0;
    _next = 0;
  }

MessageQueue::~MessageQueue ()
  {
    delete [] _queue;
  }

Queue *MessageQueue::container (Queue *queue)
  {
    if (queue)
      {
        _container = queue;
      }

    return (_container);
  }

Event::Priority::Level MessageQueue::priority ()
  {
    return (_priority);
  }

Event::Priority::Level MessageQueue::priority (Event::Priority::Level level)
  {
    _priority = level;
    return (priority ());
  }

int MessageQueue::inject (Event *event)
  {
    _queue[_next]._event = event->copy ();
    _next = (_next + 1) % _size;
    return (0);
  }

int MessageQueue::next ()
  {
    return (_next);
  }

int MessageQueue::print ()
  {
    int loop;

    cout << "priority: " << Event::Priority::name (_priority) << "\n";
    for (loop = _current; loop < _next; loop++)
      {
        cout << "[" << loop << "] ";
        _queue[loop].print ();
      }
    return (FALSE);
  }

int MessageQueue::size ()
  {
    return (_size);
  }

int MessageQueue::process ()
  {
    while (_current != _next)
      {
        if (_queue[_current].process ())
          {
            return (TRUE);
          }

        _current = (_current + 1) % _size;
      }
    
    return (FALSE);
  }

//
// system message queue, a collection of the other queues
//
Queue::Queue ()
  {
  }

Queue::~Queue ()
  {
    delete [] _queues;
  }

int Queue::cleanup ()
  {
    delete _queue;

    return (FALSE);
  }

Queue *Queue::create ()
  {
    if (_queue == NULL)
      {
        int loop;

        _queue = new Queue;

        _queue->_size = Event::Priority::SYSTEM - Event::Priority::LOW + 1;
        _queue->_queues = new MessageQueue[_queue->_size];

        for (loop = Event::Priority::LOW; loop < Event::Priority::SYSTEM + 1; loop++)
          {
            _queue->_queues[Event::Priority::SYSTEM - loop].container (_queue);
            _queue->_queues[Event::Priority::SYSTEM - loop].priority ((Event::Priority::Level) loop);
          }
      }

    if (_mutex == NULL)
      {
        _mutex = new Mutex;
      }

    return (_queue);
  }

int Queue::inject (Event *event)
  {
    _mutex->lock ();

    if (_debug & Debug::INJECT)
      {
        cout << "TRACE:QUEUE:";
        cout << "EVENT:[" << event->event () << "]:";
        cout << "PRIORITY:[" << event->priority () << "]\n" << flush;
      }

    _queue->_queues[Event::Priority::SYSTEM - event->priority ()].inject (event);
    _mutex->unlock ();

    return (FALSE);
  }

int Queue::debug ()
  {
    return (_debug);
  }

int Queue::debug (int level)
  {
    _debug = level;
    return (debug ());
  }

int Queue::print ()
  {
    int loop;

    cout << "------------------------------\n";;
    cout << "system event queue:\n";
    for (loop = Event::Priority::LOW; loop < Event::Priority::SYSTEM + 1; loop++)
      {
        _queues[loop].print ();
      }
    cout << "------------------------------\n" << flush;

    return (FALSE);
  }

int Queue::process ()
  {
    int loop;

    if (_debug & Debug::INJECT)
      {
        // _queue->print ();
      }

    for (loop = Event::Priority::LOW; loop < Event::Priority::SYSTEM + 1; loop++)
      {
        if (_queue->_queues[loop].process ())
          {
            return (TRUE);
          }
      }

    //
    // the queue is empty, release to the rest of the system
    //
#ifdef WIN32
    Sleep (1);
#else
#ifdef VXWORKS
    taskDelay (0);
#else
    // POSIX
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 1000;
    nanosleep(&delay, NULL);
#endif
#endif

    return (FALSE);
  }

int Queue::run ()
  {
    _running = true;
    while (!process ())
      {
      }

    return (FALSE);
  }

int Queue::stop (void)
  {
    Event event (Constants::Events::eTerminate);

    return (Queue::inject (&event));
  }

bool Queue::running (void)
  {
    return (_running);
  }
