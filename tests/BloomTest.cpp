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
    SHA256((guchar *) data1, strlen(data1), this->md1);

    static const gchar *data2 = "bar";
    this->md2 = new guchar[SHA256_DIGEST_LENGTH];
    SHA256((guchar *) data2, strlen(data2), this->md2);
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

void bf_op(struct bloom *bloom, guchar *md1, guchar *md2) {
    Bloom::insert(bloom, md1, sizeof (md1));
    gboolean has_md1 = Bloom::contains(bloom, md1, sizeof (md1));
    CPPUNIT_ASSERT(has_md1 == TRUE);
    gboolean has_md2 = Bloom::contains(bloom, md2, sizeof (md2));
    CPPUNIT_ASSERT(has_md2 == FALSE);
}

void bf_serdes(
        struct bloom *bloom1, guchar *md1,
        struct bloom *bloom2, guchar *md2) {

    GString *string = g_string_sized_new(1024);
    Bloom::serialize(string, bloom1);
    Bloom::init(bloom2);

    struct const_buffer buffer = {string->str, string->len};
    CPPUNIT_ASSERT(Bloom::deserialize(bloom2, &buffer));

    CPPUNIT_ASSERT(bloom1->n_hash_funcs == bloom2->n_hash_funcs);
    CPPUNIT_ASSERT(bloom1->data->len == bloom2->data->len);
    CPPUNIT_ASSERT(0 == memcmp(
            bloom1->data->str, bloom2->data->str, bloom2->data->len));

    CPPUNIT_ASSERT(Bloom::contains(bloom2, md1, sizeof (md1)) == TRUE);
    CPPUNIT_ASSERT(Bloom::contains(bloom2, md2, sizeof (md2)) == FALSE);

    g_string_free(string, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void BloomTest::testInitAndFree() {
    bf_init(this->bloom1);
    bf_free(this->bloom1);
}

void BloomTest::testInsertAndContains() {
    bf_init(this->bloom1);
    bf_op(this->bloom1, this->md1, this->md2);
    bf_free(this->bloom1);
}

void BloomTest::testSerializedAndDeserialize() {
    bf_init(this->bloom1);
    bf_init(this->bloom2);

    bf_op(this->bloom1, this->md1, this->md2);
    bf_serdes(this->bloom1, this->md1, this->bloom2, this->md2);

    bf_free(this->bloom2);
    bf_free(this->bloom1);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
