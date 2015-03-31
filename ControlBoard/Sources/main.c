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

#include "libohiboard.h"
#include "board.h"
#include "cli.h"

int main(void)
{
    uint8_t i;

	uint32_t foutBUS;
	uint32_t foutSYS;
	System_Errors errore = ERRORS_NO_ERROR;

	Clock_Config config = {
			.source = CLOCK_CRYSTAL,
			.fext = 16000000,
			.foutSys = 10000000,
			.busDivider = 2,
			.flexbusDivider = 2,
			.flashDivider = 4,
	};

	SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

	errore = Clock_Init(&config);
    errore = Clock_setDividers(config.busDivider, config.flexbusDivider, config.flashDivider);
    foutSYS = Clock_getFrequency(CLOCK_SYSTEM);
    foutBUS = Clock_getFrequency(CLOCK_BUS);

    Cli_init();

    for (;;)
    {
        Cli_check();

        i++;
    }
    /* Never leave main */
    return 0;
}
