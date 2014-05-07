CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

PROJECT( UtilityPrograms )

# High level project configuration

SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Products" )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Products" )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Products" )

# Build with Multiple Processes

IF(MSVC)
  OPTION( BUILD_WITH_MULTIPLE_PROCESSES "/MP compiler flag for full processor utilization" ON )
  MARK_AS_ADVANCED( BUILD_WITH_MULTIPLE_PROCESSES )
ENDIF()

# Required dependencies 

## OpenStudio

SET( OPENSTUDIO_BUILD_DIR "" CACHE PATH "Build or install directory for OpenStudio build to link against")

INCLUDE ( "${OPENSTUDIO_BUILD_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/OpenStudioCoreConfig.cmake" )

IF(MSVC)
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Products/Debug")
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Products/Release")
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Products/RelWithDebInfo")
  LIST( APPEND CMAKE_LIBRARY_PATH ${OPENSTUDIO_LIB_DIR} )

  file(GLOB DEBUG_DLLS "${OPENSTUDIO_BUILD_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/Debug/*.dll")
  file(GLOB RELEASE_DLLS "${OPENSTUDIO_BUILD_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/Release/*.dll")
  file(GLOB RELDBG_DLLS "${OPENSTUDIO_BUILD_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/RelWithDebInfo/*.dll")

  file(COPY ${DEBUG_DLLS} DESTINATION "${CMAKE_BINARY_DIR}/Products/Debug/")
  file(COPY ${RELEASE_DLLS} DESTINATION "${CMAKE_BINARY_DIR}/Products/Release/")
  file(COPY ${RELDBG_DLLS} DESTINATION "${CMAKE_BINARY_DIR}/Products/RelWithDebInfo/")
ENDIF()

INCLUDE_DIRECTORIES( ${OpenStudioCore_INCLUDE_DIRS} )

link_directories(${OpenStudioCore_LIBRARY_DIRS})

## Boost
IF(APPLE)
  set(Boost_USE_STATIC_LIBS ON)  
ENDIF()
IF (WIN32)
  set(Boost_USE_STATIC_LIBS ON)  
ENDIF (WIN32)

SET( Boost_ADDITIONAL_VERSIONS "1.40" )
FIND_PACKAGE( Boost 1.40.0 REQUIRED COMPONENTS filesystem regex program_options system thread date_time serialization)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
MARK_AS_ADVANCED( Boost_LIB_DIAGNOSTIC_DEFINITIONS )
ADD_DEFINITIONS( -DBOOST_FILESYSTEM_VERSION=2 )

## Boost-log
ADD_DEFINITIONS( -DBOOST_LOG_NO_LIB ) # disable autolinking
ADD_DEFINITIONS( -DBOOST_LOG_SETUP_NO_LIB ) # disable autolinking

SET(BoostLog_DIR "${OPENSTUDIO_BUILD_DIR}/BoostLog-prefix/src/BoostLog-build")
find_package(BoostLog REQUIRED)
INCLUDE_DIRECTORIES( ${BoostLog_INCLUDE_DIRS} )
LINK_DIRECTORIES( ${BoostLog_LIBRARY_DIRS} )

## Qt
FIND_PACKAGE (Qt4 4.6.0 COMPONENTS QtCore REQUIRED)
INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )

# QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to
# a CMake script that sets up this directory for building with Qt4
INCLUDE( ${QT_USE_FILE} )
MARK_AS_ADVANCED( QT_QMAKE_EXECUTABLE QT_QTMOTIF_INCLUDE_DIR QT_QTMOTIF_LIBRARY_DEBUG QT_QTMOTIF_LIBRARY_RELEASE )

# QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to
# a CMake script that sets up this directory for building with Qt4
INCLUDE( ${QT_USE_FILE} )

# Dependencies

SET( DEPENDENCIES
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS}
  ${QT_LIBS}
  openstudio_utilities
  # openstudio_model
  # openstudio_osversion
  # openstudio_contam
  # openstudio_energyplus
  boost_log
)

# generate rules for building source files from the resources
#qt4_add_resources( resources.qrc )

# Executables

#add_executable(epwtowth epwtowth.cpp)

#TARGET_LINK_LIBRARIES( epwtowth ${DEPENDENCIES} )

add_executable(epwtest epwtest.cpp)

TARGET_LINK_LIBRARIES( epwtest ${DEPENDENCIES} )
