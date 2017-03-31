/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2017 Laurent Thiebaut & Olivia Merle
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
 * ZType_Vector3b.h
 *
 *  Created on: 2 janv. 2017
 *      Author: laurent
 */

#ifndef Z_TYPE_ZVECTOR3B_H
#define Z_TYPE_ZVECTOR3B_H

//#ifndef Z_TYPE_ZVECTOR3B_H
//#  include "ZType_ZVector3B.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

struct ZVector3B_Init
{
  Long   x;
  Long   y;
  Long   z;
};

class ZVector3B
{
  public:

  Long   x;
  Long   y;
  Long   z;

  inline ZVector3B() {}
  inline ZVector3B( Long x, Long y, Long z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }
  inline ZVector3B ( const ZVector3B & Vec3B )
  {
    x = Vec3B.x;
    y = Vec3B.y;
    z = Vec3B.z;
  }
  inline ZVector3B ( const ZVector3d & Vec3d );
  inline ZVector3B ( Long Scalar )
  {
    x = Scalar;
    y = Scalar;
    z = Scalar;
  }
  inline ZVector3B ( double Scalar )
  {
    x = (Long)Scalar;
    y = (Long)Scalar;
    z = (Long)Scalar;
  }

  inline ZVector3B & operator = (const ZVector3B & Vec3B)
  {
    x = Vec3B.x;
    y = Vec3B.y;
    z = Vec3B.z;
    return(*this);
  }

  inline bool operator == (const ZVector3B & Vec3B) const
  {
    if ((Vec3B.x != x)||(Vec3B.y != y)||(Vec3B.z !=z)) return(false);
    return(true);
  }

  inline bool operator != (const ZVector3B & Vec3B) const
  {
    if ((Vec3B.x != x)||(Vec3B.y != y)||(Vec3B.z !=z)) return(true);
    return(false);
  }

  inline ZVector3B & operator = (const ZVector3d & Vec3d);

  inline ZVector3B & operator = (const Long Value)
  {
    x = y = z = Value;
    return(*this);
  }

  inline ZVector3B & operator = (ZVector3B_Init const Value)
  {
    x = Value.x;
    y = Value.y;
    z = Value.z;

    return(*this);
  }

  inline ZVector3B & operator += (const ZVector3B & Vec3B)
  {
    x += Vec3B.x;
    y += Vec3B.y;
    z += Vec3B.z;
    return(*this);
  }

  inline ZVector3B & operator -= (const ZVector3B & Vec3B)
  {
    x -= Vec3B.x;
    y -= Vec3B.y;
    z -= Vec3B.z;
    return(*this);
  }

  inline ZVector3B & operator /= (const ZVector3B & Vec3B)
  {
    x /= Vec3B.x;
    y /= Vec3B.y;
    z /= Vec3B.z;
    return(*this);
  }

  inline ZVector3B & operator *= (const ZVector3B & Vec3B)
  {
    x *= Vec3B.x;
    y *= Vec3B.y;
    z *= Vec3B.z;
    return(*this);
  }

  inline ZVector3B operator - () const
  {
    ZVector3B Result;

    Result.x = -x;
    Result.y = -y;
    Result.z = -z;

    return(Result);
  }

  inline ZVector3B operator - (const ZVector3B Vec3B) const
  {
    ZVector3B Result;

    Result.x = x - Vec3B.x;
    Result.y = y - Vec3B.y;
    Result.z = z - Vec3B.z;

    return(Result);
  }

  inline ZVector3B operator + (const ZVector3B Vec3B) const
  {
    ZVector3B Result;

    Result.x = x + Vec3B.x;
    Result.y = y + Vec3B.y;
    Result.z = z + Vec3B.z;

    return(Result);
  }


  inline ZVector3B & GetSign( const ZVector3B & Vec3B )
  {
    x = (Vec3B.x > 0) - (Vec3B.x < 0);
    y = (Vec3B.y > 0) - (Vec3B.y < 0);
    z = (Vec3B.z > 0) - (Vec3B.z < 0);

    return(*this);
  }

  inline double Distance( const ZVector3B & Vec3B ) const
  {
    register double dist, tmp;
    tmp = (double)(x-Vec3B.x);
    dist = tmp*tmp;
    tmp = (double)(y-Vec3B.y);
    dist+= tmp*tmp;
    tmp = (double)(z-Vec3B.z);
    dist+= tmp*tmp;
    return(sqrt(dist));
  }

};


#endif /* Z_TYPE_ZVECTOR3B_H */
