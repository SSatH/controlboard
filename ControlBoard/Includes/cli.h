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

#ifndef CLI_H_
#define CLI_H_

#include "board.h"

typedef struct
{
    char *name;
    char *description;
    char *params;
    void (*cmdFunction)(void *cmd, char *cmdLine);
} Cli_Command;

void Cli_init(void);
void Cli_usage(Cli_Command *cmd);
void Cli_check(void);

//extern void cliFuncStatus(void *cmd, char *cmdLine);
//extern void cliFuncHelp(void *cmd, char *cmdLine);
//extern void cliFuncVer(void *cmd, char *cmdLine);


#endif /* CLI_H_ */
