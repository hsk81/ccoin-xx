/*
 * File:   KeysetTest.cpp
 * Author: hsk81
 *
 * Created on Feb 1, 2014, 1:30:35 PM
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

	struct key keys[4];

	// generate keys
	for (unsigned int i = 0; i < ARRAY_SIZE(keys); i++) {
		struct key *key = &keys[i];
		CPPUNIT_ASSERT(Key::init(key) == true);
		CPPUNIT_ASSERT(Key::generate(key) == true);
	}

	// initialize keyset
	struct keyset keyset;
	Keyset::init(&keyset);

	// add all but one to keyset
	for (unsigned int i = 0; i < (ARRAY_SIZE(keys) - 1); i++) {
		CPPUNIT_ASSERT(Keyset::add(&keyset, &keys[i]) == true);
    }

	// verify all-but-one are in keyset
	for (unsigned int i = 0; i < ARRAY_SIZE(keys) - 1; i++) {
		unsigned char md160[RIPEMD160_DIGEST_LENGTH];
		void *pubkey; size_t length; bool result;

        result = Key::get_public(&keys[i], &pubkey, &length);
		CPPUNIT_ASSERT(result == true);
        
        Util::Hash160(md160, pubkey, length);

        result = Keyset::lookup(&keyset, pubkey, length, true);
		CPPUNIT_ASSERT(result == false);
        result = Keyset::lookup(&keyset, pubkey, length, false);
		CPPUNIT_ASSERT(result == true);

        result = Keyset::lookup(&keyset, md160, sizeof(md160), true);
		CPPUNIT_ASSERT(result == true);
        result = Keyset::lookup(&keyset, md160, sizeof(md160), false);
		CPPUNIT_ASSERT(result == false);
        
		free(pubkey);
	}

	// verify last key not in keyset
	{
		struct key *key = &keys[ARRAY_SIZE(keys) - 1];
		unsigned char md160[RIPEMD160_DIGEST_LENGTH];
		void *pubkey; size_t pklen; bool result;
        
        result = Key::get_public(key, &pubkey, &pklen);
		CPPUNIT_ASSERT(result == true);

		Util::Hash160(md160, pubkey, pklen);

        result = Keyset::lookup(&keyset, pubkey, pklen, true);
		CPPUNIT_ASSERT(result == false);
        result = Keyset::lookup(&keyset, pubkey, pklen, false);
		CPPUNIT_ASSERT(result == false);

        result = Keyset::lookup(&keyset, md160, sizeof(md160), true);
		CPPUNIT_ASSERT(result == false);
        Keyset::lookup(&keyset, md160, sizeof(md160), false);
		CPPUNIT_ASSERT(result == false);

		free(pubkey);
	}
    
	// free keyset
    Keyset::free(&keyset);

    // free keys
	for (unsigned int i = 0; i < ARRAY_SIZE(keys); i++) {
		struct key *key = &keys[i];
		Key::free(key);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
