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
 * ZSectorTagHash.h
 *
 *  Created on: 26 mars 2011
 *      Author: laurent
 */

#ifndef Z_ZSECTORTAGHASH_H
#define Z_ZSECTORTAGHASH_H

//#ifndef Z_ZSECTORTAGHASH_H
//#  include "ZSectorTagHash.h"
//#endif

class ZSectorTagHash
{
  private:
    enum {Tag_Size_x = 256,
          Tag_Size_y = 16,
          Tag_Size_z = 256,
          Tag_Mask_x = 0xff,
          Tag_Mask_y = 0xf,
          Tag_Mask_z = 0xff,
          Tag_Shift_x = 8,
          Tag_Shift_y = 4,
          Tag_Shift_z = 8
    };

    struct TagEntry
    {
      TagEntry * Next;
      TagEntry * Prev;

      Long x,y,z;
    };

    TagEntry ** TagHash;
    ZMemSize    TagHashSize;

    static ZMonoSizeMemoryPool DefaultMemoryPool;
    ZMemoryPool * MemPool;
  public:

    ZSectorTagHash()
    {
      ZMemSize i;

      MemPool = & DefaultMemoryPool;

      TagHashSize = Tag_Size_x * Tag_Size_y * Tag_Size_z;
      TagHash = new TagEntry * [TagHashSize];
      for (i=0;i<TagHashSize;i++) TagHash[i] = 0;
    }

    ~ZSectorTagHash()
    {
      ZMemSize i;
      TagEntry * Entry, * NewEntry;

      for (i=0;i<TagHashSize;i++)
      {
        Entry = TagHash[i];
        while (Entry)
        {
          NewEntry = Entry->Next;
          MemPool->FreeMem(Entry);
          Entry = NewEntry;
        }
      }
      delete [] TagHash;
      TagHash = 0;
    }

    bool inline find (Long x, Long y, Long z)
    {
      TagEntry * Entry;
      ULong Offset;

      Offset =  ( (ULong)x & Tag_Mask_x)
              + ( ((ULong)y << Tag_Shift_x) & Tag_Mask_y)
              + (((ULong)z << (Tag_Shift_x + Tag_Shift_y)) & Tag_Mask_z);

      Entry = TagHash[Offset];
      while(Entry)
      {
        if ( (Entry->x == x) && (Entry->y == y) && (Entry->z == z) ) return(true);
        Entry = Entry->Next;
      }
      return(false);

    }

    void Add( Long x, Long y, Long z )
    {
      TagEntry * Entry;
      ULong Offset;

      // Offset = x + (y << Tag_Shift_x) + (z << (Tag_Shift_x + Tag_Shift_y));

      Offset =  ( (ULong)x & Tag_Mask_x)
              + ( ((ULong)y << Tag_Shift_x) & Tag_Mask_y)
              + (((ULong)z << (Tag_Shift_x + Tag_Shift_y)) & Tag_Mask_z);


      Entry = TagHash[Offset];
      while(Entry)
      {
        if ( (Entry->x == x) && (Entry->y == y) && (Entry->z == z) ) return;
        Entry = Entry->Next;
      }

      Entry = (TagEntry * )MemPool->AllocMem(sizeof(TagEntry));

      Entry->x = x; Entry->y = y; Entry->z = z;

      Entry->Prev = 0;
      Entry->Next = TagHash[Offset];
      if (TagHash[Offset]) Entry->Next->Prev = Entry;
      TagHash[Offset] = Entry;
    }

    void Remove( Long x, Long y, Long z )
    {
      TagEntry * Entry;
      ULong Offset;

      //Offset = x + (y << Tag_Shift_x) + (z << (Tag_Shift_x + Tag_Shift_y));
      Offset =  ( (ULong)x & Tag_Mask_x)
              + ( ((ULong)y << Tag_Shift_x) & Tag_Mask_y)
              + (((ULong)z << (Tag_Shift_x + Tag_Shift_y)) & Tag_Mask_z);

      Entry = TagHash[Offset];
      while(Entry)
      {
        if ( (Entry->x == x) && (Entry->y == y) && (Entry->z == z) )
        {
          if (Entry->Prev == 0) TagHash[Offset] = Entry->Next;
          else                  Entry->Prev->Next = Entry->Next;
          if (Entry->Next != 0) Entry->Next->Prev = Entry->Prev;

          MemPool->FreeMem(Entry);

          return;
        }
        Entry = Entry->Next;
      }
    }
};


#endif /* ZSECTORTAGHASH_H_ */
