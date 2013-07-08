/*
 * File:   Base58Test.cpp
 * Author: hsk81
 *
 * Created on Jul 7, 2013, 8:00:00 PM
 */

#include "./TestLib.h"
#include <glib.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define TEST_SRCDIR "./tests"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

char *TestLib::filename(const char *basename) {

    return g_strdup_printf("%s/%s", TEST_SRCDIR, basename);
}

json_t *TestLib::read_json(const char *filename) {

    json_error_t err;
    json_t *ret = json_load_file(filename, JSON_REJECT_DUPLICATES, &err);    
    return ret;
}

void TestLib::dumphex(const char *prefix, const void *pointer, size_t length) {

    if (prefix)
        fprintf(stderr, "%s: ", prefix);

    const unsigned char *p = (unsigned char*)pointer;
    for (unsigned int i = 0; i < length; i++) {
        fprintf(stderr, "%02x", p[i]);
    }

    fprintf(stderr, "\n");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
