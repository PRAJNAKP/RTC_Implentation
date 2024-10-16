/* UART0 Driver*/

#include<lpc21xx.h>

void uart0_init(unsigned int baud)
{
unsigned int result,pclk;
int a[]={15,60,30,15,15};
pclk=a[VPBDIV]*1000000;

result=pclk/(16*baud);
PINSEL0|=0x5;//TXD0 & RXD0 functionality selection
U0LCR=0x83;
U0DLL=result&0xFF;//97;//(U0DLL=Pclk/16*baud);
U0DLM=(result>>8)&0xFF;//0;
U0LCR=0x03;

}

/*UART0 Tx function*/
#define THRE ((U0LSR>>5)&1)//THRE flag Line Status Register
void uart0_tx(unsigned char data)
{
U0THR=data;
while(THRE==0);
}


/*UART0  Tx String function*/
void uart0_tx_string(char *ptr)
{
while(*ptr)
{
U0THR=*ptr;
while(THRE==0);
ptr++;
}
}


/*uart0 Tx integer function*/
void uart0_tx_integer(int num){
int a[10],i;
if(num==0)
uart0_tx('0');
if(num<0){
uart0_tx('-');
num=-num;
}
i=0;
while(num>0){
a[i]=(num%10)+48;
num/=10;
i++;
}
for(i=i-1;i>=0;i--)
uart0_tx(a[i]);
}

/*uart0_float function*/
void uart0_tx_float(float f){
int i;
//if(f==0)
//uart0_tx('0');
if(f<0){
uart0_tx('-');
f=-f;
}
i=f;
uart0_tx_integer(i);
i=(f-i)*100;
uart0_tx('.');
uart0_tx_integer(i);
}



/*UART0 Rx Function*/
#define RDR (U0LSR&1)//RBR flag on Line Status Reg
unsigned char uart0_rx(void)
{
while(RDR==0);

return U0RBR;
}

/*uart Rx string fun*/
void uart0_rx_string(char *ptr,char max_bytes){
int i;
for(i=0;i<max_bytes;i++){
while(RDR==0);
ptr[i]=U0RBR;
if(ptr[i]=='\r')   //wait for enter key
break;
}
 ptr[i]='\0';  //adding null char
}



