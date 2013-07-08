/*
 * File:   UtilTest.h
 * Author: hsk81
 *
 * Created on Jul 8, 2013, 8:36:45 PM
 */

#ifndef UTILTEST_H
#define	UTILTEST_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class UtilTest : public CPPUNIT_NS::TestFixture {

    CPPUNIT_TEST_SUITE(UtilTest);
    CPPUNIT_TEST(testReverseCopyMethod);
    CPPUNIT_TEST(testIpv4MappedMethod);
    CPPUNIT_TEST_SUITE_END();

public:
    UtilTest() {};
    virtual ~UtilTest() {};
    void setUp() {};
    void tearDown() {};

private:
    void testReverseCopyMethod();
    void testIpv4MappedMethod();
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif	/* UTILTEST_H */
