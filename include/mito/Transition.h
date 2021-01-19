#ifndef __TRANSITION_H__
#define __TRANSITION_H__

/**
 * Include system headers.
 */
#include <iostream>
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
 * Include application object definitions.
 */
#include "mito/Actor.h"
#include "mito/BehaviorNode.h"
#include "mito/State.h"
#include "mito/Port.h"

/**
 * The Guard is an object that can be used to direct the flow of control or
 * the choice of a specific transition during event processing. The method
 * that is associated with the Guard is eveluated within the contect of an
 * encapsulating Actor (or compatible sub-class). The number of attempts and
 * actual 'true' results are kept for debug and analysis purposes.
 */
class __DLL__ Guard
  {
private:
    /**
     * A pointer to a method defined in the encapsulating Actor. The flow of
     * control is directed by the return value of this method.
     */
    METHOD _method;
    /**
     * The string name of the Guard.
     */
    std::string _name;
    /**
     * The number of times this guard has been evaluated.
     */
    int _attempt;
    /**
     * The number of times this Guard has evaluated to 'true'.
     */
    int _count;

public:
    /**
     * A simple constructor.
     */
    Guard ();
    /**
     * A simple destructor.
     */
    ~Guard ();
    /**
     * A mutator method that sets the pointer to the method and the name of
     * the Guard.
     */
    int set (METHOD method, const std::string &label);
    /**
     * An accessor function that calls the method within the context of
     * another Actor object. An event is supplied to allow the method to be
     * evaluated based on any combination of the Actor state and/or contents
     * of the incoming Event.
     */
    int action (Actor *actor, Event *event);
    /**
     * A method to print the relevant Guard data to an output stream.
     */
    ostream &print (ostream &os, int level);
  };

/**
 * The GuardList is a class that maintains a list of Guard instances. Once
 * added, the Guard instances may be evaluated within the contect of an
 * encapsulating Actor. The evaluation of any of the contained Guard instances
 * as 'true' will cause the GuardList to also evaluate as 'true'.
 */
class __DLL__ GuardList
  {
private:
    /**
     * The number of Guard instances held in the list.
     */
    int _size;
    /**
     * The list of Guard instances.
     */
    Guard _guards[Constants::Length::List::GUARDS];

public:
    /**
     * A simple constructor.
     */
    GuardList ();
    /**
     * A simple destructor.
     */
    ~GuardList ();
    /**
     * An accessor method to return the number of Guard instances held in the
     * list.
     */
    int size ();
    /**
     * A mutator method to add a Guard to the end of the list.
     */
    int add (const std::string &label, METHOD function);
    /**
     * An accessor function that evaluates the Guard instances contained
     * in the list within the context of another Actor object. An event is
     * supplied to allow the evaluation to be based on any combination of
     * the Actor state and/or contents of the incoming Event.
     */
    int action (Actor *actor, Event *event);
    /**
     * A method to print the relevant GuardList data to an output stream.
     */
    ostream &print (ostream &os, int level);
  };

/**
 * The Transistion is a class that...
 */
class __DLL__ Transition
  {
private:
    /**
     *
     */
    Port *_port;
    /**
     *
     */
    int _event;
    /**
     *
     */
    int _internal;
    /**
     *
     */
    METHOD _method;
    /**
     *
     */
    std::string _name;
    /**
     *
     */
    int _count;
    /**
     *
     */
    class BehaviorNode *_node;
    /**
     *
     */
    GuardList _guards;

public:
    /**
     *
     */
    Transition ();
    /**
     *
     */
    ~Transition ();
    /**
     *
     */
    int event ();
    /**
     *
     */
    int event (int id);
    /**
     *
     */
    const Port *port ();
    /**
     *
     */
    const Port *port (Port *item);
    /**
     *
     */
    int internal ();
    /**
     *
     */
    int internal (int type);
    /**
     *
     */
    METHOD method ();
    /**
     *
     */
    METHOD method (METHOD pointer);
    /**
     *
     */
    BehaviorNode *node ();
    /**
     *
     */
    BehaviorNode *node (BehaviorNode *destination);
    /**
     *
     */
    const std::string name ();
    /**
     *
     */
    const std::string name (const std::string &id);
    /**
     *
     */
    int init (int id, BehaviorNode *destination, const std::string &label,
        METHOD function);
    /**
     *
     */
    int guard (const std::string &label, METHOD function);
    /**
     *
     */
    int count ();
    /**
     *
     */
    METHOD action ();
    /**
     *
     */
    int test (Event *event, Actor *actor);
    /**
     *
     */
    int fire (Event *event, Actor *actor);
    /**
     *
     */
    ostream &print (ostream &os, int level);
  };

/**
 *
 */
class __DLL__ TransitionList
  {
private:
    /**
     *
     */
    int _size;
    /**
     *
     */
    Transition _transitions[Constants::Length::List::TRANSITIONS];

public:
    /**
     *
     */
    TransitionList ();
    /**
     *
     */
    ~TransitionList ();
    /**
     *
     */
    int size ();
    /**
     *
     */
    Transition *add (int event, Port *port, BehaviorNode *terminal, METHOD code,
        int internal, const std::string &name);
    /**
     *
     */
    Transition *operator[] (int index);
    /**
     *
     */
    ostream &print (ostream &os, int level);
  };

/**
 *
 */
class __DLL__ TransitionPath
  {
private:
    /**
     *
     */
    Transition *_path[10];
    /**
     *
     */
    int _size;

public:
    /**
     *
     */
    TransitionPath ();
    /**
     *
     */
    ~TransitionPath ();
    /**
     *
     */
    int add (Transition *transition);
    /**
     *
     */
    int fire (Event *event, Actor *actor);
    /**
     *
     */
    BehaviorNode *terminal ();
  };

#endif

