#include <string>
#include <iostream>

#include "mito/Url.h"

/**
  *
  */
Url::Url ()
  {
    _title = "";
    _url = "";
    _hits = 0;

    return;
  };

/**
  *
  */
Url::~Url ()
  {
    return;
  };

bool Url::visible ()
  {
    return (_visible);
  }

int Url::respond (WebServer *server, char *method, char *document)
  {
    _hits++;
    return (0);
  }

FunctionUrl::FunctionUrl (const std::string &url, URLFUNC function,
    const std::string &title, bool visible)
  {
    _url = url;
    _title = title;
    _action = function;
    _visible = visible;

    return;
  }

FunctionUrl::~FunctionUrl ()
  {
    return;
  }

int FunctionUrl::respond (WebServer *server, char *method, char *document)
  {
    if (_action)
      {
        (_action) (server, method, document);
        Url::respond (server, method, document);
        return (0);
      }

    return (1);
  }

MethodUrl::MethodUrl (const std::string &url, Component *component, URLMETH method,
    const std::string &title, bool visible)
  {
    _url = url;
    _title = title;
    _actor = component;
    _method = method;
    _visible = visible;

    return;
  }

MethodUrl::~MethodUrl ()
  {
    return;
  }

int MethodUrl::respond (WebServer *server, char *method, char *document)
  {
    if ((_actor != NULL) && (_method != NULL))
      {
        ((_actor)->*(_method)) (server, method, document);
        Url::respond (server, method, document);
        return (0);
      }

    return (1);
  }

bool Url::Compare (const Url *a, const Url *b)
  {
    return (a->_url < b->_url);
  }

