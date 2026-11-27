#
#  Makefile for BGAOL
#
#  Herbert J. Bernstein, Bernstein + Sons
#  Lawrence C. Andrews, Micro Encoder Inc.
#
#  1 July 2012
#  Rev 5 November 2012
#
#  Modify the following definitions for your system
#
#  HTTPDSERVER is the name of the server on which the
#  installation is being made
#
#  *************************************************
#  *** YOU MUST CHANGE THIS DEFINITION TO PERMIT ***
#  ***              REMOTE ACCESS                ***
#  *************************************************
#
HTTPDSERVER	?=	HOST.DOMAIN/~$(USER)
#
#  SEARCHURL IS THE URL FOR NEW SEARCH
#
SEARCHURL	?=	http://$(HTTPDSERVER)/bgaol#search
#
#  BINDEST is the installation directory for the executable
#  of bgaol
BINDEST		?=	$(HOME)/bin
#
#  CGIBIN is the installation directory for the cgi-bin script
#  iterate.csh
CGIBIN		?=	$(HOME)/public_html/cgi-bin
#
#  CGIBINEXT is the external name of the directory for the
#  cgi-bin script bgaol.csh
CGIBINEXT	?=	/cgi-bin
#
#  CGIMETHOD is either GET or POST
#
CGIMETHOD	?=	POST
#
#  HTDOCS is the installation directory for the HTML document
#  bgaol.html
HTDOCS		?=	$(HOME)/public_html/bgaol
#
#  HTDOCSEXT is the external name of the directory for the 
#  HTML document bgaol.html
HTDOCSEXT   ?=   /bgaol
#
#
#  Default compile flag definition to select debug mode under unix
#FFLAGS	=	-g
FFLAGS	?=	-O3
FC	?=	flang
#
#  For IBM AIX xlf compilation with full optimization try this
#FFLAGS	=	-O3 -qstrict
#FC	=	xlf
#
#
#
#
#  You should not have to edit below this line
#********************************************************************
#
#
# mathscribe is used by the web page to set formulae
#
MATHSCRIBEVERSION ?= 0.2.0
MATHSCRIBEPATH ?= mathscribe-$(MATHSCRIBEVERSION)
MATHSCRIBETARBALL ?= $(MATHSCRIBEPATH).tar.gz
MATHSCRIBETARBALLURL ?= http://downloads.sf.net/iterate/$(MATHSCRIBETARBALL)
MATHSCRIBEURL = http://$(HTTPDSERVER)$(HTDOCSEXT)/$(MATHSCRIBEPATH)

#
# BGAOL URLS
#
BGAOLVERSION = 1.1
BGAOLTARBALLURL = http://downloads.sf.net/iterate/bgaol-$(BGAOLVERSION).tar.gz
BGAOLZIPURL = http://downloads.sf.net/iterate/bgaol-$(BGAOLVERSION).zip


CGIPATH		?=	http://$(HTTPDSERVER)$(CGIBINEXT)
BINPATH		?=	$(BINDEST)/bgaol
HTFLAGS 	=	-DCGIBIN=$(CGIPATH) \
		-DHTTPDSERVER=$(HTTPDSERVER) \
		-DMATHSCRIBEURL=$(MATHSCRIBEURL) \
		-DBGAOLTARBALLURL=$(BGAOLTARBALLURL) \
		-DBGAOLZIPURL=$(BGAOLZIPURL) \
		-DCGIMETHOD=$(CGIMETHOD) \
		-DHTDOCS=$(HTDOCS)
		


#
#
edit:	
		@/bin/echo "**************************************"
		@/bin/echo "* You must edit Makefile before      *"
		@/bin/echo "* installing BGAOL                   *"
		@/bin/echo "**************************************"
		@/bin/echo "You must define the following in Makefile"
		@/bin/echo "or as environment variables:"
		@/bin/echo ""
		@/bin/echo "HTTPDSERVER ---- currently:" $(HTTPDSERVER)
		@/bin/echo "SEARCHURL ------ currently:" $(SEARCHURL)
		@/bin/echo "BINDEST -------- currently:" $(BINDEST)
		@/bin/echo "HTDOCS  -------- currently:" $(HTDOCS)
		@/bin/echo "CGIPATH  ------- currently:" $(CGIPATH)
		@/bin/echo "CGIMETHOD ------ currently:" $(CGIMETHOD)
		@/bin/echo "FC  ------------ currently:" $(FC)
		@/bin/echo "FFLAGS  -------- currently:" $(FFLAGS)
		@/bin/echo ""
		@/bin/echo "**************************************"
		@/bin/echo "* Then:                              *"
		@/bin/echo "*     make edit_done                 *"
		@/bin/echo "*     make install                   *"
		@/bin/echo "**************************************"
#
edit_done:	bgaol bgaol.html bgaol.csh
		touch edit
#
clean:
		-@rm -f edit
		-@rm -f bgaol.html
		-@rm -f bgaol
		-@rm -f *.o
		-@rm -f bgaol.csh
		-@rm -f *.bak
		-@rm -f follower/*,o
		-@rm -f follower/Follower
		-@rm -f follower/F_LargeNCDIST.txt
		-@rm -f follower/F_Summary.diff
		-@rm -f follower/F_Summary.txt
		-@rm -f follower/F_Step.txt
		-@rm -f follower/F_Step_\&_NCDIST.txt
		-@rm -f follower/Follower.diff
		-@rm -f follower/Follower.out
		-@rm -f follower/randTest
		-@rm -f follower/randTest.diff
		-@rm -f follower/randTest.out
		-@rm -f follower/fort.*
		-@rm -f follower/RHRAND.o
		-@rm -f testcases/BGAOL_tests*_cur
		-@rm -f testcases/bgaol_tests.diff
#
bgaol.html:	bgaol.html.m4 Makefile $(MATHSCRIBEPATH)
		m4 -d $(HTFLAGS) < bgaol.html.m4 > bgaol.html
#
bgaol.csh:	bgaol.csh.m4 Makefile
		m4 -DSEARCHURL=$(SEARCHURL) \
		-DBINPATH=$(BINPATH) \
		-DSEARCHURL=$(SEARCHURL)\
		-DCGIMETHOD=$(CGIMETHOD)\
		-DHTDOCS=$(HTDOCS)\
		< bgaol.csh.m4 > bgaol.csh
#
install:	edit_done bgaol bgaol.csh follower/Follower bgaol.html \
		$(MATHSCRIBEPATH)
		-mkdir -p $(BINDEST)
		-mkdir -p $(CGIBIN)
		-mkdir -p $(HTDOCS)
		chmod 755 bgaol
		chmod 755 bgaol.csh
		cp bgaol $(BINDEST)
		chmod 755 follower/Follower
		cp follower/Follower $(BINDEST)
		cp bgaol.csh $(CGIBIN)
		chmod 755 $(CGIBIN)/bgaol.csh
		cp bgaol.html $(HTDOCS)
		ln -f -s $(HTDOCS)/bgaol.html $(HTDOCS)/index.html
		cp -r $(MATHSCRIBEPATH) $(HTDOCS)/$(MATHSCRIBEPATH)

#		
bgaol.o:	bgaol.f MKGAOL.FOR MKREFL.FOR E3TOG6.FOR near6.for NEAR.FOR
bgaol:		bgaol.o 
		$(FC) $(FFLAGS) -o bgaol bgaol.o
	
bgaol.f:    BGAOL.FOR
		ln -s -f BGAOL.FOR bgaol.f

follower/RHRAND.o:	follower/RHRAND.for
		(cd follower; $(FC) -ffloat-store $(FFLAGS) -c RHRAND.for)

follower/Follower:	follower/Follower.for follower/RHRAND.o follower/MKREFL.FOR NEAR.FOR E3TOG6.FOR MKGAOL.FOR
		(cd follower; $(FC) $(FFLAGS) -o Follower Follower.for RHRAND.o)

follower/randTest:	follower/randTest.for follower/RHRAND.o
		(cd follower; $(FC) $(FFLAGS) -o randTest randTest.for RHRAND.o)

follower/randTest.diff:	follower/randTest follower/randTest_orig.out
		(cd follower; ./randTest > randTest.out; diff -bu randTest.out randTest_orig.out > randTest.diff)

follower/F_Summary.diff: follower/Follower follower/F_Summary_orig.txt
		@/bin/echo "Follower run started, please be patient"
		-(cd follower; ./Follower >Follower.out; diff -bu F_Summary.txt F_Summary_orig.txt > F_Summary.diff)

testcases/bgaol_tests.diff: bgaol
		@/bin/echo "testall.csh started, please be patient"
		(cd testcases; /bin/csh ../testall.csh)


tests:		follower/randTest.diff follower/F_Summary.diff follower/F_Step_orig.txt follower/Follower_orig.out \
		testcases/BGAOL-cF.in \
		testcases/bgaol_tests_orig \
		testcases/case01.in \
		testcases/case02.in \
		testcases/case03.in \
		testcases/case04.in \
		testcases/case05.in \
		testcases/case06.in \
		testcases/case07.in \
		testcases/case08.in \
		testcases/case09.in \
		testcases/case10.in \
		testcases/case11.in \
		testcases/case12.in \
		testcases/case13.in \
		testcases/case14.in \
		testcases/case15.in \
		testcases/case16.in \
		testcases/case17.in \
		testcases/case18.in \
		testcases/case19.in \
		testcases/case20.in \
		testcases/case21.in \
		testcases/case22.in \
		testcases/case23.in \
		testcases/case24.in \
		testcases/case25.in \
		testcases/case26.in \
		testcases/case27.in \
		testcases/case28.in \
		testcases/case29.in \
		testcases/case30.in \
		testcases/case32.in \
		testcases/case33.in \
		testcases/case34.in \
		testcases/case35.in \
		testcases/case36.in \
		testcases/case37.in \
		testcases/case38.in \
		testcases/case39.in \
		testcases/case40.in \
		testcases/case41.in \
		testcases/case42.in \
		testcases/case43.in \
		testcases/bgaol_tests.diff
		(cd follower; cat randTest.diff)
		(cd follower; cat F_Summary.diff; diff -bu F_Step.txt F_Step_orig.txt | head -50)
		-(cd follower; diff -bu Follower.out Follower_orig.out | head -50 | tail +9)

$(MATHSCRIBETARBALL):
		wget http://downloads.sf.net/iterate/$(MATHSCRIBETARBALL)

$(MATHSCRIBEPATH): $(MATHSCRIBETARBALL)
		gunzip < $(MATHSCRIBETARBALL) | tar xvf -
		chmod -R 755 $(MATHSCRIBEPATH)
		touch $(MATHSCRIBEPATH)

