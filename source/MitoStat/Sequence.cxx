#include <stdio.h>
#include <stdlib.h>

#include "mito/Sequence.h"

using namespace mito;

Sequence::Sequence (int length)
  {
    Statistics ();

    _data = NULL;
    _length = length;
    _index = 0;

    if (_length > 0)
      {
        _data = (double *) malloc (_length * sizeof (*_data));
      }

    return;
  }

Sequence::~Sequence ()
  {
    if (_data != NULL)
      {
        free (_data);
      }

    return;
  }

int Sequence::sample (double value)
  {
    Statistics::sample (value);

    if (_data != NULL)
      {
        if (_length > 0)
          {
            _data[_index++] = value;
            _index = _index % _length;
          }

      }

    return (n ());
  }

int Sequence::print (FILE *stream)
  {
    int loop;

    Statistics::print (stream);
    for (loop = 0; loop < _length; loop++)
      {
        fprintf (stream, "%d: %f\n", loop, _data[loop]);
      }

    return (0);
  }
