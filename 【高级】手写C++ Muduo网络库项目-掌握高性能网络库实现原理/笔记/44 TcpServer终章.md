之前呢，我们已经把这个TCP connection相关的重要的成员方法呢，都已经输出完了啊。如果说我们在讲TCP connection本身的时候，有一些接口，包括它的一些细节的实现，你还有一些疑问的话。我相信你今天把这个TCP server。这一个类相关的这个核心的操作，我们在写完了分析完的话，应该绝对都能够解决你的一些。疑问啊，好之前我们已经写了一部分了，对吧啊？跟我一起来回忆一下啊，

希望你不要忘啊，之前呢，我们也结合它。画了这样的一个它的一个交互图嘛，是不是啊？当创建一个TCP server的时候，这里边最重要的就是。我们接触了一个妹路普，对吧？创建了一个accept or注意acceptor acceptor，这个构造函数相当于就是我们在编写TCP。服务器程序的时候呢，创建一个socket是吧？l blocking的socket，然后去绑定IP地址端口号，

然后进行listen。这是呢，创建了这个底层的，这个什么event loop pole呃event loop thread pole。呃，事件循环线程池啊。刚开始默认那这个池里边儿就没有创建过额外的这个线程嘛，对吧？事件线程呃，默认只有一个主线程跟一个主路May loop。这里边呢，绑定了一个回调，唉，绑定了一个回调，注意accept new connection。

这什么意思呢？首先呢，你回忆一下构造在这个accept or的这个构造函数里边啊，首先呢，它是不是绑定了一个？accept or相对应也有一个China，我们它也是属于一个对于类似fd的一个封装，对吧？call back当有新的事件，连接了以后呢？这里边就会执行这个。handle read啊，handle read，handle read在这里边呢，相当于就是调用excel返回一个跟客户端通信用的这个新的。

connect fd这个connect fd呢，我们最终就要把它进行一个封装。对吧啊，进行一个封装，把它分发到这个saber loop里边，大于等于零的话呢，如果有这么一个new connection call back，就会执行这个回调。啊，把客户端通信用的这个connect fd以及客户端的IP地址端口呢，传给这个回调，这个回调当然就是TCP server给accept or了。是吧啊，就是呢，我们的这里看到了吧？

I accepted set new connection call back。这是什么方法呢？对应的是不是就是它的这个new connection方法啊啊？就是new connection方法。OK吧，它还提供了一个方法set threat number是设置底层的，这个事件循环线程式的线程数量的啊，当start以后，它通过一个原子的变量started。在控制了一下呢，在一个县城里边儿呢，只能去调用一次TCP server，是不是start呀？这一句话呢，是启动底层的这个路补线城池啊，

这个呢，就是开始启动什么啊，开始启动这个listen了。啊，开始启动listen监听新用户的连接，新用户连接成功以后呢，返回连接通信用的这个通信用的这个connect fd。是不是就会执行我们预制的这个回调啊？就到new connection里边儿来了，也就是说。有一个。新的客户端的这个连接。会执行。这个回调。回调操作。

把客户端通信的这个socket fd以及客户端的IP就是端口号打包的，这个ML的address传给是不是这个回调啊？这个没有忘记吧啊，忘记的同学呢？去看一看这里边儿。的这个处理。好不好啊？因为我们现在今天这节课要写TCP server了，你不能把它的这个核心给忘了啊，核心给忘了，但是呢，基于这样的一个流程，我们把这个图是不是还给大家？画了一下，那么大家来看我们，

现在呢TCP connection完了以后呢，就可以直接实现这个new connection了。大家来跟我看一下啊，这个new connection的这个实现。首先呢？这就是我们图上画的啊。图上画。执行这个。开始要进行选择一个sub loop，要派要封装connection，要派发这个connection了。对不对？首先呢，一上来。定义了一个LOL oop。

调用了thread pole。什么get next loop？这是轮询算法。选择一个选择一个，这个sub loop。来来管理是不是对应的这个channel啊？一个TCP connection对应一个channel啊？然后这要给connection一个名字了，所以他先定了一个。64字节的一个差八分。printf.大家涉及字符串连接，可以用一下这个多多用一下SN printf啊。这是一个比较安全的。这里边杠百分之s由于井号啊，

百分之d啊。IP port.点c杠STR。然后就是next connect ID啊。这表示了一个连接的名称。然后就给next action ID加加这个呢，之所以定了一个整形的变量，没有进行原子变量的定义，因为这个不需要。因为这个new connection只有在me loop里边儿处理，只有一个线程在处理它是吧啊，不涉及线程安全问题。st d string.CON n name等于。内杠加这个。

buffer啊，最终这是connection的这个名称啊。然后在这儿加了一个login fo，加了一个。用日志打了个点啊。TCP.server connection在这里边儿。加了一个中括号，这是以百分之s。然后写了一个new。connection.又是一个百分之s。from啊from谁是吧？from百分之s。先是name杠c杠STR。

再是CON name点c杠STR。再是p点AD点r two IP pot。这返回的是个string还是c杠STR？好，大家在这里边跟我看啊m at address local addr。用了get local addr相当于呢，就是用了通信的这个so pfd。来获取了。当前的IP就是跟这个socket绑定的IP地址跟端口号嘛，对吧啊？通过sock fd。获取。其绑定的本机的IP地址和端口信息。所以相信大家这个方法呢，应该是用的不多啊，

应该是用的不多，在这儿呢，我们可以直接。直接过去。get local addr.我们把这个。方法直接拿过来啊。直接写到这儿吧。这是我们的。sock add rin.这是一个local a。local啊。然后是b zero。包含一下相应的头文件吧。strings点h啊。

bz e.b zero.local set of local.然后呢？就是什么方法呢？so CK len啊add rl en。等于什么？等于一个size of。local好吧。啊，接着就是。get sock name.第一个就是sock。fd.我们的socket fd。

OK吧啊，就是这个socket fd第二个就是什么呀？sock addr这也是一星号。local.然后呢？就是add rl en，如果其小于零的话就表示。有问题啊，我们给它打印一个l，这是打印一个l级别的。好。好，这就是这一段代码的这个含义啊。接下来就可以直接创建谁了。创建。

根据。连接成功的sock fd。是吧，创建TCP。connection.连接对象。好在这呢！TCP connection ptr用智能指针来管理的啊！new TCP。connection.第一个是什么参数啊？LOL oop它所在的这个事件循环。第二个就是它的这个名字啊。第三个就是它的这个。底层打包的这个fd。

通过这个fd，可以创建底层，就可以创建这个socket对象，跟这个channel了是吧啊？然后接着是什么？是local。本地的IP地址跟端口号是吧？以及对端的就是客户端的IP地址跟端口。好了，在这呢。就写完了啊。我们看一下。incomplete type.is not allowed.在这里边，

我们为什么会有这样的一个错误呢？呃，看一看，这是一二三四五五个。构造函数，五个参数，一二三四五。没啥问题是吧？啊，先写吧，一会儿编译再跟我们真正报错的时候，我们再来解决。然后呢？TCP server不是有一个map嘛？你看你看你回去看看它的这个成员变量啊，

它有一个map嘛。这里边呢就是。连接的名字以及对应的这个连接存到这个connections里边了好吧。那我们现在呢，也可以存起来啦。connections中括号儿，它的connect是name。就等于当前的这个conn了。然后呢，就可以给它设置什么东西了。set.connection call back.设置connection call back。你看这个哈。回调。

下面的回调都是从从这这个都是用户啊设置的。设置给TCP server的。ver，然后TCP server又设置给谁呀？TCP connection，然后TCP connection又设置给China。channel注册到polar上去，polar最终又通知not not if I通知谁通知channel。是不是调用回调啊？OK，这条路子现在能搞明白啊，我们已经强调很多遍了。然后再conn。我们这个有错误，它所以它联想不了啊。

嗯。我先编译一下吧，看具体的错误是什么啊？这个是第79行。invalid use of incomplete type.class这个我们看看啊。嗯。这里边我们隐藏了一个错误呢。这里边出现了一个in不完全的，这个类型是吧？嗯，大家来跟我们看一下哦，那是因为我们在这里边只是类型前置声明了，没有包含人家相应的这个头文件。所以没有识别是吧？

现在是不是OK了？唉，现在OK了啊？OK，就能够帮助我们，所以在这一点上VS code是远远不如这个clan的啊。clan的这个代码帮助在很多异常情况下还是非常强大的。这个message call back。这是我们最最经常设置的两个，是不是？然后connection。还有这个set red complete。read complete callback.你看原封不动的TCP server的这些copy，又设置给TCP connection。

哎，接下来这个就有意思了。就是什么CON指向的，我要着重的给大家去，再把这两个强调一下啊跟我看看。STD.band TCP.s over remove connection.this STD.please holders.一个参数占位符啊，一个参数占位符。然后接下来就是l loop。执行的run in loop。STD.

半TCP。connection.它的connection established。coin.好了，在这里边儿，大家来注意跟我听一下啊，我这里边儿先着重的介绍一下这个，这个呢new connection什么时候掉的呢？I accept or。有一个心。客户的连接在这里边呢，就会帮我们去调用这个回调，这个回调呢，就是我们TCP server在这里边预先。

给accept or设置的这个pcp server，new connection当有一条新连接的话，它在这里边儿所做的事情，先是组装了这个连接的名字。然后呢，创建了这条连接。给这条连接呢，绑定了相应的回调，这些回调呢，我们刚才说了是吧，最重要的是啊，在这里边儿设置了。设置了如何？关闭。连接的。

这个回调。对吧啊，设置了如何关闭连接的这个回调？OK吧啊，你看这里边是不是有这个remove connection啊啊TCP server的这个？remove connection啊。好，如何关闭的？那在这里边儿呢，这个是回调，我们暂且先不说，因为这里边儿肯定不会去直接执行这个方法了，回调是在其他地方，是不是这个执行的这个啊？我们先不看，

大家先不关注，之前你已经知道了，但是有些同学可能没有留心，最重要的是在这里边。啊，直接调用。这个TCP connection的。这个方法也就是说呢，现在只要有一个新的客户端连接。最终呢，就会生成相应的TCP connection对象，并直接调用TCP connection对象的connect。establish的方法，做的什么事情呢啊？做的事情就是把这个连接它的这个state从刚开始的connecting变成connected连接建立。

哎，然后抬了一下。然后呢，把当前的这个channel呢，就是跟客户端通信的这个channel啊，然后直接enable reading这个相当于就是向puller注册了China。channel的是不是epo in可读事件啊？唉，然后呢？这一块儿呢，就破了就开始呢，就监听。是不是监听这个channel上的这个事件了啊？然后之前呢，最这里边儿就是选择了一个相应的这个路谱。

啊，鉴定事件以后呢？这里边儿就执行回调了。啊，我们用户预置的那个on connection方法呢？就会。调用到，然后我们在那里边呢，比如说调用connection的connected方法就会看到啊，连接已经建立成功。因为这里边儿已经设置状态了嘛，你在调用connection的connected方法的时候，我这一发现state是不是等于connected呢？当然了，连接建立成功了。

是吧啊，连接建立成功了。这一套流程。应该搞明白了吧啊。好，我再在这再阐述一遍啊，再阐述一遍。在这里边儿呢，相当于。创建了1 new connection，你看创建了1 new connection在这儿啊。创建new connection在这里边绑定了一个什么？绑定了一个相应的一个。绑定相应的这个回调。然后调用了这个TCP connection，

the connection established。对不对？比如说选择了这个l路普吧，那相当于就是把相应的这个China相应通信的这个。connect fd注册到是不是？这个萨博路普的这个polo上啊啊，注册好以后呢？呃，就把当前这个连接的状态从connecting改成connected。是不是在调用了你，你用户的这个on connection回调？就有连接建立成功了。是不是啊？就这个很简单。那么，

在这里边儿，如果说是我们的。连接关闭了，我们之前都说了嘛。用户会调用connection的什么方法？12档。这是关闭走的流程。调用这个上下档，那么同学们来看上下档做的事情是什么事情啊？就是你之前连接了才能上下当嘛，对吧啊？上下当下下当完了以后执行这个。啊，等待数据发送完，数据发送完，

它直接调用这个数据，未发送完，它是handle red等数据发送完了以后，由于状态改成了disconnecting它依然。最终会调用到沙当应路普，那么也就是说呢？最终的这个核心呢？最终的核心就会调用到socket的什么shut down right。杀当right关闭这个socket写端以后呢底层的这个China啊polo就会给China呢上报的这个。e破HUB事件那么相当于这个？连接就要进行一个关闭channel调用的，这个close的这个call back这个回调。channel的这个close call back是谁给的？就是TCP connection给的。TCP connection给的这个。

close call back那TCP connection的close call back是谁给的吗？那就是TCP server给的。好吧啊，就是TCP server给的。那么在这里边，你来看看。底层的这个channel啊。被调用这个close call back以后，调用的就是connection的谁啊？handle close了。是不是憨豆close啊？底层channel底层的这个polo啊。通知channel调用它的什么方法啊？close call back方法在这里边，最终就回调到connection的什么方法啊？

connection呢，是不是handle close方法啊？哎，在这里边大家看set state。啊，那你从杀了当刚才的disconnecting变成disconnected，然后呢？把这个什么？China的这个事件啊。感兴趣的事件从polo上是不是全部给它删除掉啊？没问题吧啊。OK，然后是获取当前的这个connection对象，这没有新创建connection对象，这只是个智能指针啊，

获取当前的这个。对象，然后调用这个connection回调，这相当于就是连接关闭掉了。对不对啊？我们用户注册那个on connection连接建立成功，跟连接关闭都会响应到，然后这里边是执行close call back，这个close call back是谁给的呀？这执行的是TCP server。啊TCP server在这里边给的，你看set close call back。也就是执行的是这个。回调方法。OK吧，

你看在这里边儿关闭一个这个c啊，这里边儿做的事情，把这个状态改一下。China呢，从底层po去掉所有感兴趣的事件。然后执行用户的回电有连接呢，关闭了是吧？再执行呢，这个TCP server的remove connection。做了什么事情呢？大家来跟我看。就剩这一个接口了。我们把这两个再实现一下啊。大家来跟我看一下，这里边儿呢remove connection丢呢，

还是remove connection in loop？loop指向了run in loop。s kd.band TCP connection.它的。哪个方法啊？唉，这个不是TCP connection啊，是TCP server。的这个remove connection in loop。this,conn.好了吧啊。注意一下啊。诶，

这个不加封号，这封号应该是在这儿加的。那么大家来。注意在这里边儿呢，最终调用的就是remove connection in loop，那在这儿做了什么事情？在这呢，先做了一个。日志的一个打点记录啊。name百分之s。然后呢，先是这个name杠点c杠STR。对吧啊，第二个就是一个连接。百分之s。

conn.指向的。name c杠STR。好，这日志呢？我们算是这个输出完了。size杠tn=connection。connections erase啊，先把它从那个脉谱表中先怎么样？先删掉。connection的名字作为这个键对吧啊？a=1就说明呢，断言真的是要删除一个erase这个返回的就是删除的。个数对吧啊？我们对于返回值就不用判断了吧？

这个event loop。LOL oop这没有获取一个新的。还有路虎，这只是拿到了这个connection。获取这个connection所在的这个loop OK吧，做什么事情呢？l loop困印loop。半啊，拐来拐去又拐到这个。connection的destroy身上了啊。好了，所以呢，我们最终呢，我们又搞清楚了这个它的删除是怎么做的了啊？把它删除就是这样做的。

首先呢，由这个。decimal polar通知channel。这个socket要关闭了是吧？关闭的话呢？channel调用的这个close call back这个回调方法。就是之前的TCP connection给它预制的，这个handle clothes。状态改了一下。channel的这个事件从polo中全部删除。给用户进行通知连接又关闭了。close call bag这个调用呢，就是预制的就是TCP server remove connection。啊，那这个回调这个方法的话呢，

把这个相应的connection信息在TCP server里边儿，这个connection map里边儿删除掉。完了以后呢，拿这条连接对应的这个路谱啊，然后再去执行这条连接的什么？connection destroy.相当于呢，就说到底啊。就是channel调用close call back回调先执行了TCP connection的handle close方法，然后又跑到TCP server里边儿。把这个连接从它的connection map里边儿删掉，然后再回到TCP connection执行它的这个什么方法啊？它的这个handle，这个connection destroy方法对吧啊？第四位方法是什么？

channel直接remove就这个直接remove了，把channel从pro中这个删除了。这就完了。好的吧，哎，这个呢，那就完了。希望大家把它的这个连接的这个建立啊，以及连接的这个remove它的这个整个逻辑给它搞清楚啊，它的回调呢还是？比较多的，我们后边儿呢，整个儿输出完代码，输出完我们在总体的这个流程上再给大家去总结一下啊。还欠一个就是它的这个析构函数啊。

我们来看一下它的这个析构函数。做的事情。日志我们就不打印了吧？这里边就是做的事情，就是connections啊。便利这个connections。然后把这里边的什么东西啊？呃。it指向的second，it指向的second，就是连接的这个智能指针reset啊，那就是不再用智能指针去。去不再用这个强智能指针去管理资源了，相当于资源就没有人管理了，就自动怎么样啊？

就自动。就可以释放掉了，否则的话，有强制能指针去指向资源，资源是无法释放的，对吧？然后再把所有的。connection再怎么样再去destroy掉，相当于把连接呢删除掉。啊，auto item。connections.在这里边，我们直接就是item点。second.

点什么啊？哦，我这里边还不能直接怎么样啊？不能直接去reset，要不然呢？这个。没有办法去给谁啊？呃。我们看能不能直接去reset呀？直接去reset的话呢，connection对象是不是就没有人访问了啊？在这里边相当于就是。item指向的这个second。来，我给大家说一下，

这里边这样写的这个好处啊。首先你要看清楚在TCP server的成员变量里边儿，它本身有一个connection map，它的这个键对应的这个值就是一个指向啊。TCP connection的一个强制能指针。是不是啊？在这里边做的这件事情就是。TCP server本身的这个强智能指针，它不会再指向TCP connection对象了，它现在让这个一个局部的一个对象。智能指针对象来指向。啊，好吧，它的好处是出了这个诱惑号。TCP connection这个对象。

就可以被怎么样了？可以，这个局部的。强就是share的ptr智能指针对象。出右括号。可以自动。释放什么还可以自动释放是不是？new出来的。new new出来的。出来的TCP。connection对象资源啊。对不对诶？这就是它的这个好处。啊，这就是我们在看人家源码，

从人家源码上得到的一些细节性的设计啊，这个考虑的还是非常不错的。通过这个连接。如果说是啊。如果说是我们刚才呢，直接把a atom点second reset了，那我们也就无法去再访问new出来的TCP connection对象了，对吧？在这儿也就没有办法了，再去调用pcp connection的这个connect destroy方法啦，各位。run in loop.sdd.band TCP connection.它的connection destroyed conn。

除了这个右括号。这个对象就被释放了，是不是啊？刚好啊，刚好。这里边就执行了这个销毁连接。销毁。连接对这个内容也是代码逻辑表达的，这个内容也是非常的清晰啊。那整个的这个过程呢，我们就完了。这个代码核心的代码我们就完了啊。我们这个大的这场唱不容易啊。这个同学们能从前往后看到这一节课的也是非常的不容易。希望呢？

把我们所讲的。代码的设计，尤其是流程相关的结合，这张图流程相关的好好画一画，画清楚啊，我们学了这么。多节课一定要把model库表达的核心的，这个所谓的one loop per thread。这个编程模型啊。给大家一定要搞清楚啊。这个我们付出一定需要有收获啊。如果呢，中间还有一些逻辑上的一些不明白的地方，我们后边儿呢，我们还会给大家再去整体的讲一下这个流程。

啊，再梳理一下，希望大家有什有疑问的啊啊，再去解决一下好吧啊，如果还有什么问题呢？好，欢迎大家呢。在网上来咨询老师啊，在网上来咨询老师。那整个也编译通过了，那下节课我们就开始呢，去检测一下我们所写的这个model库。它的这个。在开发服务器的时候，是否能开发一个正常功能的这个服务器？

好吧，我们现在写这个，写好了以后呢？大家可以看到我们。这个model库我们编译出来so库用不用依赖boost库啊？不用是吧？我们是全部把依赖的boost库翻译成CA加幺幺的，这个语法依赖的。呃，非常的轻便，而且里边儿呢，很多跟多线程有关的，我们也都直接翻译成CA加幺幺相关的。跟linux底层的这个API偶合性也比较少了。相对来说，

可一致性是不是也就是高很多了啊？呃polo这个地方，我们只做了一个实例化，就是支持这个epo啊，支持epo大家后续呢可以。把破也扩充出来，人家木头库也扩充了破，对吧啊？好，那希望大家听完这节课，把TCP server相关的这个代码补充完，主要呢了解一下TCP server在这里边儿怎么去启动accept or。没look的对吧啊，如何是拿到新的这个连接客户端连接的connect fd？然后如何呢？

去回调到new connection创建连接啊，分配一个sub loop。怎么去注册这些回调？怎么去注册呢？连接的建立跟连接的这个删除的。是不是啊？我们的TCP server跟TCP connection做的事情就是这些事情。剩下的就全部交给疑问的路。跟polar呢这个事件分化性。对吧啊，整个的这个设计呢，还是非常不错的，希望大家呢，花一些时间好好的去研究一下对。对于大家的这个oop的这个编码，

思编程思想啊，以及这个one loop pro thread就是通过这个l复用。啊epo加上这个l blocking这么一个网络服务器的这个设计啊，都是有很大帮助的啊，有很好的这个提升的。好，这节课的内容呢？我们就给大家啊，说到这里。