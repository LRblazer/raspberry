这个项目主要将树莓派上获取的温度和台灯的状态通过ESP8266上传腾讯云，并且显示在OLED的屏幕上,并且同步到sqlite3数据库中

腾讯云也可以下发命令控制灯的亮灭

主要是通过串口编程与AT指令控制ESP8266

第一个OLED文件为OLED的驱动程序

qcloud存放着腾讯云官方的ESP8266固件

my_comport用于串口编程的学习

project为整个系统项目

效果图在下面网站：
https://gitee.com/xuxinhua666/raspberry/blob/master/ESP8266/README.md
























