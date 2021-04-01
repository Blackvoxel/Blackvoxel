/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2021 Laurent Thiebaut & Olivia Merle
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
 * ZFabMachineInfos2.h
 *
 *  Created on: 2 mars 2021
 *      Author: laurent
 */

#ifndef Z_ZFABMACHINEINFOS2_H
#define Z_ZFABMACHINEINFOS2_H

//#ifndef Z_ZFABMACHINEINFOS2_H
//#  include "ZFabMachineInfos2.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

#ifndef Z_ZFASTBIT_ARRAY_H
#  include "z/ZFastBit_Array.h"
#endif

#define ZFABINFO2_MAXFABRESULT 16
#define ZFABINFO2_MAXFABMATERIALS 32
#define ZFABINFO2_MAXMACHINEMATERIALS 64

#include <stdio.h>

class ZFabInfos2
{
  public:

    class ZFabEntry
    {
      public:
        UShort  VoxelType;
        UShort  Quantity;
    };

    class ZTransformation
    {
      public:
        ZTransformation * Next;
        ULong       FabListEntryCount;
        ULong       ResultCount;
        ZFabEntry   FabList[ZFABINFO2_MAXFABMATERIALS];
        UShort      Result_VoxelType[ZFABINFO2_MAXFABRESULT];
        ULong       Result_Quantity[ZFABINFO2_MAXFABRESULT];

        ZTransformation()
        {
          ULong i;

          Next = 0;
          FabListEntryCount = 0;
          ResultCount = 0;

          for (i=0;i<ZFABINFO2_MAXFABRESULT;i++)
          {
            Result_VoxelType[i]=0;
            Result_Quantity[i]=0;
          }

          for (i=0;i<ZFABINFO2_MAXFABMATERIALS;i++)
          {
            FabList[i].VoxelType = 0;
            FabList[i].Quantity  = 0;
          }

        }

        ~ZTransformation()
        {
          ULong i;

          if (Next) {delete Next; Next = 0;}

          FabListEntryCount = 0;
          ResultCount = 0;

          for (i=0;i<ZFABINFO2_MAXFABRESULT;i++)
          {
            Result_VoxelType[i]=0;
            Result_Quantity[i]=0;
          }

          for (i=0;i<ZFABINFO2_MAXFABMATERIALS;i++)
          {
            FabList[i].VoxelType = 0;
            FabList[i].Quantity  = 0;
          }
        }

        void AddCondition(UShort VoxelType, ULong Quantity)
        {
          if (FabListEntryCount>=ZFABINFO2_MAXFABMATERIALS)
          {
            printf("ERROR : Maximum Material Count reached.\n");
            return;
          }

          FabList[FabListEntryCount].VoxelType = VoxelType;
          FabList[FabListEntryCount].Quantity  = Quantity;
          FabListEntryCount++;
        }

        void AddResult(UShort VoxelType, ULong Quantity)
        {
          if (ResultCount >= ZFABINFO2_MAXFABRESULT)
          {
            printf("ERROR : Maximum Result Count reached.\n");
            return;
          }

          Result_VoxelType[ResultCount]=VoxelType;
          Result_Quantity[ResultCount] =Quantity;
          ResultCount++;
        }

        ZTransformation * GetNext()
        {
          return(Next);
        }

        ULong GetEntryCount() {return(FabListEntryCount);}
        ZFabEntry * GetEntry(ULong EntryNum) {return(&FabList[EntryNum]);}
        bool MatchEntry(UShort VoxelType, ULong Quantity)
        {
          ULong i;

          for (i=0;i<ZFABINFO2_MAXFABMATERIALS;i++)
          {
            if (FabList[i].VoxelType == VoxelType && FabList[i].Quantity == Quantity) return(true);
          }

          return(false);
        }

        ULong GetResultCount() {return(ResultCount);}
        ULong GetMaxResultCount() {return(ZFABINFO2_MAXFABRESULT);}
        bool GetResult(ULong Index, UShort & VoxelType, ULong & Quantity)
        {
          if (Index > ResultCount) {VoxelType = 0; Quantity=0; return(false); }

          VoxelType = Result_VoxelType[Index];
          Quantity  = Result_Quantity[Index];
          return(true);
        }


    };


  protected:

    ZTransformation * TransformationList;
    UShort            ValidatorVoxelType;
    ZFastBit_Array    MaterialTag;
    UShort            MaterialTable[ZFABINFO2_MAXMACHINEMATERIALS];
    UShort            MaterialCount;

  public:

    ZFabInfos2()
    {
      TransformationList = 0;
      ValidatorVoxelType = 1;
      MaterialTag.InitSize(65536);
      MaterialCount = 0;
    }

    ~ZFabInfos2()
    {
      if ( TransformationList ) delete TransformationList;
    }

    ZTransformation * AddTransformation()
    {
      ZTransformation * Tr;

      if (!TransformationList) {TransformationList = new ZTransformation; return(TransformationList);}

      Tr = TransformationList;
      while(Tr->Next) Tr=Tr->Next;
      Tr->Next = new ZTransformation;
      return(Tr->Next);
    }

    void SetValidator(UShort VoxelType)
    {
      ValidatorVoxelType = VoxelType;
    }

    UShort GetValidator() { return (ValidatorVoxelType); }

    void _AddMaterialToTable(UShort VoxelType)
    {
      ULong i;

      for (i=0;i<ZFABINFO2_MAXMACHINEMATERIALS;i++)
      {
        if (i==MaterialCount)
        {
          MaterialTable[i] = VoxelType;
          return;
        }

        if (MaterialTable[i] == VoxelType) return;
      }
      printf("ERROR : Material table for machine is full\n");
      return;
    }

    void UpdateAll()
    {
      ULong i;
      ZTransformation * Transformation;
      UShort VoxelType;

      MaterialTag.Clear();
      MaterialCount = 0;
      for (Transformation = this->TransformationList; Transformation; Transformation=Transformation->Next)
      {
        for(i=0;i<Transformation->FabListEntryCount;i++)
        {
          VoxelType = Transformation->FabList[i].VoxelType;
          MaterialTag.Set(VoxelType,true);
          _AddMaterialToTable(VoxelType);
        }
      }
    }

    bool IsMaterialAccepted(UShort VoxelType)
    {
      return(MaterialTag.Get(VoxelType));
    }

    ZTransformation * GetFirstTransformation()
    {
      return(TransformationList);
    }


};



#endif /* Z_ZFABMACHINEINFOS2_H */
