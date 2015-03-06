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

#define FW_MAJOR_VERSION         1
#define FW_MINOR_VERSION         0
/**
 *  Time stamp of current firmware version (-5GMT + DST).
 *  http://www.currenttimestamp.com/
 */
#define FW_TIME_VERSION          1402325338
/**
 * Macro for board version.
 *
 * 0: OHIBOARD-R1 + SS@H000-0
 */
#define PCB_VERSION              0
#define PROJECT_NAME             "SS@H000-0"
#define PROJECT_COPYRIGTH        "(C) 2015 SS@H Team"

#if PCB_VERSION ==  0



#endif

#endif /* BOARD_H_ */
