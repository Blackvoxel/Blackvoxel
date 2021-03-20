/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2021 Laurent Thiebaut & Olivia Merle
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
 * ZDirection.h
 *
 *  Created on: 8 févr. 2021
 *      Author: laurent
 */

#ifndef Z_ZDIRS_H
#define Z_ZDIRS_H

//#ifndef Z_ZDIRS_H
//#  include "ZDirs.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

class ZDirs
{
  public:
    static ZVector3L Std_Dir[6];
    static ZVector3d Std_DirD[6];
    static Long      Opposite_Dir[6];
    static ZPolar3d  Dir_Vector[6];

};

#endif /* Z_ZDIRS_H */
