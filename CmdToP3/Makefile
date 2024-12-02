CXX	?=	g++-9 -g -O3 -std=gnu++17 -fpermissive
COMMAND = CmdToP3

all:	$(COMMAND)


libPolar.a:	../LatticeRepresentationLib/Polar.cpp ../LatticeRepresentationLib/Polar.h
	$(CXX) -I ../LatticeRepresentationLib -I ../LRL_Tools -c ../LatticeRepresentationLib/Polar.cpp -o Polar.o
	ar rcs libPolar.a Polar.o

libP3.a:	../LatticeRepresentationLib/P3.cpp ../LatticeRepresentationLib/P3.h
	$(CXX) -I ../LatticeRepresentationLib -I ../LRL_Tools -c ../LatticeRepresentationLib/P3.cpp -o P3.o
	ar rcs libP3.a P3.o

$(COMMAND):	$(COMMAND).cpp \
	../LatticeRepresentationLib/B4.cpp \
	../LatticeRepresentationLib/C3.h ../LatticeRepresentationLib/C3.cpp \
	../LatticeRepresentationLib/CellInputData.cpp \
	../LatticeRepresentationLib/ColorTables.h ../LatticeRepresentationLib/ColorTables.cpp \
	../LatticeRepresentationLib/DC7u.cpp \
	../LatticeRepresentationLib/D13.cpp \
	../LatticeRepresentationLib/D7_Boundary.cpp ../LatticeRepresentationLib/D7_BoundaryList.cpp  \
	../LatticeRepresentationLib/D7_ClassifySubboundaries.cpp ../LatticeRepresentationLib/D7.cpp \
	../LatticeRepresentationLib/D7_Subboundary.cpp ../LatticeRepresentationLib/Delone.cpp \
	../LatticeRepresentationLib/DeloneFitResults.cpp \
	../LatticeRepresentationLib/EnhancedS6.cpp ../LatticeRepresentationLib/EnhancedS6.h \
	../LRL_Tools/FileOperations.cpp ../LatticeRepresentationLib/G6.cpp \
	../LRL_Tools/HSV.cpp ../LRL_Tools/HSV.h ../LRL_Tools/RGB.cpp ../LRL_Tools/RGB.h \
	../LatticeRepresentationLib/GenerateLatticeTypeExamples.cpp \
	../LatticeRepresentationLib/LabeledSellaMatrices.cpp \
	../LatticeRepresentationLib/LatticeConverter.cpp ../LatticeRepresentationLib/LRL_Cell.cpp  \
	../LatticeRepresentationLib/LRL_Cell_Degrees.cpp ../LatticeRepresentationLib/LRL_CoordinateConversionMatrices.cpp \
	../LRL_Tools/LRL_inverse.cpp ../LRL_Tools/LRL_MaximaTools.cpp \
	../LatticeRepresentationLib/LRL_ReadLatticeData.cpp ../LRL_Tools/LRL_StringTools.cpp \
	../LatticeRepresentationLib/MatD13.cpp \
	../LatticeRepresentationLib/MatD7.cpp ../LatticeRepresentationLib/MatG6.cpp ../LatticeRepresentationLib/MatMN.cpp \
	../LatticeRepresentationLib/MatN.cpp ../LatticeRepresentationLib/MatS6.cpp ../LatticeRepresentationLib/Niggli.cpp \
	../LatticeRepresentationLib/Polar.cpp ../LatticeRepresentationLib/Polar.h \
	../LatticeRepresentationLib/S6.cpp ../LatticeRepresentationLib/S6Dist.cpp \
	../LatticeRepresentationLib/S6_Matrices.cpp ../LatticeRepresentationLib/Selling.cpp \
	../LatticeRepresentationLib/S6BoundaryTransforms.h ../LatticeRepresentationLib/S6BoundaryTransforms.cpp \
	../LatticeRepresentationLib/Sella.cpp \
	../LatticeRepresentationLib/VecN.cpp  ../LatticeRepresentationLib/V7.cpp \
	../LatticeRepresentationLib/Vector_2.cpp ../LatticeRepresentationLib/Vector_2.h \
	../LRL_Tools/LRL_Vector3.cpp \
	libP3.a libPolar.a 
	$(CXX) -I ../LatticeRepresentationLib -I ../LRL_Tools -o $(COMMAND) $(COMMAND).cpp \
	../LatticeRepresentationLib/B4.cpp \
	../LatticeRepresentationLib/C3.cpp \
	../LatticeRepresentationLib/CellInputData.cpp \
	../LatticeRepresentationLib/ColorTables.cpp \
	../LatticeRepresentationLib/DC7u.cpp \
	../LatticeRepresentationLib/D13.cpp \
	../LatticeRepresentationLib/D7_Boundary.cpp ../LatticeRepresentationLib/D7_BoundaryList.cpp  \
	../LatticeRepresentationLib/D7_ClassifySubboundaries.cpp ../LatticeRepresentationLib/D7.cpp \
	../LatticeRepresentationLib/D7_Subboundary.cpp ../LatticeRepresentationLib/Delone.cpp \
	../LatticeRepresentationLib/DeloneFitResults.cpp \
	../LatticeRepresentationLib/EnhancedS6.cpp \
	../LRL_Tools/FileOperations.cpp ../LatticeRepresentationLib/G6.cpp \
	../LRL_Tools/HSV.cpp ../LRL_Tools/RGB.cpp \
	../LatticeRepresentationLib/GenerateLatticeTypeExamples.cpp \
	../LatticeRepresentationLib/LabeledSellaMatrices.cpp \
	../LatticeRepresentationLib/LatticeConverter.cpp ../LatticeRepresentationLib/LRL_Cell.cpp  \
	../LatticeRepresentationLib/LRL_Cell_Degrees.cpp ../LatticeRepresentationLib/LRL_CoordinateConversionMatrices.cpp \
	../LRL_Tools/LRL_inverse.cpp ../LRL_Tools/LRL_MaximaTools.cpp \
	../LatticeRepresentationLib/LRL_ReadLatticeData.cpp ../LRL_Tools/LRL_StringTools.cpp \
	../LatticeRepresentationLib/MatD13.cpp \
	../LatticeRepresentationLib/MatD7.cpp ../LatticeRepresentationLib/MatG6.cpp ../LatticeRepresentationLib/MatMN.cpp \
	../LatticeRepresentationLib/MatN.cpp ../LatticeRepresentationLib/MatS6.cpp ../LatticeRepresentationLib/Niggli.cpp \
	../LatticeRepresentationLib/S6.cpp ../LatticeRepresentationLib/S6Dist.cpp \
	../LatticeRepresentationLib/S6_Matrices.cpp ../LatticeRepresentationLib/Selling.cpp \
	../LatticeRepresentationLib/S6BoundaryTransforms.cpp \
	../LatticeRepresentationLib/Sella.cpp \
	../LatticeRepresentationLib/VecN.cpp  ../LatticeRepresentationLib/V7.cpp \
	../LatticeRepresentationLib/Vector_2.cpp \
	../LRL_Tools/LRL_Vector3.cpp \
        libP3.a

clean:
	rm -rf $(COMMAND)
	rm -rf libPolar.a libP3.a
