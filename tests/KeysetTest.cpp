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

    // initialize keyset
    struct TKeyset keyset;
    Keyset::init(&keyset);

    // add all but one to keyset
    for (guint i = 0; i < (ARRAY_SIZE(keys) - 1); i++) {
        CPPUNIT_ASSERT(Keyset::add(&keyset, &keys[i]) == TRUE);
    }

    // verify all-but-one are in keyset
    for (guint i = 0; i < ARRAY_SIZE(keys) - 1; i++) {
        guchar md160[RIPEMD160_DIGEST_LENGTH];
        gpointer pubkey;
        gsize length;
        gboolean result;

        result = Key::get_public(&keys[i], &pubkey, &length);
        CPPUNIT_ASSERT(result == TRUE);

        Util::Hash160(md160, pubkey, length);

        result = Keyset::lookup(&keyset, pubkey, length, TRUE);
        CPPUNIT_ASSERT(result == FALSE);
        result = Keyset::lookup(&keyset, pubkey, length, FALSE);
        CPPUNIT_ASSERT(result == TRUE);

        result = Keyset::lookup(&keyset, md160, sizeof (md160), TRUE);
        CPPUNIT_ASSERT(result == TRUE);
        result = Keyset::lookup(&keyset, md160, sizeof (md160), FALSE);
        CPPUNIT_ASSERT(result == FALSE);

        free(pubkey);
    }

    // verify last key not in keyset
    {
        struct TKey *key = &keys[ARRAY_SIZE(keys) - 1];
        guchar md160[RIPEMD160_DIGEST_LENGTH];
        gpointer pubkey;
        gsize pklen;
        gboolean result;

        result = Key::get_public(key, &pubkey, &pklen);
        CPPUNIT_ASSERT(result == TRUE);

        Util::Hash160(md160, pubkey, pklen);

        result = Keyset::lookup(&keyset, pubkey, pklen, TRUE);
        CPPUNIT_ASSERT(result == FALSE);
        result = Keyset::lookup(&keyset, pubkey, pklen, FALSE);
        CPPUNIT_ASSERT(result == FALSE);

        result = Keyset::lookup(&keyset, md160, sizeof (md160), TRUE);
        CPPUNIT_ASSERT(result == FALSE);
        Keyset::lookup(&keyset, md160, sizeof (md160), FALSE);
        CPPUNIT_ASSERT(result == FALSE);

        free(pubkey);
    }

    // free keyset
    Keyset::free(&keyset);

    // free keys
    for (guint i = 0; i < ARRAY_SIZE(keys); i++) {
        struct TKey *key = &keys[i];
        Key::free(key);
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
