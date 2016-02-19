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

void Base58Test::encode(const gchar* pch_hex, const gchar* pch_b58) {

    gsize _size_hex = strlen(pch_hex) / 2;
    guchar *_puch = (guchar*) calloc(1, _size_hex);
    gsize _size_puch;

    gboolean _decoded =
            HexCode::decode(_puch, _size_hex, pch_hex, &_size_puch);
    CPPUNIT_ASSERT(_decoded);

    GString *_string = Base58::encode(_puch, _size_puch);
    CPPUNIT_ASSERT(!strcmp(_string->str, pch_b58));

    free(_puch);
    g_string_free(_string, TRUE);
}

void Base58Test::decode(const gchar* pch_hex, const gchar* pch_b58) {

    gsize _size_hex = strlen(pch_hex) / 2;
    guchar *_puch = (guchar*) calloc(1, _size_hex);
    gsize _size_puch;

    gboolean decoded =
            HexCode::decode(_puch, _size_hex, pch_hex, &_size_puch);
    CPPUNIT_ASSERT(decoded);

    GString *_string = Base58::decode(pch_b58);
    gboolean _is_equal = memcmp(_string->str, _puch, _size_puch < _string->len
            ? _size_puch : _string->len) == 0;

    CPPUNIT_ASSERT(_is_equal);
    CPPUNIT_ASSERT(_string->len == _size_puch);

    free(_puch);
    g_string_free(_string, TRUE);
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

    const gchar *_filename = "Base58.json";
    gchar *_path = TestLib::filename(_filename);
    json_t *_data = TestLib::read_json(_path);
    CPPUNIT_ASSERT(json_is_array(_data));

    gsize n_tests = json_array_size(_data);
    for (guint i = 0; i < n_tests; i++) {
        json_t *inner = json_array_get(_data, i);
        CPPUNIT_ASSERT(json_is_array(inner));
        json_t *j_raw = json_array_get(inner, 0);
        CPPUNIT_ASSERT(json_is_string(j_raw));
        json_t *j_enc = json_array_get(inner, 1);
        CPPUNIT_ASSERT(json_is_string(j_enc));

        this->encode(json_string_value(j_raw), json_string_value(j_enc));
        this->decode(json_string_value(j_raw), json_string_value(j_enc));
    }

    free(_path);
    json_decref(_data);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Base58Test::checkPrivateKeyEnc(
        const gchar *pch_b58, GString* payload,
        gboolean do_compress, gboolean is_testnet) {

    CPPUNIT_ASSERT(payload != NULL);

    GString *_payload = g_string_sized_new(payload->len + 1);
    g_string_append_len(_payload, payload->str, payload->len);
    if (do_compress) g_string_append_c(_payload, 1);

    GString *_string_b58 = Base58::encode_check(
            _payload->str, _payload->len,
            is_testnet ? PRIVKEY_ADDRESS_TEST : PRIVKEY_ADDRESS, true);

    CPPUNIT_ASSERT(_string_b58 != NULL);
    CPPUNIT_ASSERT(strcmp(_string_b58->str, pch_b58) == 0);

    g_string_free(_string_b58, TRUE);
    g_string_free(_payload, TRUE);
    g_string_free(payload, TRUE);
}

void Base58Test::checkPrivateKeyDec(
        const gchar* pch_b58, GString* payload,
        gboolean do_compress, gboolean is_testnet) {

    CPPUNIT_ASSERT(payload != NULL);

    GString *_payload = g_string_sized_new(payload->len + 1);
    g_string_append_len(_payload, payload->str, payload->len);
    if (do_compress) g_string_append_c(_payload, 1);

    guchar _address_type;
    GString *_string_b58_dec =
            Base58::decode_check(pch_b58, &_address_type);
    CPPUNIT_ASSERT(_string_b58_dec != NULL);

    if (is_testnet) {
        CPPUNIT_ASSERT(_address_type == PRIVKEY_ADDRESS_TEST);
    } else {
        CPPUNIT_ASSERT(_address_type == PRIVKEY_ADDRESS);
    }

    if (do_compress) {
        CPPUNIT_ASSERT(_string_b58_dec->len == 33);
        CPPUNIT_ASSERT(_string_b58_dec->str[32] == 1);
    } else {
        CPPUNIT_ASSERT(_string_b58_dec->len == 32);
    }

    CPPUNIT_ASSERT(_string_b58_dec->len == _payload->len);
    CPPUNIT_ASSERT(memcmp(
            _string_b58_dec->str, _payload->str, _payload->len) == 0);

    g_string_free(_string_b58_dec, TRUE);
    g_string_free(_payload, TRUE);
    g_string_free(payload, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Base58Test::checkPublicKeyEnc(
        const gchar *pch_b58, GString *payload,
        const gchar *address_type, gboolean is_testnet) {

    CPPUNIT_ASSERT(payload != NULL);

    gboolean _is_pubkey = strcmp(address_type, "pubkey") == 0;
    gboolean _is_script = strcmp(address_type, "script") == 0;
    CPPUNIT_ASSERT(_is_pubkey || _is_script);

    enum AddressType _type = _is_pubkey
            ? is_testnet ? PUBKEY_ADDRESS_TEST : PUBKEY_ADDRESS
            : is_testnet ? SCRIPT_ADDRESS_TEST : SCRIPT_ADDRESS;

    GString *_string_b58_enc = Base58::encode_check(
            payload->str, payload->len, _type, true);

    CPPUNIT_ASSERT(strcmp(_string_b58_enc->str, pch_b58) == 0);

    g_string_free(_string_b58_enc, TRUE);
    g_string_free(payload, TRUE);
}

void Base58Test::checkPublicKeyDec(
        const gchar* pch_b58, GString* payload,
        const gchar* address_type, gboolean is_testnet) {

    CPPUNIT_ASSERT(payload != NULL);

    gboolean _is_pubkey = (strcmp(address_type, "pubkey") == 0);
    gboolean _is_script = (strcmp(address_type, "script") == 0);
    CPPUNIT_ASSERT(_is_pubkey || _is_script);

    enum AddressType type = _is_pubkey
            ? is_testnet ? PUBKEY_ADDRESS_TEST : PUBKEY_ADDRESS
            : is_testnet ? SCRIPT_ADDRESS_TEST : SCRIPT_ADDRESS;

    guchar _address_type_dec;
    GString *_string_b58_dec = 
            Base58::decode_check(pch_b58, &_address_type_dec);

    CPPUNIT_ASSERT(_string_b58_dec != NULL);
    CPPUNIT_ASSERT(type == _address_type_dec);
    CPPUNIT_ASSERT(_string_b58_dec->len == 20);
    CPPUNIT_ASSERT(payload->len == _string_b58_dec->len);
    CPPUNIT_ASSERT(memcmp(
            payload->str, _string_b58_dec->str, _string_b58_dec->len) == 0);

    g_string_free(_string_b58_dec, TRUE);
    g_string_free(payload, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Base58Test::testKeysMethod() {

    const gchar *_filename = "Base58Keys.json";
    gchar *_path = TestLib::filename(_filename);
    json_t *_data = TestLib::read_json(_path);
    CPPUNIT_ASSERT(json_is_array(_data));

    gsize _n_tests = json_array_size(_data);
    for (guint _i= 0; _i < _n_tests; _i++) {
        json_t *inner = json_array_get(_data, _i);
        CPPUNIT_ASSERT(json_is_array(inner));

        json_t *_j_b58 = json_array_get(inner, 0);
        CPPUNIT_ASSERT(json_is_string(_j_b58));
        json_t *_j_payload = json_array_get(inner, 1);
        CPPUNIT_ASSERT(json_is_string(_j_payload));
        json_t *_j_meta = json_array_get(inner, 2);
        CPPUNIT_ASSERT(json_is_object(_j_meta));

        json_t *_j_addrtype = json_object_get(_j_meta, "addrType");
        CPPUNIT_ASSERT(!_j_addrtype || json_is_string(_j_addrtype));
        json_t *_j_compress = json_object_get(_j_meta, "isCompressed");
        CPPUNIT_ASSERT(!_j_compress || json_is_true(_j_compress) 
                || json_is_false(_j_compress));

        gboolean _is_privkey = 
                json_is_true(json_object_get(_j_meta, "isPrivkey"));
        gboolean _is_testnet = 
                json_is_true(json_object_get(_j_meta, "isTestnet"));

        if (_is_privkey) {
            this->checkPrivateKeyEnc(
                    json_string_value(_j_b58),
                    HexCode::to_string(json_string_value(_j_payload)),
                    json_is_true(_j_compress),
                    _is_testnet);
            this->checkPrivateKeyDec(
                    json_string_value(_j_b58),
                    HexCode::to_string(json_string_value(_j_payload)),
                    json_is_true(_j_compress),
                    _is_testnet);
        } else {
            this->checkPublicKeyEnc(
                    json_string_value(_j_b58),
                    HexCode::to_string(json_string_value(_j_payload)),
                    json_string_value(_j_addrtype),
                    _is_testnet);
            this->checkPublicKeyDec(
                    json_string_value(_j_b58),
                    HexCode::to_string(json_string_value(_j_payload)),
                    json_string_value(_j_addrtype),
                    _is_testnet);
        }
    }

    free(_path);
    json_decref(_data);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
