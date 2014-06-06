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
 * ZType_ZVector3d_CrossFunc.h
 *
 *  Created on: 10 févr. 2012
 *      Author: laurent
 */

#ifndef Z_ZTYPE_ZVECTOR3D_CROSSFUNC_H
#define Z_ZTYPE_ZVECTOR3D_CROSSFUNC_H

//#ifndef Z_ZTYPE_ZVECTOR3D_CROSSFUNC_H
//#  include "ZTYpe_ZVector3d_CrossFunc.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

inline ZVector3d::ZVector3d( const ZPolar3d & Polar)
{
  x = Polar.Len * sin(Polar.yaw / 57.295779506) * cos(Polar.pitch / 57.295779506);
  y = Polar.Len * sin(Polar.yaw / 57.295779506) * sin(Polar.pitch / 57.295779506);
  z = Polar.Len * cos(Polar.yaw / 57.295779506);
}

inline ZVector3d & ZVector3d::operator = (const ZVector3L & Vect3L)
{
  x = (double)Vect3L.x;
  y = (double)Vect3L.y;
  z = (double)Vect3L.z;
  return(*this);
}

inline ZVector3d ZVector3d::operator + (const ZPolar3d & Polar) const
{
 ZVector3d Result;

 Result.x = x + Polar.Len * sin(Polar.yaw / 57.295779506) * cos(Polar.pitch / 57.295779506);
 Result.y = y + Polar.Len * sin(Polar.yaw / 57.295779506) * sin(Polar.pitch / 57.295779506);
 Result.z = z + Polar.Len * cos(Polar.yaw / 57.295779506);

 return(Result);
}

inline ZVector3d ZVector3d::operator - (const ZPolar3d & Polar) const
{
 ZVector3d Result;

 Result.x = x - Polar.Len * sin(Polar.yaw / 57.295779506) * cos(Polar.pitch / 57.295779506);
 Result.y = y - Polar.Len * sin(Polar.yaw / 57.295779506) * sin(Polar.pitch / 57.295779506);
 Result.z = z - Polar.Len * cos(Polar.yaw / 57.295779506);

 return(Result);
}

inline ZVector3d ZVector3d::operator * (const ZPolar3d & Polar) const
{
 ZVector3d Result;

 Result.x = x * Polar.Len * sin(Polar.yaw / 57.295779506) * cos(Polar.pitch / 57.295779506);
 Result.y = y * Polar.Len * sin(Polar.yaw / 57.295779506) * sin(Polar.pitch / 57.295779506);
 Result.z = z * Polar.Len * cos(Polar.yaw / 57.295779506);

 return(Result);
}

inline ZVector3d ZVector3d::operator / (const ZPolar3d & Polar) const
{
 ZVector3d Result;

 Result.x = x / Polar.Len * sin(Polar.yaw / 57.295779506) * cos(Polar.pitch / 57.295779506);
 Result.y = y / Polar.Len * sin(Polar.yaw / 57.295779506) * sin(Polar.pitch / 57.295779506);
 Result.z = z / Polar.Len * cos(Polar.yaw / 57.295779506);

 return(Result);
}

inline ZVector3d & ZVector3d::operator += (const ZPolar3d & Polar)
{
 x += Polar.Len * sin(Polar.yaw / 57.295779506) * cos(Polar.pitch / 57.295779506);
 y += y + Polar.Len * sin(Polar.yaw / 57.295779506) * sin(Polar.pitch / 57.295779506);
 z += z + Polar.Len * cos(Polar.yaw / 57.295779506);

 return(*this);
}

inline ZVector3d & ZVector3d::operator -= (const ZPolar3d & Polar)
{
 x -= Polar.Len * sin(Polar.yaw / 57.295779506) * cos(Polar.pitch / 57.295779506);
 y -= Polar.Len * sin(Polar.yaw / 57.295779506) * sin(Polar.pitch / 57.295779506);
 z -= Polar.Len * cos(Polar.yaw / 57.295779506);

 return(*this);
}

inline ZVector3d & ZVector3d::operator *= (const ZPolar3d & Polar)
{
  x *= Polar.Len * sin(Polar.yaw / 57.295779506) * cos(Polar.pitch / 57.295779506);
  y *= Polar.Len * sin(Polar.yaw / 57.295779506) * sin(Polar.pitch / 57.295779506);
  z *= Polar.Len * cos(Polar.yaw / 57.295779506);

  return(*this);
}

inline ZVector3d & ZVector3d::operator /= (const ZPolar3d & Polar)
{
  x /= Polar.Len * sin(Polar.yaw / 57.295779506) * cos(Polar.pitch / 57.295779506);
  y /= Polar.Len * sin(Polar.yaw / 57.295779506) * sin(Polar.pitch / 57.295779506);
  z /= Polar.Len * cos(Polar.yaw / 57.295779506);

  return(*this);
}

inline ZVector3d & ZVector3d::operator = (const ZPolar3d & Polar)
{
  //double xp,yp;
  /*
  x = Polar.Len * sin(Polar.yaw / 57.295779506) * cos(Polar.pitch / 57.295779506);
  y = Polar.Len * sin(Polar.yaw / 57.295779506) * sin(Polar.pitch / 57.295779506);
  z = Polar.Len * cos(Polar.yaw / 57.295779506);
  */


  x = Polar.Len * sin(Polar.yaw / 57.295779506) * cos(Polar.pitch / 57.295779506);
  y = Polar.Len * sin(Polar.pitch / 57.295779506);
  z = Polar.Len * cos(Polar.yaw / 57.295779506) * cos(Polar.pitch / 57.295779506);
/*
  xp = Polar.Len * sin(Polar.yaw / 57.295779506) * cos(Polar.pitch / 57.295779506) * cos( Polar.roll / 57.295779506);
  yp = Polar.Len * sin(Polar.pitch / 57.295779506) * cos(Polar.roll / 57.295779506);
  z = Polar.Len * cos(Polar.yaw / 57.295779506) * cos(Polar.pitch / 57.295779506);

  // Z axis rotation

  x = xp*cos(Polar.roll / 57.295779506) - yp*sin(Polar.roll / 57.295779506);
  y = xp*sin(Polar.roll / 57.295779506) + yp*cos(Polar.roll / 57.295779506);
*/
  return(*this);
}

#endif /* Z_ZTYPE_ZVECTOR3D_CROSSFUNC_H */
