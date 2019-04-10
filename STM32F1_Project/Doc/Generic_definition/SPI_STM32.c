#include "SPI_STM32.h"

//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
//PA5-SCK PA6-MISO PA7-MISI
void SPI1_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_SPI1|RCC_APB2Periph_GPIOA, ENABLE );
	
  /*!< Configure SPI_FLASH_SPI_CS_PIN pin: SPI_FLASH Card CS pin */
 // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
 //	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 // GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

  /* SPI1 configuration */
  // W25X16: data input on the DIO pin is sampled on the rising edge of the CLK. 
  // Data on the DO and DIO pins are clocked out on the falling edge of CLK.
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
  SPI_Init(SPI1 , &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(SPI1 , ENABLE);
	
	SPI1_ReadWriteByte(0XFF);
}

//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPI1->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); //使能SPI1
} 

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节

u8 SPI1_ReadWriteByte(u8 byte)
{
  //检查指定的SPI标志位设置与否:发送缓存空标志位
  while (SPI_I2S_GetFlagStatus(SPI1 , SPI_I2S_FLAG_TXE) == RESET);

  //通过外设SPIx发送一个数据
  SPI_I2S_SendData(SPI1,byte);

  //检查指定的SPI标志位设置与否:接受缓存非空标志位
  while (SPI_I2S_GetFlagStatus(SPI1 , SPI_I2S_FLAG_RXNE) == RESET);

  //返回通过SPIx最近接收的数据	
  return SPI_I2S_ReceiveData(SPI1);
}

u8 SPI1_ReadByte(void)
{
  return (SPI1_ReadWriteByte(0XFF));
}
