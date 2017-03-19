//
// Created by jemy on 15/03/2017.
//

#include "http.h"
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

size_t pili_response_callback(char *contents, size_t size, size_t nmemb, void *resp_body) {
    size_t real_size = size * nmemb;
    struct pili_memory_buffer *mem = (struct pili_memory_buffer *) resp_body;
    mem->memory = realloc(mem->memory, mem->size + real_size + 1);
    if (mem->memory == NULL) {
        /* out of memory! */
        printf("pili response callback out of memory\n");
        return 0;
    }
    memcpy(&(mem->memory[mem->size]), contents, real_size);
    mem->size += real_size;
    mem->memory[mem->size] = 0;
    return real_size;
}

int pili_post_request(const char *url, const struct curl_slist *headers, const char *body,
                      int *resp_code, struct pili_memory_buffer *resp_body, char *error) {
    int ret = 0;
    CURL *client;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    client = curl_easy_init();
    if (client) {
        curl_easy_setopt(client, CURLOPT_URL, url);
        curl_easy_setopt(client, CURLOPT_POST, 1);
        curl_easy_setopt(client, CURLOPT_WRITEFUNCTION, pili_response_callback);
        curl_easy_setopt(client, CURLOPT_WRITEDATA, (void *) resp_body);

        if (headers) {
            curl_easy_setopt(client, CURLOPT_HTTPHEADER, headers);
        }
        if (body) {
            curl_easy_setopt(client, CURLOPT_POSTFIELDS, body);
            curl_easy_setopt(client, CURLOPT_POSTFIELDSIZE, strlen(body));
        }

        curl_easy_setopt(client, CURLOPT_USERAGENT, PILI_USER_AGENT);
        //set verbose
        //curl_easy_setopt(client, CURLOPT_VERBOSE, 1);
        //print response header
        //curl_easy_setopt(client, CURLOPT_HEADER, 1);

        res = curl_easy_perform(client);

        if (res != CURLE_OK) {
            if (error) {
                sprintf(error, "curl request error, %d %s", res, curl_easy_strerror(res));
            }
            ret = -1;
        } else {
            //parse
            curl_easy_getinfo(client, CURLINFO_RESPONSE_CODE, resp_code);
        }

        curl_easy_cleanup(client);
    } else {
        ret = -1;
    }

    curl_global_cleanup();
    return ret;
}


int pili_get_request(const char *url, const struct curl_slist *headers,
                     int *resp_code, struct pili_memory_buffer *resp_body, char *error) {
    int ret = 0;
    CURL *client;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    client = curl_easy_init();
    if (client) {
        curl_easy_setopt(client, CURLOPT_URL, url);
        curl_easy_setopt(client, CURLOPT_WRITEFUNCTION, pili_response_callback);
        curl_easy_setopt(client, CURLOPT_WRITEDATA, (void *) resp_body);

        if (headers) {
            curl_easy_setopt(client, CURLOPT_HTTPHEADER, headers);
        }


        curl_easy_setopt(client, CURLOPT_USERAGENT, PILI_USER_AGENT);
        //set verbose
        //curl_easy_setopt(client, CURLOPT_VERBOSE, 1);
        //print response header
        //curl_easy_setopt(client, CURLOPT_HEADER, 1);

        res = curl_easy_perform(client);

        if (res != CURLE_OK) {
            if (error) {
                sprintf(error, "curl request error, %d %s", res, curl_easy_strerror(res));
            }
            ret = -1;
        } else {
            //parse
            curl_easy_getinfo(client, CURLINFO_RESPONSE_CODE, resp_code);
        }

        curl_easy_cleanup(client);
    } else {
        ret = -1;
    }

    curl_global_cleanup();
    return ret;
}