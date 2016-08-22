
FIND_PACKAGE(Boost 1.55 COMPONENTS system filesystem iostreams unit_test_framework date_time thread regex serialization program_options signals REQUIRED)
FIND_PACKAGE(Wt REQUIRED)
#FIND_PACKAGE(Fcgi REQUIRED)

ADD_DEFINITIONS(-DWT_THREADED -D_REENTRANT -DBOOST_SPIRIT_THREADSAFE)

# Make sure the compiler can find include files.
include_directories (${PROJECT_SOURCE_DIR}/src)
include_directories (${PROJECT_SOURCE_DIR}/src/core)
include_directories (${PROJECT_SOURCE_DIR}/src/inout)
include_directories (${PROJECT_SOURCE_DIR}/src/web)
include_directories (${PROJECT_SOURCE_DIR}/src/util)
include_directories (${PROJECT_SOURCE_DIR}/src/web/stat)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${Wt_INCLUDE_DIR})
#include_directories (${DTL_INCLUDE_DIRS})


set(FLUGBUCH2_SOURCE_FILES
	${PROJECT_SOURCE_DIR}/src/core/CoreStructures.cpp
	${PROJECT_SOURCE_DIR}/src/core/FlightDatabase.cpp
	${PROJECT_SOURCE_DIR}/src/core/WaypointOptimizer.cpp
#	${PROJECT_SOURCE_DIR}/src/inout/inout_flyhigh.cpp
	${PROJECT_SOURCE_DIR}/src/inout/inout_igc.cpp
	${PROJECT_SOURCE_DIR}/src/inout/igc_storage.cpp
	${PROJECT_SOURCE_DIR}/src/inout/inout_xml.cpp
	${PROJECT_SOURCE_DIR}/src/util/SystemInformation.cpp
	${PROJECT_SOURCE_DIR}/src/web/AreaTable.cpp
	${PROJECT_SOURCE_DIR}/src/web/DetailTableBase.cpp
	${PROJECT_SOURCE_DIR}/src/web/FlightTable.cpp
	${PROJECT_SOURCE_DIR}/src/web/GliderTable.cpp
	${PROJECT_SOURCE_DIR}/src/web/IgcImportForm.cpp
	${PROJECT_SOURCE_DIR}/src/web/LocationTable.cpp
	${PROJECT_SOURCE_DIR}/src/web/MainMenu.cpp
	${PROJECT_SOURCE_DIR}/src/web/StatisticsTab.cpp
	${PROJECT_SOURCE_DIR}/src/web/TabControl.cpp
	${PROJECT_SOURCE_DIR}/src/web/WelcomeScreen.cpp
	${PROJECT_SOURCE_DIR}/src/web/main.cpp
	${PROJECT_SOURCE_DIR}/src/web/stat/StatFlightReport.cpp
	${PROJECT_SOURCE_DIR}/src/web/stat/StatMap.cpp
	${PROJECT_SOURCE_DIR}/src/web/stat/StatStandard.cpp
	${PROJECT_SOURCE_DIR}/src/web/stat/Wt/WGeoPosEdit.cpp
)

IF(WIN32)
  set(FLUGBUCH2_SOURCE_FILES
    ${FLUGBUCH2_SOURCE_FILES}
    ${PROJECT_SOURCE_DIR}/src/inout/inout_mdb_win.cpp
  )
ELSE(WIN32)
  set(FLUGBUCH2_SOURCE_FILES
    ${FLUGBUCH2_SOURCE_FILES}
    ${PROJECT_SOURCE_DIR}/src/inout/inout_mdb.cpp
  )
ENDIF(WIN32)

add_executable(flugbuch2_wt.fcgi
	${FLUGBUCH2_SOURCE_FILES}
)

add_executable(flugbuch2_wt_http
	${FLUGBUCH2_SOURCE_FILES}
)

set(BOOST_FLB_LIBRARIES
	${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
#        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        ${Boost_DATE_TIME_LIBRARY}
        ${Boost_THREAD_LIBRARY}
#        ${Boost_REGEX_LIBRARY}
        ${Boost_SERIALIZATION_LIBRARY}
        ${Boost_SIGNALS_LIBRARY}
#        ${Boost_IOSTREAMS_LIBRARY}
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

INSTALL(TARGETS   flugbuch2_wt.fcgi  RUNTIME                           DESTINATION share/flugbuch2/www_docroot )
INSTALL(TARGETS   flugbuch2_wt_http  RUNTIME                           DESTINATION share/flugbuch2/www_docroot )
INSTALL(FILES     ${PROJECT_SOURCE_DIR}/res/flugbuch2.css        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/flugbuch2/www_docroot )
INSTALL(FILES     ${PROJECT_SOURCE_DIR}/res/robots.txt           DESTINATION ${CMAKE_INSTALL_PREFIX}/share/flugbuch2/www_docroot )
INSTALL(FILES     ${PROJECT_SOURCE_DIR}/res/favicon.ico          DESTINATION ${CMAKE_INSTALL_PREFIX}/share/flugbuch2/www_docroot )
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/res/ext                  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/flugbuch2/www_docroot/ )
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/res/img                  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/flugbuch2/www_docroot/ )
INSTALL(DIRECTORY /usr/share/Wt/resources                              DESTINATION ${CMAKE_INSTALL_PREFIX}/share/flugbuch2/www_docroot/ )
INSTALL(FILES     ${PROJECT_SOURCE_DIR}/inst/flugbuch2.local.conf         DESTINATION ${CONFIGDIRAPACHE}/sites-available )
INSTALL(FILES     ${PROJECT_SOURCE_DIR}/inst/flugbuch2.ulrichard.ch.conf  DESTINATION ${CONFIGDIRAPACHE}/sites-available )
INSTALL(FILES     ${PROJECT_SOURCE_DIR}/inst/wt_config.xml       DESTINATION /etc/flugbuch2)


