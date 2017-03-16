//
// Created by jemy on 16/03/2017.
//

#include "url_factory.h"
#include "auth.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

const char *pili_rtmp_publish_url_with_noauth(const char *rtmp_pubilsh_domain, const char *hub_name,
                                              const char *stream_key) {
    size_t dst_len = strlen(rtmp_pubilsh_domain) + strlen(hub_name) + strlen(stream_key);
    dst_len += 10;

    char *rtmp_publish_url = (char *) malloc(sizeof(char) * dst_len);
    memset(rtmp_publish_url, 0, dst_len);
    sprintf(rtmp_publish_url, "rtmp://%s/%s/%s", rtmp_pubilsh_domain, hub_name, stream_key);
    return rtmp_publish_url;
}

const char *pili_rtmp_pubilsh_url_with_expiry(const char *rtmp_pubilsh_domain, const char *hub_name,
                                              const char *stream_key, long deadline, const char *access_key,
                                              const char *secret_key) {
    int deadline_len = snprintf(NULL, 0, "%ld", deadline);
    size_t to_sign_len = strlen(hub_name) + strlen(stream_key) + deadline_len;
    to_sign_len += 5;

    char *path = (char *) malloc(sizeof(char) * to_sign_len);
    sprintf(path, "/%s/%s?e=%ld", hub_name, stream_key, deadline);
    const char *token = pili_hmac_sha1(access_key, secret_key, path);

    size_t dst_len = strlen(rtmp_pubilsh_domain) + strlen(path) + strlen(token);
    dst_len += 15;
    char *rtmp_publish_url = (char *) malloc(sizeof(char) * dst_len);
    sprintf(rtmp_publish_url, "rtmp://%s%s&token=%s", rtmp_pubilsh_domain, path, token);
    free((void *) path);
    free((void *) token);
    return rtmp_publish_url;
}

const char *pili_rtmp_play_url(const char *rtmp_play_domain, const char *hub_name, const char *stream_key) {
    size_t dst_len = strlen(rtmp_play_domain) + strlen(hub_name) + strlen(stream_key);
    dst_len += 10;

    char *rtmp_play_url = (char *) malloc(sizeof(char) * dst_len);
    memset(rtmp_play_url, 0, dst_len);
    sprintf(rtmp_play_url, "rtmp://%s/%s/%s", rtmp_play_domain, hub_name, stream_key);
    return rtmp_play_url;
}

const char *pili_hdl_play_url(const char *hdl_play_domain, const char *hub_name, const char *stream_key) {
    size_t dst_len = strlen(hdl_play_domain) + strlen(hub_name) + strlen(stream_key);
    dst_len += 14;

    char *hdl_play_url = (char *) malloc(sizeof(char) * dst_len);
    memset(hdl_play_url, 0, dst_len);
    sprintf(hdl_play_url, "http://%s/%s/%s.flv", hdl_play_domain, hub_name, stream_key);
    return hdl_play_url;
}

const char *pili_hls_play_url(const char *hls_play_domain, const char *hub_name, const char *stream_key) {
    size_t dst_len = strlen(hls_play_domain) + strlen(hub_name) + strlen(stream_key);
    dst_len += 15;

    char *hls_play_url = (char *) malloc(sizeof(char) * dst_len);
    memset(hls_play_url, 0, dst_len);
    sprintf(hls_play_url, "http://%s/%s/%s.m3u8", hls_play_domain, hub_name, stream_key);
    return hls_play_url;
}

const char *pili_snapshot_url(const char *snapshot_domain, const char *hub_name, const char *stream_key) {
    size_t dst_len = strlen(snapshot_domain) + strlen(hub_name) + strlen(stream_key);
    dst_len += 14;

    char *snapshot_url = (char *) malloc(sizeof(char) * dst_len);
    memset(snapshot_url, 0, dst_len);
    sprintf(snapshot_url, "http://%s/%s/%s.jpg", snapshot_domain, hub_name, stream_key);
    return snapshot_url;
}