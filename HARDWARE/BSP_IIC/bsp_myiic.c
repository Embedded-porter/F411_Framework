#include "bsp_myiic.h"
#include "delay.h"

//IIC初始化
void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();   //使能GPIOB时钟
    
    //PB6,7初始化设置
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    IIC_SDA=1;
    IIC_SCL=1;  
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
unsigned char IIC_Wait_Ack(void)
{
	unsigned char ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(unsigned char txd)
{                        
	unsigned char t;   
	SDA_OUT(); 	    
	IIC_SCL=0;//拉低时钟开始数据传输
	for(t=0;t<8;t++)
	{              
		IIC_SDA=(txd&0x80)>>7;
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
	}	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
unsigned char IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
	for(i=0;i<8;i++ )
	{
		IIC_SCL=0; 
		delay_us(2);
		IIC_SCL=1;
		receive<<=1;
		if(READ_SDA)receive++;   
		delay_us(1); 
	}					 
	if (!ack)
		IIC_NAck();//发送nACK
	else
		IIC_Ack(); //发送ACK   
	return receive;
}

//读1个字节，ACK和NACK在外部单独处理
unsigned char IIC_Read_One_Byte(void)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
	for(i=0;i<8;i++ )
	{
		IIC_SCL=0; 
		delay_us(2);
		IIC_SCL=1;
		receive<<=1;
		if(READ_SDA)receive++;   
		delay_us(1); 
	}					  
	return receive;
}

/*
*函数名：i2c_CheckDevice
*功能说明：检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*形参：_Address
*返回值：返回值0表示正确，返回1表示未检测到设备
*/
uint8_t i2c_CheckDevice(uint32_t _Address )
{
    uint8_t ucAck;
  	IIC_Init();    //配置GPIO
    IIC_Start();       //发送启动信号
    /*发送设备地址+读写控制位*/
    IIC_Send_Byte(_Address);
    ucAck = IIC_Wait_Ack();    //检测设备的ACK应答
    IIC_Stop();    //发送停止信号
    return ucAck;
}