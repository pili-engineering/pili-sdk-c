#include <stdio.h>
#include <stdlib.h>
#include "pili/url_factory.h"
#include "pili/auth.h"
#include <time.h>
#include <stream.h>
#include <string.h>

int main(int argc, char **argv) {
    int exit;

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

      */

    char *saveas_file_name=pili_stream_saveas_whole(access_key,secret_key,hub_name,stream_key,0,error);
    printf("stream saveas ret: %s\terror: %s\n",saveas_file_name,error);
    free((void*)saveas_file_name);

}