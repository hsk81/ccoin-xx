/*
 * File:   TestLib.cpp
 * Author: hsk81
 */

#include "./TestLib.h"
#include <glib.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define TEST_SRCDIR "./tests"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

gchar *TestLib::filename(const gchar *basename) {

    return g_strdup_printf("%s/%s", TEST_SRCDIR, basename);
}

json_t *TestLib::read_json(const gchar *filename) {

    json_error_t _error;
    return json_load_file(filename, JSON_REJECT_DUPLICATES, &_error);
}

void TestLib::dumphex(const gchar *prefix, gconstpointer pointer, gsize size) {

    if (prefix) {
        fprintf(stderr, "%s: ", prefix);
    }

    const guchar *_puch = (guchar*)pointer;
    for (guint _i = 0; _i < size; _i++) {
        fprintf(stderr, "%02x", _puch[_i]);
    }

    fprintf(stderr, "\n");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
