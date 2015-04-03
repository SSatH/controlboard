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

#include "cli.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  CLI_BUFFER_SIZE      16

static char Cli_buffer[CLI_BUFFER_SIZE];
static uint8_t Cli_bufferIndex = 0;

static void Cli_functionHelp(void* cmd, char* params);
static void Cli_functionVersion(void* cmd, char* params);
static void Cli_functionStatus(void* cmd, char* params);

/* Analog ADC useful define */
#define ADC_FULLSCALE            4096
#define ADC_BGR_mV               1000
#define ADC_VTEMP25_dmV          7190
#define ADC_VTEMP_SLOPE_uVC_hot  1769
#define ADC_VTEMP_SLOPE_uVC_cold 1646
#define ADC_VTEMP_SLOPE_uVC      1715

#define BATTERY_GAIN             20/10

const Cli_Command Cli_commandTable[] =
{
    {"help",    "", "", Cli_functionHelp},
    {"version", "Print actual version of board and firmware", "", Cli_functionVersion},
    {"status", "Print temperature of MCU (in °C), battery voltage and Vcc (in mV)", "", Cli_functionStatus},
};

#define CLI_COMMAND_TABLE_SIZED         (sizeof Cli_commandTable / sizeof Cli_commandTable[0])

static Uart_Config Cli_uartConfig = {

    .rxPin = DEBUG_RX_PIN,
    .txPin = DEBUG_TX_PIN,

    .clockSource = UART_CLOCKSOURCE_BUS,

    .dataBits = UART_DATABITS_EIGHT,
    .parity = UART_PARITY_NONE,

    .baudrate = DEBUG_BAUDRATE,
};

static Adc_Config ADCconf = {
        .adcPin = ADC_PINS_PTE24,
        .clkDiv = 1,
        .clkSource = ADC_BUS_CLOCK_DIV2,
        .sampleLength  = ADC_SHORT_SAMPLE,
        .covertionSpeed = ADC_NORMAL_CONVERTION,
        .resolution = ADC_RESOLUTION_12BIT,
        .average = ADC_AVERAGE_4_SAMPLES,
        .contConv = ADC_SINGLE_CONVERTION,
        .voltRef = ADC_VREF,
};

static void Cli_getCommand (char* name, Cli_Command* cmdFound)
{
    Cli_Command cmd = {name, NULL, NULL, NULL};
    uint8_t i;

    for (i = 0; i < CLI_COMMAND_TABLE_SIZED; i++)
    {
        if(strncmp(name, Cli_commandTable[i].name, strlen(Cli_commandTable[i].name)) == 0)
        {
            cmdFound->name = Cli_commandTable[i].name;
            cmdFound->description = Cli_commandTable[i].description;
            cmdFound->params = Cli_commandTable[i].params;
            cmdFound->cmdFunction = Cli_commandTable[i].cmdFunction;
            return;
        }
    }
    /* If we don't find any command, return null. */
    cmdFound = 0;
}

static void Cli_prompt(void)
{
    Uart_sendString(DEBUG_DEV, "\r\n> ");
    memset(Cli_buffer, 0, sizeof(Cli_buffer));
    Cli_bufferIndex = 0;
}

static void Cli_functionHelp(void* cmd, char* params)
{
    uint8_t i;

    Uart_sendString(DEBUG_DEV, "Available commands:\r\n");
    for (i = 0; i < CLI_COMMAND_TABLE_SIZED; i++)
    {
        Uart_sendString(DEBUG_DEV,Cli_commandTable[i].name);
        Uart_sendString(DEBUG_DEV,": ");
        Uart_sendString(DEBUG_DEV,Cli_commandTable[i].description);
        Uart_sendString(DEBUG_DEV,"\r\n\t");
        Uart_sendString(DEBUG_DEV,Cli_commandTable[i].params);
        Uart_sendString(DEBUG_DEV,"\r\n");
    }
}

static void Cli_functionVersion(void* cmd, char* params)
{
    Uart_sendString(DEBUG_DEV, "Board Version: ");
    Uart_sendString(DEBUG_DEV, PCB_VERSION_STRING);
    Uart_sendString(DEBUG_DEV,"\r\n");
    Uart_sendString(DEBUG_DEV, "Firmware Version: ");
    Uart_sendString(DEBUG_DEV, FW_VERSION_STRING);
    Uart_sendString(DEBUG_DEV,"\r\n");
}

static void Cli_functionStatus(void* cmd, char* params)
{
    uint16_t Measures_bandgap;

    /** CPU voltage measured by ADC in 1mV */
    uint16_t Measures_cpuVoltage = 0;
    /** CPU temperature measured by ADC in 0.1°C */
    uint16_t Measures_cpuTemperature = 0;
    /** Battery voltage measured by ADC in 1mV */
    uint16_t Measures_batteryVoltage = 0;

    uint16_t cpuTemperature;

    uint8_t stringValue[6];

    Adc_readValue(ADC0, ADC_CH_BANDGAP, &Measures_bandgap);

    /* Measure Vdd Voltage */
    Measures_cpuVoltage = (uint16_t) (ADC_FULLSCALE * (uint32_t) ADC_BGR_mV / Measures_bandgap);

    /* Measure CPU Temperature */
    Adc_readValue(ADC0, ADC_CH_TEMP, &Measures_cpuTemperature);
    cpuTemperature = (int16_t)
            (Measures_cpuTemperature * ((int32_t) ADC_BGR_mV * 10) / Measures_bandgap);
    if(cpuTemperature > ADC_VTEMP25_dmV)
    {
        Measures_cpuTemperature = (uint16_t) (250 -
                ((cpuTemperature - ADC_VTEMP25_dmV) * (uint32_t) 1000 / ADC_VTEMP_SLOPE_uVC));
    }
    else
    {
        Measures_cpuTemperature = (uint16_t) (250 +
                        ((ADC_VTEMP25_dmV - cpuTemperature) * (uint32_t) 1000 / ADC_VTEMP_SLOPE_uVC));
    }


    /* Battery voltage */
    Adc_readValue(ADC0, ADC_CH_SE17, &Measures_batteryVoltage);
    Measures_batteryVoltage = (uint16_t)
            (Measures_batteryVoltage * (uint32_t) ADC_BGR_mV * BATTERY_GAIN / Measures_bandgap);
//    Measures_batteryVoltage = (uint16_t) (Measures_batteryVoltage * (uint32_t) 3300 * 100 / 4096 /100);

    u16td(stringValue, Measures_cpuVoltage);
    Uart_sendString(DEBUG_DEV, "Voltage of MCU: ");
    Uart_sendString(DEBUG_DEV, stringValue);
    Uart_sendString(DEBUG_DEV,"\r\n");

    u16td(stringValue, Measures_cpuTemperature);
    Uart_sendString(DEBUG_DEV, "Temperature of MCU: ");
    Uart_sendString(DEBUG_DEV, stringValue);
    Uart_sendString(DEBUG_DEV,"\r\n");

    u16td(stringValue, Measures_batteryVoltage);
    Uart_sendString(DEBUG_DEV, "Voltage of battery: ");
    Uart_sendString(DEBUG_DEV, stringValue);
    Uart_sendString(DEBUG_DEV,"\r\n");
}

void Cli_check(void)
{
    Cli_Command cmd = {NULL, NULL, NULL, NULL};
    char c;

    if (Uart_isCharPresent(DEBUG_DEV))
    {
    	Uart_getChar(DEBUG_DEV, &c);
    	Cli_buffer[Cli_bufferIndex++] = c;
    }

	if ((Cli_bufferIndex != 0) &&
	    (Cli_buffer[Cli_bufferIndex-2] == '\r') && (Cli_buffer[Cli_bufferIndex-1] == '\n'))
    {
		Uart_sendString(DEBUG_DEV, "\r\n");
		Cli_getCommand(Cli_buffer,&cmd);

		if (cmd.name != NULL)
		    cmd.cmdFunction(&cmd, Cli_buffer + strlen(cmd.name));
		else
	        Uart_sendString(DEBUG_DEV, "Command not found!");

		Cli_bufferIndex = 0;
        Cli_prompt();
	}
	else if(Cli_bufferIndex > CLI_BUFFER_SIZE-1)
	{
	    Cli_bufferIndex = 0;
		Cli_prompt();
	}
}

void Cli_init(void)
{
    Uart_open (DEBUG_DEV, 0, &Cli_uartConfig);

    Adc_init (ADC0, &ADCconf);

    Uart_sendString (DEBUG_DEV, PROJECT_NAME);
    Uart_sendString (DEBUG_DEV, "\r\n");
    Uart_sendString (DEBUG_DEV, PROJECT_COPYRIGTH);
    Uart_sendString (DEBUG_DEV, "\r\n");
    Cli_functionVersion(0,0);
    Uart_sendString (DEBUG_DEV, "\r\nCLI ready.\r\n");

    Cli_bufferIndex = 0;
    Cli_prompt();
}

