/*
 * File:   BloomTest.cpp
 * Author: hsk81
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
    static const gchar *data1 = "foo";
    this->md1 = new guchar[SHA256_DIGEST_LENGTH];
    SHA256((guchar*) data1, strlen(data1), this->md1);

    static const gchar *data2 = "bar";
    this->md2 = new guchar[SHA256_DIGEST_LENGTH];
    SHA256((guchar*) data2, strlen(data2), this->md2);
}

BloomTest::~BloomTest() {
    delete[] this->md1;
    delete[] this->md2;
}

void BloomTest::setUp() {
    this->filter1 = new struct TBloom;
    this->filter2 = new struct TBloom;
}

void BloomTest::tearDown() {
    delete this->filter1;
    delete this->filter2;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void bf_init(struct TBloom *filter) {
    CPPUNIT_ASSERT(Bloom::init(filter, 1000, 0.001));
}

void bf_free(struct TBloom *filter) {
    Bloom::free(filter);
    CPPUNIT_ASSERT(filter->data == NULL);
}

void bf_op(struct TBloom *filter, guchar *md1, guchar *md2) {
    Bloom::insert(filter, md1, sizeof (md1));

    gboolean has_md1 = Bloom::contains(filter, md1, sizeof (md1));
    CPPUNIT_ASSERT(has_md1 == TRUE);

    gboolean has_md2 = Bloom::contains(filter, md2, sizeof (md2));
    CPPUNIT_ASSERT(has_md2 == FALSE);
}

void bf_serdes(
        struct TBloom *filter1, guchar *md1,
        struct TBloom *filter2, guchar *md2) {

    GString *string = g_string_sized_new(1024);
    Bloom::serialize(string, filter1);
    Bloom::init(filter2);

    struct TConstantBuffer buffer = {string->str, string->len};
    CPPUNIT_ASSERT(Bloom::deserialize(filter2, &buffer));

    CPPUNIT_ASSERT(filter1->n_hash_funcs == filter2->n_hash_funcs);
    CPPUNIT_ASSERT(filter1->data->len == filter2->data->len);
    CPPUNIT_ASSERT(0 == memcmp(
            filter1->data->str, filter2->data->str, filter2->data->len));

    CPPUNIT_ASSERT(Bloom::contains(filter2, md1, sizeof (md1)) == TRUE);
    CPPUNIT_ASSERT(Bloom::contains(filter2, md2, sizeof (md2)) == FALSE);

    g_string_free(string, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void BloomTest::testInitAndFree() {
    bf_init(this->filter1);
    bf_free(this->filter1);
}

void BloomTest::testInsertAndContains() {
    bf_init(this->filter1);
    bf_op(this->filter1, this->md1, this->md2);
    bf_free(this->filter1);
}

void BloomTest::testSerializedAndDeserialize() {
    bf_init(this->filter1);
    bf_init(this->filter2);

    bf_op(this->filter1, this->md1, this->md2);
    bf_serdes(this->filter1, this->md1, this->filter2, this->md2);

    bf_free(this->filter2);
    bf_free(this->filter1);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
