#include "pwm.h"
static uint Pwmscale=30;  //pwm的初值 25%


void main  ()
{
	init();
	pwmset(Pwmscale);   //设置初值
		while(1)
		{
		}
 
}
