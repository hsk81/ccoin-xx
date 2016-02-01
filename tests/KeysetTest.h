/*
 * File:   KeysetTest.h
 * Author: hsk81
 *
 * Created on Feb 1, 2014, 1:30:35 PM
 */

#ifndef KEYSETTEST_H
#define KEYSETTEST_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class KeysetTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(KeysetTest);
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

public:
    KeysetTest() {};
    virtual ~KeysetTest() {};
    void setUp() {};
    void tearDown() {};

private:
    void test();
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif /* KEYSETTEST_H */
