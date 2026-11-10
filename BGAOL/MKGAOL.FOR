C     MKGAOL -- Make GAOL (make lots of cells)
C     a subroutine of
C     Bravais General Analysis of Lattices (BGAOL)
C     The Program formerly known as ITERATE
C
C     Lawrence C Andrews[1] and Herbert J. Bernstein[2,*]
C
C     [1] Micro Encoder Inc., 11533 NE 118th St, #200,
C         Kirkland, WA 98034-7111 USA
C     [2] Dowling College, 1300 William Floyd Parkway,
C         Shirley, NY 11967 USA
C     [*] To whom correspondence should be addressed.
C         Email: yaya@dowling.edu
C
C     Copyright 1996, 2012, all rights reserved
C
C*******************************************************
C    You may redistribute this program under the terms
C    of the GPL.
C
C    ALternatively you may redistribute this functions
C    and subroutines of this program as an API under the
C    terms of the LGPL
C*******************************************************
C*************************** GPL NOTICES ******************************
C*                                                                    *
C* This program is free software; you can redistribute it and/or      *
C* modify it under the terms of the GNU General Public License as     *
C* published by the Free Software Foundation; either version 2 of     *
C* (the License, or (at your option) any later version.               *
C*                                                                    *
C* This program is distributed in the hope that it will be useful,    *
C* but WITHOUT ANY WARRANTY; without even the implied warranty of     *
C* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      *
C* GNU General Public License for more details.                       *
C*                                                                    *
C* You should have received a copy of the GNU General Public License  *
C* along with this program; if not, write to the Free Software        *
C* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA           *
C* 02111-1307  USA                                                    *
C*                                                                    *
C**********************************************************************/

C************************* LGPL NOTICES *******************************
C*                                                                    *
C* This library is free software; you can redistribute it and/or      *
C* modify it under the terms of the GNU Lesser General Public         *
C* License as published by the Free Software Foundation; either       *
C* version 2.1 of the License, or (at your option) any later version. *
C*                                                                    *
C* This library is distributed in the hope that it will be useful,    *
C* but WITHOUT ANY WARRANTY; without even the implied warranty of     *
C* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  *
C* Lesser General Public License for more details.                    *
C*                                                                    *
C* You should have received a copy of the GNU Lesser General Public   *
C* License along with this library; if not, write to the Free         *
C* Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,    *
C* MA  02110-1301  USA                                                *
C*                                                                    *
      BLOCK DATA PRJS
      real*8 prj(36,25), prjperp(36,25)
      real*8 prjhat(36,15)
      integer ms(36,18)
      common /projectors/ prj,prjperp,prjhat,ms

C The projectors for the 15 base types (5-D boundaries
C in G6), plus a few extra for internal boundaries

C     case1
      data (prj(i,1),i=1,36) /
     * .5D0,.5D0,0.D0,0.D0,0.D0,0.D0,
     * .5D0,.5D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,1.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,1.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,1.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,1.D0 /

       data (prjhat(i,1),i=1,36) /
     * .5D0,.5D0,0.D0,0.D0,0.D0,0.D0,
     * .5D0,.5D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,1.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,.5D0,.5D0,0.D0,
     * 0.D0,0.D0,0.D0,.5D0,.5D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,1.D0 /

      data (prjperp(i,1),i=1,36) /
     *  .5D0,-.5D0,0.D0,0.D0,0.D0,0.D0,
     * -.5D0, .5D0,0.D0,0.D0,0.D0,0.D0,
     *  0.D0, 0.D0,0.D0,0.D0,0.D0,0.D0,
     *  0.D0, 0.D0,0.D0,0.D0,0.D0,0.D0,
     *  0.D0, 0.D0,0.D0,0.D0,0.D0,0.D0,
     *  0.D0, 0.D0,0.D0,0.D0,0.D0,0.D0 /


C     case2
      data (prj(i,2),i=1,36) /
     * 1.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,.5D0,.5D0,0.D0,0.D0,0.D0,
     * 0.D0,.5D0,.5D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,1.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,1.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,1.D0 /

      data (prjhat(i,2),i=1,36) /
     * 1.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,.5D0,.5D0,0.D0,0.D0,0.D0,
     * 0.D0,.5D0,.5D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,1.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,.5D0,.5D0,
     * 0.D0,0.D0,0.D0,0.D0,.5D0,.5D0 /

      data (prjperp(i,2),i=1,36) /
     * 0.D0, 0.D0, 0.D0,0.D0,0.D0,0.D0,
     * 0.D0, .5D0,-.5D0,0.D0,0.D0,0.D0,
     * 0.D0,-.5D0, .5D0,0.D0,0.D0,0.D0,
     * 0.D0, 0.D0, 0.D0,0.D0,0.D0,0.D0,
     * 0.D0, 0.D0, 0.D0,0.D0,0.D0,0.D0,
     * 0.D0, 0.D0, 0.D0,0.D0,0.D0,0.D0 /

C     case3
      data (prj(i,3),i=1,36) /
     * 1.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,1.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,1.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,1.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,1.D0 /

      data (prjhat(i,3),i=1,36) /
     * 1.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,1.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,1.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0 /

      data (prjperp(i,3),i=1,36) /
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,1.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0 /


C     case4
      data (prj(i,4),i=1,36) /
     * 1.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,1.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,1.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,1.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,1.D0 /

      data (prjhat(i,4),i=1,36) /
     * 1.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,1.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,1.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0 /

      data (prjperp(i,4),i=1,36) /
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,1.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0 /

C     case5
      data (prj(i,5),i=1,36) /
     * 1.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,1.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,1.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,1.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,1.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0 /

      data (prjhat(i,5),i=1,36) /
     * 1.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,1.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,1.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0 /

      data (prjperp(i,5),i=1,36) /
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,0.D0,
     * 0.D0,0.D0,0.D0,0.D0,0.D0,1.D0 /

C     case6
      data (prj(i,6),i=1,36) /
     * 1.D0,  0.D0, 0.D0,  0.D0, 0.D0, 0.D0,
     * 0.D0,  .5D0, 0.D0,  .5D0, 0.D0, 0.D0,
     * 0.D0,  0.D0, 1.D0,  0.D0, 0.D0, 0.D0,
     * 0.D0,  .5D0, 0.D0,  .5D0, 0.D0, 0.D0,
     * 0.D0,  0.D0, 0.D0,  0.D0, 1.D0, 0.D0,
     * 0.D0,  0.D0, 0.D0,  0.D0, 0.D0, 1.D0 /

      data (prjhat(i,6),i=1,36) /
     * 1.D0,  0.D0, 0.D0,  0.D0, 0.D0, 0.D0,
     * 0.D0,  .5D0, 0.D0,  .5D0, 0.D0, 0.D0,
     * 0.D0,  0.D0, 1.D0,  0.D0, 0.D0, 0.D0,
     * 0.D0,  .5D0, 0.D0,  .5D0, 0.D0, 0.D0,
     * 0.D0,  0.D0, 0.D0,  0.D0, .2D0, .4D0,
     * 0.D0,  0.D0, 0.D0,  0.D0, .4D0, .8D0 /

      data (prjperp(i,6),i=1,36) /
     * 0.D0,  0.D0, 0.D0,  0.D0, 0.D0, 0.D0,
     * 0.D0,  .5D0, 0.D0, -.5D0, 0.D0, 0.D0,
     * 0.D0,  0.D0, 0.D0,  0.D0, 0.D0, 0.D0,
     * 0.D0, -.5D0, 0.D0,  .5D0, 0.D0, 0.D0,
     * 0.D0,  0.D0, 0.D0,  0.D0, 0.D0, 0.D0,
     * 0.D0,  0.D0, 0.D0,  0.D0, 0.D0, 0.D0 /

C     case7
      data (prj(i,7),i=1,36) /
     * 1.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     * 0.D0, .5D0, 0.D0, .5D0, 0.D0, 0.D0,
     * 0.D0, 0.D0, 1.D0, 0.D0, 0.D0, 0.D0,
     * 0.D0, .5D0, 0.D0, .5D0, 0.D0, 0.D0,
     * 0.D0, 0.D0, 0.D0, 0.D0, 1.D0, 0.D0,
     * 0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 1.D0 /

      data (prjhat(i,7),i=1,36) /
     * 1.D0,  0.D0, 0.D0,  0.D0, 0.D0, 0.D0,
     * 0.D0,  .5D0, 0.D0,  .5D0, 0.D0, 0.D0,
     * 0.D0,  0.D0, 1.D0,  0.D0, 0.D0, 0.D0,
     * 0.D0,  .5D0, 0.D0,  .5D0, 0.D0, 0.D0,
     * 0.D0,  0.D0, 0.D0,  0.D0, .2D0, .4D0,
     * 0.D0,  0.D0, 0.D0,  0.D0, .4D0, .8D0 /

      data (prjperp(i,7),i=1,36) /
     * 0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     * 0.D0, .5D0, 0.D0,-.5D0, 0.D0, 0.D0,
     * 0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     * 0.D0,-.5D0, 0.D0, .5D0, 0.D0, 0.D0,
     * 0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     * 0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0 /


C     case8
      data (prj(i,8),i=1,36) /
     * 1.D0,  0.D0, 0.D0,  0.D0, 0.D0, 0.D0,
     * 0.D0,  .5D0, 0.D0, -.5D0, 0.D0, 0.D0,
     * 0.D0,  0.D0, 1.D0,  0.D0, 0.D0, 0.D0,
     * 0.D0, -.5D0, 0.D0,  .5D0, 0.D0, 0.D0,
     * 0.D0,  0.D0, 0.D0,  0.D0, 1.D0, 0.D0,
     * 0.D0,  0.D0, 0.D0,  0.D0, 0.D0, 1.D0 /

      data (prjperp(i,8),i=1,36) /
     * 0.D0,  0.D0, 0.D0,  0.D0, 0.D0, 0.D0,
     * 0.D0,  .5D0, 0.D0,  .5D0, 0.D0, 0.D0,
     * 0.D0,  0.D0, 0.D0,  0.D0, 0.D0, 0.D0,
     * 0.D0,  .5D0, 0.D0,  .5D0, 0.D0, 0.D0,
     * 0.D0,  0.D0, 0.D0,  0.D0, 0.D0, 0.D0,
     * 0.D0,  0.D0, 0.D0,  0.D0, 0.D0, 0.D0 /

C     case9
      data (prj(i,9),i=1,36) /
     *  .5D0, 0.D0, 0.D0, 0.D0, .5D0, 0.D0,
     *  0.D0, 1.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 1.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 1.D0, 0.D0, 0.D0,
     *  .5D0, 0.D0, 0.D0, 0.D0, .5D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 1.D0 /

      data (prjhat(i,9),i=1,36) /
     *  .5D0, 0.D0, 0.D0, 0.D0, .5D0, 0.D0,
     *  0.D0, 1.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 1.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, .2D0, 0.D0, .4D0,
     *  .5D0, 0.D0, 0.D0, 0.D0, .5D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, .4D0, 0.D0, .8D0 /

      data (prjperp(i,9),i=1,36) /
     *  .5D0, 0.D0, 0.D0, 0.D0,-.5D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     * -.5D0, 0.D0, 0.D0, 0.D0, .5D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0 /


C     caseA
      data (prj(i,10),i=1,36) /
     *  .5D0, 0.D0, 0.D0, 0.D0, .5D0, 0.D0,
     *  0.D0, 1.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 1.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 1.D0, 0.D0, 0.D0,
     *  .5D0, 0.D0, 0.D0, 0.D0, .5D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 1.D0 /


      data (prjhat(i,10),i=1,36) /
     *  .5D0, 0.D0, 0.D0, 0.D0, .5D0, 0.D0,
     *  0.D0, 1.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 1.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, .2D0, 0.D0, .4D0,
     *  .5D0, 0.D0, 0.D0, 0.D0, .5D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, .4D0, 0.D0, .8D0 /

      data (prjperp(i,10),i=1,36) /
     *  .5D0, 0.D0, 0.D0, 0.D0,-.5D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     * -.5D0, 0.D0, 0.D0, 0.D0, .5D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0 /


C     caseB
      data (prj(i,11),i=1,36) /
     *   .5D0, 0.D0, 0.D0, 0.D0, -.5D0, 0.D0,
     *   0.D0, 1.D0, 0.D0, 0.D0,  0.D0, 0.D0,
     *   0.D0, 0.D0, 1.D0, 0.D0,  0.D0, 0.D0,
     *   0.D0, 0.D0, 0.D0, 1.D0,  0.D0, 0.D0,
     *  -.5D0, 0.D0, 0.D0, 0.D0,  .5D0, 0.D0,
     *   0.D0, 0.D0, 0.D0, 0.D0,  0.D0, 1.D0 /

      data (prjperp(i,11),i=1,36) /
     *   .5D0, 0.D0, 0.D0, 0.D0,  .5D0, 0.D0,
     *   0.D0, 0.D0, 0.D0, 0.D0,  0.D0, 0.D0,
     *   0.D0, 0.D0, 0.D0, 0.D0,  0.D0, 0.D0,
     *   0.D0, 0.D0, 0.D0, 0.D0,  0.D0, 0.D0,
     *   .5D0, 0.D0, 0.D0, 0.D0,  .5D0, 0.D0,
     *   0.D0, 0.D0, 0.D0, 0.D0,  0.D0, 0.D0 /


C     caseC
      data (prj(i,12),i=1,36) /
     *  .5D0, 0.D0, 0.D0, 0.D0, 0.D0, .5D0,
     *  0.D0, 1.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 1.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 1.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 1.D0, 0.D0,
     *  .5D0, 0.D0, 0.D0, 0.D0, 0.D0, .5D0 /

      data (prjhat(i,12),i=1,36) /
     *  .5D0, 0.D0, 0.D0, 0.D0, 0.D0, .5D0,
     *  0.D0, 1.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 1.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, .2D0, .4D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, .4D0, .8D0, 0.D0,
     *  .5D0, 0.D0, 0.D0, 0.D0, 0.D0, .5D0 /

      data (prjperp(i,12),i=1,36) /
     *  .5D0, 0.D0, 0.D0, 0.D0, 0.D0,-.5D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     * -.5D0, 0.D0, 0.D0, 0.D0, 0.D0, .5D0 /


C     caseD
      data (prj(i,13),i=1,36) /
     *  .5D0, 0.D0, 0.D0, 0.D0, 0.D0, .5D0,
     *  0.D0, 1.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 1.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 1.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 1.D0, 0.D0,
     *  .5D0, 0.D0, 0.D0, 0.D0, 0.D0, .5D0 /

      data (prjhat(i,13),i=1,36) /
     *  .5D0, 0.D0, 0.D0, 0.D0, 0.D0, .5D0,
     *  0.D0, 1.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 1.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, .2D0, .4D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, .4D0, .8D0, 0.D0,
     *  .5D0, 0.D0, 0.D0, 0.D0, 0.D0, .5D0 /

      data (prjperp(i,13),i=1,36) /
     *  .5D0, 0.D0, 0.D0, 0.D0, 0.D0,-.5D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     * -.5D0, 0.D0, 0.D0, 0.D0, 0.D0, .5D0 /


C     caseE
      data (prj(i,14),i=1,36) /
     *   .5D0, 0.D0, 0.D0, 0.D0, 0.D0, -.5D0,
     *   0.D0, 1.D0, 0.D0, 0.D0, 0.D0,  0.D0,
     *   0.D0, 0.D0, 1.D0, 0.D0, 0.D0,  0.D0,
     *   0.D0, 0.D0, 0.D0, 1.D0, 0.D0,  0.D0,
     *   0.D0, 0.D0, 0.D0, 0.D0, 1.D0,  0.D0,
     *  -.5D0, 0.D0, 0.D0, 0.D0, 0.D0,  .5D0 /

      data (prjperp(i,14),i=1,36) /
     *   .5D0, 0.D0, 0.D0, 0.D0, 0.D0,  .5D0,
     *   0.D0, 0.D0, 0.D0, 0.D0, 0.D0,  0.D0,
     *   0.D0, 0.D0, 0.D0, 0.D0, 0.D0,  0.D0,
     *   0.D0, 0.D0, 0.D0, 0.D0, 0.D0,  0.D0,
     *   0.D0, 0.D0, 0.D0, 0.D0, 0.D0,  0.D0,
     *   .5D0, 0.D0, 0.D0, 0.D0, 0.D0,  .5D0 /

C     caseF
      data (prj(i,15),i=1,36) /
     *  .8D0,-.2D0, 0.D0,-.2D0,-.2D0,-.2D0,
     * -.2D0, .8D0, 0.D0,-.2D0,-.2D0,-.2D0,
     *  0.D0, 0.D0, 1.D0, 0.D0, 0.D0, 0.D0,
     * -.2D0,-.2D0, 0.D0, .8D0,-.2D0,-.2D0,
     * -.2D0,-.2D0, 0.D0,-.2D0, .8D0,-.2D0,
     * -.2D0,-.2D0, 0.D0,-.2D0,-.2D0, .8D0 /

      data (prjhat(i,15),i=1,36) /
     * .55D0, .05D0, 0.D0, .05D0,-.45D0,-.2D0,
     * .05D0, .55D0, 0.D0,-.45D0, .05D0,-.2D0,
     *  0.D0,  0.D0, 1.D0,  0.D0,  0.D0, 0.D0,
     * .05D0,-.45D0, 0.D0, .55D0, .05D0,-.2D0,
     *-.45D0, .05D0, 0.D0, .05D0, .55D0,-.2D0,
     * -.2D0, -.2D0, 0.D0, -.2D0, -.2D0, .8D0 /

      data (prjperp(i,15),i=1,36) /
     *  .2D0, .2D0, 0.D0, .2D0, .2D0, .2D0,
     *  .2D0, .2D0, 0.D0, .2D0, .2D0, .2D0,
     *  0.D0, 0.D0, 0.D0, 0.D0, 0.D0, 0.D0,
     *  .2D0, .2D0, 0.D0, .2D0, .2D0, .2D0,
     *  .2D0, .2D0, 0.D0, .2D0, .2D0, .2D0,
     *  .2D0, .2D0, 0.D0, .2D0, .2D0, .2D0 /


C     case678X g2=g4=g5=g6=0
      data (prj(i,16),i=1,36) /
     * 1.D0, 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 1.D0, 0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0, 0.D0,  0.D0,  0.D0 /

      data (prjperp(i,16),i=1,36) /
     * 0.D0, 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,
     * 0.D0, 1.D0, 0.D0, 0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0, 1.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0, 0.D0,  1.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0, 0.D0,  0.D0,  1.D0 /


C     case9ABCDEX g1=g4=g5=g6=0
      data (prj(i,17),i=1,36) /
     * 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0, 1.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 1.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0 /

      data (prjperp(i,17),i=1,36) /
     * 1.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0,  1.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0,  0.D0,  1.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  1.D0 /

C     caseFX g1=g2=g4=g5=g6=0
      data (prj(i,18),i=1,36) /
     * 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 1.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0 /

      data (prjperp(i,18),i=1,36) /
     * 1.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0, 1.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0,  1.D0,  0.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0,  0.D0,  1.D0,  0.D0,
     * 0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  1.D0 /


C     DATA boundary67 /
      data (prj(i,19),i=1,36) /
     * 1.D0,  0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0,  .5D0, 0.D0,  .5D0,  0.D0,  0.D0,
     * 0.D0,  0.D0, 1.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0,  .5D0, 0.D0,  .5D0,  0.D0,  0.D0,
     * 0.D0,  0.D0, 0.D0,  0.D0,  .5D0,  .5D0,
     * 0.D0,  0.D0, 0.D0,  0.D0,  .5D0,  .5D0 /

      data (prjperp(i,19),i=1,36) /
     * 0.D0,  0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0,  .5D0, 0.D0, -.5D0,  0.D0,  0.D0,
     * 0.D0,  0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     * 0.D0, -.5D0, 0.D0,  .5D0,  0.D0,  0.D0,
     * 0.D0,  0.D0, 0.D0,  0.D0,  .5D0, -.5D0,
     * 0.D0,  0.D0, 0.D0,  0.D0, -.5D0,  .5D0 /

C     DATA boundary9A /
      data (prj(i,20),i=1,36) /
     *  .5D0, 0.D0, 0.D0,  0.D0,  .5D0,  0.D0,
     *  0.D0, 1.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     *  0.D0, 0.D0, 1.D0,  0.D0,  0.D0,  0.D0,
     *  0.D0, 0.D0, 0.D0,  .5D0,  0.D0,  .5D0,
     *  .5D0, 0.D0, 0.D0,  0.D0,  .5D0,  0.D0,
     *  0.D0, 0.D0, 0.D0,  .5D0,  0.D0,  .5D0 /

      data (prjperp(i,20),i=1,36) /
     *  .5D0, 0.D0, 0.D0,  0.D0, -.5D0,  0.D0,
     *  0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     *  0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     *  0.D0, 0.D0, 0.D0,  .5D0,  0.D0, -.5D0,
     * -.5D0, 0.D0, 0.D0,  0.D0, -.5D0,  0.D0,
     *  0.D0, 0.D0, 0.D0, -.5D0,  0.D0,  .5D0 /


C     DATA boundaryCD /
      data (prj(i,21),i=1,36) /
     *  .5D0, 0.D0, 0.D0,  0.D0,  0.D0,  .5D0,
     *  0.D0, 1.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     *  0.D0, 0.D0, 1.D0,  0.D0,  0.D0,  0.D0,
     *  0.D0, 0.D0, 0.D0,  .5D0,  .5D0,  0.D0,
     *  0.D0, 0.D0, 0.D0,  .5D0,  .5D0,  0.D0,
     *  .5D0, 0.D0, 0.D0,  0.D0,  0.D0, .5D0 /

      data (prjperp(i,21),i=1,36) /
     *  .5D0, 0.D0, 0.D0,  0.D0,  0.D0, -.5D0,
     *  0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     *  0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     *  0.D0, 0.D0, 0.D0,  .5D0, -.5D0,  0.D0,
     *  0.D0, 0.D0, 0.D0, -.5D0,  .5D0,  0.D0,
     * -.5D0, 0.D0, 0.D0,  0.D0,  0.D0,  .5D0 /


C      case boundary12 g1=g2=g3
       data (prj(i,22),i=1,36) /
     *  0.3333333333D0, 0.3333333333D0,0.3333333333D0,
     *  0.D0,  0.D0,  0.D0,
     *  0.3333333333D0, 0.3333333333D0,0.3333333333D0,
     *  0.D0,  0.D0,  0.D0,
     *  0.3333333333D0, 0.3333333333D0,0.3333333333D0,
     *  0.D0,  0.D0,  0.D0,
     *  0.D0, 0.D0, 0.D0,  1.D0,  0.D0,  0.D0,
     *  0.D0, 0.D0, 0.D0,  0.D0,  1.D0,  0.D0,
     *  0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  1.D0 /


       data (prjperp(i,22),i=1,36) /
     *  0.6666666667D0,-0.3333333333D0,-0.3333333333D0,
     *  0.D0,  0.D0,  0.D0,
     * -0.3333333333D0, 0.6666666667D0,-0.3333333333D0,
     *  0.D0,  0.D0,  0.D0,
     * -0.3333333333D0,-0.3333333333D0, 0.6666666667D0,
     *  0.D0,  0.D0,  0.D0,
     *  0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     *  0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0,
     *  0.D0, 0.D0, 0.D0,  0.D0,  0.D0,  0.D0 /

C      case boundary 8F g4=-g2, g1+g2+g4+g5+g6 = 0
       data (prj(i,23),i=1,36) /
     *  0.6666666667D0,           0.D0,           0.D0,
     *             0.D0,-0.3333333333D0,-0.3333333333D0,
     *            0.D0,           .5D0,           0.D0,
     *            -.5D0,           0.D0,           0.D0,
     *            0.D0,           0.D0,           1.D0,
     *             0.D0,           0.D0,           0.D0,
     *            0.D0,          -.5D0,          0.D0,
     *             .5D0,           0.D0,           0.D0,
     * -0.3333333333D0,            0.D0,          0.D0,
     *             0.D0,  0.6666666667D0,-0.3333333333D0,
     * -0.3333333333D0,            0.D0,          0.D0,
     *             0.D0, -0.3333333333D0, 0.6666666667D0/

       data (prjperp(i,23),i=1,36) /
     *  0.3333333333D0,           0.D0,           0.D0,
     *             0.D0, 0.3333333333D0, 0.3333333333D0,
     *            0.D0,            .5D0,          0.D0,
     *             .5D0,            0.D0,          0.D0,
     *            0.D0,            0.D0,          0.D0,
     *             0.D0,            0.D0,          0.D0,
     *            0.D0,            .5D0,          0.D0,
     *             .5D0,            0.D0,          0.D0,
     *  0.3333333333D0,            0.D0,          0.D0,
     *             0.D0,  0.3333333333D0, 0.3333333333D0,
     *  0.3333333333D0,            0.D0,          0.D0,
     *             0.D0,  0.3333333333D0, 0.3333333333D0/

C      case boundary BF g5=-g1, g1+g2+g4+g5+g6 = 0
       data (prj(i,24),i=1,36) /
     *            .5D0,           0.D0,            0.D0,
     *             0.D0,          -.5D0,            0.D0,
     *             0.D0, 0.6666666667D0,           0.D0,
     *  -0.3333333333D0,           0.D0, -0.3333333333D0,
     *            0.D0,           0.D0,            1.D0,
     *             0.D0,           0.D0,            0.D0,
     *            0.D0,-0.3333333333D0,            0.D0,
     *   0.6666666667D0,           0.D0, -0.3333333333D0,
     *           -.5D0,           0.D0,            0.D0,
     *             0.D0,           .5D0,            0.D0,
     *            0.D0, -0.3333333333D0,           0.D0,
     * -0.3333333333D0,             0.D0, 0.6666666667D0/

       data (prjperp(i,24),i=1,36) /
     *            .5D0,           0.D0,            0.D0,
     *             0.D0,           .5D0,            0.D0,
     *             0.D0, 0.3333333333D0,           0.D0,
     *   0.3333333333D0,           0.D0,  0.3333333333D0,
     *            0.D0,           0.D0,            0.D0,
     *             0.D0,           0.D0,            0.D0,
     *            0.D0, 0.3333333333D0,            0.D0,
     *   0.3333333333D0,           0.D0,  0.3333333333D0,
     *            .5D0,           0.D0,            0.D0,
     *             0.D0,           .5D0,            0.D0,
     *            0.D0,  0.3333333333D0,           0.D0,
     *  0.3333333333D0,             0.D0, 0.3333333333D0/

C      case boundary EF g6=-g1, g1+g2+g4+g5+g6 = 0
       data (prj(i,25),i=1,36) /
     *           0.5D0,            0.D0,          0.D0,
     *             0.D0,            0.D0,        -0.5D0,
     *            0.D0,  0.6666666667D0,          0.D0,
     *  -0.3333333333D0, -0.3333333333D0,          0.D0,
     *            0.D0,            0.D0,          1.D0,
     *             0.D0,            0.D0,          0.D0,
     *            0.D0, -0.3333333333D0,          0.D0,
     *   0.6666666667D0, -0.3333333333D0,          0.D0,
     *            0.D0, -0.3333333333D0,          0.D0,
     *  -0.3333333333D0,  0.6666666667D0,          0.D0,
     *          -0.5D0,            0.D0,          0.D0,
     *             0.D0,            0.D0,         0.5D0/
       data (prjperp(i,25),i=1,36) /
     *           0.5D0,            0.D0,          0.D0,
     *             0.D0,            0.D0,         0.5D0,
     *            0.D0,  0.3333333333D0,          0.D0,
     *   0.3333333333D0,  0.3333333333D0,          0.D0, 
     *            0.D0,            0.D0,          0.D0,
     *             0.D0,            0.D0,          0.D0,
     *            0.D0,  0.3333333333D0,          0.D0,
     *   0.3333333333D0,  0.3333333333D0,          0.D0, 
     *            0.D0,  0.3333333333D0,          0.D0,
     *   0.3333333333D0,  0.3333333333D0,          0.D0, 
     *           0.5D0,            0.D0,          0.D0,
     *             0.D0,            0.D0,         0.5D0/



C
C    The following matrices are the transformation
C    matrices that may be applied at the associated
C    boundaries
C
C    M_1 (g1 = g2, a -> b, b -> a)
      DATA (MS(I,1), I=1,36) /
     2 0,1,0,0,0,0,
     3 1,0,0,0,0,0,
     4 0,0,1,0,0,0,
     5 0,0,0,0,1,0,
     6 0,0,0,1,0,0,
     7 0,0,0,0,0,1 /

C    M_2 (g2 = g3, b -> c, c -> b)
      DATA (MS(I,2), I=1,36) /
     2 1,0,0,0,0,0,
     3 0,0,1,0,0,0,
     4 0,1,0,0,0,0,
     5 0,0,0,1,0,0,
     6 0,0,0,0,0,1,
     7 0,0,0,0,1,0 /

C    M_3 (g4 = 0, a -> -a)
      DATA (MS(I,3),I=1,36) /
     2 1,0,0,0,0,0,
     3 0,1,0,0,0,0,
     4 0,0,1,0,0,0,
     5 0,0,0,1,0,0,
     6 0,0,0,0,-1,0,
     7 0,0,0,0,0,-1 /

C    M_4 (g5 = 0, b -> -b)
      DATA (MS(I,4),I=1,36) /
     2 1,0,0,0,0,0,
     3 0,1,0,0,0,0,
     4 0,0,1,0,0,0,
     5 0,0,0,-1,0,0,
     6 0,0,0,0,1,0,
     7 0,0,0,0,0,-1 /

C    M_5 (g6 = 0, c -> -c)
      DATA (MS(I,5),I=1,36) /
     2 1,0,0,0,0,0,
     3 0,1,0,0,0,0,
     4 0,0,1,0,0,0,
     5 0,0,0,-1,0,0,
     6 0,0,0,0,-1,0,
     7 0,0,0,0,0,1 /

C    M_6 (g2 = g4, g5 >= g6, b -> -b, c -> b - c)
      DATA (MS(I,6),I=1,36) /
     2 1, 0, 0, 0, 0, 0,
     3 0, 1, 0, 0, 0, 0,
     4 0, 1, 1,-1, 0, 0,
     5 0,-2, 0, 1, 0, 0,
     6 0, 0, 0, 0,-1, 1,
     7 0, 0, 0, 0, 0,-1 /

C    M_7 (g2 = g4, g5 < g6, c -> b - c)
      DATA (MS(I,7),I=1,36) /
     2 1, 0, 0, 0, 0, 0,
     3 0, 1, 0, 0, 0, 0,
     4 0, 1, 1,-1, 0, 0,
     5 0, 2, 0,-1, 0, 0,
     6 0, 0, 0, 0,-1, 1,
     7 0, 0, 0, 0, 0, 1 /

C    M_8 (g2 = -g4, a -> -a, c -> b + c)
      DATA (MS(I,8),I=1,36) /
     2 1, 0, 0, 0, 0, 0,
     3 0, 1, 0, 0, 0, 0,
     4 0, 1, 1, 1, 0, 0,
     5 0, 2, 0, 1, 0, 0,
     6 0, 0, 0, 0,-1,-1,
     7 0, 0, 0, 0, 0,-1 /

C    M_9 (g1 = g5, g4 >= g6, b -> -b, c -> c - a)
      DATA (MS(I,9),I=1,36) /
     2 1, 0, 0, 0, 0, 0,
     3 0, 1, 0, 0, 0, 0,
     4 1, 0, 1, 0,-1, 0,
     5 0, 0, 0,-1, 0, 1,
     6-2, 0, 0, 0, 1, 0,
     7 0, 0, 0, 0, 0,-1 /

C    M_A (g1 = g5, g4 < g6, c -> a - c)
      DATA (MS(I,10),I=1,36) /
     2 1, 0, 0, 0, 0, 0,
     3 0, 1, 0, 0, 0, 0,
     4 1, 0, 1, 0,-1, 0,
     5 0, 0, 0,-1, 0, 1,
     6 2, 0, 0, 0,-1, 0,
     7 0, 0, 0, 0, 0, 1 /

C    M_B (g1 = -g5, b -> -b, c -> a + c)
      DATA (MS(I,11),I=1,36) /
     2 1, 0, 0, 0, 0, 0,
     3 0, 1, 0, 0, 0, 0,
     4 1, 0, 1, 0, 1, 0,
     5 0, 0, 0,-1, 0,-1,
     6 2, 0, 0, 0, 1, 0,
     7 0, 0, 0, 0, 0,-1 /

C    M_C (g1 = g6, g4 >= g5, b -> -b, b -> b - a)
      DATA (MS(I,12),I=1,36) /
     2 1, 0, 0, 0, 0, 0,
     3 1, 1, 0, 0, 0,-1,
     4 0, 0, 1, 0, 0, 0,
     5 0, 0, 0,-1, 1, 0,
     6 0, 0, 0, 0,-1, 0,
     7-2, 0, 0, 0, 0, 1 /

C    M_D (g1 = g6, g4 < g5, b -> a - b)
      DATA (MS(I,13),I=1,36) /
     2 1, 0, 0, 0, 0, 0,
     3 1, 1, 0, 0, 0,-1,
     4 0, 0, 1, 0, 0, 0,
     5 0, 0, 0,-1, 1, 0,
     6 0, 0, 0, 0, 1, 0,
     7 2, 0, 0, 0, 0,-1 /

C    M_E (g1 = -g6, b -> a + b, c -> -c )
      DATA (MS(I,14),I=1,36) /
     2 1, 0, 0, 0, 0, 0,
     3 1, 1, 0, 0, 0, 1,
     4 0, 0, 1, 0, 0, 0,
     5 0, 0, 0,-1,-1, 0,
     6 0, 0, 0, 0,-1, 0,
     7 2, 0, 0, 0, 0, 1 /

C    M_F (g1+g2+g3+g4+g5+g6 = g3, c -> -(a+b+c))
      DATA (MS(I,15),I=1,36) /
     2 1, 0, 0, 0, 0, 0,
     3 0, 1, 0, 0, 0, 0,
     4 1, 1, 1, 1, 1, 1,
     5 0,-2, 0,-1, 0,-1,
     6-2, 0, 0, 0,-1,-1,
     7 0, 0, 0, 0, 0, 1 /

C    M_1.M_2 (a -> b, b -> c, c -> a)
      DATA (MS(I,16), I=1,36) /
     2 0,0,1,0,0,0,
     3 1,0,0,0,0,0,
     4 0,1,0,0,0,0,
     5 0,0,0,0,0,1,
     6 0,0,0,1,0,0,
     7 0,0,0,0,1,0 /

C    M_2.M_1 (a -> c, b -> a, c -> b)
      DATA (MS(I,17), I=1,36) /
     2 0,1,0,0,0,0,
     3 0,0,1,0,0,0,
     4 1,0,0,0,0,0,
     5 0,0,0,0,1,0,
     6 0,0,0,0,0,1,
     7 0,0,0,1,0,0 /

C    M_2.M_1.M_2 (a -> c, c -> a)
      DATA (MS(I,18), I=1,36) /
     2 0,0,1,0,0,0,
     3 0,1,0,0,0,0,
     4 1,0,0,0,0,0,
     5 0,0,0,0,0,1,
     6 0,0,0,0,1,0,
     7 0,0,0,1,0,0/

      END

      Block Data histogram
      integer ihist(22,36)
      common /hist/ihist
      data ihist/792*0/
      end

C**********************************************************************/


C      CALL MKGAOL (MXTREE,TREE,NVMAX,V,NV,GRED,GE,
C    *  RATIO, VDIST, IVB,'MKGAOL')

C**********************************************************************C
      SUBROUTINE MKGAOL
     * (MXTREE,TREE,NVMAX,V,NV,GRED,GE,RATIO,
     *  VDIST,IVB,TEST)
      implicit none
      integer mxswtc, mxtree, nvmax, nv
      real*8 ratio
      PARAMETER (MXSWTC=3)
      CHARACTER *6 TEST
      real*8 TREE(MXTREE), V(6,NVMAX), GRED(6), GE(6)
      real*8 VDIST(NVMAX)
      real*8 NEWGE(6)
      real*8 VT(6),VTRED(6)
      real*8 dmin, gerr, grmin
      real*8 XDOTVN, NCDIST
      integer IVB(NVMAX)
      integer i, nv1, nexamined, id
      integer nearst_N
      real*8 SEEDS(6,15)
      real*8 SEEDDIST(15)
      LOGICAL GOODSEED(15)
      LOGICAL NEARRED,NEAR2RED,NRED
      real*8 dred

      real*8 prj(36,25), prjperp(36,25)
      real*8 prjhat(36,15)
      integer ms(36,18)
      common /projectors/ prj,prjperp,prjhat,ms





C----------------------------------------------------------------------C

      IF (TEST .NE. 'MKGAOL') THEN
         WRITE (*,*) ' TEST WAS WRONG IN MKGAOL'
         STOP
      ENDIF

C     Make the perpendicular projectors
C     Each of the PERP projectors takes a g6
C     vector to the space othgonal to the
C     corresponding PRJ.  The length of that
C     vector is therefore the distance to that
C     polytope


C     Generate seed points on each of the 15 boundaries

      call GENSEEDS(GRED,GE,SEEDS,SEEDDIST,
     * GOODSEED, PRJ, PRJPERP)

      do i=1,MXTREE
        Tree(i) = 0
      enddo

      NV = 0
      gerr = DSQRT(XDOTVN(6,GRED,GRED))*RATIO
      DMIN = min(0.05,0.1*DSQRT(XDOTVN(6,GRED,GRED))*RATIO)
      grmin = gred(1)**2+gred(2)**2+gred(3)**2
      if (DMIN.GT.999999999.9989D0) THEN
        WRITE (*,'(a,G13.3)') ' DMIN IN MKGOAL ',DMIN
      else
        WRITE (*,'(a,F13.3)') ' DMIN IN MKGOAL ',DMIN
      endif
C     Populate the tree with the original cell and
C     the nearby neighbors on the boundary
      NV = NV+1
C      write(*,*) "calling bldtre_n"
      CALL BLDTRE_N (.FALSE., MXTREE,6, GRED,NV*16,TREE,'BLDTRE_N')
      CALL CPYVN(6,GRED,V(1,NV))
      nexamined = NV
      IVB(NV) = 0
      VDIST(NV) = 0.D0
C      write(*,*)"SEED DIST",NV,SEEDDIST(NV)
C      call printg6('Storing ',GRED)
C
C     Add the good seeds that are not duplicates to
C     the tree, and for each one apply the matrix
C     for that boundary and if the image is nearly
C     reduced, add that as well.
C
      do i = 1,15
        if (GOODSEED(i)) then
C        write(*,'(a,Z2,6f9.2)')
C     *   "I, SEEDS(I) =",I, SEEDS(1,i),SEEDS(2,i),
C     *   SEEDS(3,i), SEEDS(4,i), SEEDS(5,i), SEEDS(6,i)

          NV1 = NV + 1
          IF (NEARST_N(.FALSE.,MXTREE,6,SEEDS(1,i),DMIN,TREE,NV1,ID,
     *      'NEARST_N') .EQ. 0) THEN
            NV = NV + 1
            CALL BLDTRE_N (.FALSE., MXTREE,6, SEEDS(1,i),
     *            NV*16+i,TREE,'BLDTRE_N')
            CALL CPYVN(6,SEEDS(1,i),V(1,NV))
            IVB(NV) = 1
            VDIST(NV) = SEEDDIST(i)
            if (VDIST(NV).LT.1.D-4) VDIST(NV) = 0.D0
C            write(*,*)"NV, SEED DIST",NV,VDIST(NV)
C            call printg6('Store SEED ',SEEDS(1,i))
          ENDIF
          call imv6(SEEDS(1,i),MS(1,i),vt)
          NV1 = NV+1
          NRED=NEARRED(vt,1.0D-6,'NEARRED')
          if (NRED) THEN
          VDIST(NV1) = NCDIST(vt,GRED)
          if (VDIST(NV1).LT.gerr .and.
     *       NEARST_N(.false.,MXTREE,6,VT,DMIN,TREE,NV1,ID,
     *      'NEARST_N') .EQ. 0) THEN
            NV = NV+1
            CALL BLDTRE_N (.FALSE.,MXTREE,6,VT,NV*16+i,TREE,'BLDTRE_N')
            CALL CPYVN(6,VT,V(1,NV))
            IVB(NV) = 1
            if (VDIST(NV).LT.1.D-4) VDIST(NV) = 0.D0
C            write(*,*)"NV, M*SEED DIST",NV,VDIST(NV)
C            call printg6('Store M*SEED ',VT)
          ENDIF
          ELSE
          NRED=NEAR2RED(vt,gerr,vtred,dred,'NEAR2RED')
          VDIST(NV1) = NCDIST(vtred,GRED)
          if (VDIST(NV1).LT.gerr .and.
     *       NEARST_N(.false.,MXTREE,6,vtred,DMIN,TREE,NV1,ID,
     *      'NEARST_N') .EQ. 0) THEN
            NV = NV+1
            CALL BLDTRE_N(.FALSE.,MXTREE,6,vtred,
     *          NV*16+i,TREE,'BLDTRE_N')
            CALL CPYVN(6,vtred,V(1,NV))
            IVB(NV) = 1
            if (VDIST(NV).LT.1.D-4) VDIST(NV) = 0.D0
C            write(*,*)"NV, M*SEED ** DIST",NV,VDIST(NV)
C            call printg6('Store M*SEED ** ',VT)
          ENDIF
          ENDIF
C        else
C           call printg6('Reject SEED ',SEEDS(1,i))
        endif
      enddo

      DO I = 1,6
        NEWGE(I) = max(GE(I),DMIN)
      enddo
C
C     Now we have seeds to examine from nexamined+1
C     through NV.  In the course of doing so, NV may
C     increase.

1000  nexamined = nexamined+1
      if (nexamined .gt. NV .OR. NV.GT.NVMAX-31) THEN
         WRITE (*,*) ' NV IN MKREFL ',NV
         return
      ENDIF
      if (ivb(nexamined).gt.10) go to 1000

      DO I = 1,6
        NEWGE(I) = max(GE(I)/(1.414D0**IVB(nexamined)),DMIN)
      ENDDO

      gerr = DSQRT(XDOTVN(6,NEWGE,NEWGE))

C     Generate seed points on each of the 15 boundaries

      call GENSEEDS(V(1,nexamined),NEWGE,SEEDS,SEEDDIST,
     * GOODSEED, PRJ, PRJPERP)
C
C     Add the good seeds that are not duplicates to
C     the tree, and for each one apply the matrix
C     for that boundary and if the image is nearly
C     reduced, add that as well.
C
C     Differs from the prior loop only in the distance
C     calculation
C
      do i = 1,15
        if (GOODSEED(i) .and.
     *    SEEDS(1,i)**2 + SEEDS(2,i)**2 + SEEDS(3,i)**2
     *    .lt. 10.0D0*grmin) then
          NV1 = NV + 1
          IF (NEARST_N(.false.,MXTREE,6,SEEDS(1,i),DMIN
     *       *(1.85D0**IVB(nexamined)),TREE,NV1,ID,
     *      'NEARST_N') .EQ. 0) THEN
            NV = NV + 1
            CALL BLDTRE_N (.FALSE., MXTREE,6, SEEDS(1,i),
     *           NV*16+i,TREE,'BLDTRE_N')
            CALL CPYVN(6,SEEDS(1,i),V(1,NV))
            IVB(NV) = IVB(nexamined)+1
            VDIST(NV) = dsqrt(SEEDDIST(i)**2+
     *        VDIST(nexamined)**2)
            if (VDIST(NV).LT.1.D-4) VDIST(NV) = 0.D0
C            write(*,*)"NV, SEED DIST,IB",NV,VDIST(NV),IB
C            call printg6('Store SEED ',SEEDS(1,i))
          ENDIF
          call imv6(SEEDS(1,i),MS(1,i),vt)
          NV1 = NV+1
          NRED=NEARRED(vt,1.0D-6,'NEARRED')
          if (NRED) THEN
          VDIST(NV1) = NCDIST(vt,GRED)
          if (VDIST(NV1).LT.gerr .and.
     *       NEARST_N(.false.,MXTREE,6,VT,DMIN
     *       *(1.85D0**IVB(nexamined)),TREE,NV1,ID,
     *      'NEARST_N') .EQ. 0) THEN
            NV = NV+1
            CALL BLDTRE_N (.FALSE.,MXTREE,6,VT,
     *          NV*16+i,TREE,'BLDTRE_N')
            CALL CPYVN(6,VT,V(1,NV))
            IVB(NV) = IVB(nexamined)+1
            if (VDIST(NV).LT.1.D-4) VDIST(NV) = 0.D0
C            write(*,*)"NV, M*SEED DIST,IB",NV,VDIST(NV),IB
C            call printg6('Store M*SEED ',VT)
          ENDIF
          ELSE
          NRED=NEAR2RED(vt,gerr,vtred,dred,'NEAR2RED')
          VDIST(NV1) = NCDIST(vtred,GRED)
          if (VDIST(NV1).LT.gerr .and.
     *       NEARST_N(.false.,MXTREE,6,vtred,DMIN
     *       *(1.85D0**IVB(nexamined)),TREE,NV1,ID,
     *      'NEARST_N') .EQ. 0) THEN
            NV = NV+1
            CALL BLDTRE_N (.FALSE.,MXTREE,6,vtred,
     *           NV*16+i,TREE,'BLDTRE_N')
            CALL CPYVN(6,vtred,V(1,NV))
            IVB(NV) = 1
            if (VDIST(NV).LT.1.D-4) VDIST(NV) = 0.D0
C            write(*,*)"NV, M*SEED ** DIST",NV,VDIST(NV)
C            call printg6('Store M*SEED ** ',VT)
          ENDIF
          ENDIF
C        else
C           call printg6('Reject SEED ',SEEDS(1,i))
        endif
      enddo
      go to 1000
      end

      SUBROUTINE MKPERP( PRJ, PRJPERP)
      implicit none
      real*8 PRJ(36),PRJPERP(36)
      integer i
      do i = 1,36
        PRJPERP(i) = - PRJ(i)
      enddo
      do i = 1,36,7
        PRJPERP(i) = 1.D0+PRJPERP(i)
      enddo
C      WRITE(*,*) "PRJ",PRJ
C      WRITE(*,*) "PRJPERP",PRJPERP
      RETURN
      END

      SUBROUTINE PRINTG6(TEXT,VECTOR)
      implicit none
      character*(*) TEXT
      real*8 VECTOR(6)
C      real*8 FVECTOR(6)
      WRITE(*,'(A,1x,6F9.2)')TEXT,VECTOR

      RETURN
      END


      SUBROUTINE GENSEEDS( GVEC, GE, SEEDS, SEEDDIST,
     * GOODSEED, PRJ, PRJPERP)
C
C     GENSEEDS TAKES AN ARBITRARY G6 VECTOR AND PROJECTS
C     IT ONTO EACH OF THE 15 BOUNDARIES.  EVEN IF THE
C     ORIGINAL VECTOR WAS REDUCED THE PROJECTED VECTOR
C     MAY FAIL TO BE REDUCED, AND MAY EVEN FAIL TO BE
C     NEARLY REDUCED.  IT WILL BE MARKED AS A GOOD SEED
C     PROVIDED IT IS BOTH NEARLY REDUCED AND IS WITHIN
C     3.5 TIMES THE ERROR BOX GE OF GVEC.
C
      implicit none
      real*8 GVEC(6), GE(6)
      real*8 PRJ(36,25),PRJPERP(36,25)
      real*8 SEEDS(6,15), SEEDDIST(15)
      logical GOODSEED(15)
C      character*20 SEEDLAB
      real*8 vtemp(6)
      real*8 boundary67
      real*8 boundary9A
      real*8 boundaryCD
      real*8 a,dred
      integer i, ip
      real*8 anorm, gerr
      logical NEARRED,NEAR2RED
C----------------------------------------------------------------------C

C     Compute the seeds and the seed distances

C      write(*,*) "GENSEEDS GVEC, GE"
C      write(*,*) GVEC
C      write(*,*) GE

      gerr= anorm(6,GE)

      call rmv6( GVEC, PRJPERP(1,19), vtemp )
      boundary67 = anorm(6,vtemp)
      call rmv6( GVEC, PRJPERP(1,20), vtemp )
      boundary9A = anorm(6,vtemp)
      call rmv6( GVEC, PRJPERP(1,21), vtemp )
      boundaryCD = anorm(6,vtemp)

C      call printg6('GENSEEDS GVEC ',GVEC)

      do ip=1,15
        call rmv6( GVEC, PRJ(1,ip), SEEDS(1,ip))
        call rmv6( GVEC, PRJPERP(1,ip), vtemp )
C        write(*,'(A,i2/,6(6F9.3/))'),"PRJ",
C     *  ip,(PRJ(i,ip),i=1,36)
C        write(*,'(A,i2/,6(6F9.3/))'),"PRJPERP",
C     *  ip,(PRJPERP(i,ip),i=1,36)
        a = anorm(6,vtemp)
        if (((GVEC(4)-1.d-6*sqrt(GVEC(2)*GVEC(3)))*
     *     (GVEC(5)-1.d-6*sqrt(GVEC(1)*GVEC(3)))*
     *     (GVEC(6)-1.d-6*sqrt(GVEC(1)*GVEC(3)))
     *     .le. 0.D0
     *        .and. (ip .eq. 6 .or. ip .eq. 7
     *          .or. ip .eq. 9 .or. ip .eq. 10
     *          .or. ip .eq. 12.or. ip .eq. 13))
     *        .or. (
     *     (GVEC(4)-1.d-6*sqrt(GVEC(2)*GVEC(3)))*
     *     (GVEC(5)-1.d-6*sqrt(GVEC(1)*GVEC(3)))*
     *     (GVEC(6)-1.d-6*sqrt(GVEC(1)*GVEC(3)))
     *     .gt. 0.D0
     *        .and. (ip .eq. 8 .or. ip .eq. 11
     *          .or. ip .eq. 14. or. ip .eq. 15))) then
            if ( ip .eq. 6 .or. ip .eq. 7 .or. ip .eq. 8 ) then
              call rmv6( GVEC, PRJ(1,16), SEEDS(1,ip) )
              call rmv6( GVEC, PRJPERP(1,16), vtemp )
            endif
            if ( ip .ge. 9 .and. ip .le. 14) then
              call rmv6( GVEC, PRJ(1,17), SEEDS(1,ip) )
              call rmv6( GVEC, PRJPERP(1,17), vtemp )
            endif
            if ( ip .eq. 15) then
              call rmv6( GVEC, PRJ(1,18), SEEDS(1,ip) )
              call rmv6( GVEC, PRJPERP(1,18), vtemp )
            endif
            a = anorm(6,vtemp)
        endif
        if ( ip .eq.  6 .and. GVEC(5) .le. GVEC(6)
     *     .and. GVEC(5) .gt. 0.D0) then
           a = boundary67
        elseif ( ip .eq.  7 .and. GVEC(5) .ge. GVEC(6)
     *     .and. GVEC(6) .gt. 0.D0) then
           a = boundary67
        elseif ( ip .eq.  9 .and. GVEC(4) .le. GVEC(6)
     *     .and. GVEC(4) .gt. 0.D0 ) then
           a = boundary9A
        elseif ( ip .eq. 10 .and. GVEC(4) .ge. GVEC(6)
     *     .and. GVEC(6) .gt. 0.D0 ) then
           a = boundary9A
        elseif ( ip .eq. 12 .and. GVEC(4) .le. GVEC(5)
     *     .and. GVEC(4) .gt. 0.D0 ) then
           a = boundaryCD
        elseif ( ip .eq. 13 .and. GVEC(4) .ge. GVEC(5)
     *     .and. GVEC(5) .gt. 0.D0 ) then
           a = boundaryCD
        endif

C       get distance and mark as bad if outside
C       3.5 times the errorbox

        SEEDDIST(ip) = a
        GOODSEED(ip) = NEARRED(SEEDS(1,ip),1.0D-6,'NEARRED')
        if (.NOT.GOODSEED(ip)) then
          GOODSEED(ip) = NEAR2RED(SEEDS(1,ip),gerr,
     *    vtemp,dred,'NEAR2RED')
          call CPYVN(6,vtemp,SEEDS(1,ip))
C         call printg6("Non-reduced projection",seeds(1,ip))

        endif
        do i = 1,6
          if (abs(GVEC(i)-SEEDS(i,ip)) .gt.
     *      3.5D0*GE(i)) GOODSEED(ip) = .false.
        enddo
C        write(seedlab,'(''BDRY'', I2)')IP
C        call printg6(seedlab,seeds(1,ip))
      enddo

      return
      end

C
C     Compute XS (distances to boundary sets) and
C             YS (signed distances along boundaries)
C

      SUBROUTINE BDCOORD(GVEC,XS,YS)
      implicit none
      real*8 sqrt2, sqrt5
      real*8 XS(6),YS(6)
      real*8 GVEC(6)
C
C     The boundaries are in the order
C       1, 2, 678, 9AB, CDE, F
      sqrt2 = SQRT(2.D0)
      sqrt5 = SQRT(5.D0)
      XS(1) = (GVEC(2) - GVEC(1))/sqrt2
      XS(2) = (GVEC(3) - GVEC(2))/sqrt2
      XS(3) = (GVEC(2) - ABS(GVEC(4)))/sqrt2
      XS(4) = (GVEC(1) - ABS(GVEC(5)))/sqrt2
      XS(5) = (GVEC(1) - ABS(GVEC(6)))/sqrt2
      XS(6) = (GVEC(1)+GVEC(2) +
     *  GVEC(4) + GVEC(5) + GVEC(6))/sqrt5
      YS(1) = (ABS(GVEC(4)) - ABS(GVEC(5)))/sqrt2
      YS(2) = (ABS(GVEC(5)) - ABS(GVEC(6)))/sqrt2
      YS(3) = (GVEC(5) - ABS(GVEC(6)/2.D0))/sqrt2
      YS(4) = (GVEC(4) - ABS(GVEC(6)/2.D0))/sqrt2
      YS(5) = (GVEC(4) - ABS(GVEC(5)/2.D0))/sqrt2
      YS(6) = (GVEC(2) - GVEC(1) + GVEC(4) - GVEC(5))
     *  /2.D0
      END

C    Map a G6 vector onto the boundaries after
C    applying the 24-way unfolding

      subroutine bdmaps(gvec,vecs, dists, pgs, mpgs)

      implicit none

      real*8 prj(36,25), prjperp(36,25)
      real*8 prjhat(36,15)
      integer ms(36,18)
      common /projectors/ prj,prjperp,prjhat,ms

      integer ii, jj
      real*8 xtemp

      real*8 gvec(6), vecs(6,24), dists(24,15)
      real*8 pgs(6,24,15),mpgs(6,24,15)

C
C      1 --  6 +++
C      7 -- 12 +--
C     13 -- 18 -+-
C     19 -- 24 --+
C

      do ii = 1, 24
        call cpyvn(6,gvec,vecs(1,ii))
        if (ii.ge.7.and.ii.le.18)
     *      vecs(6,ii) = -vecs(6,ii)
        if ((ii.ge.7.and.ii.le.12)
     *       .or.(ii.ge.19))
     *      vecs(5,ii) = -vecs(5,ii)
        if (ii.ge.13)
     *      vecs(4,ii) = -vecs(4,ii)
        jj = mod(ii-1,6)
        if (jj.eq.2.or.jj.eq.3) then
          xtemp = vecs(1,ii)
          vecs(1,ii) = vecs(2,ii)
          vecs(2,ii) = xtemp
          xtemp = vecs(4,ii)
          vecs(4,ii) = vecs(5,ii)
          vecs(5,ii) = xtemp
        endif
        if (jj.eq.1.or.jj.ge.4) then
          xtemp = vecs(2,ii)
          vecs(2,ii) = vecs(3,ii)
          vecs(3,ii) = xtemp
          xtemp = vecs(5,ii)
          vecs(5,ii) = vecs(6,ii)
          vecs(6,ii) = xtemp
        endif
        if (jj.eq.4) then
          xtemp = vecs(1,ii)
          vecs(1,ii) = vecs(2,ii)
          vecs(2,ii) = xtemp
          xtemp = vecs(4,ii)
          vecs(4,ii) = vecs(5,ii)
          vecs(5,ii) = xtemp
        endif
        if (jj.eq.3.or.jj.eq.5) then
          xtemp = vecs(2,ii)
          vecs(2,ii) = vecs(3,ii)
          vecs(3,ii) = xtemp
          xtemp = vecs(5,ii)
          vecs(5,ii) = vecs(6,ii)
          vecs(6,ii) = xtemp
        endif
        dists(ii,1) =
     * dabs(vecs(2,ii)-vecs(1,ii))/dsqrt(2.D0)
        dists(ii,2) =
     * dabs(vecs(3,ii)-vecs(2,ii))/dsqrt(2.D0)
        dists(ii,3) = dabs(vecs(4,ii))
        dists(ii,4) = dabs(vecs(5,ii))
        dists(ii,5) = dabs(vecs(6,ii))
        dists(ii,6) =
     * dabs(vecs(2,ii)-vecs(4,ii))/dsqrt(2.D0)
        dists(ii,7) =
     * dabs(vecs(2,ii)-vecs(4,ii))/dsqrt(2.D0)
        dists(ii,8) =
     * dabs(vecs(2,ii)+vecs(4,ii))/dsqrt(2.D0)
        dists(ii,9) =
     * dabs(vecs(1,ii)-vecs(5,ii))/dsqrt(2.D0)
        dists(ii,10) =
     * dabs(vecs(1,ii)-vecs(5,ii))/dsqrt(2.D0)
        dists(ii,11) =
     * dabs(vecs(1,ii)+vecs(5,ii))/dsqrt(2.D0)
        dists(ii,12) =
     * dabs(vecs(1,ii)-vecs(6,ii))/dsqrt(2.D0)
        dists(ii,13) =
     * dabs(vecs(1,ii)-vecs(6,ii))/dsqrt(2.D0)
        dists(ii,14) =
     * dabs(vecs(1,ii)+vecs(6,ii))/dsqrt(2.D0)
        dists(ii,15) =
     * dabs(vecs(1,ii)+vecs(2,ii)+vecs(4,ii)
     *   +vecs(5,ii)+vecs(6,ii))/dsqrt(5.D0)
        do jj = 1,15
          call rmv6(vecs(1,ii),
     *      PRJ(1,jj), pgs(1,ii,jj))
        call imv6(pgs(1,ii,jj),
     *      MS(1,jj),mpgs(1,ii,jj))
        enddo
      enddo

      return
      end

C    Map a G6 vector onto the intersection of the
C    face diagonal and body diagonal boundaries.
C
C    For a given g6 point, we need the distance
C    to  8F directly or via 69 and M_6
C        BF directly of via 69 and M_9
C        EF firectly or via 6C and M_C
C    in addition we need M_F applied to the
C    F targets
C
C    So the distances and pgs in order are
C    ||P_8F_perp.g||  P_8F.g  M_8.P_8F.g
C    ||P_BF_perp.g||  P_BF.g  M_B.P_BF.g
C    ||P_EF_perp.g||  P_EF.g  M_E.P_EF.g
C    ||P_6C_perp.g||  P_6C.g  M_C.P_6C.g
C    ||P_69_perp.g||  P_69.g  M_6.P_69.g
C                             M_9.P_69.g
C    
C     bfmaps must be called first to generate the vecs
C     array.
C
C     The viable pairs for a given g1 and g2 for
C     consideration are then
C
C     boundary  distances
C     69:  ||P_69_perp.g1|| ||P_69_perp.g2|| P_69.g1 -- P_69.g2
C          ||P_69_perp.g1|| ||P_8F_perp.g2|| P_69.g1 -- M_8.P_8F.g2
C          ||P_69_perp.g1|| ||P_BF_perp.g2|| P_69.g1 -- M_B.P_BF.g2
C          and with g1 and g2 exchanged
C     6C:  ||P_6C_perp.g1|| ||P_6C_perp.g2|| P_6C.g1 -- P_6C.g2
C          ||P_6C_perp.g1|| ||P_EF_perp.g2|| P_69.g1 -- M_C.P_EF.g2
C          and with g1 and g2 exchanged
C     8F:  ||P_8F_perp.g1|| ||P_8F_perp.g2|| P_8F.g1 -- P_8F.g2
C          ||P_8F_perp.g1|| ||P_69_perp.g2|| P_8F.g1 -- M_6.P_69.g
C     BF:  ||P_BF_perp.g1|| ||P_BF_perp.g2|| P_BF.g1 -- P_BF.g2
C          ||P_BF_perp.g1|| ||P_69_perp.g2|| P_BF.g1 -- M_9.P_69.g
C     EF:  ||P_EF_perp.g1|| ||P_EF_perp.g2|| P_EF.g1 -- P_EF.g2
C          ||P_EF_perp.g1|| ||P_6C_perp.g2|| P_EF.g1 -- M_C.P_6C.g

      subroutine bdfmaps(vecs, dists, pgs, mpgs,
     *  nmpgs)

      implicit none

      real*8 prj(36,25), prjperp(36,25)
      real*8 prjhat(36,15)
      integer ms(36,18)
      common /projectors/ prj,prjperp,prjhat,ms

      integer ii, jj
      real*8 xtemp

      real*8 vecs(6,24), dists(24,5)
      real*8 pgs(6,24,5),mpgs(6,24,6)
      integer nmpgs(5)
      real*8 pgtemp(6)

      do ii = 1, 24
        dists(ii,4) = dsqrt((vecs(1,ii)-vecs(6,ii))**2
     *                    + (vecs(2,ii)-vecs(4,ii))**2)/
     *                dsqrt(2.D0)
        dists(ii,5) = dsqrt((vecs(1,ii)-vecs(5,ii))**2
     *                    + (vecs(2,ii)-vecs(4,ii))**2)/
     *                dsqrt(2.D0)
        dists(ii,1) = dsqrt(
     *  2.D0*(vecs(6,ii)+vecs(5,ii)+vecs(1,ii))**2
     *  +3.D0*(vecs(4,ii)+vecs(2,ii))**2
     *  )/dsqrt(6.D0)
        dists(ii,2) = dsqrt(
     *  2.D0*(vecs(6,ii)+vecs(4,ii)+vecs(2,ii))**2
     *  +3.D0*(vecs(5,ii)+vecs(1,ii))**2
     *  )/dsqrt(6.D0)
        dists(ii,3) = dsqrt(
     *  2.D0*(vecs(5,ii)+vecs(4,ii)+vecs(2,ii))**2
     *  +3.D0*(vecs(6,ii)+vecs(1,ii))**2
     *  )/dsqrt(6.D0)
C
C       PRJ(*,23) is P_8F
C       PRJ(*,24) is P_BF
C       PRJ(*,25) is P_EF
C
        do jj = 1,3
          call rmv6(vecs(1,ii),
     *      PRJ(1,22+jj), pgs(1,ii,jj))
          call imv6(pgs(1,ii,jj),
     *      MS(1,jj*3+5),mpgs(1,ii,jj))
        enddo
        call rmv6(vecs(1,ii),PRJ(1,6),pgtemp)
        call rmv6(pgtemp,PRJ(1,12),pgs(1,ii,4))
        call rmv6(pgtemp,PRJ(1,9),pgs(1,ii,5))
        call imv6(pgs(1,ii,4),
     *      MS(1,12),mpgs(1,ii,4))
        call imv6(pgs(1,ii,5),
     *      MS(1,6),mpgs(1,ii,5))
        call imv6(pgs(1,ii,5),
     *      MS(1,9),mpgs(1,ii,6))
C           dists(ii,1) = 1.D38
C           dists(ii,2) = 1.D38
C           dists(ii,3) = 1.D38
C           dists(ii,4) = 1.D38
C           dists(ii,5) = 1.D38
      enddo
      nmpgs(1) = 1
      nmpgs(2) = 1
      nmpgs(3) = 1
      nmpgs(4) = 1
      nmpgs(5) = 2
      return
      end


C     Compute the best distance between 2 G6 vectors
C     allowing for permulations of g1, g2, g3 as
C     well as sign changes
C
      real*8 function g123dist(v1,v2)
      implicit none
      real*8 v1(6),v2(6),vtemp(6)
      real*8 g456dist
      integer i
      common/xdebug/xdebug
      logical xdebug
      do i = 1,6
        vtemp(i) = v2(i)
      enddo
C     123
      g123dist = g456dist(v1,vtemp)
C     213
      vtemp(1)=v2(2)
      vtemp(2)=v2(1)
      vtemp(4)=v2(5)
      vtemp(5)=v2(4)
      g123dist = min(g123dist,g456dist(v1,vtemp))
C     231
      vtemp(2)=v2(3)
      vtemp(3)=v2(1)
      vtemp(5)=v2(6)
      vtemp(6)=v2(4)
      g123dist = min(g123dist,g456dist(v1,vtemp))
C     321
      vtemp(1)=v2(3)
      vtemp(2)=v2(2)
      vtemp(4)=v2(6)
      vtemp(5)=v2(5)
      g123dist = min(g123dist,g456dist(v1,vtemp))
C     312
      vtemp(3)=v2(2)
      vtemp(2)=v2(1)
      vtemp(6)=v2(5)
      vtemp(5)=v2(4)
      g123dist = min(g123dist,g456dist(v1,vtemp))
C     132
      vtemp(1)=v2(1)
      vtemp(2)=v2(3)
      vtemp(4)=v2(4)
      vtemp(5)=v2(6)
      g123dist = min(g123dist,g456dist(v1,vtemp))
      return
      end


C     Compute the best distance between 2 G6 vectors
C     allowing for cell-preserving sign changes in
C     g4,5,6
C
      real*8 function g456dist(v1,v2)
      implicit none
      real*8 v1(6),v2(6),vtemp
      real*8 xdot
      common/xdebug/xdebug
      logical xdebug

      integer ii

      xdot = 0.D0

      do ii = 1, 6
        vtemp = v1(ii)-v2(ii)
        xdot = xdot+vtemp*vtemp
      enddo
      g456dist = sqrt(xdot
     *  +4.D0*min(0.D0,
     *         v1(4)*v2(4)+v1(5)*v2(5),
     *         v1(4)*v2(4)+v1(6)*v2(6),
     *         v1(5)*v2(5)+v1(6)*v2(6)))
C       if (xdebug) then
C         write(7,'(a,1x,6f9.2,1x,6f9.2,1x,f12.4)')
C     *  'g456dist',v1,v2,g456dist
C       endif
      end

C
C     Compute the minimal distance between two Niggli-reduced
C     vectors in the Niggli Cone
C
C
      REAL*8 FUNCTION NCDIST(gv1,gv2)
      implicit none
      real*8 gv1(6), gv2(6)
      real*8 FOLDMDIST
      real*8 NCEDIST
      real*8 g456dist
      common/xdebug/xdebug
      logical xdebug
      real*8 prj(36,25), prjperp(36,25)
      real*8 prjhat(36,15)
      integer ms(36,18)
      common /projectors/ prj,prjperp,prjhat,ms

C      if (xdebug) then
C        write(7,'(a,1x,6f9.2)') "NCDIST gv1 ",gv1
C        write(7,'(a,1x,6f9.2)') "NCDIST gv2 ",gv2
C      endif
      NCDIST = NCEDIST(gv1,gv2)
      return
      end

C     NCEDIST -- distance between two points in the
C     Niggli Cone following the embedding paths
C     to the 15 boundaries

      REAL*8 FUNCTION NCEDIST(gvec1,gvec2)
      implicit none

      real*8 gvec1(6), vecs1(6,24), dists1(24,15)
      real*8 pgs1(6,24,15),mpgs1(6,24,15)
      real*8 fdists1(24,5),fdists2(24,5)
      real*8 fpgs1(6,24,5),fpgs2(6,24,5)
      real*8 fmpgs1(6,24,6), fmpgs2(6,24,6)
      integer nmpgs(5)
      real*8 gvec2(6), vecs2(6,24), dists2(24,15)
      real*8 pgs2(6,24,15),mpgs2(6,24,15)
      real*8 g123dist, g456dist
      real*8 dpg1pg2
      integer jx1, jx2, ix2, jo1, jo2, kx1, kx2
      integer jord(15), jord2(15)
      integer i1,i2,j1,j2
      common/xdebug/xdebug
      logical xdebug
      data jord/1,2,9,10,11,6,7,8,12,13,14,3,4,5,15/
      data jord2/1,2,11,10,9,8,7,6,14,13,12,3,4,5,15/

      call bdmaps(gvec1,vecs1,dists1,pgs1,mpgs1)
      call bdfmaps(vecs1,fdists1,fpgs1,fmpgs1,nmpgs)
      call bdmaps(gvec2,vecs2,dists2,pgs2,mpgs2)
      call bdfmaps(vecs2,fdists2,fpgs2,fmpgs2,nmpgs)

      NCEDIST = g123dist(gvec1,gvec2)

      do i1 = 1,24
      do ix2 = 1,24
      i2 = mod(i1+ix2-2,24)+1
      do jx1 = 1,15
      j1 = jord(jx1)
      if (dists1(i1,j1).lt.NCEDIST) then
        jx2 = jx1
        j2 = jord2(jx2)
        if (j1.eq.j2) then
          if(dists1(i1,j1)+dists2(i2,j2)
     *      .lt.NCEDIST) then
          dpg1pg2 = min(
     *  g456dist(pgs1(1,i1,j1),pgs2(1,i2,j2)),
     *  g456dist(pgs1(1,i1,j1),mpgs2(1,i2,j2)),
     *  g456dist(mpgs1(1,i1,j1),pgs2(1,i2,j2)),
     *  g456dist(mpgs1(1,i1,j1),mpgs2(1,i2,j2)))
          NCEDIST = min(NCEDIST,
     *     dsqrt((dists1(i1,j1)+dists2(i2,j2))**2
     *       + dpg1pg2**2))
          endif
        else
          if (dists1(i1,j1)+dists2(i2,j1)
     *      .lt.NCEDIST) then
          dpg1pg2 =
     *  g456dist(pgs1(1,i1,j1),pgs2(1,i2,j1))
          NCEDIST = min(NCEDIST,
     *     dsqrt((dists1(i1,j1)+dists2(i2,j1))**2
     *       + dpg1pg2**2))
          endif
          if (dists1(i1,j1)+dists2(i2,j2)
     *      .lt.NCEDIST) then
          dpg1pg2 =
     *  g456dist(pgs1(1,i1,j1),mpgs2(1,i2,j2))
          NCEDIST = min(NCEDIST,
     *     dsqrt((dists1(i1,j1)+dists2(i2,j2))**2
     *       + dpg1pg2**2))
          endif
        endif
      endif
      enddo
C     69
      if (fdists1(i1,5)+fdists2(i2,5).lt.NCEDIST) then
C        if (xdebug) then
C          write(7,*)"69 shortcut 1",
C     *     fdists1(i1,5),fdists2(i2,5),
C     *     g456dist(fpgs1(1,i1,5),fpgs2(1,i2,5))
C        endif
        NCEDIST = min(NCEDIST,
     *    dsqrt((fdists1(i1,5)+fdists2(i2,5))**2
     *      + g456dist(fpgs1(1,i1,5),fpgs2(1,i2,5))**2))
      endif
      if (fdists1(i1,5)+fdists2(i2,1).lt.NCEDIST) then
C        if (xdebug) then
C          write(7,*)"69 shortcut 2",
C     *     fdists1(i1,5),fdists2(i2,1),
C     *     g456dist(fpgs1(1,i1,5),fmpgs2(1,i2,1))
C        endif
        NCEDIST = min(NCEDIST,
     *    dsqrt((fdists1(i1,5)+fdists2(i2,1))**2
     *      + g456dist(fpgs1(1,i1,5),fmpgs2(1,i2,1))**2))
      endif
      if (fdists1(i1,5)+fdists2(i2,2).lt.NCEDIST) then
C        if (xdebug) then
C          write(7,*)"69 shortcut 3",
C     *     fdists1(i1,5),fdists2(i2,2),
C     *     g456dist(fpgs1(1,i1,5),fmpgs2(1,i2,2))
C        endif
        NCEDIST = min(NCEDIST,
     *    dsqrt((fdists1(i1,5)+fdists2(i2,2))**2
     *      + g456dist(fpgs1(1,i1,5),fmpgs2(1,i2,2))**2))
      endif
      if (fdists1(i1,1)+fdists2(i2,5).lt.NCEDIST) then
C        if (xdebug) then
C          write(7,*)"69 shortcut 4",
C     *     fdists1(i1,1),fdists2(i2,5),
C     *     g456dist(fmpgs1(1,i1,1),fmpgs2(1,i2,5))
C        endif
        NCEDIST = min(NCEDIST,
     *    dsqrt((fdists1(i1,1)+fdists2(i2,5))**2
     *      + g456dist(fmpgs1(1,i1,1),fpgs2(1,i2,5))**2))
      endif
      if (fdists1(i1,2)+fdists2(i2,5).lt.NCEDIST) then
C        if (xdebug) then
C          write(7,*)"69 shortcut 4",
C     *     fdists1(i1,2),fdists2(i2,5),
C     *     g456dist(fmpgs1(1,i1,2),fpgs2(1,i2,5))
C        endif
        NCEDIST = min(NCEDIST,
     *    dsqrt((fdists1(i1,2)+fdists2(i2,5))**2
     *      + g456dist(fmpgs1(1,i1,2),fpgs2(1,i2,5))**2))
      endif
C     6C
      if (fdists1(i1,4)+fdists2(i2,4).lt.NCEDIST) then
        NCEDIST = min(NCEDIST,
     *    dsqrt((fdists1(i1,4)+fdists2(i2,4))**2
     *      + g456dist(fpgs1(1,i1,4),fpgs2(1,i2,4))**2))
      endif
      if (fdists1(i1,4)+fdists2(i2,3).lt.NCEDIST) then
        NCEDIST = min(NCEDIST,
     *    dsqrt((fdists1(i1,4)+fdists2(i2,3))**2
     *      + g456dist(fpgs1(1,i1,4),fmpgs2(1,i2,3))**2))
      endif
      if (fdists1(i1,3)+fdists2(i2,4).lt.NCEDIST) then
        NCEDIST = min(NCEDIST,
     *    dsqrt((fdists1(i1,3)+fdists2(i2,4))**2
     *      + g456dist(fmpgs1(1,i1,3),fpgs2(1,i2,4))**2))
      endif
C     8F
      if (fdists1(i1,1)+fdists2(i2,1).lt.NCEDIST) then
C        if (xdebug) then
C          write(7,*)"8F shortcut 1",
C     *     fdists1(i1,1),fdists2(i2,1),
C     *     g456dist(fpgs1(1,i1,1),fpgs2(1,i2,1))
C        endif
        NCEDIST = min(NCEDIST,
     *    dsqrt((fdists1(i1,1)+fdists2(i2,1))**2
     *      + g456dist(fpgs1(1,i1,1),fpgs2(1,i2,1))**2))
      endif
      if (fdists1(i1,1)+fdists2(i2,5).lt.NCEDIST) then
C        if (xdebug) then
C          write(7,*)"8F shortcut 2",
C     *     fdists1(i1,1),fdists2(i2,5),
C     *     g456dist(fpgs1(1,i1,1),fmpgs2(1,i2,5))
C        endif
        NCEDIST = min(NCEDIST,
     *    dsqrt((fdists1(i1,1)+fdists2(i2,5))**2
     *      + g456dist(fpgs1(1,i1,1),fmpgs2(1,i2,5))**2))
      endif
      if (fdists1(i1,5)+fdists2(i2,1).lt.NCEDIST) then
C        if (xdebug) then
C          write(7,*)"8F shortcut 3",
C     *     fdists1(i1,5),fdists2(i2,1),
C     *     g456dist(fmpgs1(1,i1,5),fpgs2(1,i2,1))
C        endif
        NCEDIST = min(NCEDIST,
     *    dsqrt((fdists1(i1,5)+fdists2(i2,1))**2
     *      + g456dist(fmpgs1(1,i1,5),fpgs2(1,i2,1))**2))
      endif
C     BF
      if (fdists1(i1,2)+fdists2(i2,2).lt.NCEDIST) then
C        if (xdebug) then
C          write(7,*)"BF shortcut 1",
C     *     fdists1(i1,1),fdists2(i2,1),
C     *     g456dist(fpgs1(1,i1,1),fpgs2(1,i2,1))
C        endif
        NCEDIST = min(NCEDIST,
     *    dsqrt((fdists1(i1,1)+fdists2(i2,1))**2
     *      + g456dist(fpgs1(1,i1,1),fpgs2(1,i2,1))**2))
      endif
      if (fdists1(i1,2)+fdists2(i2,5).lt.NCEDIST) then
C        if (xdebug) then
C          write(7,*)"BF shortcut 2",
C     *     fdists1(i1,2),fdists2(i2,5),
C     *     g456dist(fpgs1(1,i1,2),fmpgs2(1,i2,6))
C        endif
        NCEDIST = min(NCEDIST,
     *    dsqrt((fdists1(i1,2)+fdists2(i2,5))**2
     *      + g456dist(fpgs1(1,i1,2),fmpgs2(1,i2,6))**2))
      endif
      if (fdists1(i1,5)+fdists2(i2,2).lt.NCEDIST) then
C        if (xdebug) then
C          write(7,*)"BF shortcut 3",
C     *     fdists1(i1,5),fdists2(i2,2),
C     *     g456dist(fmpgs1(1,i1,6),fpgs2(1,i2,2))
C        endif
        NCEDIST = min(NCEDIST,
     *    dsqrt((fdists1(i1,5)+fdists2(i2,2))**2
     *      + g456dist(fmpgs1(1,i1,6),fpgs2(1,i2,2))**2))
      endif
C     EF
      if (fdists1(i1,3)+fdists2(i2,3).lt.NCEDIST) then
C        if (xdebug) then
C          write(7,*)"EF shortcut 1",
C     *     fdists1(i1,3),fdists2(i2,3),
C     *     g456dist(fpgs1(1,i1,3),fpgs2(1,i2,3))
C        endif
        NCEDIST = min(NCEDIST,
     *    dsqrt((fdists1(i1,3)+fdists2(i2,3))**2
     *      + g456dist(fpgs1(1,i1,3),fpgs2(1,i2,3))**2))
      endif
      if (fdists1(i1,3)+fdists2(i2,4).lt.NCEDIST) then
C        if (xdebug) then
C          write(7,*)"EF shortcut 2",
C     *     fdists1(i1,3),fdists2(i2,4),
C     *     g456dist(fpgs1(1,i1,3),fmpgs2(1,i2,4))
C        endif
        NCEDIST = min(NCEDIST,
     *    dsqrt((fdists1(i1,3)+fdists2(i2,4))**2
     *      + g456dist(fpgs1(1,i1,3),fmpgs2(1,i2,4))**2))
      endif
      if (fdists1(i1,4)+fdists2(i2,3).lt.NCEDIST) then
C        if (xdebug) then
C          write(7,*)"EF shortcut 3",
C     *     fdists1(i1,4),fdists2(i2,3),
C     *     g456dist(fmpgs1(1,i1,4),fpgs2(1,i2,3))
C        endif
        NCEDIST = min(NCEDIST,
     *    dsqrt((fdists1(i1,4)+fdists2(i2,3))**2
     *      + g456dist(fmpgs1(1,i1,4),fpgs2(1,i2,3))**2))
      endif
      enddo
      enddo
      return
      end

