#include <stdio.h>
#include <stdlib.h>

#include "mito/Statistics.h"
#include "mito/Sequence.h"

int main (int argc, char *argv[])
  {
    mito::Statistics a;
    mito::Sequence s (10);

    int loop;

    for (loop = 0; loop < 1e7; loop++)
      {
        double value = -0.5 + (double) rand () / RAND_MAX;
        a.sample (value);
        s.sample (value);
      }

    a.print (stdout);
    fprintf (stdout, "----------\n");
    s.print (stdout);

    return (0);
  }
