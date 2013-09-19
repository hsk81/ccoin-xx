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

void Serialize::bytes(GString *s, const void *p, size_t len) {
	g_string_append_len(s, (const gchar*)p, len);
}

void Serialize::u16(GString *s, uint16_t v_) {
	uint16_t v = GUINT16_TO_LE(v_);
	g_string_append_len(s, (gchar*)&v, sizeof(v));
}

void Serialize::u32(GString *s, uint32_t v_) {
	uint32_t v = GUINT32_TO_LE(v_);
	g_string_append_len(s, (gchar*)&v, sizeof(v));
}

void Serialize::u64(GString *s, uint64_t v_) {
	uint64_t v = GUINT64_TO_LE(v_);
	g_string_append_len(s, (gchar*)&v, sizeof(v));
}

///////////////////////////////////////////////////////////////////////////////

void Serialize::varlen(GString *s, uint32_t vlen) {

	if (vlen < 253) {
		unsigned char uch = vlen;
		Serialize::bytes(s, &uch, 1);
	} else if (vlen < 0x10000) {
		unsigned char uch = 253;
		Serialize::bytes(s, &uch, 1);
		Serialize::u16(s, (uint16_t) vlen);
	} else {
		unsigned char uch = 254;
		Serialize::bytes(s, &uch, 1);
		Serialize::u32(s, vlen);
	}

	// u64 case intentionally not implemented
}

void Serialize::varstr(GString *s, GString *s_in) {

	if (!s_in || !s_in->len) {
		Serialize::varlen(s, 0);
		return;
	}

	Serialize::varlen(s, s_in->len);
	Serialize::bytes(s, s_in->str, s_in->len);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool Deserialize::bytes(void *po, struct const_buffer *buf, size_t len) {
	if (buf->length < len) return false;

	memcpy(po, buf->pointer, len);
	buf->pointer += len;
	buf->length -= len;

	return true;
}

bool Deserialize::u16(uint16_t *vo, struct const_buffer *buf) {
	uint16_t v; if (!Deserialize::bytes(&v, buf, sizeof(v))) return false;
	*vo = GUINT16_FROM_LE(v);
	return true;
}

bool Deserialize::u32(uint32_t *vo, struct const_buffer *buf) {
	uint32_t v; if (!Deserialize::bytes(&v, buf, sizeof(v))) return false;
	*vo = GUINT32_FROM_LE(v);
	return true;
}

bool Deserialize::u64(uint64_t *vo, struct const_buffer *buf) {
	uint64_t v; if (!Deserialize::bytes(&v, buf, sizeof(v))) return false;
	*vo = GUINT64_FROM_LE(v);
	return true;
}

///////////////////////////////////////////////////////////////////////////////

bool Deserialize::varlen(uint32_t *lo, struct const_buffer *buf) {

	uint32_t len;
	unsigned char c;
	if (!Deserialize::bytes(&c, buf, 1)) return false;

	if (c == 253) {
		uint16_t v16;
		if (!Deserialize::u16(&v16, buf)) return false;
		len = v16;
	} else if (c == 254) {
		uint32_t v32;
		if (!Deserialize::u32(&v32, buf)) return false;
		len = v32;
	} else if (c == 255) {
		uint64_t v64;
		if (!Deserialize::u64(&v64, buf)) return false;
		len = (uint32_t) v64; // WARNING: truncate!
	} else {
		len = c;
    }

	*lo = len;
	return true;
}

bool Deserialize::varstr(GString **so, struct const_buffer *buf) {
    
	if (*so) {
		g_string_free(*so, TRUE);
		*so = NULL;
	}

	uint32_t len;
	if (!Deserialize::varlen(&len, buf)) return false;
	if (buf->length < len) return false;

	GString *s = g_string_sized_new(len);
	g_string_append_len(s, (const gchar*)buf->pointer, len);

	buf->pointer += len;
	buf->length -= len;

	*so = s;
	return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
