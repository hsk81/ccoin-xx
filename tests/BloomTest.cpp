/*
 * File:   BloomTest.cpp
 * Author: hsk81
 *
 * Created on Sep 19, 2013, 9:05:40 AM
 */

#include "BloomTest.h"
#include "TestLib.h"

#include "../include/Buffer.h"
#include "../include/Bloom.h"

#include <openssl/sha.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION(BloomTest);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void BloomTest::testAll() {
	struct bloom bloom1;
	CPPUNIT_ASSERT(Bloom::init(&bloom1, 1000, 0.001));

    static const char *data1 = "foo";
	unsigned char md1[SHA256_DIGEST_LENGTH];
	SHA256((unsigned char *)data1, strlen(data1), md1);

    static const char *data2 = "bar";
	unsigned char md2[SHA256_DIGEST_LENGTH];
	SHA256((unsigned char *)data2, strlen(data2), md2);

	Bloom::insert(&bloom1, md1, sizeof(md1));
	CPPUNIT_ASSERT(Bloom::contains(&bloom1, md1, sizeof(md1)) == true);
	CPPUNIT_ASSERT(Bloom::contains(&bloom1, md2, sizeof(md2)) == false);

	GString *g_string = g_string_sized_new(1024);
	Bloom::serialize(g_string, &bloom1);
	struct bloom bloom2;
	Bloom::__init(&bloom2);

	struct const_buffer buffer = { g_string->str, g_string->len };
	CPPUNIT_ASSERT(Bloom::deserialize(&bloom2, &buffer));

	CPPUNIT_ASSERT(bloom1.n_hash_funcs == bloom2.n_hash_funcs);
	CPPUNIT_ASSERT(bloom1.v_data->len == bloom2.v_data->len);
	CPPUNIT_ASSERT(0 == memcmp(
        bloom1.v_data->str, bloom2.v_data->str, bloom2.v_data->len
    ));

	CPPUNIT_ASSERT(Bloom::contains(&bloom2, md1, sizeof(md1)) == true);
	CPPUNIT_ASSERT(Bloom::contains(&bloom2, md2, sizeof(md2)) == false);
    
	g_string_free(g_string, TRUE);

    Bloom::free(&bloom2);
	CPPUNIT_ASSERT(bloom2.v_data == NULL);
    Bloom::free(&bloom1);
	CPPUNIT_ASSERT(bloom1.v_data == NULL);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
