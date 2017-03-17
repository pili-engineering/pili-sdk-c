#include <stdio.h>
#include <stdlib.h>
#include "pili/url_factory.h"
#include <time.h>
#include <stream.h>

int main(int argc, char **argv) {

    const char *access_key = "";
    const char *secret_key = "";

    const char *rtmp_publish_domain = "pili-publish.ps.qiniucdn.com";
    const char *rtmp_play_domain = "pili-live-rtmp.ps.qiniucdn.com";
    const char *hdl_play_domain = "pili-live-hdl.ps.qiniucdn.com";
    const char *hls_play_domain = "pili-live-hls.ps.qiniucdn.com";
    const char *snapshot_domain = "pili-static.ps.qiniucdn.com";

    const char *hub_name = "NIU7PS";
    const char *stream_key = "zhebao-tui";

    ///url factory example code
    const char *rtmp_publish_url_noauth = pili_rtmp_publish_url_with_noauth(rtmp_publish_domain, hub_name, stream_key);
    printf("rtmp publish(noauth):\t%s\n", rtmp_publish_url_noauth);

    long deadline = (long) time(NULL) + 3600;
    const char *rtmp_publish_url_expiry = pili_rtmp_pubilsh_url_with_expiry(rtmp_publish_domain, hub_name, stream_key,
                                                                            deadline,
                                                                            access_key, secret_key);
    printf("rtmp publish(expiry):\t%s\n", rtmp_publish_url_expiry);

    const char *rtmp_play_url = pili_rtmp_play_url(rtmp_play_domain, hub_name, stream_key);
    printf("rtmp play:\t%s\n", rtmp_play_url);

    const char *hdl_play_url = pili_hdl_play_url(hdl_play_domain, hub_name, stream_key);
    printf("hdl play:\t%s\n", hdl_play_url);

    const char *hls_play_url = pili_hls_play_url(hls_play_domain, hub_name, stream_key);
    printf("hls play:\t%s\n", hls_play_url);

    const char *snapshot_url = pili_snapshot_url(snapshot_domain, hub_name, stream_key);
    printf("snapshot:\t%s\n", snapshot_url);

    free((void *) rtmp_play_url);
    free((void *) hdl_play_url);
    free((void *) hls_play_url);
    free((void *) snapshot_url);


    //shared
    char error[1024];
    int ret;

    /*
    ///create stream
    long ts = (long) time(NULL);
    int ts_len = snprintf(NULL, 0, "%ld", ts);
    ts_len += 5;
    char *new_stream_key = (char *) malloc(sizeof(char) * ts_len);
    memset(new_stream_key, 0, ts_len);
    sprintf(new_stream_key, "csdk%ld", ts);

    printf("create new stream: %s\n", new_stream_key);
    ret = pili_create_stream(access_key, secret_key, hub_name, new_stream_key, error);
    printf("create stream ret: %d\terror: %s\n", ret, error);
    free((void *) new_stream_key);


    //get stream attribute
    struct pili_stream_attribute attribute;
    ret = pili_stream_attribute(access_key, secret_key, hub_name, stream_key, &attribute, error);
    printf("stream attribute ret: %d\terror: %s\n", ret, error);
    printf("stream createdAt: %ld\n", attribute.created_at);
    printf("stream updatedAt: %ld\n", attribute.updated_at);
    printf("stream expireAt: %ld\n", attribute.expire_at);
    printf("stream disabledTill: %ld\n", attribute.disabled_till);


    //get stream status
    stream_key="csdk1489721888";
    struct pili_stream_status status;
    ret = pili_stream_status(access_key, secret_key, hub_name, stream_key, &status, error);
    printf("stream status ret: %d\terror: %s\n", ret, error);
    printf("stream startAt: %ld\n", status.start_at);
    printf("stream clientIP: %s\n", status.client_ip);
    printf("stream bps:%ld\n", status.bps);
    printf("stream audioFPS: %ld\n", status.audio_fps);
    printf("stream videoFPS: %ld\n", status.video_fps);
    printf("stream dataFPS: %ld\n", status.data_fps);



    //stream_key = "csdk1489721888";

    //disable the stream forever
    ret = pili_stream_disable_till(access_key, secret_key, hub_name, stream_key, -1, error);
    printf("stream disable till ret: %d\terror: %s\n", ret, error);

    //disable the stream for an hour
    long disable_till = (long) time(NULL) + 3600;
    ret = pili_stream_disable_till(access_key, secret_key, hub_name, stream_key, disable_till, error);
    printf("stream disable till %ld ret: %d\terror: %s\n", disable_till, ret, error);

    //enable the stream
    ret = pili_stream_enable(access_key, secret_key, hub_name, stream_key, error);
    printf("stream enabled ret: %d\terror: %s\n", ret, error);



    char *saveas_file_name=pili_stream_saveas_whole(access_key,secret_key,hub_name,stream_key,0,error);
    printf("stream saveas ret: %s\terror: %s\n",saveas_file_name,error);
    free((void*)saveas_file_name);

*/

    //get stream history
    struct pili_stream_history_item *head = (struct pili_stream_history_item *) malloc(
            sizeof(struct pili_stream_history_item));
    ret = pili_stream_history(access_key, secret_key, hub_name, stream_key, 0, 0, head, error);
    printf("stream history ret: %d\terror: %s\n", ret, error);
    if (ret == 0) {
        struct pili_stream_history_item *iter = head;
        while (iter) {
            printf("start: %ld\tend: %ld\n", head->start, head->end);
            iter = iter->next;
        }
        iter = head;
        while (iter) {
            struct pili_stream_history_item *fp = iter;
            free((void *) fp);
            iter = iter->next;
        }
    }

    //list stream
    struct pili_stream_list_ret list_ret;
    char *prefix = "";
    char *marker = "";
    int limit = 10;
    int live_only = 0;
    ret = pili_stream_list(access_key, secret_key, hub_name, prefix, live_only, limit, marker, &list_ret, error);
    printf("stream list ret: %d\terror: %s\n", ret, error);
    if (ret == 0) {
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