#include "stm32f10x_exti.h"
#include "SCCB.h"
#include "OV7725.h"

#include "misc.h"
#include "SensorConfig.h"
#include "usart1.h"



u8 Ov7725_vsync;	 /* ֡ͬ���ź� */
volatile u8 Frame_Count = 0 ;
extern u8 OV7725_REG_NUM;
extern Register_Info Sensor_Config[];


/************************************************
 * ��������FIFO_GPIO_Configuration
 * ����  ��FIFO GPIO����
 * ����  ����
 * ���  ����
 * ע��  ����
 ************************************************/
void FIFO_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);

    /*PB5(FIFO_WRST--FIFOд��λ)*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*PD3(FIFO_WEN--FIFOдʹ��)  PD6(FIFO_REN--FIFO��ʹ��)*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /*PE0(FIFO_RRST--FIFO����λ)  PE2(FIFO_RCLK-FIFO��ʱ��)*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /*PB8-PB15(FIFO_DATA--FIFO�������)*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    FIFO_CS_L();	  					/*����ʹFIFO���ʹ��*/
    FIFO_WE_H();   						/*����ʹFIFOд����*/

}




/************************************************
 * ��������VSYNC_GPIO_Configuration
 * ����  ��OV7725 GPIO����
 * ����  ����
 * ���  ����
 * ע��  ����
 ************************************************/
static void VSYNC_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	  /*PA0---VSYNC*/
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}



/************************************************
 * ��������VSYNC_NVIC_Configuration
 * ����  ��VSYNC�ж�����
 * ����  ����
 * ���  ����
 * ע��  ����
 ************************************************/
static  void VSYNC_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}





/************************************************
 * ��������VSYNC_EXTI_Configuration
 * ����  ��OV7725 VSYNC�жϹܽ�����
 * ����  ����
 * ���  ����
 * ע��  ����
 ************************************************/
static  void VSYNC_EXTI_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising ; /*�����ش���*/
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_GenerateSWInterrupt(EXTI_Line0);	/*�жϹҵ� EXTI_Line0  ��*/
}





/************************************************
 * ��������VSYNC_Init
 * ����  ��OV7725 VSYNC�ж��������
 * ����  ����
 * ���  ����
 * ע��  ����
 ************************************************/
void VSYNC_Init(void)
{
    VSYNC_GPIO_Configuration();
    VSYNC_EXTI_Configuration();
    VSYNC_NVIC_Configuration();
}







/************************************************
 * ��������Sensor_Init
 * ����  ��Sensor��ʼ��
 * ����  ����
 * ���  ������1�ɹ�������0ʧ��
 * ע��  ����
 ************************************************/
ErrorStatus Ov7725_Init(void)
{
    u16 i = 0;
    u8 Sensor_IDCode = 0;
//	printf("\r\n>>>>>>STM32F103 OV7725&FIFO Demo<<<<<<\r\n");
//	DEBUG("OV7725 Register Config Start!");
    if( 0 == SCCB_WriteByte ( 0x12, 0x80 ) ) /*��λsensor */
    {
        DEBUG("����:SCCBд���ݴ���");
//        Delay_ms(50);
        return ERROR ;
    }
//    Delay_ms(50);
    if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, 0x0b ) )	 /* ��ȡsensor ID��*/
    {        
        DEBUG("����:��ȡIDʧ��"); 
		return ERROR;

    }
//    DEBUG("Get ID success��SENSOR ID is 0x%x", Sensor_IDCode);
//    DEBUG("Config Register Number is %d ", OV7725_REG_NUM);
    if(Sensor_IDCode == OV7725_ID)
    {
        for( i = 0 ; i < OV7725_REG_NUM ; i++ )
        {
            if( 0 == SCCB_WriteByte(Sensor_Config[i].Address, Sensor_Config[i].Value) )
            {                
				DEBUG("����:д�Ĵ���0x%xʧ��", Sensor_Config[i].Address);
				return ERROR;
            }
        }
    }
    else
    {
        return ERROR;
    }
//	DEBUG("OV7725 Register Config Success!");
    return SUCCESS;
}


