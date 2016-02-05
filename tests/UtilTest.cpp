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

static const gchar *s_a = "12345";
static const gchar *s_b = "54321";

void UtilTest::testReverseCopyMethod() {

    gsize a_len = strlen(s_a);
    gchar buf[a_len + 1];

    Util::reverse_copy((guchar *) buf, (guchar *) s_a, a_len);
    buf[a_len] = 0;

    CPPUNIT_ASSERT(!strcmp(s_b, buf));
}

static const gchar *addr1 = "\0\0\0\0\0\0\0\0\0\0\xff\xff\x1\x2\x3\x4";
static const gchar *addr2 = "\0\0\x32\0\0\0\0\0\0\0\xff\xff\x1\x2\x3\x4";

void UtilTest::testIpv4MappedMethod() {

    gboolean rc = Net::is_ipv4_mapped((const guchar*) addr1);
    CPPUNIT_ASSERT(rc);
    rc = Net::is_ipv4_mapped((const guchar*) addr2);
    CPPUNIT_ASSERT(!rc);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
