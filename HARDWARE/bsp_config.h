#ifndef _BSP_CONFIG_H
#define _BSP_CONFIG_H

/**************************硬件原理图**********************************
SPI通信:  SPI_CS:PA4
          SPI_SCK:PA5
          SPI_MISO:PA6
          SPI_MOSI:PA7
          性能要求:DMA+SPI 主机模式 通信速度:10M
AT24CXX:  SCL:PB8
          SDA:PB9
          芯片型号:AT24C32 存储空间:4096字节

10路ADC:  PD1:PA0
          PD2:PA1
          PD3:PA2
          PD4:PA3
          PD5:PB0
          PD6:PB1
          PD7:PC0
          PD8:PC1
          PD9:PC2
          PD10:PC3

SP LOS1: 分路器进光功率检测引脚
SP LOS2: 分路器进光功率检测引脚   

CSX:VOA的片选引脚
I2C3 SCL:VOA驱动
I2C3 SDA:VOA驱动



**********************************************************************/
#include <stdio.h>

//#define DBG_PRINTF(...)
#define DBG_PRINTF printf

#endif // !_BSP_CONFIG_H