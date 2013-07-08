/* 
 * File:   Util.cpp
 * Author: hsk81
 * 
 * Created on July 5, 2013, 6:37 PM
 */

#include "../include/Util.h"

#include <openssl/sha.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Util::Bu::reverse_copy(unsigned char* target, const unsigned char* source,
        size_t length) {
    
    for (unsigned int i = 0; i < length; i++) {
        target[length - i - 1] = source[i];
    }
}

void Util::Bu::Hash(unsigned char *md256, const void *data,
        size_t data_length) {

    unsigned char md1[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)data, data_length, md1);
    SHA256(md1, SHA256_DIGEST_LENGTH, md256);
}

void Util::Bu::Hash4(unsigned char *md32, const void *data,
        size_t data_length) {

    unsigned char md256[SHA256_DIGEST_LENGTH];
    Util::Bu::Hash(md256, data, data_length);
    memcpy(md32, md256, 4);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Util::Bn::setvch(BIGNUM* value, const void* data, size_t data_length) {
    
    const unsigned char *data_ = (unsigned char*)data;
    unsigned int vch_sz = data_length + 4;
    unsigned char vch[vch_sz];

    vch[0] = (data_length >> 24) & 0xff;
    vch[1] = (data_length >> 16) & 0xff;
    vch[2] = (data_length >> 8) & 0xff;
    vch[3] = (data_length >> 0) & 0xff;

    Bu::reverse_copy(vch + 4, data_, data_length);
    BN_mpi2bn(vch, vch_sz, value);
}

GString *Util::Bn::getvch(const BIGNUM* value) {
    
    /* get MPI format size */
    unsigned int sz = BN_bn2mpi(value, NULL);
    if (sz <= 4) return g_string_new(NULL);

    /* store bignum as MPI */
    GString *s_be = g_string_sized_new(sz);
    g_string_set_size(s_be, sz);
    BN_bn2mpi(value, (unsigned char *) s_be->str);

    /* copy-swap MPI to little endian, sans 32-bit size prefix */
    unsigned int le_sz = sz - 4;
    GString *s_le = g_string_sized_new(le_sz);
    g_string_set_size(s_le, le_sz);
    Bu::reverse_copy((unsigned char *)s_le->str,
            (unsigned char *)s_be->str + 4, le_sz);

    g_string_free(s_be, TRUE);
    return s_le;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
