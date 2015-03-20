CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT( UtilityPrograms )

# High level project configuration

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Products" )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Products" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Products" )

# Build with Multiple Processes

IF(MSVC)
  OPTION( BUILD_WITH_MULTIPLE_PROCESSES "/MP compiler flag for full processor utilization" ON )
  MARK_AS_ADVANCED( BUILD_WITH_MULTIPLE_PROCESSES )
ENDIF()

# Required dependencies 

## OpenStudio

set(OPENSTUDIO_BUILD_DIR "" CACHE PATH "Build or install directory for OpenStudio build to link against")

INCLUDE ( "${OPENSTUDIO_BUILD_DIR}/OSCore-prefix/src/OSCore-build/OpenStudioCoreConfig.cmake" )

IF(MSVC)
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Products/Debug")
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Products/Release")
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Products/RelWithDebInfo")
  LIST( APPEND CMAKE_LIBRARY_PATH ${OPENSTUDIO_LIB_DIR} )

  file(GLOB DEBUG_DLLS "${OPENSTUDIO_BUILD_DIR}/OSCore-prefix/src/OSCore-build/Products/Debug/*.dll")
  file(GLOB RELEASE_DLLS "${OPENSTUDIO_BUILD_DIR}/OSCore-prefix/src/OSCore-build/Products/Release/*.dll")
  file(GLOB RELDBG_DLLS "${OPENSTUDIO_BUILD_DIR}/OSCore-prefix/src/OSCore-build/Products/RelWithDebInfo/*.dll")

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

find_package(Boost 1.55.0 COMPONENTS filesystem regex program_options system thread chrono date_time log QUIET)

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

## Qt
find_package(Qt5Widgets)
find_package(Qt5Sql)
find_package(Qt5Network)
find_package(Qt5Xml)
find_package(Qt5WebEngine)
find_package(Qt5WebEngineWidgets)
find_package(Qt5Concurrent)
find_package(Qt5PrintSupport)

mark_as_advanced(
  Qt5Concurrent_DIR
  Qt5Core_DIR
  Qt5Gui_DIR
  Qt5Gui_EGL_LIBRARY
  Qt5Location_DIR
  Qt5Multimedia_DIR
  Qt5MultimediaWidgets_DIR
  Qt5Network_DIR
  Qt5OpenGL_DIR
  Qt5Positioning_DIR
  Qt5PrintSupport_DIR
  Qt5Qml_DIR
  Qt5Quick_DIR
  Qt5Sensors_DIR
  Qt5Sql_DIR
  Qt5WebEngine_DIR
  Qt5WebEngineWidgets_DIR
  Qt5Widgets_DIR
  Qt5Xml_DIR
)

# Dependencies

SET( DEPENDENCIES
  ${Boost_LIBRARIES}
  openstudio_utilities
  openstudio_model
  openstudio_osversion
  openstudio_energyplus
)

# Executables

add_executable(epwtowth epwtowth.cpp)
TARGET_LINK_LIBRARIES(epwtowth ${DEPENDENCIES})

add_executable(epwtest epwtest.cpp)
TARGET_LINK_LIBRARIES(epwtest ${DEPENDENCIES})

#add_executable(addafnidf addafnidf.cpp)
#TARGET_LINK_LIBRARIES(addafnidf ${DEPENDENCIES})

add_executable(builddemomodel builddemomodel.cpp)
TARGET_LINK_LIBRARIES(builddemomodel ${DEPENDENCIES})
