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

void HexCode::encode(gchar *hex_string, gconstpointer pointer, gsize length) {

    const guchar *puch = (guchar*) pointer;
    for (guint i = 0; i < length; i++) {
        guchar v, n1, n2;

        v = puch[i];
        n1 = v >> 4;
        n2 = v & 0xf;

        *hex_string++ = hexdigit[n1];
        *hex_string++ = hexdigit[n2];
    }

    *hex_string = 0;
}

gboolean HexCode::decode(
        gpointer pointer, gsize max_length,
        const gchar *hex_string, gsize *hex_length) {

    if (!pointer || !hex_string)
        return FALSE;
    if (!strncmp(hex_string, "0x", 2))
        hex_string += 2;
    if (strlen(hex_string) > (max_length * 2))
        return FALSE;

    guchar *buffer = (guchar*) pointer;
    gsize length = 0;

    while (*hex_string) {
        guchar c1 = (guchar) hex_string[0];
        guchar v1 = hexdigit_val[c1];
        if (!v1 && (c1 != '0')) return FALSE;

        guchar c2 = (guchar) hex_string[1];
        guchar v2 = hexdigit_val[c2];
        if (!v2 && (c2 != '0')) return FALSE;

        *buffer = (v1 << 4) | v2;

        length++;
        buffer++;
        hex_string += 2;
    }

    if (hex_length) {
        *hex_length = length;
    }

    return TRUE;
}

GString *HexCode::to_string(const gchar *hex_string) {

    if (!hex_string || !*hex_string) return NULL;
    if (!strncmp(hex_string, "0x", 2)) hex_string += 2;

    gsize length = strlen(hex_string) / 2;
    GString *string = g_string_sized_new(length);
    g_string_set_size(string, length);
    memset(string->str, 0, length);

    gsize out_length = 0;
    gboolean rc = HexCode::decode(string->str, length, hex_string, &out_length);

    if (!rc || (length != out_length)) {
        g_string_free(string, TRUE);
        return NULL;
    }

    return string;
}

gboolean HexCode::is_hex(const gchar *hex_string, gboolean prefix) {
    throw "not implemented";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
