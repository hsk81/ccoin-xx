/*
 * File:   HexCodeTest.cpp
 * Author: hsk81
 *
 * Created on Jul 9, 2013, 10:27:13 AM
 */

#include "HexCodeTest.h"
#include "TestLib.h"

#include "../include/HexCode.h"

#include <string.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION(HexCodeTest);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static const char *data = "domitilla cantat lucius ridet\x1\x2\x3\x4";
static size_t data_length = strlen(data);
static char *hex_string;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void HexCodeTest::testEncode1Method() {

    const char *data1 = "\xf1\x1f";
    size_t data1_len = strlen(data1);

    size_t alloc_len = (data_length * 2) + 1000;
    char *hexstr1 = (char*) malloc(alloc_len);
    memset(hexstr1, 0xef, alloc_len);

    HexCode::encode(hexstr1, data1, data1_len);
    CPPUNIT_ASSERT(strcmp(hexstr1, "f11f") == 0);
    free(hexstr1);
}

void HexCodeTest::testEncode2Method() {

    size_t alloc_len = (data_length * 2) + 1000;
    hex_string = (char*) malloc(alloc_len);
    memset(hex_string, 0xef, alloc_len);

    HexCode::encode(hex_string, data, data_length);
    CPPUNIT_ASSERT(strlen(hex_string) == (data_length * 2));
    CPPUNIT_ASSERT((unsigned char) hex_string[(data_length * 2) + 1] == 0xef);
}

void HexCodeTest::testDecode1Method() {

    char decode_buf[(data_length * 2) + 1000];
    memset(decode_buf, 0xef, sizeof (decode_buf));

    size_t out_len = 0;
    bool rc = HexCode::decode(decode_buf, 10, hex_string, &out_len);

    CPPUNIT_ASSERT(!rc);
    CPPUNIT_ASSERT(out_len == 0);

    memset(decode_buf, 0xef, sizeof (decode_buf));
    rc = HexCode::decode(decode_buf, sizeof (decode_buf), hex_string, &out_len);

    CPPUNIT_ASSERT(rc);
    CPPUNIT_ASSERT(out_len == data_length);
    CPPUNIT_ASSERT(memcmp(data, decode_buf, out_len) == 0);
    CPPUNIT_ASSERT((unsigned char) decode_buf[out_len] == 0xef);
}

void HexCodeTest::testDecode2Method() {

    GString *s = HexCode::to_string(hex_string);

    CPPUNIT_ASSERT(s != NULL);
    CPPUNIT_ASSERT(s->len == data_length);
    CPPUNIT_ASSERT(memcmp(s->str, data, data_length) == 0);

    g_string_free(s, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
