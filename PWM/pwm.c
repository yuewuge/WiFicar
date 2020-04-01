#include "pwm.h"
uint pwm=0,Count=0;
uint Pwmvalue=0;
uint flag = 0;
sbit key = P2^1;

void delayms()
{
	uint i,j;
	for(i=0;i<2000;i++)
	for(j=0;j<2;j++);
}


void init ()
{
	TMOD=0X22;  //双定时器都打开工作方式2
	SCON=0X50;
	TH1=0XFD;    //9600的波特率
	TL1=0XFD;
	TL0=245;	 //1ms定时器
	TH0=245;
	ET0=1;
	TR0=1;
	TR1=1;
	EA=1;
	ES=1;
	PS=1;       //串口中断有最高优先级
	pwm=0;     //高电平的占空比
	Count=0;  //时间计数
	
}
 
void pwmset(uint NewPwmValue)       //设置占空比 （调节PWM）
{
	if((NewPwmValue>=1)&&(NewPwmValue<=99))
	{
			pwm=NewPwmValue;
	}
	else     //新设置的占空比没有意义
	{	
			pwm=0x01;   //输出一个脉冲
	}
}
 
 
void pwmproduce() interrupt 1    //定时0的方式二中断
{
	if(flag == 0){	 //制动
		PWM_OUT1=0;
		PWM_OUT2=0;		   
		PWM_OUT3=0;
		PWM_OUT4=0;
	}else if(flag == 1){		  //前进
		 Count++;           //时间计数 每1ms加一
		 PWM_OUT2=0;
		 PWM_OUT1=1;
		 PWM_OUT4=0;
		 PWM_OUT3=1;
		if(Count<=pwm)    //计数时间小于规定占空比
		{
			ENA = 1;
			ENB = 1;
		}
		else                      //到达了规定时间
		{
			ENA = 0;
			ENB = 0;
			if(Count>=100)   //计数值到了100
			{
					Count=0;     //计数值清零
			}
		}
	}else if(flag == 2){			//后退
		 Count++;           //时间计数 每1ms加一
		 PWM_OUT1=0;
		 PWM_OUT2=1;
		 PWM_OUT4=1;
		 PWM_OUT3=0;
		if(Count<=pwm)    //计数时间小于规定占空比
		{
			ENA = 1;	        //输出高电平
			ENB = 1;
		}
		else                      //到达了规定时间
		{
			ENA = 0;        //输出低电平
			ENB = 0;
			if(Count>=100)  //计数值到了100
			{
				Count=0;     //计数值清零
			}
		}
	}else if(flag == 3){			//前进左拐
		 Count++;           //时间计数 每1ms加一
		 PWM_OUT2=0;
		 PWM_OUT1=1;
		 PWM_OUT4=0;
		 PWM_OUT3=1;
		if(Count<=pwm)    //计数时间小于规定占空比
		{
			ENA = 1;	        //输出高电平
			if(Count<=(pwm/4)){
			 	 ENB = 1;
			}else{
				ENB = 0;
			}
			
		}
		else                      //到达了规定时间
		{
			ENA = 0;        //输出低电平
			ENB = 0;
			if(Count>=100)  //计数值到了100
			{
				Count=0;     //计数值清零
			}
		}
	}else if(flag == 4){			//前进右拐
		 Count++;           //时间计数 每1ms加一
		 PWM_OUT2=0;
		 PWM_OUT1=1;
		 PWM_OUT4=0;
		 PWM_OUT3=1;
		if(Count<=pwm)    //计数时间小于规定占空比
		{
				        //输出高电平
			ENB = 1;
			if(Count<=(pwm/4)){
			 	ENA = 1;
			}else{
				ENA = 0;
			}
		}
		else                      //到达了规定时间
		{
			ENA = 0;        //输出低电平
			ENB = 0;
			if(Count>=100)  //计数值到了100
			{
				Count=0;     //计数值清零
			}
		}
	}else if(flag == 5){			//后退左拐
		 Count++;           //时间计数 每1ms加一
		 PWM_OUT1=0;
		 PWM_OUT2=1;
		 PWM_OUT4=1;
		 PWM_OUT3=0;
		if(Count<=pwm)    //计数时间小于规定占空比
		{
			ENA = 1;	        //输出高电平
			
			if(Count<=(pwm/4)){
			 	ENB = 1;
			}else{
				ENB = 0;
			}
		}
		else                      //到达了规定时间
		{
			ENA = 0;        //输出低电平
			ENB = 0;
			if(Count>=100)  //计数值到了100
			{
					Count=0;     //计数值清零
			}
		}
	}else if(flag == 6){			//后退右拐
		 Count++;           //时间计数 每1ms加一
		 PWM_OUT1=0;
		 PWM_OUT2=1;
		 PWM_OUT4=1;
		 PWM_OUT3=0;
		if(Count<=pwm)    //计数时间小于规定占空比
		{
			ENB = 1;	        //输出高电平
			
			if(Count<=(pwm/4)){
			 	ENA = 1;
			}else{
				ENA = 0;
			}
		}
		else                      //到达了规定时间
		{
			ENA = 0;        //输出低电平
			ENB = 0;
			if(Count>=100)  //计数值到了100
			{
					Count=0;     //计数值清零
			}
		}
	}
	
}
void chuan() interrupt 4      //串口中断
{
	RI=0;   
	Pwmvalue=SBUF;          //接受串口发来的占空比数据
	if(Pwmvalue=='1'){	   //前进
		flag = 1;
		 pwmset(30);       //设置新的占空比	 
	}else if(Pwmvalue=='0'){	//制动
		flag = 0;
	} else if(Pwmvalue=='2'){  //后退
		flag = 2;
		 pwmset(30);       //设置新的占空比
	} else if(Pwmvalue=='3'){  //前进左拐
		flag = 3;
		 pwmset(30);       //设置新的占空比
	} else if(Pwmvalue=='4'){  //前进右拐
		flag = 4;
		 pwmset(30);       //设置新的占空比
	}else if(Pwmvalue=='5'){  //后退
		flag = 5;
		 pwmset(30);       //设置新的占空比
	}else if(Pwmvalue=='6'){  //后退
		flag = 6;
		 pwmset(30);       //设置新的占空比
	}
	
	ES=0;						
	TI=1;						
	while(!TI);
	TI=0;
	ES=1;
}
