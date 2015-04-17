/*
 * timer.c
 *
 *  Created on: 17/apr/2015
 *      Author: Alessio
 */

/******************************************************************************
* Beehive Sensor Control Board
* Copyright (C) 2015 Loccioni Group <http://www.loccioni.com>
* Loccioni Group, Via Fiume 16, Angeli di Rosora (AN), Italy
* ALL RIGHT RESERVED
*
* Author:
*  Marco Giammarini <m.giammarini@loccioni.com>
******************************************************************************/

/**
* @file Sources/board.c
* @author Marco Giammarini <m.giammarini@loccioni.com>
* @brief Timer utility implementations.
*/

#include "board.h"

#include "timer.h"

static uint32_t Timer_milliseconds;

static uint8_t Timer_measuresLF;

static uint32_t Timer_testTimeout;

static uint16_t Timer_ledBlinking;

static uint8_t Timer_test = 0;


static Ftm_Config baseTime = {
    .mode           = FTM_MODE_FREE,
    .timerFrequency = 1000,
    .initCounter    = 0,
};

void Timer_baseTime (void)
{
    Timer_milliseconds++;

    Timer_ledBlinking++;
    if (Timer_ledBlinking == 1000)
    {
        LED_01_TOGGLE();
        Timer_ledBlinking = 0;
        Timer_measuresLF++;

        if (Timer_measuresLF == 3)
        {
            Timer_measuresLF = 0;
        }
    }
}

void Timer_init (void)
{
    Ftm_init(FTM1,Timer_baseTime,&baseTime);
    Timer_milliseconds = 0;

    Timer_ledBlinking = 0;
    Timer_measuresLF = 0;
}

void Timer_delay (uint32_t msDelay)
{
    uint32_t currTicks = Timer_milliseconds;

    while ((Timer_milliseconds - currTicks) < msDelay);
}

uint32_t Timer_currentMillis (void)
{
    return Timer_milliseconds;
}



