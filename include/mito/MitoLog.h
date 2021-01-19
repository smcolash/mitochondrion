#ifndef __MITOLOG_H__
#define __MITOLOG_H__

//
// include system headers
//
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string>

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

//
// include application headers
//
#include "Mutex.h"
#include "WebServer.h"

/**
  * This structure defines the actual storage of an individual log entry.
  */
class __DLL__ MitoLogRecord
  {
public:
    /**
     * Enumeration of the types of events that can be logged.
     */
    typedef enum
      {
        INVALID = 0,
        TEXT,
        CODE,
        EVENT_SEND,
        EVENT_RECEIVE,
        STATE,
        TIMER_SET,
        TIMER_CANCEL,
        TIMER_EXPIRE,
        MUTEX_TAKE,
        MUTEX_GIVE,
        ALARM_SET,
        ALARM_CLEAR
      } EVENT_TYPE;

    /**
     * Enumeration of the types of alarms that can be logged.
     */
    typedef enum
      {
        CLEAR = 0,
        WARNING,
        CRITICAL,
        FAULT
      } ALARM_TYPE;

public:
    MitoLogRecord ();
    MitoLogRecord (MitoLogRecord::EVENT_TYPE event_type,
        const std::string &context);
    ~MitoLogRecord ();

    int copy (const MitoLogRecord &record);
    void clear (void);

public:
    /**
      * The high-resolution timestamp of the event being logged.
      */
    double timestamp;
    /**
      * Enumeration to describe what type of event is being logged.
      */
    EVENT_TYPE type;
    /**
      * The text label of the event being logged.
      */
    char label[32];
    /**
      * The name of the actor or software entity logging the event.
      */
    char actor[32];
    /**
      * The data being logged to describe the event.
      */
    union
      {
        /**
          * Free-text to describe the event.
          */
        char text[128];
        /**
          * Hold data about a timer or timer event.
          */
        struct
          {
            char name[64];
            int value;
          } timer;
        struct
          {
            char name[64];
          } mutex;
        struct
          {
            ALARM_TYPE condition;
          } alarm;
        struct
          {
            int code;
            char from[64];
            char to[64];
          } event;
        struct
          {
            char from[64];
            char to[64];
          } state;
      } data;
  };

/**
  * This structure defined the association between two communication endpoints
  * in the association log data.
  */
typedef struct
  {
    /**
      * The sending endpoint of the message.
      */
    std::string sender;
    /**
      * The receiving endpoint of the message.
      */
    std::string receiver;
    /**
      * The number of times this path has been taken.
      */
    unsigned int count;
  } MitoAssocRecord;

class __DLL__ MitoAssoc
  {
private:
    unsigned int _size;
    unsigned int _count;
    MitoAssocRecord *_data;

public:
    MitoAssoc (unsigned int size = 100)
      {
        _size = size;
        _count = 0;
        _data = new MitoAssocRecord[size];
      };

    ~MitoAssoc (void)
      {
        if (_data)
          {
            delete [] _data;
          }
      };

    unsigned int size (void)
      {
        return (_count);
      };

    int add (const std::string &from, const std::string &to)
      {
        int unsigned loop;
        for (loop = 0; loop < _count; loop++)
          {
            if ((from == _data[loop].sender) &&
                (to == _data[loop].receiver))
              {
                _data[loop].count++;
                 return (0);
              }
          }
        //
        // add a new one
        //
        _data[_count].sender = from;
        _data[_count].receiver = to;
        _data[_count].count++;
        _count++;
        return (0);
      };

    const std::string sender (int index)
      {
        return (_data[index].sender);
      };

    const std::string receiver (int index)
      {
        return (_data[index].receiver);
      };

    unsigned int count (int index)
      {
        return (_data[index].count);
      };
  };

/**
  * The MitoLog class implements a general purpose trace buffer that can log a
  * variety of event types such as inter-object messages, finite-state-machine
  * events and general text annotation. An instance of the object can be
  * configured to have any number of entries held internally in a circular
  * buffer. Access is provided to this buffer through method calls and through
  * a web-server thread that runs specifically to service a single instance of
  * this class.
  */
class __DLL__ MitoLog
  {
private:
    /**
      * The index to the current element in the circular buffer of history
      * data.
      */
    static unsigned long int _index;
    /**
      * The total number of elements configured to be held in the circular
      * history buffer.
      */
    static unsigned long int _size;
    /**
      * The base address of the circular buffer data.
      */
    static MitoLogRecord *_data;
    /**
      * The interface to the embedded web server.
      */
    static WebServer _server;
    /**
      * The mutex that is used to synchronize access to the circular history
      * buffer.
      */
    static Mutex *_mutex;
    /**
      *
      */
    static int _count;
    /**
      * The name of the log object as shown in the history buffer data.
      */
    static std::string _name;
    /**
      * The URL that the web server presented the history buffer data as.
      */
    static std::string _url;
    /**
      * A list of communication paths generated from the 'event' log entries.
      */
    static MitoAssoc* _assoc;

private:
    /**
      * Copy a record into the active log data.
      */
    int add (const MitoLogRecord &record);
    /**
      *
      */
    int add (const std::string &label, const std::string &actor,
        const char *format, va_list list);
    /**
      *
      */
    int add (const std::string &label, const std::string &actor,
        const char *format, ...);

public:
    /**
      * The default constructor. The number of events that can be held in the
      * circular buffer can be specified as the argument to this constructor.
      * The default number of entries is '1024'.
      */
    MitoLog (int size = 1024, const std::string &name = "");
    /**
      * The default destructor. This shuts down the web server thread and
      * cleans up all of the internal storage used by the object.
      */
    ~MitoLog ();
    /**
      * Get the name of the log instance.
      */
    const std::string name ();
	/**
	  * Get the port of the underlying web server.
	  */
	int port ();
    /**
      * Set the port of the underlying web server.
      */
    int port (int number);
    /**
      * Get the relative URL of the log data on the web server.
      */
    const std::string url ();
    /**
      * Set the relative URL of the log data on the web server.
      */
    const std::string url (const std::string &name);
    /**
      * Start the underlying web server.
      */
    int start ();
    /**
      * Stop the underlying web server.
      */
    int stop ();
    /**
      * Access the pointer to the underlying web server.
      */
    WebServer *server (void);
    /**
      * Add a text entry to the history data.
      */
    int text (const std::string &actor, const char *format, ...);
    /**
      * Add a state transition to the history data.
      */
    int transition (const std::string &actor, int code,
        const std::string &from,
        const std::string &to);
    /**
      * Add a code event entry to the history data.
      */
    int code (const std::string &actor, const std::string &name, int action);
    /**
     * Add an event enumeration translation.
     */
    int eventname (const std::string &name, int code);
    /**
      * Add an event/message to the history data.
      */
    int event (const std::string &actor, int code,
        const std::string &from, const std::string &to);
    /**
      * Add a timer set event to the history data.
      */
    int timer (const std::string &actor, const std::string &name, int timeout);
    /**
      * Add a timer cancellation event to the history data.
      */
    int cancel (const std::string &actor, const std::string &name);
    /**
      * Add a timeout event to the history data.
      */
    int timeout (const std::string &actor, const std::string &name);
    /**
      * Add a mutex event to the history data.
      */
    int mutex (const std::string &actor, int state, const std::string &name);
    /**
      * Add an alarm event to the history data.
      */
    int alarm (const std::string &actor, MitoLogRecord::ALARM_TYPE condition);
    /**
      * Read a set of history data from a file descriptor.
      */
    int read (int fd);
    /**
      * Produce the history data as HTTP/HTML text through the underlying web
      * server.
      */
    int http ();
    /**
      * Produce the data gathered through access to the log objects as an
      * association diagram in 'dot' format.
      */
    int assoc ();
    /**
      * Produce the history data as simple text.
      */
    int print (FILE *file = stdout);
    /**
      * Print text to the standard output device with automatic line wrapping.
      */
    static void emit (const std::string &text);
  };

#endif
