# This file sets:
# - DTL_INCLUDE_DIRS
# - DTL_LIBRARIES
# - DTL_FOUND


FIND_PATH(DTL_INCLUDE_DIR
    DTL.h
  PATHS
    /usr/include
    /usr/local/include
    /usr/include/dtl
    /usr/local/include/dtl
	${USERLIB_ROOT}/include
	${USERLIB_ROOT}/dtl/lib
	${USERLIB_ROOT}/dtl/include
	c:/libraries/dtl/lib
	c:/dtl/lib
)

FIND_LIBRARY(DTL_LIB DTL
  /usr/lib
  /usr/local/lib
  ${USERLIB_ROOT}/lib
  ${USERLIB_ROOT}/dtl/lib
  ${USERLIB_ROOT}/dtl/lib/release
	c:/libraries/dtl/lib/release
	c:/dtl/lib/release
	c:/libraries/dtl/lib
	c:/dtl/lib
)


SET(DTL_FOUND FALSE)

IF(DTL_INCLUDE_DIR
    AND DTL_LIB)
  SET(DTL_FOUND TRUE)
  SET(DTL_LIBRARIES ${DTL_LIB})
  SET(DTL_INCLUDE_DIRS ${DTL_INCLUDE_DIR})
ENDIF(DTL_INCLUDE_DIR
    AND DTL_LIB)

IF(DTL_FOUND)
    IF(NOT Dtl_FIND_QUIETLY)
      MESSAGE(STATUS "Found the Dtl libraries at ${DTL_LIBRARIES}")
      MESSAGE(STATUS "Found the Dtl headers at ${DTL_INCLUDE_DIRS}")
    ENDIF (NOT Dtl_FIND_QUIETLY)
ELSE(DTL_FOUND)
    IF(Dtl_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could NOT find required dtl libraries")
      MESSAGE(FATAL_ERROR "Please get the library from dtemplatelib.sourceforge.net.")
    ENDIF(Dtl_FIND_REQUIRED)
ENDIF(DTL_FOUND)
