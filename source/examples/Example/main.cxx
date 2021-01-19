/**
 * A slightly non-trivial example of three objects passing a message between
 * each other in a round-robin manner. On completion of the cycle a timer is
 * set, delaying the initiation of the next cycle. Live access is granted to
 * the internal log data through an embedded web service. This same service
 * can be used to query general status and ultimately shut down the
 * application.
 */

/**
 * Include the definition of the top-level Actors.
 */
#include "A.h"
#include "B.h"
#include "C.h"
#include "D.h"

/**
  * An object that holds a set of high-level structure and behavior.
  */
class Application
  {
private:
    A a;
    B b;
    C c;
    D d;
public:
    Application (std::string name)
      {
        //
        // Create the top-level application objects.
        //
        a.name (name + "_a");
        b.name (name + "_b");
        c.name (name + "_c");
        d.name (name + "_d");

        //
        // Bind the actor ports to provide full-duplex messaging.
        //
        Port::bind (a.port_b, b.port_a);
        Port::bind (b.port_b, c.port_a);
        Port::bind (c.port_b, a.port_a);
        Port::bind (a.port_c, d.port_a);
      };

    ~Application ()
      {
      };
  };

/**
  * An object that holds a set of high-level structure and behavior.
  */
class Cluster
  {
private:
    D central;

    A a_1;
    B b_1;
    C c_1;

    A a_2;
    B b_2;
    C c_2;

    A a_3;
    B b_3;
    C c_3;

    A a_4;
    B b_4;
    C c_4;
public:
    Cluster (std::string name)
      {
        //
        // Create the top-level application objects.
        //
        central.name (name + "_central");

        a_1.name (name + "_1_a");
        b_1.name (name + "_1_b");
        c_1.name (name + "_1_c");

        a_2.name (name + "_2_a");
        b_2.name (name + "_2_b");
        c_2.name (name + "_2_c");

        a_3.name (name + "_3_a");
        b_3.name (name + "_3_b");
        c_3.name (name + "_3_c");

        a_4.name (name + "_4_a");
        b_4.name (name + "_4_b");
        c_4.name (name + "_4_c");

        //
        // Bind the actor ports to provide full-duplex messaging.
        //
        Port::bind (a_1.port_b, b_1.port_a);
        Port::bind (b_1.port_b, c_1.port_a);
        Port::bind (c_1.port_b, a_1.port_a);
        Port::bind (a_1.port_c, central.port_a);

        Port::bind (a_2.port_b, b_2.port_a);
        Port::bind (b_2.port_b, c_2.port_a);
        Port::bind (c_2.port_b, a_2.port_a);
        Port::bind (a_2.port_c, central.port_b);

        Port::bind (a_3.port_b, b_3.port_a);
        Port::bind (b_3.port_b, c_3.port_a);
        Port::bind (c_3.port_b, a_3.port_a);
        Port::bind (a_3.port_c, central.port_c);

        Port::bind (a_4.port_b, b_4.port_a);
        Port::bind (b_4.port_b, c_4.port_a);
        Port::bind (c_4.port_b, a_4.port_a);
        Port::bind (a_4.port_c, central.port_d);
      };

    ~Cluster ()
      {
      };
  };

/**
  * Shutdown web service callback.
  */
int shutdown_callback (WebServer *server, const std::string &command,
    const std::string &url)
  {
    //
    // register the shutdown event
    //
    MitoLog log;
    log.eventname ("eTerminate", Constants::Events::eTerminate);

    //
    // produce the HTTP header
    //
    server->produce (200, "text/html", "");

    //
    // product the shutdown message
    //
    server->produce ("<html>\n");
    server->produce ("<body>\n");
    server->produce ("shutting down\n");
    server->produce ("</body>\n");
    server->produce ("</html>\n");

    //
    // shut down the main event queue
    //
    Queue::stop ();

    //
    // return to the caller
    //
    return (0);
  }

/**
 * Main application entry point.
 */
#ifdef WIN32
#include <tchar.h>
int _tmain(int argc, _TCHAR* argv[])
#else
int main (int argc, char *argv[])
#endif
  {
    int port = 80;

    //
    // get the log port, if specified
    //
    if (argc > 1)
      {
        port = atoi ((char *) argv[1]);
      }

    //
    // Create an interface to the trace buffer with 'N' elements.
    //
    MitoLog log (1000, "Log");

    //
    // Anchor access to the log at a specific URL and start the web server
    //
    log.url ("/history.xml");
    log.port (port);
    log.start ();

    //
    // Add access to the shutdown mechanism through a URL in the log service
    //
    (log.server ())->add ("/shutdown.cgi", shutdown_callback,
        "Shutdown control for the embedded behavior.");

    //
    // create multiple, independent sets of top-level behavior
    //
    Application a1 ("1");
    Application a2 ("2");
    //Application a3 ("3");
    //Application a4 ("4");

    //Cluster cluster ("X");

    //
    // mark the start of execution
    //
    log.text ("main", "start of execution");

    //
    // Run the event queue until termination is requested.
    //
    Queue::run ();

    //
    // mark the end of execution
    //
    log.text ("main", "end of execution");

    //
    // Clean up the infrastructure.
    //
    Queue::cleanup ();

    //
    // Return the the caller.
    //
    return (FALSE);
  }
