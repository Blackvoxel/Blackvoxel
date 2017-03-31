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
 * ZVoxelTypeManager.h
 *
 *  Created on: 3 oct. 2011
 *      Author: laurent
 */

#ifndef Z_ZVOXELTYPEMANAGER_H
#define Z_ZVOXELTYPEMANAGER_H

//#ifndef Z_ZVOXELTYPEMANAGER_H
//#  include "ZVoxelTypeManager.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZOBJECT_H
#  include "z/ZObject.h"
#endif

#ifndef Z_ZVOXELTYPE_H
#  include "ZVoxelType.h"
#endif

#ifndef Z_ZFASTBIT_ARRAY_H
#  include "z/ZFastBit_Array.h"
#endif

#ifndef Z_ZFABMACHINEINFOS_H
#  include "ZFabMachineInfos.h"
#endif



class ZGame;

class ZVoxelTypeManager : public ZObject
{
  ZGame * GameEnv;

  ULong LoadedTexturesCount;

  public:

    ZVoxelType ** VoxelTable;
    ZFastBit_Array_64k * ActiveTable;


    ULong GetTexturesCount() { return(LoadedTexturesCount); }

    ZVoxelTypeManager()
    {
      ULong i;
      GameEnv = 0;
      LoadedTexturesCount = 0;
      ActiveTable = new ZFastBit_Array_64k;
      ActiveTable->Clear();
      VoxelTable = new ZVoxelType * [65536];
      for (i=0;i<65536;i++)
      {
        VoxelTable[i]=0;
      }

    }

    virtual ~ZVoxelTypeManager()
    {
      ULong i;

      for (i=0;i<65536;i++)
      {
        if (VoxelTable[i])
        {
          if (!VoxelTable[i]->VoxelType == i) {delete VoxelTable[i]; VoxelTable[i]=0;}
        }
        if (ActiveTable) { delete ActiveTable; ActiveTable = 0; }
      }
      delete [] VoxelTable;
      VoxelTable = 0;
    }


    void AddVoxelType(UShort TypeNum, ZVoxelType * VoxelType)
    {
      if (VoxelTable[TypeNum])
      {
        if (VoxelTable[TypeNum]->VoxelType == TypeNum) delete VoxelTable[TypeNum];
      }
      VoxelTable[TypeNum] = VoxelType;
      if (VoxelType) VoxelType->VoxelType = TypeNum;
      ActiveTable->Set(TypeNum,VoxelType->Is_Active);
    }

    void SetGameEnv(ZGame * GameEnv)
    {
      this->GameEnv = GameEnv;
    }



    void FillZeroSlots(UShort VoxelTypeUsedToFill)
    {
      ULong i;

      // Mark this voxeltype as a null voxeltype

      VoxelTable[VoxelTypeUsedToFill]->Is_NoType = true;

      // Fill the empty slots to point to this voxeltype;

      for (i=0;i<65536;i++)
      {
        if (VoxelTable[i]==0) VoxelTable[i] = VoxelTable[VoxelTypeUsedToFill];
      }
    }

    ZVoxelType * GetVoxelType(UShort TypeNum)
    {
      return( VoxelTable[TypeNum] );
    }

    Bool LoadVoxelTypes();

    /*
    ULong CreateVoxelExtension(UShort VoxelType)
    {
      ZVoxelType * VoxelTypeEntry;

      VoxelTypeEntry = VoxelTable[VoxelType]; if (!VoxelTypeEntry) return(0);
      return(VoxelTypeEntry->CreateVoxelExtension());
    }
*/
    void DeleteVoxelExtension(UShort VoxelType, ULong VoxelExtension)
    {
      ZVoxelType * VoxelTypeEntry;

      VoxelTypeEntry = VoxelTable[VoxelType]; if (!VoxelTypeEntry) return;
      if (VoxelTypeEntry->ExtensionType == 0) return;
      VoxelTypeEntry->DeleteVoxelExtension(VoxelExtension);
    }

    void DumpInfos();

    void OutFabInfos();

    bool _Internal_CompareTransTables( ZString & Message, ZFabInfos * Fab,  ZFabInfos::ZTransformation * Tr1,  ZFabInfos::ZTransformation * Tr2 );
    bool _Internal_CompareTransTables_V2( ZString & Message, ZFabInfos * Fab,  ZFabInfos::ZTransformation * Tr1,  ZFabInfos::ZTransformation * Tr2 );

    void FindFabConflics();
    void FindFabConflicts_V2();

};



#endif /* Z_ZVOXELTYPEMANAGER_H */
