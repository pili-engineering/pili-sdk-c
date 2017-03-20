INCLUDE_PATH=-Ibase64 -IcJSON -Ipili -I/usr/local/Cellar/openssl/1.0.2k/include
LINK_PATH=-L/usr/local/Cellar/openssl/1.0.2k/lib

SOURCE_FILES=\
	base64/b64.c\
	base64/urlsafe_b64.c\
	cJSON/cJSON.c\
	pili/auth.c\
	pili/http.c\
	pili/stream.c\
	pili/url_factory.c\
	main.c


all: $(SOURCE_FILES)
	gcc -g $^ -o pili_sdk_c $(INCLUDE_PATH) $(LINK_PATH) -lcurl -lcrypto -lssl -lm
