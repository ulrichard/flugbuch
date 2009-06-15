CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#IF(COMMAND CMAKE_POLICY)
#  CMAKE_POLICY(SET CMP0003 OLD)
#ENDIF(COMMAND CMAKE_POLICY)

PROJECT(flugbuch2)
SET(CMAKE_MODULE_PATH ${flugbuch2_SOURCE_DIR})

SET(VERSION_SERIES 2)
SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 6)

SET(FLB_CONNECTOR "The wt connector" wtfcgi)

IF(NOT SHARED_LIBS)
  IF(WIN32)
    OPTION(SHARED_LIBS "Compile shared libraries" OFF)
  ELSE(WIN32)
    OPTION(SHARED_LIBS "Compile shared libraries" ON)
  ENDIF(WIN32)
ENDIF(NOT SHARED_LIBS)

SET(BUILD_SHARED_LIBS ${SHARED_LIBS})

IF(WIN32)
  SET(USERLIB_ROOT "c:/libraries" CACHE PATH "directory containing the dependency libraries")
#  SET(LIB_INSTALL_DIR "lib" CACHE STRING "Default path for libraries within ${CMAKE_INSTALL_PREFIX}")
  SET(RUNDIR "c:/flugbuch2" CACHE PATH "Not really used in WIN32")
  IF( NOT DEFINED CONFIGDIR )
    SET(CONFIGDIR ${RUNDIR} CACHE STRING "Path for the configuration files")
  ENDIF( NOT DEFINED CONFIGDIR )
  SET(CONFIGDIRAPACHE ${RUNDIR} CACHE STRING "Path for the apache2 configuration files")
  SET(DEPLOYROOT c:/program files/flugbuch2 CACHE PATH "Path to deploy examples into")
  SET(BOOST_DIR ${USERLIB_ROOT} CACHE PATH "Use boost in directory")
  SET(BOOST_VERSION "1_35" CACHE STRING "Use boost version in BOOST_DIR")
#  SET(GD_DIR ${USERLIB_ROOT} CACHE PATH "Search path for libgd")
  OPTION(BOOST_DYNAMIC "Link to boost DLLs (OFF means static link)" OFF)
ELSE(WIN32)
  SET(USERLIB_ROOT /usr CACHE PATH "Other installation prefix for dependent libraries")
#  SET(LIB_INSTALL_DIR "lib" CACHE STRING "Default path for libraries within ${CMAKE_INSTALL_PREFIX}")
  SET(RUNDIR "/usr/flugbuch2/run" CACHE PATH "Default path for wt session management (only used by FCGI connector; not relative to CMAKE_INSTALL_PREFIX)")
  IF( NOT DEFINED CONFIGDIR )
    SET(CONFIGDIR "/etc/flugbuch2" CACHE STRING "Path for the configuration files")
  ENDIF( NOT DEFINED CONFIGDIR )
  SET(CONFIGDIRAPACHE /etc/apache2 CACHE STRING "Path for the apache2 configuration files")
  SET(DEPLOYROOT /var/www/localhost/htdocs/flugbuch2 CACHE PATH "Path to deploy into)")
  SET(BOOST_DIR /usr CACHE PATH "Boost installation path prefix")
  SET(BOOST_COMPILER gcc CACHE STRING "Boost libraries compiler signature")
  SET(BOOST_VERSION "1_35" CACHE STRING "Boost libraries version signature")
#  SET(GD_DIR "/usr/lib" CACHE PATH "Search path for libgd2")
ENDIF(WIN32)

MARK_AS_ADVANCED( CONFIGDIR )
SET(CONFIGURATION "${CONFIGDIR}/wt_config.xml" CACHE PATH "Path to the wt configuration file")

SET(WEBUSER apache CACHE STRING "Webserver username (e.g. apache or www)")
SET(WEBGROUP apache CACHE STRING "Webserver groupname (e.g. apache or www or users)")


INCLUDE(FindThreads)
INCLUDE(WtFindBoost.txt)
INCLUDE(WtFindFcgi.txt)
#INCLUDE(WtFindAsio.txt)
#INCLUDE(WtFindSsl.txt)
INCLUDE(FindWt.cmake)
INCLUDE(FindDtl.txt)

IF(NOT DTL_FOUND)
  MESSAGE("** Error finding Flugbuch2 required library: ")
  MESSAGE("** Could not find a database template library installation.")
  MESSAGE("** Please get the library from dtemplatelib.sourceforge.net.")
  MESSAGE(FATAL_ERROR "** Flugbuch2 requires the following C++ libraries: dtl")
ENDIF(NOT DTL_FOUND)

IF(NOT BOOST_WT_FOUND)
  MESSAGE("** Error finding Flugbuch2 required library: ")
  MESSAGE("** Could not find a boost installation in " ${BOOST_DIR} ".")
  MESSAGE("** It may be necessary to set appropriate values for the")
  MESSAGE("   variables BOOST_DIR, BOOST_COMPILER, and BOOST_VERSION")
  MESSAGE(FATAL_ERROR "** Flugbuch2 requires the following C++ boost libraries: boost_date_time, boost_regex, boost_program_options, boost_signals, and optionally boost_thread")
ENDIF(NOT BOOST_WT_FOUND)

ADD_DEFINITIONS(-DWT_THREADED -D_REENTRANT -DBOOST_SPIRIT_THREADSAFE)

FIND_PACKAGE(Wt REQUIRED)

# Make sure the compiler can find include files.
include_directories (${PROJECT_SOURCE_DIR}/../../src) 
include_directories (${PROJECT_SOURCE_DIR}/../../src/core)
include_directories (${PROJECT_SOURCE_DIR}/../../src/inout)
include_directories (${PROJECT_SOURCE_DIR}/../../src/web)
include_directories (${PROJECT_SOURCE_DIR}/../../src/util)
include_directories (${PROJECT_SOURCE_DIR}/../../src/web/stat)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${Wt_INCLUDE_DIR})
include_directories (${DTL_INCLUDE_DIRS})

# Make sure the linker can find the required libraries once it is built.
LINK_DIRECTORIES(${BOOST_LIB_DIRS})

IF(NOT WIN32)
LINK_DIRECTORIES(
	/usr/lib
	/usr/local/lib
)
ENDIF(NOT WIN32)

IF(WIN32)
  IF(BOOST_DYNAMIC)
    ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)
  ENDIF(BOOST_DYNAMIC)
ENDIF(WIN32)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

# Add executable called "flugbuch2_wt.fcgi" that is built from the source files
# "demo.cxx" and "demo_b.cxx". The extensions are automatically found.
add_executable(flugbuch2_wt.fcgi 
	${PROJECT_SOURCE_DIR}/../../src/core/CoreStructures.cpp
	${PROJECT_SOURCE_DIR}/../../src/core/FlightDatabase.cpp
#	${PROJECT_SOURCE_DIR}/../../src/inout/inout_flyhigh.cpp
	${PROJECT_SOURCE_DIR}/../../src/inout/inout_igc.cpp
	${PROJECT_SOURCE_DIR}/../../src/inout/inout_mdb.cpp
#	${PROJECT_SOURCE_DIR}/../../src/inout/inout_mdb_win.cpp
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

# Link the executable to the Hello library.
target_link_libraries(flugbuch2_wt.fcgi 
	${BOOST_WT_LIBRARIES}
	${BOOST_THREAD_LIB_MT}
	${BOOST_REGEX_LIB_MT}
	${BOOST_SIGNALS_LIB_MT}
	${BOOST_PO_LIB_MT}
	${BOOST_DT_LIB_MT}
	${BOOST_SER_LIB_MT}
	${BOOST_SYSTEM_LIB_MT}
	${BOOST_FS_LIB_MT}
	${DTL_LIBRARIES}
	${Wt_LIBRARIES}
	${Wt_LIBRARY}
	${Wt_FCGI_LIBRARY}
	${Wt_EXT_LIBRARY}	
	${FCGI_LIBRARIES}
#	libfcgi
#	libpthread
	odbc
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

INSTALL(FILES ${PROJECT_SOURCE_DIR}/flugbuch2_wt.fcgi DESTINATION ${DEPLOYROOT} )
INSTALL(FILES ${PROJECT_SOURCE_DIR}/../../res/* DESTINATION ${DEPLOYROOT} )
INSTALL(FILES ${PROJECT_SOURCE_DIR}/../../inst/wt_config.xml DESTINATION ${CONFIGDIR} )
INSTALL(FILES ${PROJECT_SOURCE_DIR}/../../inst/flugbuch2.local DESTINATION ${CONFIGDIRAPACHE}/sites-available )



