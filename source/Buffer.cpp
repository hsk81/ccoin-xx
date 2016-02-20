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

    const struct TBuffer *buffer = (struct TBuffer*) key;
    return Util::Djb2::hash(0x1721, buffer->pointer, buffer->size);
}

gboolean Buffer::equal(
        gconstpointer lhs_pointer, gconstpointer rhs_pointer) {

    const struct TBuffer *lhs_buffer = (struct TBuffer*) lhs_pointer;
    const struct TBuffer *rhs_buffer = (struct TBuffer*) rhs_pointer;

    if (lhs_buffer->size != rhs_buffer->size) {
        return FALSE;
    }

    return 0 == memcmp(
            lhs_buffer->pointer, rhs_buffer->pointer, lhs_buffer->size);
}

void Buffer::free(struct TBuffer *buffer) {
    if (!buffer) return;

    ::free(buffer->pointer);
    ::free(buffer);
}

void Buffer::free(gpointer pointer) {
    Buffer::free((struct TBuffer*) pointer);
}

struct TBuffer* Buffer::copy(gconstpointer pointer, gsize size) {

    struct TBuffer *buffer = (struct TBuffer*) malloc(sizeof (*buffer));
    if (!buffer) goto exit_error;
    buffer->pointer = malloc(size);
    if (!buffer->pointer) goto exit_error_free;

    memcpy(buffer->pointer, pointer, size);
    buffer->size = size;

    return buffer;
exit_error_free:
    Buffer::free(buffer);
exit_error:
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
