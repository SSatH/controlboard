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
#include "board.h"
#include "libohiboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char version[16];
//char cliBuf[32];
char cliBuf[5];
char tempBuf[64];
int cliBufIndex;



// this table must be sorted by command name
const cliCommand_t cliCommandTable[] = {
    {"help", "", cliFuncHelp},
    {"stat", "", cliFuncStatus},
    {"vers", "", cliFuncVer}
};

#define CLI_N_CMDS         (sizeof cliCommandTable / sizeof cliCommandTable[0])

#define CLI_HELP_CMD           "help"
#define CLI_STATUS_CMD         "stat"
#define CLI_VERSION_CMD        "vers"

static Uart_Config Debug_config = {

    .rxPin = DEBUG_RX_PIN,
    .txPin = DEBUG_TX_PIN,

    .clockSource = UART_CLOCKSOURCE_BUS,

    .dataBits = UART_DATABITS_EIGHT,
    .parity = UART_PARITY_NONE,

    .baudrate = DEBUG_BAUDRATE,
};

void cliUsage(cliCommand_t *cmd) {
	Uart_sendString(DEBUG_DEV, "usage: ");
	Uart_sendString(DEBUG_DEV, cmd->name);
	Uart_putChar(DEBUG_DEV,' ');
	Uart_sendString(DEBUG_DEV, cmd->params);
    Uart_sendString(DEBUG_DEV, "\r\n");
}

void cliFuncHelp(void *cmd, char *cmdLine) {
    int i;

    Uart_sendString(DEBUG_DEV, "Available commands:\r\n\n");

    for (i = 0; i < CLI_N_CMDS; i++) {
    Uart_sendString(DEBUG_DEV, cliCommandTable[i].name);
    Uart_putChar(DEBUG_DEV,' ');
	Uart_sendString(DEBUG_DEV, cliCommandTable[i].params);
	Uart_sendString(DEBUG_DEV, "\r\n");
    }
}

void cliFuncVer(void *cmd, char *cmdLine) {
//	sprintf(tempBuf, "FW ver %s%s%s\r\n", FW_MAJOR_VERSION, ".", FW_MINOR_VERSION);
	sprintf(tempBuf, "FW ver %s\r\n", FW_VERSION);
    Uart_sendString (DEBUG_DEV, tempBuf);
}

void cliPrompt(void) {
	Uart_sendString (DEBUG_DEV, "\r\n> ");
    memset(cliBuf, 0, sizeof(cliBuf));
    cliBufIndex = 0;
}

void cliFuncStatus(void *cmd, char *cmdLine) {
    const char *formatFloat = "%-12s%10.2f\r\n";
    const char *formatInt = "%-12s%10d\r\n";
    const char *formatString = "%-12s%10s\r\n";

    uint16_t BatteryLevel = 0;
    uint16_t MicroVcc = 0;
    uint16_t MicroTemperature = 0;

    sprintf(tempBuf, formatInt, "Battery level", BatteryLevel);
    Uart_sendString (DEBUG_DEV, tempBuf);

    sprintf(tempBuf, formatInt, "Processor Voltage", MicroVcc);
    Uart_sendString (DEBUG_DEV, tempBuf);

    sprintf(tempBuf, formatInt, "Processor Temperature", MicroTemperature);
    Uart_sendString (DEBUG_DEV, tempBuf);
}

void cliCheck(void) {
    cliCommand_t *cmd = NULL;
    char c;

    if (Uart_isCharPresent(DEBUG_DEV))
    {
    	Uart_getChar(DEBUG_DEV, &c);
    	cliBuf[cliBufIndex++] = c;
    	if(cliBufIndex == 4)
    	{
    		cliBuf[cliBufIndex] = '\0';
    	}
    }

	if (cliBufIndex == 4)
	{
		Uart_sendString(DEBUG_DEV, "\r\n");
		if(stringCompare (cliBuf, CLI_HELP_CMD) == 0)
		{
			cliFuncHelp(0, 0);
		}
		else if(stringCompare (cliBuf, CLI_STATUS_CMD) == 0)
		{
			cliFuncStatus(0,0);
		}
		else if(stringCompare (cliBuf, CLI_VERSION_CMD) == 0)
		{
			cliFuncVer(0,0);
		}
		else
		{
			Uart_sendString(DEBUG_DEV, "Command Error2 \r\n");
	    }

		cliBufIndex = 0;

		cliPrompt();
	}
	else if(cliBufIndex > 4)
	{
		Uart_sendString(DEBUG_DEV, "Command Error1 \r\n");
		cliBufIndex = 0;
		cliPrompt();
	}

    return;
}


void cliInit(void) {
	Uart_open (DEBUG_DEV, 0, &Debug_config);
	Uart_sendString (DEBUG_DEV, PROJECT_NAME);
	Uart_sendString (DEBUG_DEV, "\r\n");
	Uart_sendString (DEBUG_DEV, PROJECT_COPYRIGTH);
	Uart_sendString (DEBUG_DEV, "\r\n");
	cliFuncVer(0,0);
	Uart_sendString (DEBUG_DEV, "\r\nCLI ready.\r\n");

    cliPrompt();
}

