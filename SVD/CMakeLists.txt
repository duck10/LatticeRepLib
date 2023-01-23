set(PROJECT_NAME SVD)
project(${PROJECT_NAME} CXX)

################################################################################
# Header files
################################################################################
set(HEADERS_FILES
    svd.h
)
source_group("Headers" FILES ${HEADERS_FILES})

################################################################################
# Source files
################################################################################
set(SRC_FILES
    MAIN_SVD.cpp
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
        $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/../LRL_Tools;${CMAKE_CURRENT_SOURCE_DIR}/../LatticeRepresentationLib;${CMAKE_CURRENT_SOURCE_DIR}/.>
          $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/../LRL_Tools;${CMAKE_CURRENT_SOURCE_DIR}/../LatticeRepresentationLib;${CMAKE_CURRENT_SOURCE_DIR}/.>
    )
endif()
include_directories(../LatticeRepresentationLib)
include_directories(../LRLTools)

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
            $<$<CONFIG:Release>:/GL;/O2;/Oi;/MD;/Gy;/W3;/fp:precise;/EHsc;/GS;/diagnostics:classic;/Zc:wchar_t;/Zc:forScope;/Zc:inline>
              $<$<CONFIG:Debug>:/Od;/MDd;/W3;/fp:precise;/EHsc;/GS;/diagnostics:classic;/Zc:wchar_t;/Zc:forScope;/Zc:inline>
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME} LatticeRepresentationLib)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} LatticeRepresentationLib)



























set(PROJECT_NAME SVD)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "svd_backup.txt"
)
source_group("" FILES ${no_group_source_files})

set(Source_Files
    "MAIN_SVD.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${no_group_source_files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE SVD)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../LatticeRepresentationLib"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../LRL_Tools;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../LatticeRepresentationLib"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    LatticeRepresentationLib
)

