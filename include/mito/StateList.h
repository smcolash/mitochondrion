#ifndef __STATELIST_H__
#define __STATELIST_H__

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

#include "mito/Constants.h"

class State;

//
//
//
class __DLL__ StateList
  {
private:
  int _size;
  State *_states[Constants::Length::String::STATE];

public:
    StateList ();
    int destroy ();
    ~StateList ();
    int size ();
    int add (State *state);
    int trim (int index);
    State *operator[] (int index);
    int join (StateList &list);
    int reverse ();
  };

#endif // __STATE_H__

