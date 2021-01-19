#ifndef __FD_H__
#define __FD_H__

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

#include "mito/Constants.h"
#include "mito/Port.h"

//
// Resolve forward references.
//
class Actor;
class Event;

class __DLL__ FdPort : public Port
  {
public:
    FdPort (Actor *, const std::string &id);
    virtual ~FdPort ();

    virtual int set (int fd, Event *data = (Event *) NULL);
    virtual int cancel (int);
  };

#endif

