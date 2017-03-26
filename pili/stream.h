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

struct pili_stream_history_item {
    long start;
    long end;
    struct pili_stream_history_item *next;
};

struct pili_stream_history_ret {
    struct pili_stream_history_item *head;
    int items_count;
};

struct pili_stream_list_item {
    char *key;
    struct pili_stream_list_item *next;
};

struct pili_stream_list_ret {
    struct pili_stream_list_item *head;
    int items_count;
    char *marker;
};

/**
 * return 0 on success, or find error in *error
 * */
int pili_create_stream(const char *access_key, const char *secret_key, const char *hub_name,
                       const char *stream_key, char *error);

/**
 * return 0 on success, or find error in *error
 * */
int pili_stream_attribute(const char *access_key, const char *secret_key, const char *hub_name,
                          const char *stream_key, struct pili_stream_attribute *attribute, char *error);

/**
 * return 0 on success, or find error in *error
 * */
int pili_stream_status(const char *access_key, const char *secret_key, const char *hub_name,
                       const char *stream_key, struct pili_stream_status *status, char *error);

int pili_stream_list(const char *access_key, const char *secret_key, const char *hub_name,
                     const char *prefix, const int live_only, const int limit, const char *marker,
                     struct pili_stream_list_ret *list_ret, char *error);

/**
 * return 0 on success, or find error in *error
 * */
int pili_stream_history(const char *access_key, const char *secret_key, const char *hub_name,
                        const char *stream_key, const long start_time, const long end_time,
                        struct pili_stream_history_ret *history_ret, char *error);

/**
 * return 0 on success, or find error in *error
 * */
int pili_stream_enable(const char *access_key, const char *secret_key, const char *hub_name,
                       const char *stream_key, char *error);

/**
 * return 0 on success, or find error in *error
 * */
int pili_stream_disable_till(const char *access_key, const char *secret_key, const char *hub_name,
                             const char *stream_key, const long disable_till_time, char *error);

/**
 * return saveas file not null on success, or find error in *error
 * */
const char *pili_stream_saveas_whole(const char *access_key, const char *secret_key, const char *hub_name,
                                     const char *stream_key, const char *dst_file_name, char *error);

/**
 * return saveas file not null on success, or find error in *error
 * */
const char *pili_stream_saveas_period(const char *access_key, const char *secret_key, const char *hub_name,
                                      const char *stream_key, const char *dst_file_name,
                                      const long start_time, const long end_time, char *error);

unsigned long pili_get_stream_storage(const char *access_key, const char *secret_key, const char *hub_name,
                                      const char *stream_key, const char *hub_bucket, char *error);


#endif //PILI_SDK_C_STREAM_H
