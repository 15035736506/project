#ifndef _motor_h
#define _motor_h

#define STOP 0
#define RUN  1
#define stop         0
#define forward      1
#define goback       2
#define turnleft     3
#define turnright    4
#define ZKBmin  125
#define ZKBmax  198

typedef struct
{
  unsigned int ZKB_left;
  unsigned int ZKB_right;
  unsigned char direction; 
}speedZKB;

unsigned char timerB_start();
void Stop_conf();
void GoForward_conf(unsigned int ZKB);
void DrawBack_conf(unsigned int ZKB);
void TurnLeft_conf(unsigned int ZKB);
void TurnRight_conf(unsigned int ZKB);

unsigned char speedconf(speedZKB now_ZKB);


#endif

