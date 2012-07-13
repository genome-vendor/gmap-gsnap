static char rcsid[] = "$Id: interval.c 40271 2011-05-28 02:29:18Z twu $";
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "interval.h"
#include <stdio.h>
#include <stdlib.h>		/* For qsort */
#include "mem.h"


#ifdef DEBUG
#define debug(x) x
#else
#define debug(x)
#endif


#define T Interval_T

T
Interval_new (unsigned int low, unsigned int high, int type) {
  T new = (T) MALLOC(sizeof(*new));

  if (low < high) {
    new->low = low;
    new->high = high;
    new->sign = +1;
  } else if (low > high) {
    new->low = high;
    new->high = low;
    new->sign = -1;
  } else {
    new->low = low;
    new->high = high;
    new->sign = 0;
  }
  new->type = type;
  return new;
}

T
Interval_copy (T old) {
  T new = (T) MALLOC(sizeof(*new));

  new->low = old->low;
  new->high = old->high;
  new->sign = old->sign;
  new->type = old->type;
  return new;
}
  
void
Interval_free (T *old) {
  if (*old) {
    FREE(*old);
  }
  return;
}

void
Interval_print (T this) {
  printf("%u %u %d",this->low,this->high,this->type);
  return;
}

unsigned int
Interval_low (T this) {
  return this->low;
}

unsigned int
Interval_high (T this) {
  return this->high;
}

void
Interval_store_length (T this, unsigned int length) {
  this->high = this->low - 1 + length;
  return;
}

int
Interval_sign (T this) {
  return this->sign;
}

unsigned int
Interval_length (T this) {
  return this->high - this->low + 1;
}

int
Interval_type (T this) {
  return this->type;
}

/* Have to subtract 1 because intervals array is zero-based */
unsigned int
Interval_array_low (struct T *intervals, int index) {
  return intervals[index-1].low;
}

/* Have to subtract 1 because intervals array is zero-based */
unsigned int
Interval_array_high (struct T *intervals, int index) {
  return intervals[index-1].high;
}


/* Have to subtract 1 because intervals array is zero-based */
bool
Interval_is_contained (unsigned int x, struct T *intervals, int index) {
  unsigned int low = intervals[index-1].low;
  unsigned int high = intervals[index-1].high;

  if (low <= x && x <= high) {
    return true;
  } else {
    return false;
  }
}

/* Have to subtract 1 because intervals array is zero-based */
bool
Interval_overlap_p (unsigned int x, unsigned int y, struct T *intervals, int index) {
  unsigned int low = intervals[index-1].low;
  unsigned int high = intervals[index-1].high;

  if (x <= high && y >= low) {
    return true;
  } else {
    return false;
  }
}




/************************************************************************/
/* These sorting procedures are accessed only by iit-write.c            */
/************************************************************************/

static struct T *current_intervals;

/* Have to subtract 1 because intervals array is zero-based */
static int 
sigma_compare (const void *i, const void *j) {
  int x = * (int *) i;
  int y = * (int *) j;

  unsigned int a = current_intervals[x-1].low;
  unsigned int b = current_intervals[y-1].low;

  if (a < b) {
    return -1;
  } else if (a > b) {
    return 1;
  } else {
    return 0;
  }
}

/* Have to subtract 1 because intervals array is zero-based */
static int 
omega_compare (const void *i, const void *j) {
  int x = * (int *) i;
  int y = * (int *) j;

  unsigned int a = current_intervals[x-1].high;
  unsigned int b = current_intervals[y-1].high;

  if (a < b) {
    return -1;
  } else if (a > b) {
    return 1;
  } else {
    return 0;
  }
}



/* These routines sort table[i..j] in place.  Assume that
   current_intervals has been set. */
void
Interval_qsort_by_sigma (int *table, int i, int j, struct T *intervals) {
  current_intervals = intervals;
  qsort(&(table[i]), j - i + 1, sizeof(int), sigma_compare);
  return;
}

void
Interval_qsort_by_omega (int *table, int i, int j, struct T *intervals) {
  current_intervals = intervals;
  qsort(&(table[i]), j - i + 1, sizeof(int), omega_compare);
  return;
}


int
Interval_cmp (const void *a, const void *b) {
  T x = * (T *) a;
  T y = * (T *) b;

  debug(printf("Comparing %u..%u with %u..%u => ",x->low,x->high,y->low,y->high));
  if (x->low < y->low) {
    debug(printf("-1\n"));
    return -1;
  } else if (x->low > y->low) {
    debug(printf("+1\n"));
    return +1;
  } else if (x->high < y->high) {
    debug(printf("-1\n"));
    return -1;
  } else if (x->high > y->high) {
    debug(printf("+1\n"));
    return +1;
  } else if (x->type < y->type) {
    debug(printf("-1\n"));
    return -1;
  } else if (x->type > y->type) {
    debug(printf("-1\n"));
    return +1;
  } else {
    debug(printf("0\n"));
    return 0;
  }
}


int
Interval_cmp_low (const void *a, const void *b) {
  T x = * (T *) a;
  T y = * (T *) b;

  debug(printf("Comparing %u..%u with %u..%u => ",x->low,x->high,y->low,y->high));
  if (x->low < y->low) {
    debug(printf("-1\n"));
    return -1;
  } else if (x->low > y->low) {
    debug(printf("+1\n"));
    return +1;
  } else if (x->type < y->type) {
    debug(printf("-1\n"));
    return -1;
  } else if (x->type > y->type) {
    debug(printf("-1\n"));
    return +1;
  } else {
    debug(printf("0\n"));
    return 0;
  }
}


int
Interval_cmp_high (const void *a, const void *b) {
  T x = * (T *) a;
  T y = * (T *) b;

  debug(printf("Comparing %u..%u with %u..%u => ",x->low,x->high,y->low,y->high));
  if (x->high < y->high) {
    debug(printf("-1\n"));
    return -1;
  } else if (x->high > y->high) {
    debug(printf("+1\n"));
    return +1;
#if 0
    /* Not needed by Splicetrie_retrieve_via_introns */
  } else if (x->type < y->type) {
    debug(printf("-1\n"));
    return -1;
  } else if (x->type > y->type) {
    debug(printf("-1\n"));
    return +1;
#endif
  } else {
    debug(printf("0\n"));
    return 0;
  }
}


int
Interval_windex_cmp (const void *a, const void *b) {
  struct Interval_windex_T x = * (struct Interval_windex_T *) a;
  struct Interval_windex_T y = * (struct Interval_windex_T *) b;

  return Interval_cmp((void *) &x.interval,(void *) &y.interval);
}

