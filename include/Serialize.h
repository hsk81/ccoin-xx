/* 
 * File:   Serialize.h
 * Author: hsk81
 */

#ifndef SERIALIZE_H
#define SERIALIZE_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "Buffer.h"
#include <glib.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

namespace Serialize {
    void bytes(GString *string, gconstpointer pointer, gsize size);
    void u16(GString *string, guint16 value);
    void u32(GString *string, guint32 value);
    void u64(GString *string, guint64 value);

    void var_size(GString *string, guint32 size);
    void var_string(GString *string, GString *string_in);
}

namespace Deserialize {
    gboolean bytes(void *pointer, struct const_buffer *buffer, gsize size);
    gboolean u16(guint16 *value, struct const_buffer *buffer);
    gboolean u32(guint32 *value, struct const_buffer *buffer);
    gboolean u64(guint64 *value, struct const_buffer *buffer);

    gboolean var_size(guint32 *size, struct const_buffer *buffer);
    gboolean var_string(GString **string, struct const_buffer *buffer);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif /* SERIALIZE_H */
