#include "W25QXX.h"

/* Private typedef -----------------------------------------------------------*/
//#define W25QXX_PageSize      4096
#define W25QXX_PageSize      				256
#define W25QXX_PerWritePageSize      256

/* W25Q64：一共是8M字节=8*1024*1024=8388608(Byte)，
分为128块(64K)，每一块有分为16个扇区(4K)，
所以扇区的个数是：128*16=2048(个) 扇区地址计算方法为 扇区位数*4096
假如要擦除第1000个的扇区，那么这个扇区的字节起始就是1000*4096=4096000，计算地址的时候是使用字节来计算的。
每扇区拥有16页，每页256个字节
16（页）*256（字节）*16（扇区）*128（块）=8M
*/

/* Private define ------------------------------------------------------------*/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 

#define WIP_Flag                  0x01  /* 正在写入（WIP）标志 */

u32 SPICSPin;
u8 SPICSGPIOx;
void SendCS_HIGH()                     //置高
{
	switch(SPICSGPIOx)
	{
		case GPIOa:GPIO_SetBits(GPIOA,SPICSPin);	break;
		case GPIOb:GPIO_SetBits(GPIOB,SPICSPin);	break;
		case GPIOc:GPIO_SetBits(GPIOC,SPICSPin);	break;
		case GPIOd:GPIO_SetBits(GPIOD,SPICSPin);	break;
		case GPIOe:GPIO_SetBits(GPIOE,SPICSPin);	break;
		case GPIOf:GPIO_SetBits(GPIOF,SPICSPin);	break;
		case GPIOg:GPIO_SetBits(GPIOG,SPICSPin);	break;
	}
}
void SendCS_LOW()										  //置低
{
	switch(SPICSGPIOx)
	{
		case GPIOa:GPIO_ResetBits(GPIOA,SPICSPin);	break;
		case GPIOb:GPIO_ResetBits(GPIOB,SPICSPin);	break;
		case GPIOc:GPIO_ResetBits(GPIOC,SPICSPin);	break;
		case GPIOd:GPIO_ResetBits(GPIOD,SPICSPin);	break;
		case GPIOe:GPIO_ResetBits(GPIOE,SPICSPin);	break;
		case GPIOf:GPIO_ResetBits(GPIOF,SPICSPin);	break;
		case GPIOg:GPIO_ResetBits(GPIOG,SPICSPin);	break;
	}
}
	
/*******************************************************************************
* Function Name  : W25QXX_Init
* Description    : 初始化SPI FLASH的IO口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u32 W25QXX_Init(u8 CSGPIOx,u32 CS_PIN)
{
	u32 DeviceID;
	SPICSGPIOx=CSGPIOx;
	SPICSPin=CS_PIN;

	pinMode(SPICSGPIOx,CS_PIN,OUTPUT,Speed_50MHz);
  SendCS_HIGH();
	
	SPI1_Init();
	
	DeviceID=W25QXX_ReadDeviceID();
	return DeviceID;
}
/*******************************************************************************
* Function Name  : W25QXX_SectorErase
* Description    : 擦除指定的FLASH扇区。
* Input          : SectorAddr：擦除扇区的地址。
* Output         : None
* Return         : None
*******************************************************************************/
void W25QXX_SectorErase(u32 SectorAddr)
{
  /* Send write enable instruction */
  W25QXX_WriteEnable();
  W25QXX_WaitForWriteEnd();
  /* Sector Erase */
  /* Select the FLASH: Chip Select low */
  SendCS_LOW();
  /* Send Sector Erase instruction */
  SPI1_ReadWriteByte(W25X_SectorErase);
  /* Send SectorAddr high nibble address byte  24bit address */ 
  SPI1_ReadWriteByte((SectorAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  SPI1_ReadWriteByte((SectorAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  SPI1_ReadWriteByte(SectorAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  SendCS_HIGH();
  /* Wait the end of Flash writing */
  W25QXX_WaitForWriteEnd();
}

/*******************************************************************************
* Function Name  : W25QXX_BulkErase
* Description    : 擦除整个芯片.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void W25QXX_BulkErase(void)
{
  /* Send write enable instruction */
  W25QXX_WriteEnable();

  /* Bulk Erase */
  /* Select the FLASH: Chip Select low */
  SendCS_LOW();
  /* Send Bulk Erase instruction  */
  SPI1_ReadWriteByte(W25X_ChipErase);
  /* Deselect the FLASH: Chip Select high */
  SendCS_HIGH();

  /* Wait the end of Flash writing */
  W25QXX_WaitForWriteEnd();
}

/*******************************************************************************
* Function Name  : W25QXX_PageWrite
* Description    : 使用单个写入将多个字节写入FLASH
*									 循环（写入页面序列）
* 								 字节数不能超过FLASH页面大小
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH,
*                    must be equal or less than "W25QXX_PageSize" value.
* Output         : None
* Return         : None
*******************************************************************************/
void W25QXX_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  /* Enable the write access to the FLASH */
  W25QXX_WriteEnable();

  /* Select the FLASH: Chip Select low */
  SendCS_LOW();
  /* Send "Write to Memory " instruction */
  SPI1_ReadWriteByte(W25X_PageProgram);
  /* Send WriteAddr high nibble address byte to write to */
  SPI1_ReadWriteByte((WriteAddr & 0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */
  SPI1_ReadWriteByte((WriteAddr & 0xFF00) >> 8);
  /* Send WriteAddr low nibble address byte to write to */
  SPI1_ReadWriteByte(WriteAddr & 0xFF);

  if(NumByteToWrite > W25QXX_PerWritePageSize)
  {
     NumByteToWrite = W25QXX_PerWritePageSize;
     //printf("\n\r Err: SPI_FLASH_PageWrite too large!");
  }

  /* while there is data to be written on the FLASH */
  while (NumByteToWrite--)
  {
    /* Send the current byte */
    SPI1_ReadWriteByte(*pBuffer);
    /* Point on the next byte to be written */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  SendCS_HIGH();

  /* Wait the end of Flash writing */
  W25QXX_WaitForWriteEnd();
}

/*******************************************************************************
* Function Name  : W25QXX_BufferWrite
* Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.
* Input          : - pBuffer :数据存储区
*                  - WriteAddr : 开始写入的地址(24bit)
*                  - NumByteToWrite : 要写入的字节数(最大65535)
* Output         : None
* Return         : None
*******************************************************************************/
void W25QXX_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % W25QXX_PageSize;
  count = W25QXX_PageSize - Addr;
  NumOfPage =  NumByteToWrite / W25QXX_PageSize;
  NumOfSingle = NumByteToWrite % W25QXX_PageSize;

  if (Addr == 0) /* WriteAddr is W25QXX_PageSize aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < W25QXX_PageSize */
    {
      W25QXX_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > W25QXX_PageSize */
    {
      while (NumOfPage--)
      {
        W25QXX_PageWrite(pBuffer, WriteAddr, W25QXX_PageSize);
        WriteAddr +=  W25QXX_PageSize;
        pBuffer += W25QXX_PageSize;
      }

      W25QXX_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /* WriteAddr is not W25QXX_PageSize aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < W25QXX_PageSize */
    {
      if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > W25QXX_PageSize */
      {
        temp = NumOfSingle - count;

        W25QXX_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        W25QXX_PageWrite(pBuffer, WriteAddr, temp);
      }
      else
      {
        W25QXX_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > W25QXX_PageSize */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / W25QXX_PageSize;
      NumOfSingle = NumByteToWrite % W25QXX_PageSize;

      W25QXX_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        W25QXX_PageWrite(pBuffer, WriteAddr, W25QXX_PageSize);
        WriteAddr +=  W25QXX_PageSize;
        pBuffer += W25QXX_PageSize;
      }

      if (NumOfSingle != 0)
      {
        W25QXX_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}
//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		W25QXX_PageWrite(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 
//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)						
//NumByteToWrite:要写入的字节数(最大65535)   
u8 W25QXX_BUFFER[4096];		 
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
   	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		W25QXX_BufferRead(W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			W25QXX_SectorErase(secpos);		//擦除这个扇区
			for(i=0;i<secremain;i++)	   		//复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  				//指针偏移
			WriteAddr+=secremain;				//写地址偏移	   
		   	NumByteToWrite-=secremain;			//字节数递减
			if(NumByteToWrite>4096)secremain=4096;//下一个扇区还是写不完
			else secremain=NumByteToWrite;		//下一个扇区可以写完了
		}	 
	};	 
}

//void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
//{
//  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, i;
//	u32 CurrentAddr;

//	
//	CurrentAddr = WriteAddr;
//	
//  Addr = WriteAddr % W25QXX_PageSize;
//  count = W25QXX_PageSize - Addr;
//	
//	NumOfPage = ( NumByteToWrite - count ) / W25QXX_PageSize;
//	NumOfSingle = ( NumByteToWrite - count ) % W25QXX_PageSize;
//	

//	if ( count )
//	{
//		SPI_FLASH_PageWrite ( pBuffer, CurrentAddr, count );
//		
//		CurrentAddr += count;
//		pBuffer += count;
//		
//	}
//	
//	for ( i = 0; i < NumOfPage; i ++ )
//	{
//		SPI_FLASH_PageWrite ( pBuffer, CurrentAddr, W25QXX_PageSize );
//		
//		CurrentAddr += W25QXX_PageSize;
//		pBuffer += W25QXX_PageSize;
//		
//	}
//	
//	if ( NumOfSingle )
//		SPI_FLASH_PageWrite ( pBuffer, CurrentAddr, NumOfSingle );

//}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferRead
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void W25QXX_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	u16 i; 
  /* Select the FLASH: Chip Select low */
  SendCS_LOW();
    SPI1_ReadWriteByte(W25X_ReadData);         	//发送读取命令   
    SPI1_ReadWriteByte((u8)((ReadAddr)>>16));  	//发送24bit地址    
    SPI1_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPI1_ReadWriteByte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI1_ReadWriteByte(0XFF);   	//循环读数  
    }
  /* Deselect the FLASH: Chip Select high */
  SendCS_HIGH();
}

/*******************************************************************************
* Function Name  : W25QXX_ReadID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
u32 W25QXX_ReadID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* Select the FLASH: Chip Select low */
  SendCS_LOW();

  /* Send "RDID " instruction */
  SPI1_ReadWriteByte(W25X_JedecDeviceID);

  /* Read a byte from the FLASH */
  Temp0 = SPI1_ReadWriteByte(0xFF);

  /* Read a byte from the FLASH */
  Temp1 = SPI1_ReadWriteByte(0xFF);

  /* Read a byte from the FLASH */
  Temp2 = SPI1_ReadWriteByte(0xFF);

  /* Deselect the FLASH: Chip Select high */
  SendCS_HIGH();

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}
/*******************************************************************************
* Function Name  : W25QXX_ReadDeviceID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
u32 W25QXX_ReadDeviceID(void)
{
  u32 Temp = 0;

  /* Select the FLASH: Chip Select low */
  SendCS_LOW();

  /* Send "RDID " instruction */
  SPI1_ReadWriteByte(W25X_DeviceID);
  SPI1_ReadWriteByte(0xFF);
  SPI1_ReadWriteByte(0xFF);
  SPI1_ReadWriteByte(0xFF);
  
  /* Read a byte from the FLASH */
  Temp = SPI1_ReadWriteByte(0xFF);

  /* Deselect the FLASH: Chip Select high */
  SendCS_HIGH();

  return Temp;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/

void SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  SendCS_LOW();

  /* Send "Read from Memory " instruction */
  SPI1_ReadWriteByte(W25X_ReadData);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI1_ReadWriteByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI1_ReadWriteByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI1_ReadWriteByte(ReadAddr & 0xFF);
}


/*******************************************************************************
* Function Name  : SPI_FLASH_SendHalfWord
* Description    : Sends a Half Word through the SPI interface and return the
*                  Half Word received from the SPI bus.
* Input          : Half Word : Half Word to send.
* Output         : None
* Return         : The value of the received Half Word.
*******************************************************************************/
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI1 , SPI_I2S_FLAG_TXE) == RESET);

  /* Send Half Word through the SPI1 peripheral */
  SPI_I2S_SendData(SPI1 , HalfWord);

  /* Wait to receive a Half Word */
  while (SPI_I2S_GetFlagStatus(SPI1 , SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the Half Word read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1 );
}

/*******************************************************************************
* Function Name  : W25QXX_WriteEnable
* Description    : 启用对FLASH的写入访问。
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void W25QXX_WriteEnable(void)
{
  //使能器件
  SendCS_LOW();

  /* Send "Write Enable" instruction */
  SPI1_ReadWriteByte(W25X_WriteEnable);

  /* Deselect the FLASH: Chip Select high */
  SendCS_HIGH();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WaitForWriteEnd
* Description    : 查询写入标志位WIP状态并等待直到写入完成
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void W25QXX_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;

  /* Select the FLASH: Chip Select low */
  SendCS_LOW();

  /* Send "Read Status Register" instruction */
  SPI1_ReadWriteByte(W25X_ReadStatusReg);

  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = SPI1_ReadWriteByte(0xFF);	 
  }
  while ((FLASH_Status & WIP_Flag) == SET); /* Write in progress */

  /* Deselect the FLASH: Chip Select high */
  SendCS_HIGH();
}


//进入掉电模式
void W25QXX_PowerDown(void)   
{ 
  /* Select the FLASH: Chip Select low */
  SendCS_LOW();

  /* Send "Power Down" instruction */
  SPI1_ReadWriteByte(W25X_PowerDown);

  /* Deselect the FLASH: Chip Select high */
  SendCS_HIGH();
}   

//唤醒
void W25QXX_WAKEUP(void)   
{
  /* Select the FLASH: Chip Select low */
  SendCS_LOW();
  /* Send "Power Down" instruction */
  SPI1_ReadWriteByte(W25X_ReleasePowerDown);
  /* Deselect the FLASH: Chip Select high */
  SendCS_HIGH();
}   
   
/*********************************************END OF FILE**********************/
