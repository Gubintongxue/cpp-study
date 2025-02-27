# muduo网络简介

## muduo基本使用和原理

### 底层epoll+linux 多线程  boost

这节课我们给大家说一下muduo网络库的一个基本的使用和原理啊。

首先呢，这个muduo库呢，实际上底层啊，用的就是epoll+linux的这个pthread的线程库啊，

所以呢，它只能装在这个linux的环境当中。

这个它还依赖boost库,boost库是一个也是一个用C++写的，非常强大的一个开源库啊，

那么这个怎么安装啊？

muduo库的这个编译安装和环境搭建这个我博客上也已经给大家写清楚了啊。

大家进入这个博客，一步一步来就行了，相关的boost库呢，还有muduo库的一些源码文件啊，

你也都可以在这个文章上有下载的这个链接地址。



那么，在课件上呢？

我给大家列举了一些啊muduo库比较重要的内容，比如说像这个muduo库的客户端服务器的编程。

实际上，在我们的项目中，因为我们的服务器一般要做到高并发，对吧？

所以我们服务器端呢，就用muduo库的编程客户端在这里边，只是向服务器请求服务啊，

客户端本身呢，并不需要有高并发的这样的一个性能要求。

所以客户端你想用muduo库的客户端编程也可以啊，不想用也无所谓，

那么应该说是在线下讲课的时候呢，我们给大家呢？

呃，在做这个网络项目的时候都给大家说过这个muduo库的一些应用，对吧？

那么在这节课里边呢？我们给大家再汇总总结一下啊。

大家在linux上去开发一个网络程序呢，应该用的最多的这个第三方库啊就是libevent还有这个muduo，

网络库实际上它们俩都是支持这个epoll加线程池这样的一个。

啊，基于多路lO复用的epoll的这样的一个网络模型，其性能是非常强大的。

那么在这里边，我给大家列了这么一个编程的这个简要说明，然后呢，又给大家介绍了在muduo库中还可以专门去。

呃，人家封装了啊，对于这个线程的这个创建啊，包括这个线程池。有thread pool对吧？



这如果说是项目中啊，在业务代码上，如果想使用这个线程池啊，里边儿，可以创建线程给线程的提交任务。

muduo库里边儿也有直接的这个方案啊，来做这件事情。那么muduo库还有完善的一个日志系统好吧

啊muduo库呢？它的网络模型在这里边，我给大家列举了。

就是从最初始的单机到后来的啊，逐渐变成高并发的网络模型，就是网络模型的这么一个迭代。

啊，直到呢，最终的这个reactors in process就是基于进程的，这个reactor模型也就是nginx采用的网络模块设计。

nginx相信呢，大家并不陌生啊，它是一个非常强大的http服务器，还有反向代理，负载均衡的功能。

在这个项目中呢，我们就会基于它的TCP负载均衡来做，我们的这个聊天儿服务器的这个集群配置啊。



嗯，那么在方案四就是我们muduo库所采用的就是非常著名的one loop pro thread，就是一个线程啊，

一个事件循环。说直白点啊，大家要去设计一个这个高性能的网络啊，

模型实际上不管是这个C++里边的网络库啊，还是呢像JAVA里边的这个网络库呢？

实际上，做到高并发，基本上啊，都是这样去设计的，



首先呢，它有一个lO线程啊,lO线程拿这个muduo库里边来说。

那么IO线程里边有一个epoll。IO线程专门做什么事情啊？

我们可以把它称作Main reactor。就是主反应堆对吧？

这个Main reactor呢？主要通过它的一个accept or就是在muduo库上的一个实现啊，做什么呢？

做这个新用户的链接。是吧，这是lO线程啊，

那新用户的链接的话呢？链接上了以后，它会把这些链接呢分发给啊，通过一定的负载算法呢，

分发给不同的。

唉，稍等一下，我把这个图简单给大家画一下啊，

分发给不同的这个工作线程。

工作线程啊，实际上呢，在这里边自然是个epoll啊。

这个工作线程上的这个epoll呢，做的事情是什么事情呢？

就是处理已连接用户的读写。是不是事件啊？读写事件啊，读事件和写事件。

这个lO线程呢，是专门去做这个新用户连接的，

一般这个线程的数量呢，会跟CPU的核数对等，这样来说呢，尽量就做到高并发了。

那么，用lO复用的好处是一个线程呢？可以监听多个是不是套接字啊？

尤其是呢，对于这个连接量大而活跃量少，一般都是这样的一个场景啊，epoll有其非常大的这个性能优势。

那么，在这个网络环境中，我们这样去设计的话啊，那相当于就是啊，加上一个线程池是不是唉？

这线程都可以通过线程池来创建出来。

这实际上啊，就是muduo网络库的这个底层实现啊。就是muduo网络库的一个底层的，这个实现那么如果说这每一个工作线程的epoll啊，所监听的已连接用户。

如果要做比较耗时的IO操作，比如说传文件。

啊传视频传音频对不对？那就需要单独开一个线程。做这个耗时的IO操作啊，比如说是传送文件啦，

音视频啦，是不是唉？

这时候呢，就单独开辟线程来处理。没问题吧，



如果你还是在同样的这个工作线程epoll线程上去处理，这个比较耗时的IO操作的话呢，

那当前这个epoll可能就无法及时的处理，其他依然注册在这个epoll上的这个fd啊，

就是socket的读写事件了。



这个相信大家应该都能够明白。那么，在这里边儿呢？

给大家列举了一下啊，==方案的特点是one loop per就是一个线程，一个事件循环==，

有一个Main reactor负载accept连接。

相当于就是这里边lO线程啊，然后呢，把连接分发到某个sub reactor就是子反应堆上啊，

==这个muduo库呢，采用这个round Robin就是轮巡的方式呢，来选择这个sub reactor==啊。

那么，该连接的所有操作都在那个sub reactor所处的线程中完成，你既然注册到哪个工作线程的epoll事件上了，那你所做的这个所有的读写操作呢，都在这个epoll上完成。

好吧啊。多个连接呢，可能被分派到多个线程中，以充分利用这个CPU那一般上来说呢reactor poll的大小是固定的啊，根据CPU。

这个数目确定意思就是说这线程的这个数量。一般根据呢CPU的核数来确定啊。

呃。在这里边儿给大家列举了一些内容啊，如果有过多的。如果有过多的耗费CPU IO的计算操作可以提交到创建的threat pool线程池中，专门处理耗时的计算任务。



也就是说，这里边给大家说的，如果呢，你某一个已连接用户的可读写事件，需要做耗时的IO操作，对吧啊，包括CPU IO。磁盘网络lO是不是啊？

那我们需要单独开辟线程来操作，单独开辟线程，你在这里边儿，如果你使用muduo库的话，我们说了啊，你muduo库有专门的thread pool，你可以自己独立的去创建。

线程池给它里边放一个线程，放三个线程，放五个线程啊，这是由你自己来确定的好吧？

那么，在最下边这里边儿呢？

我给大家又列举了一下muduo的这个reactor模型，reactor和proactor是我们网络常用的啊。

这个高性能的网络设计模型在这里边呢，给大家还截取了一段维基百科对于reactor模型的一个解释啊。

这里边儿就主要涉及了反应堆事件分发器是吧啊，还有一个服务处理器。

就是我们像一大家在平时呢，学习的时候呢，可能仅仅只是向一上注册了一个。

套接字对吧？然后当这个套接字发生事情的时候呢，你才去解决它并没有进行一个合理的一个oop的封装啊，

实际上呢，我们可以给每一个这个像epoll上注册的这个fd就是socket匹配一个任务处理器嗯，

就是相当于一个回调啊，当这个epoll监听到socket或者是我们注册的这个fd啊，有事件发生的话呢，

我们可以匹配到这个fd所对应的这个任务处理器啊，业务处理器直接把它处理掉。



好吧，那么这里边就是Main reactor  这是客户端的连接，



它muduo库上有一个accept or这么一个对象啊，专门封装了。

我们在这里边儿所说的lO线程里边儿做的新用处理，新用户连接的事件的这么一个类对象啊，

然后呢，通过round Robin来分发到不同的子反应堆上。

子反应堆再进行读写对吧啊，编码解码唉，



可能因为你这个数据流过来可能是一个json。是不是字符串啊？我需要把Json字符串解码成一个数据对象，是不是才能从数据对象里边获取到键值对儿？

啊，等我发送给用户，发送的是send的时候啊，

那我需要呢？把数据对象啊，把我组装的数据对象先给它序列化成encode编码成这个节省字符串，

然后呢，再通过网络啊发送出去。



好的吧啊，那么这节课呢，我们主要给大家去讲解一下这个muduo啊，网络库。它的一些基本的知识内容。呃，当然了，如果要对它进行深入的了解啊，那你需要把muduo库的这个代码呢？

啊，拿出来，因为它是个开源的，仔细的去阅读一下啊，它的整个的源码也就是五六千行，但是呢是。是非常经典的，尤其是像你在学习这个oop语言C++。

啊，它有很多非常好的封装的，这个思想里边儿大量的使用了，像这个智能指针啊。啊，绑定器啊，函数对象回调机制啊啊，

都是非常好的，可以做到模块儿的这个解偶啊，做到软件的这个高内取低偶合啊。好，这节课的主要内容就给大家先说到这里。