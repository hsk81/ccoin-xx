/*
 * File:   KeysetTest.cpp
 * Author: hsk81
 */

#include "KeysetTest.h"
#include "../include/Key.h"
#include "../include/Util.h"

#include <openssl/ripemd.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION(KeysetTest);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void KeysetTest::test() {
    struct TKey keys[4];

    // generate keys
    for (guint i = 0; i < ARRAY_SIZE(keys); i++) {
        struct TKey *key = &keys[i];
        CPPUNIT_ASSERT(Key::init(key) == TRUE);
        CPPUNIT_ASSERT(Key::generate(key) == TRUE);
    }

    // initialize key set
    struct TKeyset keyset;
    Keyset::init(&keyset);

    // add all but one to key set
    for (guint i = 0; i < (ARRAY_SIZE(keys) - 1); i++) {
        CPPUNIT_ASSERT(Keyset::add(&keyset, &keys[i]) == TRUE);
    }

    // verify all-but-one are in key set
    for (guint i = 0; i < ARRAY_SIZE(keys) - 1; i++) {
        guchar md160[RIPEMD160_DIGEST_LENGTH];
        gpointer pubkey;
        gsize size;

        CPPUNIT_ASSERT(
                Key::get_public(&keys[i], &pubkey, &size) == TRUE);

        Util::Hash160(md160, pubkey, size);

        CPPUNIT_ASSERT(
                Keyset::lookup(&keyset, pubkey, size, TRUE) == FALSE);
        CPPUNIT_ASSERT(
                Keyset::lookup(&keyset, pubkey, size, FALSE) == TRUE);
        CPPUNIT_ASSERT(
                Keyset::lookup(&keyset, md160, sizeof (md160), TRUE) == TRUE);
        CPPUNIT_ASSERT(
                Keyset::lookup(&keyset, md160, sizeof (md160), FALSE) == FALSE);

        free(pubkey);
    }

    // verify last key not in key set
    {
        struct TKey *key = &keys[ARRAY_SIZE(keys) - 1];
        guchar md160[RIPEMD160_DIGEST_LENGTH];
        gpointer pubkey;
        gsize size;

        CPPUNIT_ASSERT(
                Key::get_public(key, &pubkey, &size) == TRUE);

        Util::Hash160(md160, pubkey, size);

        CPPUNIT_ASSERT(
                Keyset::lookup(&keyset, pubkey, size, TRUE) == FALSE);
        CPPUNIT_ASSERT(
                Keyset::lookup(&keyset, pubkey, size, FALSE) == FALSE);
        CPPUNIT_ASSERT(
                Keyset::lookup(&keyset, md160, sizeof (md160), TRUE) == FALSE);
        CPPUNIT_ASSERT(
                Keyset::lookup(&keyset, md160, sizeof (md160), FALSE) == FALSE);

        free(pubkey);
    }

    // free key set
    Keyset::free(&keyset);

    // free keys
    for (guint i = 0; i < ARRAY_SIZE(keys); i++) {
        struct TKey *key = &keys[i];
        Key::free(key);
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
