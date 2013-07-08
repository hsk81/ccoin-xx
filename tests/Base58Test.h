/*
 * File:   Base58Test.h
 * Author: hsk81
 *
 * Created on Jul 6, 2013, 8:51:25 AM
 */

#ifndef BASE58TEST_H
#define	BASE58TEST_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <stdbool.h>
#include <glib.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class Base58Test : public CPPUNIT_NS::TestFixture {

    CPPUNIT_TEST_SUITE(Base58Test);
    CPPUNIT_TEST(testEncodeMethod);
    CPPUNIT_TEST(testDecodeMethod);
    CPPUNIT_TEST(testEncodeAndDecodeMethod);
    CPPUNIT_TEST(testKeysMethod);
    CPPUNIT_TEST_SUITE_END();

public:
    Base58Test() {};
    virtual ~Base58Test() {};
    void setUp() {};
    void tearDown() {};

private:
    void encode(const char *hex_string, const char *b58_string);
    void decode(const char *hex_string, const char *b58_string);    
private:
    void testEncodeMethod();
    void testDecodeMethod();
    void testEncodeAndDecodeMethod();    

private:
    void checkPrivateKeyEnc(const char *b58_string, GString *payload, 
            bool compress, bool is_testnet);
    void checkPrivateKeyDec(const char *b58_string, GString *payload,
            bool compress, bool is_testnet);

    void checkPublicKeyEnc(const char *b58_string, GString *payload,
            const char *address_type_string, bool is_testnet);
    void checkPublicKeyDec(const char *b58_string, GString *payload,
            const char *address_type_string, bool is_testnet);
private:
    void testKeysMethod();
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif	/* BASE58TEST_H */
