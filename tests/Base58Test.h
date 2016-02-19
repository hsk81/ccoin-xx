/*
 * File:   Base58Test.h
 * Author: hsk81
 */

#ifndef BASE58TEST_H
#define BASE58TEST_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
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
    void encode(
            const gchar *pch_hex,
            const gchar *pch_b58);
    void decode(
            const gchar *pch_hex,
            const gchar *pch_b58);

private:
    void testEncodeMethod();
    void testDecodeMethod();
    void testEncodeAndDecodeMethod();

private:
    void checkPrivateKeyEnc(
            const gchar *pch_b58,
            GString *payload,
            gboolean do_compress,
            gboolean is_testnet);
    void checkPrivateKeyDec(
            const gchar *pch_b58,
            GString *payload,
            gboolean do_compress,
            gboolean is_testnet);

    void checkPublicKeyEnc(
            const gchar *pch_b58,
            GString *payload,
            const gchar *address_type,
            gboolean is_testnet);
    void checkPublicKeyDec(
            const gchar *pch_b58,
            GString *payload,
            const gchar *address_type,
            gboolean is_testnet);

private:
    void testKeysMethod();
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif /* BASE58TEST_H */
