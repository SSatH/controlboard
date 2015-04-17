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
* @file Includes/board.h
* @author Marco Giammarini <m.giammarini@loccioni.com>
* @brief Timer utility defines.
*/

#ifndef __TIMER_H
#define __TIMER_H

#include "board.h"

//typedef enum _Timer_Timeout
//{
//    TIMER_TIMEOUT_TEST,
//    TIMER_TIMEOUT_PUMP,
//
//    TIMER_TIMEOUT_IMSCOMM,
//    TIMER_TIMEOUT_SOMCOMM,
//    TIMER_TIMEOUT_MIRPASCOMM,
//    TIMER_TIMEOUT_PRECONCOMM,
//} Timer_Timeout;


void Timer_baseTime (void);

void Timer_init (void);

uint32_t Timer_currentMillis (void);
void Timer_delay (uint32_t msDelay);

//void Timer_startTimeout (Timer_Timeout type, uint32_t timeout);
//void Timer_stopTimeout (Timer_Timeout type);

#endif /* __TIMER_H */
