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
    void bytes(GString *s, const void *p, size_t len);
    void u16(GString *s, uint16_t v_);
    void u32(GString *s, uint32_t v_);
    void u64(GString *s, uint64_t v_);

    void varlen(GString *s, uint32_t vlen);
    void varstr(GString *s, GString *s_in);
}

namespace Deserialize {
    bool bytes(void *po, struct const_buffer *buf, size_t len);
    bool u16(uint16_t *vo, struct const_buffer *buf);
    bool u32(uint32_t *vo, struct const_buffer *buf);
    bool u64(uint64_t *vo, struct const_buffer *buf);

    bool varlen(uint32_t *lo, struct const_buffer *buf);
    bool varstr(GString **so, struct const_buffer *buf);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif	/* SERIALIZE_H */
