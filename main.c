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

    ///create stream
    long ts = (long) time(NULL);
    int ts_len = snprintf(NULL, 0, "%ld", ts);
    ts_len += 5;
    char *new_stream_key = (char *) malloc(sizeof(char) * ts_len);
    memset(new_stream_key, 0, ts_len);
    sprintf(new_stream_key, "csdk%ld", ts);

    printf("create new stream: %s\n", new_stream_key);
    char error[1024];
    int ret = pili_create_stream(access_key, secret_key, hub_name, new_stream_key, error);
    printf("%d\t%s\n", ret, error);
}