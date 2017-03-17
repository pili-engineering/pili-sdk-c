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

- [安装](#installation)
- [使用](#usage)
    - [配置](#configuration)
	- [直播流地址](#url)
		- [生成无鉴权 RTMP 推流地址](#generate-rtmp-publish-url-noauth)
		- [生成带鉴权 RTMP 推流地址](#generate-rtmp-publish-url-expiry)
		- [生成直播 RTMP 播放地址](#generate-rtmp-play-url)
		- [生成直播 HLS 播放地址](#generate-hls-play-url)
		- [生成直播 HDL-FLV 播放地址](#generate-hdl-play-url)
		- [生成直播截图地址](#generate-snapshot-play-url)
	- [直播流管理](#hub)
		- [创建流](#create-a-new-stream)
		- [查询流信息](#get-stream-info)
		- [列出所有的直播流](#list-steams)
		- [列出正在直播的流](#list-live-streams)
		- [禁播流](#disable-a-stream)
		- [启用流](#enable-a-stream)
		- [获取直播实时信息](#get-stream-live-status)
		- [保持直播回放](#save-stream-live-playback)
		- [直播流历史记录](#get-stream-history-record)

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



