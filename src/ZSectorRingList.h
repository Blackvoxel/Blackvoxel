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
#ifndef Z_ZSECTORRINGLIST_H
#define Z_ZSECTORRINGLIST_H

//#ifndef Z_ZSECTORRINGLIST_H
//#  include "ZSectorRingList.h"
//#endif


class ZSectorRingList : public ZObject
{
    private:
      ZVoxelSector ** SectorList;
      volatile ULong ListSize;
      volatile ULong WrapMask;
      volatile ULong Start;
      volatile ULong End;
      volatile ULong nEntries;

    public:

      ZSectorRingList(ULong SizeInPowerOfTwo)
      {
        ListSize = SizeInPowerOfTwo;
        WrapMask = ListSize - 1;
        Start = End = nEntries = 0;
        SectorList = new ZVoxelSector * [ListSize];
      }

      ~ZSectorRingList()
      {

        if (SectorList)
        {
          delete [] SectorList;
        }
        ListSize = WrapMask = Start = End = nEntries = 0;
      }

      inline bool PushToList( ZVoxelSector * Sector )
      {
        if (nEntries > (ListSize - 4)) return(false);
        SectorList[End] = Sector;
        End = (End + 1) & WrapMask;
        nEntries ++;
        // printf("Push");
        return(true);
      }

      inline bool IsNotFull()
      {
        if (nEntries > (ListSize - 4)) return(false);
        return(true);
      }

      inline ZVoxelSector * PullFromList()
      {
        ZVoxelSector * Sector;

        if (Start == End) return(0);

        Sector = SectorList[Start];SectorList[Start]=0;

        Start = (Start + 1) & WrapMask;
        if (nEntries) nEntries --;

        // printf("Pull");

        return(Sector);
      }

      void FreeRemainingContent()
      {
        ZVoxelSector * Sector;
        while ( (Sector = PullFromList()) )
        {
          delete Sector;
        }
      }

      ULong debug_getstart() {return( Start); }
      ULong debug_GetEnd()   {return( End); }
      ULong debug_GetnEntries() {return(nEntries);}
  };


#endif
