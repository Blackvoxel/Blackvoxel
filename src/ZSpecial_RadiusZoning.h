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
 * ZSpecial_RadiusZoning.h
 *
 *  Created on: 23 mars 2011
 *      Author: laurent
 */

#ifndef Z_ZSPECIAL_RADIUSZONING_H
#define Z_ZSPECIAL_RADIUSZONING_H

//#ifndef Z_ZSPECIAL_RADIUSZONING_H
//#  include "ZSpecial_RadiusZoning.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZOBJECT_H
#  include "z/ZObject.h"
#endif

#ifndef _MATH_H
#  include <math.h>
#endif

#ifndef _STDIO_H
#  include <stdio.h>
#endif

class ZRadius_Zoning : public ZObject
{
  private:

    UByte * ZoneData;
    ULong   ZoneSize;
    ZVector3L Size;
    ZVector3L ArraySize;

    //double   Mid_x,Mid_y,Mid_z;

  public:

    ZRadius_Zoning()
    {
      ZoneData = 0;
      ZoneSize = 0;
    }

    ~ZRadius_Zoning()
    {
      delete [] (ZoneData);
    }

    void SetSize( ULong Size_x, ULong Size_y, ULong Size_z )
    {
      ULong i;

      Size.x = Size_x; Size.y = Size_y; Size.z = Size_z;
      ArraySize.x = (Size.x << 1) + 1;
      ArraySize.y = (Size.y << 1) + 1;
      ArraySize.z = (Size.z << 1) + 1;


      ZoneSize = (ULong)( ArraySize.x * ArraySize.y * ArraySize.z );
      if (ZoneData) delete [] ZoneData;
      ZoneData = new UByte[ZoneSize];

      for (i=0;i<ZoneSize;i++) ZoneData[i]=0;
    }



    void DrawZones(double ZoneRadius, UByte ZoneMark)
    {
      Long x,y,z;
      ULong Off_y,Off_z,Inc_y,Inc_z;
      ZVector3d Vector;
      double Distance;

      Inc_y = ArraySize.x;
      Inc_z = ArraySize.x * ArraySize.y;

      for (x=0;x<ArraySize.x;x++)
        for (y=0,Off_y=0;y<ArraySize.y;y++,Off_y += Inc_y)
          for (z=0,Off_z=0;z<ArraySize.z;z++, Off_z+= Inc_z)
          {
            Vector.x = x - Size.x; Vector.y = y - Size.y; Vector.z = z - Size.z;
            Distance = sqrt( Vector.x * Vector.x + Vector.y * Vector.y + Vector.z * Vector.z );
            if (Distance < ZoneRadius) ZoneData[ x + Off_y + Off_z ] = ZoneMark;
          }

    }

/*
    void DrawZones( double Zone1, double Zone2, double Zone3, double Zone4)
    {
      double x,y,z;
      ULong  xe,ye,ze;
      double sx = (double)Size_x,sy = (double)Size_y,sz = (double)Size_z;
      double mpx = sx / 2 - 0.5, mpy = sy / 2 - 0.5, mpz = sz / 2 - 0.5;
      double pointdist;
      UByte  Zone;
      ULong  Offset;

      double Probe[20];

      for (x=0,xe=0 ; xe<Size_x ; x++,xe++)
        for (y=0,ye=0 ; ye<Size_y ; y++,ye++)
          for (z=0,ze=0 ; ze<Size_z ; z++,ze++)
          {
            pointdist = sqrt(  ((mpx-x) * (mpx-x)) + ((mpy-y) * (mpy-y)) + ((mpz-z) * (mpz-z)) );
            if ( y == (Size_y/2)  && x == (Size_x/2))
            {
              Probe[ze] = pointdist;
            }
            if      (pointdist <Zone4 ) Zone = 4;
            else if (pointdist <Zone3 ) Zone = 3;
            else if (pointdist <Zone2 ) Zone = 2;
            else if (pointdist <Zone1 ) Zone = 1;
            else                        Zone = 0;

            Offset = xe + ye * Size_x + ze * (Size_x * Size_y);
            ZoneData[Offset] = Zone;
          }

      // printf("Probe:");
      // for ( Offset=0 ; Offset<20 ; Offset++) printf(" %ld:[%lf]", Offset, Probe[Offset]);
    }
*/
    inline ULong GetZone(Long x, Long y, Long z)
    {
      ULong  Offset = Size.x + x + (Size.y + y) * ArraySize.x + (Size.z + z) * (ArraySize.x * ArraySize.y);

      return( ZoneData[Offset] );
    }

    void DebugOut()
    {
      Long x,y,z;
      ULong Offset;

      printf("---------------------------------------------------\n");
        y = ArraySize.y/2;
        for (x=0;x<ArraySize.x;x++)
        {
          for (z=0;z<ArraySize.z;z++)
          {
            Offset = x + y * ArraySize.x + z * (ArraySize.x * ArraySize.y);
            printf("%d", ZoneData[Offset]);
          }
          printf("\n");
        }

        z = ArraySize.z/2;
        for (x=0;x<ArraySize.x;x++)
        {
          for (y=0;y<ArraySize.y;y++)
          {
            Offset = x + y * ArraySize.x + z * (ArraySize.x * ArraySize.y);
            printf("%d", ZoneData[Offset]);
          }
          printf("\n");
        }
    }
};



#endif
