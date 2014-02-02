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
	${OBJECTDIR}/source/Bloom.o \
	${OBJECTDIR}/source/Buffer.o \
	${OBJECTDIR}/source/HexCode.o \
	${OBJECTDIR}/source/Key.o \
	${OBJECTDIR}/source/Message.o \
	${OBJECTDIR}/source/Net.o \
	${OBJECTDIR}/source/Serialize.o \
	${OBJECTDIR}/source/Util.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1 \
	${TESTDIR}/TestFiles/f7 \
	${TESTDIR}/TestFiles/f5 \
	${TESTDIR}/TestFiles/f4 \
	${TESTDIR}/TestFiles/f3 \
	${TESTDIR}/TestFiles/f6 \
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

${OBJECTDIR}/source/Bloom.o: source/Bloom.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/Bloom.o source/Bloom.cpp

${OBJECTDIR}/source/Buffer.o: source/Buffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/Buffer.o source/Buffer.cpp

${OBJECTDIR}/source/HexCode.o: source/HexCode.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/HexCode.o source/HexCode.cpp

${OBJECTDIR}/source/Key.o: source/Key.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/Key.o source/Key.cpp

${OBJECTDIR}/source/Message.o: source/Message.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/Message.o source/Message.cpp

${OBJECTDIR}/source/Net.o: source/Net.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/Net.o source/Net.cpp

${OBJECTDIR}/source/Serialize.o: source/Serialize.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/Serialize.o source/Serialize.cpp

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

${TESTDIR}/TestFiles/f7: ${TESTDIR}/tests/BlockTest.o ${TESTDIR}/tests/BlockTestRunner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f7 $^ ${LDLIBSOPTIONS} `pkg-config --libs glib-2.0` `pkg-config --libs openssl` `pkg-config --libs jansson`   `cppunit-config --libs`   

${TESTDIR}/TestFiles/f5: ${TESTDIR}/tests/BloomTest.o ${TESTDIR}/tests/BloomTestRunner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f5 $^ ${LDLIBSOPTIONS} `pkg-config --libs glib-2.0` `pkg-config --libs openssl` `pkg-config --libs jansson`   `cppunit-config --libs`   

${TESTDIR}/TestFiles/f4: ${TESTDIR}/tests/FileIOTest.o ${TESTDIR}/tests/FileIOTestRunner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f4 $^ ${LDLIBSOPTIONS} `pkg-config --libs glib-2.0` `pkg-config --libs openssl` `pkg-config --libs jansson`   `cppunit-config --libs`   

${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/HexCodeTest.o ${TESTDIR}/tests/HexCodeTestRunner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS} `pkg-config --libs glib-2.0` `pkg-config --libs openssl` `pkg-config --libs jansson`   `cppunit-config --libs` `cppunit-config --libs`   

${TESTDIR}/TestFiles/f6: ${TESTDIR}/tests/KeysetTest.o ${TESTDIR}/tests/KeysetTestRunner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f6 $^ ${LDLIBSOPTIONS} `pkg-config --libs glib-2.0` `pkg-config --libs openssl` `pkg-config --libs jansson`   `cppunit-config --libs` `cppunit-config --libs`   

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


${TESTDIR}/tests/BlockTest.o: tests/BlockTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0`   `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/BlockTest.o tests/BlockTest.cpp


${TESTDIR}/tests/BlockTestRunner.o: tests/BlockTestRunner.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0`   `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/BlockTestRunner.o tests/BlockTestRunner.cpp


${TESTDIR}/tests/BloomTest.o: tests/BloomTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0`   `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/BloomTest.o tests/BloomTest.cpp


${TESTDIR}/tests/BloomTestRunner.o: tests/BloomTestRunner.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0`   `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/BloomTestRunner.o tests/BloomTestRunner.cpp


${TESTDIR}/tests/FileIOTest.o: tests/FileIOTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0`   `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/FileIOTest.o tests/FileIOTest.cpp


${TESTDIR}/tests/FileIOTestRunner.o: tests/FileIOTestRunner.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0`   `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/FileIOTestRunner.o tests/FileIOTestRunner.cpp


${TESTDIR}/tests/HexCodeTest.o: tests/HexCodeTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0`   `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/HexCodeTest.o tests/HexCodeTest.cpp


${TESTDIR}/tests/HexCodeTestRunner.o: tests/HexCodeTestRunner.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0`   `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/HexCodeTestRunner.o tests/HexCodeTestRunner.cpp


${TESTDIR}/tests/KeysetTest.o: tests/KeysetTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0`   `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/KeysetTest.o tests/KeysetTest.cpp


${TESTDIR}/tests/KeysetTestRunner.o: tests/KeysetTestRunner.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0`   `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/KeysetTestRunner.o tests/KeysetTestRunner.cpp


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

${OBJECTDIR}/source/Bloom_nomain.o: ${OBJECTDIR}/source/Bloom.o source/Bloom.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	@NMOUTPUT=`${NM} ${OBJECTDIR}/source/Bloom.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/Bloom_nomain.o source/Bloom.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/source/Bloom.o ${OBJECTDIR}/source/Bloom_nomain.o;\
	fi

${OBJECTDIR}/source/Buffer_nomain.o: ${OBJECTDIR}/source/Buffer.o source/Buffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	@NMOUTPUT=`${NM} ${OBJECTDIR}/source/Buffer.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/Buffer_nomain.o source/Buffer.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/source/Buffer.o ${OBJECTDIR}/source/Buffer_nomain.o;\
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

${OBJECTDIR}/source/Key_nomain.o: ${OBJECTDIR}/source/Key.o source/Key.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	@NMOUTPUT=`${NM} ${OBJECTDIR}/source/Key.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/Key_nomain.o source/Key.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/source/Key.o ${OBJECTDIR}/source/Key_nomain.o;\
	fi

${OBJECTDIR}/source/Message_nomain.o: ${OBJECTDIR}/source/Message.o source/Message.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	@NMOUTPUT=`${NM} ${OBJECTDIR}/source/Message.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/Message_nomain.o source/Message.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/source/Message.o ${OBJECTDIR}/source/Message_nomain.o;\
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

${OBJECTDIR}/source/Serialize_nomain.o: ${OBJECTDIR}/source/Serialize.o source/Serialize.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	@NMOUTPUT=`${NM} ${OBJECTDIR}/source/Serialize.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 `pkg-config --cflags glib-2.0` -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/Serialize_nomain.o source/Serialize.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/source/Serialize.o ${OBJECTDIR}/source/Serialize_nomain.o;\
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
	    ${TESTDIR}/TestFiles/f7 || true; \
	    ${TESTDIR}/TestFiles/f5 || true; \
	    ${TESTDIR}/TestFiles/f4 || true; \
	    ${TESTDIR}/TestFiles/f3 || true; \
	    ${TESTDIR}/TestFiles/f6 || true; \
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
