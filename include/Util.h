/* 
 * File:   Util.h
 * Author: hsk81
 *
 * Created on July 5, 2013, 6:37 PM
 */

#ifndef UTIL_H
#define	UTIL_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include <glib.h>
#include <openssl/bn.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

namespace Util {

    void reverse_copy(unsigned char *target, const unsigned char *source,
            size_t length);

    void Hash(unsigned char *md256, const void *data, size_t length);
    void Hash4(unsigned char *md32, const void *data, size_t length);

    int open_file(const char *filename);
    bool read_file(const char *filename, void **data, size_t *length,
            size_t max_file_size);

    namespace BigNum {
        GString *getvch(const BIGNUM *value);
        void setvch(BIGNUM *value, const void *data, size_t length);
    }
    
    namespace Djb2 {
        unsigned long hash(unsigned long hash, const void *buffer,
                size_t length);
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif	/* UTIL_H */
