/* inverse.c  basic Gaussian elimination version */
/* http://userpages.umbc.edu/~squire/f14-455/download/inverse.c */

#include "stdafx.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LRL_inverse.h"


#define abs(a) ((a)<0?(-(a)):(a))

void inverse( int n, double A [ ], double AA [ ] ) {
   int *ROW, *COL;
   double *TEMP;
   int HOLD, I_pivot, J_pivot;
   double pivot, abs_pivot;
   int i, j, k;

   ROW = (int *)calloc( n, sizeof( int ) );
   COL = (int *)calloc( n, sizeof( int ) );
   TEMP = (double *)calloc( n, sizeof( double ) );
   memcpy( AA, A, n*n*sizeof( double ) );

   /*              set up row and column interchange vectors */
   for ( k=0; k<n; k++ ) {
      ROW[k] = k ;
      COL[k] = k ;
   }

   /*               begin main reduction loop */
   for ( k=0; k<n; k++ ) {
      /*             find largest element for pivot */
      pivot = AA[ROW[k]*n+COL[k]] ;
      I_pivot = k;
      J_pivot = k;
      for ( i=k; i<n; i++ ) {
         for ( j=k; j<n; j++ ) {
            abs_pivot = abs( pivot ) ;
            if ( abs( AA[ROW[i]*n+COL[j]] ) > abs_pivot ) {
               I_pivot = i ;
               J_pivot = j ;
               pivot = AA[ROW[i]*n+COL[j]] ;
            }
         }
      }
      if ( abs( pivot ) < 1.0E-65 ) {
         free( ROW );
         free( COL );
         free( TEMP );
         printf( "MATRIX is SINGULAR !!! \n" );
         return;
      }

      HOLD = ROW[k];
      ROW[k]= ROW[I_pivot];
      ROW[I_pivot] = HOLD ;
      HOLD = COL[k];
      COL[k]= COL[J_pivot];
      COL[J_pivot] = HOLD ;

      /*               reduce about pivot */
      AA[ROW[k]*n+COL[k]] = 1.0 / pivot ;
      for ( j=0; j<n; j++ ) {
         if ( j != k ) {
            AA[ROW[k]*n+COL[j]] = AA[ROW[k]*n+COL[j]] * AA[ROW[k]*n+COL[k]];
         }
      }

      /*               inner reduction loop */
      for ( i=0; i<n; i++ ) {
         if ( k != i ) {
            for ( j=0; j<n; j++ ) {
               if ( k != j ) {
                  AA[ROW[i]*n+COL[j]] = AA[ROW[i]*n+COL[j]] - AA[ROW[i]*n+COL[k]] *
                     AA[ROW[k]*n+COL[j]] ;
               }
            }
            AA[ROW[i]*n+COL[k]] = -AA[ROW[i]*n+COL[k]] * AA[ROW[k]*n+COL[k]] ;
         }
      }
   }
   /*      end of main reduction loop */

   /*      unscramble rows */
   for ( j=0; j<n; j++ ) {
      for ( i=0; i<n; i++ ) {
         TEMP[COL[i]] = AA[ROW[i]*n+j];
      }
      for ( i=0; i<n; i++ ) {
         AA[i*n+j] = TEMP[i] ;
      }
   }

   /*      unscramble columns */
   for ( i=0; i<n; i++ ) {
      for ( j=0; j<n; j++ ) {
         TEMP[ROW[j]] = AA[i*n+COL[j]] ;
      }
      for ( j=0; j<n; j++ ) {
         AA[i*n+j] = TEMP[j] ;
      }
   }
   free( ROW );
   free( COL );
   free( TEMP );
} /* end inverse */
