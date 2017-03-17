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

int pili_create_stream(const char *access_key, const char *secret_key, const char *hub_name, const char *stream_key,
                       char *error);

int pili_stream_attribute(const char *access_key, const char *secret_key, const char *hub_name, const char *stream_key,
                          struct pili_stream_attribute *attribute, char *error);

#endif //PILI_SDK_C_STREAM_H
