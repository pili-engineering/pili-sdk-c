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

    size_t dst_encoded_len = urlsafe_b64_encode(digest, digest_len, NULL, 0) + 1;
    char *dst_encoded_str = (char *) malloc(dst_encoded_len);
    memset(dst_encoded_str, 0, dst_encoded_len);
    urlsafe_b64_encode(digest, digest_len, dst_encoded_str, dst_encoded_len);

    size_t final_token_len = strlen(access_key) + strlen(dst_encoded_str) + 2;
    char *final_token = (char *) malloc(sizeof(char) * (final_token_len));
    sprintf(final_token, "%s:%s", access_key, dst_encoded_str);
    free((void *) dst_encoded_str);
    return final_token;
}

const char *pili_sign_request(const char *access_key, const char *secret_key, const char *host, const char *method,
                              const char *path, const char *content_type, const char *body, const char *query) {
    char *data_to_sign_fmt = "%s %s%s%s\nHost: %s\nContent-Type: %s\n\n%s";
    size_t data_to_sign_len = snprintf(NULL, 0, data_to_sign_fmt, method, path,
                                       query ? "?" : "", query ? query : "", host, content_type, body ? body : "") + 1;
    char *data_to_sign = (char *) malloc(sizeof(char) * data_to_sign_len);
    sprintf(data_to_sign, data_to_sign_fmt, method, path,
            query ? "?" : "", query ? query : "", host, content_type, body ? body : "");
    printf("%s\n", data_to_sign);
    const char *sign = pili_hmac_sha1(access_key, secret_key, data_to_sign);
    size_t dst_token_len = strlen(sign) + 7;
    char *final_token = (char *) malloc(dst_token_len);

    sprintf(final_token, "Qiniu %s", sign);
    free((void *) data_to_sign);
    free((void *) sign);
    return final_token;
}