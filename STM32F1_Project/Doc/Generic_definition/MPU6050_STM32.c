#include "MPU6050_STM32.h"


unsigned long now, lastTime = 0;
float dt;                                   //΢��ʱ��

int16_t ax, ay, az, gx, gy, gz;            		   	  //���ٶȼ�������ԭʼ����
float aax=0, aay=0,aaz=0, agx=0, agy=0, agz=0;  	  //�Ƕȱ���
long axo = 0, ayo = 0, azo = 0;             				//���ٶȼ�ƫ����
long gxo = 0, gyo = 0, gzo = 0;           		   	  //������ƫ����
long agxo=0, agyo=0,agzo=0;
float pi = 3.1415926;
float AcceRatio = 16384.0;                			 	  //���ٶȼƱ���ϵ��
float GyroRatio = 131.0;              			      	//�����Ǳ���ϵ��
float eagx,eagy,eagz;
uint8_t n_sample = 8;                   			    	//���ٶȼ��˲��㷨��������
float aaxs[8] = {0}, aays[8] = {0}, aazs[8] = {0};  //x,y���������
long aax_sum, aay_sum,aaz_sum;                      //x,y�������

float a_x[10]={0}, a_y[10]={0},a_z[10]={0} ,g_x[10]={0} ,g_y[10]={0},g_z[10]={0}; //���ٶȼ�Э����������
float Px=1, Rx, Kx, Sx, Vx, Qx;                     //x�Ῠ��������
float Py=1, Ry, Ky, Sy, Vy, Qy;             				//y�Ῠ��������
float Pz=1, Rz, Kz, Sz, Vz, Qz;            				  //z�Ῠ��������

void InitMPU6050(u8 GPIOx,u32 MPUSCL,u32 MPUSDA)
{
	SendLine_IIC(GPIOx,MPUSCL,MPUSDA);
	delay_ms(20);
	IICwriteByte(SlaveAddress,PWR_MGMT_1, 0x00);	      //�������״̬
	IICwriteByte(SlaveAddress,SMPLRT_DIV, 0x07);
	IICwriteByte(SlaveAddress,CONFIG, 0x06);
	IICwriteByte(SlaveAddress,GYRO_CONFIG, 0x18);
	IICwriteByte(SlaveAddress,ACCEL_CONFIG, 0x01);
}

u32 GetData_MPU6050(u8 REG_Address)
{
	u8 H,L;
	H=I2C_ReadOneByte(SlaveAddress,REG_Address);
	L=I2C_ReadOneByte(SlaveAddress,REG_Address+1);
	return (H<<8)+L;   //�ϳ�����
}

void GetData_6BIT()
{
	ax=GetData_MPU6050(ACCEL_XOUT_H);
	ay=GetData_MPU6050(ACCEL_YOUT_H);
	az=GetData_MPU6050(ACCEL_ZOUT_H);
	gx=GetData_MPU6050(GYRO_XOUT_H);
	gy=GetData_MPU6050(GYRO_YOUT_H);
	gz=GetData_MPU6050(GYRO_ZOUT_H);
}
