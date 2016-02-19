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

static const gchar *pch_1 = "12345";
static const gchar *pch_2 = "54321";

void UtilTest::testReverseCopyMethod() {

    gsize _size = strlen(pch_1);
    gchar _puch[_size + 1];

    Util::reverse_copy((guchar *) _puch, (guchar *) pch_1, _size);
    _puch[_size] = 0;

    CPPUNIT_ASSERT(!strcmp(pch_2, _puch));
}

static const gchar *address_1 = "\0\0\0\0\0\0\0\0\0\0\xff\xff\x1\x2\x3\x4";
static const gchar *address_2 = "\0\0\x32\0\0\0\0\0\0\0\xff\xff\x1\x2\x3\x4";

void UtilTest::testIpv4MappedMethod() {

    gboolean mapped_1 = Net::is_ipv4_mapped((const guchar*) address_1);
    CPPUNIT_ASSERT(mapped_1);
    gboolean mapped_2 = Net::is_ipv4_mapped((const guchar*) address_2);
    CPPUNIT_ASSERT(mapped_2 == FALSE);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
