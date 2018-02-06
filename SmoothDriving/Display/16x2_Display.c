/*
 * _16x2_Display.c
 *
 * Created: 12/4/2017 12:00:05 AM
 *  Author: Matt
 */ 

 #include "16x2_Display.h"

 void LCDCommand(unsigned char cmd)
 {
	LCDSetShiftRegister(cmd);
	 //LCD_DPRT = cmd;				//send cmd to data port
	LCD_CPRT &= ~(1<<LCD_RS);   //RS = 0 for command
	LCD_CPRT &= ~(1<<LCD_RW);   //RW = 0 for write
	LCD_CPRT |= (1<<LCD_EN);    //EN = 1 for H-to-L pulse
	_delay_us(1);					//wait to make enable wide
	LCD_CPRT &= ~(1<<LCD_EN);   //EN = 0 for H-to_L pulse
	_delay_us(100);				//wait to make enable wide
 }
 void LCDData(unsigned char data)
 {
	LCDSetShiftRegister(data);
	//LCD_DPRT = data;				//send data to data port
	LCD_CPRT |= (1<<LCD_RS);		//RS = 1 for data
	LCD_CPRT &= ~(1<<LCD_RW);   //RW = 0 for write
	LCD_CPRT |= (1<<LCD_EN);		//EN = 1 for H-to-L pulse
	_delay_us(1);					//wait to make enable wide
	LCD_CPRT &= ~(1<<LCD_EN);   //EN = 0 for H-to_L pulse
	_delay_us(100);				//wait to make enable wide
 }
 void LCDInit()
 {
	//LCD_DDDR = 0xFF;
	LCD_CDDR |= (1 << LCD_EN | 1 << LCD_RS | 1 << LCD_RW);
	SHIFT_DDR |= (1 << SHIFT_CLK | 1 << SHIFT_DATA);
	SHIFT_PORT &= ~(1 << SHIFT_CLK);
	LCD_CPRT &=~(1<<LCD_EN);   //LCD_EN = 0
	
	_delay_us(2000);
	//wait for init
	LCDCommand(0x38);   //initialize LCD 2 line, 5x7
	LCDCommand(0x0E);   //display on, cursor on
	LCDCommand(0x01);   //clear LCD
	_delay_us(2000);		 //wait
	LCDCommand(0x06);   //shift cursor right
 }
 void LCDSet(char* str)
 {
	LCDInit(); // Clear screen
	uint8_t currentLine = 1;
	for(unsigned char i = 0; i < 16; i++)
	{
	 	if(str[i] == 0)
	 	{
		 	return;
	 	}
	 	if(i == 8 || str[i] == '\n') // Go to next 8 blocks if string exceeds 8 characters
	 	{
		 	LCDGotoXY(1,2);
	 	}
		LCDData(str[i]);
 	}
 }
 void LCDAppend(char* str)
 {
	unsigned char i = 0; // Append data to current line
	while (str[i]!=0 && i < 16)
	{
		if(str[i] == '\n')
			LCDGotoXY(1,2);
		else
			LCDData(str[i]); i++;
	}
 }
 void LCDGotoXY(unsigned char x, unsigned char y)
 {
	unsigned char firstCharAdr[] = {0x80, 0xC0};   // locations of the first character of each line
 
	LCDCommand(firstCharAdr[y-1] + x-1);
	_delay_us(100);
 }
 void LCDSetShiftRegister(unsigned char data)
 {
	uint8_t i;
	for(i = 0; i < 8; i++)
	{
		SHIFT_PORT &= ~(1 << SHIFT_DATA | 1 << SHIFT_CLK); // Set clk and data to 0
		if(i > SHIFT_DATA)
		{
			SHIFT_PORT |= ((1 << SHIFT_DATA) & (data >> (SHIFT_DATA - 1)));
		}
		else
		{
			SHIFT_PORT|= ((1 << SHIFT_DATA) & (data << (SHIFT_DATA - i)));
		}
		
		SHIFT_PORT |= (1 << SHIFT_CLK);  // pulse clock
	}
 }
 void LCDSetShiftRegisterSlow(unsigned char data)
 {
	 uint8_t i;
	 for(i = 0; i < 8; i++)
	 {
		 SHIFT_PORT &= ~(1 << SHIFT_DATA | 1 << SHIFT_CLK); // Set clk and data to 0
		 if(i > SHIFT_DATA)
		 {
			 SHIFT_PORT |= ((1 << SHIFT_DATA) & (data >> (SHIFT_DATA - 1)));
		 }
		 else
		 {
			 SHIFT_PORT|= ((1 << SHIFT_DATA) & (data << (SHIFT_DATA - i)));
		 }
	 
		 SHIFT_PORT |= (1 << SHIFT_CLK);  // pulse clock
		 _delay_ms(1000);
	 }
 }