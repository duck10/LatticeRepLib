
COMMANDS  =  \
	CheckInput \
	CmdCmplx \
	CmdDelone \
	CmdDists \
	CmdLM \
	CmdNiggli \
	CmdPath \
	CmdPerturb \
	CmdS6Refl \
	CmdSella  \
	CmdToB4 \
	CmdToC3 \
	CmdToCell \
	CmdToD13 \
	CmdToDC \
	CmdToG6 \
	CmdToS6 \
	CmdToU \
	CmdVolume

BIN	= $(PWD)/bin

all:	$(COMMANDS)

$(BIN):
	mkdir -p $(BIN)

$(COMMANDS):  $(BIN)/$@
	(cd $@; make all; mv $@ $(BIN))
