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
 * ZFabMachineInfos.h
 *
 *  Created on: 23 déc. 2012
 *      Author: laurent
 */

#ifndef Z_ZFABMACHINEINFOS_H
#define Z_ZFABMACHINEINFOS_H

//#ifndef Z_ZFABMACHINEINFOS_H
//#  include "ZFabMachineInfos.h"
//#endif


class ZFabInfos
{
  public:
    class ZFabMaterial
    {
      public:
        UShort VoxelType;
        UShort SlotNum;
    };


    class ZFabEntry
    {
      public:
        UShort  Index;
        UShort  Quantity;
    };
    class ZTransformation
    {
      public:
      enum {Max_ResultVoxels=16};
      ZFabEntry * FabList;
      ULong       EntryCount;
      ULong       MaxEntryCount;
      UShort      Result_VoxelType[Max_ResultVoxels];
      ULong       Result_Quantity[Max_ResultVoxels];


      ZTransformation()
      {
        ZMemSize i;

        FabList = 0;
        EntryCount = 0;
        MaxEntryCount = 0;

        for (i=0; i<Max_ResultVoxels; i++) { Result_VoxelType[i] = 0; Result_Quantity[i] = 0; }
      }
      ~ZTransformation()
      {
        if (FabList) delete [] FabList;
        EntryCount = 0;
        MaxEntryCount = 0;
      }
    };

    ZFabMaterial *    MaterialTable;
    ULong             MaterialCount;
    ULong             MaxMaterialCount;
    ZTransformation * TransformationTable;
    ULong             TransformationCount;
    ULong             MaxTransformationCount;
    UShort            PurgeSlot;
    ULong             PurgeCount;
    UShort            ValidationSlot;
    ULong             ValidationCount;

    ZFabInfos()
    {
      MaterialTable = 0;
      MaterialCount    = 0;
      MaxMaterialCount = 0;

      TransformationTable    = 0;
      TransformationCount    = 0;
      MaxTransformationCount = 0;
      PurgeSlot = 0;
      PurgeCount = 1000000;
      ValidationSlot = 0;
      ValidationCount = 1;
    }

    ~ZFabInfos()
    {
      if (MaterialTable)
      {
        delete MaterialTable;
        MaterialTable = 0;
      }
      if (TransformationTable)
      {
        delete [] TransformationTable;
        TransformationTable = 0;
      }

      MaterialCount    = 0;
      MaxMaterialCount = 0;
      TransformationCount    = 0;
      MaxTransformationCount = 0;
    }

    void AddMaterial(UShort VoxelType)
    {
      ZMemSize i;
      if (!MaterialTable)
      {
        MaxMaterialCount = 128; MaterialTable = new ZFabMaterial[MaxMaterialCount];
        for(i=0;i<MaxMaterialCount;i++) {MaterialTable[i].SlotNum = 0; MaterialTable[i].VoxelType =0;}
      }
      if (MaterialCount < MaxMaterialCount)
      {
        MaterialTable[MaterialCount].SlotNum   = MaterialCount;
        MaterialTable[MaterialCount].VoxelType = VoxelType;
        MaterialCount++;
      }
    }

    ULong AddTransformation()
    {
      ZMemSize i;
      if (!TransformationTable)
      {
        TransformationCount    = 0;
        MaxTransformationCount = 64;
        TransformationTable = new ZTransformation[MaxTransformationCount];
      }
      if (TransformationCount < MaxTransformationCount)
      {
        for (i=0;i<ZTransformation::Max_ResultVoxels;i++) {TransformationTable[TransformationCount].Result_VoxelType[i]=0;TransformationTable[TransformationCount].Result_Quantity[i]=0;}
        return(TransformationCount++);
      }
      return(-1);
    }

    void AddCondition(ULong TransformationNum, UShort Index, UShort Quantity)
    {
      if (!TransformationTable) return;
      if (!TransformationTable[TransformationNum].FabList)
      {

        TransformationTable[TransformationNum].MaxEntryCount = 16;
        TransformationTable[TransformationNum].EntryCount    = 0;
        TransformationTable[TransformationNum].FabList = new ZFabEntry[ TransformationTable[TransformationNum].MaxEntryCount ];
      }

      if ( TransformationTable[TransformationNum].EntryCount <  TransformationTable[TransformationNum].MaxEntryCount )
      {
        TransformationTable[TransformationNum].FabList[ TransformationTable[TransformationNum].EntryCount ].Index = Index;
        TransformationTable[TransformationNum].FabList[ TransformationTable[TransformationNum].EntryCount ].Quantity = Quantity;
        TransformationTable[TransformationNum].EntryCount++;
      }
    }

    void SetResult(ULong TransformationNum, UShort ResultNum, UShort VoxelType, ULong Quantity)
    {
      if (ResultNum < ZTransformation::Max_ResultVoxels)
      {
        TransformationTable[TransformationNum].Result_VoxelType[ResultNum] = VoxelType;
        TransformationTable[TransformationNum].Result_Quantity[ResultNum] = Quantity;
      }
    }

    void SetPurgeCondition(UShort Index, ULong Quantity)
    {
      PurgeSlot  = Index;
      PurgeCount = Quantity;
    }
    void SetValidationCondition(UShort Index, ULong Quantity)
    {
      ValidationSlot  = Index;
      ValidationCount = Quantity;
    }

};



#endif /* Z_ZFABMACHINEINFOS_H */
