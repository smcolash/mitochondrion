#include "mito/BehaviorNode.h"
#include "mito/State.h"

TransitionList::TransitionList ()
  {
    _size = 0;
  }

TransitionList::~TransitionList ()
  {
  }

int TransitionList::size ()
  {
    return (_size);
  }

Transition *TransitionList::add (int event, Port *port, BehaviorNode *terminal,
    METHOD method, int internal, const std::string &name)
  {
    if (_size < Constants::Length::List::TRANSITIONS)
      {
        Transition *temp = &_transitions[_size++];

        temp->event (event);
        temp->port (port);
        temp->node (terminal);
        temp->method (method);
        temp->internal (internal);
        temp->name (name);

        return (temp);
      }

    return (NULL);
  }

Transition *TransitionList::operator[] (int index)
  {
    return (&_transitions[index]);
  }

