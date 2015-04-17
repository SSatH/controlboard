/******************************************************************************
 * Copyright (C) 2015 Smart and Security @ Home Team
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>
 *
 * Authors:
 *  Marco Giammarini <m.giammarini@warcomeb.it>
 *  Alessio Paolucci <a.paolucci89@gmail.com>
 ******************************************************************************/

#include "board.h"

union Board_TaskStatusType Board_taskStatus;

union Board_SystemStatusType Board_systemStatus;

void Board_setPinout (void)
{
    /* LED for DEBUG */
    Gpio_config(LED_LED0_PIN, LED_LED0_CONFIG);
    Gpio_config(LED_LED1_PIN, LED_LED1_CONFIG);
    Gpio_config(LED_LED2_PIN, LED_LED2_CONFIG);
    Gpio_config(LED_LED3_PIN, LED_LED3_CONFIG);
    Gpio_config(LED_LED4_PIN, LED_LED4_CONFIG);

    Gpio_config(LED_01_PIN, LED_01_CONFIG);

    /* DIGITAL SIGNALS Pin Setting */
}

void Board_initGpio (void)
{
    /* LED for DEBUG */
    LED_LED0_OFF();
    LED_LED1_OFF();
    LED_LED2_OFF();
    LED_LED3_OFF();
    LED_LED4_OFF();

    /* DIGITAL OUTPUT */

}


