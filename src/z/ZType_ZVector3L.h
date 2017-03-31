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
 * ZType_ZVector3L.h
 *
 *  Created on: 9 févr. 2012
 *      Author: laurent
 */

#ifndef Z_TYPE_ZVECTOR3L_H
#define Z_TYPE_ZVECTOR3L_H

//#ifndef Z_TYPE_ZVECTOR3L_H
//#  include "ZType_ZVector3L.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

class ZVector3B;


class ZVector3L
{
  public:

  Long   x;
  Long   y;
  Long   z;

  inline ZVector3L() {}
  inline ZVector3L( Long x, Long y, Long z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }
  inline ZVector3L ( const ZVector3L & Vec3L )
  {
    x = Vec3L.x;
    y = Vec3L.y;
    z = Vec3L.z;
  }
  inline ZVector3L ( const ZVector3d & Vec3d );
  inline ZVector3L ( const ZVector3B & Vec3B );
  inline ZVector3L ( Long Scalar )
  {
    x = Scalar;
    y = Scalar;
    z = Scalar;
  }
  inline ZVector3L ( double Scalar )
  {
    x = (Long)Scalar;
    y = (Long)Scalar;
    z = (Long)Scalar;
  }

  inline ZVector3L & operator = (const ZVector3L & Vec3L)
  {
    x = Vec3L.x;
    y = Vec3L.y;
    z = Vec3L.z;
    return(*this);
  }

  inline bool operator == (const ZVector3L & Vec3L) const
  {
    if ((Vec3L.x != x)||(Vec3L.y != y)||(Vec3L.z !=z)) return(false);
    return(true);
  }

  inline bool operator != (const ZVector3L & Vec3L) const
  {
    if ((Vec3L.x != x)||(Vec3L.y != y)||(Vec3L.z !=z)) return(true);
    return(false);
  }

  inline ZVector3L & operator = (const ZVector3d & Vec3d);
  inline ZVector3L & operator = (const ZVector3B & Vec3B);

  inline ZVector3L & operator = (const Long Value)
  {
    x = y = z = Value;
    return(*this);
  }

  inline ZVector3L & operator += (const ZVector3B & Vec3B);
  inline ZVector3L & operator += (const ZVector3L & Vec3L)
  {
    x += Vec3L.x;
    y += Vec3L.y;
    z += Vec3L.z;
    return(*this);
  }

  inline ZVector3L & operator -= (const ZVector3B & Vec3B);
  inline ZVector3L & operator -= (const ZVector3L & Vec3L)
  {
    x -= Vec3L.x;
    y -= Vec3L.y;
    z -= Vec3L.z;
    return(*this);
  }

  inline ZVector3L & operator /= (const ZVector3L & Vec3L)
  {
    x /= Vec3L.x;
    y /= Vec3L.y;
    z /= Vec3L.z;
    return(*this);
  }

  inline ZVector3L & operator *= (const ZVector3L & Vec3L)
  {
    x *= Vec3L.x;
    y *= Vec3L.y;
    z *= Vec3L.z;
    return(*this);
  }

  inline ZVector3L operator - () const
  {
    ZVector3L Result;

    Result.x = -x;
    Result.y = -y;
    Result.z = -z;

    return(Result);
  }


  inline ZVector3L operator - (const ZVector3B Vec3B) const;
  inline ZVector3L operator - (const ZVector3L Vec3L) const
  {
    ZVector3L Result;

    Result.x = x - Vec3L.x;
    Result.y = y - Vec3L.y;
    Result.z = z - Vec3L.z;

    return(Result);
  }

  inline ZVector3L operator + (const ZVector3B Vec3B) const;

  inline ZVector3L operator + (const ZVector3L Vec3L) const
  {
    ZVector3L Result;

    Result.x = x + Vec3L.x;
    Result.y = y + Vec3L.y;
    Result.z = z + Vec3L.z;

    return(Result);
  }


  inline ZVector3L & GetSign( const ZVector3L & Vec3L )
  {
    x = (Vec3L.x > 0) - (Vec3L.x < 0);
    y = (Vec3L.y > 0) - (Vec3L.y < 0);
    z = (Vec3L.z > 0) - (Vec3L.z < 0);

    return(*this);
  }

  inline double Distance( const ZVector3L & Vec3L ) const
  {
    register double dist, tmp;
    tmp = (double)(x-Vec3L.x);
    dist = tmp*tmp;
    tmp = (double)(y-Vec3L.y);
    dist+= tmp*tmp;
    tmp = (double)(z-Vec3L.z);
    dist+= tmp*tmp;
    return(sqrt(dist));
  }

};


#endif /* Z_ZTYPE_ZVECTOR3L_H */
