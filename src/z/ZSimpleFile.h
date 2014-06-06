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
 * ZGeneric_SimpleList.h
 *
 *  Created on: 26 oct. 2011
 *      Author: laurent
 */

#ifndef Z_SIMPLEFILE_H
#define Z_SIMPLEFILE_H

// #ifndef Z_SIMPLEFILE_H
// #include "ZSimpleFile.h"
// #endif

#ifndef Z_ZMEMPOOL_H
#  include "ZMemPool.h"
#endif

class ZSimpleFile
{
  protected:

    static ZMonoSizeMemoryPool MemPool;

    struct ZElement
    {
      ZElement * Next;
      ZElement * Pred;
      void * Data;
    };

    ZElement Head;
    ZElement Tail;
    ULong Count;

  public:

    ZSimpleFile()
    {
      Head.Next = &Tail;
      Tail.Pred = &Head;
      Count = 0;
    }

    ~ZSimpleFile()
    {
      ZElement * Element, * OldElement;
      Element = Head.Next;
      while(Element != &Tail)
      {
        OldElement = Element;
        Element = Element->Next;
        MemPool.FreeMem(OldElement);
      }

      Tail.Pred = &Head;
      Tail.Next = &Tail;
      Count = 0;
    }

    inline void PushToFile(void * Data)
    {
      ZElement * Element;

      Element = (ZElement * )MemPool.AllocMem(sizeof(ZElement));
      Element->Data = Data;

      Element->Next = Head.Next;
      Element->Pred = &Head;
      Head.Next->Pred = Element;
      Head.Next = Element;
      Count++;
    }

    inline void * PullFromFile()
    {
      void * Data;
      ZElement * Element;

      if (Tail.Pred == &Head) return(0);
      Element = Tail.Pred;
      Element->Pred->Next = Element->Next;
      Element->Next->Pred = Element->Pred;
      Data = Element->Data;
      MemPool.FreeMem(Element);

      Count--;
      return(Data);
    }

    inline ULong GetCount()
    {
      return(Count);
    }
};




#endif /* Z_SIMPLEFILE_H */
