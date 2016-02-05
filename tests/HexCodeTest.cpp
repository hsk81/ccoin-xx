/*
 * File:   HexCodeTest.cpp
 * Author: hsk81
 */

#include "HexCodeTest.h"
#include "TestLib.h"

#include "../include/HexCode.h"
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION(HexCodeTest);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static const gchar *data = "domitilla cantat lucius ridet\x1\x2\x3\x4";
static gsize length = strlen(data);
static gchar *hex_string;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void HexCodeTest::testEncode1Method() {

    const gchar *data1 = "\xf1\x1f";
    gsize data1_len = strlen(data1);

    gsize alloc_len = (length * 2) + 1000;
    gchar *hexstr1 = (gchar*) malloc(alloc_len);
    memset(hexstr1, 0xef, alloc_len);

    HexCode::encode(hexstr1, data1, data1_len);
    CPPUNIT_ASSERT(strcmp(hexstr1, "f11f") == 0);
    free(hexstr1);
}

void HexCodeTest::testEncode2Method() {

    gsize alloc_len = (length * 2) + 1000;
    hex_string = (gchar*) malloc(alloc_len);
    memset(hex_string, 0xef, alloc_len);

    HexCode::encode(hex_string, data, length);
    CPPUNIT_ASSERT(strlen(hex_string) == (length * 2));
    CPPUNIT_ASSERT((guchar) hex_string[(length * 2) + 1] == 0xef);
}

void HexCodeTest::testDecode1Method() {

    gchar buffer[(length * 2) + 1000];
    memset(buffer, 0xef, sizeof (buffer));

    gsize out_length = 0;
    gboolean rc1 =
            HexCode::decode(buffer, 10, hex_string, &out_length);

    CPPUNIT_ASSERT(!rc1);
    CPPUNIT_ASSERT(out_length == 0);

    memset(buffer, 0xef, sizeof (buffer));
    gboolean rc2 =
            HexCode::decode(buffer, sizeof (buffer), hex_string, &out_length);

    CPPUNIT_ASSERT(rc2);
    CPPUNIT_ASSERT(out_length == length);
    CPPUNIT_ASSERT(memcmp(data, buffer, out_length) == 0);
    CPPUNIT_ASSERT((guchar) buffer[out_length] == 0xef);
}

void HexCodeTest::testDecode2Method() {

    GString *string = HexCode::to_string(hex_string);

    CPPUNIT_ASSERT(string != NULL);
    CPPUNIT_ASSERT(string->len == length);
    CPPUNIT_ASSERT(memcmp(string->str, data, length) == 0);

    g_string_free(string, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
