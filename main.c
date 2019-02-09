/* Includes */
#include <stddef.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "Rcc.h"
#include "lcd.h"

void delay(__uint16_t Time_ms);


int main(void)
{

	// set clock 18 MHz for MCU
	RCC_HSEConfig(RCC_HSE_ON);
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_2);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

	delay(3000);

	LCD_GPIOPins LCD_Pins;
	LCD_Pins.LCD_PORT = GPIOB;
	LCD_Pins.LCD_PinBit4 = GPIO_Pin_6;
	LCD_Pins.LCD_PinBit5 = GPIO_Pin_7;
	LCD_Pins.LCD_PinBit6 = GPIO_Pin_9;
	LCD_Pins.LCD_PinBit7 = GPIO_Pin_8;
	LCD_Pins.LCD_PinE = GPIO_Pin_14;
	LCD_Pins.LCD_PinRS = GPIO_Pin_12;
	LCD_Pins.LCD_PinRW = GPIO_Pin_13;
	LCD_4BitInit(&LCD_Pins);
	LCD_Printf("Hello world!");


	LCD_Clear();
	LCD_Printf("Hi Ghasem!!");
}

void delay(__uint16_t Time_ms){
	while(Time_ms--)
		for (__uint16_t i = 0; i <= (LCD_FREQ / 10000); i++);// wait 1ms
};
