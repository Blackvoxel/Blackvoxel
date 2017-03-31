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
 * ZType_ZVector3d.h
 *
 *  Created on: 10 févr. 2012
 *      Author: laurent
 */

#ifndef Z_ZTYPE_ZVECTOR3D_H
#define Z_ZTYPE_ZVECTOR3D_H

// #ifndef Z_ZTYPE_ZVECTOR3D_H
// #  include "ZType_ZVector3d.h"
// #endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

class ZPolar3d;

class ZVector3d
{
  public:
    double x,y,z;

  inline ZVector3d() {}


  inline ZVector3d(const double Init[3])
  {
    this->x = Init[0];
    this->y = Init[1];
    this->z = Init[2];
  }

  inline ZVector3d(double x, double y, double z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  inline ZVector3d( double Value )
  {
    x = y = z = Value;
  }

  inline ZVector3d( const ZPolar3d & Polar);


  inline ZVector3d( const ZVector3d & Vec3d )
  {
    x = Vec3d.x;
    y = Vec3d.y;
    z = Vec3d.z;
  }

  inline ZVector3d operator + (const ZVector3d & Vec3d) const
  {
    ZVector3d Result;

    Result.x = x + Vec3d.x;
    Result.y = y + Vec3d.y;
    Result.z = z + Vec3d.z;

    return(Result);
  }

  inline ZVector3d operator + (const double Scalar) const
  {
    ZVector3d Result;

    Result.x = x + Scalar;
    Result.y = y + Scalar;
    Result.z = z + Scalar;

    return(Result);
  }

  inline ZVector3d operator + (const ZPolar3d & Polar) const;


  inline ZVector3d operator - (const ZVector3d & Vec3d) const
  {
    ZVector3d Result;

    Result.x = x - Vec3d.x;
    Result.y = y - Vec3d.y;
    Result.z = z - Vec3d.z;

    return(Result);
  }
  inline ZVector3d operator - (const double Scalar) const
  {
    ZVector3d Result;

    Result.x = x - Scalar;
    Result.y = y - Scalar;
    Result.z = z - Scalar;

    return(Result);
  }

  inline ZVector3d operator - (const ZPolar3d & Polar) const;

  inline ZVector3d operator - () const
  {
    ZVector3d Result;

    Result.x = -x;
    Result.y = -y;
    Result.z = -z;

    return(Result);
  }

  inline ZVector3d operator * (const ZVector3d & Vec3d) const
  {
    ZVector3d Result;

    Result.x = x * Vec3d.x;
    Result.y = y * Vec3d.y;
    Result.z = z * Vec3d.z;

    return(Result);
  }

  inline ZVector3d operator * (const double Scalar) const
  {
    ZVector3d Result;

    Result.x = x * Scalar;
    Result.y = y * Scalar;
    Result.z = z * Scalar;

    return(Result);
  }

  inline ZVector3d operator * (const ZPolar3d & Polar) const;


  inline ZVector3d operator / (const ZVector3d & Vec3d) const
  {
    ZVector3d Result;

    Result.x = x / Vec3d.x;
    Result.y = y / Vec3d.y;
    Result.z = z / Vec3d.z;

    return(Result);
  }

  inline ZVector3d operator / (const double Scalar) const
  {
    ZVector3d Result;

    Result.x = x / Scalar;
    Result.y = y / Scalar;
    Result.z = z / Scalar;

    return(Result);
  }

  inline ZVector3d operator / (const ZPolar3d & Polar) const;


  inline ZVector3d & operator += (const ZVector3d & Vec3d)
  {
    x += Vec3d.x;
    y += Vec3d.y;
    z += Vec3d.z;
    return(*this);
  }

  inline ZVector3d & operator += (const double Scalar)
  {
    x += Scalar;
    y += Scalar;
    z += Scalar;
    return(*this);
  }

  inline ZVector3d & operator += (const ZPolar3d & Polar);

  inline ZVector3d & operator -= (const ZVector3d & Vec3d)
  {
    x -= Vec3d.x;
    y -= Vec3d.y;
    z -= Vec3d.z;
    return(*this);
  }

  inline ZVector3d & operator -= (const double Scalar)
  {
    x -= Scalar;
    y -= Scalar;
    z -= Scalar;
    return(*this);
  }

  inline ZVector3d & operator -= (const ZPolar3d & Polar);


  inline ZVector3d & operator *= (const ZVector3d & Vec3d)
  {
    x *= Vec3d.x;
    y *= Vec3d.y;
    z *= Vec3d.z;
    return(*this);
  }

  inline ZVector3d & operator *= (const double Scalar)
  {
    x *= Scalar;
    y *= Scalar;
    z *= Scalar;
    return(*this);
  }

  inline ZVector3d & operator *= (const ZPolar3d & Polar);


  inline ZVector3d & operator /= (const ZVector3d & Vec3d)
  {
    x /= Vec3d.x;
    y /= Vec3d.y;
    z /= Vec3d.z;
    return(*this);
  }

  inline ZVector3d & operator /= (const double Scalar)
  {
    x /= Scalar;
    y /= Scalar;
    z /= Scalar;
    return(*this);
  }

  inline ZVector3d & operator /= (const ZPolar3d & Polar);

  inline ZVector3d & operator = (const double Init[3])
  {
    x = Init[0];
    y = Init[1];
    z = Init[2];
    return(*this);
  }


  inline ZVector3d & operator = (const ZVector3L & Vect3L);


  inline ZVector3d & operator = (const ZVector3d & Vec3d)
  {
    x = Vec3d.x;
    y = Vec3d.y;
    z = Vec3d.z;
    return(*this);
  }

  inline ZVector3d & operator = (const double Scalar)
  {
    x = Scalar;
    y = Scalar;
    z = Scalar;
    return(*this);
  }

  inline ZVector3d & operator = (const ZPolar3d & Polar);

  inline double Distance(ZVector3d & SecondPoint)
  {
    register double t1,t2;

    t1 = SecondPoint.x - x;
    t2 = t1 * t1;
    t1 = SecondPoint.y - y;
    t2 += t1 * t1;
    t1 = SecondPoint.z - z;
    t2 += t1 * t1;
    return( sqrt(t2) );
  }

  inline bool operator == (const ZVector3d & Vect3d) const
  {
    if ( (Vect3d.x == x) && (Vect3d.y == y) && (Vect3d.z == z) ) return(true);
    return(false);
  }

  inline bool operator != (const ZVector3d & Vect3d) const
  {
    if ( (Vect3d.x == x) && (Vect3d.y == y) && (Vect3d.z == z) ) return(false);
    return(true);
  }

  inline ZVector3d & Rotate( double yaw, double pitch, double roll)
  {
    double radconv = 57.295779513;
    double xp1,zp1,yp1;
    yaw /= radconv;
    pitch /= radconv;
    roll /= radconv;

    // Y axis rotation
    zp1  = z*cos(yaw) - x*sin(yaw);
    xp1  = z*sin(yaw) + x*cos(yaw);

    // X axis rotation

    yp1 = y*cos(pitch) - zp1*sin(pitch);
    z   = y*sin(pitch) + zp1*cos(pitch);

    // Z axis rotation

    x = xp1*cos(roll) - yp1*sin(roll);
    y = xp1*sin(roll) + yp1*cos(roll);

    return(*this);
  }

  class ZTransformParam
  {
    public:
      double Sin_Yaw, Cos_Yaw, Sin_Pitch, Cos_Pitch, Sin_Roll, Cos_Roll;
      double Translate_x,Translate_y, Translate_z;
      inline void SetRotation(double Yaw, double Pitch, double Roll)
      {
        double radconv = 57.295779513;
        Yaw/=radconv;
        Sin_Yaw = sin(Yaw);
        Cos_Yaw = cos(Yaw);
        Pitch/=radconv;
        Sin_Pitch = sin(Pitch);
        Cos_Pitch = cos(Pitch);
        Roll/=radconv;
        Sin_Roll = sin(Roll);
        Cos_Roll = cos(Roll);
      }
      inline void SetTranslation(double x, double y, double z)
      {
        Translate_x = x;
        Translate_y = y;
        Translate_z = z;
      }

  };

  // Optimized version

  inline void Transform( ZTransformParam * FastParam )
  {
    register double xp1,zp1,yp1;

    x += FastParam->Translate_x;
    y += FastParam->Translate_y;
    z += FastParam->Translate_z;

    // Y axis rotation
    zp1  = z*FastParam->Cos_Yaw - x*FastParam->Sin_Yaw;
    xp1  = z*FastParam->Sin_Yaw + x*FastParam->Cos_Yaw;

    // X axis rotation

    yp1 = y*FastParam->Cos_Pitch - zp1*FastParam->Sin_Pitch;
    z   = y*FastParam->Sin_Pitch + zp1*FastParam->Cos_Pitch;

    // Z axis rotation

    x = xp1*FastParam->Cos_Roll - yp1*FastParam->Sin_Roll;
    y = xp1*FastParam->Sin_Roll + yp1*FastParam->Cos_Roll;
  }

};


#endif /* Z_ZTYPE_ZVECTOR3D_H */
