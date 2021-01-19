#ifndef __PORT_H__
#define __PORT_H__

#include <string>

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

#include "mito/Constants.h"

class Actor;
class Event;
class Queue;
class MitoLog;
class Mutex;

/**
  * The 'Port' class is the basic inter-Actor communication interface. A pair
  * of these objects are contained within two Actor objects and logically
  * bound to each other at run-time. In this way the Actor class definitions
  * remain generic and unrelated to each other even when used within a
  * specific context or application.
  */
class __DLL__ Port
  {
private:
    /**
      * Holds a reference to the containing Actor instance.
      */
    Actor *_actor;
    /**
      * Holds the name given to the current instance.
      */
    std::string _name;
    /**
      * Holds a reference to the corresponding instance with which to exchange
      * messages.
      */
    class Port *_binding;
    /**
      * Holds a reference to the main system event queue.
      */
    Queue *_queue;
    /**
      *
      */
    int _timeout;
    /**
      * Holds a reference to the system logging facility such that the flow of
      * messages through the instance may be recorded for debugging and
      * verification if needed.
      */
    MitoLog *_log;
    /**
      * Holds a reference to a mutex object to allow safe message exchanges
      * with ports held on other threads of execution.
      */
    Mutex *_mutex;

public:
    /**
      * Constructor that provides the association to the containing Actor
      * object and a naming label.
      */
    Port (Actor *actor = (Actor *) NULL, const std::string &id = "");
    /**
      * Simple destructor.
      */
    virtual ~Port ();
    /**
      * Initialization method to associate the object with a containing Actor
      * object and assign a naming label.
      */
    int init (Actor *actor, const std::string &id);
    /**
      * Initialization method to associate the object with a containing Actor
      * object and assign a naming label.
      */
    int init (Actor *actor);
    /**
      * Static method to provide port associations or 'bindings'. These
      * bindings are a one-to-one bidirectional association that allows
      * messages to be passed at run-time.
      */
    static int bind (Port *first, Port *second);
    /**
      * Instance method that implements a binding within an object.
      */
    virtual int bind (Port *port);
    /**
      *
      */
    virtual int policy (int timeout);
    /**
      * Accessor method to retrieve the naming label of an instance.
      */
    virtual const std::string name ();
    /**
      * Accessor/mutator method to assign a naming label to an instance.
      */
    virtual const std::string name (const std::string &id);
    /**
      * Member function to return the name of the contining Actor object.
      */
    class Actor *actor ();
    /**
      * Accessor/mutator method to assign and retrieve the debug level of an
      * instance.
      */
    static int debug (int);
    /**
      * Method to inject a message into an instance. The message is relayed
      * directly to the containing Actor object, if possible.
      */
    virtual int inject (Event *);
    /**
      * Method to send a message out of the instance. The message is relayed
      * directly to the central event queue, if possible.
      */
    virtual int send (Event *);
    /**
      *
      */
    virtual int lock ();
    /**
      *
      */
    virtual int unlock ();
    /**
     * Method to print the contents of the port object during debugging.
     */
    virtual int print ();
  };

#endif

