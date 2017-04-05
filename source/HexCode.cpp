/*
 * File:   HexCode.cpp
 * Author: hsk81
 */

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "../include/HexCode.h"
#include <string.h>
#include <exception>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static const guchar hexdigit_val[256] = {
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, //008
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, //016
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, //024
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, //032
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, //040
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, //048
    0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, //056: 0-7
    0x8, 0x9, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, //064: 8,9
    0x0, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x0, //072: a-f
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, //080
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, //088
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, //096
    0x0, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x0, //104: A-F
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, //112
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, //120
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, //128
};

static const gchar hexdigit[] = "0123456789abcdef";

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void HexCode::encode(gchar *hex_chars, gconstpointer pointer, gsize size) {

    const guchar *puch = (guchar*) pointer;
    for (guint i = 0; i < size; i++) {
        guchar v, n1, n2;

        v = puch[i];
        n1 = v >> 4;
        n2 = v & 0xf;

        *hex_chars++ = hexdigit[n1];
        *hex_chars++ = hexdigit[n2];
    }

    *hex_chars = 0;
}

gboolean HexCode::decode(
        gpointer pointer, gsize max_size,
        const gchar *hex_chars, gsize *hex_size) {

    if (!pointer || !hex_chars)
        return FALSE;
    if (!strncmp(hex_chars, "0x", 2))
        hex_chars += 2;
    if (strlen(hex_chars) > (max_size * 2))
        return FALSE;

    guchar *buffer = (guchar*) pointer;
    gsize size = 0;

    while (*hex_chars) {
        guchar c1 = (guchar) hex_chars[0];
        guchar v1 = hexdigit_val[c1];
        if (!v1 && (c1 != '0'))
            return FALSE;

        guchar c2 = (guchar) hex_chars[1];
        guchar v2 = hexdigit_val[c2];
        if (!v2 && (c2 != '0'))
            return FALSE;

        *buffer = (v1 << 4) | v2;

        size++;
        buffer++;
        hex_chars += 2;
    }

    if (hex_size) {
        *hex_size = size;
    }

    return TRUE;
}

GString *HexCode::to_string(const gchar *hex_chars) {

    if (!hex_chars || !*hex_chars)
        return NULL;
    if (!strncmp(hex_chars, "0x", 2))
        hex_chars += 2;

    gsize size = strlen(hex_chars) / 2;
    GString *string = g_string_sized_new(size);
    g_string_set_size(string, size);
    memset(string->str, 0, size);

    gsize out_size = 0;
    gboolean rc = HexCode::decode(string->str, size, hex_chars, &out_size);

    if (!rc || (size != out_size)) {
        g_string_free(string, TRUE);
        return NULL;
    }

    return string;
}

GString *from_string(gconstpointer pointer, gsize size) {
    throw "not implemented";
}


gboolean HexCode::is_hex(const gchar *hex_chars, gboolean prefix) {
    throw "not implemented";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
