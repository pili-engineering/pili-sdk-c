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
                    }
                }
                cJSON_Delete(resp_root);
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
    path_len += 19;
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
                    }
                }
            } else {
                if (attribute) {
                    cJSON *created_at_obj = cJSON_GetObjectItem(resp_root, "createdAt");
                    if (created_at_obj) {
                        char *created_at = cJSON_Print(created_at_obj);
                        attribute->created_at = strtol(created_at, NULL, 10);
                        free((void *) created_at);
                    }
                    cJSON *updated_at_obj = cJSON_GetObjectItem(resp_root, "updatedAt");
                    if (updated_at_obj) {
                        char *updated_at = cJSON_Print(updated_at_obj);
                        attribute->updated_at = strtol(updated_at, NULL, 0);
                        free((void *) updated_at);
                    }
                    cJSON *expire_at_obj = cJSON_GetObjectItem(resp_root, "expireAt");
                    if (expire_at_obj) {
                        char *expire_at = cJSON_Print(expire_at_obj);
                        attribute->expire_at = strtol(expire_at, NULL, 0);
                        free((void *) expire_at);
                    }
                    cJSON *disabled_till_obj = cJSON_GetObjectItem(resp_root, "disabledTill");
                    if (disabled_till_obj) {
                        char *disabled_till = cJSON_Print(disabled_till_obj);
                        attribute->disabled_till = strtol(disabled_till, NULL, 0);
                        free((void *) disabled_till);
                    }
                }
            }
            cJSON_Delete(resp_root);
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

int pili_stream_status(const char *access_key, const char *secret_key, const char *hub_name, const char *stream_key,
                       struct pili_stream_status *status, char *error) {
    int ret = 0;
    size_t encoded_stream_key_len = urlsafe_b64_encode(stream_key, strlen(stream_key), NULL, 0);
    char *encoded_stream_key = (char *) malloc(sizeof(char) * (encoded_stream_key_len + 1));
    memset(encoded_stream_key, 0, encoded_stream_key_len + 1);
    urlsafe_b64_encode(stream_key, strlen(stream_key), encoded_stream_key, encoded_stream_key_len);

    size_t path_len = strlen(hub_name) + strlen(encoded_stream_key);
    path_len += 24;
    char *path = (char *) malloc(sizeof(char) * path_len);
    memset(path, 0, path_len);
    sprintf(path, "/v2/hubs/%s/streams/%s/live", hub_name, encoded_stream_key);

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
                    }
                }
            } else {
                if (status) {
                    cJSON *start_at_obj = cJSON_GetObjectItem(resp_root, "startAt");
                    if (start_at_obj) {
                        char *start_at = cJSON_Print(start_at_obj);
                        status->start_at = strtol(start_at, NULL, 10);
                        free((void *) start_at);
                    }

                    cJSON *client_ip_obj = cJSON_GetObjectItem(resp_root, "clientIP");
                    if (client_ip_obj) {
                        char *client_ip = cJSON_Print(client_ip_obj);
                        memset(status->client_ip, 0, sizeof(status->client_ip));
                        strcpy(status->client_ip, client_ip);
                        free((void *) client_ip);
                    }

                    cJSON *bps_obj = cJSON_GetObjectItem(resp_root, "bps");
                    if (bps_obj) {
                        char *bps = cJSON_Print(bps_obj);
                        status->bps = strtol(bps, NULL, 10);
                        free((void *) bps);
                    }
                    printf("%ld\n", status->bps);
                    cJSON *fps_root_obj = cJSON_GetObjectItem(resp_root, "fps");
                    if (fps_root_obj) {
                        //audio fps
                        cJSON *audio_fps_obj = cJSON_GetObjectItem(fps_root_obj, "audio");
                        char *audio_fps = cJSON_Print(audio_fps_obj);
                        status->audio_fps = strtol(audio_fps, NULL, 10);
                        free((void *) audio_fps);

                        //video fps
                        cJSON *video_fps_obj = cJSON_GetObjectItem(fps_root_obj, "video");
                        char *video_fps = cJSON_Print(video_fps_obj);
                        status->video_fps = strtol(video_fps, NULL, 10);
                        free((void *) video_fps);

                        //data fps
                        cJSON *data_fps_obj = cJSON_GetObjectItem(fps_root_obj, "data");
                        char *data_fps = cJSON_Print(data_fps_obj);
                        status->data_fps = strtol(data_fps, NULL, 10);
                        free((void *) data_fps);
                    }
                }
            }
            cJSON_Delete(resp_root);
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