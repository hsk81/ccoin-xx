/*
 * File:   FileIOTest.cpp
 * Author: hsk81
 */

#include "FileIoTest.h"
#include "TestLib.h"

#include "../include/Util.h"
#include "../include/HexCode.h"

#include <openssl/sha.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION(FileIoTest);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define RANDOM_DATA_SHA1SUM "18833691a6d0ad9c481dcbc6d0da0d3245d7c627"

gpointer pointer = NULL;
gsize size = 0;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void FileIoTest::setUp() {
    this->filename = TestLib::filename("FileIORandom.dat");
};

void FileIoTest::tearDown() {
    free(this->filename);
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void FileIoTest::testReadMethod() {

    gboolean rc_100 =
            Util::read_file(this->filename, &pointer, &size, 100);

    CPPUNIT_ASSERT(!rc_100);
    CPPUNIT_ASSERT(pointer == NULL);
    CPPUNIT_ASSERT(size == 0);

    gboolean rc_100m =
            Util::read_file(this->filename, &pointer, &size, 104857600);

    CPPUNIT_ASSERT(rc_100m);
    CPPUNIT_ASSERT(pointer != NULL);
    CPPUNIT_ASSERT(size == 8193);

    guchar md[SHA_DIGEST_LENGTH];
    SHA1((const guchar*) pointer, size, md);
    gchar hexstr[(SHA_DIGEST_LENGTH * 2) + 1];
    HexCode::encode(hexstr, md, SHA_DIGEST_LENGTH);

    CPPUNIT_ASSERT(strcmp(hexstr, RANDOM_DATA_SHA1SUM) == 0);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
