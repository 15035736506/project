#include "includes.h"
#include "APP.h"

//static OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];		  //∂®“Â’ª
OS_STK  TASK_START_STK[START_STK_SIZE];
OS_STK  task_led2_stk[START_STK_SIZE];

#define TASK_LED2_PRIO 2
void Task_LED2(void *p_arg);  
int main(void)
{
  	BSP_Init();

	LED1(ON);
	OSInit();
//	OSTaskCreate(TaskStart,(void *)0,&TASK_START_STK[START_STK_SIZE-1], START_TASK_Prio);
	OSTaskCreate(Task_LED2,(void *)0,
	   &task_led2_stk[START_STK_SIZE-1], TASK_LED2_PRIO);
	OSStart();

    return 0;
}


void Task_LED2(void *p_arg)
{
	(void)p_arg;

	while(1)
	{
		LED2(ON);
		OSTimeDlyHMSM(0,0,0,300);
		LED2(OFF);
		OSTimeDlyHMSM(0,0,0,300);
	}
}

