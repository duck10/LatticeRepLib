all:	CmdCmplx/CmdCmplx \
	CmdDelone/CmdDelone \
	CmdDists/CmdDists \
	cmdgen/CmdGen \
	CmdLM/CmdLM \
	CmdNiggli/CmdNiggli \
	CmdPath/CmdPath \
	CmdPerturb/CmdPerturb \
	CmdS6Refl/CmdS6Refl \
	CmdSella/CmdSella \
	cmdSort/cmdSort \
	CmdToB4/CmdToB4 \
	CmdToC3/CmdToC3 \
	CmdToCell/CmdToCell \
	CmdToD13/CmdToD13 \
	CmdToDC/CmdToDC \
	CmdToG6/CmdToG6 \
	CmdToS6/CmdToS6 \
	CmdToU/CmdToU \
	CmdVolume/CmdVolume

CmdCmplx/CmdCmplx: CmdCmplx/CmdCmplx.cpp
	(cd CmdCmplx; make all)

CmdDelone/CmdDelone: CmdDelone/CmdDelone.cpp
	(cd CmdDelone; make all)

CmdDists/CmdDists: CmdDists/CmdDists.cpp
	(cd CmdDists; make all)

cmdgen/CmdGen:  cmdgen/CmdGen.cpp
	(cd cmdgen; make all)

CmdLM/CmdLM: CmdLM/CmdLM.cpp
	(cd CmdLM; make all)

CmdNiggli/CmdNiggli: CmdNiggli/CmdNiggli.cpp
	(cd CmdNiggli; make all)

CmdPath/CmdPath: CmdPath/CmdPath.cpp
	(cd CmdPath; make all)

CmdPerturb/CmdPerturb: CmdPerturb/CmdPerturb.cpp
	(cd CmdPerturb; make all)

CmdS6Refl/CmdS6Refl: CmdS6Refl/CmdS6Refl.cpp
	(cd CmdS6Refl; make all)

CmdSella/CmdSella: CmdSella/CmdSella.cpp
	(cd CmdSella; make all)

cmdSort/cmdSort: cmdSort/cmdSort.cpp
	(cd cmdSort; make all)

CmdToB4/CmdToB4: CmdToB4/CmdToB4.cpp
	(cd CmdToB4; make all)

CmdToC3/CmdToC3: CmdToC3/CmdToC3.cpp
	(cd CmdToC3; make all)

CmdToCell/CmdToCell: CmdToCell/CmdToCell.cpp
	(cd CmdToCell; make all)

CmdToD13/CmdToD13: CmdToD13/CmdToD13.cpp
	(cd CmdToD13; make all)

CmdToDC/CmdToDC: CmdToDC/CmdToDC.cpp
	(cd CmdToDC; make all)

CmdToG6/CmdToG6: CmdToG6/CmdToG6.cpp
	(cd CmdToG6; make all)

CmdToS6/CmdToS6: CmdToS6/CmdToS6.cpp
	(cd CmdToS6; make all)

CmdToU/CmdToU: CmdToU/CmdToU.cpp
	(cd CmdToU; make all)

CmdVolume/CmdVolume: CmdVolume/CmdVolume.cpp
	(cd CmdVolume; make all)




clean:
	rm -rf CmdCmplx/CmdCmplx
	rm -rf CmdDelone/CmdDelone
	rm -rf CmdDists/CmdDists
	rm -rf cmdgen/CmdGen
	rm -rf CmdLM/CmdLM
	rm -rf CmdNiggli/CmdNiggli
	rm -rf CmdPath/CmdPath
	rm -rf CmdPerturb/CmdPerturb
	rm -rf CmdS6Refl/CmdS6Refl
	rm -rf CmdSella/CmdSella
	rm -rf cmdSort/cmdSort
	rm -rf CmdToB4/CmdToB4
	rm -rf CmdToC3/CmdToC3
	rm -rf CmdToCell/CmdToCell
	rm -rf CmdToD13/CmdToD13
	rm -rf CmdToDC/CmdToDC
	rm -rf CmdToG6/CmdToG6
	rm -rf CmdToS6/CmdToS6
	rm -rf CmdToU/CmdToU
	rm -rf CmdVolume/CmdVolume

