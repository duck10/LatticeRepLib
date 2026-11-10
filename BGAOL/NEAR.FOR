C**********************************************************************C
      SUBROUTINE BLDTRE_N (debug,MXTREE,NVEC,X,IDIN,TREE,TEST)
      implicit none

      CHARACTER*8 TEST
      LOGICAL DEBUG
      integer nvec
      REAL*8 X(NVEC)
      integer mxtree, idin
      real*8 DL, DR
      real*8 TREE(MXTREE)
      real*8 TREELN_N
      integer ifree, ipoint, iright, ileft, nodsiz
      INTEGER LINK,RMAX,ID,ICHILD
      DATA LINK,RMAX,ID,ICHILD  /1,2,3,4/
C----------------------------------------------------------------------C
      IF (TEST .NE. 'BLDTRE_N' .AND. TEST .NE. 'BLDTRE_N')
     2     CALL BADCAL_N (TEST,'BLDTRE_N')
     
C      write(*,*) "BLDTRE_N"

C   THE NODE SIZE IS 4 PLUS THE SIZE OF THE VECTOR
      NODSIZ = 4+NVEC
      IPOINT = 2
      IF (TREE(1) .GT. 0D0) THEN
          IFREE = TREE(1)
      ELSE
         IFREE = 2
      ENDIF
      tree(ifree) = 0.0D0
 1000 CONTINUE
      IF (DEBUG) WRITE (*,*)
     2 ' AFTER 1000 IN BLDTRE_N, IPOINT,TREE(IPOINT) ',
     3   IPOINT,TREE(IPOINT)
      IF (DEBUG) WRITE (*,*) 'IFREE,TREE(1),TREE(2) ',
     2     IFREE,INT(TREE(1)),INT(TREE(2))

      IF (TREE(IPOINT) .EQ. 0D0) THEN
         IF (DEBUG) WRITE (*,*) ' A NEW NODE IS BEING ALLOCATED'
         IPOINT = IFREE
         TREE(IPOINT) = -1.0D0
         TREE(IPOINT+LINK) = -1.0D0
         TREE(IPOINT+ID) = IDIN
         CALL CPYVN (NVEC,X,TREE(IPOINT+ICHILD))
         TREE(1) = IFREE + NODSIZ
         RETURN
      ELSEIF (TREE(IPOINT) .EQ. -1D0) THEN
         IF (DEBUG) WRITE (*,*) ' RIGHT CHILD OF NODE IS BEING FILLED'
         TREE(IPOINT) = IFREE
         TREE(IFREE+LINK) = -1.0D0
         TREE (IFREE+RMAX) = -1.0D0
         TREE(IFREE+ID) = IDIN
         CALL CPYVN (NVEC,X,TREE(IFREE+ICHILD))
         IFREE = IFREE + NODSIZ
         TREE(1) = IFREE
         RETURN
      ELSE
         IRIGHT = TREE(IPOINT)
         DL = TREELN_N (NVEC,X,TREE(IPOINT+ICHILD))
         DR = TREELN_N (NVEC,X,TREE(IRIGHT+ICHILD))
         IF (DEBUG) WRITE (*,*) ' DL,DR ',DL,DR
         IF (DR .GT. DL) THEN
             IF (DEBUG) WRITE (*,*) DR,DL,IPOINT,LINK,RMAX
             IF (TREE(IPOINT+LINK) .LE. 1.0D-20) THEN
                TREE(IPOINT+RMAX) = DL
                TREE(IPOINT+LINK) = IFREE
                IPOINT = IFREE
             ELSE
                TREE(IPOINT+RMAX) =
     2                 MAX(DL,TREE(IPOINT+RMAX))
                IPOINT = TREE(IPOINT+LINK)
             ENDIF

         ELSE
             IF (TREE(IRIGHT+LINK) .LE. 1.0D-20) THEN
                 TREE(IRIGHT+RMAX) = DR
                 TREE(IRIGHT+LINK) = IFREE
                 IPOINT = IFREE
              ELSE
                 TREE(IRIGHT+RMAX) =
     2                  MAX(DR,TREE(IRIGHT+RMAX))
                 IPOINT = TREE(IRIGHT+LINK)
              ENDIF

         ENDIF
         GO TO 1000
      ENDIF
      END

C**********************************************************************C
      integer FUNCTION IUNSTK_N(NEXT,ISTAK,ISTKP)
      implicit none

      INTEGER ISTAK(1000)
      integer next, istkp
      LOGICAL DEBUG
      DATA DEBUG /.FALSE./
C----------------------------------------------------------------------C
      IF (DEBUG) WRITE (*,*) ' IUNSTK_N,NEXT ',NEXT
      IF (ISTKP .GT. 0) THEN
         NEXT = ISTAK(ISTKP)
         ISTKP = ISTKP-1
      ELSE
         NEXT = 0
      ENDIF
      IUNSTK_N = NEXT
      END

C**********************************************************************C
      integer FUNCTION NEARST_N 
     *    (debug,MXTREE,NVEC,X,RADMAX,TREE,IP,IDOUT,TEST)
      implicit none

      real*8 TREELN_N
      integer i, ncycle, numneg, nzero
      CHARACTER*8 TEST
      LOGICAL DEBUG
      integer maxstk
      integer nvec, IUNSTK_N
      parameter (maxstk=1000)
      INTEGER ISTAK(maxstk)
      integer istkp
      integer mxtree
      real*8 TREE(MXTREE)
      real*8 X(NVEC), DL, DR
      INTEGER LINK,RMAX,ID,ICHILD
      DATA LINK,RMAX,ID,ICHILD /1,2,3,4/
      integer RIGHT,LEFT,END
      integer DIR,DIRPRV, idout, ip, ipoint, iprev, iright
      real*8 curmin
      REAL*8 RADMAX
      DATA RIGHT,LEFT,END /111,112,113/
C----------------------------------------------------------------------C

      IF (TEST .NE. 'NEARST_N' .AND. TEST .NE. 'NEARST_N')
     2    CALL BADCAL_N (TEST,'NEARST_N')

      ISTKP = 0
      IP = 0
      IPOINT = 2
      CURMIN = RADMAX
      if (debug) write(*,*) "curmin on entry",curmin
      DIR = LEFT
      DIRPRV = RIGHT
      IPREV = IPOINT -1
      IDOUT = 0
      if (tree(1).le.0.0D0) go to 8000
 1000 CONTINUE
      IPREV = IPOINT
      DIRPRV = DIR
      IF (DEBUG) WRITE (*,*)  ' IN NEARST_N 1000, IPOINT = ',IPOINT
      IF (TREE(IPOINT) .EQ. 0D0) THEN
         IF (DEBUG) WRITE (*,*) ' AT AN END WITH IPOINT = ',IPOINT
         DIR = END
      ELSEIF (DIR .EQ. RIGHT) THEN
         IRIGHT = TREE(IPOINT)
         IF (DEBUG) WRITE (*,*) ' WENT RIGHT WITH IPOINT ', IPOINT
         DR = TREELN_N (NVEC,TREE(IRIGHT+ICHILD),X)
         IF (DEBUG) then 
            write(*,* ) "DR,curmin,delta", DR,curmin,DR-curmin
         endif
         IF (DR .LT. CURMIN) THEN
            CURMIN = DR
            if (debug) write(*,*) "DR,curmin", DR, curmin
            IP = IRIGHT + ICHILD
            IDOUT = TREE(IRIGHT+ID)
         ENDIF
         IF (TREE(IRIGHT+LINK) .LE. 1.0D-20) THEN
            IF (DEBUG) WRITE (*,*) ' ON RIGHT BRANCH, UNSTACK A POINT'
            DIR = END
         ELSEIF (TREE(IRIGHT+RMAX)+CURMIN .GT. DR) THEN
            IPOINT = TREE(IRIGHT+LINK)
            DIR = LEFT
         ELSE
             DIR = END
         ENDIF
      ELSE
         IF (DEBUG) WRITE (*,*) ' WENT LEFT, IPOINT ',IPOINT
         DIR = LEFT
         IF (TREE(IPOINT) .GT. 0D0) THEN
            IF (DEBUG) WRITE (*,*) ' STACK ONE '
            CALL TRSTCK_N(IPOINT,ISTAK,ISTKP)
         ENDIF
         DL = TREELN_N (NVEC,TREE(IPOINT+ICHILD),X)
         IF (DEBUG) then 
            write(*,* ) "DL,curmin,delta", DL,curmin,DL-curmin
         endif
         IF (DL .LT. CURMIN) THEN
            CURMIN = DL
            if (debug) write(*,*) "DL,curmin", DL, curmin
            IP = IPOINT+ICHILD
            IDOUT = TREE(IPOINT+ID)
         ENDIF
         IF (TREE(IPOINT+LINK) .LE. 1.0D-20) THEN
            IF (DEBUG) WRITE (*,*) ' NO LEFT LINK, GO BACK'
            DIR = END
         ELSEIF (TREE(IPOINT+RMAX) .LT. 0.0D0) THEN
            IF (DEBUG) WRITE (*,*) ' NO DESCENDING LEFT TREE'
            DIR = END
         ELSEIF (TREE(IPOINT+RMAX)+CURMIN .GT. DL) THEN
            IF (DEBUG) WRITE (*,*) ' GOING TO GO DOWN ONE LEVEL ',
     2         ' IPOINT AND UPDATE ',IPOINT,' ',INT(TREE(IPOINT+LINK))
            IPOINT = TREE(IPOINT+LINK)
         ELSE
           IF (DEBUG) WRITE (*,*) ' NO CLOSER POINTS ON LEFT '
           IF (DEBUG) WRITE (*,*) 'CURMIN,TREE(IPOINT+RMAX),DL ',
     2        CURMIN,TREE(IPOINT+RMAX),DL
           DIR = END
         ENDIF
      ENDIF
      IF (DIR .EQ. END) THEN
         if (debug) write (*,*) ' call unstk ',ipoint,istkp,maxstk
         IF (IUNSTK_N(IPOINT,ISTAK,ISTKP) .LE. 0) GO TO 8000
         DIR = RIGHT
      ENDIF
      GO TO 1000
 8000 CONTINUE
      NEARST_N = IP
      END
C**********************************************************************C
      real*8 FUNCTION TREELN_N (NVEC,A,B)
C-----GET THE SEPARATION BETWEEN THE ENDS OF TWO VECTORS
      implicit none

      integer nvec, i
      real*8 sum
      real*8 A(NVEC),B(NVEC)
C----------------------------------------------------------------------C
      SUM = 0.0
      DO 1000 I=1,NVEC
         SUM = SUM + (A(I)-B(I))**2
 1000 CONTINUE
      TREELN_N = dSQRT(SUM)
      END

C**********************************************************************C
      SUBROUTINE TRSTCK_N (NEXT,ISTAK,ISTKP)
      implicit none

      integer next, istkp
      INTEGER ISTAK(1000)
      LOGICAL DEBUG
      DATA DEBUG /.FALSE./
C----------------------------------------------------------------------C
      IF (DEBUG) WRITE (*,*) ' IN TRSTCK_N, STACK POINTER,IPOINT ',
     2    ISTKP,NEXT
      ISTKP = ISTKP + 1
      ISTAK(ISTKP) = NEXT
      END
      
      
C**********************************************************************C
      SUBROUTINE BADCAL_N (A,B)
      implicit none

      CHARACTER *8 A,B
C----------------------------------------------------------------------C
      WRITE (*,*)  ' BAD SUBROUTINE CALL TO ',B
      WRITE (*,*)  ' CALLING NAME =',A
      STOP
      END
