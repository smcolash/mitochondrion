#include <stdio.h>
#include <sys/types.h>

#ifdef WIN32
#include <windows.h>
#include <process.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <winsock2.h>
#else
#ifdef VXWORKS
#include "vxWorks.h"
#include "ioLib.h"
#include "taskLib.h"
#include "sockLib.h"
#include "string.h"
#include "netinet/in.h"
#include "netinet/tcp.h"
#else
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#endif
#endif

#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

#include "mito/Mutex.h"
#include "mito/Url.h"
//#include "mito/UrlList.h"
#include "mito/WebServer.h"
#include "mito/MitoLog.h"

extern double timestamp ();

/**
  * A static flag that indicates whether the web server has been initialized
  * or not. Useful in environments where the network subsystem may need
  * additional initialization before first use in a program (Windows).
  */
static int _initialized = FALSE;

#ifndef WIN32
/**
  * Set up a dummy SIGPIPE handler
  */
void sigpipe_handler (int signum)
  {
    return;
  }
#endif

/**
  * Initialize the network subsystem for use by the server object.
  */
static int initialize ()
  {
    if (_initialized != TRUE)
      {
#ifdef WIN32
        WORD version;
        WSADATA result;
        int status;

        //
        // request initialization of a Winsock 2.0 library
        // 
        version = MAKEWORD (2, 0);
        status = WSAStartup (version, &result);
        if (status != 0)
          {
            //
            // failed to initialize an appropriate Winsock library
            //
            return (TRUE);
          }
 
        //
        // confirm the revision of the library
        // 
        if (LOBYTE (result.wVersion) != 2 || HIBYTE (result.wVersion ) != 0 )
          {
            //
            // failed to initialize the requested library
            //
            WSACleanup ();
            return (TRUE); 
          }
#else
        //
        // set up a dummy SIGPIPE handler
        //
        struct sigaction sa;
        sigfillset (&sa.sa_mask);
        sa.sa_handler = sigpipe_handler;
        sa.sa_flags = 0;
        sigaction (SIGPIPE, &sa, NULL);
#endif
        _initialized = TRUE;
      }
 
    return (FALSE);
  }

/**
  *
  */
bool compare_urls (const std::string &a, const std::string &b)
  {
    return (a.compare (a));
  }

/**
  * Index callback for the web server.
  */
int WebServer::index_callback (WebServer *server, const std::string &command,
    const std::string &url)
  {
    char buffer[1024];

    //
    // produce the HTTP header
    //
    server->produce (200, "text/html", "");

    //
    // produce the HTML document
    //
    server->produce ("<html>\n");
    server->produce ("<head>\n");
    // FIXME // server->produce ("<link rel='alternate' type='application/rss+xml' href='/feed.rss' title='Mitochondrion Service'>\n");
    server->produce ("<link rel='stylesheet' type='text/css' href='style.css' />\n");
    server->produce ("</head>\n");
    server->produce ("<body>\n");
    server->produce ("<div class='title'>Index</div>\n");
    server->produce ("<ul>\n");

    std::sort (_urls.begin (), _urls.end (), Url::Compare);

    unsigned int index;
    for (index = 0; index < _urls.size (); index++)
      {
        if (_urls[index]->visible ())
          {
            sprintf (buffer, "<li><a href=\"%s\">%s</a></li>\n",
                _urls[index]->_url.c_str (),
                _urls[index]->_url.c_str ());
            server->produce (buffer);
          }
      }

    server->produce ("</ul>\n");

    server->produce ("</body>\n");
    server->produce ("</html>\n");

    return (0);
  }

/**
  * Default callback for a URL that was requested but not registered to the
  * server instance.
  */
int WebServer::default_callback (WebServer *server, const std::string &command,
    const std::string &url)
  {
    char response[1024];
    char buffer[1024];

    _misses++;

    memset (&response, 0x00, sizeof (response));

    strlcat (response, "<html>\n", sizeof (response));
    strlcat (response, "<head>\n", sizeof (response));
    strlcat (response, "<title>404 Not Found</title>\n", sizeof (response));
    strlcat (response, "</head>\n", sizeof (response));
    strlcat (response, "<body>\n", sizeof (response));
    strlcat (response, "<h1>Not Found</h1>\n", sizeof (response));
    sprintf (buffer,
        "The requested URL %s was not found on this server.<P>\n",
        url.c_str ());
    strlcat (response, buffer, sizeof (response) - 0);
    strlcat (response, "</body>\n", sizeof (response));
    strlcat (response, "</html>\n", sizeof (response));

    return (server->produce (404, "text/html", "", strlen (response), response));
  }

/**
  * Helper function to run the web server event loop on a seperate thread. The
  * exact signature of the thread function and the return type are selected as
  * appropriate by the ifdef statements.
  */
#ifdef WIN32
static unsigned int WINAPI launch (LPVOID handle)
#else
#ifdef VXWORKS
static int launch (void *handle)
#else
static void *launch (void *handle)
#endif
#endif
  {
    WebServer *server = (WebServer *) handle;

    server->loop ();

#ifdef WIN32
    return (FALSE);
#else
#ifdef VXWORKS
    return (FALSE);
#else
    return (NULL);
#endif
#endif
  }

//
//
//
int WebServer::status_callback (WebServer *server, const std::string &command,
    const std::string &url)
  {
    char buffer[1024];

    double uptime = ::timestamp () - _start;

    //
    // produce the HTTP header
    //
    server->produce (200, "text/html", "");

    //
    // produce the HTML document
    //
    server->produce ("<html>\n");
    server->produce ("<head>\n");
    server->produce ("<link rel='stylesheet' type='text/css' href='style.css' />\n");
    server->produce ("</head>\n");
    server->produce ("<body>\n");
    server->produce ("<div class='title'>Status</div>\n");
    server->produce ("<ul>\n");
    sprintf (buffer, "<li>uptime: %0.1f seconds\n", uptime);
    server->produce (buffer);
    sprintf (buffer, "<li>%d URLS registered\n", (int) _urls.size ());
    server->produce (buffer);
    sprintf (buffer, "<li>%d pages served\n", _hits);
    server->produce (buffer);
    sprintf (buffer, "<li>%d invalid page request\n", _misses);
    server->produce (buffer);
    server->produce ("</ul>\n");
    server->produce ("</body>\n");
    server->produce ("</html>\n");

    return (0);
  }

//
//
//
int WebServer::rss_callback (WebServer *server, const std::string &command,
    const std::string &url)
  {
    //
    // get the local base URL
    //
    char buffer[1024];
    sprintf (buffer, "http://localhost:%d", server->port ());
    std::string base (buffer);
    base = "";

    //
    // produce the HTTP header
    //
    server->produce (200, "text/xml", "");
    //server->produce ("cache-control: no-cache");
    //server->produce ("cache-control: max-age=0");

    //
    // produce the XML document
    //
    server->produce ("<?xml version='1.0'?>\n");
    server->produce ("<rss version='2.0'>\n");
    server->produce ("  <channel>\n");
    server->produce ("    <title>MitoKernel Embedded Behavior</title>\n");
    server->produce ("    <link>" + base + "</link>\n");

    unsigned int index;
    for (index = 0; index < _urls.size (); index++)
      {
        if (_urls[index]->visible ())
          {
            server->produce ("    <item>\n");
            server->produce ("      <title>" + _urls[index]->_title + "</title>\n");
            server->produce ("      <link>" + base + _urls[index]->_url + "</link>\n");
            server->produce ("      <description>" + _urls[index]->_title + "</description>\n");
            server->produce ("    </item>\n");
          }
      }

    server->produce ("  </channel>\n");
    server->produce ("</rss>\n");

    return (0);
  }

//
//
//
WebServer::WebServer (int port, int depth)
  {
    _thread = 0;
    _port = port;
    _depth = depth;
    _fd = -1;
    _client = -1;
    _hits = 0;
    _misses = 0;

    _urls.clear ();
    _mutex = new Mutex;

    add ("/", this, (URLMETH) &WebServer::index_callback,
        "Embedded web service main index document.");
    add ("/status.cgi", this, (URLMETH) &WebServer::status_callback,
        "Embedded web service status data.");
// FIXME //    add ("/feed.rss", this, (URLMETH) &WebServer::rss_callback,
// FIXME //        "Embedded web service RSS feed.");

    return;
  }

//
//
//
WebServer::~WebServer ()
  {
    stop ();
    delete (_mutex);

    return;
  }

//
//
//
unsigned int WebServer::hits ()
  {
    return (_hits);
  }

//
//
//
unsigned int WebServer::misses ()
  {
    return (_misses);
  }

//
//
//
int WebServer::port ()
  {
    return (_port);
  }

//
//
//
int WebServer::port (int number)
  {
    if (running ())
      {
        return (-1);
      }

    _mutex->lock ();
    _port = number;
    _mutex->unlock ();

    return (port ());
  }

//
//
//
int WebServer::running ()
  {
    int temp;

    _mutex->lock ();
    temp = _running;
    _mutex->unlock ();

    return (temp);
  }

//
//
//
int WebServer::running (int value)
  {
    _mutex->lock ();
    _running = value;
    _mutex->unlock ();

    return (running ());
  }

//
//
//
void WebServer::loop ()
  {
    struct sockaddr_in server_addr;
    int nodelay;
    struct linger linger;
    int reuseaddr;

    struct sockaddr_in channel;
    socklen_t size;

    char request[1024];
    char method[1024];
    char document[1024];

    //
    // initialize the network subsystem
    //
    initialize ();

    //
    // do not start the loop if already running
    //
    if (running () == TRUE)
      {
        return;
      }

    //
    // change state to running
    //
    running (TRUE);

    //
    // create and configure a socket for the server
    //
    if ((_fd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
      {
        return;
      }

    //
    // set socket options for performance
    //
    nodelay = 1;
    if (setsockopt (_fd, IPPROTO_TCP, TCP_NODELAY,
        (char *) &nodelay , sizeof (nodelay)))
      {
        return;
      }

    linger.l_onoff = 0;
    linger.l_linger = 0;
    if (setsockopt (_fd, SOL_SOCKET, SO_LINGER,
        (char *) &linger, sizeof (linger)))
      {
        return;
      }

    reuseaddr = 1;
    if (setsockopt (_fd, SOL_SOCKET, SO_REUSEADDR,
        (char *) &reuseaddr, sizeof (reuseaddr)))
      {
        return;
      }

    //
    // configure the socket for use
    //
    memset (&server_addr, 0x00, sizeof (server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons (_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //
    // provide the socket to the system to use
    //
    if (bind (_fd, (struct sockaddr *) &server_addr, sizeof (struct sockaddr)) == -1)
      {
        return;
      }

    if (listen (_fd, _depth) == -1)
      {
        return;
      }

    //
    // loop until the server is shutdown
    //
    while (running ())
      {
        struct timeval timeout;
        fd_set mask;

        FD_ZERO (&mask);
        FD_SET (_fd, &mask);
        timeout.tv_sec = 0;
        timeout.tv_usec = (unsigned long int) 1e5;

        //
        // poll the server socket to avoid blocking
        //
        if (select (FD_SETSIZE, &mask, NULL, NULL, &timeout) < 1)
          {
            //
            // allow other tasks to execute
            //
#ifdef WIN32
            Sleep (1);
#else
#ifdef VXWORKS
            taskDelay (0);
#else
            sched_yield ();
#endif
#endif
            continue;
          }

        //
        // prevent cancellation of the thread during a request
        //
#ifdef VXWORKS
        taskSafe ();
#endif

        //
        // accept a connection on the server file descriptor, if available
        //
        size = sizeof (channel);
        if ((_client = accept (_fd, (struct sockaddr *) &channel, &size)) > -1)
          {
            //
            // clear out the incoming message buffer and receive the request
            //
            memset (&request, 0x00, sizeof (request));
            recv (_client, request, sizeof (request), 0);
            sscanf (request, "%s %s", method, document);

            //
            // FIXME...
            //
            char *arg = strchr (document, '?');
            if (arg != NULL)
              {
                *arg = '\0';
              }

            //
            // produce a response to the request
            //
            int index = locate (document);
            if (index < 0)
              {
                default_callback (this, method, document);
              }
            else
              {
                _urls[index]->respond (this, method, document);
              }

            //
            // count the responses
            //
            _hits++;

            //
            // clean up the connection
            //
            shutdown (_client, 0x02);
            close (_client);
            _client = -1;
          }

        //
        // allow cancellation of the thread outside of a request
        //
#ifdef VXWORKS
        taskUnsafe ();
#endif
      }

#ifdef WIN32
    return;
#else
#ifdef VXWORKS
    return;
#else
    pthread_exit (NULL);
#endif
#endif
  }

//
//
//
int WebServer::add (const std::string &url, URLFUNC callback,
    const std::string &title, bool visible)
  {
    FunctionUrl *handler = new FunctionUrl (url, callback, title, visible);
    _urls.push_back (handler);

    return (0);
  }

//
//
//
int WebServer::add (const std::string &url, Component *actor, URLMETH method,
    const std::string &title, bool visible)
  {
    MethodUrl *handler = new MethodUrl (url, actor, method, title, visible);
    _urls.push_back (handler);

    return (0);
  }

//
//
//
int WebServer::remove (const std::string &url)
  {
    int index = locate (url);
    if (index > -1)
      {
        delete _urls[index];
        _urls.erase (_urls.begin () + index);
      }

    return (0);
  }

//
//
//
int WebServer::start ()
  {
    _start = ::timestamp ();
#ifdef WIN32
    _beginthreadex (NULL, 0, launch, this, 0, &_thread);
#else
#ifdef VXWORKS
    _thread = taskSpawn ("web", 100, 0x00, (64 * 1024), launch, (int) this,
        0, 0, 0, 0, 0, 0, 0, 0, 0);
#else
    pthread_create (&_thread, NULL, launch, (void *) this);
#endif
#endif

    return (FALSE);
  }

//
//
//
int WebServer::stop()
  {
#ifdef WIN32
#else
#ifdef VXWORKS
#else
    void *result;
#endif
#endif

    //
    // change state to not running
    //
    running (FALSE);

    //
    // wait for the server thread to complete
    //
#ifdef WIN32
#else
#ifdef VXWORKS
    taskDelete (_thread);
#else
    pthread_join (_thread, &result);
#endif
#endif
    _thread = 0;

    //
    // close the server listen port
    //
    close (_fd);

    return (FALSE);
  }

/**
  *
  */
int WebServer::produce (const char *data)
  {
    unsigned int bytes = strlen (data);

    return (produce (data, bytes));
  }

/**
  *
  */
int WebServer::produce (const char *data, unsigned int bytes)
  {
    //
    // send the string data directly to the HTTP stream
    //
    if (data == NULL)
      {
        return (TRUE);
      }

    if (bytes > 0)
      {
        if (send (_client, data, bytes, 0) < 0)
          {
            return (TRUE);
          }
      }

    //
    // return with success
    //
    return (FALSE);
  }

/**
  *
  */
int WebServer::produce (const std::string &data)
  {
    return (produce (data.c_str ()));
  }
  
/**
  *
  */
int WebServer::produce (int status, const std::string &type,
    const std::string &header, int size)
  {
    //
    // buffer for creating components of the HTTP header
    //
    char buffer[1024];

    //
    // produce the HTTP header
    //
    sprintf (buffer, "HTTP/1.1 %d OK\r\n", status);
    if (send (_client, buffer, strlen (buffer), 0) < 0)
      {
        return (TRUE);
      }

    sprintf (buffer, "Connection: close\r\n");
    if (send (_client, buffer, strlen (buffer), 0) < 0)
      {
        return (TRUE);
      }

    sprintf (buffer, "Content-Type: %s\r\n", type.c_str ());
    if (send (_client, buffer, strlen (buffer), 0) < 0)
      {
        return (TRUE);
      }

    //
    // send the size of the message data being sent
    //
    if (size > 0)
      {
        sprintf (buffer, "Content-Length: %d\r\n", size);
        if (send (_client, buffer, strlen (buffer), 0) < 0)
          {
            return (TRUE);
          }
      }

    //
    // send the additional HTTP header, if supplied
    //
    if (header != "")
      {
        if (send (_client, header.c_str (), strlen (header.c_str ()), 0) < 0)
          {
            return (TRUE);
          }
      }

    //
    // terminate the header section
    //
    sprintf (buffer, "\r\n");
    if (send (_client, buffer, strlen (buffer), 0) < 0)
      {
        return (TRUE);
      }

    //
    // return with success
    //
    return (FALSE);
  }

/**
  *
  */
int WebServer::produce (int status, const std::string &type,
    const std::string &header, int size, const std::string &data)
  {
    //
    // produce the HTTP header
    //
    if (produce (status, type, header, size) == FALSE)
      {
        //
        // send the message data if available
        //
        return (produce ((char *) data.c_str ()));
      }

    //
    // return with error
    //
    return (TRUE);
  }

int WebServer::locate (const std::string &url)
  {
    unsigned int index;
    for (index = 0; index < _urls.size (); index++)
      {
        std::string test = _urls[index]->_url + "/";

        if ((_urls[index]->_url == url) ||
            (_urls[index]->_url == test))
          {
            return (index);
          }
      }

    //
    // all done
    //
    return (-1);
  }
