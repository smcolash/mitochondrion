#include "mito/State.h"

StateList::StateList ()
  {
    int loop;

    _size = 0;
    for (loop = 0; loop < Constants::Length::List::STATES; loop++)
      {
        _states[loop] = NULL;
      }
  }

int StateList::destroy ()
  {
    int loop;
    for (loop = 0; loop < _size; loop++)
      {
        if (_states[loop] != NULL)
          {
            delete &_states[loop];
            _states[loop] = NULL;
          }
      }

    return (0);
  }

StateList::~StateList ()
  {
  }

int StateList::size ()
  {
    return (_size);
  }

int StateList::add (State *state)
  {
  if (_size < Constants::Length::List::TRANSITIONS)
      {
        _states[_size] = state;
        _size++;
      }

    return (size ());
  }

int StateList::trim (int index)
  {
    if (index < size ())
      {
        int loop;

        for (loop = index; loop < size (); loop++)
          {
            _states[loop] = NULL;
          }
        _size = index;
      }

    return (size ());
  }

State *StateList::operator[] (int index)
  {
    return (_states[index]);
  }

int StateList::join (StateList &list)
  {
    int outer;
    int inner;

    for (outer = 0; outer < size (); outer++)
      {
        for (inner = 0; inner < list.size (); inner++)
          {
            if (_states[outer] == list[inner])
              {
                trim (outer);
                break;
              }
          }
      }

    return (size ());
  }

int StateList::reverse ()
  {
    int loop;

    for (loop = 0; loop < (size () / 2); loop++)
      {
        State *temp;

        temp = _states[loop];
        _states[loop] = _states[size () - loop - 1];
        _states[size () - loop - 1] = temp;
      }

    return (size ());
  }

