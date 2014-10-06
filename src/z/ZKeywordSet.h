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
 * ZKeywordSet.h
 *
 *  Created on: Pre-2000 code.
 *      Author: laurent
 */

#ifndef Z_ZKEYWORDSET_H
#define Z_ZKEYWORDSET_H

//#ifndef Z_ZKEYWORDSET_H
//#  include "ZKeywordSet.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

#ifndef Z_ZSTRING_H
#  include "ZString.h"
#endif

#ifndef Z_ZCHARFILTER_H
#  include "ZCharFilter.h"
#endif

// ***********************************************************************
// *                             KeyWord Set                             *
// ***********************************************************************

struct ZKeyWordTable
{
  char * Name;
  ULong Value;
};

class ZKeyWordSet
{
  public:
    virtual bool AddKeyTable(ZKeyWordTable * KeyTable)=0;
    virtual bool AddKeyWord(const char * KeyWord,ULong AssociatedValue) =0;
    virtual ULong SearchKey(char ** File) = 0;
    virtual ~ZKeyWordSet() {};
};


// ***********************************************************************
// *                          Fast KeyWord Set                           *
// ***********************************************************************



class ZFastKeyWordSet : public ZKeyWordSet
{
  struct KeyEntry
  {
    KeyEntry * NextKey; // 0= No Next
    ULong KeyInfo; // 0= No Key;
  };
  KeyEntry * KeyTable;

  public:
    ZCharFilter Filter;
    bool Init();
    bool AddKeyTable(ZKeyWordTable * KeyTable);
    bool AddKeyWord(const char * KeyWord,ULong AssociatedValue);
    ULong SearchKey(char ** File);
    ZFastKeyWordSet() {KeyTable = 0;if (!(KeyTable = NewKeyTable())) throw;}
    ~ZFastKeyWordSet() {DestroyKeyTable();}
  private:
    KeyEntry * NewKeyTable();
    void DestroyKey(KeyEntry * Key);
    void DestroyKeyTable();
};


#endif
