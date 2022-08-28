#include "Lcd/lcd.h"
#include "Lepton/lepton.h"
int switch_display = 1;

void LCD_Init(LCD *This, SPI_HandleTypeDef *hspi) {
	This->hspi = hspi;
	This->ramAddr = FT801_RAM_G;
	This->cmd_offset = 0;
	LCD_reset();
	LCD_wakeup(This);

	do {

		This->id = FT801memRead8(This->hspi, REG_ID);

	} while (This->id != 0x7C);

	LCD_displayInit(This);
	LCD_touchInit(This);
	LCD_audioInit(This);

	This->gpio = FT801memRead8(This->hspi, REG_GPIO);
	This->gpio |= 0x80;
	FT801memWrite8(This->hspi, REG_GPIO, This->gpio);
	FT801memWrite8(This->hspi, REG_PCLK, This->lcdPclk);


	//LCD_calibration(This);


}
void LCD_reset(void) {
	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_SET);	// Initial state of SPI CS - high
	HAL_GPIO_WritePin(GPIOD, FT801_PD_N, GPIO_PIN_SET); // Initial state of PD_N - high
	HAL_Delay(20);										// Wait 20ms
	HAL_GPIO_WritePin(GPIOD, FT801_PD_N, GPIO_PIN_RESET); // Reset FT801
	HAL_Delay(20);									// Wait 20ms
	HAL_GPIO_WritePin(GPIOD, FT801_PD_N, GPIO_PIN_SET); 	// FT801 is awake
	HAL_Delay(20);
}

void LCD_wakeup(LCD *This) {
	FT801cmdWrite(This->hspi, FT801_ACTIVE);
	HAL_Delay(5);
	FT801cmdWrite(This->hspi, FT801_CLKEXT);
	HAL_Delay(5);
	FT801cmdWrite(This->hspi, FT801_CLK48M);
	HAL_Delay(5);
}

void LCD_displayInit(LCD *This) {

#ifdef LCD_QVGA																					// QVGA display parameters
	This->lcdWidth = 320;						// Active width of LCD display
	This->lcdHeight = 240;						// Active height of LCD display
	This->lcdHcycle = 408;					// Total number of clocks per line
	This->lcdHoffset = 70;								// Start of active line
	This->lcdHsync0 = 0;					// Start of horizontal sync pulse
	This->lcdHsync1 = 10;						// End of horizontal sync pulse
	This->lcdVcycle = 263;					// Total number of lines per screen
	This->lcdVoffset = 13;							// Start of active screen
	This->lcdVsync0 = 0;						// Start of vertical sync pulse
	This->lcdVsync1 = 2;						// End of vertical sync pulse
	This->lcdPclk = 8;											// Pixel Clock
	This->lcdSwizzle = 2;							// Define RGB output pins
	This->lcdPclkpol = 0;						// Define active edge of PCLK
	This->lcdcspread = 1;						// output clock spreading enable
#endif

#ifdef LCD_WQVGA																				// WQVGA display parameters
	This->lcdWidth   = 480;																			// Active width of LCD display
	This->lcdHeight  = 272;																			// Active height of LCD display
	This->lcdHcycle  = 548;																			// Total number of clocks per line
	This->lcdHoffset = 43;																			// Start of active line
	This->lcdHsync0  = 0;																				// Start of horizontal sync pulse
	This->lcdHsync1  = 41;																			// End of horizontal sync pulse
	This->lcdVcycle  = 292;																			// Total number of lines per screen
	This->lcdVoffset = 12;																			// Start of active screen
	This->lcdVsync0  = 0;																				// Start of vertical sync pulse
	This->lcdVsync1  = 10;																			// End of vertical sync pulse
	This->lcdPclk    = 5;																				// Pixel Clock
	This->lcdSwizzle = 0;																				// Define RGB output pins
	This->lcdPclkpol = 1;																				// Define active edge of PCLK
	This->lcdcspread = 1;						// output clock spreading enable
#endif

	FT801memWrite16(This->hspi, REG_HSIZE, This->lcdWidth);
	FT801memWrite16(This->hspi, REG_HCYCLE, This->lcdHcycle);
	FT801memWrite16(This->hspi, REG_HOFFSET, This->lcdHoffset);
	FT801memWrite16(This->hspi, REG_HSYNC0, This->lcdHsync0);
	FT801memWrite16(This->hspi, REG_HSYNC1, This->lcdHsync1);
	FT801memWrite16(This->hspi, REG_VSIZE, This->lcdHeight);
	FT801memWrite16(This->hspi, REG_VCYCLE, This->lcdVcycle);
	FT801memWrite16(This->hspi, REG_VOFFSET, This->lcdVoffset);
	FT801memWrite16(This->hspi, REG_VSYNC0, This->lcdVsync0);
	FT801memWrite16(This->hspi, REG_VSYNC1, This->lcdVsync1);
	FT801memWrite8(This->hspi, REG_SWIZZLE, This->lcdSwizzle);
	FT801memWrite8(This->hspi, REG_PCLK_POL, This->lcdPclkpol);
	FT801memWrite8(This->hspi, REG_CSPREAD, This->lcdcspread);

}

void LCD_touchInit(LCD *This){
  FT801memWrite8(This->hspi, REG_TOUCH_MODE, 3);									// on mode
  FT801memWrite8(This->hspi, REG_INT_EN, 1);
  FT801memWrite8(This->hspi, REG_INT_MASK, 2);
}
void LCD_audioInit(LCD *This){
  FT801memWrite8(This->hspi, REG_VOL_PB, ZERO);											// turn recorded audio volume down
  FT801memWrite8(This->hspi, REG_VOL_SOUND, ZERO);									// turn synthesizer volume down
  FT801memWrite16(This->hspi, REG_SOUND, 0x6000);										// set synthesizer to mute
}

uint32_t LCD_readTouch(LCD *This)
{
	uint32_t xy = FT801memRead32(This->hspi, REG_CTOUCH_SCREEN_XY);
	if(switch_display){
		LCD_displayLogo(This);
	}
	else{
		LCD_startFrame(This);
		FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, COLOR_RGB(0x00, 0x00, 0xff));
		LCD_endFrame(This);
	}
	switch_display = !switch_display;
	FT801memRead8(This->hspi, REG_INT_FLAGS);			// clear interrupt flags
	return xy;
}

void LCD_displayLogo(LCD *This)
{


	FT801memWrite32(This->hspi, FT801_RAM_DL, CLEAR(1, 1, 1));
	FT801memWrite32(This->hspi, FT801_RAM_DL + 4, BEGIN(FT801_BITMAPS));
	//circular buffering
	if(This->ramAddr + 153600 > FT801_RAM_G_SIZE){
		This->ramAddr = FT801_RAM_G;
	}
	FT801memWrite32(This->hspi, FT801_RAM_DL + 8, BITMAP_SOURCE(This->ramAddr));
	This->ramAddr = FT801memWriteRAM32(This->hspi,This->ramAddr,logo,153600);
	FT801memWrite32(This->hspi, FT801_RAM_DL + 12, BITMAP_LAYOUT(FT801_RGB565,2*This->lcdWidth, This->lcdHeight));
	FT801memWrite32(This->hspi, FT801_RAM_DL + 16, BITMAP_SIZE(FT801_NEAREST,FT801_BORDER,FT801_BORDER,This->lcdWidth,This->lcdHeight));
	FT801memWrite32(This->hspi, FT801_RAM_DL + 20, VERTEX2II(0,0,0,0));
	FT801memWrite32(This->hspi, FT801_RAM_DL + 24, END());
	FT801memWrite32(This->hspi, FT801_RAM_DL + 28, DISPLAY()); // display the image
	FT801memWrite8(This->hspi, REG_DLSWAP, FT801_DLSWAP_FRAME);

}
void LCD_number(LCD *This, uint16_t x, uint16_t y, uint16_t font, uint16_t options, uint32_t number)
{
//	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, BEGIN(FT801_BITMAPS));
//	This->cmd_offset += 4;
//	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, SCISSOR_XY(x,y) );
//	This->cmd_offset += 4;
//	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, SCISSOR_SIZE( 100 , 100 ));
//	This->cmd_offset += 4;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, CMD_NUMBER);
	This->cmd_offset += 4;
	FT801memWrite16(This->hspi, FT801_RAM_CMD + This->cmd_offset, x);
	This->cmd_offset += 2;
	FT801memWrite16(This->hspi, FT801_RAM_CMD + This->cmd_offset, y);
	This->cmd_offset += 2;
	FT801memWrite16(This->hspi, FT801_RAM_CMD + This->cmd_offset, font);
	This->cmd_offset += 2;
	FT801memWrite16(This->hspi, FT801_RAM_CMD + This->cmd_offset, options);
	This->cmd_offset += 2;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, number);
	This->cmd_offset += 4;

}
void LCD_text(LCD *This, uint16_t x, uint16_t y, uint16_t font, uint16_t options, char* text)
{
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, CMD_TEXT);
	This->cmd_offset += 4;
	FT801memWrite16(This->hspi, FT801_RAM_CMD + This->cmd_offset, 0);
	This->cmd_offset += 2;
	FT801memWrite16(This->hspi, FT801_RAM_CMD + This->cmd_offset, 0);
	This->cmd_offset += 2;
	FT801memWrite16(This->hspi, FT801_RAM_CMD + This->cmd_offset, font);
	This->cmd_offset += 2;
	FT801memWrite16(This->hspi, FT801_RAM_CMD + This->cmd_offset, options);
	This->cmd_offset += 2;
	uint16_t text_len = strlen(text);
	for(int i = 0 ; i < text_len ; i++ , This->cmd_offset += 1)
	{
		FT801memWrite8(This->hspi, FT801_RAM_CMD + This->cmd_offset, text[i] );
	}
	FT801memWrite8(This->hspi, FT801_RAM_CMD + This->cmd_offset, 0);
	This->cmd_offset += 1;
}
void LCD_cross(LCD *This, uint16_t center, uint16_t scale)
{

	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, BEGIN(LINES) );
	This->cmd_offset += 4;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, LINE_WIDTH(10));
	This->cmd_offset += 4;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, VERTEX2F(16*(center - scale), 16*center ));	//draw unit = 1/16 pixel
	This->cmd_offset += 4;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, VERTEX2F(16*(center + scale), 16*center ));
	This->cmd_offset += 4;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, VERTEX2F(16*center, 16*(center - scale) ));	//draw unit = 1/16 pixel
	This->cmd_offset += 4;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, VERTEX2F(16*center, 16*(center + scale) ));
	This->cmd_offset += 4;


//	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, BEGIN(LINES) );
//	This->cmd_offset += 4;
//	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, VERTEX2II(x, y, 0, 0) );
//	This->cmd_offset += 4;
//	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, VERTEX2II(110, y, 0, 0));
//	This->cmd_offset += 4;
}
void LCD_startFrame(LCD *This)
{
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, CMD_DLSTART);
	This->cmd_offset += 4;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, CLEAR_COLOR_RGB(64,64,64));
	This->cmd_offset += 4;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, CLEAR(1, 1, 1));
	This->cmd_offset += 4;

//	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, COLOR_RGB(0xff, 0xff, 0xff));
//	This->cmd_offset += 4;
}
void LCD_displayIR(LCD *This, uint16_t *IRmap)
{
	if(This->ramAddr + LEPTON_FRAME_SIZE > FT801_RAM_G_SIZE){
		This->ramAddr = FT801_RAM_G;
	}
	This->ramAddr = FT801memWriteRAM8(This->hspi,0,IRmap, LEPTON_FRAME_SIZE );

	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, BEGIN(FT801_BITMAPS));
	This->cmd_offset += 4;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, BITMAP_SOURCE(0));
	This->cmd_offset += 4;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, BITMAP_LAYOUT(FT801_RGB565,2*LEPTON_FRAME_WIDTH, LEPTON_FRAME_HEIGHT));
	This->cmd_offset += 4;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, BITMAP_TRANSFORM_A(2*LEPTON_FRAME_WIDTH));
	This->cmd_offset += 4;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, BITMAP_TRANSFORM_E(2*LEPTON_FRAME_HEIGHT));
	This->cmd_offset += 4;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, BITMAP_SIZE(FT801_NEAREST,FT801_BORDER,FT801_BORDER, 2*LEPTON_FRAME_WIDTH, 2*LEPTON_FRAME_HEIGHT));
	This->cmd_offset += 4;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, VERTEX2II(0,0,0,0));
	This->cmd_offset += 4;

}
void LCD_gradient(LCD *This, uint8_t x, uint8_t y, uint32_t startColor,  uint32_t endColor, uint16_t width, uint16_t height)
{
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, SCISSOR_XY(x,y) );
	This->cmd_offset += 4;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, SCISSOR_SIZE( width , height ));
	This->cmd_offset += 4;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, CMD_GRADIENT);
	This->cmd_offset += 4;
	FT801memWrite16(This->hspi, FT801_RAM_CMD + This->cmd_offset, x);
	This->cmd_offset += 2;
	FT801memWrite16(This->hspi, FT801_RAM_CMD + This->cmd_offset, y);
	This->cmd_offset += 2;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, startColor);
	This->cmd_offset += 4;
	FT801memWrite16(This->hspi, FT801_RAM_CMD + This->cmd_offset, 2*width);
	This->cmd_offset += 2;
	FT801memWrite16(This->hspi, FT801_RAM_CMD + This->cmd_offset, y);
	This->cmd_offset += 2;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, endColor);
	This->cmd_offset += 4;
}
void LCD_endFrame(LCD *This)
{
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, END());
	This->cmd_offset += 4;
	FT801memWrite16(This->hspi, REG_CMD_WRITE, This->cmd_offset);
	//This->cmd_offset += 4;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, DISPLAY());
	This->cmd_offset += 4;
	FT801memWrite16(This->hspi, REG_CMD_WRITE, This->cmd_offset);
	FT801memWrite8(This->hspi, REG_DLSWAP, FT801_DLSWAP_FRAME);
	This->cmd_offset  = 0;
}
void LCD_calibration(LCD *This)
{
	LCD_startFrame(This);
	//LCD_text(This, 30, 30,37, 0, "Please tap on the dot");
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, CMD_CALIBRATE );
	This->cmd_offset += 4;
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, 0);
	This->cmd_offset += 4;
	LCD_endFrame(This);
	for(int i = 0; i < 2; i++) while((FT801memRead8(This->hspi, REG_INT_FLAGS) & 0x4) == 0);		// waiting for 3 touches
}
void LCD_button(LCD *This, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t font, uint16_t options)
{
	FT801memWrite32(This->hspi, FT801_RAM_CMD + This->cmd_offset, CMD_BUTTON);
	This->cmd_offset += 4;
	FT801memWrite16(This->hspi, FT801_RAM_CMD + This->cmd_offset, x);
	This->cmd_offset += 2;
	FT801memWrite16(This->hspi, FT801_RAM_CMD + This->cmd_offset, y);
	This->cmd_offset += 2;
	FT801memWrite16(This->hspi, FT801_RAM_CMD + This->cmd_offset, w);
	This->cmd_offset += 2;
	FT801memWrite16(This->hspi, FT801_RAM_CMD + This->cmd_offset, h);
	This->cmd_offset += 2;
	FT801memWrite16(This->hspi, FT801_RAM_CMD + This->cmd_offset, font);
	This->cmd_offset += 2;
	FT801memWrite16(This->hspi, FT801_RAM_CMD + This->cmd_offset, options);
	This->cmd_offset += 2;
	FT801memWrite8(This->hspi, FT801_RAM_CMD + This->cmd_offset,'\0');
	This->cmd_offset += 1;


}
