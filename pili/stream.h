//
// Created by jemy on 16/03/2017.
//

#ifndef PILI_SDK_C_STREAM_H
#define PILI_SDK_C_STREAM_H


struct pili_stream_attribute {
    long created_at;
    long updated_at;
    long expire_at;
    long disabled_till;
};

struct pili_stream_status {
    long start_at;
    char client_ip[50];
    long bps;
    long audio_fps;
    long video_fps;
    long data_fps;
};

int pili_create_stream(const char *access_key, const char *secret_key, const char *hub_name, const char *stream_key,
                       char *error);

int pili_stream_attribute(const char *access_key, const char *secret_key, const char *hub_name, const char *stream_key,
                          struct pili_stream_attribute *attribute, char *error);

int pili_stream_status(const char *access_key, const char *secret_key, const char *hub_name, const char *stream_key,
                       struct pili_stream_status *status, char *error);

#endif //PILI_SDK_C_STREAM_H
