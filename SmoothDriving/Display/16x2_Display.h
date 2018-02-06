/*
 * _16x2_Display.h
 *
 * Created: 12/3/2017 11:59:58 PM
 *  Author: Matt
 */ 

#ifndef SIXTEENBYTWODISPLAY_H_
#define SIXTEENBYTWODISPLAY_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define LCD_DPRT PORTD //LCD DATA PORT
#define LCD_DDDR DDRD  //LCD DATA DDR
#define LCD_DPIN PIND  //LCD DATA PIN
#define LCD_CPRT PORTD //LCD COMMANDS PORT
#define LCD_CDDR DDRD  //LCD COMMANDS DDR
#define LCD_CPIN PIND  //LCD COMMANDS PIN
#define LCD_RS  2      //LCD RS   PD2 (PC.0)
#define LCD_RW  3      //LCD RW   PD3 (PC.1)
#define LCD_EN  4      //LCD EN   PD4 (PC.2)

#define SHIFT_DDR DDRD
#define SHIFT_PORT PORTD
#define SHIFT_CLK 5	   // PD 5
#define SHIFT_DATA 6   // PD 6

void LCDCommand(unsigned char cmd);
void LCDData(unsigned char data);
void LCDInit();
void LCDSet(char* str);
void LCDAppend(char* str);
void LCDGotoXY(unsigned char x, unsigned char y);
void LCDSetShiftRegister(unsigned char data);
void LCDSetShiftRegisterSlow(unsigned char data);
#endif