#include "hclib.h"
/**********************************************************************************************/
/*  This program is part of the Barcelona OpenMP Tasks Suite                                  */
/*  Copyright (C) 2009 Barcelona Supercomputing Center - Centro Nacional de Supercomputacion  */
/*  Copyright (C) 2009 Universitat Politecnica de Catalunya                                   */
/*                                                                                            */
/*  This program is free software; you can redistribute it and/or modify                      */
/*  it under the terms of the GNU General Public License as published by                      */
/*  the Free Software Foundation; either version 2 of the License, or                         */
/*  (at your option) any later version.                                                       */
/*                                                                                            */
/*  This program is distributed in the hope that it will be useful,                           */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of                            */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                             */
/*  GNU General Public License for more details.                                              */
/*                                                                                            */
/*  You should have received a copy of the GNU General Public License                         */
/*  along with this program; if not, write to the Free Software                               */
/*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA            */
/**********************************************************************************************/

/*
 * Original code from the Cilk project (by Keith Randall)
 * 
 * Copyright (c) 2000 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 */

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <alloca.h>
#include "bots.h"
#include "app-desc.h"
#include <omp.h>


/* Checking information */

static int solutions[] = {
        1,
        0,
        0,
        2,
        10, /* 5 */
        4,
        40,
        92,
        352,
        724, /* 10 */
        2680,
        14200,
        73712,
        365596,
};
#define MAX_SOLUTIONS sizeof(solutions)/sizeof(int)

#ifdef FORCE_TIED_TASKS
int mycount=0;
hclib_pragma_marker("omp", "threadprivate(mycount)");
#endif

int total_count;


/*
 * <a> contains array of <n> queen positions.  Returns 1
 * if none of the queens conflict, and returns 0 otherwise.
 */
int ok(int n, char *a)
{
     int i, j;
     char p, q;

     for (i = 0; i < n; i++) {
	  p = a[i];

	  for (j = i + 1; j < n; j++) {
	       q = a[j];
	       if (q == p || q == p - (j - i) || q == p + (j - i))
		    return 0;
	  }
     }
     return 1;
}

#ifndef FORCE_TIED_TASKS
void nqueens_ser (int n, int j, char *a, int *solutions)
#else
void nqueens_ser (int n, int j, char *a)
#endif
{
#ifndef FORCE_TIED_TASKS
	int res;
#endif
	int i;

	if (n == j) {
		/* good solution, count it */
#ifndef FORCE_TIED_TASKS
		*solutions = 1;
#else
		mycount++;
#endif
		return;
	}

#ifndef FORCE_TIED_TASKS
	*solutions = 0;
#endif

     	/* try each possible position for queen <j> */
	for (i = 0; i < n; i++) {
		{
	  		/* allocate a temporary array and copy <a> into it */
	  		a[j] = (char) i;
	  		if (ok(j + 1, a)) {
#ifndef FORCE_TIED_TASKS
	       			nqueens_ser(n, j + 1, a,&res);
				*solutions += res;
#else
	       			nqueens_ser(n, j + 1, a);
#endif
			}
		}
	}
}

#if defined(IF_CUTOFF)

#ifndef FORCE_TIED_TASKS
void nqueens(int n, int j, char *a, int *solutions, int depth)
#else
void nqueens(int n, int j, char *a, int depth)
#endif
{
#ifndef FORCE_TIED_TASKS
	int *csols;
#endif
	int i;

	if (n == j) {
		/* good solution, count it */
#ifndef FORCE_TIED_TASKS
		*solutions = 1;
#else
		mycount++;
#endif
		return;
	}


#ifndef FORCE_TIED_TASKS
	*solutions = 0;
	csols = (int *)alloca(n*sizeof(int));
	memset(csols,0,n*sizeof(int));
#endif

     	/* try each possible position for queen <j> */
	for (i = 0; i < n; i++) {
hclib_pragma_marker("omp", "task untied if(depth < bots_cutoff_value)");
		{
	  		/* allocate a temporary array and copy <a> into it */
	  		char * b = (char *)alloca(n * sizeof(char));
	  		memcpy(b, a, j * sizeof(char));
	  		b[j] = (char) i;
	  		if (ok(j + 1, b))
#ifndef FORCE_TIED_TASKS
	       			nqueens(n, j + 1, b,&csols[i],depth+1);
#else
	       			nqueens(n, j + 1, b,depth+1);
#endif
		}
	}

hclib_pragma_marker("omp", "taskwait");
#ifndef FORCE_TIED_TASKS
	for ( i = 0; i < n; i++) *solutions += csols[i];
#endif
}

#elif defined(FINAL_CUTOFF)

#ifndef FORCE_TIED_TASKS
void nqueens(int n, int j, char *a, int *solutions, int depth)
#else
void nqueens(int n, int j, char *a, int depth)
#endif
{
#ifndef FORCE_TIED_TASKS
	int *csols;
#endif
	int i;


	if (n == j) {
		/* good solution, count it */
#ifndef FORCE_TIED_TASKS
		*solutions += 1;
#else
		mycount++;
#endif
		return;
	}


#ifndef FORCE_TIED_TASKS
        char final = omp_in_final();
        if ( !final ) {
	  *solutions = 0;
	  csols = (int *)alloca(n*sizeof(int));
	  memset(csols,0,n*sizeof(int));
        }
#endif

     	/* try each possible position for queen <j> */
	for (i = 0; i < n; i++) {
hclib_pragma_marker("omp", "task untied final(depth+1 >= bots_cutoff_value) mergeable");
		{
                        char *b;
                        int *sol;
			if ( omp_in_final() && depth+1 > bots_cutoff_value ) {
		           b = a;
#ifndef FORCE_TIED_TASKS
                           sol = solutions;
#endif
                        } else {
	  		/* allocate a temporary array and copy <a> into it */
	  		   b = (char *)alloca(n * sizeof(char));
	  		   memcpy(b, a, j * sizeof(char));
#ifndef FORCE_TIED_TASKS
                           sol = &csols[i];
#endif
                        } 
	  		b[j] = i;
	  		if (ok(j + 1, b))
#ifndef FORCE_TIED_TASKS
	       			nqueens(n, j + 1, b,sol,depth+1);
#else
	       			nqueens(n, j + 1, b,depth+1);
#endif
		}
	}

hclib_pragma_marker("omp", "taskwait");
#ifndef FORCE_TIED_TASKS
       if ( !final ) {
	for ( i = 0; i < n; i++) *solutions += csols[i];
       }
#endif
}

#elif defined(MANUAL_CUTOFF)

#ifndef FORCE_TIED_TASKS
void nqueens(int n, int j, char *a, int *solutions, int depth)
#else
void nqueens(int n, int j, char *a, int depth)
#endif
{
#ifndef FORCE_TIED_TASKS
	int *csols;
#endif
	int i;


	if (n == j) {
		/* good solution, count it */
#ifndef FORCE_TIED_TASKS
		*solutions = 1;
#else
		mycount++;
#endif
		return;
	}


#ifndef FORCE_TIED_TASKS
	*solutions = 0;
	csols = (int *)alloca(n*sizeof(int));
	memset(csols,0,n*sizeof(int));
#endif

     	/* try each possible position for queen <j> */
	for (i = 0; i < n; i++) {
		if ( depth < bots_cutoff_value ) {
hclib_pragma_marker("omp", "task untied");
			{
	  			/* allocate a temporary array and copy <a> into it */
	  			char * b = (char *)alloca(n * sizeof(char));
	  			memcpy(b, a, j * sizeof(char));
	  			b[j] = (char) i;
	  			if (ok(j + 1, b))
#ifndef FORCE_TIED_TASKS
	       				nqueens(n, j + 1, b,&csols[i],depth+1);
#else
		       			nqueens(n, j + 1, b,depth+1);
#endif
			}
		} else {
  			a[j] = (char) i;
  			if (ok(j + 1, a))
#ifndef FORCE_TIED_TASKS
       				nqueens_ser(n, j + 1, a,&csols[i]);
#else
	       			nqueens_ser(n, j + 1, a);
#endif
		}
	}

hclib_pragma_marker("omp", "taskwait");
#ifndef FORCE_TIED_TASKS
	for ( i = 0; i < n; i++) *solutions += csols[i];
#endif
}


#else 

#ifndef FORCE_TIED_TASKS
typedef struct _pragma352 {
    int n;
    int j;
    char *a;
    int *solutions;
    int depth;
    int *csols;
    int i;
 } pragma352;

static void pragma352_hclib_async(void *____arg);
void nqueens(int n, int j, char *a, int *solutions, int depth)
#else
void nqueens(int n, int j, char *a, int depth)
#endif
{
#ifndef FORCE_TIED_TASKS
	int *csols;
#endif
	int i;


	if (n == j) {
		/* good solution, count it */
#ifndef FORCE_TIED_TASKS
		*solutions = 1;
#else
		mycount++;
#endif
		return;
	}


#ifndef FORCE_TIED_TASKS
	*solutions = 0;
	csols = (int *)alloca(n*sizeof(int));
	memset(csols,0,n*sizeof(int));
#endif

     	/* try each possible position for queen <j> */
	for (i = 0; i < n; i++) {
 { 
pragma352 *ctx = (pragma352 *)malloc(sizeof(pragma352));
ctx->n = n;
ctx->j = j;
ctx->a = a;
ctx->solutions = solutions;
ctx->depth = depth;
ctx->csols = csols;
ctx->i = i;
hclib_async(pragma352_hclib_async, ctx, NO_FUTURE, ANY_PLACE);
 } 
	}

 hclib_end_finish(); hclib_start_finish(); ;
#ifndef FORCE_TIED_TASKS
	for ( i = 0; i < n; i++) *solutions += csols[i];
#endif
} static void pragma352_hclib_async(void *____arg) {
    pragma352 *ctx = (pragma352 *)____arg;
    int n; n = ctx->n;
    int j; j = ctx->j;
    char *a; a = ctx->a;
    int *solutions; solutions = ctx->solutions;
    int depth; depth = ctx->depth;
    int *csols; csols = ctx->csols;
    int i; i = ctx->i;
    hclib_start_finish();
{
	  		/* allocate a temporary array and copy <a> into it */
	  		char * b = (char *)alloca(n * sizeof(char));
	  		memcpy(b, a, j * sizeof(char));
	  		b[j] = (char) i;
	  		if (ok(j + 1, b))
#ifndef FORCE_TIED_TASKS
       				nqueens(n, j + 1, b,&csols[i],depth); //FIXME: depth or depth+1 ???
#else
	       			nqueens(n, j + 1, b,depth); //FIXME: see above
#endif
		} ;     ; hclib_end_finish();
}



#endif

typedef struct _main_entrypoint_ctx {
    int size;
 } main_entrypoint_ctx;

static void main_entrypoint(void *____arg) {
    main_entrypoint_ctx *ctx = (main_entrypoint_ctx *)____arg;
    int size; size = ctx->size;
{
	total_count=0;

        bots_message("Computing N-Queens algorithm (n=%d) ", size);
hclib_start_finish(); {
			char *a;

			a = (char *)alloca(size * sizeof(char));
#ifndef FORCE_TIED_TASKS
			nqueens(size, 0, a, &total_count,0);
#else
			nqueens(size, 0, a, 0);
#endif
		} ; hclib_end_finish(); 
	bots_message(" completed!\n");
    } ; }

void find_queens (int size)
{
main_entrypoint_ctx *ctx = (main_entrypoint_ctx *)malloc(sizeof(main_entrypoint_ctx));
ctx->size = size;
hclib_launch(main_entrypoint, ctx);
free(ctx);

} 


int verify_queens (int size)
{
	if ( size > MAX_SOLUTIONS ) return BOTS_RESULT_NA;
	if ( total_count == solutions[size-1]) return BOTS_RESULT_SUCCESSFUL;
	return BOTS_RESULT_UNSUCCESSFUL;
}
