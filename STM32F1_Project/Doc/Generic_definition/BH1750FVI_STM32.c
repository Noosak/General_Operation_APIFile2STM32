#include "BH1750FVI_STM32.h"

void BH1750FVI_Init(u8 GPIOx,u32 SCLGPIOPINx,u32 SDAGPIOPINx)
{
	SendLine_IIC(GPIOx,SCLGPIOPINx,SDAGPIOPINx);
}
void BH1750_Mode(u8 Dev,u8 REG_Address)
{
		SendSDAmode(1);	
    IIC_Start();                  						//��ʼ�ź�
		IIC_Send_Byte(Dev);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(REG_Address);   //���͵�ַ
		IIC_Wait_Ack();	
		IIC_Stop();
}

u8 BH1750_Read(u8 Dev,u8 length,u8 *data)
{
  u8 count = 0;
	IIC_Start();
	IIC_Send_Byte(Dev+1);  //�������ģʽ	
	IIC_Wait_Ack();
	
    for(count=0;count<length;count++){
		 
		 if(count!=length-1)data[count]=IIC_Read_Byte(1);  //��ACK�Ķ�����
		 	else  data[count]=IIC_Read_Byte(0);	 //���һ���ֽ�NACK
	}
    IIC_Stop();//����һ��ֹͣ����
    return count;
}

u16 BH1750GetData(u8 Dev,u8 GPIOx,u32 SCLGPIOPINx,u32 SDAGPIOPINx)
{
	u8 BH1750DataBuff[2];
	u16 BH1750_Data;
	BH1750FVI_Init(GPIOx,SCLGPIOPINx,SDAGPIOPINx);
	BH1750_Mode(Dev,BH1750FVI_H_Resolution);
	BH1750_Read(Dev,2,BH1750DataBuff);
	BH1750_Data=((BH1750DataBuff[0]<<8)|BH1750DataBuff[1])/1.2;
	return BH1750_Data;
}
