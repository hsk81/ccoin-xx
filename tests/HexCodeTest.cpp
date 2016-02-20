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

static const gchar *chars = "domitilla cantat lucius ridet\x1\x2\x3\x4";
static gsize size = strlen(chars);
static gchar *hex_chars;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void HexCodeTest::testEncode1Method() {

    const gchar *chars = "\xf1\x1f";
    gsize size = strlen(chars);

    gsize alloc_size = (size * 2) + 1000;
    gchar *hex_chars = (gchar*) malloc(alloc_size);
    memset(hex_chars, 0xef, alloc_size);

    HexCode::encode(hex_chars, chars, size);
    CPPUNIT_ASSERT(strcmp(hex_chars, "f11f") == 0);
    free(hex_chars);
}

void HexCodeTest::testEncode2Method() {

    gsize alloc_size = (size * 2) + 1000;
    hex_chars = (gchar*) malloc(alloc_size);
    memset(hex_chars, 0xef, alloc_size);

    HexCode::encode(hex_chars, chars, size);
    CPPUNIT_ASSERT(strlen(hex_chars) == size * 2);
    CPPUNIT_ASSERT((guchar) hex_chars[(size * 2) + 1] == 0xef);
}

void HexCodeTest::testDecode1Method() {

    gchar buffer[(size * 2) + 1000];
    memset(buffer, 0xef, sizeof (buffer));

    gsize out_size = 0;
    gboolean rc1 = HexCode::decode(
            buffer, 10, hex_chars, &out_size);

    CPPUNIT_ASSERT(!rc1);
    CPPUNIT_ASSERT(out_size == 0);

    memset(buffer, 0xef, sizeof (buffer));
    gboolean rc2 =HexCode::decode(
            buffer, sizeof (buffer), hex_chars, &out_size);

    CPPUNIT_ASSERT(rc2);
    CPPUNIT_ASSERT(out_size == size);
    CPPUNIT_ASSERT(memcmp(chars, buffer, out_size) == 0);
    CPPUNIT_ASSERT((guchar) buffer[out_size] == 0xef);
}

void HexCodeTest::testDecode2Method() {

    GString *string = HexCode::to_string(hex_chars);

    CPPUNIT_ASSERT(string != NULL);
    CPPUNIT_ASSERT(string->len == size);
    CPPUNIT_ASSERT(memcmp(string->str, chars, size) == 0);

    g_string_free(string, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
