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

const char *pili_sign_request(const char *access_key, const char *secret_key, const char *host, const char *method,
                              const char *path, const char *content_type, const char *body, const char *query) {
    char data_to_sign[1024];
    memset(data_to_sign, NULL, 1024);
    char *data_offset = data_to_sign;

    //method
    strcpy(data_offset, method);
    data_offset += strlen(method);

    //space
    *data_offset = ' ';
    data_offset += 1;

    //path
    strcpy(data_offset, path);
    data_offset += strlen(path);

    //query
    if (query) {
        *data_offset = '?';
        data_offset += 1;
        strcpy(data_offset, query);
        data_offset += strlen(query);
    }


    //host
    strcpy(data_offset, "\nHost: ");
    data_offset += 7;
    strcpy(data_offset, host);
    data_offset += strlen(host);

    //content type
    if (content_type) {
        strcpy(data_offset, "\nContent-Type: ");
        data_offset += 15;
        strcpy(data_offset, content_type);
        data_offset += strlen(content_type);
    }

    strcpy(data_offset, "\n\n");
    data_offset += 2;

    if (body) {
        strcpy(data_offset, body);
    }

    const char *sign = pili_hmac_sha1(access_key, secret_key, data_to_sign);
    size_t dst_token_len = strlen(sign) + 7;
    char *final_token = (char *) malloc(dst_token_len);
    memset(final_token, 0, dst_token_len);

    sprintf(final_token, "Qiniu %s", sign);
    free((void *) sign);
    return final_token;
}