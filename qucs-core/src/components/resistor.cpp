/*
 * resistor.cpp - resistor class implementation
 *
 * Copyright (C) 2003, 2004, 2005 Stefan Jahn <stefan@lkcc.org>
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
 * the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
 * Boston, MA 02110-1301, USA.  
 *
 * $Id: resistor.cpp,v 1.24 2005-10-24 09:10:25 raimi Exp $
 *
 */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "complex.h"
#include "object.h"
#include "node.h"
#include "circuit.h"
#include "component_id.h"
#include "resistor.h"
#include "constants.h"

resistor::resistor () : circuit (2) {
  type = CIR_RESISTOR;
}

void resistor::calcSP (nr_double_t) {
  // calculate s-parameters
  nr_double_t z = getPropertyDouble ("R") / z0;
  setS (NODE_1, NODE_1, z / (z + 2));
  setS (NODE_2, NODE_2, z / (z + 2));
  setS (NODE_1, NODE_2, 2 / (z + 2));
  setS (NODE_2, NODE_1, 2 / (z + 2));
}

void resistor::calcNoiseSP (nr_double_t) {
  // calculate noise correlation matrix
  nr_double_t r = getPropertyDouble ("R");
  nr_double_t T = getPropertyDouble ("Temp");
  nr_double_t f = kelvin (T) * 4.0 * r * z0 / sqr (2.0 * z0 + r) / T0;
  setN (NODE_1, NODE_1, +f); setN (NODE_2, NODE_2, +f);
  setN (NODE_1, NODE_2, -f); setN (NODE_2, NODE_1, -f);
}

void resistor::calcNoiseAC (nr_double_t) {
  // calculate noise current correlation matrix
  nr_double_t r = getPropertyDouble ("R");
  nr_double_t T = getPropertyDouble ("Temp");
  nr_double_t f = kelvin (T) / T0 * 4.0 / r;
  setN (NODE_1, NODE_1, +f); setN (NODE_2, NODE_2, +f);
  setN (NODE_1, NODE_2, -f); setN (NODE_2, NODE_1, -f);
}

void resistor::initDC (void) {
  nr_double_t r = getPropertyDouble ("R");

  // for non-zero resistances usual MNA entries
  if (r != 0.0) {
    nr_double_t g = 1.0 / r;
    setVoltageSources (0);
    allocMatrixMNA ();
    setY (NODE_1, NODE_1, +g); setY (NODE_2, NODE_2, +g);
    setY (NODE_1, NODE_2, -g); setY (NODE_2, NODE_1, -g);
  }
  // for zero resistances create a zero voltage source
  else {
    setVoltageSources (1);
    setInternalVoltageSource (1);
    allocMatrixMNA ();
    clearY ();
    voltageSource (VSRC_1, NODE_1, NODE_2);
  }
}

/* The calcDC() function is here partly implemented again because the
   circuit can be used to simulate controlled non-zero resistances. */
void resistor::calcDC (void) {
  nr_double_t r = getPropertyDouble ("R");

  // for non-zero resistances usual MNA entries
  if (r != 0.0) {
    nr_double_t g = 1.0 / r;
    setY (NODE_1, NODE_1, +g); setY (NODE_2, NODE_2, +g);
    setY (NODE_1, NODE_2, -g); setY (NODE_2, NODE_1, -g);
  }
}

void resistor::initAC (void) {
  initDC ();
}

void resistor::calcAC (nr_double_t) {
  calcDC ();
}

void resistor::initTR (void) {
  initDC ();
}

void resistor::calcTR (nr_double_t) {
  calcDC ();
}
