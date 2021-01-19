#ifndef __BEHAVIORNODE_H__
#define __BEHAVIORNODE_H__

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
class Transition;
class Event;

/**
 * The BehaviorNode is the base class for all behavior objects such as the
 * State class and the ChoicePoint class. The basic characteristic of a
 * BehaviorNode object is that is is connected to other BehaviorNode objects
 * via Transitions.
 */
class __DLL__ BehaviorNode
  {
private:
    /**
     * An index to assist in naming the node if no name is presented.
     */
    static int _index;
    /**
     * A pointer to the actor that contains this behavior. Used in conjuntion
     * with MEHTOD pointers to execute guard and transition code.
     */
    class Actor *_actor;

private:
    /**
     * Debug bit mask (see Constatns.h).
     */
    int _debug;
    /**
     * Run-time name of the node.
     */
    std::string _name;

public:
    /**
     * Constructor method.
     */
    BehaviorNode (const std::string &label);
    BehaviorNode ();
    /**
     * Destructor method.
     */
    virtual ~BehaviorNode ();
    /**
     * Method to return whether the node is transient or not. A transient
     * node passes through to another transition while a not transient node
     * (State, for example) will be the destination of a transition. 
     */
    virtual int transient ();
    /**
     * Method to return the next transition to follow based on the input
     * Event.
     */
    virtual Transition *evaluate (Event *event, Actor *actor);
    /**
     * Debug bit mask accessor method.
     */
    virtual int debug ();
    /**
     * Debug bit mask mutator method.
     */
    virtual int debug (int);
    /**
     * Name accessor method.
     */
    virtual const std::string name ();
    /**
     * Name mutator method.
     */
    virtual const std::string name (const std::string &name);
  };

#endif

