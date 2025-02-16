CXX	?=	g++-9 -g -O3 -std=gnu++17 -fpermissive
COMMAND = CmdToCell

all:	$(COMMAND)

$(COMMAND):	$(COMMAND).cpp \
	../InputHandler/BaseControlVariables.h ../InputHandler/BaseControlVariables.cpp ../InputHandler/BlockLimits.h \
	../InputHandler/ControlFeature.h ../InputHandler/ControlFeatures.h ../InputHandler/ControlFeatures.cpp \
	../InputHandler/DistanceTypes.h ../InputHandler/DistanceTypes.cpp ../InputHandler/DistanceTypesUtils.h \
	../InputHandler/InputHandler.h ../InputHandler/InputHandler.cpp \
	../InputHandler/LatticeCell.h \
	../LatticeRepresentationLib/B4.cpp ../LatticeRepresentationLib/C3.cpp \
	../LatticeRepresentationLib/C3Plot.cpp ../LatticeRepresentationLib/CellInputData.cpp \
	../LatticeRepresentationLib/ColorTables.h ../LatticeRepresentationLib/ColorTables.cpp \
	../LatticeRepresentationLib/DC7u.cpp \
	../LatticeRepresentationLib/D13.cpp \
	../LatticeRepresentationLib/D7_Boundary.cpp ../LatticeRepresentationLib/D7_BoundaryList.cpp  \
	../LatticeRepresentationLib/D7_ClassifySubboundaries.cpp ../LatticeRepresentationLib/D7.cpp \
	../LatticeRepresentationLib/D7_Subboundary.cpp ../LatticeRepresentationLib/Delone.cpp \
	../LatticeRepresentationLib/DeloneFitResults.cpp \
	../LatticeRepresentationLib/EnhancedS6.h ../LatticeRepresentationLib/EnhancedS6.cpp \
	../LRL_Tools/LRL_CreateFileName.h ../LRL_Tools/LRL_CreateFileName.cpp \
	../LRL_Tools/FileOperations.cpp ../LatticeRepresentationLib/G6.cpp \
	../LRL_Tools/HSV.cpp ../LRL_Tools/HSV.h ../LRL_Tools/RGB.cpp ../LRL_Tools/RGB.h \
	../LatticeRepresentationLib/GenerateLatticeTypeExamples.cpp \
	../LatticeRepresentationLib/LabeledSellaMatrices.cpp \
	../LatticeRepresentationLib/LatticeConverter.cpp ../LatticeRepresentationLib/LRL_Cell.cpp  \
	../LatticeRepresentationLib/LRL_Cell_Degrees.cpp ../LatticeRepresentationLib/LRL_CoordinateConversionMatrices.cpp \
	../LatticeRepresentationLib/EnhancedS6.cpp ../LatticeRepresentationLib/EnhancedS6.h \
	../LRL_Tools/LRL_inverse.cpp ../LRL_Tools/LRL_MaximaTools.cpp \
	../LatticeRepresentationLib/LRL_ReadLatticeData.cpp ../LRL_Tools/LRL_StringTools.cpp \
	../LatticeRepresentationLib/MatD13.cpp \
	../LatticeRepresentationLib/MatD7.cpp ../LatticeRepresentationLib/MatG6.cpp ../LatticeRepresentationLib/MatMN.cpp \
	../LatticeRepresentationLib/MatN.cpp ../LatticeRepresentationLib/MatS6.cpp ../LatticeRepresentationLib/Niggli.cpp \
	../LatticeRepresentationLib/Polar.h ../LatticeRepresentationLib/Polar.cpp \
	../LatticeRepresentationLib/RI.h ../LatticeRepresentationLib/RI.cpp \
	../LatticeRepresentationLib/S6.cpp ../LatticeRepresentationLib/S6Dist.cpp \
	../LatticeRepresentationLib/S6_Matrices.cpp ../LatticeRepresentationLib/Selling.cpp \
	../LatticeRepresentationLib/S6BoundaryTransforms.h ../LatticeRepresentationLib/S6BoundaryTransforms.cpp \
	../LatticeRepresentationLib/Sella.cpp \
	../LatticeRepresentationLib/VecN.cpp  ../LatticeRepresentationLib/V7.cpp ../LRL_Tools/LRL_Vector3.cpp \
	../LatticeRepresentationLib/Vector_2.h ../LatticeRepresentationLib/Vector_2.cpp \
	../LatticeRepresentationLib/WebIO.h ../LatticeRepresentationLib/WebIO.cpp
	$(CXX) -I . -I ../LatticeRepresentationLib -I ../LRL_Tools -I ../InputHandler -o $(COMMAND) $(COMMAND).cpp \
	../InputHandler/BaseControlVariables.cpp  \
	../InputHandler/ControlFeatures.cpp \
	../InputHandler/DistanceTypes.cpp \
	../InputHandler/InputHandler.cpp \
	../LatticeRepresentationLib/B4.cpp ../LatticeRepresentationLib/C3.cpp \
	../LatticeRepresentationLib/C3Plot.cpp ../LatticeRepresentationLib/CellInputData.cpp \
	../LatticeRepresentationLib/ColorTables.cpp \
	../LatticeRepresentationLib/DC7u.cpp \
	../LatticeRepresentationLib/D13.cpp \
	../LatticeRepresentationLib/D7_Boundary.cpp ../LatticeRepresentationLib/D7_BoundaryList.cpp  \
	../LatticeRepresentationLib/D7_ClassifySubboundaries.cpp ../LatticeRepresentationLib/D7.cpp \
	../LatticeRepresentationLib/D7_Subboundary.cpp ../LatticeRepresentationLib/Delone.cpp \
	../LatticeRepresentationLib/DeloneFitResults.cpp \
	../LatticeRepresentationLib/EnhancedS6.cpp \
	../LRL_Tools/LRL_CreateFileName.cpp \
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
	../LatticeRepresentationLib/Polar.cpp \
	../LatticeRepresentationLib/RI.cpp \
	../LatticeRepresentationLib/S6.cpp ../LatticeRepresentationLib/S6Dist.cpp \
	../LatticeRepresentationLib/S6_Matrices.cpp ../LatticeRepresentationLib/Selling.cpp \
	../LatticeRepresentationLib/S6BoundaryTransforms.cpp \
	../LatticeRepresentationLib/Sella.cpp \
	../LatticeRepresentationLib/VecN.cpp  ../LatticeRepresentationLib/V7.cpp ../LRL_Tools/LRL_Vector3.cpp \
	../LatticeRepresentationLib/Vector_2.cpp \
	../LatticeRepresentationLib/WebIO.cpp

clean:
	rm -rf $(COMMAND)
