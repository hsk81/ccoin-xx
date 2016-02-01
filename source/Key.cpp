/* 
 * File:   Key.h
 * Author: hsk81
 *
 * Created on February 1, 2014, 1:42 PM
 */

#include "../include/Key.h"
#include "../include/Buffer.h"
#include "../include/Util.h"

#include <openssl/obj_mac.h>
#include <openssl/ripemd.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool Key::init(struct key *key) {

    memset(key, 0, sizeof (*key));
    key->ec_key = EC_KEY_new_by_curve_name(NID_secp256k1);
    return !!key->ec_key;
}

void Key::free(struct key *key) {
    if (key->ec_key) {
        EC_KEY_free(key->ec_key);
        key->ec_key = NULL;
    }
}

bool Key::generate(struct key *key) {

    if (!key->ec_key) return false;
    if (!EC_KEY_generate_key(key->ec_key)) return false;
    if (!EC_KEY_check_key(key->ec_key)) return false;

    EC_KEY_set_conv_form(key->ec_key, POINT_CONVERSION_COMPRESSED);
    return true;
}

bool Key::get_public(struct key *key, void **pointer, size_t *length) {
    if (!EC_KEY_check_key(key->ec_key)) {
        return false;
    }

    size_t size = i2o_ECPublicKey(key->ec_key, 0);
    unsigned char *pch = (unsigned char*) malloc(size);
    unsigned char *pch_orig = pch;
    i2o_ECPublicKey(key->ec_key, &pch);

    *pointer = pch_orig;
    *length = size;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Keyset::init(struct keyset *keyset) {

    memset(keyset, 0, sizeof (*keyset));
    keyset->pubkey = g_hash_table_new_full(
            Buffer::hash, Buffer::equal, Buffer::free, NULL);
    keyset->pubkey_hash = g_hash_table_new_full(
            Buffer::hash, Buffer::equal, Buffer::free, NULL);
}

void Keyset::free(struct keyset *keyset) {

    g_hash_table_unref(keyset->pubkey);
    g_hash_table_unref(keyset->pubkey_hash);
}

bool Keyset::add(struct keyset *keyset, struct key *key) {

    void *pointer = NULL;
    size_t size = 0;

    if (!Key::get_public(key, &pointer, &size)) {
        return false;
    }

    struct buffer *pubkey = (buffer*) malloc(sizeof (struct buffer));
    pubkey->pointer = pointer;
    pubkey->size = size;

    unsigned char md160[RIPEMD160_DIGEST_LENGTH];
    Util::Hash160(md160, pointer, size);

    struct buffer *pubkey_hash = Buffer::copy(md160, RIPEMD160_DIGEST_LENGTH);

    g_hash_table_replace(keyset->pubkey, pubkey, pubkey);
    g_hash_table_replace(keyset->pubkey_hash, pubkey_hash, pubkey_hash);

    return true;
}

bool Keyset::lookup(const struct keyset *keyset, const void *data, size_t length,
        bool is_hash) {

    struct const_buffer buffer = {data, length};
    GHashTable *hash_table;

    if (is_hash) {
        hash_table = keyset->pubkey_hash;
    } else {
        hash_table = keyset->pubkey;
    }

    return g_hash_table_lookup_extended(hash_table, &buffer, NULL, NULL);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
