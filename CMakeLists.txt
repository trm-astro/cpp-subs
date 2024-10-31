cmake_minimum_required(VERSION 3.23)
project(cpp-subs)

message("CMakeLists.txt for cpp-subs")
message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
 
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

# Add MacPorts
INCLUDE_DIRECTORIES(/opt/local/include)
LINK_DIRECTORIES(/opt/local/lib)

############################################################
# Check for and install dependencies
############################################################

include("${PROJECT_SOURCE_DIR}/build/build/Release/generators/conan_toolchain.cmake")

find_package(SOFA REQUIRED)
find_package(PCRE2 REQUIRED)

############################################################
# Create a library
############################################################
add_library(cpp-subs SHARED 
    ${SOURCES}
)

target_include_directories(cpp-subs
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME} PUBLIC sofa::sofa pcre2::pcre2)

############################################################
# Create an executable
############################################################

# No executable in this code, this is a library


############################################################
# Install
############################################################

# Library
# Note: may not work on windows
install (TARGETS cpp-subs
    LIBRARY DESTINATION lib)

# Header files
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ 
    DESTINATION include)