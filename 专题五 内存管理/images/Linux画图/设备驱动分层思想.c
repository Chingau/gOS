
核心层处理公共工作



输入设备驱动
input三层：

输入事件驱动程序(drivers/input/evdev.c)(提供file_operations)
————————————————————————————————————————————
输入核心层(驱动程序员主要在这层写程序)
————————————————————————————————————————————
输入设备驱动层(一般由芯片厂商提供)

输入核心层：
向输入设备驱动层(向下)提供
static input_dev *input_allocate_device(void);//分配input_dev
void input_free_device(struct input_dev *dev);//释放input_dev

int __must_check input_register_device(sttruct input_dev *);//注册input_dev设备
void input_unregister_device(struct input_dev *);//注销input_dev设备

向输入事件驱动层(向上)报告输入事件：
void input_event(,,,);//报告事件
void inpu_report_key(,,); //报告按键事件
...	//执行绝对坐标事件
void input_sync(struct input_dev *dev); //此函数会唤醒休眠的输入事件驱动程序来读取事件缓冲区中的事件

输入事件表示为 input_event 结构(缓冲区)
输入设备表示为 input_dev 结构

输入事件驱动程序通过file_operations实现的read来读取event缓冲区的事件，然后通过VFS上传给应用层

输入设备驱动层向核心层上报输入内容。
===================================================================================================

RTC设备驱动
RTC两层：
核心层(提供file_operations)
------------------------------------------
RTC设备驱动层(驱动程序员主要在这层写程序)

核心层：
RTC通用字符设备驱动(drivers/rtc/rtc-dev.c)
向RTC设备驱动层提供：
rtc_device_register()	//RTC设备注册
rtc_device_unregister()	//RTC设备注销
提供 rtc_class_ops 结构体，以描述底层RTC硬件操作

核心层提供的file_operations中的open,release等函数最终callback到由底层实现的rtc_class_ops中的函数。
===================================================================================================

Framebuffer设备驱动
核心层：(drivers/video/fbdev/core/fbmem.c)(提供file_operations)
向下提供帧缓冲设备fb_info的注册、注销及其相关成员维护
------------------------------------------
设备驱动层：(驱动程序员主要在这层写程序)
最终实现针对硬件的操作就callback到fb_info结构中的fb_ops相关函数。(xxxfb.c文件，如/drivers/video/fbdev/mxsfb.c)

核心层fbmem.c中实现file_operations中的相关函数操作，例如fb_write函数中，
fb_write会检查fb_info->fb_ops->write是否实现，如果实现则调用已实现的fb_info->fb_ops->write函数，
否则就执行fb_write通用操作规范。相关代码如下：
static ssize_t fb_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	struct fb_info *info = file_fb_info(file);
	...
	if (info->fbops->fb_write)
		return info->fbops->fb_write(info, buf, count, ppos);//对于特殊的LCD显存，驱动提供重写的机会
	...
	//通用LCD显存的操作方法
	...
}
===================================================================================================

终端设备驱动
tty核心层(tty_io.c)
tty线路规程(n_tty.c)
tty驱动(xxx_tty.c)

tty核心层(tty_io.c)
tty核心层是一个标准的字符设备驱动，对上提供了file_operations
对下定义了tty_driver的架构。

tty驱动(针对特定的tty设备)
因为tty核心层有提供tty_driver结构，所以tty驱动层的主要工作就是填充tty_driver及
实现tty_operations的成员函数。
（可以看出，file_operations中的write()会间接调用tty_operations中的write()成员函数）

tty设备发送数据的流程：
	tty核心从一个用户获取将要发送给一个tty设备的数据，tty核心将数据传递给tty线路规程驱动，接着数据被传递到tty驱动层，
	tty驱动将数据转换为可以发送给硬件的格式。
tty接收数据的流程：
	从tty硬件接收到的数据向上交给tty驱动层，接着进入tty线路规程驱动，再进入核心层，在这里它被一个用户获取。
===================================================================================================	
	
	
	
	













