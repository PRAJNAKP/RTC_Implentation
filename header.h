 
typedef  unsigned char u8;
typedef  unsigned int u32;
typedef  signed char s8;
typedef  signed int s32;

/*delay functions*/
extern void delay_ms(unsigned int);
extern void delay_sec(unsigned int);


/*8 bit and 4 bit mode lcd functions*/
extern void lcd_data(unsigned char);
extern void lcd_cmd(unsigned char);
extern void lcd_init(void);
extern void lcd_string(char *);
extern void lcd_itoa(int);
void lcd_ftoa(float);
extern void lcd_cgram(void);
extern void lcd_print(u8 );

/*uart tx functions*/
extern void uart0_init(unsigned int baud);
extern void uart0_tx(unsigned char data);
extern void uart0_tx_string(char *ptr);
extern void uart0_tx_integer(int);
extern void uart0_tx_float(float);
/*uart rx functions*/
extern unsigned char uart0_rx(void);
extern void uart0_rx_string(char *ptr,char);

/*ADC functions*/
u32 adc_read(u8 );
void adc_init(void);


/*SPI functions*/
#define CS0 (1<<7)
void spi0_init(void);
u8 spi0(u8 data);
u32 mcp3204_read(u8 );

/*i2c functions*/
extern void i2c_init(void);
extern void i2c_byte_write(u8,u8,u8);
u8 i2c_byte_read_frame(u8,u8);
//u8 i2c_byte_read(u8,u8);










