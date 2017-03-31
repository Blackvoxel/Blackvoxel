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
 * ZToolsPointing.h
 *
 *  Created on: 31 déc. 2016
 *      Author: laurent
 */

#ifndef Z_ZTOOLSPOINTER_H
#define Z_ZTOOLSPOINTER_H

//#ifndef Z_ZTOOLSPOINTER_H
//#  include "ZToolsPointer.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

class ZToolsPointer
{
  public:
    ZVector3L StartLocation;
    ZVector3L EndLocation;

    ZToolsPointer()
    {
      StartLocation = 0;
      EndLocation = 0;
    }
};



#endif /* Z_ZTOOLSPOINTER_H */
