/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2014 Laurent Thiebaut & Olivia Merle
 *
 * Blackvoxel is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Blackvoxel is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
 * ZOs_Specific_Various.h
 *
 *  Created on: 24 juin 2014
 *      Author: laurent
 */

#ifndef Z_ZOS_SPECIFIC_VARIOUS_H
#define Z_ZOS_SPECIFIC_VARIOUS_H

//#ifndef Z_ZOS_SPECIFIC_VARIOUS_H
//#  include "ZOs_Specific_Various.h"
//#endif

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif


// Windows Operating system

#ifdef ZENV_OS_WINDOWS

  void Windows_DisplayConsole();

#if defined( _MSC_VER ) || defined( __WATCOMC__ )
#    define __sync_bool_compare_and_swap(a,b,c) (InterlockedCompareExchangePointer((void*volatile*)a,c,b), (*a)==(c))
#endif


#endif

#endif /* Z_ZOS_SPECIFIC_VARIOUS_H */
