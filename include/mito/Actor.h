#ifndef __ACTOR_H__
#define __ACTOR_H__

/**
  * Include system definitions.
  */
#include <string>

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

/**
 * Include application constants.
 */
#include "mito/Constants.h"

/**
 * Handle forward references in the class definition.
 */
class Actor;
class Event;

/**
 * Definition of the standard method signature.
 */
typedef int (Actor::*METHOD) (class Event *);

/**
 * Include platform files.
 */
#include "mito/Component.h"
#include "mito/Port.h"
#include "mito/State.h"
#include "mito/Queue.h"
#include "mito/MitoLog.h"

/**
 * The Actor is the base class for all user-defined or application classes
 * used in the system. The basic characteristics that make it useful are the
 * method signature (METHOD) and the standardized 'inject' method.
 */
class __DLL__ Actor : public Component
  {
private:
    /**
     * Pointer to the actor's top-level state machine.
     */
    class State *_behavior;
    /**
      * Pointer to the actor's private control port.
      */
    class Port *_control;
    /**
     * Debug bit mask (see Constants.h).
     */
    int _debug;
    /**
     * Run-time name of the class.
     */
    std::string _type;
    /**
     * Run-time name of the actor.
     */
    std::string _name;
protected:
    /**
     * Class name mutator method.
     */
    virtual const std::string type (const std::string &id);
public:
    /**
     * Constructor method.
     */
    Actor ();
    /**
     * Destructor method.
     */
    virtual ~Actor ();
    /**
     * Top-level state machine accessor method.
     */
    virtual class State *behavior ();
    /**
     * Top-level state machine mutator method.
     */
    virtual class State *behavior (class State *top);
    /**
     * Debug bit mask accessor method.
     */
    virtual int debug ();
    /**
     * Debug bit mask mutator method.
     */
    virtual int debug (int mode);
    /**
     * Method to print the actor and internal stame machine decompositions.
     */
    virtual int print ();
    /**
     * Stream output function.
     */
    friend ostream& operator<< (ostream &os, Actor &actor);
    /**
     * Class name accessor method.
     */
    virtual const std::string type ();
    /**
     * Instance name accessor method.
     */
    virtual const std::string name ();
    /**
     * Instance name mutator method.
     */
    virtual const std::string name (const std::string &id);
    /**
     * Standard event injection method.
     */
    virtual int inject (Event *event);
    /**
     * Default callback for events unhandled by the internal behavior
     * (states) of the actor.
     */
    virtual int unhandled (Event *event);
  };

#endif

