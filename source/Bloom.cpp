/*
 * File:   Bloom.cpp
 * Author: hsk81
 */

#include "../include/Bloom.h"
#include "../include/Buffer.h"
#include "../include/Serialize.h"

#include <math.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define LN2SQUARED \
    0.4804530139182014246671025263266649717305529515945455L
#define LN2 \
    0.6931471805599453094172321214581765680755001343602552L

static const guchar bit_mask[8] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

static inline guint32 ROTL32(guint32 x, guint8 r) {
    return (x << r) | (x >> (32 - r));
}

static void string_resize(GString *string, guint index) {

    guint new_size = index + 1;
    guint cur_size = string->len;
    if (cur_size >= new_size) return;

    g_string_set_size(string, new_size);
    guint pad = new_size - cur_size;
    memset(string->str + cur_size, 0, pad);
}

/**
 * The following is MurmurHash3 (x86_32), see:
 *
 * http://code.google.com/p/smhasher/source/browse/trunk/MurmurHash3.cpp
 */

static guint bloom_hash(
        struct TBloom *filter, guint n_hash_num,
        struct TConstantBuffer *buffer) {

    guint32 h1 = n_hash_num * (0xffffffffU / (filter->n_hash_funcs - 1));
    const guint32 c1 = 0xcc9e2d51;
    const guint32 c2 = 0x1b873593;
    const guchar *puch = (guchar*) buffer->pointer;
    const gint n_block = buffer->size / 4;

    // body
    const guint32 *blocks = (const guint32*) (&puch[0] + n_block * 4);

    for (gint i = -n_block; i; i++) {
        guint32 k1 = blocks[i];

        k1 *= c1;
        k1 = ROTL32(k1, 15);
        k1 *= c2;

        h1 ^= k1;
        h1 = ROTL32(h1, 13);
        h1 = h1 * 5 + 0xe6546b64;
    }

    // tail
    const guint8 * tail = (const guint8*) (&puch[0] + n_block * 4);

    guint32 k1 = 0;
    switch (buffer->size & 3) {
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1: k1 ^= tail[0];
            k1 *= c1;
            k1 = ROTL32(k1, 15);
            k1 *= c2;
            h1 ^= k1;
    };

    // finalization
    h1 ^= buffer->size;
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;

    return h1 % (filter->data->len * 8);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

gboolean Bloom::init(
        struct TBloom *filter, guint n_elements, gdouble fp_rate) {

    memset(filter, 0, sizeof (*filter));

    guint filter_size = MIN(
            (guint) (-1 / LN2SQUARED * n_elements * log(fp_rate)),
            Bloom::MAX_FILTER_SIZE * 8) / 8;

    filter->data = g_string_sized_new(filter_size);
    string_resize(filter->data, filter_size - 1);

    filter->n_hash_funcs = MIN(
            (guint) (filter->data->len * 8 / n_elements * LN2),
            Bloom::MAX_HASH_FUNCS);

    return TRUE;
}

void Bloom::init(struct TBloom *filter) {
    memset(filter, 0, sizeof (*filter));
}

void Bloom::free(struct TBloom *filter) {
    if (filter->data) {
        g_string_free(filter->data, TRUE);
        filter->data = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Bloom::serialize(
        GString *string, const struct TBloom *filter) {

    Serialize::var_string(string, filter->data);
    Serialize::u32(string, filter->n_hash_funcs);
}

gboolean Bloom::deserialize(
        struct TBloom *filter, struct TConstantBuffer *buffer) {

    if (!Deserialize::var_string(&filter->data, buffer)) return FALSE;
    if (!Deserialize::u32(&filter->n_hash_funcs, buffer)) return FALSE;

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Bloom::insert(
        struct TBloom *filter, gconstpointer data, gsize size) {

    struct TConstantBuffer buffer = {
        data, size
    };

    for (guint i = 0; i < filter->n_hash_funcs; i++) {
        guint n_index = bloom_hash(filter, i, &buffer);
        string_resize(filter->data, n_index >> 3);
        filter->data->str[n_index >> 3] |= bit_mask[7 & n_index];
    }
}

gboolean Bloom::contains(
        struct TBloom *filter, gconstpointer data, gsize size) {

    struct TConstantBuffer buffer = {
        data, size
    };

    for (guint i = 0; i < filter->n_hash_funcs; i++) {
        guint n = bloom_hash(filter, i, &buffer);
        string_resize(filter->data, n >> 3);
        if (!(filter->data->str[n >> 3] & bit_mask[7 & n])) {
            return FALSE;
        }
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
