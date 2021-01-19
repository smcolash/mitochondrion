#ifndef __MITO_STATISTICS_H__
#define __MITO_STATISTICS_H__

#include <stdio.h>
#include <ostream>

/**
 * Include library export/import declarations.
 */
#include "mito/export.h"

/**
  * \namespace mito The 'mito' namespace holds all of the Mitochondrion
  * software classes and components in order to not pollute any more general
  * or higher-level namespaces.
  */
namespace mito
{
/**
  * A class to accumulate the statistics from a set of input samples. Specific
  * attributes and characteristics are accumulated from the samples but the
  * samples themselves are not held by the class.
  */
class __DLL__ Statistics
  {
private:
    class __DLL__ Value
      {
    private:
        double _value;
        int _initialized;

    public:
        Value ()
          {
            _initialized = 0;
            _value = 0.0;

            return;
          };

        ~Value ()
          {
            return;
          };

        double value (void)
          {
            return (_value);
          };

        double value (double data)
          {
            _value = data;
            _initialized = 1;

            return (value ());
          };

        int initialized (void)
          {
            return (_initialized);
          };
      };

private:
    /**
     * The number of data points in the sample set.
     */
    int _n;
    /**
     * The sum of the data points in the sample set.
     */
    double _sumx;
    /**
     * The sum of the squared data points in the sample set.
     */
    double _sumxx;
    /**
     * The minimum value in the sample set.
     */
    Value _min;
    /**
     * The maximum value in the sample set.
     */
    Value _max;

public:
    /**
     * Simple constructor.
     */
    Statistics ();
    /**
     * Simple destructor.
     */
    ~Statistics ();
    /**
     * A method that adds a data point to the sample set.
     */
    int sample (double value);
    /**
     * A method that returns the number of data points in the sample set.
     */
    int n (void);
    /**
     * A method that returns the sum of the data points in the sample set.
     */
    double sumx (void);
    /**
     * A method that returns the sum of the squared points in the sample
     * set.
     */
    double sumxx (void);
    /**
     * A method that returns the mean value of the sample set.
     */
    double mean (void);
    /**
     * A method that returns the median value of the sample set.
     */
    double median (void);
    /**
     * A method that returns the variance of the sample set.
     */
    double variance (void);
    /**
     * A method that returns the standard deviation of the sample set.
     */
    double stdev (void);
    /**
     * A method that returns the minimum value in the sample set.
     */
    double min (void);
    /**
     * A method that returns the maximum value in the sample set.
     */
    double max (void);
    /**
     * A method to print the statistics and some other derived quantities
     * of the sample set.
     */
    int print (FILE *stream);

public:
    /**
     * Output operator for inspection and debugging.
     */
    friend std::ostream &operator<< (std::ostream &stream,
        const Statistics &data);
  };
};
#endif
