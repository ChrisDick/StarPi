########### Tutorial four ##############
PROJECT(indi-starpi C CXX)

cmake_minimum_required(VERSION 3.7)

#CMake modules
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../indi/indi/cmake_modules/")
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../indi/cmake_modules/")
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake_modules/")
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/")
include(GNUInstallDirs)
set(BIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/bin")

#set(INDISKEL "/usr/share/indi/starpi_sk.xml")

# Required Files
find_package(INDI REQUIRED)

set(STARPI_VERSION_MAJOR 0)
set(STARPI_VERSION_MINOR 1)
set(STARPI_PORT 9999)
set(STARPI_HOST "\"127.0.0.1\"")
set(INCLUDE_PATH ${INDI_INCLUDE_DIR} )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h )
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/indi_starpi.xml.cmake ${CMAKE_CURRENT_BINARY_DIR}/indi_starpi.xml )


include_directories( ${CMAKE_CURRENT_BINARY_DIR})
include_directories( ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${INDI_INCLUDE_DIR})
include_directories(${INDI_INCLUDE_DIR}/connectionplugins/)
include_directories(~/indi/libindi/libs)
#include_directories(/home/pi/indi/libindi/libs/indibase/connectionplugins)


add_executable(indi-starpi starpi.cpp)

target_link_libraries(indi-starpi indidriver)

