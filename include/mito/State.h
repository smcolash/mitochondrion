#ifndef __STATE_H__
#define __STATE_H__

#include <iostream>
#include <string>

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

#include "mito/Constants.h"

#include "mito/Event.h"
#include "mito/Actor.h"

class Actor;
class State;

#include "mito/BehaviorNode.h"
#include "mito/StateList.h"
#include "mito/Transition.h"
#include "mito/MitoLog.h"

//
//
//
class __DLL__ State : public BehaviorNode
  {
private:
    int _unhandled;
    MitoLog log;

    StateList _states;

    Transition _entry;
    Transition _exit;
    TransitionList _transitions;

    class Actor *_actor;

    class State *_active;
    class State *_super;
    class State *_top;

    int process (Event *event);

public:
    State (const std::string &name = "");
    ~State ();

    //
    // transition initializers
    //
    Transition *initial (State *state, const std::string &name, METHOD method);
    Transition *transition (int event, const std::string &eventname, Port *port,
        BehaviorNode *node, const std::string &name, METHOD method);
    int entry (const std::string &name, METHOD method);
    int exit (const std::string &name, METHOD method);

    //
    // structure initializers
    //
    int compose (State *);
    class State *top ();
    class State *top (State *);
    class State *active ();
    class State *active (State *);
    class Actor *actor ();
    int activate ();
    int activate (Actor *);
    class State *super ();
    int destroy ();

    //
    // debug and informational methods
    //
    virtual int debug (int);
    virtual int print (int = 0);
    /**
     * Stream output function.
     */
    friend ostream& operator<< (ostream &os, State &state);

    virtual int unhandled (Event *event);

    //
    // structure iterators
    //
    int transient ();

    //
    // message injection method
    //
    int inject (Event *);
  };

#endif // __STATE_H__

