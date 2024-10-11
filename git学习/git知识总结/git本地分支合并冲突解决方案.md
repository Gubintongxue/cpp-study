## 冲突情况描述

![在这里插入图片描述](image/142200f1ba73416c8a9708b94768e29e.png#pic_center)

## 实例演示

有两个用户 win 和 ubuntu 进行协作开发

首先win方创建并[切换分支](https://so.csdn.net/so/search?q=%E5%88%87%E6%8D%A2%E5%88%86%E6%94%AF&spm=1001.2101.3001.7020)

![在这里插入图片描述](image/3a41e96ce7d241ab87f3b51bacb59ea0.png#pic_center)

然后win对内容修改

![在这里插入图片描述](image/bb828ec758714afd8037eccb8efc9258.png#pic_center)

然后提交修改进行`add`和`commit`，但还没切换回master 合并

```sh
Administrator@DESKTOP-FN2M7LS MINGW64 /d/cplusplus/00个人笔记/分布式版本控制gitgit实战课程学习资料/git实战课程学习资料/test/HelloWorld (sortdev)
$ git add .

Administrator@DESKTOP-FN2M7LS MINGW64 /d/cplusplus/00个人笔记/分布式版本控制git/git实战课程学习资料/git实战课程学习资料/test/HelloWorld (sortdev)
$ git commit -m "win修改readme，添加222222"
[sortdev 7bac9ce] win修改readme，添加222222
 1 file changed, 1 insertion(+)

```

ubuntu在master修改readme文件与win中相同位置333333，并执行`add`、`commit`和`push`

![在这里插入图片描述](image/b5fcf040d50e47ac980c11c1aae619fd.png#pic_center)

然后win切换回master分支，并进行`pull`操作，然后执行与`sortdev`进行`merge`的操作，发生**冲突**：

![在这里插入图片描述](image/38356952e2fd4b0f8201d6960c88409e.png#pic_center)

手动解决冲突，冲突文件如下

![在这里插入图片描述](image/6921534daa8148f186897c97cfa012a9.png#pic_center)

修改完冲突后就再进行`add` 、`commit` 以及`push`即可