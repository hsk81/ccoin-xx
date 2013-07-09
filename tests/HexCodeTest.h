/*
 * File:   HexCodeTest.h
 * Author: hsk81
 *
 * Created on Jul 9, 2013, 10:27:13 AM
 */

#ifndef HEXCODETEST_H
#define	HEXCODETEST_H

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
    void testDecode1Method();
    void testDecode2Method();
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif	/* HEXCODETEST_H */