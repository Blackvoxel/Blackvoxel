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
 * ZOs_Specific_EditCode.h
 *
 *  Created on: 24 févr. 2015
 *      Author: laurent
 */

#ifndef Z_ZOS_SPECIFIC_EDITCODE_H
#define Z_ZOS_SPECIFIC_EDITCODE_H

//#ifndef Z_ZOS_SPECIFIC_EDITCODE_H
//#  include "ZOs_Specific_EditCode.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif

class ZEditCode
{

  public:

    static bool EditCode(ZString * Tool, ZString * File);

};



#endif /* Z_ZOS_SPECIFIC_EDITCODE_H */
