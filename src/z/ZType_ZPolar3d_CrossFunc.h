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
 * ZType_ZPolar3d_CrossFunc.h
 *
 *  Created on: 12 févr. 2012
 *      Author: laurent
 */

#ifndef Z_ZTYPE_ZPOLAR3D_CROSSFUNC_H
#define Z_ZTYPE_ZPOLAR3D_CROSSFUNC_H

//#ifndef Z_ZTYPE_ZPOLAR3D_CROSSFUNC_H
//#  include "ZType_ZPolar3d_CrossFunc.h"
//#endif


inline ZPolar3d & ZPolar3d::operator = (const ZVector3d & Vec3d)
{
  double Len1,x,y,z;
  double pi = 3.14159265;

  x = Vec3d.x; y = Vec3d.y; z = Vec3d.z;

  Len = sqrt( Vec3d.x * Vec3d.x + Vec3d.y * Vec3d.y + Vec3d.z * Vec3d.z );
  Len1 = sqrt( Vec3d.x * Vec3d.x + Vec3d.z * Vec3d.z);


  if (z > 0.0)
  {
    if (x > 0.0)
    {
      if  (z>x) yaw = atan(x/z);
      else      yaw = pi / 2.0 - atan(z/x);
    }
    else
    {
      if (z>-x) yaw = pi * 2.0 - atan(-x/z);
      else      yaw = pi * 1.5 + atan(z/-x);
    }
  }
  else
  {
    if (x > 0.0)
    {
      if (-z>x) yaw = pi - atan(x/-z);
      else      yaw = pi / 2.0 + atan (-z/x);
    }
    else
    {
      if (-z>-x) yaw = pi + atan(-x/-z);
      else       yaw = pi * 1.5 - atan(-z/-x);
    }
  }

  if (y > 0.0)
  {
    if   (Len1 > y) pitch = atan( y / Len1);
    else            pitch = pi * 0.5 - atan (Len1 / y);
  }
  else
  {
    if (Len1 > -y) pitch = pi * 2.0 - atan ( -y / Len1 );
    else           pitch = pi * 1.5 + atan ( Len1 / -y );
  }

  yaw   *= 57.295779506;
  pitch   *= 57.295779506;
  roll = 0;
  return(*this);
}

#endif /* Z_ZTYPE_ZPOLAR3D_CROSSFUNC_H */
