CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(flugbuch2)
SET(CMAKE_MODULE_PATH ${flugbuch2_SOURCE_DIR})

SET(VERSION_SERIES 2)
SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 6)

IF(WIN32)
  SET(USERLIB_ROOT "c:/libraries" CACHE PATH "directory containing the dependency libraries")
  SET(RUNDIR "c:/flugbuch2" CACHE PATH "Not really used in WIN32")
  IF( NOT DEFINED CONFIGDIR )
    SET(CONFIGDIR ${RUNDIR} CACHE STRING "Path for the configuration files")
  ENDIF( NOT DEFINED CONFIGDIR )
  SET(CONFIGDIRAPACHE ${RUNDIR} CACHE STRING "Path for the apache2 configuration files")
  SET(DEPLOYROOT c:/program files/flugbuch2 CACHE PATH "Path to deploy examples into")
  SET(BOOST_DIR ${USERLIB_ROOT} CACHE PATH "Use boost in directory")
  SET(BOOST_VERSION "1_39" CACHE STRING "Use boost version in BOOST_DIR")
ELSE(WIN32)
  SET(USERLIB_ROOT /usr CACHE PATH "Other installation prefix for dependent libraries")
  SET(RUNDIR "/usr/flugbuch2/run" CACHE PATH "Default path for wt session management (only used by FCGI connector; not relative to CMAKE_INSTALL_PREFIX)")
  IF( NOT DEFINED CONFIGDIR )
    SET(CONFIGDIR "/etc/flugbuch2" CACHE STRING "Path for the configuration files")
  ENDIF( NOT DEFINED CONFIGDIR )
  SET(CONFIGDIRAPACHE /etc/apache2 CACHE STRING "Path for the apache2 configuration files")
  SET(DEPLOYROOT /var/www/localhost/htdocs/flugbuch2 CACHE PATH "Path to deploy into)")
  SET(BOOST_DIR /usr CACHE PATH "Boost installation path prefix")
  SET(BOOST_COMPILER gcc CACHE STRING "Boost libraries compiler signature")
  SET(BOOST_VERSION "1_37" CACHE STRING "Boost libraries version signature")
ENDIF(WIN32)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

MARK_AS_ADVANCED( CONFIGDIR )
SET(CONFIGURATION "${CONFIGDIR}/wt_config.xml" CACHE PATH "Path to the wt configuration file")

SET(WEBUSER apache CACHE STRING "Webserver username (e.g. apache or www)")
SET(WEBGROUP apache CACHE STRING "Webserver groupname (e.g. apache or www or users)")

FIND_PACKAGE(Boost REQUIRED)
FIND_PACKAGE(Wt REQUIRED)
#FIND_PACKAGE(Fcgi REQUIRED)
#FIND_PACKAGE(Dtl REQUIRED)

ADD_DEFINITIONS(-DWT_THREADED -D_REENTRANT -DBOOST_SPIRIT_THREADSAFE)

# Make sure the compiler can find include files.
include_directories (${PROJECT_SOURCE_DIR}/../../src) 
include_directories (${PROJECT_SOURCE_DIR}/../../src/core)
include_directories (${PROJECT_SOURCE_DIR}/../../src/inout)
include_directories (${PROJECT_SOURCE_DIR}/../../src/web)
include_directories (${PROJECT_SOURCE_DIR}/../../src/util)
include_directories (${PROJECT_SOURCE_DIR}/../../src/web/stat)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${Wt_INCLUDE_DIR})
#include_directories (${DTL_INCLUDE_DIRS})


set(FLUGBUCH2_SOURCE_FILES 
	${PROJECT_SOURCE_DIR}/../../src/core/CoreStructures.cpp
	${PROJECT_SOURCE_DIR}/../../src/core/FlightDatabase.cpp
#	${PROJECT_SOURCE_DIR}/../../src/inout/inout_flyhigh.cpp
	${PROJECT_SOURCE_DIR}/../../src/inout/inout_igc.cpp
	${PROJECT_SOURCE_DIR}/../../src/inout/inout_xml.cpp
	${PROJECT_SOURCE_DIR}/../../src/util/SystemInformation.cpp
	${PROJECT_SOURCE_DIR}/../../src/web/AreaTable.cpp
	${PROJECT_SOURCE_DIR}/../../src/web/DetailTableBase.cpp
	${PROJECT_SOURCE_DIR}/../../src/web/FlightTable.cpp
	${PROJECT_SOURCE_DIR}/../../src/web/GliderTable.cpp
	${PROJECT_SOURCE_DIR}/../../src/web/IgcImportForm.cpp
	${PROJECT_SOURCE_DIR}/../../src/web/LocationTable.cpp
	${PROJECT_SOURCE_DIR}/../../src/web/MainMenu.cpp
	${PROJECT_SOURCE_DIR}/../../src/web/StatisticsTab.cpp
	${PROJECT_SOURCE_DIR}/../../src/web/TabControl.cpp
	${PROJECT_SOURCE_DIR}/../../src/web/WelcomeScreen.cpp
	${PROJECT_SOURCE_DIR}/../../src/web/main.cpp
	${PROJECT_SOURCE_DIR}/../../src/web/stat/StatFlightReport.cpp
	${PROJECT_SOURCE_DIR}/../../src/web/stat/StatMap.cpp
	${PROJECT_SOURCE_DIR}/../../src/web/stat/StatStandard.cpp
	${PROJECT_SOURCE_DIR}/../../src/web/stat/Wt/WGeoPosEdit.cpp
)

IF(WIN32)
  set(FLUGBUCH2_SOURCE_FILES 
    ${FLUGBUCH2_SOURCE_FILES}
    ${PROJECT_SOURCE_DIR}/../../src/inout/inout_mdb_win.cpp
  )
ELSE(WIN32)
  set(FLUGBUCH2_SOURCE_FILES 
    ${FLUGBUCH2_SOURCE_FILES}
    ${PROJECT_SOURCE_DIR}/../../src/inout/inout_mdb.cpp
  )
ENDIF(WIN32)

add_executable(flugbuch2_wt.fcgi 
	${FLUGBUCH2_SOURCE_FILES}
)

add_executable(flugbuch2_wt_http 
	${FLUGBUCH2_SOURCE_FILES}
)

# Link the executable library.
target_link_libraries(flugbuch2_wt.fcgi 
	${BOOST_FLB_LIBRARIES}
	${Wt_LIBRARY}
	${Wt_EXT_LIBRARY}	
	${Wt_FCGI_LIBRARY}
#	${FCGI_LIBRARIES}
#	${DTL_LIBRARIES}
#	odbc

) 

target_link_libraries(flugbuch2_wt_http
	${BOOST_FLB_LIBRARIES}
	${Wt_LIBRARY}
	${Wt_HTTP_LIBRARY}
	${Wt_EXT_LIBRARY}	
#	${DTL_LIBRARIES}
#	odbc
) 

IF(WIN32)
  TARGET_LINK_LIBRARIES(flugbuch2_wt.fcgi winmm wsock32)
ENDIF(WIN32)

SET_TARGET_PROPERTIES(
  flugbuch2_wt.fcgi
PROPERTIES
#  POST_INSTALL_SCRIPT ${WT_BINARY_DIR}/WtInstall.cmake
  VERSION ${VERSION_SERIES}.${VERSION_MAJOR}.${VERSION_MINOR}
  DEBUG_POSTFIX "d"
)

INSTALL(FILES     ${PROJECT_BINARY_DIR}/flugbuch2_wt.fcgi          DESTINATION ${DEPLOYROOT} )
INSTALL(FILES     ${PROJECT_SOURCE_DIR}/../../res/flugbuch2.css    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/flugbuch2/ )
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/../../res/ext              DESTINATION ${CMAKE_INSTALL_PREFIX}/share/flugbuch2/ )
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/../../res/img              DESTINATION ${CMAKE_INSTALL_PREFIX}/share/flugbuch2/ )
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/../../res/resources        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/flugbuch2/ )
INSTALL(FILES     ${PROJECT_SOURCE_DIR}/../../inst/wt_config.xml   DESTINATION ${CONFIGDIR} )
INSTALL(FILES     ${PROJECT_SOURCE_DIR}/../../inst/flugbuch2.local DESTINATION ${CONFIGDIRAPACHE}/sites-available )



