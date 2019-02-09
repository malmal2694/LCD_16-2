#include "lcd.h"
#include "stm32f10x_gpio.h"
#include <string.h>

#define OP Out_PP
#define D1 5// 2ms (6000)
#define D2 300000//60000
#define D3 10// 10ms (30000)
#define D4 4000000 //4000

#define LCD_SEND_CHAR 1
#define LCD_SEND_COMMAND 0

// Global structure
struct LCD_GPIO *LCD_PinOut;

void LCD_Delay(__uint16_t Time_ms){
	while(Time_ms--)
		for (__uint16_t i = 0; i <= (LCD_FREQ / 10000); i++);// wait 1ms
};

/********* function for LC 8bit initilize **************/
void LCD_4BitInit(LCD_GPIOPins *LCD){
	// Set the structure for the pins that connect to LCD
	LCD_PinOut = LCD;
	GPIO_InitTypeDef LCD_GPIOInitTypeDef;
	LCD_GPIOInitTypeDef.GPIO_Mode = GPIO_Mode_Out_PP;
	LCD_GPIOInitTypeDef.GPIO_Pin = LCD->LCD_PinBit4 | LCD->LCD_PinBit5 | LCD->LCD_PinBit6 | LCD->LCD_PinBit7 |
			LCD->LCD_PinE | LCD->LCD_PinRS | LCD->LCD_PinRW;
	LCD_GPIOInitTypeDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD->LCD_PORT, &LCD_GPIOInitTypeDef);

	LCD_SendCommand(0x2C);// Send command to LCD for initialize LCD as 4 bit (0x38)
	LCD_SendCommand(0x0F);// Send command to LCD for Display ON
	LCD_Clear();// Send command to LCD for clear Display
	LCD_SendCommand(0x0E);// Send command to LCD for clear Display
	LCD_XY(1, 0);
//	LCD_SendCommand(192);// Cursor go to row 1 & column 1 // 0x80
};

/************ send command to LCD *************/
void LCD_SendCommand(char command)
{
	LCD_SendData(command, LCD_SEND_COMMAND);// Send the command to the LCD
//	Set_PIN_LCD(PIN_LCD,command); // set Pins value
//	Reset_PIN(PIN_LCD->PORT_RW,PIN_LCD->PIN_RW); // reset Pin RW
//	Reset_PIN(PIN_LCD->PORT_RS,PIN_LCD->PIN_RS); // reset Pin RS
//	Set_PIN(PIN_LCD->PORT_E,PIN_LCD->PIN_E); // set Pin Enable LCD
//	LCD_Delay(D1);// wait 2ms
//	Reset_PIN(PIN_LCD->PORT_E,PIN_LCD->PIN_E); // Reset Pin Enable LCD
	LCD_Delay(D3);// wait 2ms
//	Reset_PIN_LCD(PIN_LCD,command); // reset Pins data
};

/************ send Character to the LCD *************/
void LCD_PutChar(char character){
//	GPIO_WriteBit(LCD_PinOut->LCD_PORT, LCD_PinOut->LCD_PinRS, Bit_SET);// Set the RS pin
//	GPIO_WriteBit(LCD_PinOut->LCD_PORT ,LCD_PinOut->LCD_PinRW, Bit_RESET); // Reset the RW pin
	LCD_SendData(character, LCD_SEND_CHAR);// Send the character that displays on the LCD
//	Set_PIN(PIN_LCD->PORT_E,PIN_LCD->PIN_E); // set pin E
//	LCD_Delay(D1);// wait 2ms
//	Reset_PIN(PIN_LCD->PORT_E,PIN_LCD->PIN_E); // reset pin Enable lcd
	LCD_Delay(D3);// wait 10ms
//	Reset_PIN_LCD(PIN_LCD,Char); // reset pins data
};

/**************** Send String to LCD character *****************/
void LCD_Printf(char s[]){
	char i = strlen(s); // variable i == number length string
	char c;
	for(c = 0;c < i;c++){
		LCD_PutChar(s[c]); // send character
	}
};

/************** command Ready for LCD character **************/
void LCD_Clear(){
	LCD_SendCommand(0x01); // clear LCD character
};

void LCD_Home(){
	LCD_SendCommand(0x02); // return Home
};

void LCD_XY(char x, char y){
	if( (y == 1) && ( (x >= 0) && (x <= 16) ) ){
		LCD_SendCommand(128 | x);
	}
	else if(y == 2 && ( (x >= 0 && x <= 16) ) ){
		LCD_SendCommand( (128 | (x + 64) ));
	}
};

void LCD_SendData(char data, char dataType)
{
	if (dataType == LCD_SEND_CHAR){
		GPIO_WriteBit(LCD_PinOut->LCD_PORT, LCD_PinOut->LCD_PinRS, Bit_SET);// Set the RS pin
	}else {// (dataType == LCD_SEND_COMMAND)
		GPIO_WriteBit(LCD_PinOut->LCD_PORT, LCD_PinOut->LCD_PinRS, Bit_RESET);// Reset the RS pin
	}
	GPIO_WriteBit(LCD_PinOut->LCD_PORT ,LCD_PinOut->LCD_PinRW, Bit_RESET);// Reset the RW pin

	GPIO_WriteBit(LCD_PinOut->LCD_PORT, LCD_PinOut->LCD_PinBit7, data & (char)(1 << 7));
	GPIO_WriteBit(LCD_PinOut->LCD_PORT, LCD_PinOut->LCD_PinBit6, data & (char)(1 << 6));
	GPIO_WriteBit(LCD_PinOut->LCD_PORT, LCD_PinOut->LCD_PinBit5, data & (char)(1 << 5));
	GPIO_WriteBit(LCD_PinOut->LCD_PORT, LCD_PinOut->LCD_PinBit4, data & (char)(1 << 4));
	GPIO_WriteBit(LCD_PinOut->LCD_PORT, LCD_PinOut->LCD_PinE, Bit_SET);// Enable the enable pin
	LCD_Delay(D1);
	GPIO_WriteBit(LCD_PinOut->LCD_PORT, LCD_PinOut->LCD_PinE, Bit_RESET);// Disable the enable pin

	LCD_Delay(D1);
	GPIO_WriteBit(LCD_PinOut->LCD_PORT, LCD_PinOut->LCD_PinBit7, data & (char)(1 << 3));
	GPIO_WriteBit(LCD_PinOut->LCD_PORT, LCD_PinOut->LCD_PinBit6, data & (char)(1 << 2));
	GPIO_WriteBit(LCD_PinOut->LCD_PORT, LCD_PinOut->LCD_PinBit5, data & (char)(1 << 1));
	GPIO_WriteBit(LCD_PinOut->LCD_PORT, LCD_PinOut->LCD_PinBit4, data & (char)(1 << 0));
	GPIO_WriteBit(LCD_PinOut->LCD_PORT, LCD_PinOut->LCD_PinE, Bit_SET);// Enable the enable pin
	LCD_Delay(D1);
	GPIO_WriteBit(LCD_PinOut->LCD_PORT, LCD_PinOut->LCD_PinE, Bit_RESET);// Disable the enable pin

}
//void Set_PIN_LCD(struct PIN_LCD *PIN_LCD,char Value){
//	if( (Value - 128) >= 0){
//	Set_PIN(PIN_LCD->PORT_BIT7,PIN_LCD->PIN_BIT7); // while (value -128) >=0 set PIN
//	Value = Value - 128;
//	};
//
//	if( (Value - 64) >= 0){
//	Set_PIN(PIN_LCD->PORT_BIT6,PIN_LCD->PIN_BIT6); // while (value -64) >=0 set PIN
//	Value = Value - 64;
//	};
//
//	if( (Value - 32) >= 0){
//	Set_PIN(PIN_LCD->PORT_BIT5,PIN_LCD->PIN_BIT5); // while (value -32) >=0 set PIN
//	Value = Value - 32;
//	};
//
//	if( (Value - 16) >= 0){
//	Set_PIN(PIN_LCD->PORT_BIT4,PIN_LCD->PIN_BIT4); // while (value -16) >=0 set PIN
//	Value = Value - 16;
//	};
//
//	if( (Value - 8) >= 0){
//	Set_PIN(PIN_LCD->PORT_BIT3,PIN_LCD->PIN_BIT3); // while (value -8) >=0 set PIN
//	Value = Value - 8;
//	};
//
//	if( (Value - 4) >= 0){
//	Set_PIN(PIN_LCD->PORT_BIT2,PIN_LCD->PIN_BIT2); // while (value -4) >=0 set PIN
//	Value = Value - 4;
//	};
//
//	if( (Value - 2) >= 0){
//	Set_PIN(PIN_LCD->PORT_BIT1,PIN_LCD->PIN_BIT1); // while (value -2) >=0 set PIN
//	Value = Value - 2;
//	};
//
//	if( (Value - 1) >= 0){
//	Set_PIN(PIN_LCD->PORT_BIT0,PIN_LCD->PIN_BIT0); // while (value -1) >=0 set PIN
//	Value = Value - 1;
//	};
//
//};


//void Reset_PIN_LCD(struct PIN_LCD *PIN_LCD,char Value){
//	if( (Value - 128) >= 0){
//	Reset_PIN(PIN_LCD->PORT_BIT7,PIN_LCD->PIN_BIT7); // while (value -128) >=0 reset PIN
//	Value = Value - 128;
//	};
//
//	if( (Value - 64) >= 0){
//	Reset_PIN(PIN_LCD->PORT_BIT6,PIN_LCD->PIN_BIT6); // while (value -64) >=0 reset PIN
//	Value = Value - 64;
//	};
//
//	if( (Value - 32) >= 0){
//	Reset_PIN(PIN_LCD->PORT_BIT5,PIN_LCD->PIN_BIT5); // while (value -32) >=0 reset PIN
//	Value = Value - 32;
//	};
//
//	if( (Value - 16) >= 0){
//	Reset_PIN(PIN_LCD->PORT_BIT4,PIN_LCD->PIN_BIT4); // while (value -16) >=0 reset PIN
//	Value = Value - 16;
//	};
//
//	if( (Value - 8) >= 0){
//	Reset_PIN(PIN_LCD->PORT_BIT3,PIN_LCD->PIN_BIT3); // while (value -8) >=0 reset PIN
//	Value = Value - 8;
//	};
//
//	if( (Value - 4) >= 0){
//	Reset_PIN(PIN_LCD->PORT_BIT2,PIN_LCD->PIN_BIT2); // while (value -4) >=0 reset PIN
//	Value = Value - 4;
//	};
//
//	if( (Value - 2) >= 0){
//	Reset_PIN(PIN_LCD->PORT_BIT1,PIN_LCD->PIN_BIT1); // while (value -2) >=0 reset PIN
//	Value = Value - 2;
//	};
//
//	if( (Value - 1) >= 0){
//	Reset_PIN(PIN_LCD->PORT_BIT0,PIN_LCD->PIN_BIT0); // while (value -1) >=0 reset PIN
//	Value = Value - 1;
//	};
//
//};
