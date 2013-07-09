/* 
 * File:   Base58.cpp
 * Author: hsk81
 * 
 * Created on July 5, 2013, 4:52 PM
 */

#include "../include/Base58.h"
#include "../include/Util.h"

#include <ctype.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

namespace Base58 {
    static const char chars[] =
        "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

GString *Base58::encode(const void* data, size_t data_length) {

    const unsigned char *data_ = (unsigned char*)data;
    BIGNUM bn58, bn0, bn, dv, rem;
    BN_CTX *ctx = BN_CTX_new();

    BN_init(&bn58);
    BN_init(&bn0);
    BN_init(&bn);
    BN_init(&dv);
    BN_init(&rem);

    BN_set_word(&bn58, 58);
    BN_set_word(&bn0, 0);

    unsigned char swapbuf[data_length + 1];
    Util::reverse_copy(swapbuf, data_, data_length);
    swapbuf[data_length] = 0;

    Util::BigNum::setvch(&bn, swapbuf, sizeof(swapbuf));
    GString *rs = g_string_sized_new(data_length * 138 / 100 + 1);
    GString *rs_swap;

    while (BN_cmp(&bn, &bn0) > 0) {
        if (!BN_div(&dv, &rem, &bn, &bn58, ctx))
            goto err_out;

        BN_copy(&bn, &dv);
        unsigned int c = BN_get_word(&rem);
        g_string_append_c(rs, Base58::chars[c]);
    }

    for (unsigned int index = 0; index < data_length; index++) {
        if (data_[index] == 0)
            g_string_append_c(rs, Base58::chars[0]);
        else
            break;
    }
    
    rs_swap = g_string_sized_new(rs->len);
    g_string_set_size(rs_swap, rs->len);

    Util::reverse_copy(
        (unsigned char *) rs_swap->str, (unsigned char *) rs->str, rs->len);

    g_string_free(rs, TRUE);
    rs = rs_swap;

out:
    BN_clear_free(&bn58);
    BN_clear_free(&bn0);
    BN_clear_free(&bn);
    BN_clear_free(&dv);
    BN_clear_free(&rem);
    BN_CTX_free(ctx);

    return rs;

err_out:
    g_string_free(rs, TRUE);
    rs = NULL;
    goto out;
}

GString *Base58::encode_check(
        unsigned char address_type, bool has_address_type,
        const void *data, size_t data_length) {

    GString *s = g_string_sized_new(data_length + 1 + 4);
    if (has_address_type) g_string_append_c(s, address_type);
    g_string_append_len(s, (gchar*)data, data_length);

    unsigned char md32[4];
    Util::Hash4(md32, s->str, s->len);

    g_string_append_len(s, (gchar*)md32, 4);
    GString *s_enc = Base58::encode(s->str, s->len);
    g_string_free(s, TRUE);

    return s_enc;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

GString *Base58::decode(const char* string_in) {

    BIGNUM bn58, bn, bnChar;
    BN_CTX *ctx;
    GString *ret = NULL;

    GString *tmp;
    unsigned int leading_zero = 0;
    unsigned int be_sz;
    GString *tmp_be;

    ctx = BN_CTX_new();
    BN_init(&bn58);
    BN_init(&bn);
    BN_init(&bnChar);

    BN_set_word(&bn58, 58);
    BN_set_word(&bn, 0);

    while (isspace(*string_in))
        string_in++;

    for (const char *p = string_in; *p; p++) {
        const char *p1 = strchr(Base58::chars, *p);
        if (!p1) {
            while (isspace(*p)) p++;
            if (*p != '\0') goto out;
            break;
        }

        BN_set_word(&bnChar, p1 - Base58::chars);
        if (!BN_mul(&bn, &bn, &bn58, ctx)) goto out;
        if (!BN_add(&bn, &bn, &bnChar)) goto out;
    }

    tmp = Util::BigNum::getvch(&bn);

    if ((tmp->len >= 2) &&
        (tmp->str[tmp->len - 1] == 0) &&
        ((unsigned char)tmp->str[tmp->len - 2] >= 0x80))
        g_string_set_size(tmp, tmp->len - 1);

    for (const char *p = string_in; *p == Base58::chars[0]; p++)
        leading_zero++;

    be_sz = tmp->len + leading_zero;
    tmp_be = g_string_sized_new(be_sz);
    g_string_set_size(tmp_be, be_sz);
    memset(tmp_be->str, 0, be_sz);

    Util::reverse_copy((unsigned char *)tmp_be->str + leading_zero,
        (unsigned char *)tmp->str, tmp->len);

    g_string_free(tmp, TRUE);
    ret = tmp_be;

out:
    BN_clear_free(&bn58);
    BN_clear_free(&bn);
    BN_clear_free(&bnChar);
    BN_CTX_free(ctx);

    return ret;
}

GString *Base58::decode_check(unsigned char* address_type, 
        const char* string_in) {
    
    unsigned char md32[4];

    /* decode base58 string */
    GString *s = Base58::decode(string_in);
    if (!s) return NULL;
    if (s->len < 4) goto err_out;

    /* validate with trailing hash, then remove hash */
    Util::Hash4(md32, s->str, s->len - 4);
    if (memcmp(md32, &s->str[s->len - 4], 4)) goto err_out;
    g_string_set_size(s, s->len - 4);

    /* if address_type requested, remove from front of data string */
    if (address_type) {
        *address_type = (unsigned char)s->str[0];
        g_string_erase(s, 0, 1);
    }

    return s;

err_out:
    g_string_free(s, TRUE);
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
