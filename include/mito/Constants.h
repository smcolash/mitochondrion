#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#ifdef WIN32
#include <windows.h>
#include <process.h>
#endif

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

/**
 * Define TRUE, just in case the system does not.
 */
#ifndef TRUE
#define TRUE (1)
#endif

/**
 * Define FALSE, just in case the system does not.
 */
#ifndef FALSE
#define FALSE (0)
#endif

/**
 * Define NULL, just in case the system does not.
 */
#ifndef NULL
#define NULL ((void *) 0)
#endif

/**
 * Constants used in the subsystem objects.
 */
class __DLL__ Constants
  {
public:
    /**
     * Mandatory system-defined events.
     */
    class __DLL__ Events
      {
    public:
      typedef enum Code
        {
          eWildcard = -1,         ///< A wildcard pseudo-event that matches all.
          eInitial = -2,          ///< The initial state transition event.
          eTerminate = -3,        ///< A queue shutdown request.
          eTimer = -4,            ///< A generic timer event.
          eFile = -5              ///< A generic file descriptor event.
        } _Code;
      };

    /**
     * Array length defaults.
     */
    class __DLL__ Length
      {
    public:
        /**
         * Maximum length of various object name strings.
         */
        class __DLL__ String
          {
        public:
            typedef enum Length
              {
                ACTOR = 80,     ///< actor name string length
                PORT = 80,      ///< port name string length
                STATE = 40,     ///< state name string length
                CODE = 80       ///< code (transition, etc) name string length
              } _Length;
          };

        /**
         * Maximum size of internal storage structures.
         */
        class __DLL__ List
          {
        public:
            typedef enum Length
              {
                STATES = 10,      ///< maximum nesting level of states
                TRANSITIONS = 10, ///< maximum number of transitions per state
                GUARDS = 5,       ///< maximum number of guards per transition
                QUEUE = 256,      ///< maximum number of pending events in one rung of the queue
                TIMER = 128       ///< maximum number of pending timer events
              } _Length;
          };
      };
  };

/**
 * Debug bit mask definitions.
 */
class __DLL__ Debug
  {
public:
    typedef enum Category
      {
        NONE =          0x00000000, ///< no debug options set
        ENTRY =         0x00000001, ///< show entry actions
        EXIT =          0x00000002, ///< show exit actions
        TRANSITION =    0x00000004, ///< show transition actions
        EVENT =         0x00000008, ///< show event processing actions
        STATE =         0x00000010, ///< show state actions
        INJECT =        0x00000020, ///< show event injection
        COMPOSITION =   0x00000040, ///< show state initialization
        TIMEOUT =       0x00000080  ///< show timer queue expirations
      } _Category;
  };

#endif

