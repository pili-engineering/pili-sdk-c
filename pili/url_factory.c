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
    char *dst_fmt = "rtmp://%s/%s/%s";
    size_t dst_len = snprintf(NULL, 0, dst_fmt, rtmp_pubilsh_domain, hub_name, stream_key) + 1;

    char *rtmp_publish_url = (char *) malloc(sizeof(char) * dst_len);
    sprintf(rtmp_publish_url, dst_fmt, rtmp_pubilsh_domain, hub_name, stream_key);
    return rtmp_publish_url;
}

const char *pili_rtmp_pubilsh_url_with_expiry(const char *rtmp_pubilsh_domain, const char *hub_name,
                                              const char *stream_key, long deadline, const char *access_key,
                                              const char *secret_key) {
    char *to_sign_fmt = "/%s/%s?e=%ld";
    size_t to_sign_len = snprintf(NULL, 0, to_sign_fmt, hub_name, stream_key, deadline) + 1;
    char *req_uri = (char *) malloc(sizeof(char) * to_sign_len);
    sprintf(req_uri, to_sign_fmt, hub_name, stream_key, deadline);
    const char *token = pili_hmac_sha1(access_key, secret_key, req_uri);

    char *dst_fmt = "rtmp://%s%s&token=%s";
    size_t dst_len = snprintf(NULL, 0, dst_fmt, rtmp_pubilsh_domain, req_uri, token) + 1;
    char *rtmp_publish_url = (char *) malloc(sizeof(char) * dst_len);
    sprintf(rtmp_publish_url, dst_fmt, rtmp_pubilsh_domain, req_uri, token);

    free((void *) req_uri);
    free((void *) token);
    return rtmp_publish_url;
}

const char *pili_rtmp_play_url(const char *rtmp_play_domain, const char *hub_name, const char *stream_key) {
    char *dst_fmt = "rtmp://%s/%s/%s";
    size_t dst_len = snprintf(NULL, 0, dst_fmt, rtmp_play_domain, hub_name, stream_key) + 1;

    char *rtmp_play_url = (char *) malloc(sizeof(char) * dst_len);
    sprintf(rtmp_play_url, dst_fmt, rtmp_play_domain, hub_name, stream_key);
    return rtmp_play_url;
}

const char *pili_hdl_play_url(const char *hdl_play_domain, const char *hub_name, const char *stream_key) {
    char *dst_fmt = "http://%s/%s/%s.flv";
    size_t dst_len = snprintf(NULL, 0, dst_fmt, hdl_play_domain, hub_name, stream_key) + 1;

    char *hdl_play_url = (char *) malloc(sizeof(char) * dst_len);

    sprintf(hdl_play_url, dst_fmt, hdl_play_domain, hub_name, stream_key);
    return hdl_play_url;
}

const char *pili_hls_play_url(const char *hls_play_domain, const char *hub_name, const char *stream_key) {
    char *dst_fmt = "http://%s/%s/%s.m3u8";
    size_t dst_len = snprintf(NULL, 0, dst_fmt, hls_play_domain, hub_name, stream_key) + 1;

    char *hls_play_url = (char *) malloc(sizeof(char) * dst_len);
    sprintf(hls_play_url, dst_fmt, hls_play_domain, hub_name, stream_key);
    return hls_play_url;
}

const char *pili_snapshot_url(const char *snapshot_domain, const char *hub_name, const char *stream_key) {
    char *dst_fmt = "http://%s/%s/%s.jpg";
    size_t dst_len = snprintf(NULL, 0, dst_fmt, snapshot_domain, hub_name, stream_key) + 1;

    char *snapshot_url = (char *) malloc(sizeof(char) * dst_len);
    sprintf(snapshot_url, dst_fmt, snapshot_domain, hub_name, stream_key);
    return snapshot_url;
}