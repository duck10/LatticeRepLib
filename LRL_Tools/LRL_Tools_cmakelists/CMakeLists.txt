set(PROJECT_NAME LRL_Tools)
project(${PROJECT_NAME} CXX)

################################################################################
# Header files
################################################################################
set(HEADERS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../ConstSingleton_T.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../LRL_DataStreamToString.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../LRL_DataToSVG.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../LRL_inverse.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../LRL_MaximaTools.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../LRL_MinMaxTools.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../LRL_RandTools.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../LRL_StringTools.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../LRL_ToString.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../LRL_Vector3.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../PairReporter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../rhrand.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../spline_under_tension.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../StoreResults.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../SuperFastHash.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Theta.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../TNear.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../triple.h
)
source_group("Headers" FILES ${HEADERS_FILES})

################################################################################
# Source files
################################################################################
set(SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../LRL_inverse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../LRL_MaximaTools.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../LRL_RandTools.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../LRL_StringTools.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../LRL_Vector3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../spline_under_tension.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../SuperFastHash.cpp
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
          $<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/>
        $<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/>
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${OUT_DIR})

################################################################################
# Target name
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT TARGET_NAME
          $<$<CONFIG:Debug>:${PROJECT_NAME}>
        $<$<CONFIG:Release>:${PROJECT_NAME}>
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${PROJECT_NAME}>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TARGET_NAME})

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
          $<$<CONFIG:Debug>:_DEBUG;_LIB;UNICODE;_UNICODE>
        $<$<CONFIG:Release>:NDEBUG;_LIB;UNICODE;_UNICODE>
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
              $<$<CONFIG:Debug>:/Od;/MDd;/W3;/fp:precise;/EHsc;/GS;/diagnostics:classic;/Zc:wchar_t;/Zc:forScope;/Zc:inline>
            $<$<CONFIG:Release>:/GL;/O2;/Oi;/MD;/Gy;/W3;/fp:precise;/EHsc;/GS;/diagnostics:classic;/Zc:wchar_t;/Zc:forScope;/Zc:inline>
        )
    endif()
endif()

