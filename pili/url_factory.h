//
// Created by jemy on 16/03/2017.
//

#ifndef PILI_SDK_C_URL_FACTORY_H
#define PILI_SDK_C_URL_FACTORY_H

const char *pili_rtmp_publish_url_with_noauth(const char *rtmp_pubilsh_domain, const char *hub_name,
                                              const char *stream_key);

const char *pili_rtmp_pubilsh_url_with_expiry(const char *rtmp_pubilsh_domain, const char *hub_name,
                                              const char *stream_key, long deadline, const char *access_key,
                                              const char *secret_key);

const char *pili_rtmp_play_url(const char *rtmp_play_domain, const char *hub_name, const char *stream_key);

const char *pili_hdl_play_url(const char *rtmp_play_domain, const char *hub_name, const char *stream_key);

const char *pili_hls_play_url(const char *rtmp_play_domain, const char *hub_name, const char *stream_key);

const char *pili_snapshot_url(const char *snapshot_domain, const char *hub_name, const char *stream_key);

#endif //PILI_SDK_C_URL_FACTORY_H
