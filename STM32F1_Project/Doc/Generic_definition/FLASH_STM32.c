#include "FLASH_STM32.h"
#define STM32_FLASH_SIZE 64 	 					//所选STM32的FLASH容量大小(单位为K)
#define     FLASH_WAIT_TIMEOUT      100000


//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址

#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else 
#define STM_SECTOR_SIZE	2048
#endif

 
//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)个数   
void STMFLASH_Write_HalfWord(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
				if(FLASH_WaitForLastOperation(FLASH_WAIT_TIMEOUT)!=FLASH_TIMEOUT)
			{
				FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
			}
				FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
				WriteAddr+=2;//地址增加2.
	}  
} 
//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
void STMFLASH_Change(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//计算能够储存多少个半字。
	u32 PageAdd;	   //页地址
	u16 OffSetPage;	   //页偏移地址(16位字计算)
	u16 PageOverage; //页内剩余地址(16位字计算)
 	u16 i;    
	u32 OffSetaddr;   //实际偏移地址
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))
		return;//非法地址
	FLASH_Unlock();//解锁
	
	OffSetaddr=WriteAddr-STM32_FLASH_BASE;		//实际偏移地址.
	
	PageAdd=OffSetaddr/STM_SECTOR_SIZE;			//页地址(页位数)
	
	OffSetPage=(OffSetaddr%STM_SECTOR_SIZE)/2;		//在页内的偏移(2个字节为基本单位.)
	
	PageOverage=STM_SECTOR_SIZE/2-OffSetPage;		//页剩余空间大小
	
	if(NumToWrite<=PageOverage)PageOverage=NumToWrite;//不大于该扇区范围
	
	while(1) 
	{	
		STMFLASH_Read(PageAdd*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//读出整个页的内容
		for(i=0;i<PageOverage;i++)//校验数据
		{
			if(STMFLASH_BUF[OffSetPage+i]!=0XFFFF)
				break;//需要擦除  	  
		}
		if(i<PageOverage)//需要擦除
		{
			FLASH_ErasePage(PageAdd*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个页
			for(i=0;i<PageOverage;i++)//复制
			{
				STMFLASH_BUF[i+OffSetPage]=pBuffer[i];	  
			}
			STMFLASH_Write_HalfWord(PageAdd*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//写入整个扇区  
		}
		else 
			STMFLASH_Write_HalfWord(WriteAddr,pBuffer,PageOverage);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumToWrite==PageOverage)
			break;//写入结束了
		else//写入未结束
		{
			PageAdd++;				//页地址增1
			OffSetPage=0;				//偏移位置为0 	 
		  pBuffer+=PageOverage;  	//指针偏移
			WriteAddr+=PageOverage;	//写地址偏移	   
		  NumToWrite-=PageOverage;	//字节(16位)数递减
			if(NumToWrite>(STM_SECTOR_SIZE/2))PageOverage=STM_SECTOR_SIZE/2;//下一个扇区还是写不完
			
			else 
				PageOverage=NumToWrite;//下一个扇区可以写完了
		} 
	}
	FLASH_Lock();//上锁
}
