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

#ifndef BOARD_H_
#define BOARD_H_

//#include "libohiboard.h"

#define FW_MAJOR_VERSION              1
#define FW_MINOR_VERSION              0
/**
 *  Time stamp of current firmware version (-5GMT + DST).
 *  http://www.currenttimestamp.com/
 */
#define FW_TIME_VERSION               1402325338
/**
 * Macro for board version.
 *
 * 0: OHIBOARD-R1 + SS@H000-0
 */
#define PCB_VERSION                    0
#define PROJECT_NAME                   "SS@H000-0"
#define PROJECT_COPYRIGTH              "(C) 2015 SS@H Team"

#if PCB_VERSION ==  0

/* FLYPORT GPRS */
#define FLYPORT_DEV                    UART3
#define FLYPORT_RX_PIN                 UART_PINS_PTB10
#define FLYPORT_TX_PIN                 UART_PINS_PTB11
#define FLYPORT_BAUDRATE               115200

/* WIRELESS COMMUNICATION */
#define WIRELESS_DEV                   UART4
#define WIRELESS_RX_PIN                UART_PINS_PTE25
#define WIRELESS_TX_PIN                UART_PINS_PTE24
#define WIRELESS_BAUDRATE              115200

/* DISPLAY */
#define OLED35046P_DEV                 IIC0
#define OLED35046P_SCL                 IIC_PINS_PTB0
#define OLED35046P_SDA                 IIC_PINS_PTB1
#define OLED35046P_BAUDRATE            100000

/* KEYBOARD */
#define KEYBOARD_UP_PIN                GPIO_PINS_PTB2
#define KEYBOARD_DOWN_PIN              GPIO_PINS_PTB3
#define KEYBOARD_RIGHT_PIN             GPIO_PINS_PTB20
#define KEYBOARD_LEFT_PIN              GPIO_PINS_PTB9
#define KEYBOARD_OK_PIN                GPIO_PINS_PTB21
#define KEYBOARD_CONFIG                GPIO_PINS_INPUT

/* DIGITAL */
#define SIM_STATE_PIN                  GPIO_PINS_PTB22
#define SIM_STATE_CONFIG               GPIO_PINS_INPUT

/* DEBUG LED */
#define LED_LED0_PIN                   GPIO_PINS_PTC4
#define LED_LED0_CONFIG                GPIO_PINS_OUTPUT
#define LED_LED0_ON()                  Gpio_clear(LED_LED0_PIN)
#define LED_LED0_OFF()                 Gpio_set(LED_LED0_PIN)
#define LED_LED1_PIN                   GPIO_PINS_PTC5
#define LED_LED1_CONFIG                GPIO_PINS_OUTPUT
#define LED_LED1_ON()                  Gpio_clear(LED_LED1_PIN)
#define LED_LED1_OFF()                 Gpio_set(LED_LED1_PIN)
#define LED_LED2_PIN                   GPIO_PINS_PTC6
#define LED_LED2_CONFIG                GPIO_PINS_OUTPUT
#define LED_LED2_ON()                  Gpio_clear(LED_LED2_PIN)
#define LED_LED2_OFF()                 Gpio_set(LED_LED2_PIN)
#define LED_LED3_PIN                   GPIO_PINS_PTC7
#define LED_LED3_CONFIG                GPIO_PINS_OUTPUT
#define LED_LED3_ON()                  Gpio_clear(LED_LED3_PIN)
#define LED_LED3_OFF()                 Gpio_set(LED_LED3_PIN)
#define LED_LED4_PIN                   GPIO_PINS_PTC8
#define LED_LED4_CONFIG                GPIO_PINS_OUTPUT
#define LED_LED4_ON()                  Gpio_clear(LED_LED4_PIN)
#define LED_LED4_OFF()                 Gpio_set(LED_LED4_PIN)

#endif // (PCB_VERSION == 0)

union Board_TaskStatusType
{
    uint16_t requestPending;

    struct
    {
    	uint16_t commandReady          :1;

        uint16_t stopAll               :1;

        uint16_t notUsed               :7;
    } flags;
} extern Board_taskStatus;

union Board_SystemStatusType
{
    uint16_t actualStatus;

    struct
    {

        uint16_t notUsed               :6;
    } flags;
} extern Board_systemStatus;

typedef enum _Board_Errors
{
    /* System Error  */

} Board_Errors;

void Board_setPinout (void);

void Board_initGpio (void);

#endif /* BOARD_H_ */
