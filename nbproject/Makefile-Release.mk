#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/source/Base58.o \
	${OBJECTDIR}/source/HexCode.o \
	${OBJECTDIR}/source/Net.o \
	${OBJECTDIR}/source/Util.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1 \
	${TESTDIR}/TestFiles/f3 \
	${TESTDIR}/TestFiles/f2

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=`pkg-config --cflags glib-2.0` 
CXXFLAGS=`pkg-config --cflags glib-2.0` 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libccoin-xx.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libccoin-xx.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libccoin-xx.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libccoin-xx.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libccoin-xx.a

${OBJECTDIR}/source/Base58.o: source/Base58.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/Base58.o source/Base58.cpp

${OBJECTDIR}/source/HexCode.o: source/HexCode.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/HexCode.o source/HexCode.cpp

${OBJECTDIR}/source/Net.o: source/Net.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/Net.o source/Net.cpp

${OBJECTDIR}/source/Util.o: source/Util.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/Util.o source/Util.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/Base58Test.o ${TESTDIR}/tests/Base58TestRunner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} `pkg-config --libs glib-2.0` `pkg-config --libs openssl` `pkg-config --libs jansson`   `cppunit-config --libs`   

${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/HexCodeTest.o ${TESTDIR}/tests/HexCodeTestRunner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS} `pkg-config --libs glib-2.0` `pkg-config --libs openssl` `pkg-config --libs jansson`   `cppunit-config --libs` `cppunit-config --libs`   

${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/UtilTest.o ${TESTDIR}/tests/UtilTestRunner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS} `pkg-config --libs glib-2.0` `pkg-config --libs openssl` `pkg-config --libs jansson`   `cppunit-config --libs`   


${TESTDIR}/tests/Base58Test.o: tests/Base58Test.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0`   `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/Base58Test.o tests/Base58Test.cpp


${TESTDIR}/tests/Base58TestRunner.o: tests/Base58TestRunner.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0`   `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/Base58TestRunner.o tests/Base58TestRunner.cpp


${TESTDIR}/tests/HexCodeTest.o: tests/HexCodeTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0`   `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/HexCodeTest.o tests/HexCodeTest.cpp


${TESTDIR}/tests/HexCodeTestRunner.o: tests/HexCodeTestRunner.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0`   `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/HexCodeTestRunner.o tests/HexCodeTestRunner.cpp


${TESTDIR}/tests/UtilTest.o: tests/UtilTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0`   `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/UtilTest.o tests/UtilTest.cpp


${TESTDIR}/tests/UtilTestRunner.o: tests/UtilTestRunner.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0`   `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/UtilTestRunner.o tests/UtilTestRunner.cpp


${OBJECTDIR}/source/Base58_nomain.o: ${OBJECTDIR}/source/Base58.o source/Base58.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	@NMOUTPUT=`${NM} ${OBJECTDIR}/source/Base58.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/Base58_nomain.o source/Base58.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/source/Base58.o ${OBJECTDIR}/source/Base58_nomain.o;\
	fi

${OBJECTDIR}/source/HexCode_nomain.o: ${OBJECTDIR}/source/HexCode.o source/HexCode.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	@NMOUTPUT=`${NM} ${OBJECTDIR}/source/HexCode.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/HexCode_nomain.o source/HexCode.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/source/HexCode.o ${OBJECTDIR}/source/HexCode_nomain.o;\
	fi

${OBJECTDIR}/source/Net_nomain.o: ${OBJECTDIR}/source/Net.o source/Net.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	@NMOUTPUT=`${NM} ${OBJECTDIR}/source/Net.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/Net_nomain.o source/Net.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/source/Net.o ${OBJECTDIR}/source/Net_nomain.o;\
	fi

${OBJECTDIR}/source/Util_nomain.o: ${OBJECTDIR}/source/Util.o source/Util.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	@NMOUTPUT=`${NM} ${OBJECTDIR}/source/Util.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/Util_nomain.o source/Util.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/source/Util.o ${OBJECTDIR}/source/Util_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	    ${TESTDIR}/TestFiles/f3 || true; \
	    ${TESTDIR}/TestFiles/f2 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libccoin-xx.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
