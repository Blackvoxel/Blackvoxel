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
 * ZDirection.cpp
 *
 *  Created on: 8 févr. 2021
 *      Author: laurent
 */

#include "ZDirs.h"


ZVector3L ZDirs::Std_Dir[6] =
{
    ZVector3L( 0, 0, 1 ),
    ZVector3L( 1, 0, 0 ),
    ZVector3L( 0, 0, -1),
    ZVector3L(-1, 0, 0 ),
    ZVector3L( 0, 1, 0 ),
    ZVector3L( 0,-1, 0 )
};

ZVector3d ZDirs::Std_DirD[6] =
{
    ZVector3d( 0.0, 0.0, 1.0 ),
    ZVector3d( 1.0, 0.0, 0.0 ),
    ZVector3d( 0.0, 0.0,-1.0 ),
    ZVector3d(-1.0, 0.0, 0.0 ),
    ZVector3d( 0.0, 1.0, 0.0 ),
    ZVector3d( 0.0,-1.0, 0.0 )
};
Long ZDirs::Opposite_Dir[6] =
{
  2,
  3,
  0,
  1,
  5,
  4
};

ZPolar3d ZDirs::Dir_Vector[6] =
{
  ZPolar3d(0.0,0.0,0.0,1.0),
  ZPolar3d(90.0,0.0,0.0,1.0),
  ZPolar3d(180.0,0.0,0.0,1.0),
  ZPolar3d(270.0,0.0,0.0,1.0),
  ZPolar3d(0.0,90.0,0.0,1.0),
  ZPolar3d(0.0,270,0.0,1.0)
};
