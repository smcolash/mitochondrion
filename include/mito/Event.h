#ifndef __EVENT_H__
#define __EVENT_H__

/**
 * Include system headers.
 */
#include <iostream>
#include <string>
using namespace std;

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

/**
 * Include application constants.
 */
#include "mito/Constants.h"

/**
 * The Event is the base class for all system and user-defined event messages
 * passed through the event queue.  The basic characteristics that make it
 * useful are the event code, priority and source/destination data.
 */
class __DLL__ Event
  {
public:
    /**
     * Queue and message priorities.
     */
    class __DLL__ Priority
      {
    public:
        typedef enum Level
          {
            LOW,        ///< low event priority
            MEDIUM,     ///< medium event priority
            NORMAL,     ///< normal event priority
            HIGH,       ///< high event priority
            CRITICAL,   ///< critical event priority
            SYSTEM      ///< system-level event priority
          } _Level;

        /**
         * A method to format an event priority into a readable string.
         */
        static const std::string name (Priority::Level level)
          {
            switch (level)
              {
                case Event::Priority::LOW:
                  return ("Event::Priority::LOW");
                case Event::Priority::MEDIUM:
                  return ("Event::Priority::MEDIUM");
                case Event::Priority::NORMAL:
                  return ("Event::Priority::NORMAL");
                case Event::Priority::HIGH:
                  return ("Event::Priority::HIGH");
                case Event::Priority::CRITICAL:
                  return ("Event::Priority::CRITICAL");
                case Event::Priority::SYSTEM:
                  return ("Event::Priority::SYSTEM");
              }

            return ("undefined");
          }
      };

private:
    /**
     * Numeric event code.
     */
    int _event;
    /**
     * Event priority.
     */
    Event::Priority::Level _priority;
    /**
     * Source port.
     */
    class Port *_from;
    /**
     * Destination port.
     */
    class Port *_to;

public:
    /**
     * Simple constructor method.
     */
    Event ();
    /**
     * Constructor method with field initialization.
     */
    Event (int, Event::Priority::Level = Event::Priority::NORMAL,
        Port *to = NULL, Port *from = NULL);
    /**
     * Copy constructor.
     */
    Event (Event *other);
    /**
     * Destructor method.
     */
    virtual ~Event ();
    /**
     * Event code accessor method.
     */
    virtual int event ();
    /**
     * Event code mutator method.
     */
    virtual int event (int value);
    /**
     * Event priority accessor method.
     */
    virtual Event::Priority::Level priority ();
    /**
     * Event priority mutator method.
     */
    virtual Event::Priority::Level priority (Event::Priority::Level);
    /**
     * Destination port accessor method.
     */
    virtual class Port *to ();
    /**
     * Destination port mutator method.
     */
    virtual class Port *to (Port *port);
    /**
     * Source port accessor method.
     */
    virtual class Port *from ();
    /**
     * Source port mutator method.
     */
    virtual class Port *from (Port *port);
    /**
     * Stream output function.
     */
    friend ostream& operator<< (ostream &os, Event &event);
    /**
     * Copy method. This must be re-implemented as necessary in derived
     * classes to perform a deep copy.
     */
    virtual Event *copy ();
  };

#endif

