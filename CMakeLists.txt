CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(flugbuch2)
SET(CMAKE_MODULE_PATH ${flugbuch2_SOURCE_DIR}/cmake)

SET(VERSION_SERIES 2)
SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 21)


SET(USERLIB_ROOT /usr CACHE PATH "Other installation prefix for dependent libraries")
SET(RUNDIR "/usr/flugbuch2/run" CACHE PATH "Default path for wt session management (only used by FCGI connector; not relative to CMAKE_INSTALL_PREFIX)")
IF(NOT DEFINED CONFIGDIR)
	SET(CONFIGDIR "/etc/flugbuch2" CACHE STRING "Path for the configuration files")
ENDIF(NOT DEFINED CONFIGDIR)
SET(CONFIGDIRAPACHE /etc/apache2 CACHE STRING "Path for the apache2 configuration files")
#SET(BOOST_DIR /usr CACHE PATH "Boost installation path prefix")
#SET(BOOST_COMPILER gcc CACHE STRING "Boost libraries compiler signature")
#SET(BOOST_VERSION "1_37" CACHE STRING "Boost libraries version signature")

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

MARK_AS_ADVANCED(CONFIGDIR)
SET(CONFIGURATION "${CONFIGDIR}/wt_config.xml" CACHE PATH "Path to the wt configuration file")

SET(WEBUSER www-data CACHE STRING "Webserver username (e.g. apache or www)")
SET(WEBGROUP www-data CACHE STRING "Webserver groupname (e.g. apache or www or users)")

SUBDIRS(src)


