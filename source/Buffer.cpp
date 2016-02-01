/* 
 * File:   Buffer.cpp
 * Author: hsk81
 */

#include "../include/Buffer.h"
#include "../include/Util.h"

#include <string.h>
#include <glib.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

guint Buffer::hash(gconstpointer key) {

    const struct buffer *buffer = (struct buffer*) key;
    return Util::Djb2::hash(0x1721, buffer->pointer, buffer->size);
}

gboolean Buffer::equal(gconstpointer lhs, gconstpointer rhs) {

    const struct buffer *lhs_buffer = (struct buffer*) lhs;
    const struct buffer *rhs_buffer = (struct buffer*) rhs;

    if (lhs_buffer->size != rhs_buffer->size) {
        return FALSE;
    }

    return 0 == memcmp(
            lhs_buffer->pointer, rhs_buffer->pointer, lhs_buffer->size);
}

void Buffer::free(struct buffer *buffer) {
    if (!buffer) return;

    ::free(buffer->pointer);
    ::free(buffer);
}

void Buffer::free(gpointer data) {
    Buffer::free((struct buffer*) data);
}

struct buffer* Buffer::copy(gconstpointer data, gsize size) {

    struct buffer *buffer = (struct buffer*) malloc(sizeof (*buffer));
    if (!buffer) goto exit_error;
    buffer->pointer = malloc(size);
    if (!buffer->pointer) goto exit_error_free;

    memcpy(buffer->pointer, data, size);
    buffer->size = size;

    return buffer;
exit_error_free:
    Buffer::free(buffer);
exit_error:
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
