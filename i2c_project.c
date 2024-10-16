/*Real time clock implementation on LPC2129 using DS1307*/
#include<lpc21xx.h>
#include"header.h"
#define SW1 ((IOPIN0>>14)&1)
#define SW2 ((IOPIN0>>15)&1)

u8 hex(u8 x){
u8 r;
r=x/10;
x=(x%10);
x=(r<<4)|x;
return x;
}
void lcd_print(u8 x){
lcd_data((x/0x10)+48);
lcd_data((x%0x10)+48);
}

//u8 al=0,ah=0,ahh=0,am=0,amm=0,alm=0;

struct time{
u8 h,m,s;	
}t;
struct date{
u8 dt,m,y;	
}d;
u8 day;
u8 am_pm;
u8 flag=0,c=0;
/*main function*/

main(){
u32 h=1,hr,hh,m=0,mm,s=0,ss,a=0,y=0,yy,mo=1,mon,dd=1,ddt,dow=1,doww,date,year;
lcd_init();
i2c_init();
uart0_init(9600);
config_vic_for_eint0();
config_eint0();

while(1){
/*reading rtc*/
while(flag==0){
//uart0_tx_string("\r\nhi\r\n");
t.s=i2c_byte_read_frame(0xD0,0x0);
t.m=i2c_byte_read_frame(0xD0,0x1);
t.h=i2c_byte_read_frame(0xD0,0x2);
am_pm=(t.h>>5)&1;
//lcd_cmd(0x89);
 lcd_cmd(0x80);
 hr=(t.h&(0x1F));
 lcd_print(hr);
 uart0_tx(hr);
 lcd_data(':');
 uart0_tx(':');
 lcd_print(t.m);
 uart0_tx(t.m);
 lcd_data(':');
 uart0_tx(t.s);
 lcd_print(t.s);
 if(am_pm)
 lcd_string(" PM");
else
  lcd_string(" AM");

d.dt=i2c_byte_read_frame(0xD0,0x4);
d.m=i2c_byte_read_frame(0xD0,0x5);
d.y=i2c_byte_read_frame(0xD0,0x6);
 lcd_cmd(0xC0);
 lcd_print(d.dt);
 lcd_data('/');
 lcd_print(d.m);
 lcd_data('/');
 lcd_print(d.y);
 lcd_string(" ");
 //lcd_cmd(0xCB);
day=i2c_byte_read_frame(0xD0,0x3);
switch(day){
case 1: lcd_string("sun");break;
case 2: lcd_string("mon");break;
case 3: lcd_string("tue");break;
case 4: lcd_string("wed");break;
case 5: lcd_string("thu");break;
case 6: lcd_string("fri");break;
case 7: lcd_string("sat");break;
 }
}

/*if interrupt triggered*/
while(flag==1)
{
while(c==0){
if(SW1==0){
 while(SW1==0);
  h++;
 if(h>12)
  h=1;
 lcd_cmd(0xC0);
 hh=hex(h);
 lcd_print(hh);
 hh|=(1<<6);
 i2c_byte_write(0xD0,0x2,hh);
 }
 if(SW2==0){
 //next para setting
 while(SW2==0);
  c++;
 lcd_cmd(0x01);
 lcd_cmd(0x80);
 lcd_string("Enter AM/PM");
 lcd_cmd(0xC0);
 lcd_string("AM");
 }
}
 while(c==1)
 {
  if(SW1==0){
  while(SW1==0);
  a=!a;
  }
  lcd_cmd(0xC0);
  if(a==1){
   hh=hh|(1<<5);
   lcd_string("PM");
  }
  else
   lcd_string("AM");
  i2c_byte_write(0xD0,0x2,hh);
  if(SW2==0){
   while(SW2==0);
   c++;
  lcd_cmd(0x01);
  lcd_cmd(0x80);
  lcd_string("Enter min");
  lcd_cmd(0xC0);
  lcd_string("00");
  }
}
 while(c==2){
  if(SW1==0){
   while(SW1==0);
   m++;
   if(m>59)
    m=0;
   lcd_cmd(0xC0);
   mm=hex(m);
   lcd_print(mm);
   i2c_byte_write(0xD0,0x1,mm);
  }
  if(SW2==0){
   while(SW2==0);
   c++;
   lcd_cmd(0x01);
   lcd_cmd(0x80);
   lcd_string("Enter sec");
   lcd_cmd(0xC0);
   lcd_string("00");
   }
 }
 while(c==3){
   if(SW1==0){
     while(SW1==0);
     s++;
     if(s>59)
      s=0;
     lcd_cmd(0xC0);
     ss=hex(s);
     lcd_print(ss);
     i2c_byte_write(0xD0,0x0,ss);
    }
    if(SW2==0){
     while(SW2==0);
	 c++;
     lcd_cmd(0x01);
     lcd_cmd(0x80);
     lcd_string("Enter year");
     lcd_cmd(0xC0);
     lcd_string("00");
    }
 }
 while(c==4){
  if(SW1==0){
   while(SW1==0);
   y++;
    if(y>99)
      y=0;
	  if(y%4==0)
	   year=1;
	  else
	   year=0;
    lcd_cmd(0xC0);
    yy=hex(y);
    lcd_print(yy);
    i2c_byte_write(0xD0,0x6,yy);
   }
    if(SW2==0){
      c++;
     while(SW2==0);
     lcd_cmd(0x01);
     lcd_cmd(0x80);
     lcd_string("Enter month");
     lcd_cmd(0xC0);
     lcd_string("01"); 
    }
  }
  while(c==5){
  if(SW1==0){
   while(SW1==0);
   mo++;
    if(mo>12)
      mo=1;
    lcd_cmd(0xC0);
	if(mo==2 && year==1)
	date=29;
	else if(mo==2)
	date=28;
	else if(mo==4||mo==6||mo==9||mo==11)
	date=30;
	else date=31;
    mon=hex(mo);
    lcd_print(mon);
    i2c_byte_write(0xD0,0x5,mon);
   }
   if(SW2==0){
      c++;
     while(SW2==0);
     lcd_cmd(0x01);
     lcd_cmd(0x80);
     lcd_string("Enter date");
     lcd_cmd(0xC0);
     lcd_string("01"); 
    }
  }
  while(c==6){
  if(SW1==0){
   while(SW1==0);
   dd++;
    if(dd>date)
      dd=1;
    lcd_cmd(0xC0);
    ddt=hex(dd);
    lcd_print(ddt);
    i2c_byte_write(0xD0,0x4,ddt);
   }
   if(SW2==0){
     while(SW2==0);
	 c++;
     lcd_cmd(0x01);
     lcd_cmd(0x80);
     lcd_string("Enter day");
     lcd_cmd(0xC0);
     lcd_string("01 sun"); 
    }
  }
  while(c==7){
  if(SW1==0){
   while(SW1==0);
   dow++;
    if(dow>7)
      dow=1;
    lcd_cmd(0xC0);
     doww=hex(dow);
    lcd_print(doww);
    switch(dow){
    case 1: lcd_string(" sun");break;
    case 2: lcd_string(" mon");break;
    case 3: lcd_string(" tue");break;
    case 4: lcd_string(" wed");break;
    case 5: lcd_string(" thu");break;
    case 6: lcd_string(" fri");break;
    case 7: lcd_string(" sat");break;
    }
	i2c_byte_write(0xD0,0x3,doww);
   }
    if(SW2==0){
     while(SW2==0);
	 c++;
	 lcd_string("time set");
     flag=0;
    }
  }
 }
}
}//main



