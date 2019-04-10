#include "IIC_STM32.h"

u32 IICSDA,IICSCL,IICGPIOx;
u8 ACK;
u8 const IN=0;
u8 const OUT=1;

void SendLine_IIC(u8 GPIOx,u32 NSCL,u32 NSDA)
{
  IICSDA=NSDA;
	IICSCL=NSCL;
	IICGPIOx=GPIOx;
	pinMode(IICGPIOx,IICSDA,OUTPUT,Speed_50MHz);
	pinMode(IICGPIOx,IICSCL,OUTPUT,Speed_50MHz);
	delay_init(72);
}

void SendSDAmode(u8 Mode)
{
	if(Mode==0)
	{
		pinMode(IICGPIOx,IICSDA,INPUT,Speed_50MHz);
	}
	if(Mode==1)
	{
		pinMode(IICGPIOx,IICSDA,OUTPUT,Speed_50MHz);
	}
}

void SendLine_HIGH(u32 line)                     //�ø�
{
	switch(IICGPIOx)
	{
		case GPIOa:GPIO_SetBits(GPIOA,line);	break;
		case GPIOb:GPIO_SetBits(GPIOB,line);	break;
		case GPIOc:GPIO_SetBits(GPIOC,line);	break;
		case GPIOd:GPIO_SetBits(GPIOD,line);	break;
		case GPIOe:GPIO_SetBits(GPIOE,line);	break;
		case GPIOf:GPIO_SetBits(GPIOF,line);	break;
		case GPIOg:GPIO_SetBits(GPIOG,line);	break;
	}
}

void SendLine_LOW(u32 line)										  //�õ�
{
	switch(IICGPIOx)
	{
		case GPIOa:GPIO_ResetBits(GPIOA,line);	break;
		case GPIOb:GPIO_ResetBits(GPIOB,line);	break;
		case GPIOc:GPIO_ResetBits(GPIOC,line);	break;
		case GPIOd:GPIO_ResetBits(GPIOD,line);	break;
		case GPIOe:GPIO_ResetBits(GPIOE,line);	break;
		case GPIOf:GPIO_ResetBits(GPIOF,line);	break;
		case GPIOg:GPIO_ResetBits(GPIOG,line);	break;
	}
}

u8 ReadSDA(void)														  //��������
{
	u8 Line_DA;
	switch(IICGPIOx)
	{
		case GPIOa:Line_DA=GPIO_ReadInputDataBit(GPIOA,IICSDA);	break;
		case GPIOb:Line_DA=GPIO_ReadInputDataBit(GPIOB,IICSDA);	break;
		case GPIOc:Line_DA=GPIO_ReadInputDataBit(GPIOC,IICSDA);	break;
		case GPIOd:Line_DA=GPIO_ReadInputDataBit(GPIOD,IICSDA);	break;
		case GPIOe:Line_DA=GPIO_ReadInputDataBit(GPIOE,IICSDA);	break;
		case GPIOf:Line_DA=GPIO_ReadInputDataBit(GPIOF,IICSDA);	break;
		case GPIOg:Line_DA=GPIO_ReadInputDataBit(GPIOG,IICSDA);	break;
	}
	return Line_DA;
}

u8 IIC_Start(void)
{
	SendLine_HIGH(IICSDA);
	if(!ReadSDA()) return 0;
	delay_us(1);
	SendLine_HIGH(IICSCL);
	delay_us(5);
	SendLine_LOW(IICSDA);
	if(ReadSDA())return 0;
	delay_us(5);
	SendLine_LOW(IICSCL);
	delay_us(2);
	return 1;
}

void IIC_Stop(void)
{
	SendLine_LOW(IICSCL);
	SendLine_LOW(IICSDA);
	delay_us(1);
	SendLine_HIGH(IICSCL);
	delay_us(5);
	SendLine_HIGH(IICSDA);
	delay_us(4);
}

int IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SendSDAmode(IN);      //SDA����Ϊ����  
	SendLine_HIGH(IICSDA);
	delay_us(2);	   
	SendLine_HIGH(IICSCL);
	delay_us(2);	 
	while(ReadSDA())
	{
		ucErrTime++;
		if(ucErrTime>50)
		{
			IIC_Stop();
			return 0;
		}
	  delay_us(5);
	}
	SendLine_LOW(IICSCL);//ʱ�����0 
	SendSDAmode(OUT);	
	return 1;
} 

void IIC_Ack(void)
{
	SendLine_LOW(IICSCL);
	delay_us(5);
	SendLine_LOW(IICSDA);
	delay_us(3);
	SendLine_HIGH(IICSCL);
	delay_us(5);
	SendLine_LOW(IICSCL);
	delay_us(2);
}

void IIC_NAck(void)
{
	SendLine_LOW(IICSCL);
	delay_us(5);
	SendLine_HIGH(IICSDA);
	delay_us(3);
	SendLine_HIGH(IICSCL);
	delay_us(5);
	SendLine_LOW(IICSCL);
	delay_us(2);
}

void IIC_Send_Byte(u8 TXData)
{                        
    u8 BitCon;       
    SendLine_LOW(IICSCL);//����ʱ�ӿ�ʼ���ݴ���
    for(BitCon=0;BitCon<8;BitCon++)
    {         
			if(TXData&0x80)
				SendLine_HIGH(IICSDA);
			else
				SendLine_LOW(IICSDA);
      TXData<<=1; 	  
			SendLine_HIGH(IICSCL);
			delay_us(5); 
			SendLine_LOW(IICSCL);
			delay_us(2);
    }	 
} 	 

u8 i2cWrite(u8 addr, u8 reg, u8 len, u8 *data)
{
		int DataBit;
    if (!IIC_Start())
        return 1;
    IIC_Send_Byte(addr << 1 );
    if (!IIC_Wait_Ack()) {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();
		for (DataBit = 0; DataBit < len; DataBit++) {
        IIC_Send_Byte(data[DataBit]);
        if (!IIC_Wait_Ack()) {
            IIC_Stop();
            return 0;
        }
    }
    IIC_Stop();
    return 0;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IIC_Read_Byte(unsigned char ack)
*��������:	    //��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK 
*******************************************************************************/  
u8 IIC_Read_Byte(u8 ack)
{
	u8 i,receive=0;
	SendSDAmode(IN); //SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
		delay_us(1);
		SendLine_LOW(IICSCL);
		delay_us(5);
		SendLine_HIGH(IICSCL);
		delay_us(3);
		receive<<=1;
		if(ReadSDA())receive++;   
		delay_us(2); 
    }
	  SendLine_LOW(IICSCL);
		SendSDAmode(OUT);	
    if (ack)
        IIC_Ack(); //����ACK 
    else
        IIC_NAck();//����nACK 
    return receive;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
*��������:		
*******************************************************************************/
u8 i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    if (!IIC_Start())
        return 1;
    IIC_Send_Byte(addr << 1);
    if (!IIC_Wait_Ack()) {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte((addr << 1)+1);
    IIC_Wait_Ack();
    while (len) {
        if (len == 1)
            *buf = IIC_Read_Byte(0);
        else
            *buf = IIC_Read_Byte(1);
        buf++;
        len--;
    }
    IIC_Stop();
    return 0;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
*��������:	    ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
����	I2C_Addr  Ŀ���豸��ַ
		addr	   �Ĵ�����ַ
����   ��������ֵ
*******************************************************************************/ 
u8 I2C_ReadOneByte(u8 I2C_Addr,u8 addr)
{
	u8 res=0;
	
	IIC_Start();	
	IIC_Send_Byte(I2C_Addr);	   //����д����
	res++;
	IIC_Wait_Ack();
	IIC_Send_Byte(addr); res++;  //���͵�ַ
	IIC_Wait_Ack();	  
	IIC_Stop();//����һ��ֹͣ����	
	IIC_Start();
	IIC_Send_Byte(I2C_Addr+1); res++;          //�������ģʽ			   
	IIC_Wait_Ack();
	res=IIC_Read_Byte(0);	   
  IIC_Stop();//����һ��ֹͣ����

	return res;
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
*��������:	    ��ȡָ���豸 ָ���Ĵ����� length��ֵ
����	dev  Ŀ���豸��ַ
		reg	  �Ĵ�����ַ
		length Ҫ�����ֽ���
		*data  ���������ݽ�Ҫ��ŵ�ָ��
����   ���������ֽ�����
*******************************************************************************/ 
u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
{
  u8 count = 0;
	IIC_Start();
	IIC_Send_Byte(dev);	   //����д����
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   //���͵�ַ
  IIC_Wait_Ack();	
	IIC_Stop();
	IIC_Start();
	IIC_Send_Byte(dev+1);  //�������ģʽ	
	IIC_Wait_Ack();
	
    for(count=0;count<length;count++){
		 
		 if(count!=length-1)data[count]=IIC_Read_Byte(1);  //��ACK�Ķ�����
		 	else  data[count]=IIC_Read_Byte(0);	 //���һ���ֽ�NACK
	}
    IIC_Stop();//����һ��ֹͣ����
    return count;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data)
*��������:	    ������ֽ�д��ָ���豸 ָ���Ĵ���
����	dev  Ŀ���豸��ַ
		reg	  �Ĵ�����ַ
		length Ҫд���ֽ���
		*data  ��Ҫд�����ݵ��׵�ַ
����   �����Ƿ�ɹ�
*******************************************************************************/ 
u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data)
{
  
 	u8 count = 0;
	IIC_Start();
	IIC_Send_Byte(dev);	   //����д����
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   //���͵�ַ
    IIC_Wait_Ack();	  
	for(count=0;count<length;count++){
		IIC_Send_Byte(data[count]); 
		IIC_Wait_Ack(); 
	 }
	IIC_Stop();//����һ��ֹͣ����

    return 1; //status == 0;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICreadByte(u8 dev, u8 reg, u8 *data)
*��������:	    ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		*data  ���������ݽ�Ҫ��ŵĵ�ַ
����   1
*******************************************************************************/ 
u8 IICreadByte(u8 dev, u8 reg, u8 *data)
{
	*data=I2C_ReadOneByte(dev, reg);
    return 1;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
*��������:	    д��ָ���豸 ָ���Ĵ���һ���ֽ�
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		data  ��Ҫд����ֽ�
����   1
*******************************************************************************/ 
u8 IICwriteByte(u8 dev, u8 reg, u8 data)
{
    return IICwriteBytes(dev, reg, 1, &data);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �еĶ��λ
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		bitStart  Ŀ���ֽڵ���ʼλ
		length   λ����
		data    ��Ÿı�Ŀ���ֽ�λ��ֵ
����   �ɹ� Ϊ1 
 		ʧ��Ϊ0
*******************************************************************************/ 
u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
{

    u8 b;
    if (IICreadByte(dev, reg, &b) != 0) {
        u8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
        data <<= (8 - length);
        data >>= (7 - bitStart);
        b &= mask;
        b |= data;
        return IICwriteByte(dev, reg, b);
    } else {
        return 0;
    }
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �е�1��λ
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		bitNum  Ҫ�޸�Ŀ���ֽڵ�bitNumλ
		data  Ϊ0 ʱ��Ŀ��λ������0 ���򽫱���λ
����   �ɹ� Ϊ1 
 		ʧ��Ϊ0
*******************************************************************************/ 
u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
{
    u8 b;
    IICreadByte(dev, reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return IICwriteByte(dev, reg, b);
}

//------------------End of File----------------------------
