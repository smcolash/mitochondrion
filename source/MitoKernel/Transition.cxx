#include <string.h>

#include "mito/BehaviorNode.h"
#include "mito/State.h"

Guard::Guard ()
  {
    _method = NULL;
    _name = "NULL";
    _attempt = 0;
    _count = 0;
  }

Guard::~Guard ()
  {
  }

int Guard::set (METHOD method, const std::string &label)
  {
    _method = method;
    _name = label;

    return (FALSE);
  }

int Guard::action (Actor *actor, Event *event)
  {
    int status;

    _attempt++;
    status = ((actor)->*(_method)) (event);
    if (status == FALSE)
      {
        _count++;
      }

    return (status);
  }

ostream &Guard::print (ostream &os, int level)
  {
    int loop;

    for (loop = 0; loop < level; loop++)
      {
        os << "  ";
      }

    os << "    guard: " << _name;
    os << " [" << _count << " of " << _attempt << "]\n";

    return (os);
  }

GuardList::GuardList ()
  {
  }

GuardList::~GuardList ()
  {
  }

int GuardList::size ()
  {
    return (_size);
  }

int GuardList::add (const std::string &label, METHOD method)
  {
    if (_size < Constants::Length::List::GUARDS)
      {
        _guards[_size].set (method, label);
        _size++;
      }

    return (size ());
  }

int GuardList::action (Actor *actor, Event *event)
  {
    int loop;
    int status;

    if (size () == 0)
      {
        return (FALSE);
      }

    for (loop = 0; loop < size (); loop++)
      {
        status = _guards[loop].action (actor, event);
        if (status == FALSE)
          {
            return (status);
          }
      }
    return (TRUE);
  }


ostream &GuardList::print (ostream &os, int level)
  {
    int loop;

    for (loop = 0; loop < size (); loop++)
      {
        _guards[loop].print (os, level);
      }

    return (os);
  }

Transition::Transition ()
  {
    _name = "NULL";
    _internal = FALSE;
  }

Transition::~Transition ()
  {
  }

int Transition::event ()
  {
    return (_event);
  }

int Transition::event (int id)
  {
    _event = id;
    return (event ());
  }

const Port *Transition::port ()
  {
    return (_port);
  }

const Port *Transition::port (Port *item)
  {
    _port = item;
    return (port ());
  }

int Transition::internal ()
  {
    return (_internal);
  }

int Transition::internal (int type)
  {
    _internal = type;

    return (internal ());
  }

METHOD Transition::method ()
  {
    return (_method);
  }

METHOD Transition::method (METHOD pointer)
  {
    _method = pointer;
    return (method ());
  }

BehaviorNode *Transition::node ()
  {
    return (_node);
  }

BehaviorNode *Transition::node (BehaviorNode *destination)
  {
    _node = destination;
    return (node ());
  }

const std::string Transition::name ()
  {
    return (_name);
  }

const std::string Transition::name (const std::string &id)
  {
    _name = id;
    return (name ());
  }

int Transition::init (int id, BehaviorNode *destination,
    const std::string &label, METHOD method)
  {
    event (id);
    this->method (method);
    node (destination);
    name (label);

    return (FALSE);
  }

int Transition::guard (const std::string &label, METHOD function)
  {
    return (_guards.add (label, function));
  }

int Transition::count ()
  {
    return (_count);
  }

METHOD Transition::action ()
  {
    _count++;
    return (method ());
  }

int Transition::test (Event *message, Actor *actor)
  {
    if ((port () == NULL) || (port () == message->to ()))
      {
        if ((event () == message->event ()) ||
            (event () == Constants::Events::eWildcard))
          {
            if (!_guards.action (actor, message))
              {
                return (TRUE);
              }
          }
      }

    return (FALSE);
  }

int Transition::fire (Event *event, Actor *actor)
  {
    METHOD pointer;

    _count++;

    pointer = method ();
    if (pointer)
      {
        return (actor->*pointer) (event);
      }

    return (TRUE);
  }

ostream &Transition::print (ostream &os, int level)
  {
    int loop;

    BehaviorNode *temp = node ();

    for (loop = 0; loop < level; loop++)
      {
        os << "  ";
      }

    os << "    event '" << event () << "'";
    if (_port == NULL)
      {
        os << " on any port";
      }
    else
      {
        os << " on port '" << _port->name () << "'";
      }
    os << ", calls '" << name () << "'";
    os << " [" << count () << "]";
    os << " --> (" << temp->name () << ")\n";
    _guards.print (os, level + 1);

    return (os);
  }

//
//
//
TransitionPath::TransitionPath ()
  {
    _size = 0;
  }

TransitionPath::~TransitionPath ()
  {
  }

int TransitionPath::add (Transition *transition)
  {
    if (_size < (10 - 1))
      {
        _path[_size++] = transition;
        return (FALSE);
      }

    return (TRUE);
  }

int TransitionPath::fire (Event *event, Actor *actor)
  {
    int loop;

    for (loop = 0; loop < _size; loop++)
      {
        Transition *transition;

        transition = _path[loop];
        transition->fire (event, actor);
      }

    return (FALSE);
  }

BehaviorNode *TransitionPath::terminal ()
  {
    return (_path[_size - 1]->node ());
  }

