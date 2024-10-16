#include<lpc21xx.h>
#include"header.h"

void i2c_init(void){
PINSEL0|=0X50; //p0.2-scl,p0.3-sda
I2SCLH=I2SCLL=75;//100kbps
I2CONSET=(1<<6); //en i2c 7 master mode
}

#define SI ((I2CONSET>>3)&1)
void i2c_byte_write(u8 sa,u8 mr,u8 data){
//generate start cond
I2CONSET=(1<<5); //sta=1
I2CONCLR=(1<<3); //SI=0
while(SI==0) {
uart0_tx_string("above:start\r\n");
}
I2CONCLR=(1<<5); //sta=0
if(I2STAT!=0X8){
uart0_tx_string("ERR:start\r\n");
goto exit;
}

//send sa+w & check ack
I2DAT=sa;//send sa+w
I2CONCLR=(1<<3);
while(SI==0);
if(I2STAT==0X20){
uart0_tx_string("ERR:SA+W\r\n");
goto exit;
}

 //send mr addr & check ack
I2DAT=mr;
I2CONCLR=(1<<3);//si=0
while(SI==0);
if(I2STAT==0X30){
uart0_tx_string("ERR:m/r addr\r\n");
goto exit;
}

//send data & check ack
I2DAT=data;
I2CONCLR=(1<<3);
while(SI==0);
if(I2STAT==0X30){
uart0_tx_string("ERR:data\r\n");
goto exit;
}

//generate stop condition
exit:
I2CONCLR=(1<<3);
I2CONSET=(1<<4);
}
//u8 i2c_byte_read_frame(u8 sa,u8 mr){
u8 i2c_byte_read_frame(u8 sa,u8 mr){
u8 temp;
//generate start cond
I2CONSET=(1<<5);
I2CONCLR=(1<<3);//SI=0
while(SI==0);
I2CONCLR=(1<<5);
if(I2STAT!=0X8){
uart0_tx_string("ERR:start\r\n");
goto exit;
}

//send sa+w & check ack
I2DAT=sa;
I2CONCLR=(1<<3);
while(SI==0);
if(I2STAT==0X20){
uart0_tx_string("ERR:SA+W\r\n");
goto exit;
}

 //send mr addr & check ack
I2DAT=mr;
I2CONCLR=(1<<3);
while(SI==0);
if(I2STAT==0X30){
uart0_tx_string("ERR:m/r addr\r\n");
goto exit;
}

//generate restart cond
I2CONSET=(1<<5);
I2CONCLR=(1<<3);//SI=0
while(SI==0);
I2CONCLR=(1<<5);
if(I2STAT!=0X10){
uart0_tx_string("ERR:Restart\r\n");
goto exit;
}

//send SA+R
I2DAT=sa|1;
I2CONCLR=(1<<3);
while(SI==0);
I2CONCLR=(1<<5);
if(I2STAT==0X48){
uart0_tx_string("ERR:SA+R\r\n");
goto exit;
}

//read data
I2CONCLR=(1<<3);
while(SI==0);
temp=I2DAT;

//generate stop condition
exit:
I2CONCLR=(1<<3);
I2CONSET=(1<<4);

//return received data
return temp;
}






