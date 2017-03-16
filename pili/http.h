//
// Created by jemy on 15/03/2017.
//

#ifndef PILI_SDK_C_HTTP_H
#define PILI_SDK_C_HTTP_H

#include <curl/curl.h>

static const char *PILI_API_HOST = "pili.qiniuapi.com";
static const char *PILI_API_ADDRESS = "http://pili.qiniuapi.com";
static const char *PILI_MIME_URLENCODED = "application/x-www-form-urlencoded";
static const char *PILI_MIME_JSON = "application/json";
static const char *PILI_USER_AGENT = "pili-sdk-c/v1.0.0";

struct pili_memory_buffer {
    char *memory;
    size_t size;
};

int pili_post_request(const char *url, const struct curl_slist *headers, const char *body,
                      int *resp_code, struct pili_memory_buffer *resp_body, size_t resp_body_len, char *error);

int pili_get_request(const char *url, const struct curl_slist *headers,
                     int *resp_code, struct pili_memory_buffer *resp_body, size_t resp_body_len, char *error);

#endif //PILI_SDK_C_HTTP_H
