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
 * ZType_ZPolar3d.h
 *
 *  Created on: 12 févr. 2012
 *      Author: laurent
 */

#ifndef Z_ZTYPE_ZPOLAR3D_H
#define Z_ZTYPE_ZPOLAR3D_H

// #ifndef Z_ZTYPE_ZPOLAR3D_H
// #  include "ZType_ZPolar3d.h"
// #endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

class ZVector3d;

class ZPolar3d
{
  public:
    double yaw, pitch, roll, Len;

    inline ZPolar3d() {}
    inline ZPolar3d( double yaw, double pitch, double roll, double Len)
    {
      this->yaw   = yaw;
      this->pitch = pitch;
      this->roll  = roll;
      this->Len   = Len;
    }

    inline ZPolar3d( ZPolar3d const &Polar )
    {
      yaw   = Polar.yaw;
      pitch = Polar.pitch;
      roll  = Polar.roll;
      Len   = Polar.Len;
    }

    inline ZPolar3d operator + (ZPolar3d const & Polar) const
    {
      ZPolar3d Result;

      Result.yaw   = yaw   + Polar.yaw;
      Result.pitch = pitch + Polar.pitch;
      Result.roll  = roll  + Polar.roll;
      Result.Len   = Len   + Polar.Len;

      return(Result);
    }

    inline ZPolar3d operator - (ZPolar3d const & Polar) const
    {
      ZPolar3d Result;

      Result.yaw   = yaw   - Polar.yaw;
      Result.pitch = pitch - Polar.pitch;
      Result.roll  = roll  - Polar.roll;
      Result.Len   = Len   - Polar.Len;

      return(Result);
    }

    inline ZPolar3d & operator += (ZPolar3d const & Polar)
    {
      yaw   += Polar.yaw;
      pitch += Polar.pitch;
      roll  += Polar.roll;
      Len   += Polar.Len;

      return(*this);
    }

    inline ZPolar3d & operator -= (ZPolar3d const & Polar)
    {
      yaw   -= Polar.yaw;
      pitch -= Polar.pitch;
      roll  -= Polar.roll;
      Len   -= Polar.Len;

      return(*this);
    }

    inline ZPolar3d & operator = (ZPolar3d const & Polar)
    {
      yaw   = Polar.yaw;
      pitch = Polar.pitch;
      roll  = Polar.roll;
      Len   = Polar.Len;

      return(*this);
    }

    inline ZPolar3d & operator = (const ZVector3d & Vec3d);
};

#endif /* ZTYPE_ZPOLAR3D_H_ */
