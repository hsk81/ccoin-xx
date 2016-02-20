/* 
 * File:   Serialize.cpp
 * Author: hsk81
 */

#include "../include/Serialize.h"
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#pragma GCC diagnostic ignored "-Wpointer-arith"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Serialize::bytes(GString *string, gconstpointer pointer, gsize size) {
    g_string_append_len(string, (const gchar*) pointer, size);
}

void Serialize::u16(GString *string, guint16 value) {
    guint16 _value = GUINT16_TO_LE(value);
    g_string_append_len(string, (gchar*) & _value, sizeof (_value));
}

void Serialize::u32(GString *string, guint32 value) {
    guint32 _value = GUINT32_TO_LE(value);
    g_string_append_len(string, (gchar*) & _value, sizeof (_value));
}

void Serialize::u64(GString *string, guint64 value) {
    guint64 _value = GUINT64_TO_LE(value);
    g_string_append_len(string, (gchar*) & _value, sizeof (_value));
}

///////////////////////////////////////////////////////////////////////////////

void Serialize::var_size(GString *string, guint32 size) {

    if (size < 253) {
        guchar uch = size;
        Serialize::bytes(string, &uch, 1);
    } else if (size < 0x10000) {
        guchar uch = 253;
        Serialize::bytes(string, &uch, 1);
        Serialize::u16(string, (guint16) size);
    } else {
        guchar uch = 254;
        Serialize::bytes(string, &uch, 1);
        Serialize::u32(string, size);
    }

    // u64 case intentionally not implemented
}

void Serialize::var_string(GString *string, GString *string_in) {

    if (!string_in || !string_in->len) {
        Serialize::var_size(string, 0);
        return;
    }

    Serialize::var_size(string, string_in->len);
    Serialize::bytes(string, string_in->str, string_in->len);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

gboolean Deserialize::bytes(
        gpointer pointer, struct TConstantBuffer *buffer, gsize size) {

    if (buffer->size < size) {
        return FALSE;
    }

    memcpy(pointer, buffer->pointer, size);
    buffer->pointer += size;
    buffer->size -= size;

    return TRUE;
}

gboolean Deserialize::u16(guint16 *value, struct TConstantBuffer *buffer) {

    guint16 _value;
    if (!Deserialize::bytes(&_value, buffer, sizeof (_value))) return FALSE;
    *value = GUINT16_FROM_LE(_value);

    return TRUE;
}

gboolean Deserialize::u32(guint32 *value, struct TConstantBuffer *buffer) {

    guint32 _value;
    if (!Deserialize::bytes(&_value, buffer, sizeof (_value))) return FALSE;
    *value = GUINT32_FROM_LE(_value);

    return TRUE;
}

gboolean Deserialize::u64(guint64 *value, struct TConstantBuffer *buffer) {

    guint64 _value;
    if (!Deserialize::bytes(&_value, buffer, sizeof (_value))) return FALSE;
    *value = GUINT64_FROM_LE(_value);

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

gboolean Deserialize::var_size(guint32 *size, struct TConstantBuffer *buffer) {

    guint32 _size;
    guchar _uch;
    if (!Deserialize::bytes(&_uch, buffer, 1)) return FALSE;

    if (_uch == 253) {
        guint16 v16;
        if (!Deserialize::u16(&v16, buffer)) return FALSE;
        _size = v16;
    } else if (_uch == 254) {
        guint32 v32;
        if (!Deserialize::u32(&v32, buffer)) return FALSE;
        _size = v32;
    } else if (_uch == 255) {
        guint64 v64;
        if (!Deserialize::u64(&v64, buffer)) return FALSE;
        _size = (guint32) v64; // WARNING: truncate!
    } else {
        _size = _uch;
    }

    *size = _size;
    return TRUE;
}

gboolean Deserialize::var_string(
        GString **string, struct TConstantBuffer *buffer) {

    if (*string) {
        g_string_free(*string, TRUE);
        *string = NULL;
    }

    guint32 _size;
    if (!Deserialize::var_size(&_size, buffer)) return FALSE;
    if (buffer->size < _size) return FALSE;

    GString *_string = g_string_sized_new(_size);
    g_string_append_len(_string, (const gchar*) buffer->pointer, _size);

    buffer->pointer += _size;
    buffer->size -= _size;

    *string = _string;
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
