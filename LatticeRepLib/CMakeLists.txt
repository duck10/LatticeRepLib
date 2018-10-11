cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

project(LatticeRepLib CXX C)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE TYPE INTERNAL FORCE
)

################################################################################
# Global compiler options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
endif()

################################################################################
# Global linker options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
endif()

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours.")
endfunction()

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Sub-projects
################################################################################
cmake_policy(SET CMP0046 NEW)
include_directories(LRL_Tools)
add_subdirectory(LRL_Tools)
file(GLOB LRL_Tools_sources LRL_Tools/*.cpp LRL_Tools/*.c LRL_Tools/*/h )
SET(lrl_tools ${LRL_Tools_sources}) 
add_library(LRL_Tools STATIC ${lrl_tools})
target_link_libraries(LRL_Tools)
add_subdirectory(LatticeRepresentationLib)
add_subdirectory(PointDistanceFollower)
add_subdirectory(B4_rotations)
add_subdirectory(ConvertToC3)
add_subdirectory(DevelopS6Boundaries)
add_subdirectory(GenerateBoundaryPairs)
add_subdirectory(MultiMetricDists)
#add_subdirectory(S6Classifier)
add_subdirectory(spline_under_tension)
add_subdirectory(SVD)
add_subdirectory(SVG_Follow)
add_subdirectory(SVG_TriangleFollow)
add_subdirectory(TestBases)
add_subdirectory(Testbed)
add_subdirectory(TestC3)
add_subdirectory(TestRandom)
#add_subdirectory(TestSuperCell)

