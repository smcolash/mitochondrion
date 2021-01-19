#include <stdio.h>
#include <math.h>
#include <iostream>

#include "mito/Statistics.h"

using namespace mito;

Statistics::Statistics ()
  {
    _n = 0;
    _sumx = 0.0;
    _sumxx = 0.0;
  }

Statistics::~Statistics ()
  {
    return;
  }

int Statistics::sample (double value)
  {
    _sumx = _sumx + value;
    _sumxx = _sumxx + (value * value);

    if (_n == 0)
      {
        _min.value (value);
        _max.value (value);
      }
    else
      {
        if (value < _min.value ())
          {
            _min.value (value);
          }

        if (value > _max.value ())
          {
            _max.value (value);
          }
      }

    _n++;

    return (_n);
  }

int Statistics::n (void)
  {
    return (_n);
  }

double Statistics::sumx (void)
  {
    return (_sumx);
  }

double Statistics::sumxx (void)
  {
    return (_sumxx);
  }

double Statistics::mean (void)
  {
    if (_n == 0)
      {
        return (0.0);
      }

    return (_sumx / _n);
  }

double Statistics::median (void)
  {
    return (0.0);
  }

double Statistics::variance (void)
  {
    if (_n > 0)
      {
        return ((_sumxx / _n) - (mean () * mean ()));
      }

    return (0.0);
  }

double Statistics::stdev (void)
  {
    return (sqrt (variance ()));
  }

double Statistics::min (void)
  {
    return (_min.value ());
  }

double Statistics::max (void)
  {
    return (_max.value ());
  }

int Statistics::print (FILE *stream)
  {
    fprintf (stream, "sumx: %f\n", sumx ());
    fprintf (stream, "sumxx: %f\n", sumxx ());
    fprintf (stream, "mean: %f\n", mean ());
    fprintf (stream, "variance: %f\n", variance ());
    fprintf (stream, "stdev: %f\n", stdev ());
    fprintf (stream, "min: %f\n", min ());
    fprintf (stream, "max: %f\n", max ());    

    return (0);
  }

std::ostream &operator<< (std::ostream &stream, const Statistics &data)
  {
    stream << "TBD...\n";

    return (stream);
  }

