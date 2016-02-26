/*
 * File:   UtilTest.cpp
 * Author: hsk81
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

static const gchar *chars_lhs = "12345";
static const gchar *chars_rhs = "54321";

void UtilTest::testReverseCopyMethod() {

    gsize size = strlen(chars_lhs);
    gchar chars[size + 1];

    Util::reverse_copy((guchar*) chars, (guchar*) chars_lhs, size);
    chars[size] = 0;

    CPPUNIT_ASSERT(!strcmp(chars_rhs, chars));
}

static const gchar *addr1 = "\0\0\0\0\0\0\0\0\0\0\xff\xff\x1\x2\x3\x4";
static const gchar *addr2 = "\0\0\x32\0\0\0\0\0\0\0\xff\xff\x1\x2\x3\x4";

void UtilTest::testIpv4MappedMethod() {

    gboolean is_mapped1 = Net::is_ipv4_mapped((const guchar*) addr1);
    CPPUNIT_ASSERT(is_mapped1);
    gboolean is_mapped2 = Net::is_ipv4_mapped((const guchar*) addr2);
    CPPUNIT_ASSERT(!is_mapped2);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
