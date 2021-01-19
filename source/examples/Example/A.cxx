//
// include system headers
//
#include <sstream>

//
// include application headers
//
#include "A.h"
#include "mito/UserEvents.h"
#include "mito/Constants.h"

//
// produce a random delay interval for testing
//
static int randomize (void)
  {
    int range = 1500;
    int delta = (int) (range * ((float) rand () / RAND_MAX) - (range / 2));
    int delay = 1000 + delta;

    return (delay);
  }

int A::status_callback (WebServer *server, const std::string &command,
    const std::string &url)
  {
    std::stringstream buffer;

    //
    // produce the HTTP header
    //
    server->produce (200, "text/html", "");

    //
    // start the web page
    //
    server->produce ("<html>\n");

    server->produce ("<head>\n");
    server->produce ("<script language='javascript' type='text/javascript'>\n");
    server->produce ("function refresh ()\n");
    server->produce ("  {\n");
    server->produce ("    window.location.reload ();\n");
    server->produce ("    setTimeout ('refresh ()', 500);\n");
    server->produce ("  }\n");
    server->produce ("\n");
    server->produce ("onload = function ()\n");
    server->produce ("  {\n");
    server->produce ("    setTimeout ('refresh ()', 500);\n");
    server->produce ("  }\n");
    server->produce ("</script>\n");
    server->produce ("</head>\n");

    server->produce ("<body>\n");

    //
    // show the statistics of the actor
    //
    buffer << "name: " << name () << "<br>\n";
    buffer << "count: " << _statistics.n () << "<br>\n";
    buffer << "mean: " << _statistics.mean () << "<br>\n";
    buffer << "stdev: " << _statistics.stdev () << "<br>\n";

    //
    // show the internal status of the actor
    //
    buffer << "<pre>" << *this << "</pre>\n";

    server->produce (buffer.str ().c_str ());

    print ();

    //
    // finish the web page
    //
    server->produce ("</body>\n");
    server->produce ("</html>\n");

    //
    // return to the caller
    //
    return (0);
  }

int A::control_callback (WebServer *server, const std::string &command,
    const std::string &url)
  {
    std::stringstream buffer;

    if (true)
      {
        MitoLog log;
        log.event (log.name (), eRequest, log.name (), name ());

        Event event;
        tRequest (&event);
      }

    //
    // produce the HTTP header
    //
    server->produce (200, "text/html", "");

    //
    // produce an actor-specific status message
    //
    server->produce ("<html>\n");
    server->produce ("<body>\n");

    server->produce ("event injected\n");

    server->produce ("</body>\n");
    server->produce ("</html>\n");

    //
    // return to the caller
    //
    return (0);
  }

//
// default event handler
//
int A::unhandled (Event *event)
  {

    return (0);
  }

//
// initial transition code
//
int A::tInitial (Event *event)
  {
    //
    // register a status URL for the actor
    //
    MitoLog log;
    (log.server ())->add ("/actor/" + name () + "/status.cgi",
        (Actor *) this, (URLMETH) &A::status_callback,
        "Status for embedded component '" + name () +"'.");

    //
    // demonstrate control over the component using http
    //
    (log.server ())->add ("/actor/" + name () + "/control.cgi",
        (Actor *) this, (URLMETH) &A::control_callback,
        "Control of embedded component '" + name () +"'.");

    //
    // start a timer with a random value
    //
    int delay = randomize ();

    _statistics.sample (delay);

    timer->set (delay);

    //
    // done with the event
    //
    return (FALSE);
  }

//
// transition code for generic event receive
//
int A::tRequest (Event *event)
  {
    int delay = randomize ();

    timer->set (delay);
    _statistics.sample (delay);

    //
    // set an alarm if the delay exceeds 1000 millisecords
    //
    MitoLog log;
    if (_alarm != MitoLogRecord::WARNING)
      {
        if (delay > 1000)
          {
            log.alarm (name (), MitoLogRecord::WARNING);
            _alarm = MitoLogRecord::WARNING;
          }
      }
    else
      {
        log.alarm (name (), MitoLogRecord::CLEAR);
        _alarm = MitoLogRecord::CLEAR;
      }

    _count++;

    Event message (eRequest, Event::Priority::NORMAL);
    port_c->send (&message);

    //
    // done with the event
    //
    return (FALSE);
  }

int A::tTimeout (Event *event)
  {
    Event message (eRequest, Event::Priority::NORMAL);
    port_b->send (&message);

    //
    // done with the event
    //
    return (FALSE);
  }

//
// constructor creates and initializes object behavior
//
A::A ()
  {
    //
    // name the derived Actor type
    //
    type (__FUNCTION__);
    name (__FUNCTION__);

    //
    // create the interface ports
    //
    port_a = new Port (this, "a");
    port_b = new Port (this, "b");
    port_c = new Port (this, "c");

    //
    // create the timer port
    //
    timer = new TimerPort (this, "timer");

    //
    // create the states
    //
    State *sTop = new State ("top");
    State *sS1 = new State ("S1");
    State *sS2 = new State ("S2");

    //
    // connect the states
    //
    sTop->compose (sS1);
    sTop->compose (sS2);

    //
    // create transition and entry/exit behavior
    //
    sTop->initial (sS1, "tInitial",
        (METHOD) &A::tInitial);
    sS1->transition (Constants::Events::eTimer, "eTimer",
        timer, sS2, "tTimeout", (METHOD) &A::tTimeout);
    sS2->transition (eResponse, "eResponse",
        port_b, sS2, "tResponse", (METHOD) &A::unhandled);
    sS2->transition (eRequest, "eRequest",
        port_a, sS1, "tRequest", (METHOD) &A::tRequest);

    //
    // reset the loop count
    //
    _count = 0;

    //
    // reset the alarm code
    //
    _alarm = MitoLogRecord::CLEAR;

    //
    // activate the state machine for use
    //
    behavior (sTop);

    //
    // ready for use in the model
    //
    return;
  }

//
// destructor deletes any derived-class-specific data
//
A::~A ()
  {
    delete port_a;
    delete port_b;
    delete timer;
  }
