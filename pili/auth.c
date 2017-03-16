//
// Created by jemy on 16/03/2017.
//

#include "auth.h"
#include "urlsafe_b64.h"
#include <openssl/hmac.h>
#include <openssl/engine.h>
#include <string.h>

const char *pili_hmac_sha1(const char *access_key, const char *secret_key, const char *data) {
    char digest[EVP_MAX_MD_SIZE + 1];
    unsigned int digest_len = EVP_MAX_MD_SIZE;

    HMAC_CTX ctx;
    HMAC_CTX_init(&ctx);

    HMAC_Init_ex(&ctx, secret_key, strlen(secret_key), EVP_sha1(), NULL);
    HMAC_Update(&ctx, data, strlen(data));
    HMAC_Final(&ctx, digest, &digest_len);
    HMAC_cleanup(&ctx);

    size_t dst_encoded_len = urlsafe_b64_encode(digest, digest_len, NULL, 0);
    char *dst_encoded_str = (char *) malloc(dst_encoded_len + 1);
    memset(dst_encoded_str, 0, dst_encoded_len + 1);
    urlsafe_b64_encode(digest, digest_len, dst_encoded_str, dst_encoded_len);

    size_t final_token_len = strlen(access_key) + strlen(dst_encoded_str) + 1;
    char *final_token = (char *) malloc(sizeof(char) * (final_token_len + 1));
    memset(final_token, 0, final_token_len + 1);
    sprintf(final_token, "%s:%s", access_key, dst_encoded_str);
    free((void *) dst_encoded_str);
    return final_token;
}