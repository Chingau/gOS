# 基本语法
```
目标：依赖
tab 命令
```

目标：一般是指要编译的目标，也可以是一个动作

依赖：指执行当前目标所要依赖的选项，包括其它目标，某个具体文件或库等，一个目标可以有多个依赖

命令：该目标下要执行的具体命令，可以没有，也可以有多条；多条时，每条命令一行

# Make常用选项
可以在终端中输入 `make -h` 来查看帮助。

```
make [-f file][options][target]
```

Make 默认在当前目录中寻找 GUNmakefile, makefile, Makefile 的文件作为 make 的输入文件。

-f 可以指定除上述文件名之外的文件作为输入文件

-v 显示版本号

-n 只输出命令，但并不执行，一般用来测试

-s 只执行命令，但不显示具体命令，此处可在命令中用@符抑制命令输出

-w 显示执行前执行后的路径

-C dir 指定 makefile 所在的目录，即调用指定目录下的 makefile

# Makefile中的变量
**系统变量**

|变量|含义
|:--|:--
$*|不包括扩展名的目标文件名称
$+|所有的依赖文件，以空格分隔
$<|表示规则中的第一个条件
$?|所有时间戳比目标文件晚的依赖文件，以空格分隔
$@|目标文件的完整名称
$^|所有不重复的依赖文件，以空格分隔
$\%|如果目标是归档成员，则该变量表示目标的归档成员名称

**系统常量** (可用 make -p 查看)

|常量|含义
|:--|:--
AS|汇编程序的名称，默认为 as
CC|C编译器名称，默认 cc
CPP|C预编译器名称，默认 cc -E
CXX|C++编译器名称，默认 g++
RM|文件删除程序别名，默认 rm -f

自定义变量

定义： 变量名=变量值

使用： $(变量名)/${变量名}

# 伪目标
声明一个目标为伪目标后，makefile将不会判断目标是否存在或该目标是否需要更新。

```makefile
show:
    echo "hello world"
```

在这里我们可以直接使用 make show 来打印出 hello world，但是如果在该目录下我们又误创建了一个名为 show 的文件(touch show)，那这里我们再使用 make show，则会输出 "show is up to date"。把 show 定义成一个伪目标就是解决这个问题的。

声明伪目标的方法如下：

```
.PHONY:show
```

关于伪目标更详细的说明请参考：[https://www.wolai.com/ewNiDtX9LcUBnbHhgPEdGE](https://www.wolai.com/ewNiDtX9LcUBnbHhgPEdGE)

# 模式匹配
```
OBJ := file1.o file2.o file3.o
TARGET := target

$(TARGET):$(OBJ)
    gcc $^ -o $@

%.o:%.c
    gcc -c $^ -o $@
```

# makefile中使用shell命令
```
$(shell 命令)
```
如下：

```makefile
a := $(shell pwd)       # a 的值为当前绝对路径

FILE := abc.txt
b := $(shell if [ ! -f $(FILE) ];then \
                touch $(FILE); \
             fi;)                       # 创建abc.txt文件
```

# makefile的嵌套调用
```makefile
all:
    $(MAKE) -C dir/     # -C 即进入dir/目录下，并执行dir/目录下的makefile
    $(MAKE) -C dir/ -f xxx  #xxx是make的输入文件，使用 -f指定，不写-f参数，默认找Makefile文件

clean:
    $(MAKE) -C dir/ clean
```

```makefile
DIR := DIR1 DIR2 DIR3
.PHONY subdir $(DIR)
#方法一：
subdir:$(DIR)   # 依次(串行)进入DIR1,DIR2,DIR3三个目录并执行目录下的makefile文件         
    $(shell for dir in $(DIR); do \
                $(MAKE) -C $$dir; \
            done) # $$dir表示展开shell中的变量

#方法二：
subdir:$(DIR)
    $(MAKE) -C $@   #并发调用makefile
```

上面makefile中，方法二比方法一要高效得多，因为方法是并行执行，但并行执行可能存在一个问题，就是比如 DIR1 中的 makefile 的顺利执行需要依赖于 DIR3 中 make 后的结果，即我们要求 DIR3 要先执行 makefile。这样可以添加如下代码加以限制。

```makefile
#方法二改进
subdir:$(DIR)
    $(MAKE) -C $@
DIR1:DIR3   #说明DIR1依赖于DIR3，所以DIR3中的makefile会比DIR1中的makefile要先执行
```

# makefile的条件判断
条件判断与C语言中的宏一样

```
ifeq
ifneq
ifdef
ifndef
```

```makefile
a := 123
FLAG :=
ifeq ($(a), 123) #ifeq后面一定要有空格
    FLAG += -M
else             #没有 elif 功能
    FLAG += -Q
endif

all:
    echo $(FLAG)
```

# makefile的循环
makefile中的循环是用函数 foreach。

```
$(foreach <var>, <list>, <text>)
含义：把参数<list>中的单词逐一取出放到参数<var>所指定的变量中，
      然后再执行<text>所包含的表达式。每一次<text>会返回一个字符串，
      循环过程中，<text>的所返回的每个字符串会以空格分隔，最后循环结束时，
      <text>所返回的每个字符串所组成的整个字符串将会是foreach函数的返回值。
```

```makefile
names := a b c d
files := $(foreach n, $(names), $(n).o)  # files := a.o b.o c.o d.o
```

# makefile自定义函数
```makefile
#自定义函数本质上就是我们写的多行命令的入口
define FUNC1
    echo "hello world"
    echo $(shell ls)
endif

all:
    $(call FUNC1)
```

感觉这个函数没啥卵用。实际也很少用。仅了解一下吧。



