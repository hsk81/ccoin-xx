/* 
 * File:   Buffer.cpp
 * Author: hsk81
 * 
 * Created on September 19, 2013, 12:46 PM
 */

#include "../include/Buffer.h"
#include "../include/Util.h"

#include <stdlib.h>
#include <string.h>
#include <glib.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

unsigned int Buffer::g_hash(const void *key) {

	const struct buffer *buffer = (struct buffer*)key;
	return Util::Djb2::hash(0x1721, buffer->pointer, buffer->length);
}

bool Buffer::g_equal(const void *lhs, const void *rhs) {
    
	const struct buffer *lhs_buffer = (struct buffer*)lhs;
	const struct buffer *rhs_buffer = (struct buffer*)rhs;

	if (lhs_buffer->length != rhs_buffer->length) {
		return FALSE;
    }

	return 0 == memcmp(
        lhs_buffer->pointer, rhs_buffer->pointer, lhs_buffer->length
    );
}

void Buffer::free(struct buffer *buffer) {
	if (!buffer) return;

	::free(buffer->pointer);
	::free(buffer);
}

void Buffer::g_free(void *data) {
	Buffer::free((struct buffer*)data);
}

struct buffer* Buffer::copy(const void *data, size_t length) {
    
	struct buffer *buffer = (struct buffer*)malloc(sizeof(*buffer));
	if (!buffer) goto err_out;
	buffer->pointer = malloc(length);
	if (!buffer->pointer) goto err_out_free;

	memcpy(buffer->pointer, data, length);
	buffer->length = length;

	return buffer;

err_out_free:
	Buffer::free(buffer);
err_out:
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
