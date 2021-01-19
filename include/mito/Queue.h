#ifndef __QUEUE_H__
#define __QUEUE_H__

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

#include "mito/Constants.h"
#include "mito/Event.h"

class TimerWatcher;

class __DLL__ QueueEntry
  {
public:
    QueueEntry ();
    virtual ~QueueEntry ();
    class Event *_event;
    int process ();
    int print ();
  };

class __DLL__ MessageQueue
  {
public:
    Event::Priority::Level _priority;
    int _size;
    class QueueEntry *_queue;
    class Queue *_container;
    int _current;
    int _next;

public:
    MessageQueue (Queue * = NULL, Event::Priority::Level = Event::Priority::NORMAL, int = Constants::Length::List::QUEUE);
    virtual ~MessageQueue ();
    virtual Queue *container (Queue * = NULL);
    virtual Event::Priority::Level priority ();
    virtual Event::Priority::Level priority (Event::Priority::Level);
    virtual int size ();
    virtual int inject (Event *);
    virtual int process ();
    virtual int print ();
    virtual int next ();
  };

class Mutex;

class __DLL__ Queue
  {
protected:
    /**
      * Global control and indicator of the state of the queue.
      */
    static bool _running;

public:
    int _size;
    MessageQueue *_queues;

public:
    Queue ();
    virtual ~Queue ();
    static Queue *create ();
    static int cleanup ();

    static int debug ();
    static int debug (int);

    virtual int print ();

    static int inject (Event *);
    static int process ();
    static int run ();
    static int stop ();
    static bool running (void);

    static TimerWatcher *timer ();
  };

#endif /* __QUEUE_H__ */

