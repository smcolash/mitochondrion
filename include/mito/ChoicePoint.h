#ifndef __CHOICEPOINT_H__
#define __CHOICEPOINT_H__

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

/**
 * Include application object definitions.
 */
#include "mito/Actor.h"
#include "mito/Transition.h"

/**
 * Include the definition of the base class.
 */
#include "mito/BehaviorNode.h"

/**
 * Resolve forward class references.
 */
class Event;

/**
 * The ChoicePoint is a class that implements a pass-though behavior (unlike a
 * State) that can be used to control the path a transition may take. The
 * ChoicePoint is used by defining one input transition leading into it, a
 * control or guard function and two output transitions. The input transition
 * code is executed, then the appropriate output transition is executed, based
 * on the evaluation of the guard function. The resulting behavior or
 * destination is defined by the endpoint of the selected output transition.
 * Note that this destination may be any other BehaviorNode such as a State or
 * another ChoicePoint.
 */
class __DLL__ ChoicePoint : public BehaviorNode
  {
private:
    /**
     * A pointer to the guard function that controls the choice of output
     * transition to follow.
     */
    METHOD _guard;
    /**
     * The transition to follow on a 'true' evaluation of the guard function.
     */
    Transition _true;
    /**
     * The transition to follow on a 'false' evaluation of the guard function.
     */
    Transition _false;

public:
    /**
     * Constructor method.
     */
    ChoicePoint (const std::string &label = "");
    /**
     * Destructor method.
     */
    ~ChoicePoint ();
    /**
     * Mutator method that installs the guard function into the object.
     */
    int guard (const std::string &label, METHOD pointer);
    /**
     *  Mutator method that installs the 'true' transition into the object.
     */
    int evalTrue (BehaviorNode *node, METHOD pointer, const std::string &label);
    /**
     *  Mutator method that installs the 'true' transition into the object.
     */
    int evalFalse (BehaviorNode *node, METHOD pointer, const std::string &label);
    /**
     * Method to evaluate the behavior of the object based on the current
     * input event and potentially other application-specific data.
     */
    Transition *evaluate (Event *event, Actor *actor);
  };

#endif

