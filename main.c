/********************************* �����к�̫�������޹�˾ *******************************
* ʵ �� �� �����Գ���
* ʵ��˵�� ��ʹ�ú��������ң��������С���˶���ʵʱˢ����ʾ��·���Թ�״̬
* ʵ��ƽ̨ ����̫ARM��Ƭ��������
* ���ӷ�ʽ ����ο�interface.h�ļ�
* ע    �� ��ָ������������ͲŻ�ʹС����������ֹͣ����ָ���С����ֹͣ
* ��    �� ����̫���Ӳ�Ʒ�з���    QQ ��1909197536
* ��    �� ��http://shop120013844.taobao.com/
****************************************************************************************/

#include "stm32f10x.h"
#include "interface.h"
#include "LCD12864.h"
#include "IRCtrol.h"
#include "motor.h"
#include "uart.h"

//ȫ�ֱ�������
unsigned int speed_count=0;//ռ�ձȼ����� 50��һ����
char front_left_speed_duty=SPEED_DUTY;
char front_right_speed_duty=SPEED_DUTY;
char behind_left_speed_duty=SPEED_DUTY;
char behind_right_speed_duty=SPEED_DUTY;

unsigned char tick_5ms = 0;//5ms����������Ϊ�������Ļ�������
unsigned char tick_1ms = 0;//1ms����������Ϊ����Ļ���������
unsigned char tick_200ms = 0;//ˢ����ʾ
unsigned char tick_50ms = 0;//ˢ�¹��Թܼ��

unsigned char duoji_count=0;
unsigned char zhuanjiao = 11;//���ת�ǿ��� 5 -- 18
unsigned char duoji_dir = 0;

unsigned char IO_flag=0;

char ctrl_comm = COMM_STOP;//����ָ��
unsigned char continue_time=0;
volatile unsigned char bt_rec_flag=0;//�������Ʊ�־λ

void MeasureInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = FRONT_RIGHT_S_PIN;//����ʹ��GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����GPIO�˿��ٶ�
	GPIO_Init(FRONT_RIGHT_S_GPIO , &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = FRONT_LEFT_S_PIN;//����ʹ��GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����GPIO�˿��ٶ�
	GPIO_Init(FRONT_LEFT_S_GPIO , &GPIO_InitStructure); 
}

int main(void)
{
	delay_init();
	GPIOCLKInit();
	UserLEDInit();
	LCD12864Init();
	IRCtrolInit();
	TIM2_Init();
	MotorInit();
	ServoInit();
	USART3Conf(9600);
	RedRayInit();
	UltraSoundInit();
	
	MeasureInit();

 while(1)
 {	 
	 		if(tick_5ms >= 5)
		{
			tick_5ms = 0;
			tick_200ms++;
			if(tick_200ms >= 40)
			{
				tick_200ms = 0;
				PutStr("AT\r\n");//�������Ͳ���ָ��
				bt_rec_flag = 0;
				
				LEDToggle(LED_PIN);				
				GetDistanceDelay();
				//�������
				if(duoji_dir == 0)
				{
					zhuanjiao++;
					if(zhuanjiao == 18) duoji_dir = 1;
				}
				if(duoji_dir == 1)
				{
					zhuanjiao--;
					if(zhuanjiao == 5) duoji_dir = 0;					
				}
			}
			tick_50ms++;
			if(tick_50ms >= 10)
			{
				char void_l,search_l,search_m,search_r,void_r,speed_l,speed_r;
				search_m = 'M';
				search_r = 'M';
				search_l = 'M';
				void_l = 'M';
				void_r = 'M';
				speed_l = 'M';
				speed_r = 'M';
				
				tick_50ms = 0;
				
				if(SEARCH_M_IO == 1) search_m = '0';
				if(SEARCH_L_IO == 1) search_l = '0';
				if(SEARCH_R_IO == 1) search_r = '0';
				if(VOID_R_IO == 1) void_r = '0';
				if(VOID_L_IO == 1) void_l = '0';
				if(FRONT_LEFT_S_IO == 1) speed_l = '0';
				if(FRONT_RIGHT_S_IO == 1) speed_r = '0';
				LCD12864ShowRedRay(void_l , search_l , search_m , search_r , void_r);
				LCD12864ShowSpeed(speed_l,speed_r);
				LCD12864WriteDistance(distance_cm);
				
				if(bt_rec_flag == 1) 
					LCD12864ShowBT('Y');
				else 
					LCD12864ShowBT('N');
			}
			
			continue_time--;//200ms �޽���ָ���ͣ��
			if(continue_time == 0)
			{
				continue_time = 1;
				CarStop();
			}
			//do something
			if(ir_rec_flag == 1)//���յ������ź�
			{
				ir_rec_flag = 0;
				switch(ctrl_comm)
				{
					case COMM_UP:    CarGo();break;
					case COMM_DOWN:  CarBack();break;
					case COMM_LEFT:  CarLeft();break;
					case COMM_RIGHT: CarRight();break;
					case COMM_STOP:  CarStop();break;
					default : break;
				}
				LCD12864WriteCommand(ctrl_comm);
			}
		}
		
 }
}
