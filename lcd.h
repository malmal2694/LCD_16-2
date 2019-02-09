#include "stm32f10x_gpio.h"

#define LCD_FREQ 22000000// 18MHz

#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

// Initializes the GPIO for LCD
typedef struct LCD_GPIO{
	GPIO_TypeDef *LCD_PORT;
	uint16_t LCD_PinBit4;
	uint16_t LCD_PinBit5;
	uint16_t LCD_PinBit6;
	uint16_t LCD_PinBit7;
	uint16_t LCD_PinRS;
	uint16_t LCD_PinRW;
	uint16_t LCD_PinE;
}LCD_GPIOPins;

void LCD_4BitInit(LCD_GPIOPins *LCD);
//void Set_PIN_LCD(char Value);
//void Reset_PIN_LCD(char Value);
void LCD_SendData(char data, char dataType);
void LCD_SendCommand(char command);
void LCD_PutChar(char Character);
void LCD_Printf(char s[]);

void LCD_Clear();
void LCD_Home();
void LCD_XY(char x, char y);

#endif
