#include"includes.h"
//#include"LCD12864.h"
#include"delay.h"

//������
#define TRIG_H P3OUT |= BIT0;
#define TRIG_L P3OUT &= ~BIT0;

unsigned char distance_valid_Flag=1;//������Ч��־
unsigned int SignalTimeCount = 0;


//������ģ��
/********************
timerA����
˵����SMCLKʱ��Ҫ��̶�Ϊ8MHz��
********************/
void timerA_conf()
{
        //SMCLK��8��Ƶ��ֹͣ������TAR���㣬ʹ��TA�ж�
  TACTL = TASSEL_2 + ID_3 + MC_0 + TACLR + TAIE;  
}

//������ģ��
void SR04_port_init()
{
  P3OUT |= BIT0;  
  P3DIR |= BIT0;//TRIG���������
  
  P1OUT &= ~BIT5;
  P1DIR &= ~BIT5;//ECHO�����ź������
  P1IFG &= ~BIT5;
  P1IE |= BIT5;
  P1IES &= ~BIT5;//�����ش���
  //���ж����ٸ�Ϊ�½��ش�����������Ϊ�����ش���  
}

unsigned int SR04check()
{
  unsigned int n=0;
  float dist[3];
  unsigned int dist_cha[3];
  for(n=0;n<3;n++)
  {
    TRIG_H;
    delay_us(50);//TRIG�˿����ٸ�10us�ĸߵ�ƽ�ź�
    TRIG_L;//�����ź����
//    delay_ms(20);//��ʱ11.8ms���ϣ��Եȴ�һ�β������
    OSTimeDlyHMSM(0,0,0,20);
    if(distance_valid_Flag>0 && SignalTimeCount>58 && SignalTimeCount<11765)
      //ģ����С������������ʱ��0.02/340=58.8us
      //ģ����������������ʱ��4/340=11764.7us  (4mָ��������)
    {
      dist[n] = ((float)SignalTimeCount/2000)*34; //(��λ��cm)
//      delay_ms(70);
      OSTimeDlyHMSM(0,0,0,80);
    }     
  }
  //���˵���Ч����
  dist_cha[0] = (unsigned int)((dist[1]>dist[0])?dist[1]-dist[0]:dist[0]-dist[1]);
  dist_cha[1] = (unsigned int)((dist[2]>dist[1])?dist[2]-dist[1]:dist[1]-dist[0]);
  dist_cha[2] = (unsigned int)((dist[2]>dist[0])?dist[2]-dist[0]:dist[0]-dist[2]);
  if(dist_cha[0]<6 && dist_cha[1]<6 && dist_cha[2]<6)
  {
    return (unsigned int)(dist[1]); //��������Ч
  }
  return 0;
}

/*
void SR04(void *p_arg)
{
  float distance;
//  unsigned int n=0;

  SR04_port_init();
  timerA_conf();
//  for(;n<5;n++)
//  {  
    TRIG_H;
    delay_us(50);//TRIG�˿����ٸ�10us�ĸߵ�ƽ�ź�
    TRIG_L;//�����ź����
    delay_ms(20);//��ʱ11.8ms���ϣ��Եȴ�һ�β������
    if(distance_valid_Flag>0 && SignalTimeCount>58 && SignalTimeCount<11765)
      //ģ����С������������ʱ��0.02/340=58.8us
      //ģ����������������ʱ��4/340=11764.7us  (4mָ��������)
    {
      distance = ((float)SignalTimeCount/2000)*34; //(��λ��cm)
      delay_ms(100);
    }    
//  }  
}
*/
#pragma vector = PORT1_VECTOR
__interrupt void PORT1()
{
    OS_CPU_SR cpu_sr = 0;
  OS_ENTER_CRITICAL();
  if(P1IN&BIT5)//�����ش������ж�
  {
    TAR = 0;//��ռ�����
    TACTL |= MC_2;//������������ʼ������
    P1IES |= BIT5;//��Ϊ�½����ж�
    distance_valid_Flag = 1; //������Ч��־λ��λ
  }
  else    //�½��ش������ж�
  {
    SignalTimeCount = TAR;//��ȡ��������ֵ
    TACTL |= MC_0;//ֹͣ����
    P1IES &= ~BIT5;//��Ϊ�������ж� 
    distance_valid_Flag = 2;//�������
  }
  P1IFG = 0;//�����־ 
    OS_EXIT_CRITICAL();
}

#pragma vector = TIMERA1_VECTOR//��ʱ����������
__interrupt void TA_timer()
{
    OS_CPU_SR cpu_sr = 0;
  OS_ENTER_CRITICAL();
  switch(TAIV)
  {
  case 10: //��ʱ�����
    TACTL |= MC_0;//ֹͣ����
    TAR = 0;//��ռ�����
    SignalTimeCount = 0;
    distance_valid_Flag = 0; //�˴β����ľ�����Ч
    P1IES &= ~BIT5;//��Ϊ�������ж� 
    break;
  default:
    break;
  }
  //��ȡTAIV�󣬱�־λ��Ӳ���Զ����
    OS_EXIT_CRITICAL();
}

