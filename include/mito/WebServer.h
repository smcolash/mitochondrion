#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#ifdef WIN32
#include <windows.h>
#else
#ifdef VXWORKS
#else
#include <pthread.h>
#endif
#endif

#include <string>
#include <vector>

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

#include "mito/Component.h"
#include "mito/Url.h"

/**
  * Define TRUE only if no other previous definition exists.
  */
#ifndef TRUE
#define TRUE (1)
#endif

/**
  * Define FALSE only if no other previous definition exists.
  */
#ifndef FALSE
#define FALSE (0)
#endif

/**
  * Declare the Mutex class as a forward reference to avoid all other code
  * from needing to include its definition.
  */
class Mutex;

/**
  * A class to provide an embeddable web server running on a specified port.
  * Response to a request on a given URL is determined by the addition of
  * callback functions that are registered against the document name on the
  * server. These callback functions accept the HTTP command (GET, POST), the
  * file descriptor of the connection and the URL as requested by the
  * browser.
  */
class __DLL__ WebServer : public Component
  {
private:
    /**
      * The thread the server is executing on.
      */
#ifdef WIN32
    unsigned int _thread;
#else
#ifdef VXWORKS
    int _thread;
#else
    pthread_t _thread;
#endif
#endif
    /**
      * The TCP port the server thread is running on.
      */
    int _port;
    /**
      * The number of buffered connection requests the server will accept.
      */
    int _depth;
    /**
      * The socket descriptor that the server thread will listen for
      * connection requests on.
      */
    int _fd;
    /**
      * The socket descriptor of ther current (if any) client to the server.
      */
    int _client;
    /**
      * The state of the server (running or stopped).
      */
    int _running;
    /**
      * A mutual exclusion lock to coordinate control of the thread by the
      * class interface.
      */
    Mutex *_mutex;
    /**
      * The list of URL to callback associations that drive the servers
      * responses to client requests.
      */
    std::vector <Url *> _urls;
    /**
      *
      */
    unsigned int _hits;
    /**
      *
      */
    unsigned int _misses;
    /**
      * The time at which the web server was started.
      */
    double _start;

public:
    /**
      * A simple constructor that sets the TCP port the server will run on and
      * the number of buffered connection requests it will accept.
      */
    WebServer (int port = 80, int depth = 10);
    /**
      * A simple destructor that stops the server thread (if necessary) and
      * cleans up the server environment.
      */
    ~WebServer ();

public:
    /**
      * Returns the current state of the server thread.
      */
    int running ();
    /**
      * Sets and returns the current state of the server thread.
      */
    int running (int value);
    /**
      *
      */
    int port ();
    /**
      *
      */
    int port (int number);
    /**
      * Registers a local URL with a simple callback function.
      */
    int add (const std::string &url, URLFUNC callback,
        const std::string &title, bool visible = true);
    /**
      * Registers a local URL with an actor's callback function.
      */
    int add (const std::string &url, Component *actor, URLMETH callback,
        const std::string &title, bool visible = true);
    /**
      * Deregisters a local URL from the server.
      */
    int remove (const std::string &url);
    /**
      * Starts the server thread within the object.
      */
    int start ();
    /**
      * Stops the server thread within the object.
      */
    int stop();
    /**
      * The web server event loop.
      */
    void loop ();
    /**
      * Produce a string directly into an open file descriptor.
      */
    int produce (const char *data);
    /**
      * Produce a string directly into an open file descriptor.
      */
    int produce (const char *data, unsigned int bytes);
    /**
      * Produce a string directly into an open file descriptor.
      */
    int produce (const std::string &data);
    /**
      * Produce an HTTP header into an open file descriptor.
      */
    int produce (int status, const std::string &type,
        const std::string &header, int size = 0);
    /**
      * Produce an HTTP header and document into an open file descriptor.
      */
    int produce (int status, const std::string &type,
        const std::string &header, int size, const std::string &buffer);
    /**
      *
      */
    int index_callback (WebServer *server, const std::string &command, 
        const std::string &url);
    /**
      *
      */
    int rss_callback (WebServer *server, const std::string &command, 
        const std::string &url);
    /**
      *
      */
    int status_callback (WebServer *server, const std::string &command, 
        const std::string &url);
    /**
      *
      */
    int default_callback (WebServer *server, const std::string &command, 
        const std::string &url);
    /**
      * Find the URL handler data that corresponds with a given URL string.
      */
    int locate (const std::string &url);
    /**
      *
      */
    unsigned int hits ();
    /**
      *
      */
    unsigned int misses ();
  };

#endif

