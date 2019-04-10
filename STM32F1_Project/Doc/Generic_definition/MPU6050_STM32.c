#include "MPU6050_STM32.h"


unsigned long now, lastTime = 0;
float dt;                                   //微分时间

int16_t ax, ay, az, gx, gy, gz;            		   	  //加速度计陀螺仪原始数据
float aax=0, aay=0,aaz=0, agx=0, agy=0, agz=0;  	  //角度变量
long axo = 0, ayo = 0, azo = 0;             				//加速度计偏移量
long gxo = 0, gyo = 0, gzo = 0;           		   	  //陀螺仪偏移量
long agxo=0, agyo=0,agzo=0;
float pi = 3.1415926;
float AcceRatio = 16384.0;                			 	  //加速度计比例系数
float GyroRatio = 131.0;              			      	//陀螺仪比例系数
float eagx,eagy,eagz;
uint8_t n_sample = 8;                   			    	//加速度计滤波算法采样个数
float aaxs[8] = {0}, aays[8] = {0}, aazs[8] = {0};  //x,y轴采样队列
long aax_sum, aay_sum,aaz_sum;                      //x,y轴采样和

float a_x[10]={0}, a_y[10]={0},a_z[10]={0} ,g_x[10]={0} ,g_y[10]={0},g_z[10]={0}; //加速度计协方差计算队列
float Px=1, Rx, Kx, Sx, Vx, Qx;                     //x轴卡尔曼变量
float Py=1, Ry, Ky, Sy, Vy, Qy;             				//y轴卡尔曼变量
float Pz=1, Rz, Kz, Sz, Vz, Qz;            				  //z轴卡尔曼变量

void InitMPU6050(u8 GPIOx,u32 MPUSCL,u32 MPUSDA)
{
	SendLine_IIC(GPIOx,MPUSCL,MPUSDA);
	delay_ms(20);
	IICwriteByte(SlaveAddress,PWR_MGMT_1, 0x00);	      //解除休眠状态
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
	return (H<<8)+L;   //合成数据
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
