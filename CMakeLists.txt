cmake_minimum_required(VERSION 3.2)
project(pili_sdk_c)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#3rd party library
set(OPENSSL_ROOT_DIR /usr/local/Cellar/openssl/1.0.2k)

#find system library
find_library(CURL REQUIRED)

# compile & link
include_directories(pili base64 cJSON)
include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${CURL_INCLUDE_DIRS})

aux_source_directory(pili SRCS1)
aux_source_directory(base64 SRCS2)
aux_source_directory(cJSON SRCS3)
add_library(pili STATIC ${SRCS1} ${SRCS2} ${SRCS3})

#link library
link_libraries(${OPENSSL_CRYPTO_LIBRARY})
link_libraries(${OPENSSL_SSL_LIBRARY})

#exec
set(SOURCE_FILES main.c
                pili/auth.c
                pili/http.c
                pili/stream.c
                pili/url_factory.c
                base64/b64.c
                base64/urlsafe_b64.c
                cJSON/cJSON.c
)

add_executable(pili_sdk_c ${SOURCE_FILES})
target_link_libraries(pili_sdk_c curl)

