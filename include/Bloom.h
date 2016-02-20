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

struct TBloom {
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
            struct TBloom *filter, guint n_elements, gdouble fp_rate);

    void init(struct TBloom *filter);
    void free(struct TBloom *filter);

    void serialize(
            GString *string, const struct TBloom *filter);
    gboolean deserialize(
            struct TBloom *filter, struct TConstantBuffer *buffer);
    void insert(
            struct TBloom *filter, gconstpointer data, gsize size);
    gboolean contains(
            struct TBloom *filter, gconstpointer data, gsize size);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif /* BLOOM_H */
