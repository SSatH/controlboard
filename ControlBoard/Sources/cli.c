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

const Cli_Command Cli_commandTable[] =
{
    {"help",    "", "", Cli_functionHelp},
    {"version", "Print actual version of board and firmware", "", Cli_functionVersion},
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

    Uart_sendString (DEBUG_DEV, PROJECT_NAME);
    Uart_sendString (DEBUG_DEV, "\r\n");
    Uart_sendString (DEBUG_DEV, PROJECT_COPYRIGTH);
    Uart_sendString (DEBUG_DEV, "\r\n");
    Cli_functionVersion(0,0);
    Uart_sendString (DEBUG_DEV, "\r\nCLI ready.\r\n");

    Cli_bufferIndex = 0;
    Cli_prompt();
}

