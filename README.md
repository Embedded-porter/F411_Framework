#  STM32F411固件库@早睡早起

> 本人做单片机开发的，平常使用最多的平台是F4，本项目的主要的目的是想构建自己的一套开发固件库，方便后续的项目开发

## 文件目录

1. CODE：依赖
2. HALLIB：HAL库
3. **HARDWARE：固件库**
4. OBJ：编译生成的文件
5. SYSTEM：系统自带库
6. USER：MDK软件

> **主要的内容存放在HARDWARE固件库中！！！！！**！

###  2023-12-13

> 目前固件库包含一下驱动

1. GPIO控制模块
2. 蜂鸣器模块
3. 按键模块
4. SPI通信协议（包括轮询模式+中断模式+DMA模式）
5. 模拟I2C通信协议
6. AT24CXX模块

