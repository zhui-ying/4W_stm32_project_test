#include "interface.h"
volatile unsigned char status = 0;//����ǰ״̬��0,���� 1 ���ʹ����źţ�2 �ȴ��źŷ���,3 
unsigned int dis_count = 0;//������ʱ
volatile unsigned int distance_cm = 0;//��ǰ����

void delay_init(void)
{
   SysTick->CTRL&=0xfffffffb;//���ƼĴ�����ѡ���ⲿʱ�Ӽ�ϵͳʱ�ӵİ˷�֮һ��HCLK/8��72M/8=9M��
}

//1us ��ʱ����
void Delay_us(u32 Nus)   
{   
SysTick->LOAD=Nus*9;          //ʱ�����    72M��Ƶ     
SysTick->CTRL|=0x01;             //��ʼ����      
while(!(SysTick->CTRL&(1<<16))); //�ȴ�ʱ�䵽��   
SysTick->CTRL=0X00000000;        //�رռ�����   
SysTick->VAL=0X00000000;         //��ռ�����        
} 

void Delayms(u32 Nms)
{
	while(Nms--)
	{
		Delay_us(1000);
	}
}

//ʹ������GPIOʱ��
void GPIOCLKInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG , ENABLE);
}

void UserLEDInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = LED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����GPIO�˿��ٶ�
	GPIO_Init(LED_GPIO , &GPIO_InitStructure);
	
	LED_SET;	
}

//������Թܳ�ʼ��
void RedRayInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = SEARCH_M_PIN;//����ʹ��GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����GPIO�˿��ٶ�
	GPIO_Init(SEARCH_M_GPIO , &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = SEARCH_R_PIN;//����ʹ��GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����GPIO�˿��ٶ�
	GPIO_Init(SEARCH_R_GPIO , &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = SEARCH_L_PIN;//����ʹ��GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����GPIO�˿��ٶ�
	GPIO_Init(SEARCH_L_GPIO , &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = VOID_R_PIN;//����ʹ��GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����GPIO�˿��ٶ�
	GPIO_Init(VOID_R_GPIO , &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = VOID_L_PIN;//����ʹ��GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����GPIO�˿��ٶ�
	GPIO_Init(VOID_L_GPIO , &GPIO_InitStructure); 
}


void ServoInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = Servo_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����GPIO�˿��ٶ�
	GPIO_Init(Servo_GPIO , &GPIO_InitStructure);
	
	Servo_SET;//Ĭ�ϸ��ߵ�λmodfied by LC 2015.09.20 12:00
}

void Time4Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);//72M / 72 = 1us
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
}

//�ⲿ�ж����� ������,����û�õ��жϼ�����ʽ
void UltraSoundInit(void)	
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef 	EXTI_InitStructure;//����һ���ⲿ�ж���صĽṹ��
	NVIC_InitTypeDef NVIC_InitStructure; //����һ���жϵĽṹ��
	
	GPIO_InitStructure.GPIO_Pin = Echo_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����GPIO�˿��ٶ�
	GPIO_Init(Echo_GPIO , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Trig_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����GPIO�˿��ٶ�
	GPIO_Init(Trig_GPIO , &GPIO_InitStructure);

	GPIO_EXTILineConfig(Echo_PORTSOURCE , Echo_PINSOURCE);//��GPIO�����ó��ⲿ�ж� 
	EXTI_InitStructure.EXTI_Line = Echo_EXITLINE;//����Ӧ��GPIO�����ӵ��ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�ж��¼����ͣ��½���
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//ѡ��ģʽ���ж���
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;//ʹ�ܸ��ж�
	EXTI_Init(&EXTI_InitStructure);//�����úõĲ���д��Ĵ���
			
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	 //�׼�Ϊ0������Ƕ��
	NVIC_InitStructure.NVIC_IRQChannel = 	Echo_IRQCH;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//�����ȼ�0�����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	//�����ȼ������
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//ʹ�ܸ�ģ���ж�
	NVIC_Init(&NVIC_InitStructure);	//�жϳ�ʼ�������ṹ�嶨�������ִ��
	
	Time4Init();
	Trig_RESET;
}

//buletooth test
void BuletoothInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;//���崮�ڶ�Ӧ�ܽŵĽṹ��
	//USART3_Tx_Pin Configure 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;//��������ٶ�50MHz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//���츴�����
	GPIO_Init(GPIOB , &GPIO_InitStruct);//����ʼ���õĽṹ��װ��Ĵ���

//USART3_Rx_Pin Configure
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIOģʽ��������
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;//��������
  GPIO_Init(GPIOB, &GPIO_InitStruct);//����ʼ���õĽṹ��װ��Ĵ���	
}

//�ⲿ�ж����� ������
void Sound_Configuration(void)	
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef 	EXTI_InitStructure;//����һ���ⲿ�ж���صĽṹ��
	NVIC_InitTypeDef NVIC_InitStructure; //����һ���жϵĽṹ��
	
	GPIO_InitStructure.GPIO_Pin = Echo_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����GPIO�˿��ٶ�
	GPIO_Init(IRIN_GPIO , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Trig_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����GPIO�˿��ٶ�
	GPIO_Init(Trig_GPIO , &GPIO_InitStructure);
		
	GPIO_EXTILineConfig(Echo_PORTSOURCE , Echo_PINSOURCE);//��GPIO�����ó��ⲿ�ж� 
	EXTI_InitStructure.EXTI_Line = Echo_EXITLINE;//����Ӧ��GPIO�����ӵ��ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�ж��¼����ͣ��½���
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//ѡ��ģʽ���ж���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ�ܸ��ж�
	EXTI_Init(&EXTI_InitStructure);//�����úõĲ���д��Ĵ���
			
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	 //�׼�Ϊ0������Ƕ��
	NVIC_InitStructure.NVIC_IRQChannel = 	Echo_IRQCH;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//�����ȼ�0�����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	//�����ȼ������
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//ʹ�ܸ�ģ���ж�
	NVIC_Init(&NVIC_InitStructure);	//�жϳ�ʼ�������ṹ�嶨�������ִ��
}

//��ʱ�ķ�ʽ��ȡ����ֵ
void GetDistanceDelay(void)
{
		//���ʹ����ź�
		Trig_SET;
		status = 1;
	TIM_SetCounter(TIM4, 0);
	TIM_Cmd(TIM4, ENABLE);//�򿪶�ʱ��
		while(TIM_GetCounter(TIM4) < 11);//��ʱ����10us
		status = 2;
		Trig_RESET;
		TIM_SetCounter(TIM4, 0);
	
		while(Echo == 0)//�ȴ������ź���ʼλ��
		{
			if(TIM_GetCounter(TIM4) > 60000)//��Ӧʱ�䳬��������Χ 
			{
				status = 0;
				distance_cm = 0;//ʧ�ܺ�ͺ���
				TIM_Cmd(TIM4, DISABLE);//�رն�ʱ��
				return;//����ʧ��
			}
		}
		TIM_SetCounter(TIM4, 0);
		while(Echo == 1)//��ʼ���㳤��
		{
			int count = TIM_GetCounter(TIM4);
			if(count > 60000)//��Ӧʱ�䳬��������Χ 
			{
				status = 0;
				distance_cm = 0;//ʧ�ܺ�ͺ���
				TIM_Cmd(TIM4, DISABLE);//�رն�ʱ��
				return;//����ʧ��
			}
		}
		dis_count = TIM_GetCounter(TIM4);
		TIM_Cmd(TIM4, DISABLE);//�رն�ʱ��
		distance_cm = (unsigned int)(((long)(dis_count) * 34)/2000);//����340m/s
		status = 0;//׼���´η���		
}


/**-------------------------------------------------------
  * @������ NVIC_TIM5Configuration
  * @����   ����TIM5�ж�������������
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
static void NVIC_TIM2Configuration(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Set the Vector Table base address at 0x08000000 */
    //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

    /* Enable the TIM5 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}


void TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* TIM2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* Time base configuration */
    //��������Զ�װ�صļ���ֵ�����ڼ����Ǵ�0��ʼ�ģ�����Ϊ100us
    TIM_TimeBaseStructure.TIM_Period = (100 - 1);//10kHz
    // �������Ԥ��Ƶϵ����������Ϊ0ʱ��ʾ����Ƶ����Ҫ��1
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);//1MHz
    // �߼�Ӧ�ñ��β��漰�������ڶ�ʱ��ʱ��(CK_INT)Ƶ���������˲���(ETR,TIx)
    // ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    //���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //��ʼ����ʱ��5
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* Clear TIM5 update pending flag[���TIM5����жϱ�־] */
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    /* TIM IT enable */ //������ж�
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    /* TIM5 enable counter */
    TIM_Cmd(TIM2, ENABLE);  //������ʹ�ܣ���ʼ����

    /* �жϲ������� */
    NVIC_TIM2Configuration();
}

void LEDToggle(uint16_t Led)
{
    /* ָ���ܽ������� 1��ʵ�ֶ�Ӧ��LEDָʾ��״̬ȡ��Ŀ�� */
	LED_GPIO->ODR ^= Led;
	//��Ҫ���Ч�ʣ�����ֱ�ӵ��� LEDnOBB = !LEDnOBB;
}


