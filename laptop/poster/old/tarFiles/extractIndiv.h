// -------------------------------------------------------------------------
// extratIndiv.h
//
// written by Sharlee Climer, July 2010
//
// ------------------------------------------------------------------------

#ifndef _EXTRACTINDIV_H
#define _EXTRACTINDIV_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string.h>

#include "timer.h"


const int SYMBOL_COL = 1; // column containing symbol to be matched
const int PRINT_UNMATCHED = 0; // set to 1 to print items not matched to 'unmatched.txt'
const int MIN_CHARS_ID = 2; // mininum string length to check for match (default 2)
// if '.' or other 1-char symbol used for missing values, don't check for match

const float TOL = 0.000001; // tolerance

const int QUIET = 1;  // set to one to eliminate output to screen
const int VERBOSE = 0;  // set to one to display maximum output to screen
const int MAX_NUM_INDIVIDUALS = 100000; // maximum number of individuals
const int MAX_NUM_SNPs = 10000000; // maximum number of SNPs

inline void warning(const char* p) { fprintf(stderr,"Warning: %s \n",p); }
inline void fatal(const char* string) {fprintf(stderr,"\nFatal: %s\n\n",string); exit(1); }

#endif
