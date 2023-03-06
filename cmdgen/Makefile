CXX	?=	g++ -g -O3
COMMAND = CmdGen

all:	$(COMMAND)

$(COMMAND):	$(COMMAND).cpp \
        ../LatticeRepresentationLib/B4.cpp ../LatticeRepresentationLib/C3.cpp \
        ../LatticeRepresentationLib/C3Plot.cpp ../LatticeRepresentationLib/CellInputData.cpp \
	../LatticeRepresentationLib/DC7u.cpp \
	../LatticeRepresentationLib/D13.cpp \
        ../LatticeRepresentationLib/D7_Boundary.cpp ../LatticeRepresentationLib/D7_BoundaryList.cpp  \
        ../LatticeRepresentationLib/D7_ClassifySubboundaries.cpp ../LatticeRepresentationLib/D7.cpp \
        ../LatticeRepresentationLib/D7_Subboundary.cpp ../LatticeRepresentationLib/Delone.cpp \
        ../LatticeRepresentationLib/FileOperations.cpp ../LatticeRepresentationLib/G6.cpp \
	../LatticeRepresentationLib/GenerateLatticeTypeExamples.cpp \
	../LatticeRepresentationLib/LabeledSellaMatrices.cpp \
        ../LatticeRepresentationLib/LatticeConverter.cpp ../LatticeRepresentationLib/LRL_Cell.cpp  \
        ../LatticeRepresentationLib/LRL_Cell_Degrees.cpp ../LatticeRepresentationLib/LRL_CoordinateConversionMatrices.cpp \
        ../LatticeRepresentationLib/LRL_inverse.cpp ../LatticeRepresentationLib/LRL_MaximaTools.cpp \
        ../LatticeRepresentationLib/LRL_ReadLatticeData.cpp ../LatticeRepresentationLib/LRL_StringTools.cpp \
	../LatticeRepresentationLib/MatD13.cpp \
        ../LatticeRepresentationLib/MatD7.cpp ../LatticeRepresentationLib/MatG6.cpp ../LatticeRepresentationLib/MatMN.cpp \
        ../LatticeRepresentationLib/MatN.cpp ../LatticeRepresentationLib/MatS6.cpp ../LatticeRepresentationLib/Niggli.cpp \
        ../LatticeRepresentationLib/S6.cpp ../LatticeRepresentationLib/S6Dist.cpp \
        ../LatticeRepresentationLib/S6_Matrices.cpp ../LatticeRepresentationLib/Selling.cpp \
        ../LatticeRepresentationLib/VecN.cpp ../LatticeRepresentationLib/LRL_Vector3.cpp
	$(CXX) -I ../LatticeRepresentationLib -o $(COMMAND) $(COMMAND).cpp \
	../LatticeRepresentationLib/B4.cpp ../LatticeRepresentationLib/C3.cpp \
	../LatticeRepresentationLib/C3Plot.cpp ../LatticeRepresentationLib/CellInputData.cpp \
	../LatticeRepresentationLib/DC7u.cpp \
	../LatticeRepresentationLib/D13.cpp \
	../LatticeRepresentationLib/D7_Boundary.cpp ../LatticeRepresentationLib/D7_BoundaryList.cpp  \
	../LatticeRepresentationLib/D7_ClassifySubboundaries.cpp ../LatticeRepresentationLib/D7.cpp \
	../LatticeRepresentationLib/D7_Subboundary.cpp ../LatticeRepresentationLib/Delone.cpp \
	../LatticeRepresentationLib/FileOperations.cpp ../LatticeRepresentationLib/G6.cpp \
	../LatticeRepresentationLib/GenerateLatticeTypeExamples.cpp \
	../LatticeRepresentationLib/LabeledSellaMatrices.cpp \
	../LatticeRepresentationLib/LatticeConverter.cpp ../LatticeRepresentationLib/LRL_Cell.cpp  \
	../LatticeRepresentationLib/LRL_Cell_Degrees.cpp ../LatticeRepresentationLib/LRL_CoordinateConversionMatrices.cpp \
	../LatticeRepresentationLib/LRL_inverse.cpp ../LatticeRepresentationLib/LRL_MaximaTools.cpp \
	../LatticeRepresentationLib/LRL_ReadLatticeData.cpp ../LatticeRepresentationLib/LRL_StringTools.cpp \
	../LatticeRepresentationLib/MatD13.cpp \
	../LatticeRepresentationLib/MatD7.cpp ../LatticeRepresentationLib/MatG6.cpp ../LatticeRepresentationLib/MatMN.cpp \
	../LatticeRepresentationLib/MatN.cpp ../LatticeRepresentationLib/MatS6.cpp ../LatticeRepresentationLib/Niggli.cpp \
	../LatticeRepresentationLib/S6.cpp ../LatticeRepresentationLib/S6Dist.cpp \
	../LatticeRepresentationLib/S6_Matrices.cpp ../LatticeRepresentationLib/Selling.cpp \
	../LatticeRepresentationLib/VecN.cpp ../LatticeRepresentationLib/LRL_Vector3.cpp

clean:
	rm -rf $(COMMAND)
