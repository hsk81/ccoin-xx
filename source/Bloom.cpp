/* 
 * File:   Bloom.cpp
 * Author: hsk81
 * 
 * Created on September 19, 2013, 9:16 AM
 */

#include "../include/Bloom.h"
#include "../include/Buffer.h"
#include "../include/Serialize.h"

#include <math.h>
#include <string.h>
#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define LN2SQUARED 0.4804530139182014246671025263266649717305529515945455L
#define LN2 0.6931471805599453094172321214581765680755001343602552L

static const unsigned char bit_mask[8] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

static inline uint32_t ROTL32 (uint32_t x, int8_t r) {
    return (x << r) | (x >> (32 - r));
}

static void g_string_resize(GString *g_string, unsigned int new_index) {

	unsigned int new_sz = new_index + 1;
	unsigned int cur_sz = g_string->len;
	if (cur_sz >= new_sz) return;

	g_string_set_size(g_string, new_sz);
	unsigned int pad = new_sz - cur_sz;
	memset(g_string->str + cur_sz, 0, pad);
}

/** 
 * The following is MurmurHash3 (x86_32), see 
 * http://code.google.com/p/smhasher/source/browse/trunk/MurmurHash3.cpp
 */

static unsigned int bloom_hash(struct bloom *bf, unsigned int n_hash_num,
        struct const_buffer *v_data_to_hash) {
    
	uint32_t h1 = n_hash_num * (0xffffffffU / (bf->n_hash_funcs-1));
	const uint32_t c1 = 0xcc9e2d51;
	const uint32_t c2 = 0x1b873593;
	const unsigned char *puch = (unsigned char*)v_data_to_hash->pointer;
	const int n_block = v_data_to_hash->size / 4;

	//----------
	// body
	const uint32_t * blocks = (const uint32_t *)(&puch[0] + n_block*4);

	for(int i = -n_block; i; i++) {
		uint32_t k1 = blocks[i];

		k1 *= c1;
		k1 = ROTL32(k1,15);
		k1 *= c2;

		h1 ^= k1;
		h1 = ROTL32(h1,13);
		h1 = h1*5+0xe6546b64;
	}

	//----------
	// tail
	const uint8_t * tail = (const uint8_t*)(&puch[0] + n_block*4);

	uint32_t k1 = 0;
	switch(v_data_to_hash->size & 3) {
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1: k1 ^= tail[0];
                k1 *= c1; k1 = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
	};

	//----------
	// finalization
	h1 ^= v_data_to_hash->size;
	h1 ^= h1 >> 16;
	h1 *= 0x85ebca6b;
	h1 ^= h1 >> 13;
	h1 *= 0xc2b2ae35;
	h1 ^= h1 >> 16;

	return h1 % (bf->data->len * 8);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool Bloom::init(struct bloom *bf, unsigned int n_elements, double fp_rate) {

	memset(bf, 0, sizeof(*bf));

	unsigned int filter_size = MIN(
        (unsigned int)(-1 / LN2SQUARED * n_elements * log(fp_rate)),
        Bloom::MAX_FILTER_SIZE * 8
    ) / 8;

	bf->data = g_string_sized_new(filter_size);
	g_string_resize(bf->data, filter_size - 1);

	bf->n_hash_funcs = MIN(
        (unsigned int)(bf->data->len * 8 / n_elements * LN2),
        Bloom::MAX_HASH_FUNCS
    );

	return true;
}

void Bloom::__init(struct bloom *bf) {
	memset(bf, 0, sizeof(*bf));
}

void Bloom::free(struct bloom *bf) {
	if (bf->data) {
		g_string_free(bf->data, TRUE);
		bf->data = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Bloom::serialize(GString *g_string, const struct bloom *bf) {
	Serialize::varstr(g_string, bf->data);
	Serialize::u32(g_string, bf->n_hash_funcs);
}

bool Bloom::deserialize(struct bloom *bf, struct const_buffer *buffer) {
	if (!Deserialize::varstr(&bf->data, buffer)) return false;
	if (!Deserialize::u32(&bf->n_hash_funcs, buffer)) return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Bloom::insert(struct bloom *bf, const void *data, size_t length) {
    
	struct const_buffer v_key = {
        data, length
    };

	for (unsigned int i = 0; i < bf->n_hash_funcs; i++) {
		unsigned int n_index = bloom_hash(bf, i, &v_key);
		g_string_resize(bf->data, n_index >> 3);
		bf->data->str[n_index >> 3] |= bit_mask[7 & n_index];
	}
}

bool Bloom::contains(struct bloom *bf, const void *data, size_t length) {
    
	struct const_buffer v_key = {
        data, length
    };

	for (unsigned int i = 0; i < bf->n_hash_funcs; i++) {
		unsigned int n_index = bloom_hash(bf, i, &v_key);
		g_string_resize(bf->data, n_index >> 3);
		if (!(bf->data->str[n_index >> 3] & bit_mask[7 & n_index])) {
			return false;
        }
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
