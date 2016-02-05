/*
 * File:   FileIOTest.cpp
 * Author: hsk81
 */

#include "FileIOTest.h"
#include "TestLib.h"

#include "../include/Util.h"
#include "../include/HexCode.h"

#include <openssl/sha.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION(FileIOTest);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define RANDOM_DATA_SHA1SUM "18833691a6d0ad9c481dcbc6d0da0d3245d7c627"

gpointer data = NULL;
gsize length = 0;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void FileIOTest::setUp() {
    this->filename = TestLib::filename("FileIORandom.dat");
};

void FileIOTest::tearDown() {
    free(this->filename);
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void FileIOTest::testReadMethod() {

    gboolean rc_100 =
            Util::read_file(this->filename, &data, &length, 100);

    CPPUNIT_ASSERT(!rc_100);
    CPPUNIT_ASSERT(data == NULL);
    CPPUNIT_ASSERT(length == 0);

    gboolean rc_100m =
            Util::read_file(this->filename, &data, &length, 100 * 1024 * 1024);

    CPPUNIT_ASSERT(rc_100m);
    CPPUNIT_ASSERT(data != NULL);
    CPPUNIT_ASSERT(length == 8193);

    guchar md[SHA_DIGEST_LENGTH];
    SHA1((const guchar*) data, length, md);
    gchar hexstr[(SHA_DIGEST_LENGTH * 2) + 1];
    HexCode::encode(hexstr, md, SHA_DIGEST_LENGTH);
    CPPUNIT_ASSERT(strcmp(hexstr, RANDOM_DATA_SHA1SUM) == 0);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
