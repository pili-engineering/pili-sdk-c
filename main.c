#include <stdio.h>
#include <stdlib.h>
#include "pili/url_factory.h"
#include <time.h>
#include <stream.h>
#include <string.h>

const char *access_key = "";
const char *secret_key = "";

const char *rtmp_publish_domain = "pili-publish.ps.qiniucdn.com";
const char *rtmp_play_domain = "pili-live-rtmp.ps.qiniucdn.com";
const char *hdl_play_domain = "pili-live-hdl.ps.qiniucdn.com";
const char *hls_play_domain = "pili-live-hls.ps.qiniucdn.com";
const char *snapshot_domain = "pili-static.ps.qiniucdn.com";

const char *hub_name = "NIU7PS";
const char *stream_key = "zhebao-tui";
const int ERROR_LEN = 200;

void create_urls() {
    ///url factory example code
    const char *rtmp_publish_url_noauth = pili_rtmp_publish_url_with_noauth(rtmp_publish_domain, hub_name, stream_key);
    printf("rtmp publish(noauth):\t%s\n", rtmp_publish_url_noauth);
    free((void *) rtmp_publish_url_noauth);

    long deadline = (long) time(NULL) + 3600;
    const char *rtmp_publish_url_expiry = pili_rtmp_pubilsh_url_with_expiry(rtmp_publish_domain, hub_name, stream_key,
                                                                            deadline,
                                                                            access_key, secret_key);
    printf("rtmp publish(expiry):\t%s\n", rtmp_publish_url_expiry);
    free((void *) rtmp_publish_url_expiry);

    const char *rtmp_play_url = pili_rtmp_play_url(rtmp_play_domain, hub_name, stream_key);
    printf("rtmp play:\t%s\n", rtmp_play_url);
    free((void *) rtmp_play_url);

    const char *hdl_play_url = pili_hdl_play_url(hdl_play_domain, hub_name, stream_key);
    printf("hdl play:\t%s\n", hdl_play_url);
    free((void *) hdl_play_url);

    const char *hls_play_url = pili_hls_play_url(hls_play_domain, hub_name, stream_key);
    printf("hls play:\t%s\n", hls_play_url);
    free((void *) hls_play_url);

    const char *snapshot_url = pili_snapshot_url(snapshot_domain, hub_name, stream_key);
    printf("snapshot:\t%s\n", snapshot_url);
    free((void *) snapshot_url);
}

void create_stream() {
    char error[ERROR_LEN];
    int ret;
    memset(error, 0, ERROR_LEN);

    ///create stream
    long ts = (long) time(NULL);
    int ts_len = snprintf(NULL, 0, "csdk%ld", ts) + 1;
    char *new_stream_key = (char *) malloc(sizeof(char) * ts_len);
    sprintf(new_stream_key, "csdk%ld", ts);

    printf("create new stream: %s\n", new_stream_key);
    ret = pili_create_stream(access_key, secret_key, hub_name, new_stream_key, error);
    printf("create stream ret: %d\terror: %s\n", ret, error);
    free((void *) new_stream_key);
}

void get_stream_attribute() {
    char error[1024];
    int ret;

    memset(error, 0, ERROR_LEN);
    //get stream attribute
    struct pili_stream_attribute attribute;
    ret = pili_stream_attribute(access_key, secret_key, hub_name, stream_key, &attribute, error);
    printf("stream attribute ret: %d\terror: %s\n", ret, error);
    if (ret == 0) {
        printf("stream createdAt: %ld\n", attribute.created_at);
        printf("stream updatedAt: %ld\n", attribute.updated_at);
        printf("stream expireAt: %ld\n", attribute.expire_at);
        printf("stream disabledTill: %ld\n", attribute.disabled_till);
    }
}

void get_stream_status() {
    char error[1024];
    int ret;

    memset(error, 0, ERROR_LEN);
    //get stream status
    struct pili_stream_status status;
    ret = pili_stream_status(access_key, secret_key, hub_name, stream_key, &status, error);
    printf("stream status ret: %d\terror: %s\n", ret, error);
    if (ret == 0) {
        printf("stream startAt: %ld\n", status.start_at);
        printf("stream clientIP: %s\n", status.client_ip);
        printf("stream bps:%ld\n", status.bps);
        printf("stream audioFPS: %ld\n", status.audio_fps);
        printf("stream videoFPS: %ld\n", status.video_fps);
        printf("stream dataFPS: %ld\n", status.data_fps);
    }
}

void disable_stream_forever() {
    char error[1024];
    int ret;

    memset(error, 0, ERROR_LEN);
    //disable the stream forever
    ret = pili_stream_disable_till(access_key, secret_key, hub_name, stream_key, -1, error);
    printf("stream disable till ret: %d\terror: %s\n", ret, error);
}

void disable_stream_till() {
    char error[1024];
    int ret;

    memset(error, 0, ERROR_LEN);
    //disable the stream for an hour
    long disable_till = (long) time(NULL) + 3600;
    ret = pili_stream_disable_till(access_key, secret_key, hub_name, stream_key, disable_till, error);
    printf("stream disable till %ld ret: %d\terror: %s\n", disable_till, ret, error);
}

void enable_stream() {
    char error[1024];
    int ret;
    memset(error, 0, ERROR_LEN);

    //enable the stream
    ret = pili_stream_enable(access_key, secret_key, hub_name, stream_key, error);
    printf("stream enabled ret: %d\terror: %s\n", ret, error);
}

void saveas_stream_whole() {
    char error[1024];
    memset(error, 0, ERROR_LEN);

    const char *saveas_file_name = pili_stream_saveas_whole(access_key, secret_key, hub_name, stream_key, 0, error);
    printf("stream saveas whole ret: %s\terror: %s\n", saveas_file_name, error);
    free((void *) saveas_file_name);
}

void saveas_stream_period() {
    char error[1024];
    memset(error, 0, ERROR_LEN);

    //timestamp in seconds
    long start_time = 1489883908;
    long end_time = 1489887511;
    const char *p_saves_file_name = "saveas_example.m3u8";

    const char *saveas_file_name = pili_stream_saveas_period(access_key, secret_key, hub_name, stream_key,
                                                             p_saves_file_name, start_time, end_time, error);
    printf("stream saveas period ret: %s\terror: %s\n", saveas_file_name, error);
    free((void *) saveas_file_name);
}

void get_stream_history() {
    char error[1024];
    int ret;
    memset(error, 0, ERROR_LEN);

    //get stream history
    struct pili_stream_history_ret history_ret;
    ret = pili_stream_history(access_key, secret_key, hub_name, stream_key, 0, 0, &history_ret, error);
    printf("stream history ret: %d\terror: %s\n", ret, error);
    if (ret == 0) {
        printf("stream history items count: %d\n", history_ret.items_count);
        struct pili_stream_history_item *iter = history_ret.head;
        //skip the head
        while (iter) {
            printf("start: %ld\tend: %ld\n", iter->start, iter->end);
            iter = iter->next;
        }
        iter = history_ret.head;
        while (iter) {
            struct pili_stream_history_item *fp = iter;
            free((void *) fp);
            iter = iter->next;
        }
    }
}

void get_stream_list() {
    char error[1024];
    int ret;
    memset(error, 0, ERROR_LEN);

    //list stream
    struct pili_stream_list_ret list_ret;
    char *prefix = "";
    char *marker = "";
    int limit = 10;
    int live_only = 0;
    ret = pili_stream_list(access_key, secret_key, hub_name, prefix, live_only, limit, marker, &list_ret, error);
    printf("stream list ret: %d\terror: %s\n", ret, error);
    if (ret == 0) {
        printf("stream list items count: %d\n", list_ret.items_count);
        //if marker not empty, there are still streams to be iterated
        printf("stream list next marker: %s\n", list_ret.marker);
        struct pili_stream_list_item *iter = list_ret.head;
        while (iter) {
            printf("key: %s\n", iter->key);
            iter = iter->next;
        }
        iter = list_ret.head;
        while (iter) {
            struct pili_stream_list_item *fp = iter;
            free((void *) fp);
            iter = iter->next;
        }
        free((void *) list_ret.marker);
    }
}

int main(int argc, char **argv) {
    create_urls();
    printf("\n");
    create_stream();
    printf("\n");
    get_stream_attribute();
    printf("\n");
    get_stream_status();
    printf("\n");
    get_stream_history();
    printf("\n");
    get_stream_list();
    printf("\n");
    saveas_stream_period();
    printf("\n");
    saveas_stream_whole();
    printf("\n");
    disable_stream_forever();
    printf("\n");
    disable_stream_till();
    printf("\n");
    enable_stream();
    printf("\n");
}