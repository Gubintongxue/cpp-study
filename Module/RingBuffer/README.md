## RingBuffer

- 用于流媒体服务器

  视频采集线程进行视频采集，然后把经过编码压缩的每一帧数据循环放到环形缓冲区中。 流媒体传输线程轮流取缓冲区中的数据，进行RTP格式的处理和封装， 最后发送给每一个接入服务器的客户端

  ![](https://gitee.com/ouweibin/PictureBed/raw/master/image/RingBuffer.jpg)

  

- 用于生产者消费者模型

  生产者线程向环形缓冲区放入数据，消费者线程从环形缓冲区取出数据
