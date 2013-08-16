#include "trb.h"
#include <stdlib.h>
#include <stdio.h>

int
cmp (const void *a, const void *b, void * unused)
{
    return *(const int *) a - *(const int *) b;
}

int
main (int argc, char * argv[])
{
  int i, j, max, count, seed, lb;
  int *n, *p;
  int good = 0;
  struct trb_table *t;
  struct trb_traverser tr;

  if (argc < 3)
    {
      printf ("usage: ./lb-test <numers-count> <max-number> [ <random-seed> ]\n");
      return 1;
    }

  count = atoi (argv[1]);
  max = atoi (argv[2]);
  seed = argc > 3 ? atoi (argv[3]) : time (NULL);
  srand (seed);

  t = trb_create (&cmp, NULL, NULL);
  n = malloc (sizeof(int) * count);
  for (i = 0; i < count; ++i)
    {
      n[i] = rand () % (max + 1);
      trb_insert (t, n + i);
    }

  for (j = 0; j < max; ++j)
    {
      p = trb_t_lower_bound (&tr, t, &j);
      lb = -1;
      for (i = 0; i < count; ++i)
        {
          if (n[i] <= j && n[i] > lb)
            lb = n[i];
        }
      if (p && *p == lb || !p && lb == -1)
        ++good;
      else
        printf ("failed for %d: tree lb is %d, actual lb is %d\n", j, p ? *p : -1, lb);
    }
  printf ("%d of %d tests passed ok\n", good, max);
  free (n);
  trb_destroy (t, 0);
}
