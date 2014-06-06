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
 * ZPointList.h
 *
 *  Created on: 13 mai 2013
 *      Author: laurent
 */

#ifndef Z_ZPOINTLIST_H
#define Z_ZPOINTLIST_H

//#ifndef Z_ZPOINTLIST_H
//#  include "ZPointList.h"
//#endif


#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

class ZPointList
{
    class ZPointListEntry
    {
      public:

      ZVector3L Position;
      bool      Used;
    };

  public:

    ZPointList()
    {
      ULong i;

      for (i=0; i<16;i++)
      {
        List[i].Position = 0;
        List[i].Used = false;
      }
      Pointer = 0;
      EntryCount = 0;
    }

    ZPointListEntry List[16];
    UShort Pointer;
    ULong  EntryCount;

    void AddPosition( Long x, Long y, Long z )
    {
      List[Pointer].Position.x = x;
      List[Pointer].Position.y = y;
      List[Pointer].Position.z = z;
      List[Pointer].Used = true;
      Pointer = (Pointer + 1) & 0xF;
      EntryCount ++;
    }

    ZVector3L * GetPosition(ULong PositionNum)
    {
      ZPointListEntry * Entry;

      Entry = &List[ (Pointer - PositionNum) & 0xF ];

      if (!Entry->Used) return(0);

      return ( &Entry->Position );
    }

    ULong GetPositionCount()
    {
      return(EntryCount);
    }

};



#endif /* Z_ZPOINTLIST_H */
