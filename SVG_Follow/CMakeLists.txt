set(PROJECT_NAME SVG_Follow)
project(${PROJECT_NAME} CXX)

################################################################################
# Header files
################################################################################
set(HEADERS_FILES
    FileOperations.h
    FileWriter.h
    Follower.h
    FollowerConstants.h
    FollowerIO.h
    Glitch.h
    GlitchElement.h
    LinearAxis.h
    MapBoundaryStrings2Colors.h
    NCDist_.hpp
    OutlierFinder.h
    ReadGlobalData.h
    Rn_Path.h
    SVG_CirclePlot.h
    SVG_DistancePlot.h
    SVG_Writer.h
    triple.h
)
source_group("Headers" FILES ${HEADERS_FILES})

################################################################################
# Source files
################################################################################
set(SRC_FILES
    FileOperations.cpp
    Follow.h
    FollowerConstants.cpp
    FollowerIO.cpp
    LinearAxis.cpp
    main_SVG_Follow.cpp
    MapBoundaryStrings2Colors.cpp
    NCDist.c
    OutlierFinder.cpp
    ReadGlobalData.cpp
)
source_group("Sources" FILES ${SRC_FILES})

# Warning: pch and target are the same for every configuration
################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME}  ${SRC_FILES} ${HEADERS_FILES})

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
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})

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
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TARGET_NAME})

################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PRIVATE 
        $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/../LRL_Tools;${CMAKE_CURRENT_SOURCE_DIR}/../LatticeRepresentationLib>
          $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/../LRL_Tools;${CMAKE_CURRENT_SOURCE_DIR}/../LatticeRepresentationLib>
    )
endif()
include_directories(../LatticeRepresentationLib)
include_directories(../LRL_Tools)

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
add_dependencies(${PROJECT_NAME} LatticeRepresentationLib LRL_Tools)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} LatticeRepresentationLib LRL_Tools)

