/* 
 * File:   Serialize.h
 * Author: hsk81
 *
 * Created on September 19, 2013, 2:22 PM
 */

#ifndef SERIALIZE_H
#define	SERIALIZE_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "Buffer.h"

#include <glib.h>
#include <stdint.h>
#include <stdbool.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

namespace Serialize {
    void bytes(GString *g_string, const void *pointer, size_t length);
    void u16(GString *g_string, uint16_t value);
    void u32(GString *g_string, uint32_t value);
    void u64(GString *g_string, uint64_t value);

    void varlen(GString *g_string, uint32_t length);
    void varstr(GString *g_string, GString *g_string_in);
}

namespace Deserialize {
    bool bytes(void *pointer, struct const_buffer *buffer, size_t length);
    bool u16(uint16_t *value, struct const_buffer *buffer);
    bool u32(uint32_t *value, struct const_buffer *buffer);
    bool u64(uint64_t *value, struct const_buffer *buffer);

    bool varlen(uint32_t *length, struct const_buffer *buffer);
    bool varstr(GString **g_string, struct const_buffer *buffer);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif	/* SERIALIZE_H */
