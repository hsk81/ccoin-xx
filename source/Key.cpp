/* 
 * File:   Key.h
 * Author: hsk81
 */

#include "../include/Key.h"
#include "../include/Buffer.h"
#include "../include/Util.h"

#include <openssl/obj_mac.h>
#include <openssl/ripemd.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

gboolean Key::init(struct TKey *key) {

    memset(key, 0, sizeof (*key));
    key->ec_key = EC_KEY_new_by_curve_name(NID_secp256k1);
    return !!key->ec_key;
}

void Key::free(struct TKey *key) {

    if (key->ec_key) {
        EC_KEY_free(key->ec_key);
        key->ec_key = NULL;
    }
}

gboolean Key::generate(struct TKey *key) {

    if (!key->ec_key) return FALSE;
    if (!EC_KEY_generate_key(key->ec_key)) return FALSE;
    if (!EC_KEY_check_key(key->ec_key)) return FALSE;

    EC_KEY_set_conv_form(key->ec_key, POINT_CONVERSION_COMPRESSED);
    return TRUE;
}

gboolean Key::get_public(struct TKey *key, gpointer *pointer, gsize *size) {

    if (!EC_KEY_check_key(key->ec_key)) {
        return FALSE;
    }

    gsize puch_size = i2o_ECPublicKey(key->ec_key, 0);
    guchar *puch = (guchar*) malloc(puch_size);
    guchar *puch_orig = puch;
    i2o_ECPublicKey(key->ec_key, &puch);

    *pointer = puch_orig;
    *size = puch_size;

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Keyset::init(struct TKeyset *keyset) {

    memset(keyset, 0, sizeof (*keyset));
    keyset->pubkey = g_hash_table_new_full(
            Buffer::hash, Buffer::equal, Buffer::free, NULL);
    keyset->pubkey_hash = g_hash_table_new_full(
            Buffer::hash, Buffer::equal, Buffer::free, NULL);
}

void Keyset::free(struct TKeyset *keyset) {

    g_hash_table_unref(keyset->pubkey);
    g_hash_table_unref(keyset->pubkey_hash);
}

gboolean Keyset::add(struct TKeyset *keyset, struct TKey *key) {

    gpointer pointer = NULL;
    gsize size = 0;

    if (!Key::get_public(key, &pointer, &size)) {
        return FALSE;
    }

    struct TBuffer *pubkey = (TBuffer*) malloc(sizeof (struct TBuffer));
    pubkey->pointer = pointer;
    pubkey->size = size;

    guchar md160[RIPEMD160_DIGEST_LENGTH];
    Util::Hash160(md160, pointer, size);

    struct TBuffer *pubkey_hash = Buffer::copy(md160, RIPEMD160_DIGEST_LENGTH);

    g_hash_table_replace(keyset->pubkey, pubkey, pubkey);
    g_hash_table_replace(keyset->pubkey_hash, pubkey_hash, pubkey_hash);

    return TRUE;
}

gboolean Keyset::lookup(
        const struct TKeyset *keyset, gconstpointer pointer, gsize size,
        gboolean is_hash) {

    struct TConstantBuffer buffer = {pointer, size};
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
