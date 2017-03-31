/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2015 Laurent Thiebaut & Olivia Merle
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
 * ZGame_DevHelpers.h
 *
 *  Created on: 6 janv. 2015
 *      Author: laurent
 */

#ifndef Z_ZGAME_DEVHELPERS_H
#define Z_ZGAME_DEVHELPERS_H

//#ifndef Z_ZGAME_DEVHELPERS_H
//#  include "ZGame_DevHelpers.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif


class ZGame_DevHelpers
{

    static bool OutputZoneImage(ZString & HelperDirectory);
    static bool OutputZoneHeight(ZString & HelperDirectory);

  public:
    static bool OutputHelpers(ZString & HelperDirectory);

};

#endif /* Z_ZGAME_DEVHELPERS_H */
