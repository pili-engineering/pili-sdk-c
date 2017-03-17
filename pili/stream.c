//
// Created by jemy on 16/03/2017.
//

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <urlsafe_b64.h>
#include "stream.h"
#include "auth.h"
#include "http.h"


int pili_create_stream(const char *access_key, const char *secret_key, const char *hub_name, const char *stream_key,
                       char *error) {
    int ret = 0;
    size_t path_len = strlen(hub_name) + strlen(stream_key);
    path_len += 18;
    char *path = (char *) malloc(sizeof(char) * path_len);
    memset(path, 0, path_len);
    sprintf(path, "/v2/hubs/%s/streams", hub_name);

    //make body
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "key", cJSON_CreateString(stream_key));
    const char *body = cJSON_Print(root);

    const char *token = pili_sign_request(access_key, secret_key, PILI_API_HOST, "POST", path, PILI_MIME_JSON, body, 0);
    struct curl_slist *headers;

    char *auth_header = (char *) malloc(strlen(token) + 15);
    sprintf(auth_header, "Authorization: %s", token);
    headers = curl_slist_append(NULL, auth_header);
    headers = curl_slist_append(headers, "Content-Type: application/json");

    size_t url_len = strlen(PILI_API_ADDRESS) + strlen(path) + 1;
    char *url = (char *) malloc(url_len);
    memset(url, 0, url_len);
    sprintf(url, "%s%s", PILI_API_ADDRESS, path);

    int resp_code;
    struct pili_memory_buffer resp_body;
    resp_body.memory = malloc(1);
    resp_body.size = 0;

    int result = pili_post_request(url, headers, body, &resp_code, &resp_body, error);
    if (result != 0) {
        ret = -1;
    } else {
        if (resp_code != 200) {
            cJSON *resp_root = cJSON_Parse(resp_body.memory);
            if (resp_root) {
                if (error) {
                    cJSON *err_obj = cJSON_GetObjectItem(resp_root, "error");
                    if (err_obj) {
                        sprintf(error, "%d %s", resp_code, cJSON_Print(err_obj));
                        free((void *) err_obj);
                    }
                }
                free((void *) resp_root);
            }
        }
    }

    //clean
    curl_slist_free_all(headers);
    free((void *) token);
    free((void *) body);
    free((void *) path);
    free((void *) root);
    free((void *) resp_body.memory);
    return ret;
}

int pili_stream_attribute(const char *access_key, const char *secret_key, const char *hub_name, const char *stream_key,
                          struct pili_stream_attribute *attribute, char *error) {
    int ret = 0;
    size_t encoded_stream_key_len = urlsafe_b64_encode(stream_key, strlen(stream_key), NULL, 0);
    char *encoded_stream_key = (char *) malloc(sizeof(char) * (encoded_stream_key_len + 1));
    memset(encoded_stream_key, 0, encoded_stream_key_len + 1);
    urlsafe_b64_encode(stream_key, strlen(stream_key), encoded_stream_key, encoded_stream_key_len);


    size_t path_len = strlen(hub_name) + strlen(encoded_stream_key);
    path_len += 18;
    char *path = (char *) malloc(sizeof(char) * path_len);
    memset(path, 0, path_len);
    sprintf(path, "/v2/hubs/%s/streams/%s", hub_name, encoded_stream_key);

    const char *token = pili_sign_request(access_key, secret_key, PILI_API_HOST, "GET", path, PILI_MIME_URLENCODED, 0,
                                          0);
    struct curl_slist *headers;
    char *auth_header = (char *) malloc(strlen(token) + 15);
    sprintf(auth_header, "Authorization: %s", token);
    headers = curl_slist_append(NULL, auth_header);
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

    size_t url_len = strlen(PILI_API_ADDRESS) + strlen(path) + 1;
    char *url = (char *) malloc(url_len);
    memset(url, 0, url_len);
    sprintf(url, "%s%s", PILI_API_ADDRESS, path);

    int resp_code;
    struct pili_memory_buffer resp_body;
    resp_body.memory = malloc(1);
    resp_body.size = 0;

    int result = pili_get_request(url, headers, &resp_code, &resp_body, error);
    if (result != 0) {
        ret = -1;
    } else {
        cJSON *resp_root = cJSON_Parse(resp_body.memory);
        if (resp_root) {
            if (resp_code != 200) {
                if (error) {
                    cJSON *err_obj = cJSON_GetObjectItem(resp_root, "error");
                    if (err_obj) {
                        sprintf(error, "%d %s", resp_code, cJSON_Print(err_obj));
                        free((void *) err_obj);
                    }
                }
            } else {
                if (attribute) {
                    cJSON *created_at_obj = cJSON_GetObjectItem(resp_root, "createdAt");
                    if (created_at_obj) {
                        attribute->created_at = strtol(cJSON_Print(created_at_obj), NULL, 10);
                    }
                    cJSON *updated_at_obj = cJSON_GetObjectItem(resp_root, "updatedAt");
                    if (updated_at_obj) {
                        attribute->updated_at = strtol(cJSON_Print(updated_at_obj), NULL, 0);
                    }
                    cJSON *expire_at_obj = cJSON_GetObjectItem(resp_root, "expireAt");
                    if (expire_at_obj) {
                        attribute->expire_at = strtol(cJSON_Print(expire_at_obj), NULL, 0);
                    }
                    cJSON *disabled_till_obj = cJSON_GetObjectItem(resp_root, "disabledTill");
                    if (disabled_till_obj) {
                        attribute->disabled_till = strtol(cJSON_Print(disabled_till_obj), NULL, 0);
                    }
                }
            }
            free((void *) resp_root);
        }
    }

    //clean
    curl_slist_free_all(headers);
    free((void *) encoded_stream_key);
    free((void *) token);
    free((void *) path);
    free((void *) resp_body.memory);
    return ret;
}