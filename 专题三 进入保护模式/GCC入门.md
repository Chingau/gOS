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

 