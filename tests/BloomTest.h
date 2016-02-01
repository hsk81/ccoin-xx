/*
 * File:   BloomTest.h
 * Author: hsk81
 *
 * Created on Sep 19, 2013, 9:05:40 AM
 */

#ifndef BLOOMTEST_H
#define BLOOMTEST_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include "../include/Bloom.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class BloomTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(BloomTest);
    CPPUNIT_TEST(testInitAndFree);
    CPPUNIT_TEST(testInsertAndContains);
    CPPUNIT_TEST(testSerializedAndDeserialize);
    CPPUNIT_TEST_SUITE_END();

public:
    BloomTest();
    virtual ~BloomTest();
    void setUp();
    void tearDown();

private:
    void testInitAndFree();
    void testInsertAndContains();
    void testSerializedAndDeserialize();

private:
    struct bloom *bloom1, *bloom2;
    unsigned char *md1, *md2;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif /* BLOOMTEST_H */