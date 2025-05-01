cmake_minimum_required(VERSION 3.10)
project(CrystalLatticeTransformer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler flags
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Find existing libraries
# Uncomment and modify these if needed for your environment
# find_library(LRL_VECTOR3_LIB LRL_Vector3)
# find_library(LATTICE_CELL_LIB LatticeCell)
# find_library(INPUT_HANDLER_LIB InputHandler)

# Source files
set(SOURCE_FILES
    CellTransformer.cpp
    CellTransformerMain.cpp
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    # Add paths to your existing libraries here
    # ${CMAKE_CURRENT_SOURCE_DIR}/../LRL_Vector3
    # ${CMAKE_CURRENT_SOURCE_DIR}/../InputHandler
)

# Create the main executable
add_executable(CellTransformer ${SOURCE_FILES})

# Link against existing libraries
# Uncomment and modify these if needed for your environment
# target_link_libraries(CellTransformer
#     ${LRL_VECTOR3_LIB}
#     ${LATTICE_CELL_LIB}
#     ${INPUT_HANDLER_LIB}
# )

# Install
install(TARGETS CellTransformer DESTINATION bin)

# Add test executable
set(TEST_SOURCE_FILES
    CellTransformer.cpp
    TestCellTransformer.cpp
)

add_executable(TestCellTransformer ${TEST_SOURCE_FILES})

# Link test executable against existing libraries
# target_link_libraries(TestCellTransformer
#     ${LRL_VECTOR3_LIB}
#     ${LATTICE_CELL_LIB}
#     ${INPUT_HANDLER_LIB}
# )

install(TARGETS TestCellTransformer DESTINATION bin)