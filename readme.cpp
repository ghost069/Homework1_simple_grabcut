//编译：
g++ grabcut_opencv.cpp -o grabcut `pkg-config --cflags --libs opencv`

//使用：
1. grabcut [filename1(foreground)] [filename2(background)]

2. 在弹出的窗口中用鼠标选中人物，同时终端会提示鼠标操作与鼠标位置。松开鼠标即得到最终结果。


//注意事项：
途中会生成一个临时文件temp_resized.jpg,不可避免。请确保所在位置有足够空间与写入权限。


//算法优点：
使用便捷，生成迅速。

//算法缺点：
由于矩形框的局限性，人体向外舒展导致矩形框内背景过多的时候，如果想把周围的背景完全切去，难免切去一部分人体（如result1），或留下一部分背景（如result2）
