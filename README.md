# Pili Streaming Cloud Server-Side Library For C/C++

## 文档

本SDK对应API文档地址：[七牛直播云服务端API](https://developer.qiniu.com/pili/api/the-server-api-reference)

## 功能

- 直播流地址
	- [x] 生成无鉴权 RTMP 推流地址
	- [x] 生成 expiry 鉴权 RTMP 推流地址
	- [x] 生成 RTMP 直播播放地址
	- [x] 生成 HDL-FLV 直播播放地址
	- [x] 生成 HLS 直播播放地址
	- [x] 生成直播截图地址
	
- 直播流管理
	- [x] 创建流
	- [x] 查询流信息
	- [x] 列出所有的直播流
	- [x] 列出正在直播的流
	- [x] 禁播流
	- [x] 启用流
	- [x] 获取直播实时信息
	- [x] 保持直播回放
	- [x] 直播流历史记录
	
## 内容

- [安装](#安装)
- [使用](#使用)
    - [配置](#配置)
	- [直播流地址](#直播流地址)
		- [生成无鉴权 RTMP 推流地址](#生成无鉴权 RTMP 推流地址)
		- [生成带鉴权 RTMP 推流地址](#生成带鉴权 RTMP 推流地址)
		- [生成直播 RTMP 播放地址](#生成直播 RTMP 播放地址)
		- [生成直播 HLS 播放地址](#生成直播 HLS 播放地址)
		- [生成直播 HDL-FLV 播放地址](#生成直播 HDL-FLV 播放地址)
		- [生成直播截图地址](#生成直播截图地址)
	- [直播流管理](#直播流管理)
		- [创建流](#创建流)
		- [查询流信息](#查询流信息)
		- [列出所有的直播流](#列出所有的直播流)
		- [列出正在直播的流](#列出正在直播的流)
		- [禁用流](#禁用流)
		- [启用流](#启用流)
		- [获取直播实时信息](#获取直播实时信息)
		- [保持直播回放](#保持直播回放)
		- [直播流历史记录](#直播流历史记录)

### 安装

本SDK安装比较简单，可以直接引入源码到项目中，然后添加编译依赖接口，具体可以参考本项目中的 [Makefile](Makefile) 文件或者 [CMakeLists.txt](CMakeLists.txt) 文件的配置。
本SDK依赖的第三方库有 `base64`，`cJSON`，`libcurl`，`libopenssl`。其中`base64`和`cJSON`已经在项目中提供。其他的两个库，请根据本地的实际安装情况来设置。

### 使用

#### 配置

在使用本SDK进行开发之前，需要准备相关的参数：

```
//账号的 AK & SK

const char *access_key = "";
const char *secret_key = "";

//直播应用名称及其绑定的各业务域名

const char *hub_name = "NIU7PS";
const char *rtmp_publish_domain = "pili-publish.ps.qiniucdn.com";
const char *rtmp_play_domain = "pili-live-rtmp.ps.qiniucdn.com";
const char *hdl_play_domain = "pili-live-hdl.ps.qiniucdn.com";
const char *hls_play_domain = "pili-live-hls.ps.qiniucdn.com";
const char *snapshot_domain = "pili-static.ps.qiniucdn.com";
```

#### 直播流地址

##### 生成无鉴权 RTMP 推流地址

```
const char *rtmp_publish_url_noauth = pili_rtmp_publish_url_with_noauth(rtmp_publish_domain, hub_name, stream_key);
printf("rtmp publish(noauth):\t%s\n", rtmp_publish_url_noauth);
free((void *) rtmp_publish_url_noauth);
```

##### 生成带鉴权 RTMP 推流地址

```
long deadline = (long) time(NULL) + 3600;
const char *rtmp_publish_url_expiry = pili_rtmp_pubilsh_url_with_expiry(rtmp_publish_domain, hub_name, stream_key,
                                                                        deadline, access_key, secret_key);
printf("rtmp publish(expiry):\t%s\n", rtmp_publish_url_expiry);
free((void *) rtmp_publish_url_expiry);
```

##### 生成直播 RTMP 播放地址

```
const char *rtmp_play_url = pili_rtmp_play_url(rtmp_play_domain, hub_name, stream_key);
printf("rtmp play:\t%s\n", rtmp_play_url);
free((void *) rtmp_play_url);
```

##### 生成直播 HDL-FLV 播放地址

```
const char *hdl_play_url = pili_hdl_play_url(hdl_play_domain, hub_name, stream_key);
printf("hdl play:\t%s\n", hdl_play_url);
free((void *) hdl_play_url);
```

##### 生成直播 HLS 播放地址

```
const char *hls_play_url = pili_hls_play_url(hls_play_domain, hub_name, stream_key);
printf("hls play:\t%s\n", hls_play_url);
free((void *) hls_play_url);
```

##### 生成直播截图地址

```
const char *snapshot_url = pili_snapshot_url(snapshot_domain, hub_name, stream_key);
printf("snapshot:\t%s\n", snapshot_url);
free((void *) snapshot_url);
```

#### 直播流管理

##### 创建流

```
char error[1024];
int ret;

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
```

上面的代码使用了一个随机的流名称，可以根据实际业务需求，指定自己的流名称。

##### 查询流信息

```
char error[1024];
int ret;

//get stream attribute
struct pili_stream_attribute attribute;
ret = pili_stream_attribute(access_key, secret_key, hub_name, stream_key, &attribute, error);
if (ret == 0) {
    printf("stream attribute ret: %d\terror: %s\n", ret, error);
    printf("stream createdAt: %ld\n", attribute.created_at);
    printf("stream updatedAt: %ld\n", attribute.updated_at);
    printf("stream expireAt: %ld\n", attribute.expire_at);
    printf("stream disabledTill: %ld\n", attribute.disabled_till);
}
```

获取流信息主要是流的创建，更新，过期时间，以及禁流状态。

##### 列出所有的直播流

```
char error[1024];
int ret;

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
```

用户可以指定需要过滤的流的名称前缀，直播状态等信息来获取直播应用中的所有匹配直播流列表，其中 `limit` 指定了每次匹配的流列表的一个最大返回流数量，如果匹配的流数量超过了 `limit` 所指定的大小，则可以通过使用返回的 `list_ret.marker` 作为下一次调用该方法的 `marker` 参数，通过循环来遍历所有的匹配流列表，直到 `marker` 为空为止。

##### 列出正在直播的流

设置上述列出所有直播流代码中的 `live_only=1` 即可过滤出正在直播中的流。

##### 禁用流

```
char error[1024];
int ret;

//disable the stream forever
ret = pili_stream_disable_till(access_key, secret_key, hub_name, stream_key, -1, error);
printf("stream disable till ret: %d\terror: %s\n", ret, error);

//disable the stream for an hour
long disable_till = (long) time(NULL) + 3600;
ret = pili_stream_disable_till(access_key, secret_key, hub_name, stream_key, disable_till, error);
printf("stream disable till %ld ret: %d\terror: %s\n", disable_till, ret, error);
```

在禁用流的时候，需要指定过期时间戳，如果指定为`0`表示取消禁用，指定为`-1`表示永久禁用，指定为以秒为单位的时间戳，则表示禁用到指定的时间戳对应的时刻。

##### 启用流

```
char error[1024];
int ret;

//enable the stream
ret = pili_stream_enable(access_key, secret_key, hub_name, stream_key, error);
printf("stream enabled ret: %d\terror: %s\n", ret, error);
```

##### 获取直播实时信息

```
char error[1024];
int ret;

struct pili_stream_status status;
ret = pili_stream_status(access_key, secret_key, hub_name, stream_key, &status, error);
if (ret == 0) {
    printf("stream status ret: %d\terror: %s\n", ret, error);
    printf("stream startAt: %ld\n", status.start_at);
    printf("stream clientIP: %s\n", status.client_ip);
    printf("stream bps:%ld\n", status.bps);
    printf("stream audioFPS: %ld\n", status.audio_fps);
    printf("stream videoFPS: %ld\n", status.video_fps);
    printf("stream dataFPS: %ld\n", status.data_fps);
}
```

##### 保持直播回放

```
char error[1024];
int ret;

const char *saveas_file_name = pili_stream_saveas_whole(access_key, secret_key, hub_name, stream_key, 0, error);
printf("stream saveas ret: %s\terror: %s\n", saveas_file_name, error);
free((void *) saveas_file_name);
```

可以通过 `pili_stream_saveas_whole` 来将一个流从开始直播到结束直播的时间段内的所有切片内容持久化为`m3u8`，当然你也可以指定这期间的一段时间，这个时候就需要用到方法：`pili_stream_saveas_period`，通过该方法中的`start_time`和`end_time`来选择你想保存的切片内容。

##### 直播流历史记录

```
char error[1024];
int ret;

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
```