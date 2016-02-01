/*
 * File:   Base58Test.cpp
 * Author: hsk81
 */

#include "Base58Test.h"
#include "TestLib.h"

#include "../include/Base58.h"
#include "../include/HexCode.h"
#include "../include/CoreDefs.h"

#include <string.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION(Base58Test);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Base58Test::encode(
        const gchar* hex_characters, const gchar* b58_characters) {

    gsize hex_length = strlen(hex_characters) / 2;
    guchar *raw = (guchar*) calloc(1, hex_length);
    gsize raw_length;

    gboolean decoded = HexCode::decode(
            raw, hex_length, hex_characters, &raw_length);
    CPPUNIT_ASSERT(decoded);

    GString *string = Base58::encode(raw, raw_length);
    CPPUNIT_ASSERT(!strcmp(string->str, b58_characters));

    free(raw);
    g_string_free(string, TRUE);
}

void Base58Test::decode(
        const gchar* hex_characters, const gchar* b58_characters) {

    gsize hex_length = strlen(hex_characters) / 2;
    guchar *raw = (guchar*) calloc(1, hex_length);
    gsize raw_length;

    gboolean decoded = HexCode::decode(
            raw, hex_length, hex_characters, &raw_length);
    CPPUNIT_ASSERT(decoded);

    GString *string = Base58::decode(b58_characters);
    gint is_equal = memcmp(string->str, raw, raw_length < string->len
            ? raw_length : string->len);

    CPPUNIT_ASSERT(is_equal == 0);
    CPPUNIT_ASSERT(string->len == raw_length);

    free(raw);
    g_string_free(string, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Base58Test::testEncodeMethod() {
    this->encode("", "");
    this->encode("00000000000000000000", "1111111111");
}

void Base58Test::testDecodeMethod() {
    this->decode("", "");
    this->decode("00000000000000000000", "1111111111");
}

void Base58Test::testEncodeAndDecodeMethod() {

    const gchar *file = "Base58.json";
    gchar *path = TestLib::filename(file);
    json_t *data = TestLib::read_json(path);
    CPPUNIT_ASSERT(json_is_array(data));

    gsize n_tests = json_array_size(data);
    for (guint i = 0; i < n_tests; i++) {
        json_t *inner = json_array_get(data, i);
        CPPUNIT_ASSERT(json_is_array(inner));
        json_t *j_raw = json_array_get(inner, 0);
        CPPUNIT_ASSERT(json_is_string(j_raw));
        json_t *j_enc = json_array_get(inner, 1);
        CPPUNIT_ASSERT(json_is_string(j_enc));

        this->encode(json_string_value(j_raw), json_string_value(j_enc));
        this->decode(json_string_value(j_raw), json_string_value(j_enc));
    }

    free(path);
    json_decref(data);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Base58Test::checkPrivateKeyEnc(
        const gchar* b58_characters, GString* payload,
        gboolean do_compress, gboolean is_testnet) {

    CPPUNIT_ASSERT(payload != NULL);

    GString *payload_ = g_string_sized_new(payload->len + 1);
    g_string_append_len(payload_, payload->str, payload->len);
    if (do_compress) g_string_append_c(payload_, 1);

    GString *b58_string = Base58::encode_check(
            is_testnet ? PRIVKEY_ADDRESS_TEST : PRIVKEY_ADDRESS, true,
            payload_->str, payload_->len);

    CPPUNIT_ASSERT(b58_string != NULL);
    CPPUNIT_ASSERT(strcmp(b58_string->str, b58_characters) == 0);

    g_string_free(b58_string, TRUE);
    g_string_free(payload_, TRUE);
    g_string_free(payload, TRUE);
}

void Base58Test::checkPrivateKeyDec(
        const gchar* b58_characters, GString* payload,
        gboolean do_compress, gboolean is_testnet) {

    CPPUNIT_ASSERT(payload != NULL);

    GString *payload_ = g_string_sized_new(payload->len + 1);
    g_string_append_len(payload_, payload->str, payload->len);
    if (do_compress) g_string_append_c(payload_, 1);

    guchar address_type;
    GString *b58_string_dec =
            Base58::decode_check(&address_type, b58_characters);
    CPPUNIT_ASSERT(b58_string_dec != NULL);

    if (is_testnet) {
        CPPUNIT_ASSERT(address_type == PRIVKEY_ADDRESS_TEST);
    } else {
        CPPUNIT_ASSERT(address_type == PRIVKEY_ADDRESS);
    }

    if (do_compress) {
        CPPUNIT_ASSERT(b58_string_dec->len == 33);
        CPPUNIT_ASSERT(b58_string_dec->str[32] == 1);
    } else {
        CPPUNIT_ASSERT(b58_string_dec->len == 32);
    }

    CPPUNIT_ASSERT(b58_string_dec->len == payload_->len);
    CPPUNIT_ASSERT(memcmp(
            b58_string_dec->str, payload_->str, payload_->len) == 0);

    g_string_free(b58_string_dec, TRUE);
    g_string_free(payload_, TRUE);
    g_string_free(payload, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Base58Test::checkPublicKeyEnc(
        const gchar *b58_characters, GString *payload,
        const gchar *address_type, gboolean is_testnet) {

    CPPUNIT_ASSERT(payload != NULL);

    gboolean address_type_pubkey = (strcmp(address_type, "pubkey") == 0);
    gboolean address_type_script = (strcmp(address_type, "script") == 0);
    CPPUNIT_ASSERT(address_type_pubkey || address_type_script);

    enum AddressType a_type = address_type_pubkey
            ? (is_testnet) ? PUBKEY_ADDRESS_TEST : PUBKEY_ADDRESS
            : (is_testnet) ? SCRIPT_ADDRESS_TEST : SCRIPT_ADDRESS;

    GString *b58_string_enc = Base58::encode_check(
            a_type, true, payload->str, payload->len);

    CPPUNIT_ASSERT(strcmp(b58_string_enc->str, b58_characters) == 0);

    g_string_free(b58_string_enc, TRUE);
    g_string_free(payload, TRUE);
}

void Base58Test::checkPublicKeyDec(
        const gchar* b58_characters, GString* payload,
        const gchar* address_type, gboolean is_testnet) {

    CPPUNIT_ASSERT(payload != NULL);

    gboolean address_type_pubkey = (strcmp(address_type, "pubkey") == 0);
    gboolean address_type_script = (strcmp(address_type, "script") == 0);
    CPPUNIT_ASSERT(address_type_pubkey || address_type_script);

    enum AddressType type = (address_type_pubkey)
            ? (is_testnet) ? PUBKEY_ADDRESS_TEST : PUBKEY_ADDRESS
            : (is_testnet) ? SCRIPT_ADDRESS_TEST : SCRIPT_ADDRESS;

    guchar address_type_dec;
    GString *b58_string_dec = 
            Base58::decode_check(&address_type_dec, b58_characters);

    CPPUNIT_ASSERT(b58_string_dec != NULL);
    CPPUNIT_ASSERT(type == address_type_dec);
    CPPUNIT_ASSERT(b58_string_dec->len == 20);
    CPPUNIT_ASSERT(payload->len == b58_string_dec->len);
    CPPUNIT_ASSERT(memcmp(
            payload->str, b58_string_dec->str, b58_string_dec->len) == 0);

    g_string_free(b58_string_dec, TRUE);
    g_string_free(payload, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Base58Test::testKeysMethod() {

    const gchar *file = "Base58Keys.json";
    gchar *path = TestLib::filename(file);
    json_t *data = TestLib::read_json(path);
    CPPUNIT_ASSERT(json_is_array(data));

    size_t n_tests = json_array_size(data);
    for (guint i = 0; i < n_tests; i++) {
        json_t *inner = json_array_get(data, i);
        CPPUNIT_ASSERT(json_is_array(inner));

        json_t *j_base58 = json_array_get(inner, 0);
        CPPUNIT_ASSERT(json_is_string(j_base58));
        json_t *j_payload = json_array_get(inner, 1);
        CPPUNIT_ASSERT(json_is_string(j_payload));
        json_t *j_meta = json_array_get(inner, 2);
        CPPUNIT_ASSERT(json_is_object(j_meta));

        json_t *j_addrtype = json_object_get(j_meta, "addrType");
        CPPUNIT_ASSERT(!j_addrtype || json_is_string(j_addrtype));
        json_t *j_compress = json_object_get(j_meta, "isCompressed");
        CPPUNIT_ASSERT(!j_compress || json_is_true(j_compress) 
                || json_is_false(j_compress));

        gboolean is_privkey = json_is_true(json_object_get(j_meta, "isPrivkey"));
        gboolean is_testnet = json_is_true(json_object_get(j_meta, "isTestnet"));

        if (is_privkey) {
            this->checkPrivateKeyEnc(
                    json_string_value(j_base58),
                    HexCode::to_string(json_string_value(j_payload)),
                    json_is_true(j_compress),
                    is_testnet);
            this->checkPrivateKeyDec(
                    json_string_value(j_base58),
                    HexCode::to_string(json_string_value(j_payload)),
                    json_is_true(j_compress),
                    is_testnet);
        } else {
            this->checkPublicKeyEnc(
                    json_string_value(j_base58),
                    HexCode::to_string(json_string_value(j_payload)),
                    json_string_value(j_addrtype),
                    is_testnet);
            this->checkPublicKeyDec(
                    json_string_value(j_base58),
                    HexCode::to_string(json_string_value(j_payload)),
                    json_string_value(j_addrtype),
                    is_testnet);
        }
    }

    free(path);
    json_decref(data);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
