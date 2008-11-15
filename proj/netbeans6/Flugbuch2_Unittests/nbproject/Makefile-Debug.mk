#
# Gererated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/Debug/GNU-Linux-x86

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_Unittests/../../../src/core/FlightDatabase.o \
	${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_Unittests/../../../src/inout/inout_xml.o \
	${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_Unittests/../../../src/core/CoreStructures.o \
	${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_Unittests/../../../src/test/FlugbuchMain.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m32
CXXFLAGS=-m32

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lboost_unit_test_framework-mt-d -lboost_serialization-mt-d -lboost_date_time-mt-d

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} dist/Debug/GNU-Linux-x86/flugbuch2_unittests

dist/Debug/GNU-Linux-x86/flugbuch2_unittests: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -o dist/Debug/GNU-Linux-x86/flugbuch2_unittests ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_Unittests/../../../src/core/FlightDatabase.o: ../../../src/core/FlightDatabase.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_Unittests/../../../src/core
	$(COMPILE.cc) -g -I../../../src/core -I../../../src/inout -I../../../src/test -o ${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_Unittests/../../../src/core/FlightDatabase.o ../../../src/core/FlightDatabase.cpp

${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_Unittests/../../../src/inout/inout_xml.o: ../../../src/inout/inout_xml.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_Unittests/../../../src/inout
	$(COMPILE.cc) -g -I../../../src/core -I../../../src/inout -I../../../src/test -o ${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_Unittests/../../../src/inout/inout_xml.o ../../../src/inout/inout_xml.cpp

${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_Unittests/../../../src/core/CoreStructures.o: ../../../src/core/CoreStructures.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_Unittests/../../../src/core
	$(COMPILE.cc) -g -I../../../src/core -I../../../src/inout -I../../../src/test -o ${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_Unittests/../../../src/core/CoreStructures.o ../../../src/core/CoreStructures.cpp

${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_Unittests/../../../src/test/FlugbuchMain.o: ../../../src/test/FlugbuchMain.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_Unittests/../../../src/test
	$(COMPILE.cc) -g -I../../../src/core -I../../../src/inout -I../../../src/test -o ${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_Unittests/../../../src/test/FlugbuchMain.o ../../../src/test/FlugbuchMain.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/flugbuch2_unittests

# Subprojects
.clean-subprojects:
