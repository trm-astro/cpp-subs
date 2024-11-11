cmake_minimum_required(VERSION 3.23)
project(cpp-subs)

include(ExternalProject)

message("CMakeLists.txt for cpp-subs")
message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

set(PACKAGE_NAME subs)

############################################################
# Define sources
############################################################

set(SOURCES
    src/gauss.cc
    src/ran.cc
    src/ephem.cc
    src/gammln.cc
    src/gammp.cc
    src/gammq.cc
    src/gser.cc
    src/hitem.cc
    src/header.cc
    src/date.cc
    src/time.cc
    src/position.cc
    src/star.cc
    src/binary.cc
    src/telescope.cc
    src/svdfit.cc
    src/filnam.cc
    src/tchi.cc
    src/input.cc
    src/string_funcs.cc
    src/centroid.cc
    src/bsstep.cc
    src/mmid.cc
    src/pzextr.cc
    src/gcf.cc
    src/lud.cc
    src/gaussj.cc
    src/jacob.cc
    src/sleep.cc
    src/sigma_reject.cc
    src/what_colour.cc
    src/planck.cc
    src/fft.cc
    src/llsqr.cc
    src/fasper.cc
    src/complex.cc
    src/plot.cc
    src/formula.cc
    src/fraction.cc
    src/units.cc
    src/format.cc
    src/poly.cc
    src/rebin.cc
    src/amoeba.cc
    src/genetic.cc
    src/rtsafe.cc
    src/brent.cc
    src/dbrent.cc
    src/mnbrak.cc
    src/powell.cc
    src/safunc.cc
    src/poisson.cc
    src/extinct.cc
    src/byte_swap.cc
    src/endian.cc
    src/boxcar.cc
    src/numdiff.cc
    src/factln.cc
    src/runge_kutta.cc
    src/voigt.cc
    src/stoerm.cc
)

############################################################
# Create a library
############################################################
add_library(${PACKAGE_NAME} SHARED 
    ${SOURCES}
)

target_include_directories(${PACKAGE_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

############################################################
# Check for and install dependencies
############################################################

include("${CMAKE_BINARY_DIR}/Release/generators/conan_toolchain.cmake")

# Configure the PLPLOT library

# Default behaviour is to download and build the PLPLOT library for the system
# PLPLOT_BUILD_TYPE:
#   Nobuild: 0 (PLPLOT_USE_PATH must be set) get lib from location
#   Local: 1 (Default) Build in the build directory
#   Path: 2 (PLPLOT_USE_PATH must be set)
#   System: 3 Build in /usr/local/src/plplot
#   Brew: 4 Use Homebrew to get the library
# PLPLOT_USE_PATH (Required for Path or Nobuild)

if (NOT DEFINED PLPLOT_BUILD_TYPE)
    set(PLPLOT_BUILD_TYPE 1)
endif()

#TODO: Test this
# We need to set the name for the plplot lib
if (NOT DEFINED PLPLOT_LIB_NAME)
    set(PLPLOT_LIB_NAME libplplotcxx.dylib) # works on mac
endif()

if(NOT DEFINED PLPLOT_USE_PATH)
    if(PLPLOT_BUILD_TYPE EQUAL 0 OR PLPLOT_BUILD_TYPE EQUAL 2)
        message(FATAL_ERROR "PLPLOT_USE_PATH must be set if PLPLOT_BUILD_TYPE is ${PLPLOT_BUILD_TYPE}")
    elseif(PLPLOT_BUILD_TYPE EQUAL 1)
        set(PLPLOT_USE_PATH ${CMAKE_BINARY_DIR}/plplot_install)
    elseif(PLPLOT_BUILD_TYPE EQUAL 3)
        set(PLPLOT_USE_PATH /usr/local/src/plplot)
    endif()
endif()


if(PLPLOT_BUILD_TYPE EQUAL 0)
    message("PLPLOT_BUILD_TYPE: Nobuild")
    # Assert that PLPLOT_USE_PATH is set
    if(NOT DEFINED PLPLOT_USE_PATH)
        message(FATAL_ERROR "PLPLOT_USE_PATH must be set if PLPLOT_BUILD_TYPE is 0")
    endif()
    set(PLPLOT_LIB_PATH ${PLPLOT_USE_PATH})
elseif(PLPLOT_BUILD_TYPE EQUAL 1 OR PLPLOT_BUILD_TYPE EQUAL 2 OR PLPLOT_BUILD_TYPE EQUAL 3) # Build at defined or dfault path
    # Assert that PLPLOT_USE_PATH is set
    if(NOT DEFINED PLPLOT_USE_PATH)
        message(FATAL_ERROR "PLPLOT_USE_PATH undefined")
    else()
        message("Building Plplot at ${PLPLOT_USE_PATH}")
    endif()

    ExternalProject_Add(
        plplot
        URL https://sourceforge.net/projects/plplot/files/plplot/5.15.0%20Source/plplot-5.15.0.tar.gz
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PLPLOT_USE_PATH}
                -DCMAKE_BUILD_TYPE=Release
                -DPLPLOT_BUILD_SHARED=ON       # Optionally build shared libraries
                -DENABLE_python=OFF            # Disable Python bindings if not needed
                -DENABLE_tcl=OFF               # Disable Tcl bindings if not needed
        INSTALL_DIR ${PLPLOT_USE_PATH}
    )
    add_dependencies(${PACKAGE_NAME} plplot)
    set(PLPLOT_LIB_PATH ${PLPLOT_USE_PATH})

elseif(PLPLOT_BUILD_TYPE EQUAL 4) # Brew build of PLPLOT
    message("PLPLOT_BUILD_TYPE: Brew")
    # Set PLplot paths using Homebrew's default install location
    set(PLPLOT_LIB_PATH /opt/homebrew/opt/plplot)
endif()

if(NOT DEFINED PLPLOT_LIB_PATH)
    message(FATAL_ERROR "PLPLOT_LIB_PATH undefined")
else()
    set(PLPLOT_INCLUDE_DIR ${PLPLOT_LIB_PATH}/include/plplot)
    set(PLPLOT_LIB_DIR ${PLPLOT_LIB_PATH}/lib)
    set(PLPLOT_LIB ${PLPLOT_LIB_DIR}/${PLPLOT_LIB_NAME})
    set(PLPLOT_FOUND TRUE)
    message("PLPLOT_INCLUDE_DIR: ${PLPLOT_INCLUDE_DIR}")
    message("PLPLOT_LIB_DIR: ${PLPLOT_LIB_DIR}")
    message("PLPLOT_LIB: ${PLPLOT_LIB}")
endif()

find_package(SOFA REQUIRED)
find_package(PCRE2 REQUIRED)

if (NOT SOFA_FOUND)
    message(FATAL_ERROR "SOFA library not found.")
endif()
if (NOT PCRE2_FOUND)
    message(FATAL_ERROR "PCRE2 library not found.")
endif()
if (NOT PLPLOT_FOUND)
    message(FATAL_ERROR "PLPLOT library not found.")
endif()

target_include_directories(${PACKAGE_NAME}
    PUBLIC 
        $<BUILD_INTERFACE:${PLPLOT_INCLUDE_DIR}>
)
target_link_libraries(${PACKAGE_NAME} 
    PUBLIC 
        sofa::sofa 
        pcre2::pcre2
        ${PLPLOT_LIB_DIR}/libplplotcxx.dylib
)

############################################################
# Create an executable
############################################################

# No executable in this code, this is a library


############################################################
# Install
############################################################

# Library
# Note: may not work on windows
# Specify where the library binary will go
install(TARGETS ${PACKAGE_NAME}
    EXPORT ${PACKAGE_NAME}Targets              # Export the library target for package config
    LIBRARY DESTINATION lib              # For shared libraries
    ARCHIVE DESTINATION lib              # For static libraries
    RUNTIME DESTINATION bin              # For executables (if any)
    INCLUDES DESTINATION include         # Where to put public headers
)

# Header files
install(DIRECTORY include/ 
    DESTINATION include
)

# # Create and install CMake package configuration files
include(CMakePackageConfigHelpers)

# # Configure the version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}ConfigVersion.cmake"
    VERSION 0.1
    COMPATIBILITY AnyNewerVersion
)

configure_file(subsConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}Config.cmake"
    @ONLY
)

# # Install the config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}ConfigVersion.cmake"
    DESTINATION lib/cmake/${PACKAGE_NAME}
)

# # Export the library target so other projects can import it
install(EXPORT ${PACKAGE_NAME}Targets
    FILE ${PACKAGE_NAME}Targets.cmake
    NAMESPACE subs::
    DESTINATION lib/cmake/${PACKAGE_NAME}
)