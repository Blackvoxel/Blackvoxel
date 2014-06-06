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
#ifndef Z_ZCAMERA_H
#define Z_ZCAMERA_H

//#ifndef Z_ZCAMERA_H
//#  include "ZCamera.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

class ZCamera
{
  public:

    double x,y,z;
    double Yaw, Pitch, Roll;
    struct
    {
      bool   Activate;
      float  Red,Green,Blue,Opacity;
    } ColoredVision;
};

#endif
