#ifndef __USEREVENTS_H__
#define __USEREVENTS_H__

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

/**
 * Enumeration of user events available for use within this system. If an
 * event code is not defined here it cannot be used in the system.
 */
enum
  {
    eRequest = 1,   ///< processing request event
    eResponse   ///< processing response event
  };

#endif

