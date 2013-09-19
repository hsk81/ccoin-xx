/* 
 * File:   Bloom.h
 * Author: hsk81
 *
 * Created on September 19, 2013, 9:16 AM
 */

#ifndef BLOOM_H
#define	BLOOM_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include <stdbool.h>
#include <glib.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

struct bloom {
	GString         *v_data;
	unsigned int    n_hash_funcs;
};

namespace Bloom {

    //
    // 20k items with FP rate < 0.1% or 10k items and < 0.0001%
    //

    enum {
        MAX_FILTER_SIZE = 36000, // bytes
        MAX_HASH_FUNCS = 50,
    };

    bool init(struct bloom *bf, unsigned int n_elements, double fp_rate);
    void __init(struct bloom *bf);
    void free(struct bloom *bf);

    void insert(struct bloom *bf, const void *data, size_t length);
    bool contains(struct bloom *bf, const void *data, size_t length);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif	/* BLOOM_H */
