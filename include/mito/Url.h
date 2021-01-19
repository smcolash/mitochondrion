#ifndef __URL_H__
#define __URL_H__

#include <string>

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

#include "mito/Component.h"
#include "mito/Constants.h"

/**
  * Declare the WebServer class as a forward reference to allow it to be used
  * in the definition of the URLFUNC callback signature.
  */
class WebServer;

/**
  * The signature definition of a callback function for a URL.
  */
typedef int (*URLFUNC) (WebServer *server, const std::string &command,
    const std::string &url);

/**
  * The signature definition of a callback function for a URL.
  */
typedef int (Component::*URLMETH) (WebServer *server, const std::string &command,
    const std::string &url);

/**
  * A single URL as managed by the web service.
  */
class __DLL__ Url
  {
public:
    Url ();
    virtual ~Url ();

    /**
      * Respond to the request for the URL represented by the instance.
      */
    virtual int respond (WebServer *server, char *method, char *document);

    static bool Compare (const Url *a, const Url *b);
    virtual bool visible ();
public:
    /**
     * The title of the URL.
     */
    std::string _title;
    /**
     * The URL string iteslf.
     */
    std::string _url;
    /**
     * The number of times the URL has been requested.
     */
    unsigned int _hits;
    /**
     * A flag indicating if this URL is externally visible by default.
     */
    bool _visible;
  };

class __DLL__ FunctionUrl : public Url
  {
public:
    FunctionUrl (const std::string &url, URLFUNC function,
        const std::string &title, bool visible = true);
    ~FunctionUrl ();

    int respond (WebServer *server, char *method, char *document);
public:
    /**
     * The callback action associated with URL.
     */
    URLFUNC _action;
  };

class __DLL__ MethodUrl : public Url
  {
public:
    MethodUrl (const std::string &url, Component *component, URLMETH method,
        const std::string &title, bool visible = true);
    ~MethodUrl ();

    int respond (WebServer *server, char *method, char *document);
public:
    /**
     * The actor to provide the callback behavior.
     */
    Component *_actor;
    /**
     * The method to facilitate the callback.
     */
    URLMETH _method;
  };

#endif

