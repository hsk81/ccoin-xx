/* 
 * File:   Bloom.cpp
 * Author: hsk81
 * 
 * Created on September 19, 2013, 9:16 AM
 */

#include "../include/Bloom.h"

#include <math.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define LN2SQUARED 0.4804530139182014246671025263266649717305529515945455L
#define LN2 0.6931471805599453094172321214581765680755001343602552L

static void g_string_resize(GString *g_string, unsigned int new_index) {

	unsigned int new_sz = new_index + 1;
	unsigned int cur_sz = g_string->len;
	if (cur_sz >= new_sz) return;

	g_string_set_size(g_string, new_sz);
	unsigned int pad = new_sz - cur_sz;
	memset(g_string->str + cur_sz, 0, pad);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool Bloom::init(struct bloom *bf, unsigned int n_elements, double fp_rate) {

	memset(bf, 0, sizeof(*bf));

	unsigned int filter_size = MIN(
        (unsigned int)(-1 / LN2SQUARED * n_elements * log(fp_rate)),
        Bloom::MAX_FILTER_SIZE * 8
    ) / 8;

	bf->v_data = g_string_sized_new(filter_size);
	g_string_resize(bf->v_data, filter_size - 1);

	bf->n_hash_funcs = MIN(
        (unsigned int)(bf->v_data->len * 8 / n_elements * LN2),
        Bloom::MAX_HASH_FUNCS
    );

	return true;
}

void Bloom::__init(struct bloom *bf) {
    
}

void Bloom::free(struct bloom *bf) {
	if (bf->v_data) {
		g_string_free(bf->v_data, TRUE);
		bf->v_data = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Bloom::insert(struct bloom *bf, const void *data, size_t data_length) {
    
}

bool Bloom::contains(struct bloom *bf, const void *data, size_t data_length) {
    
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
