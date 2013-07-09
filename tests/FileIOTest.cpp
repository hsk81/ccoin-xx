/*
 * File:   FileIOTest.cpp
 * Author: hsk81
 *
 * Created on Jul 9, 2013, 11:39:02 AM
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

void *data = NULL;
size_t data_length = 0;

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

	bool rc = Util::read_file(this->filename, &data, &data_length, 100);
	CPPUNIT_ASSERT(!rc);
	CPPUNIT_ASSERT(data == NULL);
	CPPUNIT_ASSERT(data_length == 0);

	rc = Util::read_file(this->filename, &data, &data_length, 100 * 1024*1024);
	CPPUNIT_ASSERT(rc);
	CPPUNIT_ASSERT(data != NULL);
	CPPUNIT_ASSERT(data_length == 8193);

	unsigned char md[SHA_DIGEST_LENGTH];
	SHA1((const unsigned char*)data, data_length, md);
	char hexstr[(SHA_DIGEST_LENGTH * 2) + 1];
	HexCode::encode(hexstr, md, SHA_DIGEST_LENGTH);
	CPPUNIT_ASSERT(strcmp(hexstr, RANDOM_DATA_SHA1SUM) == 0);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
