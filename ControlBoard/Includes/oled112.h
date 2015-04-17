/******************************************************************************
 * Copyright (C) 2014-2015 A. C. Open Hardware Ideas Lab
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
 * @file libohigrove/relay.h
 * @author Alessio Paolucci <a.paolucci89@gmail.com>
 * @brief
 */

#ifndef __OLED112_H
#define __OLED112_H

#include "board.h"

typedef struct _OhiGroveOled112_Device
{
//    OhiGrove_Conn connector;

    Iic_DeviceHandle device;

    uint32_t baudrate;

} OhiGroveOled112_Device;

void OhiGroveOled112_init(OhiGroveOled112_Device* dev);

void OhiGroveOled112_setNormalDisplay(OhiGroveOled112_Device* dev);
void OhiGroveOled112_setInverseDisplay(OhiGroveOled112_Device* dev);

void OhiGroveOled112_setPageMode(OhiGroveOled112_Device* dev);
void OhiGroveOled112_setHorizontalMode(OhiGroveOled112_Device* dev);

void OhiGroveOled112_setTextXY(OhiGroveOled112_Device* dev, uint8_t row, uint8_t column);
void OhiGroveOled112_clearDisplay(OhiGroveOled112_Device* dev);
void OhiGroveOled112_setBrightness(OhiGroveOled112_Device* dev, uint8_t brightness);

void OhiGroveOled112_putChar(OhiGroveOled112_Device* dev, uint8_t c);
void OhiGroveOled112_putString(OhiGroveOled112_Device* dev, const char* text);
void OhiGroveOled112_drawBitmap(OhiGroveOled112_Device* dev, const uint8_t* bitmaparray, uint16_t bytes);

#endif /* __OLED112_H */
