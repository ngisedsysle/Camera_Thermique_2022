// from		https://brtchip.com/wp-content/uploads/Support/Documentation/Application_Notes/ICs/EVE/AN_312-FT800-Example-with-ARM.pdf
// modified for		ELSYS DESIGN

#ifndef __LCD_H_
#define __LCD_H_
#pragma once

#include "FT801.h"
#include "stm32h7xx_hal.h"
#include "logo.h"

// AN312 FT801 screen size
#define LCD_QVGA																			// VM800B/C 3.5" QVGA - 320x240
//#define LCD_WQVGA																				// VM800B/C 4.3" & 5.0" WQVGA - 480x272


typedef struct{
	SPI_HandleTypeDef *hspi;
	unsigned char id;
	uint16_t lcdWidth;					// Active width of LCD display
	uint16_t lcdHeight;					// Active height of LCD display
	uint16_t lcdHcycle;					// Total number of clocks per line
	uint16_t lcdHoffset;				// Start of active line
	uint16_t lcdHsync0;					// Start of horizontal sync pulse
	uint16_t lcdHsync1;					// End of horizontal sync pulse
	uint16_t lcdVcycle;					// Total number of lines per screen
	uint16_t lcdVoffset;				// Start of active screen
	uint16_t lcdVsync0;					// Start of vertical sync pulse
	uint16_t lcdVsync1;					// End of vertical sync pulse
	uint16_t lcdPclk;					// Pixel Clock
	uint8_t lcdSwizzle;				// Define RGB output pins
	uint8_t lcdPclkpol;				// Define active edge of PCLK
	uint8_t lcdcspread;
	uint8_t gpio;
	uint32_t ramAddr;
	uint16_t cmd_offset;
}LCD;



void LCD_reset(void);
void LCD_wakeup(LCD *This);
void LCD_displayInit(LCD *This);
void LCD_touchInit(LCD *This);
void LCD_audioInit(LCD *This);
void LCD_Init(LCD *This, SPI_HandleTypeDef *hspi);
uint8_t LCD_ID(LCD *This);
void LCD_displayLogo(LCD *This);
void LCD_startFrame(LCD *This);
void LCD_text(LCD *This, uint16_t x, uint16_t y, uint16_t font, uint16_t options, char* text);
void LCD_number(LCD *This, uint16_t x, uint16_t y, uint16_t font, uint16_t options, uint32_t number);
void LCD_gradient(LCD *This, uint8_t x, uint8_t y, uint32_t startColor,  uint32_t endColor, uint16_t width, uint16_t height);
void LCD_displayIR(LCD *This, uint16_t *IRmap);
void LCD_endFrame(LCD *This);
uint32_t LCD_readTouch(LCD *This);
void LCD_calibration(LCD *This);
void LCD_button(LCD *This, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t font, uint16_t options);
void LCD_cross(LCD *This, uint16_t center, uint16_t scale);
#endif /* __LCD_H_ */
