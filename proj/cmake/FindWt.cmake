# Find Wt includes and libraries
#
# This script sets the following variables:
#
#  Wt_INCLUDE_DIR
#  Wt_LIBRARIES  - Release libraries
#  Wt_FOUND  - True if release libraries found
#
# To direct the script to a particular Wt installation, use the
# standard cmake variables CMAKE_INCLUDE_PATH and CMAKE_LIBRARY_PATH
#
# To use this script to find Wt, include the following CMake snippet in your project:
#
#  FIND_PACKAGE( Wt REQUIRED )
#  INCLUDE_DIRECTORIES( ${Wt_INCLUDE_DIR} )
#  TARGET_LINK_LIBRARIES( yourexe
#    ${Wt_LIBRARY}
#    ${Wt_HTTP_LIBRARY}
#    ${Wt_EXT_LIBRARY}
#  )
#
# Copyright (c) 2007, Pau Garcia i Quiles, <pgquiles@elpauer.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
# 2009.06.12 changed to only find the release libraries by Richard Ulrich

FIND_PATH( Wt_INCLUDE_DIR NAMES Wt/WObject PATHS ENV PATH PATH_SUFFIXES include wt )

SET( Wt_FIND_COMPONENTS Release )

IF( Wt_INCLUDE_DIR )
        FIND_LIBRARY( Wt_LIBRARY NAMES wt PATHS PATH PATH_SUFFIXES lib lib-release lib_release )
        FIND_LIBRARY( Wt_EXT_LIBRARY NAMES wtext PATHS PATH PATH_SUFFIXES lib lib-release lib_release )
        FIND_LIBRARY( Wt_HTTP_LIBRARY NAMES wthttp PATHS PATH PATH_SUFFIXES lib lib-release lib_release )
        FIND_LIBRARY( Wt_FCGI_LIBRARY NAMES wtfcgi PATHS PATH PATH_SUFFIXES lib lib-release lib_release )

        IF( Wt_LIBRARY AND Wt_EXT_LIBRARY ) 
		SET( Wt_FIND_REQUIRED_Release TRUE )
		IF( Wt_FCGI_LIBRARY )
			SET( Wt_FOUND TRUE )
	                SET( Wt_LIBRARIES ${Wt_LIBRARY} ${Wt_EXT_LIBRARY} ${Wt_FCGI_LIBRARY} )
		ELSE( Wt_FCGI_LIBRARY )
			IF( Wt_HTTP_LIBRARY )
				SET( Wt_FOUND TRUE )
				SET( Wt_LIBRARIES ${Wt_LIBRARY} ${Wt_EXT_LIBRARY} ${Wt_HTTP_LIBRARY} )
			ENDIF( Wt_HTTP_LIBRARY )
	        ENDIF( Wt_FCGI_LIBRARY )
        ENDIF( Wt_LIBRARY AND Wt_EXT_LIBRARY)	

        

        IF(Wt_FOUND)
                IF (NOT Wt_FIND_QUIETLY)
                        MESSAGE(STATUS "Found the Wt libraries at ${Wt_LIBRARIES}")
                        MESSAGE(STATUS "Found the Wt headers at ${Wt_INCLUDE_DIR}")
                ENDIF (NOT Wt_FIND_QUIETLY)
        ELSE(Wt_FOUND)
                IF(Wt_FIND_REQUIRED)
                        MESSAGE(FATAL_ERROR "Could NOT find Wt")
                ENDIF(Wt_FIND_REQUIRED)
        ENDIF(Wt_FOUND)

ENDIF( Wt_INCLUDE_DIR )
