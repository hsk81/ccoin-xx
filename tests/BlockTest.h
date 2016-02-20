/*
 * File:   BlockTest.h
 * Author: hsk81
 */

#ifndef BLOCK_TEST_H
#define BLOCK_TEST_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class BlockTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(BlockTest);
    CPPUNIT_TEST(testBlock1);
    CPPUNIT_TEST(testBlock2);
    CPPUNIT_TEST_SUITE_END();

public:
    BlockTest() {};
    virtual ~BlockTest() {};
    void setUp() {};
    void tearDown() {};

private:
    void testBlock1();
    void testBlock2();
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif /* BLOCK_TEST_H */
