/*
 * tvector.cpp - simple vector template class implementation
 *
 * Copyright (C) 2004 Stefan Jahn <stefan@lkcc.org>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this package; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.  
 *
 * $Id: tvector.cpp,v 1.3 2004/10/14 13:28:25 ela Exp $
 *
 */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#if HAVE_IEEEFP_H
# include <ieeefp.h>
#endif

#include "complex.h"
#include "tvector.h"

#ifdef __MINGW32__
# define finite(x) _finite(x)
#endif

// Constructor creates an unnamed instance of the tvector class.
template <class nr_type_t>
tvector<nr_type_t>::tvector () {
  size = 0;
  data = NULL;
}

/* Constructor creates an unnamed instance of the tvector class with a
   certain length. */
template <class nr_type_t>
tvector<nr_type_t>::tvector (int s)  {
  size = s;
  if (s > 0) {
    data = new nr_type_t[s];
    memset (data, 0, sizeof (nr_type_t) * s);
  }
  else data = NULL;
}

/* The copy constructor creates a new instance based on the given
   tvector object. */
template <class nr_type_t>
tvector<nr_type_t>::tvector (const tvector & v) {
  size = v.size;
  data = NULL;

  // copy tvector elements
  if (size > 0) {
    data = new nr_type_t[size];
    memcpy (data, v.data, sizeof (nr_type_t) * size);
  }
}

/* The assignment copy constructor creates a new instance based on the
   given tvector object. */
template <class nr_type_t>
const tvector<nr_type_t>&
tvector<nr_type_t>::operator=(const tvector<nr_type_t> & v) {
  if (&v != this) {
    size = v.size;
    if (data) { delete[] data; data = NULL; }
    if (size > 0) {
      data = new nr_type_t[size];
      memcpy (data, v.data, sizeof (nr_type_t) * size);
    }
  }
  return *this;
}

// Destructor deletes a tvector object.
template <class nr_type_t>
tvector<nr_type_t>::~tvector () {
  if (data) delete[] data;
}

// Returns the tvector element at the given position.
template <class nr_type_t>
nr_type_t tvector<nr_type_t>::get (int i) {
  return data[i - 1];
}

// Sets the tvector element at the given position.
template <class nr_type_t>
void tvector<nr_type_t>::set (int i, nr_type_t z) {
  data[i - 1] = z;
}

// The function swaps the given rows with each other.
template <class nr_type_t>
void tvector<nr_type_t>::exchangeRows (int r1, int r2) {
  assert (r1 >= 1 && r2 >= 1 && r1 <= size && r2 <= size);
  r1--; r2--;
  nr_type_t s = data[r1];
  data[r1] = data[r2];
  data[r2] = s;
}

// Addition.
template <class nr_type_t>
tvector<nr_type_t> operator + (tvector<nr_type_t> a, tvector<nr_type_t> b) {
  assert (a.getSize () == b.getSize ());
  int n = a.getSize ();
  tvector<nr_type_t> res (n);
  for (int i = 1; i <= n; i++) res.set (i, a.get (i) + b.get (i));
  return res;
}

// Subtraction.
template <class nr_type_t>
tvector<nr_type_t> operator - (tvector<nr_type_t> a, tvector<nr_type_t> b) {
  assert (a.getSize () == b.getSize ());
  int n = a.getSize ();
  tvector<nr_type_t> res (n);
  for (int i = 1; i <= n; i++) res.set (i, a.get (i) - b.get (i));
  return res;
}

// Scalar multiplication.
template <class nr_type_t>
tvector<nr_type_t> operator * (nr_double_t s, tvector<nr_type_t> a) {
  int n = a.getSize ();
  tvector<nr_type_t> res (n);
  for (int i = 1; i <= n; i++) res.set (i, s * a.get (i));
  return res;
}

template <class nr_type_t>
tvector<nr_type_t> operator * (tvector<nr_type_t> a, nr_double_t s) {
  return s * a;
}

// Mean square norm.
template <class nr_type_t>
nr_double_t norm (tvector<nr_type_t> a) {
  nr_double_t n = 0;
  for (int i = 1; i <= a.getSize (); i++) n += norm (a.get (i));
  return n;
}

// Maximum norm.
template <class nr_type_t>
nr_double_t maxnorm (tvector<nr_type_t> a) {
  nr_double_t nMax = 0, n;
  for (int i = 1; i <= a.getSize (); i++) {
    n = norm (a.get (i));
    if (n > nMax) nMax = n;
  }
  return nMax;
}

// Checks validity of vector.
template <class nr_type_t>
int tvector<nr_type_t>::isFinite (void) {
  for (int i = 0; i < size; i++)
    if (!finite (real (data[i]))) return 0;
  return 1;
}

#ifdef DEBUG
// Debug function: Prints the vector object.
template <class nr_type_t>
void tvector<nr_type_t>::print (void) {
  for (int r = 1; r <= size; r++) {
    fprintf (stderr, "%+.2e\n", (double) real (get (r)));
  }
}
#endif /* DEBUG */
