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
 * ZGenericTable.h
 *
 *  Created on: 3 déc. 2012
 *      Author: laurent
 */

#ifndef Z_ZGENERICTABLE_H
#define Z_ZGENERICTABLE_H

//#ifndef Z_ZGENERICTABLE_H
//#  include "ZGenericTable.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif


template <typename Type, ZMemSize TableLen>
class ZGenericTable
{
  protected:
    Type Table[TableLen];
    ZMemSize ElementCount;

  public:

    inline ZGenericTable() { ElementCount = 0; }

    inline void Clear() { ElementCount = 0; }

    inline bool AddToTable( Type * Element )
    {
      if (ElementCount >= TableLen) return(false);
      Table[ElementCount++] = *Element;
      return(true);
    }

    inline bool CreateNew(ZMemSize & ElementIndex)
    {
      if (ElementCount >= TableLen) return(false);
      ElementIndex = ElementCount++;
      return(true);
    }

    inline Type * GetElement(ZMemSize TablePos)
    {
      return( &Table[TablePos] );
    }

    inline ZMemSize GetCount()
    {
      return(ElementCount);
    }

    inline bool IsElementInTable( Type * Element )
    {
      ZMemSize i;

      for (i=0;i<ElementCount;i++)
      {
        if (Table[i]==*Element) return(true);
      }
      return(false);
    }

    inline bool FindElement( Type * Element, ZMemSize * Pos )
    {
      ZMemSize i;

      for (i=0;i<ElementCount;i++)
      {
        if (Table[i] == *Element) { *Pos = i; return(true); }
      }
      return(false);
    }

};




#endif /* Z_ZGENERICTABLE_H */
