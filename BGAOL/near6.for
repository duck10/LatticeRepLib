C**********************************************************************C
      SUBROUTINE BADCAL (A,B)
      CHARACTER *6 A,B
C----------------------------------------------------------------------C
      WRITE (*,*)  ' BAD SUBROUTINE CALL TO ',B
      WRITE (*,*)  ' CALLING NAME =',A
      STOP
      END

C**********************************************************************C
      SUBROUTINE BLDTRE (MXTREE,X,IDIN,TREE,TEST)
      implicit none
      integer nvec, mxtree, idin, iright
      integer ifree, ipoint,link, nodsiz, id, ichild
      real*8 DL, DR
      PARAMETER (NVEC=6)
      CHARACTER*6 TEST
      LOGICAL DEBUG
      real*8 TREE(MXTREE), X(6)
      real*8 trelen
      INTEGER RMAX
      DATA LINK,RMAX,ID,ICHILD  /1,2,3,4/
      DATA DEBUG /.FALSE./
C----------------------------------------------------------------------C

      IF (TEST .NE. 'BLDTRE' .AND. TEST .NE. 'bldtre')
     2     CALL BADCAL (TEST,'BLDTRE')

C   THE NODE SIZE IS 4 PLUS THE SIZE OF THE VECTOR
      NODSIZ = 4+NVEC
      IPOINT = 2
      IF (TREE(1) .GT. 0) THEN
          IFREE = INT(TREE(1))
      ELSE
         IFREE = 2
      ENDIF
 1000 CONTINUE
      IF (DEBUG) WRITE (*,*)
     2 ' AFTER 1000 IN BLDTRE, IPOINT,TREE(IPOINT) ',
     3   IPOINT,TREE(IPOINT)
      IF (DEBUG) WRITE (*,*) 'IFREE,TREE(1),TREE(2) ',
     2     IFREE,INT(TREE(1)),INT(TREE(2))

      IF (TREE(IPOINT) .EQ. 0) THEN
         IF (DEBUG) WRITE (*,*) ' A NEW NODE IS BEING ALLOCATED'
         IPOINT = IFREE
         TREE(IPOINT) = -1
         TREE(IPOINT+LINK) = -1
         TREE(IPOINT+ID) = IDIN
         CALL CPYVN (6,X,TREE(IPOINT+ICHILD))
         TREE(1) = IFREE + NODSIZ
         RETURN
      ELSEIF (TREE(IPOINT) .EQ. -1) THEN
         IF (DEBUG) WRITE (*,*) ' RIGHT CHILD OF NODE IS BEING FILLED'
         TREE(IPOINT) = IFREE
         TREE(IFREE+LINK) = -1
         TREE (IFREE+RMAX) = -1.0
         TREE(IFREE+ID) = IDIN
         CALL CPYVN (6,X,TREE(IFREE+ICHILD))
         IFREE = IFREE + NODSIZ
         TREE(1) = IFREE
         RETURN
      ELSE
         IRIGHT = INT(TREE(IPOINT))
         DL = TRELEN (6,X,TREE(IPOINT+ICHILD))
         DR = TRELEN (6,X,TREE(IRIGHT+ICHILD))
         IF (DEBUG) WRITE (*,*) ' DL,DR ',DL,DR
         IF (DR .GT. DL) THEN
             IF (DEBUG) WRITE (*,*) DR,DL,IPOINT,LINK,RMAX
             IF (TREE(IPOINT+LINK) .LE. 0) THEN
                TREE(IPOINT+RMAX) = DL
                TREE(IPOINT+LINK) = IFREE
                IPOINT = IFREE
             ELSE
                TREE(IPOINT+RMAX) =
     2                 MAX(DL,TREE(IPOINT+RMAX))
                IPOINT = INT(TREE(IPOINT+LINK))
             ENDIF

         ELSE
             IF (TREE(IRIGHT+LINK) .LE. 0) THEN
                 TREE(IRIGHT+RMAX) = DR
                 TREE(IRIGHT+LINK) = IFREE
                 IPOINT = IFREE
              ELSE
                 TREE(IRIGHT+RMAX) =
     2                  MAX(DR,TREE(IRIGHT+RMAX))
                 IPOINT = INT(TREE(IRIGHT+LINK))
              ENDIF

         ENDIF
         GO TO 1000
      ENDIF
      END

C**********************************************************************C
      FUNCTION NEARST (MXTREE,X,RADMAX,TREE,IP,IDOUT,TEST)
      implicit none
      integer NEARST, IUNSTK
      integer mxtree
      real*8 RADMAX
      
      CHARACTER*6 TEST
      LOGICAL DEBUG
      INTEGER ISTAK(1000)
      integer ip, idout, dir, end, id
      integer ichild, ipoint, iright, istkp, left, link, right
      real*8 TREE(MXTREE),X(6)
      real*8 DL, DR, CURMIN
      real*8 trelen
      INTEGER RMAX
      DATA DEBUG /.FALSE./
      DATA LINK,RMAX,ID,ICHILD /1,2,3,4/
      DATA RIGHT,LEFT,END /111,112,113/
C----------------------------------------------------------------------C

      IF (TEST .NE. 'NEARST' .AND. TEST .NE. 'nearst')
     2    CALL BADCAL (TEST,'NEARST')

      ISTKP = 0
      IP = 0
      IPOINT = 2
      CURMIN = RADMAX
      DIR = LEFT
 1000 CONTINUE
      IF (DEBUG) WRITE (*,*)  ' IN NEARST 1000, IPOINT = ',IPOINT
      IF (TREE(IPOINT) .EQ. 0) THEN
         IF (DEBUG) WRITE (*,*) ' AT AN END WITH IPOINT = ',IPOINT
         DIR = END
      ELSEIF (DIR .EQ. RIGHT) THEN
         IRIGHT = INT(TREE(IPOINT))
         IF (DEBUG) WRITE (*,*) ' WENT RIGHT WITH IPOINT ', IPOINT
         DR = TRELEN (6,TREE(IRIGHT+ICHILD),X)
         IF (DR .LT. CURMIN) THEN
            CURMIN = DR
            IP = IRIGHT + ICHILD
            IDOUT = INT(TREE(IRIGHT+ID))
         ENDIF
         IF (TREE(IRIGHT+LINK) .LE. 0) THEN
            IF (DEBUG) WRITE (*,*) ' ON RIGHT BRANCH, UNSTACK A POINT'
            DIR = END
         ELSEIF (TREE(IRIGHT+RMAX)+CURMIN .GT. DR) THEN
            IPOINT = INT(TREE(IRIGHT+LINK))
            DIR = LEFT
         ELSE
             DIR = END
         ENDIF
      ELSE
         IF (DEBUG) WRITE (*,*) ' WENT LEFT, IPOINT ',IPOINT
         DIR = LEFT
         IF (TREE(IPOINT) .gt. 0) THEN
            IF (DEBUG) WRITE (*,*) ' STACK ONE '
            CALL STACK(IPOINT,ISTAK,ISTKP)
         ENDIF
         DL = TRELEN (6,TREE(IPOINT+ICHILD),X)
         IF (DL .LT. CURMIN) THEN
            CURMIN = DL
            IP = IPOINT+ICHILD
            IDOUT = INT(TREE(IPOINT+ID))
         ENDIF
         IF (TREE(IPOINT+LINK) .LE. 0) THEN
            IF (DEBUG) WRITE (*,*) ' NO LEFT LINK, GO BACK'
            DIR = END
         ELSEIF (TREE(IPOINT+RMAX) .LT. 0.0) THEN
            IF (DEBUG) WRITE (*,*) ' NO DESCENDING LEFT TREE'
            DIR = END
         ELSEIF (TREE(IPOINT+RMAX)+CURMIN .GT. DL) THEN
            IF (DEBUG) WRITE (*,*) ' GOING TO GO DOWN ONE LEVEL ',
     2         ' IPOINT AND UPDATE ',IPOINT,' ',INT(TREE(IPOINT+LINK))
            IPOINT = INT(TREE(IPOINT+LINK))
         ELSE
           IF (DEBUG) WRITE (*,*) ' NO CLOSER POINTS ON LEFT '
           IF (DEBUG) WRITE (*,*) 'CURMIN,TREE(IPOINT+RMAX),DL ',
     2        CURMIN,TREE(IPOINT+RMAX),DL
           DIR = END
         ENDIF
      ENDIF
      IF (DIR .EQ. END) THEN
         IF (IUNSTK(IPOINT,ISTAK,ISTKP) .LE. 0) GO TO 8000
         DIR = RIGHT
      ENDIF
      GO TO 1000
 8000 CONTINUE
      NEARST = IP
      END

C**********************************************************************C
      SUBROUTINE STACK(NEXT,ISTAK,ISTKP)
      implicit none
      integer istkp, next
      INTEGER ISTAK(1000)
      LOGICAL DEBUG
      DATA DEBUG /.FALSE./
C----------------------------------------------------------------------C
      IF (DEBUG) WRITE (*,*) ' IN STACK, STACK POINTER,IPOINT ',
     2    ISTKP,NEXT
      ISTKP = ISTKP + 1
      ISTAK(ISTKP) = NEXT
      END

C**********************************************************************C
      integer FUNCTION IUNSTK(NEXT,ISTAK,ISTKP)
      implicit none
      integer istkp, next
      INTEGER ISTAK(1000)
      LOGICAL DEBUG
      DATA DEBUG /.FALSE./
C----------------------------------------------------------------------C
      IF (DEBUG) WRITE (*,*) ' IUNSTK,NEXT ',NEXT
      IF (ISTKP .GT. 0) THEN
         NEXT = ISTAK(ISTKP)
         ISTKP = ISTKP-1
      ELSE
         NEXT = 0
      ENDIF
      IUNSTK = NEXT
      END

C**********************************************************************C
      INTEGER FUNCTION INSPHR (MXTREE,X,RADMAX,TREE,
     2    MXLIST,NLIST,LIST,IDLIST,TEST)
      implicit none
      CHARACTER*6 TEST
      LOGICAL DEBUG
      INTEGER ISTAK(1000)
      integer mxtree
      real*8 radmax
      real*8 x(6)
      real*8 trelen
      integer iunstk
      integer mxlist, nlist,dir, iprev, dirprv
      integer ipoint, iright, istkp, link
      real*8 curmin, DL, DR
      real*8 TREE(MXTREE)
      integer right, left, end, ichild, id
      INTEGER LIST(MXLIST),IDLIST(MXLIST)
      INTEGER RMAX
      DATA DEBUG /.FALSE./
      DATA LINK,RMAX,ID,ICHILD /1,2,3,4/
      DATA RIGHT,LEFT,END /111,112,113/
C----------------------------------------------------------------------C

      IF (TEST .NE. 'INSPHR' .AND. TEST .NE. 'insphr')
     2    CALL BADCAL (TEST,'INSPHR')

      ISTKP = 0
      NLIST = 0
      IPOINT = 2
      CURMIN = RADMAX
      DIR = LEFT
      DIRPRV = RIGHT
      IPREV = IPOINT -1
 1000 CONTINUE
      IF (IPREV .EQ. IPOINT .AND. DIRPRV .EQ. DIR) THEN
         WRITE (*,*) ' INTERNAL ERROR IN INSPHR '
         WRITE (*,*) ' TREE POINTER DIDN''T CHANGE',IPOINT,' ',DIR
         STOP
      ELSEIF (IPOINT .EQ. 0) THEN
         WRITE (*,*) ' INTERNAL ERROR IN INSPHR, IPOINT = 0'
         STOP
      ENDIF
      IPREV = IPOINT
      DIRPRV = DIR
      IF (DEBUG) WRITE (*,*)  ' IN INSPHR 1000, IPOINT = ',IPOINT
      IF (TREE(IPOINT) .EQ. 0) THEN
         IF (DEBUG) WRITE (*,*) ' AT AN END WITH IPOINT = ',IPOINT
         DIR = END
      ELSEIF (DIR .EQ. RIGHT) THEN
         IRIGHT = INT(TREE(IPOINT))
         IF (DEBUG) WRITE (*,*) ' WENT RIGHT WITH IPOINT ', IPOINT
         DR = TRELEN (6,TREE(IRIGHT+ICHILD),X)
         IF (DR .LT. CURMIN) THEN
            NLIST = NLIST + 1
            IF (NLIST .GT. MXLIST) GO TO 8000
            LIST(NLIST) = IRIGHT + ICHILD
            IDLIST(NLIST) = INT(TREE(IRIGHT+ID))
         ENDIF
         IF (TREE(IRIGHT+LINK) .LE. 0) THEN
            IF (DEBUG) WRITE (*,*) ' ON RIGHT BRANCH, UNSTACK A POINT'
            DIR = END
         ELSEIF (TREE(IRIGHT+RMAX)+CURMIN .GT.
     2          TRELEN (6,TREE(IRIGHT+ICHILD),X) ) THEN
            IPOINT = INT(TREE(IRIGHT+LINK))
            DIR = LEFT
         ELSE
             DIR = END
         ENDIF
      ELSE
         IF (DEBUG) WRITE (*,*) ' WENT LEFT, IPOINT ',IPOINT
         DIR = LEFT
         IF (TREE(IPOINT) .gt. 0) THEN
            IF (DEBUG) WRITE (*,*) ' STACK ONE '
            CALL STACK(IPOINT,ISTAK,ISTKP)
         ENDIF
         DL = TRELEN (6,TREE(IPOINT+ICHILD),X)
         IF (DL .LT. CURMIN) THEN
            NLIST = NLIST + 1
            IF (NLIST .GT. MXLIST) GO TO 8000
            LIST(NLIST) = IPOINT+ICHILD
            IDLIST(NLIST) = INT(TREE(IPOINT+ID))
         ENDIF
         IF (TREE(IPOINT+LINK) .LE. 0) THEN
            IF (DEBUG) WRITE (*,*) ' NO LEFT LINK, GO BACK'
            DIR = END
         ELSEIF (TREE(IPOINT+RMAX) .LT. 0.0) THEN
            IF (DEBUG) WRITE (*,*) ' NO DESCENDING LEFT TREE'
            DIR = END
         ELSEIF (TREE(IPOINT+RMAX)+CURMIN .GT. DL) THEN
            IF (DEBUG) WRITE (*,*) ' GOING TO GO DOWN ONE LEVEL ',
     2         ' IPOINT AND UPDATE ',IPOINT,' ',INT(TREE(IPOINT+LINK))
            IPOINT = INT(TREE(IPOINT+LINK))
         ELSE
           IF (DEBUG) WRITE (*,*) ' NO CLOSER POINTS ON LEFT '
           IF (DEBUG) WRITE (*,*) 'CURMIN,TREE(IPOINT+RMAX),DL ',
     2        CURMIN,TREE(IPOINT+RMAX),DL
           DIR = END
         ENDIF
      ENDIF
      IF (DIR .EQ. END) THEN
         IF (IUNSTK(IPOINT,ISTAK,ISTKP) .LE. 0) GO TO 8000
         DIR = RIGHT
      ENDIF
      GO TO 1000
 8000 CONTINUE
      IF (NLIST .LE. MXLIST) THEN
         INSPHR = NLIST
      ELSE
         INSPHR = -NLIST
         NLIST = MXLIST
      ENDIF
      END

C**********************************************************************C
      REAL*8 FUNCTION TRELEN (N,A,B)
      implicit none
      integer n
      real*8 A(N),B(N), SUM
      integer i
C----------------------------------------------------------------------C
      SUM = 0
      DO 1000 I=1,6
 1000 SUM = SUM + (A(I)-B(I))**2
      TRELEN = SQRT(SUM)
      END
