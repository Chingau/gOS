
参考：


[http://www.bytekits.com/nasm/intro.html](http://www.bytekits.com/nasm/intro.html)

# NASM 常量
## EQU 指令
EQU指令用于定义常量。 EQU伪指令的语法如下：`CONSTANT_NAME EQU expression`

例如：

```
    TOTAL_STUDENTS equ 50
    mov  ecx,  TOTAL_STUDENTS 
```

# NASM $,$$和vstart,org
## vstart, org
vstart 和 org 它们的功能是告诉编译器是把其后面的数据相对于这个指定的地址编码。所以一般在加载的时候，我们会把这个这些代码加载到其指定的地址，当然，不加载到这里也没有问题，只是在其内部取根据虚拟地址取相关的地址时可能会出错。

对于一段程序来说，如果不指定 vstart 或 org ,默认是以 0 地址进行编码的。

代码段会首先存放到 org 指定的地址处，数据段定义的数据会依次往后排放。如下：

```
[ORG 0x7c00]
[SECTION .data]
read_floppy_error:
    db "read floppy error!", 10, 13, 0      ;read floppy error!\r\n0

[SECTION .text]
[BITS 16]
global _start
_start:
    ; 设置屏幕模式为文本模式，清除屏幕
    mov ax, 3
    int 0x10

;clear zero
times 510 - ($ - $$) db 0
dw 0xaa55
```

上述代码中，mov ax,3 这句代码会被编址到 0x7c00 处，然后整个代码段共占用512字节；所以数据段中定义的 read_floppy_error 这个数据会被放到 512 字节后。

## BITS
bits 伪指令，使用方式：[BITS 16], [BITS 32]，用于告诉编译器，我下面的指令都要编译成 xx 位的机器码。比如我们运行在实模式下，就需要 [BITS 16]；

bits 指令的范围是从当前的 bits 标签直到下一个 bits 标签的范围。bits 外面的方括号是可以省略的。另外在没有指定 bits 时默认是 bits 16。