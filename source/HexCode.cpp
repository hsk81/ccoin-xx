/*
 * File:   HexCode.cpp
 * Author: hsk81
 *
 * Created on Jul 7, 2013, 5:20:15 PM
 */

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "../include/HexCode.h"

#include <string.h>
#include <stdbool.h>
#include <glib.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static const unsigned char hexdigit_val[256] = {
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

static const char hexdigit[] = "0123456789abcdef";

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void HexCode::encode(char* hex_string, const void* pointer, size_t length) {

    const unsigned char *p = (unsigned char*)pointer;
    for (unsigned int i = 0; i < length; i++) {
        unsigned char v, n1, n2;

        v = p[i];
        n1 = v >> 4;
        n2 = v & 0xf;

        *hex_string++ = hexdigit[n1];
        *hex_string++ = hexdigit[n2];
    }

    *hex_string = 0;
}

bool HexCode::decode(void* pointer, size_t max_len, const char* hex_string,
        size_t* out_length) {

    if (!pointer || !hex_string)
        return false;
    if (!strncmp(hex_string, "0x", 2))
        hex_string += 2;
    if (strlen(hex_string) > (max_len * 2))
        return false;

    unsigned char *buf = (unsigned char*)pointer;
    size_t out_len = 0;

    while (*hex_string) {
        unsigned char c1 = (unsigned char) hex_string[0];
        unsigned char c2 = (unsigned char) hex_string[1];

        unsigned char v1 = hexdigit_val[c1];
        unsigned char v2 = hexdigit_val[c2];

        if (!v1 && (c1 != '0'))
            return false;
        if (!v2 && (c2 != '0'))
            return false;

        *buf = (v1 << 4) | v2;

        out_len++;
        buf++;
        hex_string += 2;
    }

    if (out_length)
        *out_length = out_len;
    return true;
}

GString *HexCode::to_string(const char* hex_string) {
    
    if (!hex_string || !*hex_string) return NULL;
    if (!strncmp(hex_string, "0x", 2)) hex_string += 2;

    size_t slen = strlen(hex_string) / 2;
    GString *s = g_string_sized_new(slen);
    g_string_set_size(s, slen);
    memset(s->str, 0, slen);

    size_t outlen = 0;
    bool rc = HexCode::decode(s->str, slen, hex_string, &outlen);

    if (!rc || (slen != outlen)) {
        g_string_free(s, TRUE);
        return NULL;
    }

    return s;
}

bool HexCode::is_hex(const char* hex_string, bool require_prefix) {
    
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
