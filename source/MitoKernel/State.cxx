//
// include system headers
//
#include <string>
#include <stdio.h>

//
// include application headers
//
#include "mito/State.h"

//
// state index number to be used as default name
//
int _index = 0;

//
// create a state object
//
State::State (const std::string &label)
  {
    name (label);
    debug (Debug::NONE);

    //debug (Debug::STATE || Debug::EVENT || Debug::COMPOSITION);

    _unhandled = 0;
    _super = NULL;
    _top = NULL;
    _active = NULL;
    _actor = NULL;
  }

//
// state object destructor
//
State::~State ()
  {
    if (top () == this)
      {
        destroy ();
      }
  }

//
// delete the current subtree of states
//
int State::destroy ()
  {
    int loop;

    for (loop = 0; loop < _states.size (); loop++)
      {
        if (_states[loop] != NULL)
          {
            _states[loop]->destroy ();
            delete _states[loop];
          }
      }

    return (FALSE);
  }

//
// acessor for the containing actor
//
Actor *State::actor ()
  {
    return (_top->_actor);
  }

//
// set this state as the top-level, active state
//
int State::activate ()
  {
    _active = this;
    _top = this;
    _super = (State *) 0;
    top (_top);
    return (FALSE);
  }

//
// activate and attach this state tree to an actor
//
int State::activate (Actor *actor)
  {
    activate ();
    _top->_actor = actor;
    return (FALSE);
  }

//
// accessor for the topmost state
//
State *State::top ()
  {
    return (_top);
  }

//
// mutator to set all references to the top state
//
State *State::top (State *state)
  {
    int loop;

    _top = state;
    for (loop = 0; loop < _states.size (); loop++)
      {
        _states[loop]->top (state);
      }

    return (top ());
  }

//
// accessor for the active state
//
State *State::active ()
  {
    if (top () == this)
      {
        return (_active);
      }

    return (top ()->active ());
  }

//
// mutator to set the active state
//
State *State::active (State *state)
  {
    if (top () == this)
      {
        _active = state;
      }
    else
      {
        top ()->active (state);
      }

    return (active ());
  }

//
// set the debug level of the current subtree of states
//
int State::debug (int level)
  {
    int loop;

    BehaviorNode::debug (level);
    for (loop = 0; loop < _states.size (); loop++)
      {
        _states[loop]->debug (level);
      }
    return (FALSE);  
  }

//
// handler for otherwise unhandled events
//
int State::unhandled (Event *event)
  {
    if (top () == this)
      {
        actor ()->unhandled (event);
        _unhandled++;

        return (_unhandled);
      }

    return (super ()->unhandled (event));
  }

//
// accessor for superstate
//
class State *State::super ()
  {
    return (_super);
  }

//
// define the initial transition
//
Transition *State::initial (State *terminal, const std::string &label, METHOD method)
  {
    return (transition (Constants::Events::eInitial, string ("eInitial"),
          NULL, terminal, label, method));
  }

//
// define an event action
//
Transition *State::transition (int event, const std::string &eventname,
    Port *port, BehaviorNode *terminal, const std::string &label, METHOD method)
  {
    BehaviorNode *node = (BehaviorNode *) this;
    int internal = TRUE;

    log.eventname (eventname, event);

    if (BehaviorNode::debug () & Debug::COMPOSITION)
      {
        if (terminal != NULL)
          {
            log.text (actor ()->name (),
                "registering state transition for event %d", event);
          }
        else
          {
            log.text (actor ()->name (),
                "registering internal transition for event %d", event);
          }
      }

    if (terminal != NULL)
      {
        node = terminal;
        internal = FALSE;
      }

    return (_transitions.add (event, port, node, (METHOD) method, internal, label));
  }

//
// define state entry behavior
//
int State::entry (const std::string &id, METHOD method)
  {
    if (BehaviorNode::debug () & Debug::COMPOSITION)
      {
        log.text (actor ()->name (), "registering entry code");
      }

    _entry.method (method);
    _entry.name (id);

    return (FALSE);
  }

//
// define state exit behavior
//
int State::exit (const std::string &id, METHOD method)
  {
    if (BehaviorNode::debug () & Debug::COMPOSITION)
      {
        log.text (actor ()->name (), "registering exit code");
      }

    _exit.method (method);
    _exit.name (id);

    return (FALSE);
  }

//
// insert a sub-state
//
int State::compose (State *state)
  {
  if (BehaviorNode::debug () & Debug::COMPOSITION)
      {
        log.text (actor ()->name (), "adding state %s to %s",
            state->name ().c_str (), name ().c_str ());
      }

    _states.add (state);
    state->_super = this;

    return (FALSE);
  }

//
// print the decomposition of states
//
int State::print (int level)
  {
    int loop;
    int transition;

    // print the state name
    for (loop = 0; loop < level; loop++)
      {
        cout << "  ";
      }
    cout << "(" << name () << ")";
    if (active () == this)
      {
        cout << "*";
      }
    if (top () == this)
      {
        cout << ", " << _unhandled << " unhandled events";
      }
    cout << "\n";

    // print the entry code function name
    if (_entry.name () != "")
      {
        for (loop = 0; loop < level; loop++)
          {
            cout << "  ";
          }
        cout << "    + " << _entry.name ();
        cout << " [" << _entry.count () << "]\n";
      }

    // print the exit code function name
    if (_exit.name () != "")
      {
        for (loop = 0; loop < level; loop++)
          {
            cout << "  ";
          }
        cout << "    - " << _exit.name ();
        cout << " [" << _exit.count () << "]\n";
      }

    // print the event, transition code function name and resulting state
    for (transition = 0; transition < _transitions.size (); transition++)
      {
        _transitions[transition]->print (cout, level);
      }

    // descend the tree and repeat
    for (loop = 0; loop < _states.size (); loop++)
      {
         _states[loop]->print (level + 1);
      }

    cout << flush;
    return (FALSE);
  }

//
//
//
std::ostream& operator<< (std::ostream &os, State &state)
  {
    int loop;

    /* FIXME */ int level = 0;

    // print the state name
    for (loop = 0; loop < level; loop++)
      {
        os << "  ";
      }
    os << "(" << state.name () << ")";
#if 0
    if (state.active () == state)
      {
        os << "*";
      }
    if (state.top () == state)
      {
        os << ", " << _unhandled << " unhandled events";
      }
#endif
    os << "\n";

#if 0
    // print the entry code function name
    if (state.entry.name () != "")
      {
        for (loop = 0; loop < level; loop++)
          {
            os << "  ";
          }
        os << "    + " << _entry.name ();
        os << " [" << _entry.count () << "]\n";
      }

    // print the exit code function name
    if (_exit.name () != "")
      {
        for (loop = 0; loop < level; loop++)
          {
            os << "  ";
          }
        os << "    - " << _exit.name ();
        os << " [" << _exit.count () << "]\n";
      }

    // print the event, transition code function name and resulting state
    for (transition = 0; transition < _transitions.size (); transition++)
      {
        _transitions[transition]->print (os, level);
      }

    // descend the tree and repeat
    for (loop = 0; loop < state.states().size (); loop++)
      {
         state.states()[loop]->print (level + 1);
      }
#endif

    return (os);
  }

//
//
//
int State::transient ()
  {
    return (FALSE);
  }

//
// inject a message into the state machine, return values: TRUE means event not handled
//
int State::inject (Event *event)
  {
    if (_top->_active)
      {
        if (!_top->_active->process (event))
          {
            return (FALSE); 
          }
      }

    log.text (actor ()->name (),
        "invalid/unregistered event: %d in %s@%s on port %s",
        event->event (), actor ()->name ().c_str (), _active->name ().c_str (),
        (event->to ())->name ().c_str ());

    unhandled (event);

    return (TRUE);
  }

//
// search for and process an event/action
//
int State::process (Event *event)
  {
    int loop;

    if (BehaviorNode::debug () & Debug::EVENT)
      {
        log.text (actor ()->name (), "state (%s) testing event %d",
            name ().c_str (), event->event ());
      }

    // search for matching event/action
    for (loop = 0; loop < _transitions.size (); loop++)
      {
        if (_transitions[loop]->test (event, actor ()))
          {
            //
            // find the complete transition path
            //
            Transition *temp;
            TransitionPath path;

            temp = _transitions[loop];
            if ((temp->node ())->transient ())
              {
                while ((temp != NULL) && (temp->node ())->transient ())
                  {
                    path.add (temp);
                    temp = (temp->node ())->evaluate (event, actor ());
                  }
              }

            if (temp != NULL)
              {
                path.add (temp);
              }

            // process an external state transition
            State *state;
            State *terminal;
            StateList exiting;
            StateList entering;
            StateList copy;
            int level;

            // find the states from the active state up
            state = active ();
            while (state != NULL)
              {
                exiting.add (state);
                state = state->super ();
              }

            // set the initial guess at the resulting state
//            terminal = (State *) _transitions[loop]->node ();
            terminal = (State *) path.terminal ();

            // find the states from the terminal state up
            state = terminal;
            while (state != NULL)
              {
                entering.add (state);
                state = state->super ();
              }

            // trim the lists at the nearest common state
            copy = exiting;
            exiting.join (entering);
            entering.join (copy);

            // test for self-transition
            if (_transitions[loop]->node () == this)
              {
                // test for super-state
                if (active () != this)
                  {
                    // correct the terminal state
                    terminal = active ();

                    // create the round trip path
                    entering = exiting;
                  }

                // add the current state if an external self-transition
                if (!_transitions[loop]->internal ())
                  {
                    exiting.add (this);
                  }

                // fix state entry path for self-transition
                entering = exiting;
              }

            // reverse the entry list
            entering.reverse ();

            // process state exit actions
            for (level = 0; level < exiting.size (); level++)
              {
                if (BehaviorNode::debug () & Debug::EXIT)
                  {
                    log.text (actor ()->name (), (char *) "--  -(%s) exit code",
                        exiting[level]->name ().c_str ());
                  }

                exiting[level]->_exit.fire (event, actor ());
              }

            // process state transition
            if (BehaviorNode::debug () & Debug::TRANSITION)
              {
                log.text (actor ()->name (), "--  %s transition code",
                    _transitions[loop]->name ().c_str ());
              }

            path.fire (event, actor ());

            // process state entry actions
            for (level = 0; level < entering.size (); level++)
              {
                if (BehaviorNode::debug () & Debug::ENTRY)
                  {
                    log.text (actor ()->name (), "--  +(%s) entry code",
                        entering[level]->name ().c_str ());
                  }

                entering[level]->_entry.fire (event, actor ());
              }

            //
            // log the change in the trace buffer
            //
            log.transition (actor ()->name (), event->event (),
                active ()->name (), terminal->name ());

            active (terminal);

            return (FALSE);
          }
      }

  // if no matching event found in this state, defer to parent state
  if (_super)
    {
      return (_super->process (event));
    }

  return (TRUE);
}

