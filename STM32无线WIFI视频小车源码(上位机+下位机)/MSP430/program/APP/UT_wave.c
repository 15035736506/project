#include"includes.h"
//#include"LCD12864.h"
#include"delay.h"

//超声波
#define TRIG_H P3OUT |= BIT0;
#define TRIG_L P3OUT &= ~BIT0;

unsigned char distance_valid_Flag=1;//测量有效标志
unsigned int SignalTimeCount = 0;


//超声波模块
/********************
timerA配置
说明：SMCLK时钟要求固定为8MHz，
********************/
void timerA_conf()
{
        //SMCLK，8分频，停止计数，TAR清零，使能TA中断
  TACTL = TASSEL_2 + ID_3 + MC_0 + TACLR + TAIE;  
}

//超声波模块
void SR04_port_init()
{
  P3OUT |= BIT0;  
  P3DIR |= BIT0;//TRIG触发输入端
  
  P1OUT &= ~BIT5;
  P1DIR &= ~BIT5;//ECHO回响信号输出端
  P1IFG &= ~BIT5;
  P1IE |= BIT5;
  P1IES &= ~BIT5;//上升沿触发
  //在中断中再改为下降沿触发，这样即为跳变沿触发  
}

unsigned int SR04check()
{
  unsigned int n=0;
  float dist[3];
  unsigned int dist_cha[3];
  for(n=0;n<3;n++)
  {
    TRIG_H;
    delay_us(50);//TRIG端口至少给10us的高电平信号
    TRIG_L;//触发信号完毕
//    delay_ms(20);//延时11.8ms以上，以等待一次测量完成
    OSTimeDlyHMSM(0,0,0,20);
    if(distance_valid_Flag>0 && SignalTimeCount>58 && SignalTimeCount<11765)
      //模块最小测量距离所用时间0.02/340=58.8us
      //模块最大测量距离所用时间4/340=11764.7us  (4m指往返距离)
    {
      dist[n] = ((float)SignalTimeCount/2000)*34; //(单位：cm)
//      delay_ms(70);
      OSTimeDlyHMSM(0,0,0,80);
    }     
  }
  //过滤掉无效测量
  dist_cha[0] = (unsigned int)((dist[1]>dist[0])?dist[1]-dist[0]:dist[0]-dist[1]);
  dist_cha[1] = (unsigned int)((dist[2]>dist[1])?dist[2]-dist[1]:dist[1]-dist[0]);
  dist_cha[2] = (unsigned int)((dist[2]>dist[0])?dist[2]-dist[0]:dist[0]-dist[2]);
  if(dist_cha[0]<6 && dist_cha[1]<6 && dist_cha[2]<6)
  {
    return (unsigned int)(dist[1]); //该数据有效
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
    delay_us(50);//TRIG端口至少给10us的高电平信号
    TRIG_L;//触发信号完毕
    delay_ms(20);//延时11.8ms以上，以等待一次测量完成
    if(distance_valid_Flag>0 && SignalTimeCount>58 && SignalTimeCount<11765)
      //模块最小测量距离所用时间0.02/340=58.8us
      //模块最大测量距离所用时间4/340=11764.7us  (4m指往返距离)
    {
      distance = ((float)SignalTimeCount/2000)*34; //(单位：cm)
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
  if(P1IN&BIT5)//上升沿触发的中断
  {
    TAR = 0;//清空计数器
    TACTL |= MC_2;//连续计数（开始计数）
    P1IES |= BIT5;//改为下降沿中断
    distance_valid_Flag = 1; //测量有效标志位置位
  }
  else    //下降沿触发的中断
  {
    SignalTimeCount = TAR;//读取计数器的值
    TACTL |= MC_0;//停止计数
    P1IES &= ~BIT5;//改为上升沿中断 
    distance_valid_Flag = 2;//测量完成
  }
  P1IFG = 0;//清除标志 
    OS_EXIT_CRITICAL();
}

#pragma vector = TIMERA1_VECTOR//定时器共用向量
__interrupt void TA_timer()
{
    OS_CPU_SR cpu_sr = 0;
  OS_ENTER_CRITICAL();
  switch(TAIV)
  {
  case 10: //定时器溢出
    TACTL |= MC_0;//停止计数
    TAR = 0;//清空计数器
    SignalTimeCount = 0;
    distance_valid_Flag = 0; //此次测量的距离无效
    P1IES &= ~BIT5;//改为上升沿中断 
    break;
  default:
    break;
  }
  //读取TAIV后，标志位由硬件自动清除
    OS_EXIT_CRITICAL();
}

