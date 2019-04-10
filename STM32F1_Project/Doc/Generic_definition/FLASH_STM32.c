#include "FLASH_STM32.h"
#define STM32_FLASH_SIZE 64 	 					//��ѡSTM32��FLASH������С(��λΪK)
#define     FLASH_WAIT_TIMEOUT      100000


//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ

#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif

 
//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)����   
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
				WriteAddr+=2;//��ַ����2.
	}  
} 
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
void STMFLASH_Change(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����ܹ�������ٸ����֡�
	u32 PageAdd;	   //ҳ��ַ
	u16 OffSetPage;	   //ҳƫ�Ƶ�ַ(16λ�ּ���)
	u16 PageOverage; //ҳ��ʣ���ַ(16λ�ּ���)
 	u16 i;    
	u32 OffSetaddr;   //ʵ��ƫ�Ƶ�ַ
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))
		return;//�Ƿ���ַ
	FLASH_Unlock();//����
	
	OffSetaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	
	PageAdd=OffSetaddr/STM_SECTOR_SIZE;			//ҳ��ַ(ҳλ��)
	
	OffSetPage=(OffSetaddr%STM_SECTOR_SIZE)/2;		//��ҳ�ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	
	PageOverage=STM_SECTOR_SIZE/2-OffSetPage;		//ҳʣ��ռ��С
	
	if(NumToWrite<=PageOverage)PageOverage=NumToWrite;//�����ڸ�������Χ
	
	while(1) 
	{	
		STMFLASH_Read(PageAdd*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//��������ҳ������
		for(i=0;i<PageOverage;i++)//У������
		{
			if(STMFLASH_BUF[OffSetPage+i]!=0XFFFF)
				break;//��Ҫ����  	  
		}
		if(i<PageOverage)//��Ҫ����
		{
			FLASH_ErasePage(PageAdd*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�������ҳ
			for(i=0;i<PageOverage;i++)//����
			{
				STMFLASH_BUF[i+OffSetPage]=pBuffer[i];	  
			}
			STMFLASH_Write_HalfWord(PageAdd*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}
		else 
			STMFLASH_Write_HalfWord(WriteAddr,pBuffer,PageOverage);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==PageOverage)
			break;//д�������
		else//д��δ����
		{
			PageAdd++;				//ҳ��ַ��1
			OffSetPage=0;				//ƫ��λ��Ϊ0 	 
		  pBuffer+=PageOverage;  	//ָ��ƫ��
			WriteAddr+=PageOverage;	//д��ַƫ��	   
		  NumToWrite-=PageOverage;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))PageOverage=STM_SECTOR_SIZE/2;//��һ����������д����
			
			else 
				PageOverage=NumToWrite;//��һ����������д����
		} 
	}
	FLASH_Lock();//����
}
