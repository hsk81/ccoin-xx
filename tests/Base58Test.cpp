/*
 * File:   Base58Test.cpp
 * Author: hsk81
 *
 * Created on Jul 6, 2013, 8:51:25 AM
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

void Base58Test::encode(const char* hex_string, const char* b58_string) {

    size_t hs_len = strlen(hex_string) / 2;
    unsigned char *raw = (unsigned char*)calloc(1, hs_len);
    size_t out_len;

    bool rc = HexCode::decode(raw, hs_len, hex_string, &out_len);
    if (!rc) {
        fprintf(stderr, "raw %p, sizeof(raw) %lu, hexstr %p %s\n",
            raw, hs_len, hex_string, hex_string);
        CPPUNIT_ASSERT(rc);
    }

    GString *s = Base58::encode(raw, out_len);
    if (strcmp(s->str, b58_string)) {
        fprintf(stderr, "base58 mismatch: '%s' vs expected '%s'\n",
            s->str, b58_string);
        CPPUNIT_ASSERT(!strcmp(s->str, b58_string));
    }

    free(raw);
    g_string_free(s, TRUE);
}

void Base58Test::decode(const char* hex_string, const char* b58_string) {

    size_t hs_len = strlen(hex_string) / 2;
    unsigned char *raw = (unsigned char*)calloc(1, hs_len);
    size_t out_len;

    bool rc = HexCode::decode(raw, hs_len, hex_string, &out_len);
    if (!rc) {
        fprintf(stderr, "raw %p, sizeof(raw) %lu, hexstr %p %s\n",
            raw, hs_len, hex_string, hex_string);
        CPPUNIT_ASSERT(rc);
    }

    GString *s = Base58::decode(b58_string);

    if (memcmp(s->str, raw, out_len < s->len ? out_len : s->len)) {
        TestLib::dumphex("decode have", s->str, s->len);
        TestLib::dumphex("decode want", raw, out_len);
        CPPUNIT_ASSERT(memcmp(s->str, raw, out_len) == 0);
    }

    if (s->len != out_len) {
        fprintf(stderr, "decode len: have %u, want %u\n",
            (unsigned int) s->len,
            (unsigned int) out_len);
        TestLib::dumphex("decode have", s->str, s->len);
        TestLib::dumphex("decode want", raw, out_len);
        CPPUNIT_ASSERT(s->len == out_len);
    }

    free(raw);
    g_string_free(s, TRUE);
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

    const char *base_fn = "Base58.json";
    char *fn = TestLib::filename(base_fn);    
    json_t *data = TestLib::read_json(fn);
    CPPUNIT_ASSERT(json_is_array(data));

    size_t n_tests = json_array_size(data);
    for (unsigned int i = 0; i < n_tests; i++) {
        json_t *inner = json_array_get(data, i);
        CPPUNIT_ASSERT(json_is_array(inner));

        json_t *j_raw = json_array_get(inner, 0);
        CPPUNIT_ASSERT(json_is_string(j_raw));
        json_t *j_enc = json_array_get(inner, 1);
        CPPUNIT_ASSERT(json_is_string(j_enc));

        this->encode(json_string_value(j_raw), json_string_value(j_enc));
        this->decode(json_string_value(j_raw), json_string_value(j_enc));
    }

    free(fn);
    json_decref(data);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Base58Test::checkPrivateKeyEnc(const char* b58_string, GString* payload,
        bool compress, bool is_testnet) {
    
    CPPUNIT_ASSERT(payload != NULL);

    GString *pl = g_string_sized_new(payload->len + 1);
    g_string_append_len(pl, payload->str, payload->len);
    if (compress) g_string_append_c(pl, 1);

    GString *b58 = Base58::encode_check(
        is_testnet ? PRIVKEY_ADDRESS_TEST : PRIVKEY_ADDRESS, true,
        pl->str, pl->len);

    CPPUNIT_ASSERT(b58 != NULL);

    if (strcmp(b58->str, b58_string)) {
        fprintf(stderr, "base58: have %s, expected %s\n", b58->str,
            b58_string);
        CPPUNIT_ASSERT(!strcmp(b58->str, b58_string));
    }

    g_string_free(b58, TRUE);
    g_string_free(pl, TRUE);
    g_string_free(payload, TRUE);
}

void Base58Test::checkPrivateKeyDec(const char* b58_string, GString* payload,
        bool compress, bool is_testnet) {
    
    CPPUNIT_ASSERT(payload != NULL);

    GString *pl = g_string_sized_new(payload->len + 1);
    g_string_append_len(pl, payload->str, payload->len);
    if (compress) g_string_append_c(pl, 1);

    unsigned char addrtype;
    GString *dec = Base58::decode_check(&addrtype, b58_string);
    CPPUNIT_ASSERT(dec != NULL);

    if (is_testnet) {
        CPPUNIT_ASSERT(addrtype == PRIVKEY_ADDRESS_TEST);
    } else {
        CPPUNIT_ASSERT(addrtype == PRIVKEY_ADDRESS);
    }

    if (compress) {
        CPPUNIT_ASSERT(dec->len == 33);
        CPPUNIT_ASSERT(dec->str[32] == 1);
    } else {
        CPPUNIT_ASSERT(dec->len == 32);
    }

    CPPUNIT_ASSERT(dec->len == pl->len);
    CPPUNIT_ASSERT(memcmp(dec->str, pl->str, pl->len) == 0);

    g_string_free(dec, TRUE);
    g_string_free(pl, TRUE);
    g_string_free(payload, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Base58Test::checkPublicKeyEnc(const char *b58_string, GString *payload,
        const char *address_type_string, bool is_testnet) {

    CPPUNIT_ASSERT(payload != NULL);
    bool addrtype_pubkey = (strcmp(address_type_string, "pubkey") == 0);
    bool addrtype_script = (strcmp(address_type_string, "script") == 0);
    CPPUNIT_ASSERT(addrtype_pubkey || addrtype_script);

    enum AddressType addrtype = (addrtype_pubkey)
        ? (is_testnet) ? PUBKEY_ADDRESS_TEST : PUBKEY_ADDRESS
        : (is_testnet) ? SCRIPT_ADDRESS_TEST : SCRIPT_ADDRESS;

    GString *b58 = Base58::encode_check(
        addrtype, true, payload->str, payload->len);

    if (strcmp(b58->str, b58_string)) {
        fprintf(stderr, "base58: have %s, expected %s\n",
                b58->str, b58_string);
        CPPUNIT_ASSERT(!strcmp(b58->str, b58_string));
    }

    g_string_free(b58, TRUE);
    g_string_free(payload, TRUE);
}

void Base58Test::checkPublicKeyDec(const char* b58_string, GString* payload,
        const char* address_type_string, bool is_testnet) {

    CPPUNIT_ASSERT(payload != NULL);
    bool addrtype_pubkey = (strcmp(address_type_string, "pubkey") == 0);
    bool addrtype_script = (strcmp(address_type_string, "script") == 0);
    CPPUNIT_ASSERT(addrtype_pubkey || addrtype_script);

    enum AddressType addrtype = (addrtype_pubkey)
        ? (is_testnet) ? PUBKEY_ADDRESS_TEST : PUBKEY_ADDRESS
        : (is_testnet) ? SCRIPT_ADDRESS_TEST : SCRIPT_ADDRESS;

    unsigned char addrtype_dec;
    GString *dec = Base58::decode_check(&addrtype_dec, b58_string);
    CPPUNIT_ASSERT(dec != NULL);

    CPPUNIT_ASSERT(addrtype == addrtype_dec);
    CPPUNIT_ASSERT(dec->len == 20);
    CPPUNIT_ASSERT(payload->len == dec->len);
    CPPUNIT_ASSERT(memcmp(payload->str, dec->str, dec->len) == 0);

    g_string_free(dec, TRUE);
    g_string_free(payload, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Base58Test::testKeysMethod() {
    
    const char *base_fn = "Base58Keys.json";
    char *fn = NULL;

    fn = TestLib::filename(base_fn);
    json_t *data = TestLib::read_json(fn);
    CPPUNIT_ASSERT(json_is_array(data));

    size_t n_tests = json_array_size(data);
    for (unsigned int i = 0; i < n_tests; i++) {
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
        CPPUNIT_ASSERT(!j_compress || json_is_true(j_compress) || 
            json_is_false(j_compress));

        bool is_privkey = json_is_true(json_object_get(j_meta, "isPrivkey"));
        bool is_testnet = json_is_true(json_object_get(j_meta, "isTestnet"));

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

    free(fn);
    json_decref(data);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
