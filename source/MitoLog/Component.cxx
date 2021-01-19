#include <iostream>
#include <string>
#include <set>

#include "mito/Component.h"
#include "mito/Constants.h"

using namespace std;

Component::Component ()
  {
    _debug = Debug::NONE;
    type (__FUNCTION__);
    name (__FUNCTION__);

    return;
  }

Component::~Component ()
  {
    return;
  }

//
// set the debug level of the current subtree of states
//
int Component::debug ()
  {
    return (_debug);
  }

int Component::debug (int level)
  {
    _debug = level;
    return (debug ());
  }

const std::string Component::type ()
  {
    return (_type);
  }

const std::string Component::type (const std::string &value)
  {
    if (value != "")
      {
        _type = value;
      }

    return (type ());
  }

const std::string Component::name ()
  {
    return (_name);
  }

const std::string Component::name (const std::string &value)
  {
    if (value != "")
      {
        _name = value;
      }

    return (name ());
  }

int Component::print ()
  {
    return (0);
  }


