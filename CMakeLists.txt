cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
#----------------------------------------------------------------------------
# Setup the project, first we make sure the compiler is the one corresponding 
# to the ups e7 compiler.. 
#
SET(CMAKE_C_COMPILER "$ENV{GCC_FQ_DIR}/bin/gcc")
SET(CMAKE_CXX_COMPILER "$ENV{GCC_FQ_DIR}/bin/g++")

project(g4hp C CXX)

#----------------------------------------------------------------------------
# Setup geant4 and root
#
find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})
find_package(ROOT REQUIRED)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
		    ${Geant4_INCLUDE_DIR} 
		    $ENV{DK2NU_FQ_DIR}/include
		    $ENV{ROOTSYS}/include
		    )

file(GLOB sources  ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB hheaders ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB headers  ${PROJECT_SOURCE_DIR}/include/*.h)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g" )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build) # just for libg4hpDict.so
#----------------------------------------------------------------------------
#g4hpCint
#
add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/g4hpCint.cc COMMAND rootcint -f 
                   g4hpCint.cc -c include/hptuple.hh include/TrackInfo_t.hh Linkdef.h)
add_library(g4hpDict SHARED ${PROJECT_SOURCE_DIR}/g4hpCint.cc
	    ${PROJECT_SOURCE_DIR}/src/TrackInfo_t.cc 
	    ${PROJECT_SOURCE_DIR}/src/hptuple.cc )

#----------------------------------------------------------------------------
#Build g4hp
#
link_directories($ENV{CLHEP_LIB_DIR} $ENV{DK2NU_FQ_DIR}/lib ${PROJECT_SOURCE_DIR}/build)
add_executable(g4hp g4hp.cc ${sources} ${headers} ${hheaders} ${physics} )
target_link_libraries(g4hp g4hpDict ${Geant4_LIBRARIES} libdk2nuTree.so ${ROOT_LIBRARIES} )

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS g4hp  DESTINATION bin)

