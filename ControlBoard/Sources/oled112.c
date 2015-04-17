/******************************************************************************
 * Copyright (C) 2015 A. C. Open Hardware Ideas Lab
 *
 * Authors:
 *  Alessio Paolucci <a.paolucci89@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

/**
 * @file libohigrove/oled112.c
 * @author Alessio Paolucci <a.paolucci89@gmail.com>
 * @brief
 */

#include "oled112.h"

#define OHIGROVEOLED112_ADDRESS     0x78


#define OHIGROVEOLED112_MAX_X       127
#define OHIGROVEOLED112_MAX_Y       63

typedef enum
{
	OHIGROVEOLED112_CMD_COMMAND_MODE      = 0x80,
	OHIGROVEOLED112_CMD_DATA_MODE         = 0x40,
	OHIGROVEOLED112_CMD_DISPLAY_OFF       = 0xAE,
	OHIGROVEOLED112_CMD_DISPLAY_ON        = 0xAF,
	OHIGROVEOLED112_CMD_NORMAL_DISPLAY    = 0xA6,
	OHIGROVEOLED112_CMD_INVERSE_DISPLAY   = 0xA7,
	OHIGROVEOLED112_CMD_ACTIVATE_SCROLL   = 0x2F,
	OHIGROVEOLED112_CMD_DEACTIVATE_SCROLL = 0x2E,
	OHIGROVEOLED112_CMD_SET_BRIGHTNESS    = 0x81,
    OHIGROVEOLED112_CMD_ADDRESS_MODE      = 0x20,

} OhiGroveOled112_Command;

typedef enum
{
    OHIGROVEOLED112_ADDRESSMODE_HORIZONTAL = 0x00,
    OHIGROVEOLED112_ADDRESSMODE_PAGE       = 0x02,

} OhiGroveOled112_AddressMode;


static OhiGroveOled112_AddressMode OhiGroveOled112_addressMode;

/*
 * This font be freely used without any restriction(It is placed in public domain)
 *
 * 8x8 Font ASCII 32 - 127 Implemented
 * Users can modify this to support more characters(glyphs)
 * BasicFont is placed in code memory.
 */
const unsigned char OhiGroveOled112_basicFont[][8] =
{
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x5F,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x07,0x00,0x07,0x00,0x00,0x00},
  {0x00,0x14,0x7F,0x14,0x7F,0x14,0x00,0x00},
  {0x00,0x24,0x2A,0x7F,0x2A,0x12,0x00,0x00},
  {0x00,0x23,0x13,0x08,0x64,0x62,0x00,0x00},
  {0x00,0x36,0x49,0x55,0x22,0x50,0x00,0x00},
  {0x00,0x00,0x05,0x03,0x00,0x00,0x00,0x00},
  {0x00,0x1C,0x22,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x41,0x22,0x1C,0x00,0x00,0x00,0x00},
  {0x00,0x08,0x2A,0x1C,0x2A,0x08,0x00,0x00},
  {0x00,0x08,0x08,0x3E,0x08,0x08,0x00,0x00},
  {0x00,0xA0,0x60,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x08,0x08,0x08,0x08,0x08,0x00,0x00},
  {0x00,0x60,0x60,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x20,0x10,0x08,0x04,0x02,0x00,0x00},
  {0x00,0x3E,0x51,0x49,0x45,0x3E,0x00,0x00},
  {0x00,0x00,0x42,0x7F,0x40,0x00,0x00,0x00},
  {0x00,0x62,0x51,0x49,0x49,0x46,0x00,0x00},
  {0x00,0x22,0x41,0x49,0x49,0x36,0x00,0x00},
  {0x00,0x18,0x14,0x12,0x7F,0x10,0x00,0x00},
  {0x00,0x27,0x45,0x45,0x45,0x39,0x00,0x00},
  {0x00,0x3C,0x4A,0x49,0x49,0x30,0x00,0x00},
  {0x00,0x01,0x71,0x09,0x05,0x03,0x00,0x00},
  {0x00,0x36,0x49,0x49,0x49,0x36,0x00,0x00},
  {0x00,0x06,0x49,0x49,0x29,0x1E,0x00,0x00},
  {0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00},
  {0x00,0x00,0xAC,0x6C,0x00,0x00,0x00,0x00},
  {0x00,0x08,0x14,0x22,0x41,0x00,0x00,0x00},
  {0x00,0x14,0x14,0x14,0x14,0x14,0x00,0x00},
  {0x00,0x41,0x22,0x14,0x08,0x00,0x00,0x00},
  {0x00,0x02,0x01,0x51,0x09,0x06,0x00,0x00},
  {0x00,0x32,0x49,0x79,0x41,0x3E,0x00,0x00},
  {0x00,0x7E,0x09,0x09,0x09,0x7E,0x00,0x00},
  {0x00,0x7F,0x49,0x49,0x49,0x36,0x00,0x00},
  {0x00,0x3E,0x41,0x41,0x41,0x22,0x00,0x00},
  {0x00,0x7F,0x41,0x41,0x22,0x1C,0x00,0x00},
  {0x00,0x7F,0x49,0x49,0x49,0x41,0x00,0x00},
  {0x00,0x7F,0x09,0x09,0x09,0x01,0x00,0x00},
  {0x00,0x3E,0x41,0x41,0x51,0x72,0x00,0x00},
  {0x00,0x7F,0x08,0x08,0x08,0x7F,0x00,0x00},
  {0x00,0x41,0x7F,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x20,0x40,0x41,0x3F,0x01,0x00,0x00},
  {0x00,0x7F,0x08,0x14,0x22,0x41,0x00,0x00},
  {0x00,0x7F,0x40,0x40,0x40,0x40,0x00,0x00},
  {0x00,0x7F,0x02,0x0C,0x02,0x7F,0x00,0x00},
  {0x00,0x7F,0x04,0x08,0x10,0x7F,0x00,0x00},
  {0x00,0x3E,0x41,0x41,0x41,0x3E,0x00,0x00},
  {0x00,0x7F,0x09,0x09,0x09,0x06,0x00,0x00},
  {0x00,0x3E,0x41,0x51,0x21,0x5E,0x00,0x00},
  {0x00,0x7F,0x09,0x19,0x29,0x46,0x00,0x00},
  {0x00,0x26,0x49,0x49,0x49,0x32,0x00,0x00},
  {0x00,0x01,0x01,0x7F,0x01,0x01,0x00,0x00},
  {0x00,0x3F,0x40,0x40,0x40,0x3F,0x00,0x00},
  {0x00,0x1F,0x20,0x40,0x20,0x1F,0x00,0x00},
  {0x00,0x3F,0x40,0x38,0x40,0x3F,0x00,0x00},
  {0x00,0x63,0x14,0x08,0x14,0x63,0x00,0x00},
  {0x00,0x03,0x04,0x78,0x04,0x03,0x00,0x00},
  {0x00,0x61,0x51,0x49,0x45,0x43,0x00,0x00},
  {0x00,0x7F,0x41,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x02,0x04,0x08,0x10,0x20,0x00,0x00},
  {0x00,0x41,0x41,0x7F,0x00,0x00,0x00,0x00},
  {0x00,0x04,0x02,0x01,0x02,0x04,0x00,0x00},
  {0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00},
  {0x00,0x01,0x02,0x04,0x00,0x00,0x00,0x00},
  {0x00,0x20,0x54,0x54,0x54,0x78,0x00,0x00},
  {0x00,0x7F,0x48,0x44,0x44,0x38,0x00,0x00},
  {0x00,0x38,0x44,0x44,0x28,0x00,0x00,0x00},
  {0x00,0x38,0x44,0x44,0x48,0x7F,0x00,0x00},
  {0x00,0x38,0x54,0x54,0x54,0x18,0x00,0x00},
  {0x00,0x08,0x7E,0x09,0x02,0x00,0x00,0x00},
  {0x00,0x18,0xA4,0xA4,0xA4,0x7C,0x00,0x00},
  {0x00,0x7F,0x08,0x04,0x04,0x78,0x00,0x00},
  {0x00,0x00,0x7D,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x80,0x84,0x7D,0x00,0x00,0x00,0x00},
  {0x00,0x7F,0x10,0x28,0x44,0x00,0x00,0x00},
  {0x00,0x41,0x7F,0x40,0x00,0x00,0x00,0x00},
  {0x00,0x7C,0x04,0x18,0x04,0x78,0x00,0x00},
  {0x00,0x7C,0x08,0x04,0x7C,0x00,0x00,0x00},
  {0x00,0x38,0x44,0x44,0x38,0x00,0x00,0x00},
  {0x00,0xFC,0x24,0x24,0x18,0x00,0x00,0x00},
  {0x00,0x18,0x24,0x24,0xFC,0x00,0x00,0x00},
  {0x00,0x00,0x7C,0x08,0x04,0x00,0x00,0x00},
  {0x00,0x48,0x54,0x54,0x24,0x00,0x00,0x00},
  {0x00,0x04,0x7F,0x44,0x00,0x00,0x00,0x00},
  {0x00,0x3C,0x40,0x40,0x7C,0x00,0x00,0x00},
  {0x00,0x1C,0x20,0x40,0x20,0x1C,0x00,0x00},
  {0x00,0x3C,0x40,0x30,0x40,0x3C,0x00,0x00},
  {0x00,0x44,0x28,0x10,0x28,0x44,0x00,0x00},
  {0x00,0x1C,0xA0,0xA0,0x7C,0x00,0x00,0x00},
  {0x00,0x44,0x64,0x54,0x4C,0x44,0x00,0x00},
  {0x00,0x08,0x36,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x41,0x36,0x08,0x00,0x00,0x00,0x00},
  {0x00,0x02,0x01,0x01,0x02,0x01,0x00,0x00},
  {0x00,0x02,0x05,0x05,0x02,0x00,0x00,0x00}
};

static void OhiGroveOled112_sendCommand (OhiGroveOled112_Device* dev, OhiGroveOled112_Command command)
{
	Iic_start(dev->device);
	Iic_writeByte(dev->device, OHIGROVEOLED112_ADDRESS);
	Iic_writeByte(dev->device, OHIGROVEOLED112_CMD_COMMAND_MODE);
	Iic_writeByte(dev->device, command);
	Iic_stop(dev->device);
}

static void OhiGroveOled112_sendData (OhiGroveOled112_Device* dev, uint8_t data)
{
	Iic_start(dev->device);
	Iic_writeByte(dev->device, OHIGROVEOLED112_ADDRESS);
	Iic_writeByte(dev->device, OHIGROVEOLED112_CMD_DATA_MODE);
	Iic_writeByte(dev->device, data);
	Iic_stop(dev->device);
}

void OhiGroveOled112_init(OhiGroveOled112_Device* dev)
{
    System_Errors errors = ERRORS_NO_ERROR;

//    dev->device = OhiGrove_getIicDevice(dev->connector);
//    errors = OhiGrove_iicEnable(dev->connector, dev->baudrate);

    OhiGroveOled112_sendCommand(dev,OHIGROVEOLED112_CMD_DISPLAY_OFF);
    OhiGroveOled112_sendCommand(dev,OHIGROVEOLED112_CMD_DISPLAY_ON);
    OhiGroveOled112_sendCommand(dev,OHIGROVEOLED112_CMD_NORMAL_DISPLAY);
}

void OhiGroveOled112_setBrightness(OhiGroveOled112_Device* dev, uint8_t brightness)
{
    OhiGroveOled112_sendCommand(dev,OHIGROVEOLED112_CMD_SET_BRIGHTNESS);
    OhiGroveOled112_sendCommand(dev,brightness);
}

void OhiGroveOled112_setHorizontalMode(OhiGroveOled112_Device* dev)
{
    OhiGroveOled112_addressMode = OHIGROVEOLED112_ADDRESSMODE_HORIZONTAL;
    OhiGroveOled112_sendCommand(dev,OHIGROVEOLED112_CMD_ADDRESS_MODE);
    OhiGroveOled112_sendCommand(dev,OHIGROVEOLED112_ADDRESSMODE_HORIZONTAL);
}

void OhiGroveOled112_setPageMode(OhiGroveOled112_Device* dev)
{
    OhiGroveOled112_addressMode = OHIGROVEOLED112_ADDRESSMODE_PAGE,
    OhiGroveOled112_sendCommand(dev,OHIGROVEOLED112_CMD_ADDRESS_MODE);
    OhiGroveOled112_sendCommand(dev,OHIGROVEOLED112_ADDRESSMODE_PAGE);
}

void OhiGroveOled112_setTextXY(OhiGroveOled112_Device* dev, uint8_t row, uint8_t column)
{
    OhiGroveOled112_sendCommand(dev,0xB0 + row);
    OhiGroveOled112_sendCommand(dev,0x00 + (8*column & 0x0F));    // set column lower address
    OhiGroveOled112_sendCommand(dev,0x10 + ((8*column>>4)&0x0F)); // set column higher address
}

void OhiGroveOled112_clearDisplay(OhiGroveOled112_Device* dev)
{
    uint8_t i,j;
    OhiGroveOled112_sendCommand(dev,OHIGROVEOLED112_CMD_DISPLAY_OFF);
    for (j=0; j<8; j++)
    {
        OhiGroveOled112_setTextXY(dev,j,0);
        for(i=0; i<16; i++)
        {
            OhiGroveOled112_putChar(dev,' ');
        }
    }
    OhiGroveOled112_sendCommand(dev,OHIGROVEOLED112_CMD_DISPLAY_ON);
    OhiGroveOled112_setTextXY(dev,0,0);
}

void OhiGroveOled112_putChar(OhiGroveOled112_Device* dev, uint8_t c)
{
    uint8_t i = 0;

    /* Ignore non-printable ASCII characters. This can be modified for multilingual font. */
    if (c < 32 || c > 127)
        c=' ';

    for (i=0; i<8; i++)
    {
    	OhiGroveOled112_sendData(dev,OhiGroveOled112_basicFont[c-32][i]);
    }
}

void OhiGroveOled112_putString(OhiGroveOled112_Device* dev, const char* text)
{
    if (text)
    {
        while (*text)
        {
            OhiGroveOled112_putChar(dev, *text++);
        }
    }
}

void OhiGroveOled112_drawBitmap(OhiGroveOled112_Device* dev, const uint8_t* bitmaparray, uint16_t bytes)
{
    uint16_t i;
    OhiGroveOled112_AddressMode addressMode;

    addressMode = OhiGroveOled112_addressMode;
    if(addressMode != OHIGROVEOLED112_ADDRESSMODE_HORIZONTAL)
    {
        //Bitmap is drawn in horizontal mode
        OhiGroveOled112_setHorizontalMode(dev);
    }

    for (i=0; i<bytes; i++)
    {
        OhiGroveOled112_sendData(dev,bitmaparray[i]);
    }

    if(addressMode == OHIGROVEOLED112_ADDRESSMODE_PAGE)
    {
        // If page mode was used earlier, restore it.
        OhiGroveOled112_setPageMode(dev);
    }
}

void OhiGroveOled112_setNormalDisplay(OhiGroveOled112_Device* dev)
{
	OhiGroveOled112_sendCommand(dev,OHIGROVEOLED112_CMD_NORMAL_DISPLAY);
}

void OhiGroveOled112_setInverseDisplay(OhiGroveOled112_Device* dev)
{
    OhiGroveOled112_sendCommand(dev,OHIGROVEOLED112_CMD_INVERSE_DISPLAY);
}

