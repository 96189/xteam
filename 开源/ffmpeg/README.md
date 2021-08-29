
*   [官方文档](https://www.ffmpeg.org/ffmpeg.html)

*   [官方github地址](https://github.com/FFmpeg/FFmpeg/tree/master/doc/examples)

## ffmpeg基础
### 基本概念

    封装 解封装 转码 I帧 P帧 B帧

    采样率

### 文件格式

* MP4

* FLV

* 查看某格式的muxer和demuxer

    ffmpeg -h muxer=mp4

    截取部分信息如下，mp4默认的视频流是h264编码，音频流是aac编码。遇到的问题是mp4和mp3合并为mp4视频后，发现安卓和chrome浏览器播放正常，在Apple系设备播放异常，原因与此有关。
    ```
    Muxer mp4 [MP4 (MPEG-4 Part 14)]:
        Common extensions: mp4.
        Mime type: video/mp4.
        Default video codec: h264.
        Default audio codec: aac.
    ```


    ffmpeg -h demuxer=mp4

### 资料
* [视频处理入门教程](https://www.ruanyifeng.com/blog/2020/01/ffmpeg.html)

* 入门

    https://cloud.tencent.com/developer/article/1343202

    http://einverne.github.io/post/2015/12/ffmpeg-first.html

    https://www.cnblogs.com/leisure_chn/p/10584910.html

    https://cloud.tencent.com/developer/article/1409507

    https://wenku.baidu.com/view/f4e48c087fd5360cba1adbba.html

# 命令行使用ffmpeg
## 常用参数

```
-r 指定输入、输出视频的帧率(每s播放的图片数)，影响视频时长和质量。
-an 去除音频
-vn 去除视频
-vcodec 视频编码方式 如libx264
-acodec 音频编码方式 如aac
-threads 限制CPU使用率

```

## 常用操作
* 格式转换


* 合成视频

* 使用图片合成视频
    *   [文档1](https://www.cnblogs.com/Finley/p/8646711.html#%E5%B0%86%E5%9B%BE%E7%89%87%E5%90%88%E5%B9%B6%E4%B8%BA%E8%A7%86%E9%A2%91)

    *   [文档2](http://einverne.github.io/post/2015/12/ffmpeg-first.html)

* 对视频添加文字

    *   [文档1](https://stackoverflow.com/questions/17623676/text-on-video-ffmpeg)

* 提取视频里的图片

    ffmpeg -i {input_file} {out_path/%0{n}d.jpg}

# 通过API处理音频和视频

## api


## 音频视频混合

*   [音视频混合流程](https://github.com/leandromoreira/ffmpeg-libav-tutorial#transmuxing)

*   [使用API合成音视频数据](https://blog.mi.hdm-stuttgart.de/index.php/2018/03/21/livestreaming-with-libav-tutorial-part-2/)

### 问题
*   如何合成音频和视频

    https://stackoverflow.com/questions/20885843/merging-audio-and-video-files-into-one-c

    https://www.cnblogs.com/dong1/p/10268009.html

    https://github.com/slmax2017/ffmpeg_mp4_h264_mux/blob/master/main.cpp

    https://blog.csdn.net/leixiaohua1020/article/details/39802913

    https://cloud.tencent.com/developer/article/1409517

    https://cloud.tencent.com/developer/article/1333372

    https://www.jianshu.com/p/5778222ec22a

    https://github.com/nldzsz/ffmpeg-demo/blob/570f990d647fe6a36d431dfff91a246457ce67e5/cppsrc/audio_encode.cpp

    https://www.jianshu.com/p/3c95b0471d3a


*   内存IO模式

    https://www.codeproject.com/Tips/489450/Creating-Custom-FFmpeg-IO-Context

    https://stackoverflow.com/questions/5237175/process-video-stream-from-memory-buffer

*   mp4视频和mp3音频合成mp4，ios播放无声音，安卓和chrome浏览器正常

    https://blog.csdn.net/qq_29755359/article/details/107990606

    https://superuser.com/questions/851977/ffmpeg-merging-mp3-mp4-no-sound-with-copy-codec

    https://stackoverflow.com/questions/37890159/osx-ffmpeg-combining-mp3-and-png-to-mp4-resulting-in-mp4-with-no-audio

    https://www.cnblogs.com/rayjing/articles/3666042.html

*   api合成mp4和acc时的问题

    https://blog.csdn.net/qq_33349051/article/details/104061630

    http://blog.jokerhub.cn/geek/Fmpeg%202.8.6%20example%20code%20-%20transcoding/

    https://stackoverflow.com/questions/33147402/ffmpeg-malformed-aac-bitstream-detected-use-the-audio-bitstream-filter-aac-ad

    http://ffmpeg.org/doxygen/trunk/transcoding_8c-example.html

### MP4 MP3 ACC格式

    https://www.zhihu.com/question/35031045

    https://zh.wikipedia.org/wiki/MP3

    https://discussions.apple.com/thread/8343552

    https://support.apple.com/zh-cn/guide/compressor/cpsrf4de2b70/mac

    https://zh.wikipedia.org/wiki/MP4

    https://www.zhihu.com/question/24090216/answer/372335227

    https://www.zhihu.com/question/24090216/answer/372335227

    