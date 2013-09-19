/*
 * File:   UtilTest.cpp
 * Author: hsk81
 *
 * Created on Jul 8, 2013, 8:36:46 PM
 */

#include "UtilTest.h"
#include "TestLib.h"

#include "../include/Util.h"
#include "../include/Net.h"

#include <string.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION(UtilTest);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static const char *s_a = "12345";
static const char *s_b = "54321";

void UtilTest::testReverseCopyMethod() {

	size_t a_len = strlen(s_a);
	char buf[a_len + 1];
    
    Util::reverse_copy((unsigned char *)buf, (unsigned char *)s_a, a_len);
	buf[a_len] = 0;

	CPPUNIT_ASSERT(!strcmp(s_b, buf));
}

static const char *addr1 = "\0\0\0\0\0\0\0\0\0\0\xff\xff\x1\x2\x3\x4";
static const char *addr2 = "\0\0\x32\0\0\0\0\0\0\0\xff\xff\x1\x2\x3\x4";

void UtilTest::testIpv4MappedMethod() {

	bool rc = Net::is_ipv4_mapped((const unsigned char*)addr1);
	CPPUNIT_ASSERT(rc);
	rc = Net::is_ipv4_mapped((const unsigned char*)addr2);
	CPPUNIT_ASSERT(!rc);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
