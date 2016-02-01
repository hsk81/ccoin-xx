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

GString *Base58::encode(gconstpointer data, gsize size) {

    const guchar *data_pointer = (guchar*) data;
    BIGNUM zero, divisor, dividend, quotient, remainder;
    BN_CTX *context = BN_CTX_new();

    BN_init(&zero);
    BN_init(&divisor);
    BN_init(&dividend);
    BN_init(&quotient);
    BN_init(&remainder);

    BN_set_word(&zero, 0);
    BN_set_word(&divisor, 58);

    guchar swapbuf[size + 1];
    Util::reverse_copy(swapbuf, data_pointer, size);
    swapbuf[size] = 0;

    Util::BigNum::setvch(&dividend, swapbuf, sizeof (swapbuf));
    GString *result = g_string_sized_new(size * 138 / 100 + 1);
    GString *result_swap;

    while (BN_cmp(&dividend, &zero) > 0) {
        if (!BN_div(&quotient, &remainder, &dividend, &divisor, context)) {
            goto exit_error;
        }

        BN_copy(&dividend, &quotient);
        gint index = BN_get_word(&remainder);
        g_string_append_c(result, Base58::chars[index]);
    }

    for (guint index = 0; index < size; index++) {
        if (data_pointer[index] == 0) {
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
    BN_clear_free(&divisor);
    BN_clear_free(&zero);
    BN_clear_free(&dividend);
    BN_clear_free(&quotient);
    BN_clear_free(&remainder);
    BN_CTX_free(context);

    return result;

exit_error:
    g_string_free(result, TRUE);
    result = NULL;
    goto exit;
}

GString *Base58::encode_check(
        guchar address_type, gboolean has_address_type,
        gconstpointer data_pointer, gsize data_length) {

    GString *string = g_string_sized_new(data_length + 1 + 4);
    if (has_address_type) g_string_append_c(string, address_type);
    g_string_append_len(string, (gchar*) data_pointer, data_length);

    guchar md32[4];
    Util::Hash4(md32, string->str, string->len);

    g_string_append_len(string, (gchar*) md32, 4);
    GString *string_enc = Base58::encode(string->str, string->len);
    g_string_free(string, TRUE);

    return string_enc;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

GString *Base58::decode(const gchar* characters) {

    GString *result = NULL;
    BIGNUM bn58, bn, bn_char;
    BN_CTX *context;

    guint leading_zero = 0;
    GString *tmp_string;
    guint be_length;
    GString *be_string;

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
        const gchar *pch_b58 = strchr(Base58::chars, *pch);
        if (!pch_b58) {
            while (isspace(*pch)) pch++;
            if (*pch != '\0') goto exit;
            break;
        }

        BN_set_word(&bn_char, pch_b58 - Base58::chars);
        if (!BN_mul(&bn, &bn, &bn58, context)) goto exit;
        if (!BN_add(&bn, &bn, &bn_char)) goto exit;
    }

    tmp_string = Util::BigNum::getvch(&bn);

    if ((tmp_string->len >= 2) &&
            (tmp_string->str[tmp_string->len - 1] == 0) &&
            ((guchar) tmp_string->str[tmp_string->len - 2] >= 0x80)) {
        g_string_set_size(tmp_string, tmp_string->len - 1);
    }

    for (const gchar *pch = characters; *pch == Base58::chars[0]; pch++) {
        leading_zero++;
    }

    be_length = tmp_string->len + leading_zero;
    be_string = g_string_sized_new(be_length);
    g_string_set_size(be_string, be_length);
    memset(be_string->str, 0, be_length);

    Util::reverse_copy(
            (guchar *) be_string->str + leading_zero, 
            (guchar *) tmp_string->str, tmp_string->len);

    g_string_free(tmp_string, TRUE);
    result = be_string;

exit:
    BN_clear_free(&bn58);
    BN_clear_free(&bn);
    BN_clear_free(&bn_char);
    BN_CTX_free(context);

    return result;
}

GString *Base58::decode_check(
        guchar* address_type, const gchar* characters) {

    GString *result = Base58::decode(characters);
    if (!result) return NULL;
    if (result->len < 4) goto exit_error;

    guchar md32[4];

    Util::Hash4(md32, result->str, result->len - 4);
    if (memcmp(md32, &result->str[result->len - 4], 4)) goto exit_error;
    g_string_set_size(result, result->len - 4);

    if (address_type) {
        *address_type = (guchar) result->str[0];
        g_string_erase(result, 0, 1);
    }

    return result;

exit_error:
    g_string_free(result, TRUE);
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
