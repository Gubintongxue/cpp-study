## 冲突情况一

首先甲和乙同时从远程仓库拉取v1版本的代码，然后乙先修改代码产出v3版本的代码进行提交并且成功，随后甲修改v1版本代码产出v2版本的代码，此时想要提交到`origin/master`,但是远程的最新版本并不是之前的v1了，这里就产生了冲突（红线标识）。

![在这里插入图片描述](image/c664f8fddcf3423e842d8cbdd26bdd87.png#pic_center)

### 举例如下

win这边进行修改[排序算法](https://so.csdn.net/so/search?q=%E6%8E%92%E5%BA%8F%E7%AE%97%E6%B3%95&spm=1001.2101.3001.7020)为从大到小排序

![在这里插入图片描述](image/def5cb9a7c764e8d8ad0114882743a6c.png#pic_center)

然后做`add` 和`commit`操作，但没`push`

![在这里插入图片描述](image/e88cdf7ddd1143bbbc12ac961d087335.png#pic_center)

然后ubuntu这一方做出了如下修改

![在这里插入图片描述](image/85b6848b496042469c8f4ee8411365a8.png#pic_center)

然后ubuntu这边做`add` 、`commit`和`push` 操作

![在这里插入图片描述](image/302343e7dcfc4f85bdb9e77ff813da71.png#pic_center)

此时win这边做`push`操作则会出现冲突

![在这里插入图片描述](image/92b6f3efdfa942918bfb008f52718b0f.png#pic_center)

此时输入`git pull` 会自动帮我们进行合并到本地的`master`分支，然后我们再进行`push`即可

![在这里插入图片描述](image/1c3d8f3d06544708ba9f01a83678b5dd.png#pic_center)

可以看到这是一次`merge`动作

![在这里插入图片描述](image/b1352f17848447e5adfca57b8e33810c.png#pic_center)

## 冲突情况二

首先在win这边对README文件进行修改

![在这里插入图片描述](image/bd2ce9dbff2b498fa9007cad41dc2931.png#pic_center)

同时win这边进行 `add`和`commit`操作，但没有`push`

![在这里插入图片描述](image/e2da99696d634b3f9f30621f8f67942a.png#pic_center)

然后ubuntu这边对在README文件相同位置进行修改

![在这里插入图片描述](image/6ef3360dc4044c76a719f187ca4713c1.png#pic_center)

然后win这边再进行`push`操作，出现冲突

![在这里插入图片描述](image/a654b7257ffc41b3b436e76548d50d67.png#pic_center)

然后进行`pull`

![在这里插入图片描述](image/903779baf1214c4c9c93bb2b12e3b80e.png#pic_center)

但是此时由于win修改的位置和pull下来的版本**在同一个位置进行了修改产生了冲突**，不能自动`merge`，要手动解决，此时冲突的README文件如下

![在这里插入图片描述](image/5d43f8cd521449daa2ad418af13215cd.png#pic_center)

然后win这边再做 `add`和`commit`和`push`的操作解决冲突

![在这里插入图片描述](image/98de3680d7554784ba0c9a9f0a604047.png#pic_center)