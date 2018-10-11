set(PROJECT_NAME LatticeRepresentationLib)
project(${PROJECT_NAME} CXX)

################################################################################
# Header files
################################################################################
set(HEADERS_FILES
    Allman.h
    B4.h
    BasisBase.h
    C3.h
    CellInputData.h
    CS6Dist.h
    D7.h
    D7_Boundary.h
    D7_BoundaryList.h
    D7_ClassifySubboundaries.h
    D7_Subboundary.h
    D7Dist.h
    D7Dist_.hpp
    Delone.h
    G6.h
    GenerateRandomLattice.h
    LatticeConverter.h
    LRL_Cell.h
    LRL_Cell_Degrees.h
    LRL_CoordinateConversionMatrices.h
    LRL_CreateFileName.h
    LRL_MatrixBase.h
    LRL_ReadLatticeData.h
    MatB4.h
    MatD7.h
    MatG6.h
    MatMN.h
    MatN.h
    MatS6.h
    NCDist.h
    Niggli.h
    S6.h
    S6Dist.h
    Selling.h
    VecN.h
)
source_group("Headers" FILES ${HEADERS_FILES})

################################################################################
# Source files
################################################################################
set(SRC_FILES
    Allman.cpp
    B4.cpp
    C3.cpp
    CellInputData.cpp
    D7.cpp
    D7_boundary.cpp
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

# Warning: pch and target are the same for every configuration
################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${HEADERS_FILES})

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"))
    message(WARNING "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Output directory
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT OUT_DIR
        $<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/>
          $<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/>
        $<$<NOT:$<OR:$<CONFIG:Release>,$<CONFIG:Debug>>>:${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${OUT_DIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUT_DIR}")

################################################################################
# Target name
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT TARGET_NAME
        $<$<CONFIG:Release>:${PROJECT_NAME}>
          $<$<CONFIG:Debug>:${PROJECT_NAME}>
        $<$<NOT:$<OR:$<CONFIG:Release>,$<CONFIG:Debug>>>:${PROJECT_NAME}>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${TARGET_NAME}")

################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PRIVATE 
        $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/../LRL_Tools>
          $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/../LRL_Tools>
    )
endif()
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../LRL_Tools)

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:NDEBUG;_CONSOLE;UNICODE;_UNICODE>
          $<$<CONFIG:Debug>:_DEBUG;_CONSOLE;UNICODE;_UNICODE>
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:/GL;/O2;/Oi;/MD;/Gy;/W4;/fp:precise;/EHsc;/GS;/diagnostics:classic;/Zc:wchar_t;/Zc:forScope;/Zc:inline>
              $<$<CONFIG:Debug>:/Od;/MDd;/W4;/fp:precise;/EHsc;/GS;/diagnostics:classic;/Zc:wchar_t;/Zc:forScope;/Zc:inline>
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME} LRL_Tools)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} LRL_Tools)


