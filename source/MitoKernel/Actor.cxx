#include <iostream>
#include <string>
#include <set>

#include "mito/Actor.h"

#include "mito/Event.h"
#include "mito/Port.h"
#include "mito/State.h"
#include "mito/Queue.h"

using namespace std;

/**
  * A set of unique in-use Actor names.
  */
static set<string>actors;

#if 0
/**
  * A function to provide unique Actor names at runtime.
  */
static const char *unique (const char *name)
  {
    set<string>::iterator index;
    string id = name;

    index = actors.find (id);
    if (index != actors.end ())
      {
        unsigned int loop;
        for (loop = 1; loop < 1000; loop++)
          {
            char temp[256];

            sprintf (temp, "%s_%d", name, loop);
            id = temp;
            index = actors.find (id);
            if (index == actors.end ())
              {
                actors.insert (id);
                return (id.c_str ());
              }
          }
      }
    else
      {
        actors.insert (id);
        return (id.c_str ());
      }

    return (name);
  }
#endif

Actor::Actor ()
  {
    _behavior = NULL;
    _control = new Port (this, "_control");
    _debug = Debug::NONE;
    type (__FUNCTION__);
    name (__FUNCTION__);
  }

Actor::~Actor ()
  {
    if (_behavior)
      {
        delete _behavior;
      }

    if (_control)
      {
        delete (_control);
      }
  }

int Actor::inject (Event *event)
  {
    if (_behavior)
      {
        return (_behavior->inject (event));
      }
    else
      {
        return (TRUE);
      }
  }

//
// set the debug level of the current subtree of states
//
int Actor::debug ()
  {
    return (_debug);
  }

int Actor::debug (int level)
  {
    _debug = level;
    if (_behavior)
      {
        _behavior->debug (level);
      }
    return (debug ());
  }

//
// print the actor/behavior decomposition
//
int Actor::print ()
  {
    cout << "Actor '" << name () << "', class '" << type () << "'\n";
    if (_behavior)
      {
        _behavior->print ();
      }
    cout << "\n" << flush;

    return (FALSE);
  }

ostream& operator<< (ostream &os, Actor &actor)
  {
    os << "Actor '" << actor.name () << "', class '" << actor.type () << "'\n";
    if (actor.behavior ())
      {
        os << *(actor.behavior ());
      }

    os << "\n" << std::flush;

    return (os);
  }

const std::string Actor::type ()
  {
    return (_type);
  }

const std::string Actor::type (const std::string &id)
  {
    if (id != "")
      {
        _type = id;
      }

    return (type ());
  }

const std::string Actor::name ()
  {
    return (_name);
  }

const std::string Actor::name (const std::string &id)
  {
    if (id != "")
      {
        _name = id;
      }

    return (name ());
  }

State *Actor::behavior ()
  {
    return (_behavior);
  }

State *Actor::behavior (State *top)
  {
    _behavior = top;
    _behavior->activate (this);

    //
    // inject the initial event into the message queue
    //
    Event event (Constants::Events::eInitial, Event::Priority::SYSTEM,
        _control);
    Queue::inject (&event);

    return (behavior ());
  }

//
// Provide this as simply a default that is intended to be overrided if it is
// needed to be useful.
//
int Actor::unhandled (Event *event)
  {
    return (0);
  }
