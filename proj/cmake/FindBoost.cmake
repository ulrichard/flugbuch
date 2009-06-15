# This file sets:
# - BOOST_INCLUDE_DIRS
# - BOOST_LIB_DIRS
# - BOOST_FLB_FOUND  // All dependencies for flugbuch2 found
# - BOOST_SUPPORT_LIBRARIES (should always be added to the boost libs)
# - BOOST_FLB_LIBRARIES
#
# Separate compiled boost libraries are also discovered


FIND_PATH(BOOST_INCLUDE_DIRS
    boost/lexical_cast.hpp
  PATHS
    ${BOOST_DIR}/include
    ${BOOST_DIR}/include/${BOOST_VERSION}
    ${BOOST_DIR}/include/boost-${BOOST_VERSION}
    ${BOOST_DIR}
  NO_DEFAULT_PATH
)

SET (BOOST_LIB_DIRS ${BOOST_DIR}/lib)

IF(WIN32)
# FIXME: write some TRY_COMPILEs here to verify that these libs exist
  SET(BOOST_SUPPORT_LIBRARIES "")
ELSE(WIN32)
  FIND_LIBRARY(BOOST_REGEX_LIB_MT
    NAMES
      boost_regex-${BOOST_COMPILER}-mt-${BOOST_VERSION}
      boost_regex-${BOOST_COMPILER}-mt
      boost_regex-mt-${BOOST_VERSION}
      boost_regex-mt
    PATHS
      ${BOOST_LIB_DIRS}
    NO_DEFAULT_PATH
  )

  FIND_LIBRARY(BOOST_PO_LIB_MT
    NAMES
      boost_program_options-${BOOST_COMPILER}-mt-${BOOST_VERSION}
      boost_program_options-${BOOST_COMPILER}-mt
      boost_program_options-mt-${BOOST_VERSION}
      boost_program_options-mt
    PATHS
      ${BOOST_LIB_DIRS}
    NO_DEFAULT_PATH
  )

  FIND_LIBRARY(BOOST_DT_LIB_MT
    NAMES
      boost_date_time-${BOOST_COMPILER}-mt-${BOOST_VERSION}
      boost_date_time-${BOOST_COMPILER}-mt
      boost_date_time-mt-${BOOST_VERSION}
      boost_date_time-mt
    PATHS
      ${BOOST_LIB_DIRS}
    NO_DEFAULT_PATH
  )

  FIND_LIBRARY(BOOST_SIGNALS_LIB_MT
    NAMES
      boost_signals-${BOOST_COMPILER}-mt-${BOOST_VERSION}
      boost_signals-${BOOST_COMPILER}-mt
      boost_signals-mt-${BOOST_VERSION}
      boost_signals-mt
    PATHS
      ${BOOST_LIB_DIRS}
    NO_DEFAULT_PATH
  )

  FIND_LIBRARY(BOOST_THREAD_LIB_MT
    NAMES
      boost_thread-${BOOST_COMPILER}-mt-${BOOST_VERSION}
      boost_thread-${BOOST_COMPILER}-mt
      boost_thread-mt-${BOOST_VERSION}
      boost_thread-mt
    PATHS
      ${BOOST_LIB_DIRS}
    NO_DEFAULT_PATH
  )

  FIND_LIBRARY(BOOST_SYSTEM_LIB_MT
    NAMES
      boost_system-${BOOST_COMPILER}-mt-${BOOST_VERSION}
      boost_system-${BOOST_COMPILER}-mt
      boost_system-mt-${BOOST_VERSION}
      boost_system-mt
    PATHS
      ${BOOST_LIB_DIRS}
    NO_DEFAULT_PATH
  )

  FIND_LIBRARY(BOOST_FS_LIB_MT
    NAMES
      boost_filesystem-${BOOST_COMPILER}-mt-${BOOST_VERSION}
      boost_filesystem-${BOOST_COMPILER}-mt
      boost_filesystem-mt-${BOOST_VERSION}
      boost_filesystem-mt
    PATHS
      ${BOOST_LIB_DIRS}
    NO_DEFAULT_PATH
  )

  FIND_LIBRARY(BOOST_SER_LIB_MT
    NAMES
      boost_serialization-${BOOST_COMPILER}-mt-${BOOST_VERSION}
      boost_serialization-${BOOST_COMPILER}-mt
      boost_serialization-mt-${BOOST_VERSION}
      boost_serialization-mt
    PATHS
      ${BOOST_LIB_DIRS}
    NO_DEFAULT_PATH
  )

# FIXME: todo for Linux
  SET(RT_LIB "rt")
  SET(OTHER_LIBS "-lpthread")
  SET(BOOST_SUPPORT_LIBRARIES "")
ENDIF(WIN32)

SET(BOOST_FLB_FOUND FALSE)

IF(BOOST_INCLUDE_DIRS)
  IF(WIN32)
    # This is not really tested by this script yet - assume availability
    SET(BOOST_FLB_FOUND TRUE)
    SET(BOOST_FLB_LIBRARIES "")

    SET(BOOST_FLB_THREAD_LIBRARY  "")
    SET(BOOST_FLB_REGEX_LIBRARY   "")
    SET(BOOST_FLB_SIGNALS_LIBRARY "")
    SET(BOOST_FLB_PO_LIBRARY      "")
    SET(BOOST_FLB_DT_LIBRARY      "")
    SET(BOOST_FLB_SER_LIB         "")
    SET(BOOST_FLB_SYSTEM_LIB      "")
    SET(BOOST_FLB_FS_LIB          "") 

  ENDIF(WIN32)

  IF(NOT BOOST_FLB_FOUND
     AND BOOST_THREAD_LIB_MT
     AND BOOST_REGEX_LIB_MT
     AND BOOST_SIGNALS_LIB_MT
     AND BOOST_PO_LIB_MT
     AND BOOST_DT_LIB_MT
     AND BOOST_SER_LIB_MT
     AND BOOST_SYSTEM_LIB_MT
     AND BOOST_FS_LIB_MT)

    SET(BOOST_FLB_FOUND TRUE)

    SET(BOOST_FLB_LIBRARIES
        ${BOOST_THREAD_LIB_MT}
        ${BOOST_REGEX_LIB_MT}
        ${BOOST_SIGNALS_LIB_MT}
        ${BOOST_PO_LIB_MT}
        ${BOOST_DT_LIB_MT}
        ${BOOST_SER_LIB_MT}
        ${BOOST_SYSTEM_LIB_MT}
	${BOOST_FS_LIB_MT})

    SET(BOOST_FLB_THREAD_LIBRARY  ${BOOST_THREAD_LIB_MT})
    SET(BOOST_FLB_REGEX_LIBRARY   ${BOOST_REGEX_LIB_MT})
    SET(BOOST_FLB_SIGNALS_LIBRARY ${BOOST_SIGNALS_LIB_MT})
    SET(BOOST_FLB_PO_LIBRARY      ${BOOST_PO_LIB_MT})
    SET(BOOST_FLB_DT_LIBRARY      ${BOOST_DT_LIB_MT})
    SET(BOOST_FLB_SER_LIB         ${BOOST_SER_LIB_MT})
    SET(BOOST_FLB_SYSTEM_LIB      ${BOOST_SYSTEM_LIB_MT})
    SET(BOOST_FLB_FS_LIB          ${BOOST_FS_LIB_MT})

  ENDIF(NOT BOOST_FLB_FOUND
     AND BOOST_THREAD_LIB_MT
     AND BOOST_REGEX_LIB_MT
     AND BOOST_SIGNALS_LIB_MT
     AND BOOST_PO_LIB_MT
     AND BOOST_DT_LIB_MT
     AND BOOST_SER_LIB_MT
     AND BOOST_SYSTEM_LIB_MT
     AND BOOST_FS_LIB_MT)

  IF(BOOST_FLB_FOUND)
    IF(NOT Boost_FIND_QUIETLY)
      MESSAGE(STATUS "Found the Boost libraries at ${BOOST_FLB_LIBRARIES}")
      MESSAGE(STATUS "Found the Boost headers at ${BOOST_INCLUDE_DIRS}")
    ENDIF (NOT Boost_FIND_QUIETLY)
  ELSE(BOOST_FLB_FOUND)
    IF(Boost_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could NOT find required boost libraries")
      MESSAGE("** Could not find a boost installation in " ${BOOST_DIR} ".")
      MESSAGE("** It may be necessary to set appropriate values for the")
      MESSAGE("   variables BOOST_DIR, BOOST_COMPILER, and BOOST_VERSION")
      MESSAGE(FATAL_ERROR "Flugbuch2 requires the following C++ boost libraries: boost_date_time, boost_regex, boost_program_options, boost_signals, and optionally boost_thread")
    ENDIF(Boost_FIND_REQUIRED)
  ENDIF(BOOST_FLB_FOUND)
ENDIF(BOOST_INCLUDE_DIRS)


