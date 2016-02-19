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

GString *Base58::encode(gconstpointer pointer, gsize size) {

    const guchar *puch = (guchar*) pointer;
    BIGNUM zero, divisor, dividend, quotient, remainder;
    BN_CTX *context = BN_CTX_new();

    BN_init(&zero);
    BN_init(&divisor);
    BN_init(&dividend);
    BN_init(&quotient);
    BN_init(&remainder);

    BN_set_word(&zero, 0);
    BN_set_word(&divisor, 58);

    guchar puch_swap[size + 1];
    Util::reverse_copy(puch_swap, puch, size);
    puch_swap[size] = 0;

    Util::BigNum::setvch(&dividend, puch_swap, sizeof (puch_swap));
    GString *string = g_string_sized_new(size * 138 / 100 + 1);
    GString *string_swap;

    while (BN_cmp(&dividend, &zero) > 0) {
        if (!BN_div(&quotient, &remainder, &dividend, &divisor, context)) {
            goto exit_error;
        }
        BN_copy(&dividend, &quotient);
        gint index = BN_get_word(&remainder);
        g_string_append_c(string, Base58::chars[index]);
    }

    for (guint i = 0; i < size; i++) {
        if (puch[i] == 0) {
            g_string_append_c(string, Base58::chars[0]);
        } else {
            break;
        }
    }

    string_swap = g_string_sized_new(string->len);
    g_string_set_size(string_swap, string->len);

    Util::reverse_copy(
            (guchar*) string_swap->str, (guchar*) string->str, string->len);

    g_string_free(string, TRUE);
    string = string_swap;
exit:
    BN_clear_free(&divisor);
    BN_clear_free(&zero);
    BN_clear_free(&dividend);
    BN_clear_free(&quotient);
    BN_clear_free(&remainder);
    BN_CTX_free(context);

    return string;
exit_error:
    g_string_free(string, TRUE);
    string = NULL;
    goto exit;
}

GString *Base58::encode_check(
        gconstpointer pointer, gsize size,
        guchar address_type, gboolean has_address_type) {

    GString *string = g_string_sized_new(size + 4 + 1);
    if (has_address_type) g_string_append_c(string, address_type);
    g_string_append_len(string, (gchar*) pointer, size);

    guchar md32[4];
    Util::Hash4(md32, string->str, string->len);

    g_string_append_len(string, (gchar*) md32, 4);
    GString *string_enc = Base58::encode(string->str, string->len);
    g_string_free(string, TRUE);

    return string_enc;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

GString *Base58::decode(const gchar *chars) {

    GString *string = NULL;
    BIGNUM bn58, bn, bn_char;
    BN_CTX *context;

    guint leading_zero = 0;
    GString *string_tmp;
    guint size_be;
    GString *string_be;

    context = BN_CTX_new();
    BN_init(&bn58);
    BN_init(&bn);
    BN_init(&bn_char);

    BN_set_word(&bn58, 58);
    BN_set_word(&bn, 0);

    while (isspace(*chars)) {
        chars++;
    }

    for (const gchar *pch = chars; *pch; pch++) {
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

    string_tmp = Util::BigNum::getvch(&bn);

    if ((string_tmp->len >= 2) &&
            (string_tmp->str[string_tmp->len - 1] == 0) &&
            ((guchar) string_tmp->str[string_tmp->len - 2] >= 0x80)) {
        g_string_set_size(string_tmp, string_tmp->len - 1);
    }

    for (const gchar *pch = chars; *pch == Base58::chars[0]; pch++) {
        leading_zero++;
    }

    size_be = string_tmp->len + leading_zero;
    string_be = g_string_sized_new(size_be);
    g_string_set_size(string_be, size_be);
    memset(string_be->str, 0, size_be);

    Util::reverse_copy(
            (guchar*) string_be->str + leading_zero,
            (guchar*) string_tmp->str, string_tmp->len);

    g_string_free(string_tmp, TRUE);
    string = string_be;
exit:
    BN_clear_free(&bn58);
    BN_clear_free(&bn);
    BN_clear_free(&bn_char);
    BN_CTX_free(context);

    return string;
}

GString *Base58::decode_check(const gchar *chars, guchar *address_type) {

    GString *string = Base58::decode(chars);
    if (!string) return NULL;
    if (string->len < 4) goto exit_error;

    guchar md32[4];

    Util::Hash4(md32, string->str, string->len - 4);
    if (memcmp(md32, &string->str[string->len - 4], 4)) goto exit_error;
    g_string_set_size(string, string->len - 4);

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
