C      program e3g6
C      real*8 minp(3,3),m(3,3)
C      real*8 m6(6,6)
C      read (*,*) minp
C      call m3tom6 (minp,m6)
C      do 1000 i=1,3
C         write (*,*) (minp(j,i),j=1,3)
C 1000 continue
C      write (*,*)
C      do 2000 i=1,6
C         write (*,*) (m6(j,i),j=1,6)
C 2000 continue
C      write (*,*)
C      write (*,*)
C
C
C      call g6toe3 (m6,m)
C
C      do 3000 i=1,3
C         write (*,*) (m(j,i),j=1,3)
C 3000 continue
C      end

C**********************************************************************C
      real*8 function m3det(m)
      implicit none
      real*8 m(3,3)
C----------------------------------------------------------------------C
      m3det = m(1,1)*m(2,2)*m(3,3)
     *       +m(1,2)*m(2,3)*m(3,1)
     *       +m(1,3)*m(2,1)*m(3,2)
     *       -m(1,1)*m(2,3)*m(3,2)
     *       -m(1,2)*m(2,1)*m(3,3)
     *       -m(1,3)*m(2,2)*m(3,1)
      return
      end

C**********************************************************************C
      subroutine btos (n,bits,s)
      implicit none
      integer n
      integer s(n)
      integer bits,twotoi
      integer i
C----------------------------------------------------------------------C
      
      twotoi = 1
      do 1000 i=1,n
         if (mod(bits/twotoi,2) .eq. 1) then
            s(i) = -1
         else
            s(i) = 1
         endif
         twotoi = 2*twotoi
 1000 continue
      end

C**********************************************************************C
      subroutine g6toe3 (m6IN,m)
      implicit none
      real*8 m6(6,6), m6t(6,6)
      INTEGER M6IN(6,6)
      real*8 m3(3,3), m(3,3)
      real*8 mdif
      external mdif
      integer bits,bitmin
      integer signs(9)
      integer i,j,l
      real*8 t1, t1min
      real*8 m3det
C----------------------------------------------------------------------C

      DO 900 I=1,6
      DO 900 J=1,6
  900 M6(I,J) = DBLE(M6IN(I,J))

      do 1000 i=1,3
      do 1000 j=1,3
         m3(i,j) = dsqrt(m6(i,j))
 1000 continue

      do 5000 bits = 511,0,-1
         call btos(9,bits,signs)
C         write (*,*) ' bits,signs ',bits,signs
         do 2000 l=1,9
            i = (l-1)/3 + 1
            j = l -3*(i-1)
            m(i,j) = signs(l)*m3(i,j)
 2000    continue
         call m3tom6 (m,m6t)
         t1 = mdif(6,m6,m6t)
C         write (*,*)  ' m6t in g6toe3 '
C         do 3000 i=1,6
C 3000    write (*,'(1x,6f8.2)') (m6t(j,i),j=1,6)

C         write (*,*) ' bits,t1 ',bits,t1
         if (bits.eq.511) t1min = t1
         if (t1 .le. t1min) then
            t1min = t1
            bitmin = bits
         endif
         if (t1 .eq. 0.0) go to 5500
 5000 continue
 5500 continue
C      write (*,*) ' t1 ',t1,t1min,bitmin
      if (t1 .ne. 0.0) then
         call btos (9,bitmin,signs)
         do 6000 l=1,9
         i = (l-1)/3 + 1
         j = l -3*(i-1)
         m(i,j) = signs(l)*m3(i,j)
 6000    continue
      endif
      if (m3det(m).lt.0) then
         do 7000 l=1,9
         i = (l-1)/3 + 1
         j = l -3*(i-1)
         m(i,j) = -m(i,j)
 7000    continue
      endif
      do 8000 l=1,9
        i = (l-1)/3 + 1
        j = l -3*(i-1)
        if (dabs(m(i,j)).lt.1.d-8) m(i,j) = 0.D0
 8000 continue

      end


C**********************************************************************C
      subroutine dg6toe3 (m6,m)
      implicit none
      real*8 m6(6,6), m6t(6,6)
      real*8 m3(3,3), m(3,3)
      real*8 mdif
      external mdif
      integer bits,bitmin
      integer signs(9)
      integer i,j,l
      real*8 t1, t1min
      real*8 m3det
C----------------------------------------------------------------------C

      do 1000 i=1,3
      do 1000 j=1,3
         m3(i,j) = sqrt(m6(i,j))
 1000 continue

      do 5000 bits = 511,0,-1
         call btos(9,bits,signs)
C         write (*,*) ' bits,signs ',bits,signs
         do 2000 l=1,9
            i = (l-1)/3 + 1
            j = l -3*(i-1)
            m(i,j) = signs(l)*m3(i,j)
 2000    continue
         call m3tom6 (m,m6t)
         t1 = mdif(6,m6,m6t)
         if (bits.eq.511) t1min = t1
         if (t1 .le. t1min) then
            t1min = t1
            bitmin = bits
         endif
         if (t1 .eq. 0.0) go to 5500
 5000 continue
 5500 continue
C      write (*,*) ' t1 ',t1,t1min,bitmin
      if (t1 .ne. 0.0) then
         call btos (9,bitmin,signs)
         do 6000 l=1,9
         i = (l-1)/3 + 1
         j = l -3*(i-1)
         m(i,j) = signs(l)*m3(i,j)
 6000    continue
      endif
      if (m3det(m).lt.0) then
         do 7000 l=1,9
         i = (l-1)/3 + 1
         j = l -3*(i-1)
         m(i,j) = -m(i,j)
 7000    continue
      endif
      do 8000 l=1,9
        i = (l-1)/3 + 1
        j = l -3*(i-1)
        if (dabs(m(i,j)).lt.1.D-8) m(i,j) = 0.D0
 8000 continue
      end



C**********************************************************************C
      SUBROUTINE VVTOG6 (M,N1,N2,V1,V2,N3,G)
      implicit none
      integer n1,n2,n3
      real*8 M, V1(3,3),V2(3,3), G(6,6)
C----------------------------------------------------------------------C
      g(n3,1) = m*v1(n1,1)*v2(n2,1)
      g(n3,2) = m*v1(n1,2)*v2(n2,2)
      g(n3,3) = m*v1(n1,3)*v2(n2,3)
      g(n3,4) = (m*v1(n1,2)*v2(n2,3) + m*v1(n1,3)*v2(n2,2)) / 2.0
      g(n3,5) = (m*v1(n1,1)*v2(n2,3) + m*v1(n1,3)*v2(n2,1)) / 2.0
      g(n3,6) = (m*v1(n1,1)*v2(n2,2) + m*v1(n1,2)*v2(n2,1)) / 2.0
      END


C**********************************************************************C
      SUBROUTINE M3TOM6 (M3,M6)
      implicit none
      real*8 M3(3,3), M6(6,6)
C----------------------------------------------------------------------C
      call vvtog6 (1.0D0,1,1,M3,M3,1,m6)
      call vvtog6 (1.0D0,2,2,M3,M3,2,m6)
      call vvtog6 (1.0D0,3,3,M3,M3,3,m6)
      call vvtog6 (2.0D0,2,3,M3,M3,4,m6)
      call vvtog6 (2.0D0,1,3,M3,M3,5,m6)
      call vvtog6 (2.0D0,1,2,M3,M3,6,m6)
      end


C**********************************************************************C
      SUBROUTINE MM(N,M1,M2,M3)
      implicit none
      integer i,j,k,n
      real*8 M1(N,N), M2(N,N), M3(N,N)
      real*8 temp
C----------------------------------------------------------------------C

      DO 3000 I=1,N
      DO 2000 J=1,N
         TEMP = 0.0
         DO 1000 K=1,N
            TEMP = TEMP + M1(K,I)*M2(J,K)
 1000    CONTINUE
         M3(I,J) = TEMP

 2000 CONTINUE
 3000 CONTINUE
      END

C**********************************************************************C
      real*8 FUNCTION MDIF (N,M1,M2)
      implicit none
      real*8 M1(*), M2(*)
      real*8 temp
      integer i, n
C----------------------------------------------------------------------C

      TEMP = 0
      DO 1000 I=1,N*N
 1000 TEMP = TEMP + (M1(I)-M2(I))**2
      MDIF = sqrt(TEMP)
      END

C**********************************************************************C
      SUBROUTINE CPYMAT (N,M1,M2)
      implicit none
      real*8 M1(*),M2(*)
      integer i, n
C----------------------------------------------------------------------C
      DO 1000 I=1,N*N
 1000 M2(I) = M1(I)
      END


