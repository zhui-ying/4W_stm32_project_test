#include "interface.h"
volatile unsigned char status = 0;//程序当前状态，0,空闲 1 发送触发信号，2 等待信号返回,3 
unsigned int dis_count = 0;//脉宽长计时
volatile unsigned int distance_cm = 0;//当前距离

void delay_init(void)
{
   SysTick->CTRL&=0xfffffffb;//控制寄存器，选择外部时钟即系统时钟的八分之一（HCLK/8；72M/8=9M）
}

//1us 延时函数
void Delay_us(u32 Nus)   
{   
SysTick->LOAD=Nus*9;          //时间加载    72M主频     
SysTick->CTRL|=0x01;             //开始倒数      
while(!(SysTick->CTRL&(1<<16))); //等待时间到达   
SysTick->CTRL=0X00000000;        //关闭计数器   
SysTick->VAL=0X00000000;         //清空计数器        
} 

void Delayms(u32 Nms)
{
	while(Nms--)
	{
		Delay_us(1000);
	}
}

//使能所有GPIO时钟
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
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(LED_GPIO , &GPIO_InitStructure);
	
	LED_SET;	
}

//红外光电对管初始化
void RedRayInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = SEARCH_M_PIN;//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(SEARCH_M_GPIO , &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = SEARCH_R_PIN;//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(SEARCH_R_GPIO , &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = SEARCH_L_PIN;//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(SEARCH_L_GPIO , &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = VOID_R_PIN;//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(VOID_R_GPIO , &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = VOID_L_PIN;//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(VOID_L_GPIO , &GPIO_InitStructure); 
}


void ServoInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = Servo_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(Servo_GPIO , &GPIO_InitStructure);
	
	Servo_SET;//默认给高电位modfied by LC 2015.09.20 12:00
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

//外部中断配置 超声波,这里没用到中断计数方式
void UltraSoundInit(void)	
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef 	EXTI_InitStructure;//定义一个外部中断相关的结构体
	NVIC_InitTypeDef NVIC_InitStructure; //定义一个中断的结构体
	
	GPIO_InitStructure.GPIO_Pin = Echo_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(Echo_GPIO , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Trig_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(Trig_GPIO , &GPIO_InitStructure);

	GPIO_EXTILineConfig(Echo_PORTSOURCE , Echo_PINSOURCE);//将GPIO口配置成外部中断 
	EXTI_InitStructure.EXTI_Line = Echo_EXITLINE;//将对应的GPIO口连接到中断线上
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//中断事件类型，下降沿
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//选择模式，中断型
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;//使能该中断
	EXTI_Init(&EXTI_InitStructure);//将配置好的参数写入寄存器
			
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	 //阶级为0，不可嵌套
	NVIC_InitStructure.NVIC_IRQChannel = 	Echo_IRQCH;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//主优先级0，最高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	//子优先级，最低
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//使能该模块中断
	NVIC_Init(&NVIC_InitStructure);	//中断初始化，将结构体定义的数据执行
	
	Time4Init();
	Trig_RESET;
}

//buletooth test
void BuletoothInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;//定义串口对应管脚的结构体
	//USART3_Tx_Pin Configure 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;//输出引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;//设置最高速度50MHz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//推挽复用输出
	GPIO_Init(GPIOB , &GPIO_InitStruct);//将初始化好的结构体装入寄存器

//USART3_Rx_Pin Configure
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO模式悬浮输入
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;//输入引脚
  GPIO_Init(GPIOB, &GPIO_InitStruct);//将初始化好的结构体装入寄存器	
}

//外部中断配置 超声波
void Sound_Configuration(void)	
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef 	EXTI_InitStructure;//定义一个外部中断相关的结构体
	NVIC_InitTypeDef NVIC_InitStructure; //定义一个中断的结构体
	
	GPIO_InitStructure.GPIO_Pin = Echo_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(IRIN_GPIO , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Trig_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(Trig_GPIO , &GPIO_InitStructure);
		
	GPIO_EXTILineConfig(Echo_PORTSOURCE , Echo_PINSOURCE);//将GPIO口配置成外部中断 
	EXTI_InitStructure.EXTI_Line = Echo_EXITLINE;//将对应的GPIO口连接到中断线上
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//中断事件类型，下降沿
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//选择模式，中断型
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能该中断
	EXTI_Init(&EXTI_InitStructure);//将配置好的参数写入寄存器
			
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	 //阶级为0，不可嵌套
	NVIC_InitStructure.NVIC_IRQChannel = 	Echo_IRQCH;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//主优先级0，最高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	//子优先级，最低
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//使能该模块中断
	NVIC_Init(&NVIC_InitStructure);	//中断初始化，将结构体定义的数据执行
}

//延时的方式读取距离值
void GetDistanceDelay(void)
{
		//发送触发信号
		Trig_SET;
		status = 1;
	TIM_SetCounter(TIM4, 0);
	TIM_Cmd(TIM4, ENABLE);//打开定时器
		while(TIM_GetCounter(TIM4) < 11);//延时超过10us
		status = 2;
		Trig_RESET;
		TIM_SetCounter(TIM4, 0);
	
		while(Echo == 0)//等待回向信号起始位置
		{
			if(TIM_GetCounter(TIM4) > 60000)//回应时间超出正常范围 
			{
				status = 0;
				distance_cm = 0;//失败后就后退
				TIM_Cmd(TIM4, DISABLE);//关闭定时器
				return;//本次失败
			}
		}
		TIM_SetCounter(TIM4, 0);
		while(Echo == 1)//开始计算长度
		{
			int count = TIM_GetCounter(TIM4);
			if(count > 60000)//回应时间超出正常范围 
			{
				status = 0;
				distance_cm = 0;//失败后就后退
				TIM_Cmd(TIM4, DISABLE);//关闭定时器
				return;//本次失败
			}
		}
		dis_count = TIM_GetCounter(TIM4);
		TIM_Cmd(TIM4, DISABLE);//关闭定时器
		distance_cm = (unsigned int)(((long)(dis_count) * 34)/2000);//声速340m/s
		status = 0;//准备下次发送		
}


/**-------------------------------------------------------
  * @函数名 NVIC_TIM5Configuration
  * @功能   配置TIM5中断向量参数函数
  * @参数   无
  * @返回值 无
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
    //这个就是自动装载的计数值，由于计数是从0开始的，周期为100us
    TIM_TimeBaseStructure.TIM_Period = (100 - 1);//10kHz
    // 这个就是预分频系数，当由于为0时表示不分频所以要减1
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);//1MHz
    // 高级应用本次不涉及。定义在定时器时钟(CK_INT)频率与数字滤波器(ETR,TIx)
    // 使用的采样频率之间的分频比例
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    //向上计数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //初始化定时器5
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* Clear TIM5 update pending flag[清除TIM5溢出中断标志] */
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    /* TIM IT enable */ //打开溢出中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    /* TIM5 enable counter */
    TIM_Cmd(TIM2, ENABLE);  //计数器使能，开始工作

    /* 中断参数配置 */
    NVIC_TIM2Configuration();
}

void LEDToggle(uint16_t Led)
{
    /* 指定管脚输出异或 1，实现对应的LED指示灯状态取反目的 */
	LED_GPIO->ODR ^= Led;
	//若要提高效率，建议直接调用 LEDnOBB = !LEDnOBB;
}


