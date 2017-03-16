//
// Created by jemy on 16/03/2017.
//


#ifndef PILI_SDK_C_AUTH_H
#define PILI_SDK_C_AUTH_H

#include <stdio.h>

const char *pili_hmac_sha1(const char *access_key, const char *secret_key, const char *data);

const char *pili_sign_request(const char *access_key, const char *secret_key, const char *host, const char *method,
                              const char *path, const char *content_type, const char *body, const char *query);

#endif //PILI_SDK_C_AUTH_H
