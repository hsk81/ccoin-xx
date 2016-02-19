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

    const guchar *_pointer = (guchar*) pointer;
    BIGNUM _zero, _divisor, _dividend, _quotient, _remainder;
    BN_CTX *_context = BN_CTX_new();

    BN_init(&_zero);
    BN_init(&_divisor);
    BN_init(&_dividend);
    BN_init(&_quotient);
    BN_init(&_remainder);

    BN_set_word(&_zero, 0);
    BN_set_word(&_divisor, 58);

    guchar _puch_swap[size + 1];
    Util::reverse_copy(_puch_swap, _pointer, size);
    _puch_swap[size] = 0;

    Util::BigNum::setvch(&_dividend, _puch_swap, sizeof (_puch_swap));
    GString *_result = g_string_sized_new(size * 138 / 100 + 1);
    GString *_result_swap;

    while (BN_cmp(&_dividend, &_zero) > 0) {
        if (!BN_div(&_quotient, &_remainder, &_dividend, &_divisor, _context)) {
            goto exit_error;
        }
        BN_copy(&_dividend, &_quotient);
        gint _index = BN_get_word(&_remainder);
        g_string_append_c(_result, Base58::chars[_index]);
    }

    for (guint index = 0; index < size; index++) {
        if (_pointer[index] == 0) {
            g_string_append_c(_result, Base58::chars[0]);
        } else {
            break;
        }
    }

    _result_swap = g_string_sized_new(_result->len);
    g_string_set_size(_result_swap, _result->len);

    Util::reverse_copy(
            (guchar*) _result_swap->str, (guchar*) _result->str, _result->len);

    g_string_free(_result, TRUE);
    _result = _result_swap;

exit:
    BN_clear_free(&_divisor);
    BN_clear_free(&_zero);
    BN_clear_free(&_dividend);
    BN_clear_free(&_quotient);
    BN_clear_free(&_remainder);
    BN_CTX_free(_context);

    return _result;

exit_error:
    g_string_free(_result, TRUE);
    _result = NULL;
    goto exit;
}

GString *Base58::encode_check(
        gconstpointer pointer, gsize size,
        guchar address_type, gboolean has_address_type) {

    GString *_string = g_string_sized_new(size + 1 + 4);
    if (has_address_type) g_string_append_c(_string, address_type);
    g_string_append_len(_string, (gchar*) pointer, size);

    guchar _md32[4];
    Util::Hash4(_md32, _string->str, _string->len);

    g_string_append_len(_string, (gchar*) _md32, 4);
    GString *_string_enc = Base58::encode(_string->str, _string->len);
    g_string_free(_string, TRUE);

    return _string_enc;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

GString *Base58::decode(const gchar* pch) {

    GString *_string = NULL;
    BIGNUM _bn58, _bn, _bn_char;
    BN_CTX *context;

    guint _leading_zero = 0;
    GString *_tmp_string;
    guint _be_length;
    GString *_be_string;

    context = BN_CTX_new();
    BN_init(&_bn58);
    BN_init(&_bn);
    BN_init(&_bn_char);

    BN_set_word(&_bn58, 58);
    BN_set_word(&_bn, 0);

    while (isspace(*pch)) {
        pch++;
    }

    for (const gchar *_pch = _pch; *_pch; _pch++) {
        const gchar *_pch_b58 = strchr(Base58::chars, *_pch);
        if (!_pch_b58) {
            while (isspace(*_pch)) _pch++;
            if (*_pch != '\0') goto exit;
            break;
        }

        BN_set_word(&_bn_char, _pch_b58 - Base58::chars);
        if (!BN_mul(&_bn, &_bn, &_bn58, context)) goto exit;
        if (!BN_add(&_bn, &_bn, &_bn_char)) goto exit;
    }

    _tmp_string = Util::BigNum::getvch(&_bn);

    if ((_tmp_string->len >= 2) &&
            (_tmp_string->str[_tmp_string->len - 1] == 0) &&
            ((guchar) _tmp_string->str[_tmp_string->len - 2] >= 0x80)) {
        g_string_set_size(_tmp_string, _tmp_string->len - 1);
    }

    for (const gchar *_pch = _pch; *_pch == Base58::chars[0]; _pch++) {
        _leading_zero++;
    }

    _be_length = _tmp_string->len + _leading_zero;
    _be_string = g_string_sized_new(_be_length);
    g_string_set_size(_be_string, _be_length);
    memset(_be_string->str, 0, _be_length);

    Util::reverse_copy(
            (guchar*) _be_string->str + _leading_zero,
            (guchar*) _tmp_string->str, _tmp_string->len);

    g_string_free(_tmp_string, TRUE);
    _string = _be_string;

exit:
    BN_clear_free(&_bn58);
    BN_clear_free(&_bn);
    BN_clear_free(&_bn_char);
    BN_CTX_free(context);

    return _string;
}

GString *Base58::decode_check(
        const gchar* pch, guchar* address_type) {

    GString *_string = Base58::decode(pch);
    if (!_string) return NULL;
    if (_string->len < 4) goto exit_error;

    guchar _md32[4];

    Util::Hash4(_md32, _string->str, _string->len - 4);
    if (memcmp(_md32, &_string->str[_string->len - 4], 4)) {
        goto exit_error;
    }

    g_string_set_size(_string, _string->len - 4);

    if (address_type) {
        *address_type = (guchar) _string->str[0];
        g_string_erase(_string, 0, 1);
    }

    return _string;

exit_error:
    g_string_free(_string, TRUE);
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
