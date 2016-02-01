/* 
 * File:   Base58.cpp
 * Author: hsk81
 */

#include "../include/Base58.h"
#include "../include/Util.h"

#include <ctype.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

namespace Base58 {
    static const gchar chars[] =
            "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

GString *Base58::encode(
        gconstpointer data_pointer, gsize data_length) {

    const guchar *data = (guchar*) data_pointer;
    BIGNUM bn58, bn0, bn, dv, rem;
    BN_CTX *context = BN_CTX_new();

    BN_init(&bn58);
    BN_init(&bn0);
    BN_init(&bn);
    BN_init(&dv);
    BN_init(&rem);

    BN_set_word(&bn58, 58);
    BN_set_word(&bn0, 0);

    guchar swapbuf[data_length + 1];
    Util::reverse_copy(swapbuf, data, data_length);
    swapbuf[data_length] = 0;

    Util::BigNum::setvch(&bn, swapbuf, sizeof (swapbuf));
    GString *result = g_string_sized_new(data_length * 138 / 100 + 1);
    GString *result_swap;

    while (BN_cmp(&bn, &bn0) > 0) {
        if (!BN_div(&dv, &rem, &bn, &bn58, context)) {
            goto exit_error;
        }

        BN_copy(&bn, &dv);
        gint c = BN_get_word(&rem);
        g_string_append_c(result, Base58::chars[c]);
    }

    for (guint index = 0; index < data_length; index++) {
        if (data[index] == 0) {
            g_string_append_c(result, Base58::chars[0]);
        } else {
            break;
        }
    }

    result_swap = g_string_sized_new(result->len);
    g_string_set_size(result_swap, result->len);

    Util::reverse_copy(
            (guchar *) result_swap->str, (guchar *) result->str, result->len);

    g_string_free(result, TRUE);
    result = result_swap;

exit:
    BN_clear_free(&bn58);
    BN_clear_free(&bn0);
    BN_clear_free(&bn);
    BN_clear_free(&dv);
    BN_clear_free(&rem);
    BN_CTX_free(context);

    return result;

exit_error:
    g_string_free(result, TRUE);
    result = NULL;
    goto exit;
}

GString *Base58::encode_check(
        guchar address_type, bool has_address_type,
        gconstpointer data_pointer, gsize data_length) {

    GString *string = g_string_sized_new(data_length + 1 + 4);
    if (has_address_type) g_string_append_c(string, address_type);
    g_string_append_len(string, (gchar*) data_pointer, data_length);

    guchar md32[4];
    Util::Hash4(md32, string->str, string->len);

    g_string_append_len(string, (gchar*) md32, 4);
    GString *s_enc = Base58::encode(string->str, string->len);
    g_string_free(string, TRUE);

    return s_enc;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

GString *Base58::decode(const gchar* characters) {

    BIGNUM bn58, bn, bn_char;
    BN_CTX *context;
    GString *result = NULL;

    GString *tmp;
    guint leading_zero = 0;
    guint be_size;
    GString *tmp_be;

    context = BN_CTX_new();
    BN_init(&bn58);
    BN_init(&bn);
    BN_init(&bn_char);

    BN_set_word(&bn58, 58);
    BN_set_word(&bn, 0);

    while (isspace(*characters)) {
        characters++;
    }

    for (const gchar *pch = characters; *pch; pch++) {
        const gchar *pch_ = strchr(Base58::chars, *pch);
        if (!pch_) {
            while (isspace(*pch)) pch++;
            if (*pch != '\0') goto exit;
            break;
        }

        BN_set_word(&bn_char, pch_ - Base58::chars);
        if (!BN_mul(&bn, &bn, &bn58, context)) goto exit;
        if (!BN_add(&bn, &bn, &bn_char)) goto exit;
    }

    tmp = Util::BigNum::getvch(&bn);

    if ((tmp->len >= 2) &&
            (tmp->str[tmp->len - 1] == 0) &&
            ((guchar) tmp->str[tmp->len - 2] >= 0x80)) {
        g_string_set_size(tmp, tmp->len - 1);
    }

    for (const gchar *pch = characters; *pch == Base58::chars[0]; pch++) {
        leading_zero++;
    }

    be_size = tmp->len + leading_zero;
    tmp_be = g_string_sized_new(be_size);
    g_string_set_size(tmp_be, be_size);
    memset(tmp_be->str, 0, be_size);

    Util::reverse_copy(
            (guchar *) tmp_be->str + leading_zero, 
            (guchar *) tmp->str, tmp->len);

    g_string_free(tmp, TRUE);
    result = tmp_be;

exit:
    BN_clear_free(&bn58);
    BN_clear_free(&bn);
    BN_clear_free(&bn_char);
    BN_CTX_free(context);

    return result;
}

GString *Base58::decode_check(
        guchar* address_type, const gchar* characters) {

    guchar md32[4];

    // decode base58 string
    GString *string = Base58::decode(characters);
    if (!string) return NULL;
    if (string->len < 4) goto exit_error;

    // validate with trailing hash, then remove hash
    Util::Hash4(md32, string->str, string->len - 4);
    if (memcmp(md32, &string->str[string->len - 4], 4)) goto exit_error;
    g_string_set_size(string, string->len - 4);

    // if address_type requested, remove from front of data string
    if (address_type) {
        *address_type = (guchar) string->str[0];
        g_string_erase(string, 0, 1);
    }

    return string;

exit_error:
    g_string_free(string, TRUE);
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
