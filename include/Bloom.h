/*
 * File:   Bloom.h
 * Author: hsk81
 */

#ifndef BLOOM_H
#define BLOOM_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "Buffer.h"
#include <glib.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

struct BloomFilter {
    GString *data;
    guint n_hash_funcs;
};

//
// 20k items with an FP rate less than 0.1% or 10k items and less than 0.0001%
//

namespace Bloom {

    enum {
        MAX_FILTER_SIZE = 36000, // bytes
        MAX_HASH_FUNCS = 50,
    };

    gboolean init(
            struct BloomFilter *filter, guint n_elements, gdouble fp_rate);

    void init(struct BloomFilter *filter);
    void free(struct BloomFilter *filter);

    void serialize(
            GString *string, const struct BloomFilter *filter);
    gboolean deserialize(
            struct BloomFilter *filter, struct const_buffer *buffer);

    void insert(
            struct BloomFilter *filter, gconstpointer data, gsize size);
    gboolean contains(
            struct BloomFilter *filter, gconstpointer data, gsize size);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif /* BLOOM_H */
