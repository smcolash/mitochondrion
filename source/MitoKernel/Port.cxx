#include <string.h>

#include "mito/Port.h"
#include "mito/Actor.h"
#include "mito/Queue.h"
#include "mito/MitoLog.h"
#include "mito/Mutex.h"

//
// global message queue items
//
static int _count = 0;

//
// global debug flags
//
static int _debug = Debug::NONE;

//
// port class implementation
//
int Port::init (Actor *actor, const std::string &id)
  {
    name (id);

//cout << "Port::init (" << actor << ", " << _name << ") -> " << this << "\n";

    _mutex = new Mutex;

    _actor = actor;
    _binding = NULL;
    _timeout = -1;

    _queue = Queue::create ();
    _log = new MitoLog;
    _count++;

    return (_count);
  }

int Port::init (Actor *actor)
  {
    return (init (actor, "unnamed"));
  }

Port::Port (Actor *actor, const std::string &id)
  {
    init (actor, id);
  }

Port::~Port ()
  {
    _count--;
    if (_count == 0)
      {
        delete _queue;
      }

    if (_log != NULL)
      {
        delete _log;
      }
  }

const std::string Port::name ()
  {
    return (_name);
  }

const std::string Port::name (const std::string &id)
  {
    _name = id;
    return (name ());
  }

int Port::inject (Event *event)
  {
    int status = TRUE;

    _mutex->lock ();

    if ((_actor) && (_binding))
      {
        _log->event (_actor->name (), event->event (), _binding->_actor->name (),
            _actor->name ());

        if (_debug & Debug::INJECT)
          {
            cout << "inject:: "
              << _actor->name () << " "
              << event->event () << " "
              << _binding->_actor->name () << " "
              << _actor->name () << "\n";
          }
      }

    if (_actor)
      {
        status = _actor->inject (event);
      }

    _mutex->unlock ();

    return (TRUE);
  }

int Port::send (Event *event)
  {
    if (_binding)
      {
        _log->event (_actor->name (), event->event (), _actor->name (),
            _binding->_actor->name ());

        Event copy (event->event (), event->priority (), _binding, this);

        _mutex->lock ();

        if (_debug & Debug::INJECT)
          {
            if ((_actor) && (_binding))
              {
                cout << "send:: "
                  << _actor->name () << " "
                  << event->event () << " "
                  << _binding->_actor->name () << " "
                  << _actor->name () << "\n";
              }
          }

        _queue->inject (&copy);
        _mutex->unlock ();
      }
    else
      {
        _log->text (_actor->name (), (char *) "port not bound");
      }

    return (FALSE);
  }

int Port::debug (int level)
  {
    _debug = level;
    return (_debug);
  }

int Port::bind (Port *port)
  {
    _binding = port;
    return (FALSE);
  }

int Port::bind (Port *first, Port *second)
  {
    int status;
    
    //cout << "bind: " << first->name () << " <--> " << second->name () << "\n";

    status = first->bind (second);
    if (status)
      {
        return (status);
      }

    status = second->bind (first);

    return (status);
  }

int Port::policy (int timeout)
  {
    _timeout = timeout;

    return (_timeout);
  }

int Port::lock ()
  {
    return (_mutex->lock ());
  }

int Port::unlock ()
  {
    return (_mutex->unlock ());
  }

class Actor *Port::actor ()
  {
    return (_actor);
  }

int Port::print ()
  {
    cout << "--------------------\n";
    cout << "port:       " << _actor->name () << "::" << name () << " (" << this << ")\n";
    cout << "binding:    " << _binding << "\n";
    if (_binding != NULL)
      {
    cout << "binding:    " << _binding->actor ()->name () << "::" << _binding->name () << "\n";
      }
    cout << "queue:      " << _queue << "\n";
    cout << "timeout:    " << _timeout << "\n";
    cout << "log:        " << _log << "\n";
    cout << "mutex:      " << _mutex << "\n";
    cout << "--------------------\n";

    return (FALSE);
  }

