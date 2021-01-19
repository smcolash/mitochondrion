#include "stdio.h"
#include <string.h>

#include "mito/BehaviorNode.h"

int BehaviorNode::_index = 0;

//
//
//
BehaviorNode::BehaviorNode (const std::string &label)
  {
    name (label);
    _debug = Debug::NONE;
  }

BehaviorNode::BehaviorNode ()
  {
    char buffer[128];
    sprintf ((char *) &buffer, "s%d", _index++);
    name (std::string (buffer));
    _debug = Debug::NONE;
  }

//
//
//
BehaviorNode::~BehaviorNode ()
  {
  }

//
// test for transient behavior (pass-through)
//
int BehaviorNode::transient ()
  {
    return (TRUE);
  }

//
// accessor for debug level
//
int BehaviorNode::debug ()
  {
    return (_debug);
  }

//
// mutator for debug level
//
int BehaviorNode::debug (int level)
  {
    _debug = level;

    return (debug ());
  }

//
// accessor for name
//
const std::string BehaviorNode::name ()
  {
    return (_name);
  }

//
// mutator for name
//
const std::string BehaviorNode::name (const std::string &label)
  {
    _name = label;

    return (name ());
  }

//
//
//
Transition *BehaviorNode::evaluate (Event *event, Actor *actor)
  {
    return (NULL);
  }

