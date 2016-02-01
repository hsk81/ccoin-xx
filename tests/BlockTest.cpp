/*
 * File:   BlockTest.cpp
 * Author: hsk81
 *
 * Created on Feb 2, 2014, 6:01:07 PM
 */

#include "BlockTest.h"
#include "TestLib.h"

#include "../include/Util.h"
#include "../include/Message.h"
#include <jansson.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION(BlockTest);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void runTest(const char *filename_json, const char *filename_ser) {

    char* path_json = TestLib::filename(filename_json);
    json_t *meta = TestLib::read_json(path_json);
    CPPUNIT_ASSERT(json_is_object(meta));

    char* path_ser = TestLib::filename(filename_ser);
    int fd_ser = Util::open_file(path_ser);
    if (fd_ser < 0) {
        perror(path_ser);
        exit(1);
    }

    struct p2p_message msg = {};

    free(path_json);
    free(path_ser);
    json_decref(meta);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void BlockTest::testBlock1() {
    runTest("Block.000000.json", "Block.000000.ser");
}

void BlockTest::testBlock2() {
    runTest("Block.120383.json", "Block.120383.ser");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
