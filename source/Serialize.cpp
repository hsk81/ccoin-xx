/* 
 * File:   Serialize.cpp
 * Author: hsk81
 * 
 * Created on September 19, 2013, 2:22 PM
 */

#include "../include/Serialize.h"
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#pragma GCC diagnostic ignore "-Wpointer-arith"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Serialize::bytes(GString *g_string, const void *pointer, size_t length) {
	g_string_append_len(g_string, (const gchar*)pointer, length);
}

void Serialize::u16(GString *g_string, uint16_t value) {
	uint16_t v = GUINT16_TO_LE(value);
	g_string_append_len(g_string, (gchar*)&v, sizeof(v));
}

void Serialize::u32(GString *g_string, uint32_t value) {
	uint32_t v = GUINT32_TO_LE(value);
	g_string_append_len(g_string, (gchar*)&v, sizeof(v));
}

void Serialize::u64(GString *g_string, uint64_t value) {
	uint64_t v = GUINT64_TO_LE(value);
	g_string_append_len(g_string, (gchar*)&v, sizeof(v));
}

///////////////////////////////////////////////////////////////////////////////

void Serialize::varlen(GString *g_string, uint32_t length) {

	if (length < 253) {
		unsigned char uch = length;
		Serialize::bytes(g_string, &uch, 1);
	} else if (length < 0x10000) {
		unsigned char uch = 253;
		Serialize::bytes(g_string, &uch, 1);
		Serialize::u16(g_string, (uint16_t) length);
	} else {
		unsigned char uch = 254;
		Serialize::bytes(g_string, &uch, 1);
		Serialize::u32(g_string, length);
	}

	// u64 case intentionally not implemented
}

void Serialize::varstr(GString *g_string, GString *g_string_in) {

	if (!g_string_in || !g_string_in->len) {
		Serialize::varlen(g_string, 0);
		return;
	}

	Serialize::varlen(g_string, g_string_in->len);
	Serialize::bytes(g_string, g_string_in->str, g_string_in->len);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool Deserialize::bytes(void *pointer, struct const_buffer *buffer,
        size_t length) {

	if (buffer->length < length)
        return false;

	memcpy(pointer, buffer->pointer, length);
	buffer->pointer += length;
	buffer->length -= length;

	return true;
}

bool Deserialize::u16(uint16_t *value, struct const_buffer *buffer) {
	uint16_t v; if (!Deserialize::bytes(&v, buffer, sizeof(v))) return false;
	*value = GUINT16_FROM_LE(v);
	return true;
}

bool Deserialize::u32(uint32_t *value, struct const_buffer *buffer) {
	uint32_t v; if (!Deserialize::bytes(&v, buffer, sizeof(v))) return false;
	*value = GUINT32_FROM_LE(v);
	return true;
}

bool Deserialize::u64(uint64_t *value, struct const_buffer *buffer) {
	uint64_t v; if (!Deserialize::bytes(&v, buffer, sizeof(v))) return false;
	*value = GUINT64_FROM_LE(v);
	return true;
}

///////////////////////////////////////////////////////////////////////////////

bool Deserialize::varlen(uint32_t *length, struct const_buffer *buffer) {

	uint32_t len;
	unsigned char uch;
	if (!Deserialize::bytes(&uch, buffer, 1)) return false;

	if (uch == 253) {
		uint16_t v16;
		if (!Deserialize::u16(&v16, buffer)) return false;
		len = v16;
	} else if (uch == 254) {
		uint32_t v32;
		if (!Deserialize::u32(&v32, buffer)) return false;
		len = v32;
	} else if (uch == 255) {
		uint64_t v64;
		if (!Deserialize::u64(&v64, buffer)) return false;
		len = (uint32_t) v64; // WARNING: truncate!
	} else {
		len = uch;
    }

	*length = len;
	return true;
}

bool Deserialize::varstr(GString **g_string, struct const_buffer *buffer) {
    
	if (*g_string) {
		g_string_free(*g_string, TRUE);
		*g_string = NULL;
	}

	uint32_t len;
	if (!Deserialize::varlen(&len, buffer)) return false;
	if (buffer->length < len) return false;

	GString *s = g_string_sized_new(len);
	g_string_append_len(s, (const gchar*)buffer->pointer, len);

	buffer->pointer += len;
	buffer->length -= len;

	*g_string = s;
	return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
