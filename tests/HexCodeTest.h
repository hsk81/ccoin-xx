/*
 * File:   HexCodeTest.h
 * Author: hsk81
 */

#ifndef HEX_CODE_TEST_H
#define HEX_CODE_TEST_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class HexCodeTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(HexCodeTest);
    CPPUNIT_TEST(testEncode1Method);
    CPPUNIT_TEST(testEncode2Method);
    CPPUNIT_TEST(testDecode1Method);
    CPPUNIT_TEST(testDecode2Method);
    CPPUNIT_TEST_SUITE_END();

public:
    HexCodeTest() {};
    virtual ~HexCodeTest() {};
    void setUp() {};
    void tearDown() {};

private:
    void testEncode1Method();
    void testEncode2Method();
    void testDecode1Method(); //req-before: testEncode{1,2}Method!
    void testDecode2Method(); //req-before: testEncode{1,2}Method!
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif /* HEX_CODE_TEST_H */
