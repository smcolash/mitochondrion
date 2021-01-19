#ifndef __COMPONENT_H__
#define __COMPONENT_H__

/**
  * Include system definitions.
  */
#include <string>

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

/**
 * The Component is the base class for all classes that encapsulate or contain
 * behavior. Providing this base class allows for generic method callback
 * definitions that do not specifically imply a level of architecture.
 */
class __DLL__ Component
  {
private:
    /**
     * Debug bit mask (see Constants.h).
     */
    int _debug;
    /**
     * Run-time type of the class.
     */
    std::string _type;
    /**
     * Run-time name of the actor.
     */
    std::string _name;
protected:
    /**
     * Class name mutator method.
     */
    virtual const std::string type (const std::string &id);
public:
    /**
     * Constructor method.
     */
    Component ();
    /**
     * Destructor method.
     */
    virtual ~Component ();
    /**
     * Debug bit mask accessor method.
     */
    virtual int debug ();
    /**
     * Debug bit mask mutator method.
     */
    virtual int debug (int mode);
    /**
     * Method to print the actor and internal stame machine decompositions.
     */
    virtual int print ();
    /**
     * Class name accessor method.
     */
    virtual const std::string type ();
    /**
     * Instance name accessor method.
     */
    virtual const std::string name ();
    /**
     * Instance name mutator method.
     */
    virtual const std::string name (const std::string &id);
  };

#endif

