cmake_minimum_required(VERSION 3.12)
set(PROJECT_NAME LatticeRepresentationLib)
project(${PROJECT_NAME} CXX)

if (WIN32)
  set(${CMAKE_INSTALL_LIBDIR} "lib")
  set(${CMAKE_INSTALL_DATADIR} "share")
  set(${CMAKE_INSTALL_INCLUDEDIR} "include")
  set(${CMAKE_INSTALL_BINDIR} "bin")
  message(STATUS "Setting installation destination on Windows to: ${CMAKE_INSTALL_PREFIX}")
else()
  include(GNUInstallDirs)
  message(STATUS "Setting installation destination to: ${CMAKE_INSTALL_PREFIX}")
endif()

################################################################################
# Header files
################################################################################

set(HEADERS_FILES
    B4.h
    BasisBase.h
    BravaisHeirarchy.h
    C3.h
    C3Plot.h
    C6.h
    CellInputData.h
    ColorTables.h
    ConstSingleton_T.h
    CS6Dist.h
    D13.h
    D7_Boundary.h
    D7_BoundaryList.h
    D7_ClassifySubboundaries.h
    D7Dist.h
    D7.h
    D7_Subboundary.h
    DC7u.h
    DC.h
    DeloneFitResults.h
    Delone.h
    DeloneType.h
    DeloneTypeList.h
    Delone_Types.h
    DeloneTypeVarietyList.h
    DeloneTypeVerietyList.h
    DirichletCellFaces.h
    Dirichlet.h
    FileOperations.h
    FollowerConstants.h
    FollowerPathGenerator.h
    G6.h
    GenerateLatticeTypeExamples.h
    GenerateRandomLattice.h
    IT_TypeList.h
    LabeledDeloneTypeMatrices.h
    LabeledSellaMatrices.h
    LatticeConverter.h
    LM.h
    LRL_Cell_Degrees.h
    LRL_Cell.h
    LRL_CoordinateConversionMatrices.h
    LRL_CreateFileName.h
    LRL_DataStreamToString.h
    LRL_DataToSVG.h
    LRL_indices.h
    LRL_inverse.h
    LRL_LatticeMatcher.h
    LRL_MatrixBase.h
    LRL_MaximaTools.h
    LRL_MinMaxTools.h
    LRL_RandTools.h
    LRL_ReadLatticeData.h
    LRL_StringTools.h
    LRL_ToString.h
    LRL_Vector3.h
    MatB4.h
    MatD13.h
    MatD7.h
    MatDC7u.h
    MatG6.h
    MatMN.h
    MatN.h
    MatricesForOneDeloneType.h
    MatS6.h
    NCDist.h
    Niggli.h
    Niggli_Types.h
    NullOps.h
    PairReporter.h
    ParseData.h
    ReadTypedData.h
    rhrand.h
    S6BoundaryTransforms.h
    S6Dist.h
    S6.h
    S6_Matrices.h
    S6M_SellingReduce.h
    S6_Ordinals.h
    SellaBuild.h
    Sella.h
    Selling.h
    StoreResults.h
    svd.h
    SVG_Tools.h
    Theta.h
    TNear.h
    TriangleAreaFromSides.h
    triple.h
    V7Dist.h
    VecN.h
    xLRL_LatticeMatcher.h
)
source_group("Headers" FILES ${HEADERS_FILES})

################################################################################
# Source files
################################################################################
set(SRC_FILES
    B4.cpp
    C3.cpp
    CellInputData.cpp
    D7.cpp
    D7_Boundary.cpp
    D7_BoundaryList.cpp
    D7_ClassifySubboundaries.cpp
    D7_Subboundary.cpp
    D7Dist.c
    Delone.cpp
    G6.cpp
    LatticeConverter.cpp
    LRL_Cell.cpp
    LRL_Cell_Degrees.cpp
    LRL_CreateFileName.cpp
    LRL_ReadLatticeData.cpp
    MatB4.cpp
    MatD7.cpp
    MatG6.cpp
    MatMN.cpp
    MatN.cpp
    MatS6.cpp
    Niggli.cpp
    S6.cpp
    S6Dist.cpp
    Selling.cpp
    VecN.cpp
)
source_group("Sources" FILES ${SRC_FILES})

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME}_static STATIC ${SRC_FILES} ${HEADERS_FILES})
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADERS_FILES})

################################################################################
# Install
################################################################################

install(TARGETS ${PROJECT_NAME}_static ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${HEADERS_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
