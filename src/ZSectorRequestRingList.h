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
 * ZSectorRequestRingList.h
 *
 *  Created on: 26 mars 2011
 *      Author: laurent
 */

#ifndef Z_ZSECTORREQUESTRINGLIST_H
#define Z_ZSECTORREQUESTRINGLIST_H

//#ifndef Z_ZSECTORREQUESTRINGLIST_H
//#  include "ZSectorRequestRingList.h"
//#endif

struct ZSectorRequest { Long x, y, z; };

class ZSectorRequestRingList
{
  public:
    ZSectorRequest * RequestList;
    ULong ListSize;
    ULong WrapMask;
    volatile ULong Start, End, nEntries;

  public:

    ZSectorRequestRingList()
    {
      ListSize = 128;
      WrapMask = ListSize - 1;
      Start = End = nEntries = 0;
      RequestList = new ZSectorRequest[ListSize];

    }

    ~ZSectorRequestRingList()
    {
      if (RequestList) delete [] RequestList;
      ListSize = WrapMask = Start = End = nEntries = 0;
    }

    inline void PushToList( Long x, Long y, Long z )
    {
      // if (nEntries > (ListSize - 4)) return;
      if (((End + 1) & WrapMask ) == Start) return;
      RequestList[End].x = x; RequestList[End].y = y; RequestList[End].z = z;
      End = (End + 1) & WrapMask;
      // nEntries ++;
    }

    inline bool PullFromList( Long & x, Long & y, Long & z )
    {
      if (Start == End) return(false);


      x = RequestList[Start].x; y = RequestList[Start].y; z = RequestList[Start].z;
      Start = (Start + 1) & WrapMask;
      // nEntries --;
      return(true);
    }

    bool IsDataReady() { return(Start!=End); }
};


#endif
