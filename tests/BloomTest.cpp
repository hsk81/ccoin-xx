/*
 * File:   BloomTest.cpp
 * Author: hsk81
 *
 * Created on Sep 19, 2013, 9:05:40 AM
 */

#include "BloomTest.h"
#include "../include/Bloom.h"
#include <openssl/sha.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION(BloomTest);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void BloomTest::testInitAndFree() {
	struct bloom bloom;
	CPPUNIT_ASSERT(Bloom::init(&bloom, 1000, 0.001));
    Bloom::free(&bloom);
	CPPUNIT_ASSERT(bloom.v_data == NULL);
}

void BloomTest::testInsert() {
	struct bloom bloom;
	CPPUNIT_ASSERT(Bloom::init(&bloom, 1000, 0.001));

    static const char *data1 = "foo";
	unsigned char md1[SHA256_DIGEST_LENGTH];
	SHA256((unsigned char *)data1, strlen(data1), md1);

    static const char *data2 = "bar";
	unsigned char md2[SHA256_DIGEST_LENGTH];
	SHA256((unsigned char *)data2, strlen(data2), md2);

	Bloom::insert(&bloom, md1, sizeof(md1));
	CPPUNIT_ASSERT(Bloom::contains(&bloom, md1, sizeof(md1)) == true);
	CPPUNIT_ASSERT(Bloom::contains(&bloom, md2, sizeof(md2)) == false);
    
    Bloom::free(&bloom);
	CPPUNIT_ASSERT(bloom.v_data == NULL);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
