/*4 bit lcd driver */

#include<lpc21xx.h>
#include"header.h"

void lcd_data(unsigned char data)//4 bit lcd data
{ 
unsigned int temp;  //higher nibble
IOCLR1=0xFE<<16;
temp=(data&0xF0)<<16;//sending higher nibble data
IOSET1=temp;

IOSET1=1<<17;//RS=1 
IOCLR1=1<<18;//RW
IOSET1=1<<19;//EN
delay_ms(2);
IOCLR1=1<<19;

IOCLR1=0xFE<<16;//lower nibble
temp=(data&0x0F)<<20;//writing binary values on data pins
IOSET1=temp;

IOSET1=1<<17;//RS
IOCLR1=1<<18;//RW
IOSET1=1<<19;
delay_ms(2);
IOCLR1=1<<19;
}

void lcd_cmd(unsigned char cmd)
{	   
unsigned int temp; //HIGHER NIBBLE
IOCLR1=0xFE<<16;
temp=(cmd&0xF0)<<16;
IOSET1=temp;
	
	IOCLR1=1<<17;//rs=0 in cmd Mode
	IOCLR1=1<<18;//rw=0 for write operation
	IOSET1=1<<19;
	delay_ms(2);
	IOCLR1=1<<19;

//LOWER NIBBLE
IOCLR1=0xFE<<16;
temp=(cmd&0x0F)<<20;//binary data
IOSET1=temp;
IOCLR1=1<<17;//RS=0 IN CMD MODE
IOCLR1=1<<18;
IOSET1=1<<19;//TO START LCD
delay_ms(2);
IOCLR1=1<<19;
}

void lcd_init()
{
IODIR1=0xFE<<16;//set GPIO as o/p direction
IOCLR1=1<<19;  //clear enable pin
PINSEL2=0x0;
lcd_cmd(0x02);//enable 4 bit mode
//lcd_cmd(0x03);//hardware purpose
lcd_cmd(0x28);//enable 1st and 2nd row in 4 bit
lcd_cmd(0x0E);//enable cursor
lcd_cmd(0x01);//clear screen
}


//lcd string function
void lcd_string(char *ptr)
{
while(*ptr!=0)
{
lcd_data(*ptr);
ptr++;
}
}

void lcd_itoa(int num)
{
int a[12],i;
if(num==0)
lcd_data('0');
if(num<0){
lcd_data('-');
num=-num;
}
i=0;
while(num>0)
{
a[i]=(num%10)+48;
num=num/10;
i++;
}
for(i=i-1;i>=0;i--)
{
lcd_data(a[i]);
}
}

void lcd_ftoa(float f){
int i;
if(f<0)
{
lcd_data('-');
f=-f;
}
i=f;
lcd_itoa(i);
i=(f-i)*100;
lcd_data('.');
lcd_itoa(i);
}


void lcd_cgram()
{
unsigned char a[10]={0x03,0x03,0x0,0x0,0x0,0x0,0x0};
int i;
lcd_cmd(0x40);//Page0 starting address
for(i=0;i<=7;i++)
{
lcd_data(a[i]);
}
}








