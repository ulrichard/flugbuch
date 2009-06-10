CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(flugbuch2)

SET(VERSION_SERIES 2)
SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 5)

IF(WIN32)
  SET(USERLIB_ROOT "c:/libraries" CACHE PATH "directory containing the dependency libraries")
  SET(LIB_INSTALL_DIR "lib" CACHE STRING "Default path for libraries within ${CMAKE_INSTALL_PREFIX}")
  SET(RUNDIR "c:/flugbuch2" CACHE PATH "Not really used in WIN32")
  IF( NOT DEFINED CONFIGDIR )
    SET(CONFIGDIR ${RUNDIR} CACHE STRING "Path for the configuration files")
  ENDIF( NOT DEFINED CONFIGDIR )
  SET(CONFIGDIRAPACHE ${RUNDIR} CACHE STRING "Path for the apache2 configuration files")
#  SET(DEPLOYROOT c:/witty/wt-examples CACHE PATH "Path to deploy examples into")
  SET(BOOST_DIR ${USERLIB_ROOT} CACHE PATH "Use boost in directory")
  SET(BOOST_VERSION "1_35" CACHE STRING "Use boost version in BOOST_DIR")
#  SET(GD_DIR ${USERLIB_ROOT} CACHE PATH "Search path for libgd")
  OPTION(BOOST_DYNAMIC "Link to boost DLLs (OFF means static link)" OFF)
ELSE(WIN32)
  SET(USERLIB_ROOT /usr CACHE PATH "Other installation prefix for dependent libraries")
  SET(LIB_INSTALL_DIR "lib" CACHE STRING "Default path for libraries within ${CMAKE_INSTALL_PREFIX}")
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

# Make sure the compiler can find include files.
include_directories (${PROJECT_SOURCE_DIR}/../../src) 
include_directories (${PROJECT_SOURCE_DIR}/../../../src/core)
include_directories (${PROJECT_SOURCE_DIR}/../../../src/inout)
include_directories (${PROJECT_SOURCE_DIR}/../../../src/web)
include_directories (${PROJECT_SOURCE_DIR}/../../../src/util)
include_directories (${PROJECT_SOURCE_DIR}/../../../src/web/stat)
include_directories (${BOOST_INCLUDE_DIRS})

# Make sure the linker can find the required libraries once it is built.
#link_directories (${HELLO_BINARY_DIR}/Hello)
LINK_DIRECTORIES(${BOOST_LIB_DIRS})
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
	${PROJECT_SOURCE_DIR}/../../../src/core/CoreStructures.cpp
	${PROJECT_SOURCE_DIR}/../../../src/core/FlightDatabase.cpp
#	${PROJECT_SOURCE_DIR}/../../../src/inout/inout_flyhigh.cpp
	${PROJECT_SOURCE_DIR}/../../../src/inout/inout_igc.cpp
	${PROJECT_SOURCE_DIR}/../../../src/inout/inout_mdb.cpp
#	${PROJECT_SOURCE_DIR}/../../../src/inout/inout_mdb_win.cpp
	${PROJECT_SOURCE_DIR}/../../../src/inout/inout_xml.cpp
	${PROJECT_SOURCE_DIR}/../../../src/util/SystemInformation.cpp
	${PROJECT_SOURCE_DIR}/../../../src/web/AreaTable.cpp
	${PROJECT_SOURCE_DIR}/../../../src/web/DetailTableBase.cpp
	${PROJECT_SOURCE_DIR}/../../../src/web/FlightTable.cpp
	${PROJECT_SOURCE_DIR}/../../../src/web/GliderTable.cpp
	${PROJECT_SOURCE_DIR}/../../../src/web/IgcImportForm.cpp
	${PROJECT_SOURCE_DIR}/../../../src/web/LocationTable.cpp
	${PROJECT_SOURCE_DIR}/../../../src/web/MainMenu.cpp
	${PROJECT_SOURCE_DIR}/../../../src/web/StatisticsTab.cpp
	${PROJECT_SOURCE_DIR}/../../../src/web/TabControl.cpp
	${PROJECT_SOURCE_DIR}/../../../src/web/WelcomeScreen.cpp
	${PROJECT_SOURCE_DIR}/../../../src/web/main.cpp
	${PROJECT_SOURCE_DIR}/../../../src/web/stat/StatFlightReport.cpp
	${PROJECT_SOURCE_DIR}/../../../src/web/stat/StatMap.cpp
	${PROJECT_SOURCE_DIR}/../../../src/web/stat/StatStandard.cpp
	${PROJECT_SOURCE_DIR}/../../../src/web/stat/Wt/WGeoPosEdit.cpp
)

# Link the executable to the Hello library.
target_link_libraries(flugbuch2_wt.fcgi 
	libboost_date_time-mt
	libboost_serialization-mt
	libboost_signals-mt
	libboost_filesystem-mt
	libwt
	libwtext
	libwtfcgi
	libfcgi
	libboost_thread-mt
	libboost_program_options-mt
	libboost_regex-mt
	libboost_system-mt
	libpthread
	wt
	wtext
	wthttp
	DTL
	odbc
) 

INSTALL(FILES ${PROJECT_SOURCE_DIR}../../../inst/wt_config.xml DESTINATION ${CONFIGDIR} )
INSTALL(FILES ${PROJECT_SOURCE_DIR}../../../inst/wt_config.xml DESTINATION ${CONFIGDIRAPACHE}/sites-available )



