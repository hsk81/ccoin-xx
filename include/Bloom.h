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

struct bloom {
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

    gboolean init(struct bloom *bf, guint n_elements, gdouble fp_rate);
    void init(struct bloom *bf);
    void free(struct bloom *bf);

    void serialize(GString *string, const struct bloom *bf);
    gboolean deserialize(struct bloom *bf, struct const_buffer *buffer);

    void insert(struct bloom *bf, gconstpointer data, gsize length);
    gboolean contains(struct bloom *bf, gconstpointer data, gsize length);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif /* BLOOM_H */
