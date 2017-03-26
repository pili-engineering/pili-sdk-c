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


int pili_create_stream(const char *access_key, const char *secret_key, const char *hub_name,
                       const char *stream_key,
                       char *error) {
    int ret = 0;

    //create path
    char *path_fmt = "/v2/hubs/%s/streams";
    int path_len = snprintf(NULL, 0, path_fmt, hub_name) + 1;
    char *path = (char *) malloc(sizeof(char) * path_len);
    sprintf(path, path_fmt, hub_name);

    //make body
    cJSON *req_root = cJSON_CreateObject();
    cJSON_AddItemToObject(req_root, "key", cJSON_CreateString(stream_key));
    const char *body = cJSON_PrintUnformatted(req_root);

    //make headers
    const char *token = pili_sign_request(access_key, secret_key, PILI_API_HOST, "POST", path, PILI_MIME_JSON, body, 0);
    struct curl_slist *headers;

    char *auth_fmt = "Authorization: %s";
    int auth_len = snprintf(NULL, 0, auth_fmt, token) + 1;
    char *auth_header = (char *) malloc(auth_len);
    sprintf(auth_header, auth_fmt, token);

    headers = curl_slist_append(NULL, auth_header);
    headers = curl_slist_append(headers, "Content-Type: application/json");

    size_t url_len = strlen(PILI_API_ADDRESS) + strlen(path) + 1;
    char *url = (char *) malloc(url_len);
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
                        sprintf(error, "%d %s", resp_code, err_obj->valuestring);
                    }
                }
                cJSON_Delete(resp_root);
            }
            ret = -1;
        }
    }

    //clean
    curl_slist_free_all(headers);
    free((void *) req_root);
    free((void *) auth_header);
    free((void *) token);
    free((void *) body);
    free((void *) path);
    free((void *) resp_body.memory);
    return ret;
}

int pili_stream_attribute(const char *access_key, const char *secret_key, const char *hub_name,
                          const char *stream_key,
                          struct pili_stream_attribute *attribute, char *error) {
    int ret = 0;
    size_t encoded_stream_key_len = urlsafe_b64_encode(stream_key, strlen(stream_key), NULL, 0);
    char *encoded_stream_key = (char *) malloc(sizeof(char) * (encoded_stream_key_len + 1));
    memset(encoded_stream_key, 0, encoded_stream_key_len + 1);
    urlsafe_b64_encode(stream_key, strlen(stream_key), encoded_stream_key, encoded_stream_key_len);

    char *path_fmt = "/v2/hubs/%s/streams/%s";
    int path_len = snprintf(NULL, 0, path_fmt, hub_name, encoded_stream_key) + 1;
    char *path = (char *) malloc(sizeof(char) * path_len);
    sprintf(path, path_fmt, hub_name, encoded_stream_key);

    const char *token = pili_sign_request(access_key, secret_key, PILI_API_HOST, "GET", path, PILI_MIME_URLENCODED, 0,
                                          0);
    struct curl_slist *headers;

    char *auth_fmt = "Authorization: %s";
    int auth_len = snprintf(NULL, 0, auth_fmt, token) + 1;
    char *auth_header = (char *) malloc(auth_len);
    sprintf(auth_header, auth_fmt, token);

    headers = curl_slist_append(NULL, auth_header);
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

    size_t url_len = strlen(PILI_API_ADDRESS) + strlen(path) + 1;
    char *url = (char *) malloc(url_len);
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
                        sprintf(error, "%d %s", resp_code, err_obj->valuestring);
                    }
                }
                ret = -1;
            } else {
                if (attribute) {
                    cJSON *created_at_obj = cJSON_GetObjectItem(resp_root, "createdAt");
                    if (created_at_obj) {
                        attribute->created_at = (long) created_at_obj->valuedouble;
                    }
                    cJSON *updated_at_obj = cJSON_GetObjectItem(resp_root, "updatedAt");
                    if (updated_at_obj) {
                        attribute->updated_at = (long) updated_at_obj->valuedouble;
                    }
                    cJSON *expire_at_obj = cJSON_GetObjectItem(resp_root, "expireAt");
                    if (expire_at_obj) {
                        attribute->expire_at = (long) expire_at_obj->valuedouble;
                    }
                    cJSON *disabled_till_obj = cJSON_GetObjectItem(resp_root, "disabledTill");
                    if (disabled_till_obj) {
                        attribute->disabled_till = (long) disabled_till_obj->valuedouble;
                    }
                }
            }
            cJSON_Delete(resp_root);
        } else {
            ret = -1;
        }
    }

    //clean
    curl_slist_free_all(headers);
    free((void *) encoded_stream_key);
    free((void *) auth_header);
    free((void *) token);
    free((void *) path);
    free((void *) resp_body.memory);
    return ret;
}

int pili_stream_status(const char *access_key, const char *secret_key, const char *hub_name,
                       const char *stream_key,
                       struct pili_stream_status *status, char *error) {
    int ret = 0;
    size_t encoded_stream_key_len = urlsafe_b64_encode(stream_key, strlen(stream_key), NULL, 0);
    char *encoded_stream_key = (char *) malloc(sizeof(char) * (encoded_stream_key_len + 1));
    memset(encoded_stream_key, 0, encoded_stream_key_len + 1);
    urlsafe_b64_encode(stream_key, strlen(stream_key), encoded_stream_key, encoded_stream_key_len);

    char *path_fmt = "/v2/hubs/%s/streams/%s/live";
    int path_len = snprintf(NULL, 0, path_fmt, hub_name, encoded_stream_key) + 1;
    char *path = (char *) malloc(sizeof(char) * path_len);
    sprintf(path, path_fmt, hub_name, encoded_stream_key);

    const char *token = pili_sign_request(access_key, secret_key, PILI_API_HOST, "GET", path, PILI_MIME_URLENCODED, 0,
                                          0);
    struct curl_slist *headers;

    char *auth_fmt = "Authorization: %s";
    int auth_len = snprintf(NULL, 0, auth_fmt, token) + 1;
    char *auth_header = (char *) malloc(sizeof(char) * auth_len);
    sprintf(auth_header, auth_fmt, token);

    headers = curl_slist_append(NULL, auth_header);
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

    size_t url_len = strlen(PILI_API_ADDRESS) + strlen(path) + 1;
    char *url = (char *) malloc(sizeof(char) * url_len);
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
                        sprintf(error, "%d %s", resp_code, err_obj->valuestring);
                    }
                }
                ret = -1;
            } else {
                if (status) {
                    cJSON *start_at_obj = cJSON_GetObjectItem(resp_root, "startAt");
                    if (start_at_obj) {
                        status->start_at = (long) start_at_obj->valuedouble;
                    }

                    cJSON *client_ip_obj = cJSON_GetObjectItem(resp_root, "clientIP");
                    if (client_ip_obj) {
                        memset(status->client_ip, 0, sizeof(status->client_ip));
                        strcpy(status->client_ip, client_ip_obj->valuestring);
                    }

                    cJSON *bps_obj = cJSON_GetObjectItem(resp_root, "bps");
                    if (bps_obj) {
                        status->bps = (long) bps_obj->valuedouble;
                    }

                    cJSON *fps_root_obj = cJSON_GetObjectItem(resp_root, "fps");
                    if (fps_root_obj) {
                        //audio fps
                        cJSON *audio_fps_obj = cJSON_GetObjectItem(fps_root_obj, "audio");
                        status->audio_fps = (long) audio_fps_obj->valuedouble;

                        //video fps
                        cJSON *video_fps_obj = cJSON_GetObjectItem(fps_root_obj, "video");
                        status->video_fps = (long) video_fps_obj->valuedouble;

                        //data fps
                        cJSON *data_fps_obj = cJSON_GetObjectItem(fps_root_obj, "data");
                        status->data_fps = (long) data_fps_obj->valuedouble;
                    }
                }
            }
            cJSON_Delete(resp_root);
        } else {
            ret = -1;
        }
    }

    //clean
    curl_slist_free_all(headers);
    free((void *) encoded_stream_key);
    free((void *) auth_header);
    free((void *) token);
    free((void *) path);
    free((void *) resp_body.memory);
    return ret;
}

int pili_stream_enable(const char *access_key, const char *secret_key, const char *hub_name,
                       const char *stream_key, char *error) {
    return pili_stream_disable_till(access_key, secret_key, hub_name, stream_key, 0, error);
}

int pili_stream_disable_till(const char *access_key, const char *secret_key, const char *hub_name,
                             const char *stream_key, const long disable_till_time, char *error) {
    int ret = 0;

    size_t encoded_stream_key_len = urlsafe_b64_encode(stream_key, strlen(stream_key), NULL, 0);
    char *encoded_stream_key = (char *) malloc(sizeof(char) * (encoded_stream_key_len + 1));
    memset(encoded_stream_key, 0, encoded_stream_key_len + 1);
    urlsafe_b64_encode(stream_key, strlen(stream_key), encoded_stream_key, encoded_stream_key_len);

    char *path_fmt = "/v2/hubs/%s/streams/%s/disabled";
    int path_len = snprintf(NULL, 0, path_fmt, hub_name, encoded_stream_key) + 1;
    char *path = (char *) malloc(sizeof(char) * path_len);
    sprintf(path, path_fmt, hub_name, encoded_stream_key);

    //make body
    cJSON *req_root = cJSON_CreateObject();
    cJSON_AddItemToObject(req_root, "disabledTill", cJSON_CreateNumber(disable_till_time));
    const char *body = cJSON_PrintUnformatted(req_root);

    const char *token = pili_sign_request(access_key, secret_key, PILI_API_HOST, "POST", path, PILI_MIME_JSON, body, 0);
    struct curl_slist *headers;

    char *auth_fmt = "Authorization: %s";
    int auth_len = snprintf(NULL, 0, auth_fmt, token) + 1;
    char *auth_header = (char *) malloc(sizeof(char) * auth_len);
    sprintf(auth_header, auth_fmt, token);

    headers = curl_slist_append(NULL, auth_header);
    headers = curl_slist_append(headers, "Content-Type: application/json");

    size_t url_len = strlen(PILI_API_ADDRESS) + strlen(path) + 1;
    char *url = (char *) malloc(sizeof(char) * url_len);
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
                        sprintf(error, "%d %s", resp_code, err_obj->valuestring);
                    }
                }
                cJSON_Delete(resp_root);
            }
            ret = -1;
        }
    }

    //clean
    cJSON_Delete((void *) req_root);
    curl_slist_free_all(headers);
    free((void *) auth_header);
    free((void *) token);
    free((void *) body);
    free((void *) path);
    free((void *) resp_body.memory);
    return ret;
}

const char *pili_stream_saveas_whole(const char *access_key, const char *secret_key, const char *hub_name,
                                     const char *stream_key, const char *dst_file_name, char *error) {
    return pili_stream_saveas_period(access_key, secret_key, hub_name, stream_key, dst_file_name, 0, 0, error);
}

const char *pili_stream_saveas_period(const char *access_key, const char *secret_key, const char *hub_name,
                                      const char *stream_key, const char *dst_file_name,
                                      const long start_time, const long end_time, char *error) {
    char *saveas_file_name = 0;

    size_t encoded_stream_key_len = urlsafe_b64_encode(stream_key, strlen(stream_key), NULL, 0);
    char *encoded_stream_key = (char *) malloc(sizeof(char) * (encoded_stream_key_len + 1));
    memset(encoded_stream_key, 0, encoded_stream_key_len + 1);
    urlsafe_b64_encode(stream_key, strlen(stream_key), encoded_stream_key, encoded_stream_key_len);

    char *path_fmt = "/v2/hubs/%s/streams/%s/saveas";
    int path_len = snprintf(NULL, 0, path_fmt, hub_name, encoded_stream_key) + 1;
    char *path = (char *) malloc(sizeof(char) * path_len);
    sprintf(path, path_fmt, hub_name, encoded_stream_key);

    //make body
    cJSON *req_root = cJSON_CreateObject();
    if (dst_file_name) {
        cJSON_AddItemToObject(req_root, "fname", cJSON_CreateString(dst_file_name));
    }
    cJSON_AddItemToObject(req_root, "start", cJSON_CreateNumber(start_time));
    cJSON_AddItemToObject(req_root, "end", cJSON_CreateNumber(end_time));

    const char *body = cJSON_PrintUnformatted(req_root);

    const char *token = pili_sign_request(access_key, secret_key, PILI_API_HOST, "POST", path, PILI_MIME_JSON, body, 0);
    struct curl_slist *headers;

    char *auth_fmt = "Authorization: %s";
    int auth_len = snprintf(NULL, 0, auth_fmt, token) + 1;
    char *auth_header = (char *) malloc(sizeof(char) * auth_len);
    sprintf(auth_header, auth_fmt, token);

    headers = curl_slist_append(NULL, auth_header);
    headers = curl_slist_append(headers, "Content-Type: application/json");

    size_t url_len = strlen(PILI_API_ADDRESS) + strlen(path) + 1;
    char *url = (char *) malloc(sizeof(char) * url_len);
    sprintf(url, "%s%s", PILI_API_ADDRESS, path);

    int resp_code;
    struct pili_memory_buffer resp_body;
    resp_body.memory = malloc(1);
    resp_body.size = 0;

    int result = pili_post_request(url, headers, body, &resp_code, &resp_body, error);
    if (result == 0) {
        cJSON *resp_root = cJSON_Parse(resp_body.memory);
        if (resp_root) {
            if (resp_code != 200) {
                if (error) {
                    cJSON *err_obj = cJSON_GetObjectItem(resp_root, "error");
                    if (err_obj) {
                        sprintf(error, "%d %s", resp_code, err_obj->valuestring);
                    }
                }
            } else {
                cJSON *fname_obj = cJSON_GetObjectItem(resp_root, "fname");
                if (fname_obj) {
                    char *fname = fname_obj->valuestring;
                    saveas_file_name = (char *) malloc(sizeof(char) * (strlen(fname) + 1));
                    strcpy(saveas_file_name, fname);
                }
            }
            cJSON_Delete(resp_root);
        }
    }

    //clean
    cJSON_Delete(req_root);
    curl_slist_free_all(headers);
    free((void *) auth_header);
    free((void *) token);
    free((void *) body);
    free((void *) path);
    free((void *) resp_body.memory);
    return saveas_file_name;
}

int pili_stream_history(const char *access_key, const char *secret_key, const char *hub_name,
                        const char *stream_key, const long start_time, const long end_time,
                        struct pili_stream_history_ret *history_ret, char *error) {
    int ret = 0;
    size_t encoded_stream_key_len = urlsafe_b64_encode(stream_key, strlen(stream_key), NULL, 0);
    char *encoded_stream_key = (char *) malloc(sizeof(char) * (encoded_stream_key_len + 1));
    memset(encoded_stream_key, 0, encoded_stream_key_len + 1);
    urlsafe_b64_encode(stream_key, strlen(stream_key), encoded_stream_key, encoded_stream_key_len);

    char *path_fmt = "/v2/hubs/%s/streams/%s/historyactivity";
    int path_len = snprintf(NULL, 0, path_fmt, hub_name, encoded_stream_key) + 1;
    char *path = (char *) malloc(sizeof(char) * path_len);
    sprintf(path, path_fmt, hub_name, encoded_stream_key);

    //make query
    char *query_fmt = "start=%ld&end=%ld";
    int query_len = snprintf(NULL, 0, query_fmt, start_time, end_time) + 1;
    char *query = (char *) malloc(sizeof(char) * query_len);
    sprintf(query, query_fmt, start_time, end_time);

    const char *token = pili_sign_request(access_key, secret_key, PILI_API_HOST, "GET", path, PILI_MIME_URLENCODED, 0,
                                          query);
    struct curl_slist *headers;

    char *auth_fmt = "Authorization: %s";
    int auth_len = snprintf(NULL, 0, auth_fmt, token) + 1;
    char *auth_header = (char *) malloc(sizeof(char) * auth_len);
    sprintf(auth_header, auth_fmt, token);

    headers = curl_slist_append(NULL, auth_header);
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

    size_t url_len = strlen(PILI_API_ADDRESS) + strlen(path) + strlen(query) + 2;
    char *url = (char *) malloc(sizeof(char) * url_len);
    memset(url, 0, url_len);
    sprintf(url, "%s%s?%s", PILI_API_ADDRESS, path, query);

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
                        sprintf(error, "%d %s", resp_code, err_obj->valuestring);
                    }
                }
                ret = -1;
            } else {
                //parse history
                if (history_ret) {
                    cJSON *items_obj = cJSON_GetObjectItem(resp_root, "items");
                    int items_cnt = cJSON_GetArraySize(items_obj);
                    history_ret->items_count = items_cnt;
                    if (items_cnt > 0) {
                        history_ret->head = (struct pili_stream_history_item *) malloc(
                                sizeof(struct pili_stream_history_item));

                        struct pili_stream_history_item *item_next = history_ret->head;
                        struct pili_stream_history_item *item_pre = 0;

                        int i;
                        for (i = 0; i < items_cnt; i++) {
                            cJSON *item = cJSON_GetArrayItem(items_obj, i);
                            cJSON *start_obj = cJSON_GetObjectItem(item, "start");
                            long start = (long) start_obj->valuedouble;
                            cJSON *end_obj = cJSON_GetObjectItem(item, "end");
                            long end = (long) end_obj->valuedouble;

                            if (!item_next) {
                                item_next = (struct pili_stream_history_item *) malloc(
                                        sizeof(struct pili_stream_history_item));
                            }

                            item_next->start = start;
                            item_next->end = end;
                            item_next->next = 0;

                            if (item_pre) {
                                item_pre->next = item_next;
                            }

                            item_pre = item_next;
                            item_next = 0;
                        }

                        cJSON_Delete(resp_root);
                    } else {
                        history_ret->head = 0;
                    }
                }
            }

        } else {
            ret = -1;
        }
    }
    //clean
    curl_slist_free_all(headers);
    free((void *) auth_header);
    free((void *) token);
    free((void *) query);
    free((void *) path);
    free((void *) resp_body.memory);
    return ret;
}

int pili_stream_list(const char *access_key, const char *secret_key, const char *hub_name, const char *prefix,
                     const int live_only, const int limit, const char *marker,
                     struct pili_stream_list_ret *list_ret, char *error) {
    int ret = 0;

    char *path_fmt = "/v2/hubs/%s/streams";
    int path_len = snprintf(NULL, 0, path_fmt, hub_name) + 1;
    char *path = (char *) malloc(sizeof(char) * path_len);
    sprintf(path, path_fmt, hub_name);

    char *query_fmt = "liveonly=%s&prefix=%s&limit=%d&marker=%s";
    int query_len = snprintf(NULL, 0, query_fmt, (live_only == 1) ? "true" : "false", prefix ? prefix : "", limit,
                             marker ? marker : "") + 1;
    char *query = (char *) malloc(sizeof(char) * query_len);
    sprintf(query, query_fmt, (live_only == 1) ? "true" : "false", prefix ? prefix : "", limit, marker ? marker : "");

    const char *token = pili_sign_request(access_key, secret_key, PILI_API_HOST, "GET", path, PILI_MIME_URLENCODED, 0,
                                          query);
    struct curl_slist *headers;

    char *auth_fmt = "Authorization: %s";
    int auth_len = snprintf(NULL, 0, auth_fmt, token) + 1;
    char *auth_header = (char *) malloc(sizeof(char) * auth_len);
    sprintf(auth_header, auth_fmt, token);

    headers = curl_slist_append(NULL, auth_header);
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

    size_t url_len = strlen(PILI_API_ADDRESS) + strlen(path) + strlen(query) + 2;
    char *url = (char *) malloc(sizeof(char) * url_len);
    sprintf(url, "%s%s?%s", PILI_API_ADDRESS, path, query);

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
                        sprintf(error, "%d %s", resp_code, err_obj->valuestring);
                    }
                }
                ret = -1;
            } else {
                //parse history
                if (list_ret) {
                    cJSON *marker_obj = cJSON_GetObjectItem(resp_root, "marker");
                    char *next_marker = marker_obj->valuestring;
                    size_t next_marker_len = strlen(next_marker) + 1;
                    list_ret->marker = (char *) malloc(sizeof(char) * next_marker_len);
                    strcpy(list_ret->marker, next_marker);

                    cJSON *items_obj = cJSON_GetObjectItem(resp_root, "items");
                    int items_cnt = cJSON_GetArraySize(items_obj);
                    list_ret->items_count = items_cnt;
                    if (items_cnt > 0) {
                        list_ret->head = (struct pili_stream_list_item *) malloc(sizeof(struct pili_stream_list_item));

                        struct pili_stream_list_item *item_next = list_ret->head;
                        struct pili_stream_list_item *item_pre = 0;

                        int i;

                        for (i = 0; i < items_cnt; i++) {
                            cJSON *item = cJSON_GetArrayItem(items_obj, i);
                            cJSON *key_obj = cJSON_GetObjectItem(item, "key");

                            if (!item_next) {
                                item_next = (struct pili_stream_list_item *) malloc(
                                        sizeof(struct pili_stream_list_item));
                            }

                            char *key = key_obj->valuestring;
                            size_t key_len = strlen(key) + 1;
                            item_next->key = (char *) malloc(sizeof(char) * key_len);
                            strcpy(item_next->key, key);
                            item_next->next = 0;

                            if (item_pre) {
                                item_pre->next = item_next;
                            }

                            item_pre = item_next;
                            item_next = 0;
                        }
                    } else {
                        list_ret->head = 0;
                    }
                    cJSON_Delete(resp_root);
                }
            }

        } else {
            ret = -1;
        }
    }
    //clean
    curl_slist_free_all(headers);
    free((void *) auth_header);
    free((void *) token);
    free((void *) query);
    free((void *) path);
    free((void *) resp_body.memory);
    return ret;
}

unsigned long pili_list_hub_bucket(const char *access_key, const char *secret_key, const char *hub_name,
                                   const char *stream_key, const char *hub_bucket, char *marker, char *error) {
    unsigned long total_storage = 0;

    int limit = 1000;
    char *rsf_api_address = "http://rsf-z1.qiniu.com";

    //prefix
    char *prefix_fmt = "fragments/z1.%s.%s/";
    int prefix_fmt_len = snprintf(NULL, 0, prefix_fmt, hub_name, stream_key) + 1;
    char *prefix = (char *) malloc(sizeof(char) * prefix_fmt_len);
    sprintf(prefix, prefix_fmt, hub_name, stream_key);

    char *path = "/list";
    char *query_fmt = "bucket=%s&prefix=%s&limit=%d&marker=%s";
    int query_len = snprintf(NULL, 0, query_fmt, hub_bucket, prefix, limit, marker) + 1;
    char *query = (char *) malloc(sizeof(char) * query_len);
    sprintf(query, query_fmt, hub_bucket, prefix, limit, marker);

    const char *token = kodo_sign_request(access_key, secret_key, path, PILI_MIME_URLENCODED, 0,
                                          query);
    struct curl_slist *headers;

    char *auth_fmt = "Authorization: %s";
    int auth_len = snprintf(NULL, 0, auth_fmt, token) + 1;
    char *auth_header = (char *) malloc(sizeof(char) * auth_len);
    sprintf(auth_header, auth_fmt, token);

    headers = curl_slist_append(NULL, auth_header);
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

    size_t url_len = strlen(rsf_api_address) + strlen(path) + strlen(query) + 2;
    char *url = (char *) malloc(sizeof(char) * url_len);
    sprintf(url, "%s%s?%s", rsf_api_address, path, query);

    int resp_code;
    struct pili_memory_buffer resp_body;
    resp_body.memory = malloc(1);
    resp_body.size = 0;

    int result = pili_post_request(url, headers, 0, &resp_code, &resp_body, error);

    if (result == 0) {
        cJSON *resp_root = cJSON_Parse(resp_body.memory);
        if (resp_root) {
            if (resp_code != 200) {
                if (error) {
                    cJSON *err_obj = cJSON_GetObjectItem(resp_root, "error");
                    if (err_obj) {
                        sprintf(error, "%d %s", resp_code, err_obj->valuestring);
                    }
                }
            } else {
                cJSON *marker_obj = cJSON_GetObjectItem(resp_root, "marker");
                if (marker_obj) {
                    char *next_marker = marker_obj->valuestring;
                    strcpy(marker, next_marker);
                    marker[strlen(next_marker) + 1] = 0;
                } else {
                    //list ends
                    strcpy(marker, "");
                }

                cJSON *items_obj = cJSON_GetObjectItem(resp_root, "items");
                int items_cnt = cJSON_GetArraySize(items_obj);
                if (items_cnt > 0) {
                    int i;
                    for (i = 0; i < items_cnt; i++) {
                        cJSON *item = cJSON_GetArrayItem(items_obj, i);
                        cJSON *fsize_obj = cJSON_GetObjectItem(item, "fsize");
                        unsigned long fsize = (unsigned long) fsize_obj->valuedouble;
                        total_storage += fsize;
                    }
                }
            }
            cJSON_Delete(resp_root);
        }
    }
    //clean
    curl_slist_free_all(headers);
    free((void *) auth_header);
    free((void *) token);
    free((void *) prefix);
    free((void *) query);
    free((void *) resp_body.memory);
    return total_storage;

}

unsigned long pili_get_stream_storage(const char *access_key, const char *secret_key, const char *hub_name,
                                      const char *stream_key, const char *hub_bucket, char *error) {
    unsigned long total_stream_storage = 0;
    char marker[200];
    memset(marker, 0, 200);
    unsigned long total_storage = pili_list_hub_bucket(access_key, secret_key, hub_bucket, stream_key, hub_bucket,
                                                       marker, error);
    if (total_storage > 0) {
        total_stream_storage += total_storage;
        //check marker
        while (strcmp(marker, "") != 0) {
            strcpy(error, "");
            total_storage = pili_list_hub_bucket(access_key, secret_key, hub_bucket, stream_key, hub_bucket, marker,
                                                 error);
            if (total_storage > 0) {
                total_stream_storage += total_storage;
            } else {
                //encounter error
                break;
            }
        }
    }
    return total_stream_storage;
}