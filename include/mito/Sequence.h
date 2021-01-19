#ifndef __MITOSEQUENCE_H__
#define __MITOSEQUENCE_H__

#include <stdio.h>

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

#include "Statistics.h"

/**
  * \namespace mito The 'mito' namespace holds all of the Mitochondrion
  * software classes and components in order to not pollute any more general
  * or higher-level namespaces.
  */
namespace mito
{
/**
  * A class to accumulate the statistics from a set of input
  * samples. The actual samples are not generally kept but the last 'N'
  * samples are available for use at any given time.
  */
class __DLL__ Sequence : public Statistics
  {
private:
    /**
      *
      */
    int _length;
    /**
      *
      */
    int _index;
    /**
      *
      */
    double *_data;

public:
    /**
      *
      */
    Sequence (int length = 0);
    /**
      *
      */
    ~Sequence ();
    /**
      *
      */
    int sample (double value);
    /**
      *
      */
    int print (FILE *stream);
  };
};
#endif
