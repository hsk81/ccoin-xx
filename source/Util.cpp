/* 
 * File:   Util.cpp
 * Author: hsk81
 */

#include "../include/Util.h"

#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Util::reverse_copy(guchar* target, const guchar* source, gsize size) {

    for (guint _i = 0; _i < size; _i++) {
        target[size - _i - 1] = source[_i];
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Util::Hash(guchar *md256, gconstpointer data, gsize size) {

    guchar _md1[SHA256_DIGEST_LENGTH];
    SHA256((const guchar*) data, size, _md1);
    SHA256(_md1, SHA256_DIGEST_LENGTH, md256);
}

void Util::Hash4(guchar *md32, gconstpointer data, gsize size) {

    guchar _md256[SHA256_DIGEST_LENGTH];
    Util::Hash(_md256, data, size);
    memcpy(md32, _md256, 4);
}

void Util::Hash160(guchar *md160, gconstpointer data, gsize size) {

    guchar _md1[SHA256_DIGEST_LENGTH];
    SHA256((guchar*) data, size, _md1);
    RIPEMD160((guchar*) _md1, SHA256_DIGEST_LENGTH, md160);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

gint Util::open_file(const gchar *path) {

    gint _fd = open(path, O_RDONLY | O_LARGEFILE);
    if (_fd < 0) return -1;

#if _XOPEN_SOURCE >= 600 || _POSIX_C_SOURCE >= 200112L
    posix_fadvise(_fd, 0, 0, POSIX_FADV_SEQUENTIAL);
#endif

    return _fd;
}

gboolean Util::read_file(
        const gchar *path, gpointer *data, gsize *size, gsize max_size) {

    gpointer _data;
    struct stat _stat;

    *data = NULL;
    *size = 0;
    glong _rrc;

    gint _fd = Util::open_file(path);
    if (_fd < 0) return FALSE;

    if (fstat(_fd, &_stat) < 0) goto exit;
    if (_stat.st_size > max_size) goto exit;

    _data = malloc(_stat.st_size);
    if (!_data) goto exit;
    _rrc = read(_fd, _data, _stat.st_size);
    if (_rrc != _stat.st_size) goto exit_free;

    close(_fd);
    _fd = -1;

    *data = _data;
    *size = _stat.st_size;

    return TRUE;
exit_free:
    free(_data);
exit:
    if (_fd >= 0) close(_fd);
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Util::BigNum::setvch(BIGNUM* value, gconstpointer data, gsize size) {

    const guchar *_data = (guchar*) data;
    guint _puch_size = size + 4;
    guchar _puch[_puch_size];

    _puch[0] = (size >> 24) & 0xff;
    _puch[1] = (size >> 16) & 0xff;
    _puch[2] = (size >> 8) & 0xff;
    _puch[3] = (size >> 0) & 0xff;

    reverse_copy(_puch + 4, _data, size);
    BN_mpi2bn(_puch, _puch_size, value);
}

GString *Util::BigNum::getvch(const BIGNUM* value) {

    // get MPI format size
    guint _size = BN_bn2mpi(value, NULL);
    if (_size <= 4) return g_string_new(NULL);

    // store bignum as MPI
    GString *_string_be = g_string_sized_new(_size);
    g_string_set_size(_string_be, _size);
    BN_bn2mpi(value, (guchar *) _string_be->str);

    // copy-swap MPI to little endian, w/o 32-bit size prefix
    guint _size_le = _size - 4;
    GString *_string_le = g_string_sized_new(_size_le);
    g_string_set_size(_string_le, _size_le);

    reverse_copy(
            (guchar*) _string_le->str, (guchar*) _string_be->str + 4, _size_le);

    g_string_free(_string_be, TRUE);
    return _string_le;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

gulong Util::Djb2::hash(gulong hash, gconstpointer buffer, gsize size) {

    const guchar *_puch = (guchar*) buffer;
    gint _ch;

    while (size > 0) {
        _ch = *_puch++;
        size--;
        hash = ((hash << 5) + hash) ^ _ch;
    }

    return hash;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
