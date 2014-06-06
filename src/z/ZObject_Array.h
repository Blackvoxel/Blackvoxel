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
/*******************************************************************************
*                                                                              *
*                         GNU GPL V3 Free Software                             *
*                                                                              *
*  (C) 1997-2011 Laurent Thiebaut & Olivia Merle                               *
*                                                                              *
*  This file is part of EXecutePage.                                           *
*                                                                              *
*  EXecutePage is free software: you can redistribute it and/or modify         *
*  it under the terms of the GNU General Public License as published by        *
*  the Free Software Foundation, either version 3 of the License, or           *
*  (at your option) any later version.                                         *
*                                                                              *
*  NodeKiller is distributed in the hope that it will be useful,               *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of              *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
*  GNU General Public License for more details.                                *
*                                                                              *
*  You should have received a copy of the GNU General Public License           *
*  along with EXecutePage.  If not, see <http://www.gnu.org/licenses/>.        *
*                                                                              *
*******************************************************************************/

#ifndef Z_ZOBJECT_ARRAY_H
#define Z_ZOBJECT_ARRAY_H

//#ifndef Z_ZOBJECT_ARRAY_H
//#  include "ZObject_Array.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

#ifndef Z_ZOBJECT_H
#  include "ZObject.h"
#endif

#ifndef Z_ZGENERAL_OBJECT_H
#  include "ZGeneralObject.h"
#endif

class ZObjectArray : public ZGeneralObject
{
  protected:
    enum {INITIALSIZE = 80};
    ZObject * * ExpandableArray;
    ZSize MemSize;
    ZSize MaxIndex;
    ZSize NavIndex;
  public:
    ZObjectArray();
    virtual   ~ZObjectArray();
    void      Clear();
    ZObject * GetEntry(ZSize Index);
    Bool      SetEntry(ZSize Index, ZObject & Data);
    Bool      Add(ZObject & Data);

    void      StartEnum() {NavIndex = 0;}
    Bool      Enum(ZObject **Entry);
    ULong     GetLastEnumOffset();
    ULong     GetCount();
/*
    Bool      Save(ZStream & Stream) {return(ZTRUE);};
    Bool      Load(ZStream & Stream) {return(ZFALSE);};
    ULong     GetSavedLen() {return(0);};
*/
    void  BubbleSort( int (* CompareFunc )(ZObject * Object_1, ZObject * Object_2) )
    {
      ZSize i, Limit;
      int Res;
      bool NotYetSorted;
      ZObject * Temp;

      if (MaxIndex < 2) return;
      Limit = MaxIndex-1;
      do
      {
        NotYetSorted = false;
        for(i=0;i<Limit;i++)
        {
          Res = CompareFunc(ExpandableArray[i], ExpandableArray[i+1]);
          if (Res > 0) { NotYetSorted = true; Temp = ExpandableArray[i]; ExpandableArray[i] = ExpandableArray[i+1]; ExpandableArray[i+1] = Temp; }
        }
      } while (NotYetSorted);
    }
};

#endif
