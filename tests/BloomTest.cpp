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

BloomTest::BloomTest() {
    static const char *data1 = "foo";
    this->md1 = new unsigned char[SHA256_DIGEST_LENGTH];
	SHA256((unsigned char *)data1, strlen(data1), this->md1);

    static const char *data2 = "bar";
    this->md2 = new unsigned char[SHA256_DIGEST_LENGTH];
	SHA256((unsigned char *)data2, strlen(data2), this->md2);
}

BloomTest::~BloomTest() {
    delete[] this->md1;
    delete[] this->md2;
}

void BloomTest::setUp() {
	this->bloom1 = new struct bloom;
	this->bloom2 = new struct bloom;
}

void BloomTest::tearDown() {
    delete this->bloom1;
    delete this->bloom2;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void bf_init(struct bloom *bloom) {
	CPPUNIT_ASSERT(Bloom::init(bloom, 1000, 0.001));
}

void bf_free(struct bloom *bloom) {
    Bloom::free(bloom);
	CPPUNIT_ASSERT(bloom->data == NULL);
}

void bf_op(struct bloom *bloom, unsigned char *md1, unsigned char *md2) {
	Bloom::insert(bloom, md1, sizeof(md1));    
    bool has_md1 = Bloom::contains(bloom, md1, sizeof(md1));
	CPPUNIT_ASSERT(has_md1 == true);
    bool has_md2 = Bloom::contains(bloom, md2, sizeof(md2));
	CPPUNIT_ASSERT(has_md2 == false);
}

void bf_serdes(struct bloom *bloom1, unsigned char *md1,
               struct bloom *bloom2, unsigned char *md2) {

	GString *g_string = g_string_sized_new(1024);
	Bloom::serialize(g_string, bloom1);
	Bloom::__init(bloom2);

	struct const_buffer buffer = { g_string->str, g_string->len };
	CPPUNIT_ASSERT(Bloom::deserialize(bloom2, &buffer));

	CPPUNIT_ASSERT(bloom1->n_hash_funcs == bloom2->n_hash_funcs);
	CPPUNIT_ASSERT(bloom1->data->len == bloom2->data->len);
	CPPUNIT_ASSERT(0 == memcmp(
        bloom1->data->str, bloom2->data->str, bloom2->data->len
    ));

	CPPUNIT_ASSERT(Bloom::contains(bloom2, md1, sizeof(md1)) == true);
	CPPUNIT_ASSERT(Bloom::contains(bloom2, md2, sizeof(md2)) == false);
    
	g_string_free(g_string, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void BloomTest::testInitAndFree() {
    bf_init (this->bloom1);
    bf_free (this->bloom1);
}

void BloomTest::testInsertAndContains() {
    bf_init (this->bloom1);
    bf_op(this->bloom1, this->md1, this->md2);
    bf_free (this->bloom1);
}

void BloomTest::testSerializedAndDeserialize() {
    bf_init (this->bloom1);
    bf_init (this->bloom2);

    bf_op(this->bloom1, this->md1, this->md2);
    bf_serdes(this->bloom1, this->md1,
              this->bloom2, this->md2);

    bf_free (this->bloom2);
    bf_free (this->bloom1);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
