# -v
```
gcc -v test.c
```
-v参数可以详细打印出整个编译和链接过程; v即verbose

# 独立编译 -c
```
gcc -c test.c
```
-c即单独编译 test.c文件，-c即compile，该命令中不用加-o参数(即指令输出文件名)，该编译后会生成一个与源文件同名的，但后缀名为.o的文件，即 test.o

# 链接外部库 -l
```
gcc -Wall test.c /usr/lib/libm.a -o test
等价于
gcc -Wall test.c -lm -o test
```

通常，参数 -lNAME，编译器会在**标准库路径**中查找名为 libNAME.a 的库进行链接。 -l 即 link。

# 搜索路径 -I -L
```
-I path     :在path路径下搜索头文件
-L path     :在path路径下搜索库文件
```

所以我们在程序中 include 头文件时一定不要用绝对路径，因为我们在编译时会用 -I 参数指定头文件路径。在程序中使用了绝对路径后，会造成代码移植不兼容。

另外，如果我们设置了环境变量，那连 -I, -L 都不会显式指定了。头文件的环境变量为 `C_INCLUDE_PATH`，静态库文件的环境变量为 `LIBRARY_PATH`，目录与目录之间用冒号隔开。DIR1:DIR2:DIR3:...；动态库文件的环境变量为 `LD_LIBRARY_PATH`。

```
export LIBRARY_PATH=/home/xxx:$LIBRARY_PATH         ;链接程序时搜索路径
export LD_LIBRARY_PATH=/home/xxx:$LD_LIBRARY_PATH   ;加载程序时搜索路径
```

编译查找路径的优先级：

1. -I, -L 指定的路径(推荐)
2. 环境变量 C_INCLUDE_PATH, LIBRARY_PATH 的路径
3. 系统默认路径

# 创建静态库
```
ar cr libNAME.a file1.o file2.o ... filen.o
```
其中ar为创建库的命令，cr为参数，c代表create，r代表replace, 即创建或替换。

查看库文件中有多少个目标文件：
```
ar t libNAME.a      ;查看静态库中的目标文件
nm libNAME.a        ;查看静态库中的符号信息
```

# 创建动态库
```
gcc -c -fPIC file1.c file2.c
gcc -shared -o libNAME.so file.o file2.o
```

# 标准
```
-std=c89 or -std=iso9899:1990
-std=iso9899:199409
-std=c99 or -std=iso9899:1999 (推荐)
-ansi and -pedantic 告诉编译器使用标准 ANSI/ISO 来对程序进行编译，否则使用GNU C(标准C的扩展)
```

# 告警 -Wall
-W 代表 warn，后面的all代表由多个告警组成，常见的有：

```
-Wcomment       ;检查注释嵌套
-Wformat        ;检查printf/scanf传值格式是否一致
-Wunused        ;检查声明了但未使用的变量
-Wimplicit      ;隐式声明
-Wreturn-type   ;检查函数返回值
```

# 预处理，宏定义
```
gcc -Wall -DTEST XXX.c -o xxx
```

参数-D后面跟的是一个宏的名字，即如果XXX.c中有使用 TEST 宏而本身又没有定义这个宏，可以使用命令行把 TEST 这个宏给传进去，使用的就是 -D 这个参数。

我们还可以使用 -D 使宏有值，格式： -DNAME=VALUE，如果不指定VALUE，则默认为1。 还可以写成表达式： -DNAME="1+2"

GCC 中提前预定义好了很多宏，我们可以使用如下命令查看这些宏的定义：

```
cpp -dM /dev/null
```

gcc 的预处理选项 -E ：

```
gcc -E test.c -o test.i 
```

这种方式可以让我们理解比较复杂的宏定义。 当然我们还可以使用如下命令来保存编译的中间结果：

```
gcc -Wall -c test.c -save-temps     ;-save-temps参数会保存编译中间结果，源目录中会生成 test.i, test.s, test.o 这三个文件
```

# core 文件
默认情况下 Linux 不生成 core 文件，输入命令：

```
ulimit -c       ;如果输出结果为 0，表示不会生成 core 文件
```

如果想产生 core 文件，则输入命令：

```
ulimit -c unlimited
```

使用 gdb 分析 core 文件：

```
gdb a.out core.2297
```