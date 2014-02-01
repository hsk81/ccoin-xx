/* 
 * File:   Util.cpp
 * Author: hsk81
 * 
 * Created on July 5, 2013, 6:37 PM
 */

#include "../include/Util.h"

#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Util::reverse_copy(unsigned char* target, const unsigned char* source,
        size_t length) {
    
    for (unsigned int i = 0; i < length; i++) {
        target[length - i - 1] = source[i];
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Util::Hash(unsigned char *md256, const void *data, size_t length) {

    unsigned char md1[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)data, length, md1);
    SHA256(md1, SHA256_DIGEST_LENGTH, md256);
}

void Util::Hash4(unsigned char *md32, const void *data, size_t length) {

    unsigned char md256[SHA256_DIGEST_LENGTH];
    Util::Hash(md256, data, length);
    memcpy(md32, md256, 4);
}

void Util::Hash160(unsigned char *md160, const void *data, size_t length)
{
	unsigned char md1[SHA256_DIGEST_LENGTH];
	SHA256((unsigned char*)data, length, md1);
	RIPEMD160((unsigned char*)md1, SHA256_DIGEST_LENGTH, md160);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int Util::open_file(const char *filename) {

	int fd = open(filename, O_RDONLY | O_LARGEFILE);
	if (fd < 0) return -1;

#if _XOPEN_SOURCE >= 600 || _POSIX_C_SOURCE >= 200112L
	posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL);
#endif

	return fd;
}

bool Util::read_file(const char *filename, void **data, size_t *length,
        size_t max_file_size) {

	void *data_;
	struct stat st;

	*data = NULL;
	*length = 0;
    ssize_t rrc;

	int fd = Util::open_file(filename);
	if (fd < 0) return false;

	if (fstat(fd, &st) < 0) goto err_out_fd;
	if (st.st_size > max_file_size) goto err_out_fd;

	data_ = malloc(st.st_size);
	if (!data_) goto err_out_fd;
	rrc = read(fd, data_, st.st_size);
	if (rrc != st.st_size) goto err_out_mem;

	close(fd);
	fd = -1;

	*data = data_;
	*length = st.st_size;

	return true;

err_out_mem:
	free(data_);
err_out_fd:
	if (fd >= 0) close(fd);
	return false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Util::BigNum::setvch(BIGNUM* value, const void* data, size_t length) {
    
    const unsigned char *data_ = (unsigned char*)data;
    unsigned int vch_sz = length + 4;
    unsigned char vch[vch_sz];

    vch[0] = (length >> 24) & 0xff;
    vch[1] = (length >> 16) & 0xff;
    vch[2] = (length >> 8) & 0xff;
    vch[3] = (length >> 0) & 0xff;

    reverse_copy(vch + 4, data_, length);
    BN_mpi2bn(vch, vch_sz, value);
}

GString *Util::BigNum::getvch(const BIGNUM* value) {
    
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
    reverse_copy((unsigned char *)s_le->str,
        (unsigned char *)s_be->str + 4, le_sz);

    g_string_free(s_be, TRUE);
    return s_le;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

unsigned long Util::Djb2::hash(unsigned long hash, const void *buffer,
        size_t length) {
    
	const unsigned char *puch = (unsigned char*)buffer;
	int ch;

	while (length > 0) {
		ch = *puch++;
		length--;
        
		hash = ((hash << 5) + hash) ^ ch; /* hash * 33 ^ c */
	}

	return hash;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
