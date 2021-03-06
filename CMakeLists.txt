# Min version 3.9 for OpenMP find_package
cmake_minimum_required(VERSION 3.9)
project(calculatorcompare)

# From CMAKE_SYSTEM work out which of __OSX__, __GLN__, __NIX__ or __WIN__ are required
message(STATUS "Operating system: " ${CMAKE_SYSTEM})
if(CMAKE_SYSTEM MATCHES Linux.*)
  set(EXTRA_HOST_DEFINITION "-D__GLN__")
elseif(CMAKE_SYSTEM MATCHES BSD.*)
  set(EXTRA_HOST_DEFINITION "-D__NIX__")
elseif(APPLE)
  set(EXTRA_HOST_DEFINITION "-D__OSX__")
else()
  message(ERROR "Operating system not supported: " ${CMAKE_SYSTEM})
endif()

# morphologica uses c++-17 language features
set(CMAKE_CXX_STANDARD 17)

# Add the host definition to CXXFLAGS along with other switches,
# depending on OS/Compiler and your needs/preferences
if (APPLE)
  set(CMAKE_CXX_FLAGS "${EXTRA_HOST_DEFINITION} -Wall -Wfatal-errors")
else() # assume g++ (or a gcc/g++ mimic like Clang)
  set(CMAKE_CXX_FLAGS "${EXTRA_HOST_DEFINITION} -Wall -Wfatal-errors")
  set(CMAKE_C_FLAGS "${EXTRA_HOST_DEFINITION} -Wall -Wfatal-errors")
endif()

# Tell clang to be quiet about brace initialisers:
if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-braces")
endif()

# Add OpenMP flags here, if necessary
find_package(OpenMP)
if(OpenMP_FOUND)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp-simd")
endif()

# Additional GL compiler flags.
set(OpenGL_GL_PREFERENCE "GLVND")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGL3_PROTOTYPES -DGL_GLEXT_PROTOTYPES")
if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGL_SILENCE_DEPRECATION")
endif()

# Tell the program where the morph fonts are, to compile them into the binary
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMORPH_FONTS_DIR=\"\\\"${PROJECT_SOURCE_DIR}/morphologica/fonts\\\"\"")

# Assuming that you installed morphologica in-tree (i.e. 'next to' schnakenberg.cpp).
set(MORPH_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/morphologica" CACHE PATH "The path to morphologica")
#include_directories(BEFORE ${MORPH_INCLUDE_PATH}/include) # Allows GL3/gl3.h to be found
include_directories(BEFORE ${MORPH_INCLUDE_PATH})         # Allows morph/Header.h to be found

set(EIGEN_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/eigen-3.4.0" CACHE PATH "The path to Eigen")
include_directories(BEFORE ${EIGEN_INCLUDE_PATH})         # Allows Eigen/Header to be found

if(OpenMP_FOUND)
  link_libraries(OpenMP::OpenMP_CXX)
endif()

if(NOT EXISTS "${MORPH_INCLUDE_PATH}/morph")
  message(FATAL_ERROR "Make sure you have installed morphologica at the location: ${MORPH_INCLUDE_PATH}. The suggested way to do this is to git clone morphologica inside the ${PROJECT_NAME} base directory")
endif()

## Programs
add_executable(testVector testVector.cpp) # from morphologica
add_executable(testvVector testvVector.cpp) # from morphologica

add_executable(testEigen testEigen.cpp)

add_executable(exercise exercise.cpp)
add_executable(exercise_smallvecs exercise_smallvecs.cpp)

add_executable(exerciseEigen exerciseEigen.cpp)
target_compile_options(exerciseEigen PUBLIC -mavx2 -O3)

add_executable(exercisevVector exercisevVector.cpp)
add_executable(simpleEigen simpleEigen.cpp)
add_executable(simplevVector simplevVector.cpp)

# -mavx2.
add_executable(avx2_example avx2_example.c)
target_compile_options(avx2_example PUBLIC -mavx2)
add_executable(avx2_longvector avx2_longvector.c)
target_compile_options(avx2_longvector PUBLIC -mavx2)
add_executable(avx2_longvector2 avx2_longvector.cpp)
target_compile_options(avx2_longvector2 PUBLIC -mavx2)

add_executable(avx2_vVector avx2_vVector.cpp)
target_compile_options(avx2_vVector PUBLIC -mavx2 -O3)

# -mavx512f. Compiles, even on my i9 which doesn't have avx512.
add_executable(avx512_example avx512_example.c)
target_compile_options(avx512_example PUBLIC -mavx512f)

# For debugging of variables:
option(DEBUG_VARIABLES OFF)
if(DEBUG_VARIABLES)
  get_cmake_property(_variableNames VARIABLES)
  foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
  endforeach()
endif(DEBUG_VARIABLES)
