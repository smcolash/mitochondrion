#include <string>

#include "mito/ChoicePoint.h"
#include "mito/Transition.h"

ChoicePoint::ChoicePoint (const std::string &label)
  {
    name (label);
    _guard = NULL;
  }

ChoicePoint::~ChoicePoint ()
  {
  }

int ChoicePoint::guard (const std::string &label, METHOD pointer)
  {
    _guard = pointer;
    
    return (FALSE);
  }

int ChoicePoint::evalTrue (BehaviorNode *node, METHOD pointer,
    const std::string &label)
  {
    _true.init (0, node, label, (METHOD) pointer);

    return (FALSE);
  }

int ChoicePoint::evalFalse (BehaviorNode *node, METHOD pointer,
    const std::string &label)
  {
    _false.init (0, node, label, (METHOD) pointer);

    return (FALSE);
  }

Transition *ChoicePoint::evaluate (Event *event, Actor *actor)
  {
    if ((actor->*_guard) (event))
      {
        //
        // 'true' case
        //
        return (&_true);
      }
    else
      {
        //
        // 'false' case
        //
        return (&_false);
      }

    return (NULL);
  }

