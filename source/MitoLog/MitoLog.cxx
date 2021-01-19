//
// include system headers
//
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <map>

#ifdef WIN32
#include <windows.h>
#include <mmsystem.h>
#else
#include <time.h>
#include <sys/time.h>
#endif

#include <string>

//
// include application headers
//
#include "mito/MitoLog.h"
#include "mito/Mutex.h"
#include "mito/WebServer.h"

//
// static/singleton data
//
unsigned long int MitoLog::_index = 0;
unsigned long int MitoLog::_size = 0;
MitoLogRecord *MitoLog::_data = NULL;
WebServer MitoLog::_server;
Mutex *MitoLog::_mutex = new Mutex;
int MitoLog::_count = 0;
std::string MitoLog::_name = "";
std::string MitoLog::_url = "";
MitoAssoc *MitoLog::_assoc = new MitoAssoc (100);
std::map <const std::string, MitoLogRecord::ALARM_TYPE> _alarms;
std::map <const std::string, int> EventList;

//
// FIXME...
//
extern double timestamp ();

/**
  * Callback to handle access to the history data through the web server.
  */
static int history_xml_callback (WebServer *server, const std::string &command,
    const std::string &url)
  {
    MitoLog log;
    log.http ();
    return (FALSE);
  }

/**
  * Callback to handle access to the association data through the web server.
  */
static int association_xml_callback (WebServer *server, const std::string &command,
    const std::string &url)
  {
    MitoLog log;
    log.assoc ();
    return (FALSE);
  }

/**
  * Include data to represent the JavaScript source to be presented through
  * the callback.
  */
unsigned char association_js[] =
  {
#include "association.data"
  };

/**
  * Callback to provide JavaScript code to process the log data.
  */
static int association_js_callback (WebServer *server, const std::string &command,
    const std::string &url)
  {
    server->produce (200, "text/javascript", "");

    server->produce ((const char *) association_js, sizeof (association_js));

    return (FALSE);
  }

/**
  * Include data to represent the JavaScript source to be presented through
  * the callback.
  */
unsigned char style_css[] =
  {
#include "style.data"
  };

/**
  * Callback to provide CSS for the pages.
  */
static int style_css_callback (WebServer *server, const std::string &command,
    const std::string &url)
  {
    server->produce (200, "text/css", "");

    server->produce ((const char *) style_css, sizeof (style_css));

    return (FALSE);
  }

/**
  * Callback to provide an association diagram page.
  */
static int association_html_callback (WebServer *server, const std::string &command,
    const std::string &url)
  {
    server->produce (200, "text/html", "");

    server->produce ("<html>\n");
    server->produce ("<head>\n");
    server->produce ("<link rel='stylesheet' type='text/css' href='style.css' />\n");
    server->produce ("<!--[if lt IE 9]>\n");
    server->produce ("<script src='/excanvas.js'></script>\n");
    server->produce ("<![endif]-->\n");
    server->produce ("<script type='text/javascript' src='/association.js'></script>\n");
    server->produce ("<script>\n");
    server->produce ("function update ()\n");
    server->produce ("  {\n");
    server->produce ("    fetch ('/association.xml', 'diagram', association);\n");
    server->produce ("    return;\n");
    server->produce ("  }\n");
    server->produce ("window.addEventListener ('load', update, false);\n");
    server->produce ("window.addEventListener ('resize', invalidate, false);\n");
    server->produce ("</script>\n");
    server->produce ("\n");
    server->produce ("</head>\n");
    server->produce ("<body>\n");
    server->produce ("<div class='title'>Object Association and Status</div>\n");
    server->produce ("<canvas class='diagram' id='diagram' width='800' height='600'>\n");
    server->produce ("Incompatible browser.\n");
    server->produce ("</canvas>\n");
    server->produce ("<br>Update Interval: <input id='delay' type='text' value='10'><button onclick='update ();'>Refresh</button><br>\n");
    //server->produce ("<textarea id='debug' rows=10 cols=80></textarea>\n");
    server->produce ("</body>\n");
    server->produce ("</html>\n");

    return (FALSE);
  }

/**
  * Include data to represent the JavaScript source to be presented through
  * the callback.
  */
unsigned char history_js[] =
  {
#include "history.data"
  };

/**
  * Callback to provide JavaScript code to process the log data.
  */
static int history_js_callback (WebServer *server, const std::string &command,
    const std::string &url)
  {
    server->produce (200, "text/javascript", "");

    server->produce ((const char *) history_js, sizeof (history_js));

    return (FALSE);
  }

/**
  * Include data to represent the JavaScript source to be presented through
  * the callback.
  */
unsigned char excanvas_js[] =
  {
#include "excanvas.data"
  };

/**
  * Callback to provide JavaScript code to process the log data.
  */
static int excanvas_js_callback (WebServer *server, const std::string &command,
    const std::string &url)
  {
    server->produce (200, "text/javascript", "");

    server->produce ((const char *) excanvas_js, sizeof (excanvas_js));

    return (FALSE);
  }

/**
  * Include data to represent the site icon to be presented through
  * the callback.
  */
unsigned char favicon_ico[] =
  {
#include "favicon.data"
  };

/**
  * Callback to provide JavaScript code to process the log data.
  */
static int favicon_ico_callback (WebServer *server, const std::string &command,
    const std::string &url)
  {
    server->produce (200, "image/x-icon", "");

    server->produce ((const char *) favicon_ico, sizeof (favicon_ico));

    return (FALSE);
  }

/**
  * Callback to provide a message sequence chart page.
  */
static int history_html_callback (WebServer *server, const std::string &command,
    const std::string &url)
  {
    server->produce (200, "text/html", "");

    server->produce ("<html>\n");
    server->produce ("<head>\n");
    server->produce ("<link rel='stylesheet' type='text/css' href='style.css' />\n");
    server->produce ("<!--[if lt IE 9]>\n");
    server->produce ("<script src='/excanvas.js'></script>\n");
    server->produce ("<![endif]-->\n");
    server->produce ("<script language='javascript' src='/history.js'></script>\n");
    server->produce ("<script language='javascript'>\n");
    server->produce ("window.onload = function ()\n");
    server->produce ("  {\n");
    server->produce ("    fetch ('/history.xml', 'canvas', msc);\n");
    server->produce ("  };\n");
    server->produce ("</script>\n");
    server->produce ("</head>\n");
    server->produce ("<body>\n");
    server->produce ("<div class='title'>Message Sequence Diagram</div>\n");
    server->produce ("<canvas id='canvas' width='200' height='200'>");
    server->produce ("Incompatible browser.");
    server->produce ("</canvas>\n");
    server->produce ("</body>\n");
    server->produce ("</html>\n");

    return (FALSE);
  }

//
// constructor
//
MitoLog::MitoLog (int size, const std::string &name)
  {
    //
    // initialize on the first use (singleton)
    //
    if (_count == 0)
      {
        _mutex->lock ();

        //
        // initialize the history data
        //
        _size = size;
        _index = 0;
        _data = new MitoLogRecord[size];
        _name = name;

        _mutex->unlock ();
      }

    //
    // bump the reference count
    //
    _count++;
  }

//
// destructor
//
MitoLog::~MitoLog ()
  {
    //
    // decrement the useage count
    //
    _count--;
    if (_count == 0)
      {
        //
        // free any history buffers
        //
        if (_data)
          {
            delete [] _data;
            _data = NULL;
          }

        //
        // delete the mutex
        //
        if (_mutex)
          {
            delete _mutex;
          }

        //
        // stop the web server
        //
        if (_server.running ())
          {
            _server.stop ();
          }
      }
  }

//
// get the name
//
const std::string MitoLog::name ()
  {
    return (_name);
  }

//
// get the URL
//
const std::string MitoLog::url ()
  {
    return (_url);
  }

//
// set the URL
//
const std::string MitoLog::url (const std::string &name)
  {
    _url = name;

    //
    // define an access URL to the web server
    //
    _server.remove (name);
    _server.add (name, history_xml_callback,
        "Embedded history source.");

    _server.remove ("/style.css");
    _server.add ("/style.css", style_css_callback,
        "CSS definitions for the pages.", false);

    _server.remove ("/association.xml");
    _server.add ("/association.xml", association_xml_callback,
        "Component interaction data.");

    _server.remove ("/association.html");
    _server.add ("/association.html", association_html_callback,
        "Chart of component interaction data.");

    _server.remove ("/association.js");
    _server.add ("/association.js", association_js_callback,
        "JavaScript code to process log data.", false);

    _server.remove ("/history.js");
    _server.add ("/history.js", history_js_callback,
        "JavaScript code to process log data.", false);

    _server.remove ("/history.html");
    _server.add ("/history.html", history_html_callback,
        "Message sequence chart of current log data.");

    _server.remove ("/excanvas.js");
    _server.add ("/excanvas.js", excanvas_js_callback,
        "HTML5 Canvas support for Internet Explorer.", false);

    _server.remove ("/favicon.ico");
    _server.add ("/favicon.ico", favicon_ico_callback,
        "Site icon.", false);

    return (_url);
  }

//
// start the underlying web server
//
int MitoLog::start ()
  {
    return (_server.start ());
  }

//
// stop the underlying web server
//
int MitoLog::stop ()
  {
    return (_server.stop ());
  }

//
// get the port of the underlying web server
//
int MitoLog::port ()
  {
	  return (_server.port ());
  }

//
// set the port of the underlying web server
//
int MitoLog::port (int number)
  {
    return (_server.port (number));
  }

//
// access the internal web server
//
WebServer *MitoLog::server (void)
  {
    return (&_server);
  }

//
// copy a record into the active log data
//
int MitoLog::add (const MitoLogRecord &record)
  {
    //
    // copy the data into the trace buffer
    //
    _mutex->lock ();
    _data[_index % _size].copy (record);
    _index++;
    _mutex->unlock ();

    return (_index);
  }

//
// add a record to the list
//
int MitoLog::add (const std::string &label, const std::string &actor,
    const char *format, va_list list)
  {
    char text[256];

    //
    // construct the argument list into a string
    //
#ifdef WIN32
    _vsnprintf (text, sizeof (text) - 1, format, list);
#else
    vsprintf (text, format, list);
#endif

    //
    // create a record to insert into the log data
    //
    MitoLogRecord record;

    record.timestamp = timestamp ();
    strncpy (record.label, label.c_str (),
        sizeof (record.label) - 1);
    strncpy (record.actor, actor.c_str (),
        sizeof (record.actor) - 1);
    strncpy (record.data.text, text,
        sizeof (record.data.text) - 1);

    //
    // copy the data into the trace buffer
    //
    return (add (record));
  }

//
// add a record to the list
//
int MitoLog::add (const std::string &label, const std::string &actor,
    const char *format, ...)
  {
    va_list marker;

    va_start (marker, format);
    add (label, actor, format, marker);
    va_end (marker);

    return (0);
  }

//
// add a generic entry to the log
//
int MitoLog::text (const std::string &actor, const char *format, ...)
  {
    //
    // construct the argument list into a string
    //
    va_list marker;

    va_start (marker, format);

    char text[256];
#ifdef WIN32
    _vsnprintf (text, sizeof (text) - 1, format, marker);
#else
    vsnprintf (text, sizeof (text) - 1, format, marker);
#endif

    va_end (marker);

    //
    // create a record to insert into the log data
    //
    MitoLogRecord record (MitoLogRecord::TEXT, actor);
    strncpy (record.data.text, text,
        sizeof (record.data.text) - 1);

    //
    // copy the data into the trace buffer
    //
    return (add (record));
  }

//
// add a code event entry to the log
//
int MitoLog::code (const std::string &actor, const std::string &name,
    int action)
  {
    return (add ("CODE", actor, "%d:%s", action, name.c_str ()));
  }

//
// add a state transition entry to the log
//
int MitoLog::transition (const std::string &actor, int code,
    const std::string &from, const std::string &to)
  {
    MitoLogRecord record (MitoLogRecord::STATE, actor);
    strncpy (record.data.state.from, from.c_str (),
        sizeof (record.data.state.from) - 1);
    strncpy (record.data.state.to, to.c_str (),
        sizeof (record.data.state.to) - 1);

    //
    // copy the data into the trace buffer
    //
    return (add (record));
  }

//
// register an event and its name into the log
//
int MitoLog::eventname (const std::string &name, int code)
  {
    EventList[name] = code;

    return (0);
  }

//
// add a message send/receive entry to the log
//
int MitoLog::event (const std::string &actor, int code,
    const std::string &from, const std::string &to)
  {
    //
    // update the association map
    //
    _mutex->lock ();
    _assoc->add (from, to);
    _mutex->unlock ();

    //
    // add the endpoints to the association data
    //
    if (actor != from)
      {
        //
        // create a record to insert into the log data
        //
        MitoLogRecord record (MitoLogRecord::EVENT_RECEIVE, actor);
        record.data.event.code = code;
        strncpy (record.data.event.from, from.c_str (),
            sizeof (record.data.event.from) - 1);
        strncpy (record.data.event.to, to.c_str (),
            sizeof (record.data.event.to) - 1);

        //
        // copy the data into the trace buffer
        //
        return (add (record));
      }
    else
      {
        //
        // create a record to insert into the log data
        //
        MitoLogRecord record (MitoLogRecord::EVENT_SEND, actor);
        record.data.event.code = code;
        strncpy (record.data.event.from, from.c_str (),
            sizeof (record.data.event.from) - 1);
        strncpy (record.data.event.to, to.c_str (),
            sizeof (record.data.event.to) - 1);

        //
        // copy the data into the trace buffer
        //
        return (add (record));
      }

    return (0);
  }

//
// add a timer set entry to the log
//
int MitoLog::timer (const std::string &actor, const std::string &name,
    int timeout)
  {
    //
    // create a record to insert into the log data
    //
    MitoLogRecord record (MitoLogRecord::TIMER_SET, actor);
    strncpy (record.data.timer.name, name.c_str (),
        sizeof (record.data.timer.name) - 1);
    record.data.timer.value = timeout;

    //
    // copy the data into the trace buffer
    //
    return (add (record));
  }

//
// add a timer cancellation entry to the log
//
int MitoLog::cancel (const std::string &actor, const std::string &name)
  {
    //
    // create a record to insert into the log data
    //
    MitoLogRecord record (MitoLogRecord::TIMER_CANCEL, actor);
    strncpy (record.data.timer.name, name.c_str (),
        sizeof (record.data.timer.name) - 1);

    //
    // copy the data into the trace buffer
    //
    return (add (record));
  }

//
// add a timer expiration entry to the log
//
int MitoLog::timeout (const std::string &actor, const std::string &name)
  {
    //
    // create a record to insert into the log data
    //
    MitoLogRecord record (MitoLogRecord::TIMER_EXPIRE, actor);
    strncpy (record.data.timer.name, name.c_str (),
        sizeof (record.data.timer.name) - 1);

    //
    // copy the data into the trace buffer
    //
    return (add (record));
  }

//
// add a mutex entry to the log
//
int MitoLog::mutex (const std::string &actor, int state,
    const std::string &name)
  {
    //
    // create a record to insert into the log data
    //
    MitoLogRecord record;

    record.timestamp = timestamp ();
    if (state == 0)
      {
        record.type = MitoLogRecord::MUTEX_TAKE;
      }
    else
      {
        record.type = MitoLogRecord::MUTEX_GIVE;
      }

    strncpy (record.actor, actor.c_str (),
        sizeof (record.actor) - 1);
    strncpy (record.data.mutex.name, name.c_str (),
        sizeof (record.data.mutex.name) - 1);

    //
    // copy the data into the trace buffer
    //
    return (add (record));
  }

//
// add an alarm entry to the log
//
int MitoLog::alarm (const std::string &actor, MitoLogRecord::ALARM_TYPE condition)
  {
    //
    // create a record to insert into the log data
    //
    MitoLogRecord record;

    record.timestamp = timestamp ();

    switch (condition)
      {
        case MitoLogRecord::CLEAR:
          record.type = MitoLogRecord::ALARM_CLEAR;
          break;

        case MitoLogRecord::WARNING:
          record.type = MitoLogRecord::ALARM_SET;
          break;

        case MitoLogRecord::CRITICAL:
          record.type = MitoLogRecord::ALARM_SET;
          break;

        case MitoLogRecord::FAULT:
          record.type = MitoLogRecord::ALARM_SET;
          break;

        default:
          break;
      }

    strncpy (record.actor, actor.c_str (),
        sizeof (record.actor) - 1);

    record.data.alarm.condition = condition;

    _alarms[actor] = condition;

    //
    // copy the data into the trace buffer
    //
    return (add (record));
  }

//
// read a log into memory from a file descriptor
//
int MitoLog::read (int fd)
  {
    _mutex->lock ();
    _mutex->unlock ();
    return (0);
  }

//
// write the current log to a web stream
//
int MitoLog::http ()
  {
    unsigned long int start;
    unsigned long int loop;
    MitoLogRecord *temp = NULL;

    //
    // mark the entry to this method
    //
    code (_name, "http", 0);

    //
    // produce the HTTP header
    //
    _server.produce (200, "text/xml", "");

    //
    // lock the history buffers
    //
    mutex (_name, 0, "_mutex");
    _mutex->lock ();

    //
    // pass all of the history buffers through the web server
    //
    start = 0;
    if (_index >= _size)
      {
        start = _index - _size;
      }

    char buffer[1024];

    _server.produce ("<?xml version='1.0' encoding='ISO-8859-1' standalone='yes'?>\n");
    _server.produce ("<!DOCTYPE history [\n");
    _server.produce ("<!ELEMENT history (catalog,events)>\n");
    _server.produce ("<!ELEMENT catalog (event)>\n");
    _server.produce ("<!ELEMENT history (events)>\n");
    //_server.produce ("<!ATTRLIST catalog\n");
    //_server.produce ("  name TYPE CDATA\n");
    //_server.produce ("  code TYPE CDATA>\n");
    _server.produce ("]>\n");
    _server.produce ("<history>\n");

    _server.produce ("<catalog>\n");
    std::map <const std::string, int>::const_iterator iter;
    for (iter = EventList.begin (); iter != EventList.end (); ++iter)
      {
        sprintf (buffer, "<event name='%s' code='%d'></event>\n",
            iter->first.c_str (), iter->second);
        _server.produce (buffer);
      }
    _server.produce ("</catalog>\n");

    _server.produce ("<events>\n");
    for (loop = start; loop < _index; loop++)
      {
        temp = &_data[loop % _size];

        switch (temp->type)
          {
            case MitoLogRecord::TEXT:
              sprintf (buffer, "<event timestamp='%0.6f' type='%s' context='%s' text='%s'/>\n",
                  temp->timestamp, "TEXT", temp->actor,
                  temp->data.text);
              break;
            case MitoLogRecord::CODE:
              sprintf (buffer, "<event timestamp='%0.6f' type='%s' context='%s'/>\n",
                  temp->timestamp, "CODE TBD", temp->actor);
              break;
            case MitoLogRecord::EVENT_SEND:
              sprintf (buffer, "<event timestamp='%0.6f' type='%s' context='%s' code='%d' from='%s' to='%s'/>\n",
                  temp->timestamp, "EVENT_SEND", temp->actor,
                  temp->data.event.code, temp->data.event.from, temp->data.event.to);
              break;
            case MitoLogRecord::EVENT_RECEIVE:
              sprintf (buffer, "<event timestamp='%0.6f' type='%s' context='%s' code='%d' from='%s' to='%s'/>\n",
                  temp->timestamp, "EVENT_RECEIVE", temp->actor,
                  temp->data.event.code, temp->data.event.from, temp->data.event.to);
              break;
            case MitoLogRecord::TIMER_SET:
              sprintf (buffer, "<event timestamp='%0.6f' type='%s' context='%s' name='%s' value='%d'/>\n",
                  temp->timestamp, "TIMER_SET", temp->actor,
                  temp->data.timer.name, temp->data.timer.value);
              break;
            case MitoLogRecord::TIMER_CANCEL:
              sprintf (buffer, "<event timestamp='%0.6f' type='%s' context='%s' name='%s'/>\n",
                  temp->timestamp, "TIMER_CANCEL", temp->actor,
                  temp->data.timer.name);
              break;
            case MitoLogRecord::TIMER_EXPIRE:
              sprintf (buffer, "<event timestamp='%0.6f' type='%s' context='%s' name='%s'/>\n",
                  temp->timestamp, "TIMER_EXPIRE", temp->actor,
                  temp->data.timer.name);
              break;
            case MitoLogRecord::MUTEX_TAKE:
              sprintf (buffer, "<event timestamp='%0.6f' type='%s' context='%s' name='%s'/>\n",
                  temp->timestamp, "MUTEX_TAKE", temp->actor,
                  temp->data.mutex.name);
              break;
            case MitoLogRecord::MUTEX_GIVE:
              sprintf (buffer, "<event timestamp='%0.6f' type='%s' context='%s' name='%s'/>\n",
                  temp->timestamp, "MUTEX_GIVE", temp->actor,
                  temp->data.mutex.name);
              break;
            case MitoLogRecord::ALARM_CLEAR:
              sprintf (buffer, "<event timestamp='%0.6f' type='%s' context='%s' condition='%d'/>\n",
                  temp->timestamp, "ALARM_CLEAR", temp->actor,
                  temp->data.alarm.condition);
              break;
            case MitoLogRecord::ALARM_SET:
              sprintf (buffer, "<event timestamp='%0.6f' type='%s' context='%s' condition='%d'/>\n",
                  temp->timestamp, "ALARM_SET", temp->actor,
                  temp->data.alarm.condition);
              break;
            case MitoLogRecord::STATE:
              sprintf (buffer, "<event timestamp='%0.6f' type='%s' context='%s' from='%s' to='%s'/>\n",
                  temp->timestamp, "TRANSITION", temp->actor,
                  temp->data.state.from, temp->data.state.to);
              break;
            default:
              sprintf (buffer, "<event timestamp='%0.06f' type='%s' payload='%s:%0.6f:%s:%s'/>\n",
                  temp->timestamp, "UNSUPPORTED", temp->label, temp->timestamp, temp->actor, temp->data.text);
              break;
          }

        _server.produce (buffer);
      }
    _server.produce ("</events>\n");

    _server.produce ("</history>\n");

    //
    // unlock the history buffers
    //
    _mutex->unlock ();
    mutex (_name, 1, "_mutex");

    //
    // mark the exit from this method
    //
    code (_name, "http", 1);

    //
    // return with success
    //
    return (0);
  }

//
// write the current association data to a web stream
//
int MitoLog::assoc ()
  {
    unsigned int loop;

    //
    // mark the entry to this method
    //
    code (_name, "assoc", 0);

    //
    // produce the HTTP header
    //
    _server.produce (200, "text/xml", "");

    //
    // lock the history buffers
    //
    mutex (_name, 1, "_mutex");
    _mutex->lock ();

    //
    // pass the association data through the web server as xml data
    //
    _server.produce ("<?xml version='1.0' encoding='ISO-8859-1' standalone='yes'?>");
    _server.produce ("<!DOCTYPE history [\n");
    _server.produce ("<!ELEMENT history (catalog,events,association)>\n");
    _server.produce ("<!ELEMENT catalog (event)>\n");
    _server.produce ("<!ELEMENT history (events)>\n");
    _server.produce ("<!ELEMENT association (link)>");
    _server.produce ("<!ELEMENT alarms (alarm)>");
    _server.produce ("]>");
    _server.produce ("<history>\n");
    _server.produce ("<association>");
    for (loop = 0; loop < _assoc->size (); loop++)
      {
        char buffer[256];
        sprintf (buffer, "<link sender='%s' receiver='%s' count='%d'/>",
            _assoc->sender (loop).c_str (), _assoc->receiver (loop).c_str (),
            _assoc->count (loop));
        _server.produce (buffer);
      }
    _server.produce ("</association>\n");

    _server.produce ("<alarms>\n");
    std::map <const std::string, MitoLogRecord::ALARM_TYPE>::const_iterator iter;
    for (iter = _alarms.begin (); iter != _alarms.end (); ++iter)
      {
        char buffer[256];
        sprintf (buffer, "<alarm actor='%s' condition='%d'></alarm>\n",
            iter->first.c_str (), iter->second);
        _server.produce (buffer);
      }
    _server.produce ("</alarms>\n");

    _server.produce ("</history>\n");

    //
    // unlock the history buffers
    //
    _mutex->unlock ();
    mutex (_name, 0, "_mutex");

    //
    // mark the exit from this method
    //
    code (_name, "assoc", 1);

    //
    // return with success
    //
    return (0);
  }

//
// print the current log to standard output
//
int MitoLog::print (FILE *file)
  {
    unsigned long int start;
    unsigned long int loop;
    MitoLogRecord *temp = NULL;

    _mutex->lock ();

    start = 0;
    if (_index >= _size)
      {
        start = _index - _size;
      }

    for (loop = start; loop < _index; loop++)
      {
        temp = &_data[loop % _size];

        fprintf (file, "%s:%0.6f:%s:%s\n",
          temp->label, temp->timestamp, temp->actor, temp->data.text);
      }

    _mutex->unlock ();

    return (0);
  }

//
//
//
#include <iostream>
void MitoLog::emit (const std::string &text)
  {
    const unsigned int width = 75;

    std::string temp = text;
    std::string line;

    while (temp.length () > width)
      {
        int pos = temp.rfind (" ", width, 1);
        temp.replace (pos, 1, "\n");
        line += temp.substr (0, pos);
        temp.erase (0, pos);
      }

    line += temp;

    std::cout << line;

    return;
  }
