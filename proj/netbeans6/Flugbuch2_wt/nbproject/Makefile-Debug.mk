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
	${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_wt/../../../src/core/FlightDatabase.o \
	${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/src/web/FlightList_wt.o \
	${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_wt/../../../src/web/main.o \
	${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_wt/../../../src/inout/inout_xml.o \
	${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_wt/../../../src/core/CoreStructures.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/lib -lwt -lwtext -lwtfcgi -lwthttp -lboost_serialization-mt-d -lboost_date_time-mt-d

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} dist/Debug/GNU-Linux-x86/flugbuch2_wt

dist/Debug/GNU-Linux-x86/flugbuch2_wt: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -o dist/Debug/GNU-Linux-x86/flugbuch2_wt ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_wt/../../../src/core/FlightDatabase.o: ../../../src/core/FlightDatabase.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_wt/../../../src/core
	$(COMPILE.cc) -g -I../../../src/web -I../../../src/inout -I../../../src/core -o ${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_wt/../../../src/core/FlightDatabase.o ../../../src/core/FlightDatabase.cpp

${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/src/web/FlightList_wt.o: /home/richi/sourcecode/Flugbuch2/src/web/FlightList_wt.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/src/web
	$(COMPILE.cc) -g -I../../../src/web -I../../../src/inout -I../../../src/core -o ${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/src/web/FlightList_wt.o /home/richi/sourcecode/Flugbuch2/src/web/FlightList_wt.cpp

${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_wt/../../../src/web/main.o: ../../../src/web/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_wt/../../../src/web
	$(COMPILE.cc) -g -I../../../src/web -I../../../src/inout -I../../../src/core -o ${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_wt/../../../src/web/main.o ../../../src/web/main.cpp

${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_wt/../../../src/inout/inout_xml.o: ../../../src/inout/inout_xml.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_wt/../../../src/inout
	$(COMPILE.cc) -g -I../../../src/web -I../../../src/inout -I../../../src/core -o ${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_wt/../../../src/inout/inout_xml.o ../../../src/inout/inout_xml.cpp

${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_wt/../../../src/core/CoreStructures.o: ../../../src/core/CoreStructures.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_wt/../../../src/core
	$(COMPILE.cc) -g -I../../../src/web -I../../../src/inout -I../../../src/core -o ${OBJECTDIR}/_ext/home/richi/sourcecode/Flugbuch2/proj/netbeans6/Flugbuch2_wt/../../../src/core/CoreStructures.o ../../../src/core/CoreStructures.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/flugbuch2_wt

# Subprojects
.clean-subprojects:
