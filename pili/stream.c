//
// Created by jemy on 16/03/2017.
//

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
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

    int result = pili_post_request(url, headers, body, &resp_code, &resp_body, 1024, error);
    if (result != 0) {
        ret = -1;
    } else {
        if (resp_code != 200) {
            cJSON *resp_root = cJSON_Parse(resp_body.memory);
            if (resp_root) {
                if (error) {
                    cJSON *errObj = cJSON_GetObjectItem(resp_root, "error");
                    if (errObj) {
                        sprintf(error, "%d %s", resp_code, cJSON_Print(errObj));
                        free((void *) errObj);
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