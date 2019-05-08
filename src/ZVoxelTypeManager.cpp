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
 * ZVoxelTypeManager.cpp
 *
 *  Created on: 3 oct. 2011
 *      Author: laurent
 */

#include "ZVoxelTypeManager.h"
#include <stdio.h>


#ifndef Z_ZVOXELTYPE_STORAGE_H
#  include "ZVoxelType_Storage.h"
#endif

#ifndef Z_ZVOXELTYPE_USERTEXTURETRANSFORMER_H
#  include "ZVoxelType_UserTextureTransformer.h"
#endif

#ifndef Z_ZVOXELTYPE_PLANEZ1_H
#  include "ZVoxelType_PlaneZ1.h"
#endif

#ifndef Z_ZVOXELTYPE_YELLOWGAS_H
#  include "ZVoxelType_YellowGas.h"
#endif

#ifndef Z_ZVOXELTYPE_LIGHTTRANSMITTER_H
#  include "ZVoxelType_LightTransmitter.h"
#endif

#ifndef Z_VOXELTYPE_FABMACHINE_H
#  include "ZVoxelType_FabMachine.h"
#endif

#ifndef Z_ZVOXELTYPE_PROGRAMMABLE_H
#  include "ZVoxelType_Programmable.h"
#endif

#ifndef Z_ZVOXELTYPE_PROGROBOT_ASM_H
#  include "ZVoxelType_ProgRobot_Asm.h"
#endif

#ifndef Z_ZVOXELTYPE_FUSIONELEMENT_H
#  include "ZVoxelType_FusionElement.h"
#endif

#ifndef Z_VOXELTYPE_BLASTFURNACE_H
#  include "ZVoxelType_BlastFurnace.h"
#endif

#ifndef Z_ZVOXELTYPE_MININGROBOT_XR1_H
#  include "ZVoxelType_MiningRobot_xr1.h"
#endif

#ifndef Z_ZVOXELTYPE_SEQUENCER_H
#  include "ZVoxelType_Sequencer.h"
#endif

#ifndef Z_VOXELTYPE_PICKER_H
#  include "ZVoxelType_Picker.h"
#endif

#ifndef Z_VOXELTYPE_MOVER_H
#  include "ZVoxelType_Mover.h"
#endif

#ifndef Z_ZVOXELTYPE_EGMY_T1_H
#  include "ZVoxelType_Egmy_T1.h"
#endif

#ifndef Z_ZVOXELTYPE_SIMPLEBUTTON_H
#  include "ZVoxelType_SimpleButton.h"
#endif

#ifndef Z_ZVOXELTYPE_INDICATORLIGHT_H
#  include "ZVoxelType_IndicatorLight.h"
#endif

#ifndef Z_ZVOXELTYPE_RTFM_H
#  include "ZVoxelType_RTFM.h"
#endif

#ifndef Z_ZVOXELTYPE_CONCRETE_H
#  include "ZVoxelType_Concrete.h"
#endif

#ifndef Z_ZVOXELTYPE_EXAMPLE_H
#  include "ZVoxelType_Example.h"
#endif

#ifndef Z_ZVOXELTYPE_VOID_H
#  include "ZVoxelType_Void.h"
#endif

#ifndef Z_ZVOXELTYPE_SPS_H
#include "ZVoxelType_SPS.h"
#endif

#ifndef Z_ZVOXELTYPE_UNIVERSEBORDER_H
#  include "ZVoxelType_UniverseBorder.h"
#endif

#ifndef Z_ZVOXELTYPE_WIRELESSTRANSMITTER_H
#  include "ZVoxelType_WirelessTransmitter.h"
#endif

#ifndef Z_ZVOXELTYPE_WIRELESSRECEIVER_H
#  include "ZVoxelType_WirelessReceiver.h"
#endif

#ifndef Z_ZVOXELTYPE_PROGROBOT_REMOTE_H
#  include "ZVoxelType_ProgRobot_Remote.h"
#endif

void ZVoxelTypeManager::DumpInfos()
{
  ULong i;
  ZVoxelType * Vt;

  for(i=0;i<65536;i++)
  {
    Vt = this->VoxelTable[i];
    if (Vt)
    {
      printf("--------------------------------------------------------------\n");
      printf("VoxelType %lu : %s\n",(UNum)i,Vt->VoxelTypeName.String);
      printf("DrawInfo : %x\n",(UShort)Vt->DrawInfo);
    }
  }
}

void ZVoxelTypeManager::OutFabInfos()
{
  ULong VoxelType, TransNum, EntryNum, ResultNum, ResultNum2;
  ZFabInfos * FabInfos;
  ULong Field_InfoNum;
  ULong Field_ElementNum;
  ULong Field_FabResultNum;

  Field_InfoNum = 1;
  Field_ElementNum = 1;
  Field_FabResultNum = 1;

  ZString Out;

  Out << "TRUNCATE TABLE `bv_fabinfos`;\n";
  Out << "TRUNCATE TABLE `bv_fabelements`;\n";
  Out << "TRUNCATE TABLE `bv_fabresults`;\n";
  Out << "TRUNCATE TABLE `bv_voxelname`;\n";

  for (VoxelType=0;VoxelType<65536;VoxelType++)
  {
    if ( (FabInfos = VoxelTable[VoxelType]->FabInfos) )
    {
      //printf("Voxel %d has fabinfos\n", VoxelType);

      for (TransNum=0;TransNum < FabInfos->TransformationCount; TransNum++)
      {
        ZFabInfos::ZTransformation * Transformation;
        Transformation = &FabInfos->TransformationTable[TransNum];

        for (ResultNum=0; ResultNum < ZFabInfos::ZTransformation::Max_ResultVoxels ; ResultNum++ )
        {

          if (Transformation->Result_VoxelType[ResultNum])
          {
            Out << "INSERT INTO `bv_fabinfos` (`Num`, `VoxelType`, `VoxelTypeName`, `VoxelTypePageNode`, `FabVoxelType`, `FabVoxelTypeName`, `FabVoxelTypePageNode`) VALUES ";
            Out << "('"<< Field_InfoNum;
            Out <<"', '" << Transformation->Result_VoxelType[ResultNum];
            Out <<"', '" << VoxelTable[Transformation->Result_VoxelType[ResultNum]]->VoxelTypeName;
            Out <<"', '" << VoxelTable[Transformation->Result_VoxelType[ResultNum]]->Documentation_PageNum;
            Out <<"', '" << VoxelType;
            Out <<"', '" << VoxelTable[VoxelType]->VoxelTypeName;
            Out <<"', '" << VoxelTable[VoxelType]->Documentation_PageNum;
            Out <<"');\n";
            // printf("Result: %d : %d (%s)\n", Transformation->Result_VoxelType[ResultNum], Transformation->Result_Quantity[ResultNum],VoxelTable[Transformation->Result_VoxelType[ResultNum]]->VoxelTypeName.String);

            for (ResultNum2=0; ResultNum2 < ZFabInfos::ZTransformation::Max_ResultVoxels ; ResultNum2++ )
            {
              if (Transformation->Result_VoxelType[ResultNum2])
              {
                Out << "INSERT INTO `blackvoxel`.`bv_fabresults` (`Num`, `InfoNum`, `VoxelType`, `ResultQuantity`, `VoxelTypeName`, `VoxelTypePageNode`) VALUES ";
                Out << "('" << Field_FabResultNum;
                Out << "', '" << Field_InfoNum;
                Out << "', '" << Transformation->Result_VoxelType[ResultNum2];
                Out << "', '" << Transformation->Result_Quantity[ResultNum2];
                Out << "', '" << VoxelTable[ Transformation->Result_VoxelType[ResultNum2] ]->VoxelTypeName;
                Out << "', '" << VoxelTable[ Transformation->Result_VoxelType[ResultNum2] ]->Documentation_PageNum;
                Out << "');\n";

                Field_FabResultNum ++;
              }
            }



            for (EntryNum=0; EntryNum < Transformation->EntryCount; EntryNum++)
            {
              ZFabInfos::ZFabEntry * Entry;
              Entry = &Transformation->FabList[EntryNum];
              ULong Voxel = FabInfos->MaterialTable[Entry->Index].VoxelType;
              ULong Quantity = Entry->Quantity;

              Out << "INSERT INTO `blackvoxel`.`bv_fabelements` (`Num`, `InfoNum`, `Order`, `VoxelType`, `VoxelQuantity`, `VoxelTypeName`, `VoxelTypePageNode`) VALUES ";
              Out << "('" << Field_ElementNum;
              Out << "', '" << Field_InfoNum;
              Out << "', '" << EntryNum + 1;
              Out << "', '" << Voxel;
              Out << "', '" << Quantity;
              Out << "', '" << VoxelTable[Voxel]->VoxelTypeName;
              Out << "', '" << VoxelTable[Voxel]->Documentation_PageNum;
              Out << "');\n";

              // printf(" > %d : %d (%s)\n", Voxel, Quantity, VoxelTable[Voxel]->VoxelTypeName.String );
              Field_ElementNum ++;
            }
            Field_InfoNum++;
          }


        }

      }

    }

  }

  for (VoxelType=0;VoxelType<32767;VoxelType++)
  {
    if ( !VoxelTable[VoxelType]->Is_NoType )
    {
      Out << "INSERT INTO `bv_voxelname` (`VoxelType`, `VoxelName`, `MiningType`, `VoxelTypePageNode`) VALUES ";
      Out << "('"   << VoxelType;
      Out << "', '" << VoxelTable[VoxelType]->VoxelTypeName;
      Out << "', '" << VoxelTable[VoxelType]->MiningType;
      Out << "', '" << VoxelTable[VoxelType]->Documentation_PageNum;
      Out << "');\n";
    }
  }
  Out.SaveToFile("FabDatabaseOutput.sql");
  // printf("%s",Out.String);
}


bool ZVoxelTypeManager::_Internal_CompareTransTables( ZString & Message, ZFabInfos * Fab ,  ZFabInfos::ZTransformation * Tr1,  ZFabInfos::ZTransformation * Tr2 )
{
  ULong EntryNum1, EntryNum2;
  ZFabInfos::ZFabEntry * Entry1, * Entry2;
  bool Match;
  ZString Ms;

  Ms = 0;
  for (EntryNum1=0; EntryNum1<Tr1->EntryCount; EntryNum1++)
  {
    Entry1 = &Tr1->FabList[EntryNum1];

    Match = false;
    for (EntryNum2=0; EntryNum2<Tr2->EntryCount; EntryNum2++)
    {
      Entry2 = &Tr2->FabList[EntryNum2];

      if (Entry1->Index == Entry2->Index)
      {
        if (Entry1->Quantity <= Entry2->Quantity)
        {
          if (Entry1 < Entry2) Ms << "{EXCHANGE}";
          Ms << "[";
          Ms << VoxelTable[Fab->MaterialTable[Entry1->Index].VoxelType]->VoxelTypeName;
          Ms << " " << Entry1->Quantity << "/" << Entry2->Quantity;
          Ms << "]";
          Match = true; break;
        }
      }

    }

    if (!Match) { Message.Clear(); return(false); }
  }

  Message = Ms;
  return(true);
}

void ZVoxelTypeManager::FindFabConflics()
{
  ULong VoxelType, TransNum1, TransNum2 ;
  ZFabInfos * FabInfos;
  ZString Message;

  for (VoxelType=0;VoxelType<65536;VoxelType++)
  {
    if ((FabInfos = VoxelTable[VoxelType]->FabInfos ))
    {
      printf("Fab : %d\n", VoxelType);
      for (TransNum1=0;TransNum1 < FabInfos->TransformationCount; TransNum1++)
      {
        for (TransNum2=0;TransNum2 < FabInfos->TransformationCount; TransNum2++)
        {
          if (TransNum1 != TransNum2)
          if (_Internal_CompareTransTables( Message, FabInfos, &FabInfos->TransformationTable[TransNum1], &FabInfos->TransformationTable[TransNum2]))
          {
            printf("> Compare : %d vs %d %s\n",TransNum1, TransNum2, Message.String);
          }


        }

      }

    }

  }
}

void ZVoxelTypeManager::FindFabConflicts_V2()
{
  ULong VoxelType, TransNum1, TransNum2 ;
  ZFabInfos * FabInfos;
  ZString Message;

  for (VoxelType=0;VoxelType<65536;VoxelType++)
  {
    if ((FabInfos = VoxelTable[VoxelType]->FabInfos ))
    {
      printf("Machine : %d\n", VoxelType );
      for (TransNum1=0;TransNum1 < FabInfos->TransformationCount; TransNum1++)
      {
        for (TransNum2=TransNum1+1;TransNum2 < FabInfos->TransformationCount; TransNum2++)
        {
          if (TransNum1 != TransNum2)
          if (_Internal_CompareTransTables_V2( Message, FabInfos, &FabInfos->TransformationTable[TransNum1], &FabInfos->TransformationTable[TransNum2]))
          {
            printf("> Compare : %d vs %d %s\n",TransNum1, TransNum2, Message.String);
          }


        }

      }

    }

  }
}


bool ZVoxelTypeManager::_Internal_CompareTransTables_V2( ZString & Message, ZFabInfos * Fab ,  ZFabInfos::ZTransformation * Tr1,  ZFabInfos::ZTransformation * Tr2 )
{
  ULong EntryNum1, EntryNum2;
  ZFabInfos::ZFabEntry * Entry1, * Entry2;
  bool Match;
  ZString Ms;

  Ms = 0;
  for (EntryNum1=0; EntryNum1<Tr1->EntryCount; EntryNum1++)
  {
    Entry1 = &Tr1->FabList[EntryNum1];

    Match = false;
    for (EntryNum2=0; EntryNum2<Tr2->EntryCount; EntryNum2++)
    {
      Entry2 = &Tr2->FabList[EntryNum2];

      if (Entry1->Index == Entry2->Index)
      {
        if (Entry1->Quantity <= Entry2->Quantity)
        {
          Ms << "{**EXCHANGE**}";
          Ms << "[";
          Ms << VoxelTable[Fab->MaterialTable[Entry1->Index].VoxelType]->VoxelTypeName;
          Ms << " " << Entry1->Quantity << "/" << Entry2->Quantity;
          Ms << "]";
          Match = true; break;
        }
      }

    }

    if (!Match) { Message.Clear(); return(false); }
  }

  Message = Ms;
  return(true);
}


Bool ZVoxelTypeManager::LoadVoxelTypes()
{
  ULong i,t;
  ZVoxelType * VoxelType;

  i=0;
  AddVoxelType(i++, new ZVoxelType_Void(0));


  // Game VoxelTypes
  for (; i<32768; i++)
  {
    switch (i)
    {
      case 49: VoxelType = new ZVoxelType_Storage(i);                break;
      case 75: VoxelType = new ZVoxelType_UserTextureTransformer(i); break;
      case 89: VoxelType = new ZVoxelType_YellowGas(i);              break;
      case 96: VoxelType = new ZVoxelType_PlaneZ1(i);                break;
      case 100:VoxelType = new ZVoxelType_LightTransmitter(i);       break;

               // Base Machine
      case 107:VoxelType = new ZVoxelType_FabMachine(i);
               VoxelType->FabInfos = new ZFabInfos;
               VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
               VoxelType->FabInfos->AddMaterial(  3);  // Slot 1 : Blackrock Green
               VoxelType->FabInfos->AddMaterial(119);  // Slot 2 : Cuivre cube
               VoxelType->FabInfos->AddMaterial(128);  // Slot 3 : Bronze cube
               VoxelType->FabInfos->AddMaterial(117);  // Slot 4 : Iron bar
               VoxelType->FabInfos->AddMaterial(  2);  // Slot 5 : Blackrock Orange
               VoxelType->FabInfos->AddMaterial(  5);  // Slot 6 : Blackrock Sky Blue
               VoxelType->FabInfos->AddMaterial(120);  // Slot 7 : Cast Iron Bar.
               VoxelType->FabInfos->AddMaterial(118);  // Slot 8 : Inox Iron Bar.
               VoxelType->FabInfos->AddMaterial(44);   // Slot 9 : Matériau X
               VoxelType->FabInfos->AddMaterial(183);  // Slot 10 : Optical Mirror
               VoxelType->FabInfos->AddMaterial(184);  // Slot 11 : Semi transparent mirror
               VoxelType->FabInfos->AddMaterial( 20);  // Slot 12 : Verre Ordinaire
               VoxelType->FabInfos->AddMaterial(170);  // Slot 13 : Profilé acier
               VoxelType->FabInfos->AddMaterial(169);  // Slot 14 : Tole acier
               VoxelType->FabInfos->AddMaterial(145);  // Slot 15 : Fil de cuivre
               VoxelType->FabInfos->AddMaterial(53);   // Slot 16 : Blue Crystal
               VoxelType->FabInfos->AddMaterial(26);   // Slot 17 : Gold
               VoxelType->FabInfos->AddMaterial(70);   // Slot 18 : Black Wood Leaves
               VoxelType->FabInfos->AddMaterial(72);   // Slot 19 : Black Wood
               VoxelType->FabInfos->AddMaterial(6);    // Slot 20 : BlackRock Yellow
               VoxelType->FabInfos->AddMaterial(54);   // Slot 21 : Rocky Blue

               VoxelType->FabInfos->SetPurgeCondition(0,2); // if this slot go above limit, go to purge state mode.
               VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation

               // Ground Scanner
               t=VoxelType->FabInfos->AddTransformation();   // T0
                 VoxelType->FabInfos->AddCondition(t,4,128); // 128 Acier
                 VoxelType->FabInfos->AddCondition(t,2,128); // 128 Cuivre
                 VoxelType->FabInfos->AddCondition(t,8,4 );  // 4   Inox
                 VoxelType->FabInfos->AddCondition(t,16,16); // 16  Blue Crystal
                 VoxelType->FabInfos->AddCondition(t,21,16); // 16  Rocky Blue
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1   Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,251,1);  // 1   Ground Scanner.

               // Remote Programmable Robot
               t=VoxelType->FabInfos->AddTransformation();   // T1
                 //VoxelType->FabInfos->AddCondition(t,7,5);   // 10 Fonte
                 VoxelType->FabInfos->AddCondition(t,2,5);   // 5 Cuivre
                 VoxelType->FabInfos->AddCondition(t,3,4 );  // 4  Bronze
                 VoxelType->FabInfos->AddCondition(t,8,4 );  // 4  Inox
                 VoxelType->FabInfos->AddCondition(t,4,5);   // 5 Acier
                 VoxelType->FabInfos->AddCondition(t,17,2);  // 5  Or
                 VoxelType->FabInfos->AddCondition(t,5,16);  // 16 BlackRock Orange
                 VoxelType->FabInfos->AddCondition(t,1,16);  // 16 Blackrock Green
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,256,1);  // Remote Programmable Robot

               // 0 Four
               t=VoxelType->FabInfos->AddTransformation();   // T2
                 VoxelType->FabInfos->AddCondition(t,1,48);  // 48 Blackrock Green
                 VoxelType->FabInfos->AddCondition(t,7,16);  // 16 Fonte
                 VoxelType->FabInfos->AddCondition(t,8,2);   // 2 Inox Iron Bar
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,196,1);  // 1 Four
               // 1 Four à verre
               t=VoxelType->FabInfos->AddTransformation();   // T3
                 VoxelType->FabInfos->AddCondition(t,1,48);  // 48 Blackrock Green
                 VoxelType->FabInfos->AddCondition(t,7,16);  // 16 Fonte
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,172,1);  // 1 Four à verre

               // Extractor Robot XR-1
               t=VoxelType->FabInfos->AddTransformation();   // T4
                 VoxelType->FabInfos->AddCondition(t,7,1);   // 1 Fonte
                 VoxelType->FabInfos->AddCondition(t,2,8);   // 8 Cuivre
                 VoxelType->FabInfos->AddCondition(t,4,2);   // 2 Acier
                 VoxelType->FabInfos->AddCondition(t,1,16);  // 16 Blackrock Green
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,153,1);  // Extraction Robot XR-1

               // Constructor/Destructor N°2
               t=VoxelType->FabInfos->AddTransformation();   // T5
                 VoxelType->FabInfos->AddCondition(t,2,8);   // 8 cuivre
                 VoxelType->FabInfos->AddCondition(t,1,8);   // 8 BlackRock Green
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,76,1);   // Constructeur / Destructeur N°2
               // Constructor/Destructor N°3
               t=VoxelType->FabInfos->AddTransformation();   // T6
                 VoxelType->FabInfos->AddCondition(t,4,8);   // 8 fer
                 VoxelType->FabInfos->AddCondition(t,5,16);  // 16 BlackRock Orange
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,77,1);   // Constructeur / Destructeur N°3

               // Extraction Robot XR-3
               t=VoxelType->FabInfos->AddTransformation();   // T7
                 VoxelType->FabInfos->AddCondition(t,7,10);  // 10 Fonte
                 VoxelType->FabInfos->AddCondition(t,2,40);  // 40 Cuivre
                 VoxelType->FabInfos->AddCondition(t,3,16);  // 16 Bronze
                 VoxelType->FabInfos->AddCondition(t,8,16);  // 16 Inox
                 VoxelType->FabInfos->AddCondition(t,4,30);  // 30 Acier
                 VoxelType->FabInfos->AddCondition(t,17,10); // 5  Or
                 VoxelType->FabInfos->AddCondition(t,6,32);  // 32 Blackrock sky blue
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,155,1);  // Extraction Robot XR-3
               // Extraction Robot XR-2
               t=VoxelType->FabInfos->AddTransformation();   // T8
                 VoxelType->FabInfos->AddCondition(t,7,1);   // 1 Fonte
                 VoxelType->FabInfos->AddCondition(t,2,20);  // 20 Cuivre
                 VoxelType->FabInfos->AddCondition(t,3,10);  // 10 Bronze
                 VoxelType->FabInfos->AddCondition(t,8,10);  // 10 Inox
                 VoxelType->FabInfos->AddCondition(t,4,20);  // 20 Acier
                 VoxelType->FabInfos->AddCondition(t,6,16);  // 16 Blackrock sky blue
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,154,1);  // Extraction Robot XR-2
                 // Broyeur
               t=VoxelType->FabInfos->AddTransformation();   // T9
                 VoxelType->FabInfos->AddCondition(t,7,8);   // 8 Fonte
                 VoxelType->FabInfos->AddCondition(t,8,8);   // 8 Inox
                 VoxelType->FabInfos->AddCondition(t,4,2);   // 2 Acier
                 VoxelType->FabInfos->AddCondition(t,2,4);   // 4 Cuivre
                 VoxelType->FabInfos->AddCondition(t,6,16);  // 16 Blackrock sky blue
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,137,1);  // 1 Broyeur
               // Circuits intégrés
               t=VoxelType->FabInfos->AddTransformation();   // T10
                 VoxelType->FabInfos->AddCondition(t,8,8);   // 8 Inox
                 VoxelType->FabInfos->AddCondition(t,2,4);   // 4 Cuivre
                 VoxelType->FabInfos->AddCondition(t,6,16);  // 16 Blackrock sky blue
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,133,1);  // 1 Machine circuits intégrés
               // Bobineuse Moteur
               t=VoxelType->FabInfos->AddTransformation();   // T11
                 VoxelType->FabInfos->AddCondition(t,2,8);   // 8 Cuivre
                 VoxelType->FabInfos->AddCondition(t,4,4);   // 4 Acier
                 VoxelType->FabInfos->AddCondition(t,7,2);   // 2 Fonte
                 VoxelType->FabInfos->AddCondition(t,6,16);  // 16 Blackrock sky blue
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,132,1);  // 1 Bobineuse Moteur
               // Moulage de pièces
               t=VoxelType->FabInfos->AddTransformation();   // T12
                 VoxelType->FabInfos->AddCondition(t,7,8);   // 8 Fonte
                 VoxelType->FabInfos->AddCondition(t,4,2);   // 2 Acier
                 VoxelType->FabInfos->AddCondition(t,5,16);  // 16 BlackRock Orange
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,130,1);  // 1 Mouleuse de pièces
               // Forge
               t=VoxelType->FabInfos->AddTransformation();   // T13
                 VoxelType->FabInfos->AddCondition(t,7,16);  // 16 Fonte
                 VoxelType->FabInfos->AddCondition(t,4,8);   // 8 Acier
                 VoxelType->FabInfos->AddCondition(t,8,2);   // 2 Inox
                 VoxelType->FabInfos->AddCondition(t,1,16);  // 16 Blackrock Green
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,134,1);  // 1 Forge
               // Tôlerie
               t=VoxelType->FabInfos->AddTransformation();   // T14
                 VoxelType->FabInfos->AddCondition(t,7,4);   // 4 Fonte
                 VoxelType->FabInfos->AddCondition(t,4,8);   // 8 Acier
                 VoxelType->FabInfos->AddCondition(t,8,2);   // 2 Inox
                 VoxelType->FabInfos->AddCondition(t,1,16);  // 16 BlackRock Green
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,131,1);  // 1 Tolerie
               // Extrudeuse
               t=VoxelType->FabInfos->AddTransformation();   // T15
                 VoxelType->FabInfos->AddCondition(t,7,4);   // 4 Fonte
                 VoxelType->FabInfos->AddCondition(t,4,4);   // 4 Acier
                 VoxelType->FabInfos->AddCondition(t,8,2);   // 2 Inox
                 VoxelType->FabInfos->AddCondition(t,2,2);   // 2 Cuivre
                 VoxelType->FabInfos->AddCondition(t,5,16);  // 16 BlackRock Orange
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,135,1);  // 1 Extrudeuse
               // Tour
               t=VoxelType->FabInfos->AddTransformation();   // T16
                 VoxelType->FabInfos->AddCondition(t,7,4);   // 4 Fonte
                 VoxelType->FabInfos->AddCondition(t,4,4);   // 4 Acier
                 VoxelType->FabInfos->AddCondition(t,8,1);   // 1 Inox
                 VoxelType->FabInfos->AddCondition(t,2,4);   // 4 Cuivre
                 VoxelType->FabInfos->AddCondition(t,6,16);  // 16 Blackrock sky blue
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,140,1);  // 1 Tour
               // Fraiseuse
               t=VoxelType->FabInfos->AddTransformation();   // T17
                 VoxelType->FabInfos->AddCondition(t,7,4);   // 4 Fonte
                 VoxelType->FabInfos->AddCondition(t,4,4);   // 4 Acier
                 VoxelType->FabInfos->AddCondition(t,8,1);   // 1 Inox
                 VoxelType->FabInfos->AddCondition(t,2,2);   // 2 Cuivre
                 VoxelType->FabInfos->AddCondition(t,6,16);  // 16 Blackrock sky blue
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,139,1);  // 1 Fraiseuse

               // Slow Pump T0
               t=VoxelType->FabInfos->AddTransformation();   // T18
                 VoxelType->FabInfos->AddCondition(t,7,5);   // 5 Fonte
                 VoxelType->FabInfos->AddCondition(t,4,7);   // 7 Acier
                 VoxelType->FabInfos->AddCondition(t,8,4);   // 4 Inox
                 VoxelType->FabInfos->AddCondition(t,3,8);   // 8 Bronze
                 VoxelType->FabInfos->AddCondition(t,1,16);  // 16 Blackrock Green
                 //VoxelType->FabInfos->AddCondition(t,18,2);  // 2 Black Wood
                 //VoxelType->FabInfos->AddCondition(t,19,2);  // 2 Black Wood Leaves
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,250,1);  // 1 Slow Pump.

               // Tréfileuse
               t=VoxelType->FabInfos->AddTransformation();   // T19
                 VoxelType->FabInfos->AddCondition(t,7,2);   // 2 Fonte
                 VoxelType->FabInfos->AddCondition(t,4,4);   // 4 Acier
                 VoxelType->FabInfos->AddCondition(t,8,1);   // 1 Inox
                 VoxelType->FabInfos->AddCondition(t,2,2);   // 2 Cuivre
                 VoxelType->FabInfos->AddCondition(t,1,16);  // 16 BlackRock Green
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,142,1);  // 1 Tréfileuse

               // X Machine
               t=VoxelType->FabInfos->AddTransformation();   // T20
                 VoxelType->FabInfos->AddCondition(t,7,16);  // 16 Fonte
                 VoxelType->FabInfos->AddCondition(t,4,32);  // 32 Acier
                 VoxelType->FabInfos->AddCondition(t,8,8);   // 8 Inox
                 VoxelType->FabInfos->AddCondition(t,2,32);  // 32 Cuivre
                 VoxelType->FabInfos->AddCondition(t,9,4);   // 4 Materiau X
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,143,1);  // 1 X Machine

               // Machine d'assemblage
               t=VoxelType->FabInfos->AddTransformation();   // T21
                 VoxelType->FabInfos->AddCondition(t,7,16);  // 16 Fonte
                 VoxelType->FabInfos->AddCondition(t,4,32);  // 32 Acier
                 VoxelType->FabInfos->AddCondition(t,8,8);   // 8 Inox
                 VoxelType->FabInfos->AddCondition(t,2,32);  // 32 Cuivre
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,136,1);  // 1 Machine d'assemblage

               // Atomic compressor
               t=VoxelType->FabInfos->AddTransformation();   // T22
                 VoxelType->FabInfos->AddCondition(t,2,4);   // 4 Cuivre
                 VoxelType->FabInfos->AddCondition(t,4,4);   // 4 Acier
                 VoxelType->FabInfos->AddCondition(t,5,8);   //  8 Blackrock Orange
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,49,16);  // 1 Atomic compressor

               // Découpeuse laser
               t=VoxelType->FabInfos->AddTransformation();   // T23
                 VoxelType->FabInfos->AddCondition(t,14,2);  // 2 Tole acier
                 VoxelType->FabInfos->AddCondition(t,13,1);  // 1 Profilé acier
                 VoxelType->FabInfos->AddCondition(t,10,1);  // 1 Optical Mirror
                 VoxelType->FabInfos->AddCondition(t,11,1);  // 1 Semi transparent mirror
                 VoxelType->FabInfos->AddCondition(t,12,1);  // 1 Verre
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,195,1);  // 1 Découpeuse laser

               // Unité de dépot en phase vapeur
               t=VoxelType->FabInfos->AddTransformation();   // T24
                 VoxelType->FabInfos->AddCondition(t,14,4);  // 4 Tole acier
                 VoxelType->FabInfos->AddCondition(t,13,4);  // 4 Profilé acier
                 VoxelType->FabInfos->AddCondition(t,2,2);   // 2 Cuivre cube
                 VoxelType->FabInfos->AddCondition(t,15,1);  // 1 Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,182,1);  // 1 Unité de dépot en phase vapeur

               // Sequenceur
               t=VoxelType->FabInfos->AddTransformation();   // T25
                 VoxelType->FabInfos->AddCondition(t,4,8);   // 8 Acier
                 VoxelType->FabInfos->AddCondition(t,2,8);   // 8 Cuivre
                 VoxelType->FabInfos->AddCondition(t,1,4);   // 4 BlackRock Green
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,198,16); // 16 Sequenceurs
                 VoxelType->FabInfos->SetResult(t,1,199,1);  // 1  End Symbol
                 VoxelType->FabInfos->SetResult(t,2,208,1);  // 1  End Symbol

               // Selective Mover
               t=VoxelType->FabInfos->AddTransformation();   // T26
                 VoxelType->FabInfos->AddCondition(t,4,4);   // 4 Acier
                 VoxelType->FabInfos->AddCondition(t,2,2);   // 2 Cuivre
                 VoxelType->FabInfos->AddCondition(t,1,4);   // 4 BlackRock Green
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,204,8);  // 8 Selective Mover
               // Mover
               t=VoxelType->FabInfos->AddTransformation();   // T27
                 VoxelType->FabInfos->AddCondition(t,4,4);   // 4 Acier
                 VoxelType->FabInfos->AddCondition(t,2,2);   // 2 Cuivre
                 VoxelType->FabInfos->AddCondition(t,1,1);   // 1 BlackRock Green
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,209,8);  // 8 Mover

               // Unité d'imprimerie.
               t=VoxelType->FabInfos->AddTransformation();   // T28
                 VoxelType->FabInfos->AddCondition(t,17,2);  // 2 Or
                 VoxelType->FabInfos->AddCondition(t,7,2);   // 2 Fonte
                 VoxelType->FabInfos->AddCondition(t,2,2);   // 2 Cuivre cube
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,210,1);  // 1 Unité d'imprimerie.

               // Plane Z0
               t=VoxelType->FabInfos->AddTransformation();   // T29
                 VoxelType->FabInfos->AddCondition(t,9,4);   // 4 Materiau X
                 VoxelType->FabInfos->AddCondition(t,8,4);   // 4 Inox
                 VoxelType->FabInfos->AddCondition(t,4,4);   // 4 Acier
                 VoxelType->FabInfos->AddCondition(t,2,4);   // 4 Cuivre
                 VoxelType->FabInfos->AddCondition(t,16,2);  // 2 Blue Crystal
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,239,1);  // 1 Location System.

               // Slow Belt Conveyor
               t=VoxelType->FabInfos->AddTransformation();   // T30
                 VoxelType->FabInfos->AddCondition(t,4,16);  // 16 Acier
                 VoxelType->FabInfos->AddCondition(t,2,16);  // 16 Cuivre
                 VoxelType->FabInfos->AddCondition(t,6,16);  // 16 Blackrock BlueSky
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,244,64); // 64 Slow belt conveyor

               // Rotate Tool
               t=VoxelType->FabInfos->AddTransformation();   // T31
               VoxelType->FabInfos->AddCondition(t,1,8);     // 8 Blackrock Green
               VoxelType->FabInfos->AddCondition(t,5,8);     // 8 Blackrock Orange
               VoxelType->FabInfos->AddCondition(t,6,8);     // 8 Blackrock BlueSky
               VoxelType->FabInfos->AddCondition(t,4,2);     // 2 Acier
               VoxelType->FabInfos->AddCondition(t,2,2);     // 2 Cuivre
               VoxelType->FabInfos->AddCondition(t,0,1);     // 1  Blackrock Blue (Validation)
               VoxelType->FabInfos->SetResult(t,0,243,1);    // Rotate Tool

               // Melting Furnace
               t=VoxelType->FabInfos->AddTransformation();   // T32
               VoxelType->FabInfos->AddCondition(t,1,8);     // 8 Blackrock Green
               VoxelType->FabInfos->AddCondition(t,0,1);     // 1  Blackrock Blue (Validation)
               VoxelType->FabInfos->SetResult(t,0,114,1);    // Melting Furnace

               // Slow Materializer
               t=VoxelType->FabInfos->AddTransformation();   // T33
                 VoxelType->FabInfos->AddCondition(t,7,2);   // 2  Fonte
                 VoxelType->FabInfos->AddCondition(t,8,2);   // 2  Inox
                 VoxelType->FabInfos->AddCondition(t,3,2);   // 2  Bronze
                 VoxelType->FabInfos->AddCondition(t,6,16);  // 16 Blackrock sky blue
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,248,64); // 64  Slow Materializer

               // Slow De-Materializer
               t=VoxelType->FabInfos->AddTransformation();   // T34
                 VoxelType->FabInfos->AddCondition(t,7,2);   // 2  Fonte
                 VoxelType->FabInfos->AddCondition(t,8,2);   // 2  Inox
                 VoxelType->FabInfos->AddCondition(t,3,2);   // 2  Bronze
                 VoxelType->FabInfos->AddCondition(t,5,16);  //16  Blackrock Orange
                 //VoxelType->FabInfos->AddCondition(t,18,2);// 2  Black Wood Leaves
                 //VoxelType->FabInfos->AddCondition(t,19,2);// 2  Black Wood Leaves
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,249,64); // 64  Slow De-Materializer

               // Location System
               t=VoxelType->FabInfos->AddTransformation();   // T35
                 VoxelType->FabInfos->AddCondition(t,4,4);   // 4 Acier
                 VoxelType->FabInfos->AddCondition(t,2,2);   // 2 Cuivre cube
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,241,1);  // 1 Location System.

               // Analyser
               t=VoxelType->FabInfos->AddTransformation();   // T36
                 VoxelType->FabInfos->AddCondition(t,4,2);   // 2 Acier
                 VoxelType->FabInfos->AddCondition(t,2,2);   // 2 Cuivre
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,242,1);  // 1 Analyser.

               // Base machine
               t=VoxelType->FabInfos->AddTransformation();   // T37
                 VoxelType->FabInfos->AddCondition(t,4,1);   // 1 Acier
                 VoxelType->FabInfos->AddCondition(t,2,1);   // 1 Cuivre
                 VoxelType->FabInfos->AddCondition(t,20,8);  // 8 Blackrock Yellow
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,107,1);  // 1 Base Machine

               break;


      case 108:VoxelType = new ZVoxelType_Programmable(i);           break;
      case 114:VoxelType = new ZVoxelType_BlastFurnace(i);           break;
      case 115:VoxelType = new ZVoxelType_FusionElement(i);          break;

      // Mouleuse
      case 130:VoxelType = new ZVoxelType_FabMachine(i);

               VoxelType->FabInfos = new ZFabInfos;
               VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
               VoxelType->FabInfos->AddMaterial(120);  // Slot 1 : Cast Iron Bar = Fonte
               VoxelType->FabInfos->AddMaterial(118);  // Slot 2 : Inox Iron Bar
               VoxelType->FabInfos->AddMaterial(  3);  // Slot 3 : Blackrock Green

               VoxelType->FabInfos->SetPurgeCondition(0,2); // if this slot go above limit, go to purge state mode.
               VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation

               // Corps de pompe C3
               t=VoxelType->FabInfos->AddTransformation();   // T0
                 VoxelType->FabInfos->AddCondition(t,1,16);  // 16 Fonte
                 VoxelType->FabInfos->AddCondition(t,2,16);  // 16 Inox bar
                 VoxelType->FabInfos->AddCondition(t,3,16);  // 16 Blackrock Green
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,148,1);  // 1 Corps de pompe C3

               // Corps de pompe C1
               t=VoxelType->FabInfos->AddTransformation();   // T1
                 VoxelType->FabInfos->AddCondition(t,1,16);  // 16 Fonte
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,146,1);  // 1 Corps de pompe C1

               // Corps de pompe C2
               t=VoxelType->FabInfos->AddTransformation();   // T2
                 VoxelType->FabInfos->AddCondition(t,2,16);  // 16 Inox bar
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,147,1);  // 1 Corps de pompe C2

               break;

               // Tolerie
      case 131:VoxelType = new ZVoxelType_FabMachine(i);

               VoxelType->FabInfos = new ZFabInfos;
               VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
               VoxelType->FabInfos->AddMaterial(118);  // Slot 1 : Inox Iron Bar
               VoxelType->FabInfos->AddMaterial(119);  // Slot 2 : Cuivre cube
               VoxelType->FabInfos->AddMaterial(125);  // Slot 3 : Alu
               VoxelType->FabInfos->AddMaterial(117);  // Slot 4 : Iron bar

               VoxelType->FabInfos->SetPurgeCondition(0,2); // if this slot go above limit, go to purge state mode.
               VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation

               // Toles inox
               t=VoxelType->FabInfos->AddTransformation();   // T0
                 VoxelType->FabInfos->AddCondition(t,1,10);  // 10 Inox bar
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,160,10); // 10 Toles inox

                // Toles copper
               t=VoxelType->FabInfos->AddTransformation();   // T1
                 VoxelType->FabInfos->AddCondition(t,2,10);  // 10 Copper cuivre
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,161,10);  // 10 Toles copper

               // Toles alu
               t=VoxelType->FabInfos->AddTransformation();   // T2
                 VoxelType->FabInfos->AddCondition(t,3,10);  // 10 Alu
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,162,10);  // 10 Toles alu

               // Toles steel
              t=VoxelType->FabInfos->AddTransformation();   // T3
                VoxelType->FabInfos->AddCondition(t,4,10);  // 10 Iron bar
                VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                VoxelType->FabInfos->SetResult(t,0,169,10); // 10 Toles steel

              break;

      // Bobineuse
      case 132:VoxelType = new ZVoxelType_FabMachine(i);

               VoxelType->FabInfos = new ZFabInfos;
               VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
               VoxelType->FabInfos->AddMaterial(192);  // Slot 1 : Induit de moteur
               VoxelType->FabInfos->AddMaterial(193);  // Slot 2 : Corps de moteur
               VoxelType->FabInfos->AddMaterial(144);  // Slot 3 : Axe
               VoxelType->FabInfos->AddMaterial(145);  // Slot 4 : Fil de cuivre

               VoxelType->FabInfos->SetPurgeCondition(0,2); // if this slot go above limit, go to purge state mode.
               VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation

               // Moteur M12
               t=VoxelType->FabInfos->AddTransformation();  // T0
                 VoxelType->FabInfos->AddCondition(t,1,2);    // 1  Induit de moteur
                 VoxelType->FabInfos->AddCondition(t,2,1);    // 1  Corps de moteur
                 VoxelType->FabInfos->AddCondition(t,3,1);    // 1  Axe
                 VoxelType->FabInfos->AddCondition(t,4,20);   // 10 Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock
                 VoxelType->FabInfos->SetResult(t,0,138,1);   // 1  Moteur M12

               // Moteur M11
               t=VoxelType->FabInfos->AddTransformation();  // T1
                 VoxelType->FabInfos->AddCondition(t,1,1);    // 1  Induit de moteur
                 VoxelType->FabInfos->AddCondition(t,2,1);    // 1  Corps de moteur
                 VoxelType->FabInfos->AddCondition(t,3,1);    // 1  Axe
                 VoxelType->FabInfos->AddCondition(t,4,10);   // 10 Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock
                 VoxelType->FabInfos->SetResult(t,0,180,1);   // 1  Moteur M11

               break;

      // Graveuse de circuits imprimés et pièces électroniques
      case 133:VoxelType = new ZVoxelType_FabMachine(i);

               VoxelType->FabInfos = new ZFabInfos;
               VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
               VoxelType->FabInfos->AddMaterial(194);  // Slot 1 : Wafer
               VoxelType->FabInfos->AddMaterial(3);    // Slot 2 : Blackrock Green
               VoxelType->FabInfos->AddMaterial(119);  // Slot 3 : Cuivre
               VoxelType->FabInfos->AddMaterial(151);  // Slot 4 : Plomb
               VoxelType->FabInfos->AddMaterial(44);   // Slot 5 : Matériau X
               VoxelType->FabInfos->AddMaterial(189);  // Slot 6 : Microcontrôleur
               VoxelType->FabInfos->AddMaterial(145);  // Slot 7 : Fil de cuivre

               VoxelType->FabInfos->SetPurgeCondition(0,2); // if this slot go above limit, go to purge state mode.
               VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation

               // Computer board
               t=VoxelType->FabInfos->AddTransformation();  // T0
                 VoxelType->FabInfos->AddCondition(t,1,8);    // 8 Wafers
                 VoxelType->FabInfos->AddCondition(t,3,1);    // 1 Cuivre
                 VoxelType->FabInfos->AddCondition(t,4,1);    // 1 Plomb
                 VoxelType->FabInfos->AddCondition(t,2,4);    // 4 Blackrock Green
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1 BlackRock
                 VoxelType->FabInfos->SetResult(t,0,141,1);   // 1 Computer Board.
               // Transmetteur optique
               t=VoxelType->FabInfos->AddTransformation();  // T1
                 VoxelType->FabInfos->AddCondition(t,1,1);    // 1 Wafer
                 VoxelType->FabInfos->AddCondition(t,2,4);    // 1 Blackrock Green
                 VoxelType->FabInfos->AddCondition(t,3,1);    // 1 Matériau X
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1 BlackRock
                 VoxelType->FabInfos->SetResult(t,0,181,1);   // 1 Optical transmitter
               // Microcontrôleur
               t=VoxelType->FabInfos->AddTransformation();  // T2
                 VoxelType->FabInfos->AddCondition(t,1,1);    // 1 Wafer
                 VoxelType->FabInfos->AddCondition(t,2,1);    // 1 Blackrock Green
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1 BlackRock
                 VoxelType->FabInfos->SetResult(t,0,189,1);   // 1
               // Scanner courte distance
               t=VoxelType->FabInfos->AddTransformation();  // T3
                 VoxelType->FabInfos->AddCondition(t,6,1);    // 3 Microcontrôleurs
                 VoxelType->FabInfos->AddCondition(t,7,4);    // 4 Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,4,1);    // 1 Plomb
                 VoxelType->FabInfos->SetResult(t,0,191,1);   // 1 Scanner courte distance

               break;

      // Forgeuse
      case 134:VoxelType = new ZVoxelType_FabMachine(i);

               VoxelType->FabInfos = new ZFabInfos;
               VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
               VoxelType->FabInfos->AddMaterial(120);  // Slot 1 : Cast Iron Bar = Fonte
               VoxelType->FabInfos->AddMaterial(118);  // Slot 2 : Inox Iron Bar
               VoxelType->FabInfos->AddMaterial(  3);  // Slot 3 : Blackrock Green
               VoxelType->FabInfos->AddMaterial(124);  // Slot 4 : Titanium Bar

               VoxelType->FabInfos->SetPurgeCondition(0,2); // if this slot go above limit, go to purge state mode.
               VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation

               // Rotor de pompe R3
               t=VoxelType->FabInfos->AddTransformation();   // T0
                 VoxelType->FabInfos->AddCondition(t,1,16);  // 16 Fonte
                 VoxelType->FabInfos->AddCondition(t,2,16);  // 16 Inox Iron Bar
                 VoxelType->FabInfos->AddCondition(t,3,16);  // 16 Blackrock Green
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,166,1);  // 1 Rotor de pompe R3
               // Rotor de pompe R1
               t=VoxelType->FabInfos->AddTransformation();   // T1
                 VoxelType->FabInfos->AddCondition(t,1,4);   // 4 Fonte
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,163,1);  // 1 Rotor de pompe R1

               // Rotor de pompe R2
               t=VoxelType->FabInfos->AddTransformation();   // T2
                 VoxelType->FabInfos->AddCondition(t,2,4);   // 4 Inox Iron Bar
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,164,1);  // 1 Rotor de pompe R2

               // Roue
               t=VoxelType->FabInfos->AddTransformation();   // T2
                 VoxelType->FabInfos->AddCondition(t,4,8);   // 8 Titanium Bar
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,188,1);  // 1 Roue

               break;

      // Extrudeuse
      case 135:VoxelType = new ZVoxelType_FabMachine(i);

               VoxelType->FabInfos = new ZFabInfos;
               VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
               VoxelType->FabInfos->AddMaterial(118);  // Slot 1 : Inox Iron Bar
               VoxelType->FabInfos->AddMaterial(119); //  Slot 2 : Cuivre cube
               VoxelType->FabInfos->AddMaterial(117); //  Slot 3 : Iron bar
               VoxelType->FabInfos->AddMaterial(125); //  Slot 4 : Alu

               VoxelType->FabInfos->SetPurgeCondition(0,2); // if this slot go above limit, go to purge state mode.
               VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation

               // Profilé inox
               t=VoxelType->FabInfos->AddTransformation();   // T0
                 VoxelType->FabInfos->AddCondition(t,1,5);  // 5 Inox bar
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,167,10); // 10 Profil inox

               // Profilé copper
               t=VoxelType->FabInfos->AddTransformation();   // T1
                 VoxelType->FabInfos->AddCondition(t,2,5);  // 5 Copper cuivre
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,168,10);  // 10 Profil copper

               // Profilé steel
               t=VoxelType->FabInfos->AddTransformation();   // T2
                 VoxelType->FabInfos->AddCondition(t,3,5);  // 5 Iron bar
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,170,10);  // 10 Profil steel

               // Profilé alu
               t=VoxelType->FabInfos->AddTransformation();   // T3
                 VoxelType->FabInfos->AddCondition(t,4,5);  // 5 Alu
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,171,10);  // 10 Profil alu

                break;

      // Machine d'assemblage.
      case 136:VoxelType = new ZVoxelType_FabMachine(i);

               VoxelType->FabInfos = new ZFabInfos;
               VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
               VoxelType->FabInfos->AddMaterial(144);  // Slot 1 : Axe
               VoxelType->FabInfos->AddMaterial(146);  // Slot 2 : Corps de pompe C-1
               VoxelType->FabInfos->AddMaterial(147);  // Slot 3 : Corps de pompe C-2
               VoxelType->FabInfos->AddMaterial(148);  // Slot 4 : Corps de pompe C-3
               VoxelType->FabInfos->AddMaterial(163);  // Slot 5 : Rotor de pompe R-1
               VoxelType->FabInfos->AddMaterial(164);  // Slot 6 : Rotor de pompe R-2
               VoxelType->FabInfos->AddMaterial(166);  // Slot 7 : Rotor de pompe R-3
               VoxelType->FabInfos->AddMaterial(180);  // Slot 8 : Motor M-11 Little motor
               VoxelType->FabInfos->AddMaterial(138);  // Slot 9 : Motor M-12 Medium motor Pump motor
               VoxelType->FabInfos->AddMaterial(65535);// Slot 10: Motor M-30 Large motor
               VoxelType->FabInfos->AddMaterial(65535);// Slot 11: Motor M-50 Very Large motor
               VoxelType->FabInfos->AddMaterial(169);  // Slot 12: Feuille d'acier.
               VoxelType->FabInfos->AddMaterial(160);  // Slot 13: Feuille d'inox.
               VoxelType->FabInfos->AddMaterial(161);  // Slot 14: Feuille de cuivre.
               VoxelType->FabInfos->AddMaterial(162);  // Slot 15: Feuille d'alu.
               VoxelType->FabInfos->AddMaterial(170);  // Slot 16: Profilé d'acier.
               VoxelType->FabInfos->AddMaterial(167);  // Slot 17: Profilé d'inox.
               VoxelType->FabInfos->AddMaterial(168);  // Slot 18: Profilé de cuivre.
               VoxelType->FabInfos->AddMaterial(171);  // Slot 19: Profilé d'alu.
               VoxelType->FabInfos->AddMaterial(174);  // Slot 20: Désintégrateur.
               VoxelType->FabInfos->AddMaterial(177);  // Slot 21: Réintégrateur.
               VoxelType->FabInfos->AddMaterial(175);  // Slot 22: Réducteur d'espace atomique.
               VoxelType->FabInfos->AddMaterial(176);  // Slot 23: Voxel Conductor.
               VoxelType->FabInfos->AddMaterial(145);  // Slot 24: Fil de cuivre.
               VoxelType->FabInfos->AddMaterial(178);  // Slot 25: Unités de stockage compactes.
               VoxelType->FabInfos->AddMaterial(179);  // Slot 26: Récepteur / Transmetteur d'énergie.
               VoxelType->FabInfos->AddMaterial(149);  // Slot 27: Rouleaux en fonte.
               VoxelType->FabInfos->AddMaterial(3);    // Slot 28: Blackrock Green
               VoxelType->FabInfos->AddMaterial(2);    // Slot 29: Blackrock Orange
               VoxelType->FabInfos->AddMaterial(5);    // Slot 30: Blackrock cyan
               VoxelType->FabInfos->AddMaterial(7);    // Slot 31: Blackrock red
               VoxelType->FabInfos->AddMaterial(181);  // Slot 32: Laser Optical Transmitter/Receiver
               VoxelType->FabInfos->AddMaterial(183);  // Slot 33: Optical Mirror
               VoxelType->FabInfos->AddMaterial(184);  // Slot 34: Semi transparent mirror
               VoxelType->FabInfos->AddMaterial(185);  // Slot 35: Optical Prism
               VoxelType->FabInfos->AddMaterial(141);  // Slot 36: Computer Board.
               VoxelType->FabInfos->AddMaterial(186);  // Slot 37: Propulseur.
               VoxelType->FabInfos->AddMaterial(187);  // Slot 38: Piston hydraulique.
               VoxelType->FabInfos->AddMaterial(188);  // Slot 39: Roue.
               VoxelType->FabInfos->AddMaterial(189);  // Slot 40: Microcontroleur
               VoxelType->FabInfos->AddMaterial(190);  // Slot 41: Explosive Material 325
               VoxelType->FabInfos->AddMaterial(191);  // Slot 42: Material Detector
               VoxelType->FabInfos->AddMaterial(192);  // Slot 43: Induit de moteur
               VoxelType->FabInfos->AddMaterial(191);  // Slot 44: Corps de moteur
               VoxelType->FabInfos->AddMaterial(60);   // Slot 45: Charbon

               VoxelType->FabInfos->SetPurgeCondition(0,16); // if this slot go above limit, go to purge state mode.
               VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation

               // T1 Pump
               t=VoxelType->FabInfos->AddTransformation();  // T0
                 VoxelType->FabInfos->AddCondition(t,2,1);    // 1  Corps de pompe C-1
                 VoxelType->FabInfos->AddCondition(t,5,1);    // 1  Rotor de pompe R-1
                 VoxelType->FabInfos->AddCondition(t,1,1);    // 1  Axe
                 VoxelType->FabInfos->AddCondition(t,9,1);    // 1  Moteur M-12 Moteur de pompe
                 VoxelType->FabInfos->AddCondition(t,26,1);   // 1  Récepteur / Transmetteur d'énergie.
                 VoxelType->FabInfos->AddCondition(t,12,6);   // 6  Feuilles d'acier
                 VoxelType->FabInfos->AddCondition(t,16,12);  // 12 Profilés d'acier
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,94,1);    // Pompe T1.
               // T2 Pump
               t=VoxelType->FabInfos->AddTransformation();  // T1
                 VoxelType->FabInfos->AddCondition(t,3,1);    // 1  Corps de pompe C-2
                 VoxelType->FabInfos->AddCondition(t,6,1);    // 1  Rotor de pompe R-2
                 VoxelType->FabInfos->AddCondition(t,1,1);    // 1  Axe
                 VoxelType->FabInfos->AddCondition(t,9,1);    // 1  Moteur M-12 Moteur de pompe
                 VoxelType->FabInfos->AddCondition(t,26,1);   // 1  Récepteur / Transmetteur d'énergie.
                 VoxelType->FabInfos->AddCondition(t,13,6);   // 6  Feuilles d'Inox
                 VoxelType->FabInfos->AddCondition(t,17,12);  // 12 Profilés d'Inox
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,95,1);    // Pompe T2.
               // Voxel DeMaterializer
               t=VoxelType->FabInfos->AddTransformation();  // T2
                 VoxelType->FabInfos->AddCondition(t,20,1);   // 1  Desintegrateurs
                 VoxelType->FabInfos->AddCondition(t,23,1);   // 1  Voxel Conductor
                 VoxelType->FabInfos->AddCondition(t,24,1);   // 1  Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,26,1);   // 1  Récepteur / Transmetteur d'énergie.
                 VoxelType->FabInfos->AddCondition(t,12,6);   // 6  Feuilles d'acier
                 VoxelType->FabInfos->AddCondition(t,16,12);  // 12 Profilés d'acier
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,97,32);    // Voxel Dematerializer
               // Voxel Materializer
               t=VoxelType->FabInfos->AddTransformation();  // T3
                 VoxelType->FabInfos->AddCondition(t,21,1);   // 1  Réintégrateurs
                 VoxelType->FabInfos->AddCondition(t,23,1);   // 1  Voxel Conductor
                 VoxelType->FabInfos->AddCondition(t,24,1);   // 1  Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,26,1);   // 1  Récepteur / Transmetteur d'énergie.
                 VoxelType->FabInfos->AddCondition(t,12,6);   // 6  Feuilles d'acier
                 VoxelType->FabInfos->AddCondition(t,16,12);  // 12 Profilés d'acier
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1  Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,98,32);    // Voxel Materializer

               // Belt Conveyor 104
               t=VoxelType->FabInfos->AddTransformation();  // T4
                 VoxelType->FabInfos->AddCondition(t, 8,1);   // 1 Motor M-11 Little motor
                 VoxelType->FabInfos->AddCondition(t,24,1);   // 1 Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,26,1);   // 1 Récepteur / Transmetteur d'énergie.
                 VoxelType->FabInfos->AddCondition(t,27,1);   // 1 Rouleaux en fonte.
                 VoxelType->FabInfos->AddCondition(t,12,4);   // 4 Feuilles d'acier
                 VoxelType->FabInfos->AddCondition(t,16,8);   // 8 Profilés d'acier
                 VoxelType->FabInfos->AddCondition(t,28,2);   // 2 Blackrock Green
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,104,64);  // Belt Conveyor 104
               // Belt Conveyor 103
               t=VoxelType->FabInfos->AddTransformation();  // T5
                 VoxelType->FabInfos->AddCondition(t, 8,1);   // 1 Motor M-11 Little motor
                 VoxelType->FabInfos->AddCondition(t,24,1);   // 1 Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,26,1);   // 1 Récepteur / Transmetteur d'énergie.
                 VoxelType->FabInfos->AddCondition(t,27,1);   // 1 Rouleaux en fonte.
                 VoxelType->FabInfos->AddCondition(t,12,4);   // 4 Feuilles d'acier
                 VoxelType->FabInfos->AddCondition(t,16,8);   // 8 Profilés d'acier
                 VoxelType->FabInfos->AddCondition(t,28,1);   // 1 Blackrock Green
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,103,64);  // Belt Conveyor 103
               // Belt Conveyor 106
               t=VoxelType->FabInfos->AddTransformation();  // T6
                 VoxelType->FabInfos->AddCondition(t, 8,1);   // 1 Motor M-11 Little motor
                 VoxelType->FabInfos->AddCondition(t,24,1);   // 1 Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,26,1);   // 1 Récepteur / Transmetteur d'énergie.
                 VoxelType->FabInfos->AddCondition(t,27,1);   // 1 Rouleaux en fonte.
                 VoxelType->FabInfos->AddCondition(t,12,4);   // 4 Feuilles d'acier
                 VoxelType->FabInfos->AddCondition(t,16,8);   // 8 Profilés d'acier
                 VoxelType->FabInfos->AddCondition(t,29,2);   // 2 Blackrock Orange
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,106,64);  // Belt Conveyor 106
               // Belt Conveyor 105
               t=VoxelType->FabInfos->AddTransformation();  // T7
                 VoxelType->FabInfos->AddCondition(t, 8,1);   // 1 Motor M-11 Little motor
                 VoxelType->FabInfos->AddCondition(t,24,1);   // 1 Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,26,1);   // 1 Récepteur / Transmetteur d'énergie.
                 VoxelType->FabInfos->AddCondition(t,27,1);   // 1 Rouleaux en fonte.
                 VoxelType->FabInfos->AddCondition(t,12,4);   // 4 Feuilles d'acier
                 VoxelType->FabInfos->AddCondition(t,16,8);   // 8 Profilés d'acier
                 VoxelType->FabInfos->AddCondition(t,29,1);   // 1 Blackrock Orange
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,105,64);  // Belt Coinveyor 105

               // Material Optical Transmitter
               t=VoxelType->FabInfos->AddTransformation();  // T8
                 VoxelType->FabInfos->AddCondition(t,32,1);   // 1 Laser Optical Transmitter/Receiver
                 VoxelType->FabInfos->AddCondition(t,23,1);   // 1 Voxel Conductor
                 VoxelType->FabInfos->AddCondition(t,24,1);   // 1 Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,26,1);   // 1 Récepteur / Transmetteur d'énergie.
                 VoxelType->FabInfos->AddCondition(t,12,4);   // 4 Feuilles d'acier
                 VoxelType->FabInfos->AddCondition(t,16,8);   // 8 Profilés d'acier
                 VoxelType->FabInfos->AddCondition(t,29,1);   // 1 Blackrock Orange
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,100,64);  // Material Optical Transmitter

               // Wireless Transmitter
               t=VoxelType->FabInfos->AddTransformation();  // T9
                 VoxelType->FabInfos->AddCondition(t,22,50);   // 5 Atomic space suppressor
                 VoxelType->FabInfos->AddCondition(t,23,50);   // 5 Voxel Conductor
                 VoxelType->FabInfos->AddCondition(t,24,50);   // 5 Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,26,50);   // 5 Récepteur / Transmetteur d'énergie.
                 VoxelType->FabInfos->AddCondition(t,12,64);  // 20 Feuilles d'acier
                 VoxelType->FabInfos->AddCondition(t,16,64);  // 32 Profilés d'acier
                 VoxelType->FabInfos->AddCondition(t,29,64);   // 5 Blackrock Orange
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,254,1);  // Wireless Transmitter

               // Material Optical Receiver
               t=VoxelType->FabInfos->AddTransformation();  // T10
                 VoxelType->FabInfos->AddCondition(t,32,1);   // 1 Laser Optical Transmitter/Receiver
                 VoxelType->FabInfos->AddCondition(t,23,1);   // 1 Voxel Conductor
                 VoxelType->FabInfos->AddCondition(t,24,1);   // 1 Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,26,1);   // 1 Récepteur / Transmetteur d'énergie
                 VoxelType->FabInfos->AddCondition(t,12,4);   // 4 Feuilles d'acier
                 VoxelType->FabInfos->AddCondition(t,16,12);  // 12 Profilés d'acier
                 VoxelType->FabInfos->AddCondition(t,28,1);   // 1 Blackrock Green
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,101,64);  // Material Optical Receiver

               // Wireless Receiver
               t=VoxelType->FabInfos->AddTransformation();  // T11
                 VoxelType->FabInfos->AddCondition(t,22,50);   //  5 Atomic space suppressor
                 VoxelType->FabInfos->AddCondition(t,23,50);   //  5 Voxel Conductor
                 VoxelType->FabInfos->AddCondition(t,24,50);   //  5 Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,26,50);   //  5 Récepteur / Transmetteur d'énergie
                 VoxelType->FabInfos->AddCondition(t,12,64);  // 20 Feuilles d'acier
                 VoxelType->FabInfos->AddCondition(t,16,64);  // 32 Profilés d'acier
                 VoxelType->FabInfos->AddCondition(t,28,64);   //  5 Blackrock Green
                 VoxelType->FabInfos->AddCondition(t,0,1);    //  1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,255,1);   // Wireless Receiver

               // Optical Splitter
               t=VoxelType->FabInfos->AddTransformation();  // T12
                 VoxelType->FabInfos->AddCondition(t,35,2);   // 2 Optical Prism
                 VoxelType->FabInfos->AddCondition(t,34,1);   // 1 Semi transparent mirror
                 VoxelType->FabInfos->AddCondition(t,16,12);  // 12 Profilés d'acier
                 VoxelType->FabInfos->AddCondition(t,28,1);   // 1 Blackrock Green
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,102,64);  // Optical Splitter.

               // Programmable Robot
               t=VoxelType->FabInfos->AddTransformation();  // T13
                 VoxelType->FabInfos->AddCondition(t,20,6);   // 6 Desintegrateurs
                 VoxelType->FabInfos->AddCondition(t,21,6);   // 6 Réintégrateurs
                 VoxelType->FabInfos->AddCondition(t,23,1);   // 1 Voxel Conductor
                 VoxelType->FabInfos->AddCondition(t,24,8);   // 8 Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,26,2);   // 2 Récepteur / Transmetteur d'énergie.
                 VoxelType->FabInfos->AddCondition(t,25,20);  // 20 Compact storage units.
                 VoxelType->FabInfos->AddCondition(t,36,1);   // 1 Computer Board
                 VoxelType->FabInfos->AddCondition(t,9,8);    // 8 Motor M-12 Medium motor Pump motor
                 VoxelType->FabInfos->AddCondition(t,13,4);   // 4 Feuilles d'inox
                 VoxelType->FabInfos->AddCondition(t,17,12);  // 12 Profilés d'inox
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,108,1);  // Material Optical Receiver

               // Mining Robot XRT-1
               t=VoxelType->FabInfos->AddTransformation();  // T14
                 VoxelType->FabInfos->AddCondition(t,20,500);   // 500  Desintegrateurs
                 VoxelType->FabInfos->AddCondition(t,23,100);   // 100  Voxel Conductor
                 VoxelType->FabInfos->AddCondition(t,24,800);   // 800  Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,26,100);   // 100  Récepteur / Transmetteur d'énergie.
                 VoxelType->FabInfos->AddCondition(t,25,700);   // 700  Compact storage units
                 VoxelType->FabInfos->AddCondition(t,36,100);   // 100  Computer Board
                 VoxelType->FabInfos->AddCondition(t,9,400);    // 400  Motor M-12 Medium motor Pump motor
                 VoxelType->FabInfos->AddCondition(t,13,400);   // 400  Feuilles d'inox
                 VoxelType->FabInfos->AddCondition(t,17,1200);  // 1200 Profilés d'inox
                 VoxelType->FabInfos->AddCondition(t,0,1);      // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,252,1);     // XRT-1 Teleportation Extractor Robot

               // Mining Robot XR-5
               t=VoxelType->FabInfos->AddTransformation();  // T15
                 VoxelType->FabInfos->AddCondition(t,20,50);   // 50  Desintegrateurs
                 VoxelType->FabInfos->AddCondition(t,23,10);   // 10  Voxel Conductor
                 VoxelType->FabInfos->AddCondition(t,24,80);   // 80  Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,26,10);   // 10  Récepteur / Transmetteur d'énergie.
                 VoxelType->FabInfos->AddCondition(t,25,70);   // 70  Compact storage units.
                 VoxelType->FabInfos->AddCondition(t,36,10);   // 10  Computer Board
                 VoxelType->FabInfos->AddCondition(t,9,40);    // 40  Motor M-12 Medium motor Pump motor
                 VoxelType->FabInfos->AddCondition(t,13,40);   // 40  Feuilles d'inox
                 VoxelType->FabInfos->AddCondition(t,17,120);  // 120 Profilés d'inox
                 VoxelType->FabInfos->AddCondition(t,0,1);     // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,157,1);    // XR-5 Mining Robot

              // Aircraft
              t=VoxelType->FabInfos->AddTransformation();  // T16
                VoxelType->FabInfos->AddCondition(t,37,3);    // 3 Propulseurs
                VoxelType->FabInfos->AddCondition(t,38,16);   // 16 Pistons hydrauliques
                VoxelType->FabInfos->AddCondition(t, 8,16);   // 16 Motor M-11 Little motor
                VoxelType->FabInfos->AddCondition(t,39,5);    // 5  Roues.
                VoxelType->FabInfos->AddCondition(t,1,16);    // 16 Axes.
                VoxelType->FabInfos->AddCondition(t,25,20);   // 20 Compact storage units.
                VoxelType->FabInfos->AddCondition(t,26,1);    // 1  Récepteur / Transmetteur d'énergie.
                VoxelType->FabInfos->AddCondition(t,15,32);   // 32 Feuilles d'alu.
                VoxelType->FabInfos->AddCondition(t,19,32);   // 32 Profilés d'alu.
                VoxelType->FabInfos->AddCondition(t,0,1);     // 1 Blackrock Blue (Validation)
                VoxelType->FabInfos->SetResult(t,0,96,1);  // Material Optical Receiver

              // Bomb
              t=VoxelType->FabInfos->AddTransformation();  // T17
                VoxelType->FabInfos->AddCondition(t,40,1);  // 1 Microcontroleur
                VoxelType->FabInfos->AddCondition(t,42,1);  // 1 Material Detector
                VoxelType->FabInfos->AddCondition(t,41,8);  // 8 Explosive Material 325
                VoxelType->FabInfos->AddCondition(t,12,6);  // 6 Feuilles d'acier
                VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                VoxelType->FabInfos->SetResult(t,0,90,1);   // Material Optical Receiver

              // Arc furnace
              t=VoxelType->FabInfos->AddTransformation();  // T18
                VoxelType->FabInfos->AddCondition(t,28,32); // 32 Blackrock Green
                VoxelType->FabInfos->AddCondition(t,13,8);  // 8 Feuille de cuivre
                VoxelType->FabInfos->AddCondition(t,26,1);  // 1 Récepteur / Transmetteur d'énergie
                VoxelType->FabInfos->AddCondition(t,45,2);  // 2 Charbons
                VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                VoxelType->FabInfos->SetResult(t,0,173,1);  // 1 Arc furnace

              // Récepteur / Transmetteur d'énergie
              t=VoxelType->FabInfos->AddTransformation();  // T19
                VoxelType->FabInfos->AddCondition(t,24,4);  // 4 Fil de cuivre
                VoxelType->FabInfos->AddCondition(t,18,8);  // 1 Profilé de cuivre.
                VoxelType->FabInfos->AddCondition(t,14,1);  // 1 Feuille de cuivre.
                VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                VoxelType->FabInfos->SetResult(t,0,179,1);  // 1 Récepteur / Transmetteur d'énergie

              // Afficheur.
              t=VoxelType->FabInfos->AddTransformation();  // T20
                VoxelType->FabInfos->AddCondition(t,35,1);  // 1 Optical Prism
                VoxelType->FabInfos->AddCondition(t,40,1);  // 1 Microcontroleur
                VoxelType->FabInfos->AddCondition(t,26,1);  // 1 Récepteur / Transmetteur d'énergie.
                VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                VoxelType->FabInfos->SetResult(t,0,214,1);  // 1 Indicator Light

              // Interrupteur
              t=VoxelType->FabInfos->AddTransformation();  // T21
                VoxelType->FabInfos->AddCondition(t,24,2);  // 2 Fil de cuivre
                VoxelType->FabInfos->AddCondition(t,18,1);  // 1 Profilé de cuivre.
                VoxelType->FabInfos->AddCondition(t,13,1);  // 1 Feuilles d'inox
                VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                VoxelType->FabInfos->SetResult(t,0,216,1);  // 1 Indicator Light

               // Mining Robot xr-4
               t=VoxelType->FabInfos->AddTransformation();  // T22
                 VoxelType->FabInfos->AddCondition(t,20,6);   // 6 Desintegrateurs
                 VoxelType->FabInfos->AddCondition(t,23,1);   // 1 Voxel Conductor
                 VoxelType->FabInfos->AddCondition(t,24,8);   // 8 Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,26,2);   // 2 Récepteur / Transmetteur d'énergie.
                 VoxelType->FabInfos->AddCondition(t,25,20);  // 20 Compact storage units.
                 VoxelType->FabInfos->AddCondition(t,36,1);   // 1 Computer Board
                 VoxelType->FabInfos->AddCondition(t,9,1);    // 1 Motor M-12 Medium motor Pump motor
                 VoxelType->FabInfos->AddCondition(t,13,4);   // 4 Feuilles d'inox
                 VoxelType->FabInfos->AddCondition(t,17,12);  // 12 Profilés d'inox
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,156,1);  // XR-4 Mining Robot

               // Linker tool
               t=VoxelType->FabInfos->AddTransformation();  // T23
                 VoxelType->FabInfos->AddCondition(t,24,8);   // 8 Fil de cuivre
                 VoxelType->FabInfos->AddCondition(t,26,8);   // 8 Récepteur / Transmetteur d'énergie.
                 VoxelType->FabInfos->AddCondition(t,17,2);   // 2 Profilés d'inox
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,253,1);  // Linker tool

                break;

        // Broyeur
       case 137:VoxelType = new ZVoxelType_FabMachine(i);

                VoxelType->FabInfos = new ZFabInfos;
                VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
                VoxelType->FabInfos->AddMaterial(  3);  // Slot 1 : Blackrock Green
                VoxelType->FabInfos->AddMaterial(  2);  // Slot 2 : Blackrock Orange
                VoxelType->FabInfos->AddMaterial(  5);  // Slot 3 : Blackrock Sky Blue
                VoxelType->FabInfos->AddMaterial(  6);  // Slot 4 : Blackrock Jaune
                VoxelType->FabInfos->AddMaterial(  4);  // Slot 5 : Blackrock Rouge
                VoxelType->FabInfos->AddMaterial(  9);  // Slot 6 : Blackrock Gris
                VoxelType->FabInfos->AddMaterial(  7);  // Slot 7 : Blackrock Pink

                VoxelType->FabInfos->SetPurgeCondition(0,2); // if this slot go above limit, go to purge state mode.
                VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation

                // Sand 1
                t=VoxelType->FabInfos->AddTransformation();    // T0
                  VoxelType->FabInfos->AddCondition(t,1,10);   // 10  BlackRock Green
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1   Blackrock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,152,10);  // 10  Sable

                // Sand 2
                t=VoxelType->FabInfos->AddTransformation();    // T1
                  VoxelType->FabInfos->AddCondition(t,2,10);   // 10  BlackRock Orange
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1   Blackrock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,152,10);  // 10  Sable

                // Sand 3
                t=VoxelType->FabInfos->AddTransformation();    // T2
                  VoxelType->FabInfos->AddCondition(t,3,10);   // 10  Blackrock Sky Blue
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1   Blackrock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,152,10);  // 10  Sable

                // Sand 4
                t=VoxelType->FabInfos->AddTransformation();    // T3
                  VoxelType->FabInfos->AddCondition(t,4,10);   // 10  BlackRock Jaune
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1   Blackrock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,152,10);  // 10  Sable

                // Sand 5
                t=VoxelType->FabInfos->AddTransformation();    // T4
                  VoxelType->FabInfos->AddCondition(t,5,10);   // 10  BlackRock Rouge
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1   Blackrock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,152,10);  // 10  Sable

                // Substance explosive matérial 325
                t=VoxelType->FabInfos->AddTransformation();    // T5
                  VoxelType->FabInfos->AddCondition(t,6,5);    // 5   BlackRock Gris
                  VoxelType->FabInfos->AddCondition(t,7,5);    // 5   BlackRock Pink
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1   Blackrock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,190,10);  // 10  Substance explosive matérial 325 (bombe)

                break;

             // Fraiseuse
      case 139:VoxelType = new ZVoxelType_FabMachine(i);

               VoxelType->FabInfos = new ZFabInfos;
               VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
               VoxelType->FabInfos->AddMaterial(118);  // Slot 1 : Inox Bar
               VoxelType->FabInfos->AddMaterial(117);  // Slot 2 : Steel Bar
               VoxelType->FabInfos->AddMaterial(144);  // Slot 3 : Axe


               VoxelType->FabInfos->SetPurgeCondition(0,2); // if this slot go above limit, go to purge state mode.
               VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation

               // Unité de stockage compacte
               t=VoxelType->FabInfos->AddTransformation();   // T0
                 VoxelType->FabInfos->AddCondition(t,1,8);   // 8 Acier
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,178,5);  // 5 Unité de stockage compacte

               // Propulseur d'avion
               t=VoxelType->FabInfos->AddTransformation();   // T1
                 VoxelType->FabInfos->AddCondition(t,3,1);   //  1 Axe
                 VoxelType->FabInfos->AddCondition(t,2,16);  // 16 Steel Bar
                 VoxelType->FabInfos->AddCondition(t,0,1);   //  1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,186,1);  //  1 Propulseur d'avion

                break;

             // Lathe
      case 140:VoxelType = new ZVoxelType_FabMachine(i);

               VoxelType->FabInfos = new ZFabInfos;
               VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
               VoxelType->FabInfos->AddMaterial(117);  // Slot 1 : Iron bar
               VoxelType->FabInfos->AddMaterial(118);  // Slot 2 : Inox bar


               VoxelType->FabInfos->SetPurgeCondition(0,2); // if this slot go above limit, go to purge state mode.
               VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation

               // Axe
               t=VoxelType->FabInfos->AddTransformation();   // T0
                 VoxelType->FabInfos->AddCondition(t,1,4);   // 1 Acier
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,144,4);  // 1 Axe

               // Rouleau Belt Conveyor
               t=VoxelType->FabInfos->AddTransformation();   // T1
                 VoxelType->FabInfos->AddCondition(t,1,1);   // 1 Acier
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,149,1);  // 1 Rouleau Belt Conveyor

               // Piston Hydraulique
               t=VoxelType->FabInfos->AddTransformation();   // T2
                 VoxelType->FabInfos->AddCondition(t,2,8);   // 8 Inox
                 VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                 VoxelType->FabInfos->SetResult(t,0,187,1);  // 1 Piston Hydraulique

               break;

      // Tréfileuse

      case 142: VoxelType = new ZVoxelType_FabMachine(i);

                VoxelType->FabInfos = new ZFabInfos;
                VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
                VoxelType->FabInfos->AddMaterial(119);  // Slot 1 : Cube de cuivre
                VoxelType->FabInfos->AddMaterial( 20);  // Slot 2 : Basic glass

                VoxelType->FabInfos->SetPurgeCondition(0,2); // if this slot go above limit, go to purge state mode.
                VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation

                // Fil de cuivre.
                t=VoxelType->FabInfos->AddTransformation();  // T0
                  VoxelType->FabInfos->AddCondition(t,1,5);    // 5  Cubes de cuivre.
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock
                  VoxelType->FabInfos->SetResult(t,0,145,20);   // 5 Fils de cuivre.

                // Fibre optique.
                t=VoxelType->FabInfos->AddTransformation();  // T1
                  VoxelType->FabInfos->AddCondition(t,2,10);   // 10 Verres de base
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock
                  VoxelType->FabInfos->SetResult(t,0,99,10);   // 10 Verres basiques

                break;

      // X Machine

      case 143: VoxelType = new ZVoxelType_FabMachine(i);

                VoxelType->FabInfos = new ZFabInfos;
                VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
                VoxelType->FabInfos->AddMaterial(161);  // Slot 1 : Tole de cuivre
                VoxelType->FabInfos->AddMaterial(169);  // Slot 2 : Tole acier
                VoxelType->FabInfos->AddMaterial(170);  // Slot 3 : Profilé acier
                VoxelType->FabInfos->AddMaterial(167);  // Slot 4 : Profilé inox
                VoxelType->FabInfos->AddMaterial( 20);  // Slot 5 : Basic glass
                VoxelType->FabInfos->AddMaterial(189);  // Slot 6 : Microcontroleur
                VoxelType->FabInfos->AddMaterial(185);  // Slot 7 : Prisme
                VoxelType->FabInfos->AddMaterial(44);   // Slot 8 : Matériau X
                VoxelType->FabInfos->AddMaterial(  3);  // Slot 9 : Blackrock Green
                VoxelType->FabInfos->AddMaterial(  2);  // Slot 10 : Blackrock Orange

                VoxelType->FabInfos->SetPurgeCondition(0,2); // if this slot go above limit, go to purge state mode.
                VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation

                // Voxel Conductor
                t=VoxelType->FabInfos->AddTransformation();   // T0
                  VoxelType->FabInfos->AddCondition(t,1,1);   // 1 Tole de cuivre
                  VoxelType->FabInfos->AddCondition(t,3,1);   // 1 Profilé inox
                  VoxelType->FabInfos->AddCondition(t,5,1);   // 1 Basic glass
                  VoxelType->FabInfos->AddCondition(t,6,1);   // 1 Microcontroleur
                  VoxelType->FabInfos->AddCondition(t,7,1);   // 1 Prisme
                  VoxelType->FabInfos->AddCondition(t,8,1);   // 1 Matériau X
                  VoxelType->FabInfos->AddCondition(t,9,16);  // 16 Blackrock Green
                  VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,176,1);  // 1 Voxel Conductor

                // Réintégrateur
                t=VoxelType->FabInfos->AddTransformation();   // T1
                  VoxelType->FabInfos->AddCondition(t,1,1);   // 1 Tole de cuivre
                  VoxelType->FabInfos->AddCondition(t,2,1);   // 1 Tole d'acier
                  VoxelType->FabInfos->AddCondition(t,3,1);   // 1 Profilé acier
                  VoxelType->FabInfos->AddCondition(t,5,1);   // 1 Basic glass
                  VoxelType->FabInfos->AddCondition(t,6,1);   // 1 Microcontroleur
                  VoxelType->FabInfos->AddCondition(t,7,1);   // 1 Prisme
                  VoxelType->FabInfos->AddCondition(t,8,1);   // 1 Matériau X
                  VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,177,1);  // 1 Réintégrateur

                // Désintégrateur
                t=VoxelType->FabInfos->AddTransformation();   // T2
                  VoxelType->FabInfos->AddCondition(t,1,1);   // 1 Tole de cuivre
                  VoxelType->FabInfos->AddCondition(t,3,1);   // 1 Profilé acier
                  VoxelType->FabInfos->AddCondition(t,5,1);   // 1 Basic glass
                  VoxelType->FabInfos->AddCondition(t,6,1);   // 1 Microcontroleur
                  VoxelType->FabInfos->AddCondition(t,7,1);   // 1 Prisme
                  VoxelType->FabInfos->AddCondition(t,8,1);   // 1 Matériau X
                  VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,174,1);  // 1 Désintégrateur

                // Réducteur d'espace atomique
                t=VoxelType->FabInfos->AddTransformation();   // T3
                  VoxelType->FabInfos->AddCondition(t,2,1);   // 2 Tole d'acier
                  VoxelType->FabInfos->AddCondition(t,3,1);   // 2 Profilé acier
                  VoxelType->FabInfos->AddCondition(t,5,1);   // 1 Basic glass
                  VoxelType->FabInfos->AddCondition(t,6,1);   // 1 Microcontroleur
                  VoxelType->FabInfos->AddCondition(t,8,1);   // 1 Matériau X
                  VoxelType->FabInfos->AddCondition(t,10,16); // 16 Blackrock Orange
                  VoxelType->FabInfos->AddCondition(t,0,1);   // 1 Blackrock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,175,1);  // 1 Réducteur d'espace atomique

                  break;

      case 153:
      case 154:
      case 155:
      case 156:
      case 157:
      case 158:
      case 159:VoxelType = new ZVoxelType_MiningRobot_xr1(i);        break;

      // Four à verre.
      case 172:VoxelType = new ZVoxelType_FabMachine(i);

               VoxelType->FabInfos = new ZFabInfos;
               VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
               VoxelType->FabInfos->AddMaterial( 60);  // Slot 1 : Charbon
               VoxelType->FabInfos->AddMaterial(152);  // Slot 2 : Sand
               VoxelType->FabInfos->AddMaterial(151);  // Slot 3 : Plomb

               VoxelType->FabInfos->SetPurgeCondition(0,2); // if this slot go above limit, go to purge state mode.
               VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation

               // Sand 1
               t=VoxelType->FabInfos->AddTransformation();  // T0
                 VoxelType->FabInfos->AddCondition(t,1,5);    // 5  Charbons
                 VoxelType->FabInfos->AddCondition(t,2,8);    // 8  Sable
                 VoxelType->FabInfos->AddCondition(t,3,2);    // 2  Bloc de plomb
                 VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock
                 VoxelType->FabInfos->SetResult(t,0,20,10);   // 10 Verre Ordinaire
               break;

               // Arc furnace
      case 173: VoxelType = new ZVoxelType_FabMachine(i);
                VoxelType->FabInfos = new ZFabInfos;
                VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
                VoxelType->FabInfos->AddMaterial(  5);  // Slot 1 : Blackrock Blue Sky
                VoxelType->FabInfos->AddMaterial( 20);  // Slot 2 : Verre Ordinaire
                VoxelType->FabInfos->AddMaterial( 44);  // Slot 3 : Matériau X
                VoxelType->FabInfos->AddMaterial( 74);  // Slot 4 : Minerai de fer
                VoxelType->FabInfos->AddMaterial( 26);  // Slot 5 : Minerai d'or
                VoxelType->FabInfos->AddMaterial(121);  // Slot 6 : Minerai de chrome
                VoxelType->FabInfos->AddMaterial(124);  // Slot 7 : Titanium Bar

                VoxelType->FabInfos->SetPurgeCondition(0,2); // if this slot go above limit, go to purge state mode.
                VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation

                // Silicon Wafer
                t=VoxelType->FabInfos->AddTransformation();  // T0
                  VoxelType->FabInfos->AddCondition(t,1,10);   // 10 BlackRock Sky Blue
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1 BlackRock
                  VoxelType->FabInfos->SetResult(t,0,194,1);   // 1  Silicon Wafer

                // Verre bleu
                t=VoxelType->FabInfos->AddTransformation();  // T1
                  VoxelType->FabInfos->AddCondition(t,2,20);   // 20 Verre ordinaire.
                  VoxelType->FabInfos->AddCondition(t,3,1);    // 1 Matériau X
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1 BlackRock
                  VoxelType->FabInfos->SetResult(t,0,21,1);    // 1 Verre bleu

                // Verre vert
                t=VoxelType->FabInfos->AddTransformation();  // T2
                  VoxelType->FabInfos->AddCondition(t,2,20);   // 20 Verre ordinaire.
                  VoxelType->FabInfos->AddCondition(t,4,1);    // 1 Minerai de fer
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1 BlackRock
                  VoxelType->FabInfos->SetResult(t,0,22,20);   // 10 Verre vert

                // Verre rouge
                t=VoxelType->FabInfos->AddTransformation();  // T3
                  VoxelType->FabInfos->AddCondition(t,2,20);   // 20 Verre ordinaire.
                  VoxelType->FabInfos->AddCondition(t,5,1);    // 1 Minerai d'or
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1 BlackRock
                  VoxelType->FabInfos->SetResult(t,0,80,20);    // 5 Verre rouge

                // Verre jaune
                t=VoxelType->FabInfos->AddTransformation();  // T4
                  VoxelType->FabInfos->AddCondition(t,2,20);   // 20 Verre ordinaire.
                  VoxelType->FabInfos->AddCondition(t,6,1);    // 1 Minerai de chrome
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1 BlackRock
                  VoxelType->FabInfos->SetResult(t,0,81,20);    // 5 Verre jaune

                // Verre gris
                t=VoxelType->FabInfos->AddTransformation();  // T5
                  VoxelType->FabInfos->AddCondition(t,2,20);   // 20 Verre ordinaire.
                  VoxelType->FabInfos->AddCondition(t,7,1);    // 1 Titanium bar
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1 BlackRock
                  VoxelType->FabInfos->SetResult(t,0,82,20);    // 5 Verre gris

                // Prisme
                t=VoxelType->FabInfos->AddTransformation();  // T6
                  VoxelType->FabInfos->AddCondition(t,2,20);   // 20 Verre ordinaire.
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1 BlackRock
                  VoxelType->FabInfos->SetResult(t,0,185,5);   // 5 Prismes

                break;

      // Unité de dépot en phase vapeur.
      case 182: VoxelType = new ZVoxelType_FabMachine(i);
                VoxelType->FabInfos = new ZFabInfos;
                VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
                VoxelType->FabInfos->AddMaterial( 20);  // Slot 1 : Verre de base
                VoxelType->FabInfos->AddMaterial(124);  // Slot 2 : Barre de Titane


                VoxelType->FabInfos->SetPurgeCondition(0,2); // if this slot go above limit, go to purge state mode.
                VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation

                // Mirror
                t=VoxelType->FabInfos->AddTransformation();  // T0
                VoxelType->FabInfos->AddCondition(t,1,20);   // 20 Basic Glass
                VoxelType->FabInfos->AddCondition(t,2,4);    // 4  Chrome bars
                VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock
                VoxelType->FabInfos->SetResult(t,0,183,10);  // 10  Mirror

                // Semi Transparent Mirror
                t=VoxelType->FabInfos->AddTransformation();  // T0
                VoxelType->FabInfos->AddCondition(t,1,20);   // 20 Basic Glass
                VoxelType->FabInfos->AddCondition(t,2,1);    // 4  Chrome bars
                VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock
                VoxelType->FabInfos->SetResult(t,0,184,10);  // 10  Semi Transparent Mirror
                break;

      // Laser Cutting Machine
      case 195: VoxelType = new ZVoxelType_FabMachine(i);
                VoxelType->FabInfos = new ZFabInfos;
                VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
                VoxelType->FabInfos->AddMaterial(169);  // Slot 1 : Tole acier
                VoxelType->FabInfos->AddMaterial(144);  // Slot 2 : Axe

                VoxelType->FabInfos->SetPurgeCondition(0,2); // if this slot go above limit, go to purge state mode.
                VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation

                // Induit de moteur
                t=VoxelType->FabInfos->AddTransformation();    // T0
                  VoxelType->FabInfos->AddCondition(t,1,2);    // 2 Tole acier
                  VoxelType->FabInfos->AddCondition(t,2,1);    // 1 Axe
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1 Blackrock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,192,1);   // 1 Induit de moteur

                // Corps de moteur
                t=VoxelType->FabInfos->AddTransformation();    // T1
                  VoxelType->FabInfos->AddCondition(t,1,4);    // 4 Tole acier
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1 Blackrock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,193,4);   // 1 Corps de moteur

                  break;

      // Four
      case 196: VoxelType = new ZVoxelType_FabMachine(i);
                VoxelType->FabInfos = new ZFabInfos;
                VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
                VoxelType->FabInfos->AddMaterial(  3);  // Slot 1 : Blackrock Green
                VoxelType->FabInfos->AddMaterial(  2);  // Slot 2 : Blackrock Orange
                VoxelType->FabInfos->AddMaterial(  5);  // Slot 3 : Blackrock Sky Blue
                VoxelType->FabInfos->AddMaterial(  6);  // Slot 4 : Blackrock Jaune
                VoxelType->FabInfos->AddMaterial(  4);  // Slot 5 : Blackrock Rouge
                VoxelType->FabInfos->AddMaterial(  8);  // Slot 6 : Blackrock Vert citron
                VoxelType->FabInfos->AddMaterial(  9);  // Slot 7 : Blackrock Gris
                VoxelType->FabInfos->AddMaterial(  7);  // Slot 8 : Blackrock Rose
                VoxelType->FabInfos->AddMaterial( 10);  // Slot 9 : Blackrock Blanc
                VoxelType->FabInfos->AddMaterial(152);  // Slot 10: Sand
                VoxelType->FabInfos->AddMaterial( 53);  // Slot 11: Blue Crystal

                VoxelType->FabInfos->SetPurgeCondition(0,2); // if this slot go above limit, go to purge state mode.
                VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation

                // Dallage gris irrégulier
                t=VoxelType->FabInfos->AddTransformation();    // T0
                  VoxelType->FabInfos->AddCondition(t,1,4);    // 4  Blackrock Green
                  VoxelType->FabInfos->AddCondition(t,2,10);   // 10 Blackrock Orange
                  VoxelType->FabInfos->AddCondition(t,3,10);   // 10 Blackrock Sky Blue
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,11,20);   // 20 Dallage gris irrégulier

                // Dallage gris clair
                t=VoxelType->FabInfos->AddTransformation();    // T1
                  VoxelType->FabInfos->AddCondition(t,1,4);    // 4  Blackrock Green
                  VoxelType->FabInfos->AddCondition(t,2,10);   // 10 Blackrock Orange
                  VoxelType->FabInfos->AddCondition(t,4,10);   // 10 Blackrock Jaune
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,12,20);   // 20 Dallage gris clair

                // Dallage gris foncé
                t=VoxelType->FabInfos->AddTransformation();    // T2
                  VoxelType->FabInfos->AddCondition(t,1,4);    // 4  Blackrock Green
                  VoxelType->FabInfos->AddCondition(t,2,10);   // 10 Blackrock Orange
                  VoxelType->FabInfos->AddCondition(t,5,10);   // 10 Blackrock Rouge
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,13,20);   // 20 Dallage gris foncé

                // Gallet gris
                t=VoxelType->FabInfos->AddTransformation();    // T3
                  VoxelType->FabInfos->AddCondition(t,1,4);    // 4  Blackrock Green
                  VoxelType->FabInfos->AddCondition(t,2,10);   // 10 Blackrock Orange
                  VoxelType->FabInfos->AddCondition(t,6,10);   // 10 Blackrock Vert citron
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,30,20);   // 20 Galet gris

                // Gallet gris argent
                t=VoxelType->FabInfos->AddTransformation();    // T4
                  VoxelType->FabInfos->AddCondition(t,1,4);    // 4  Blackrock Green
                  VoxelType->FabInfos->AddCondition(t,2,10);   // 10 Blackrock Orange
                  VoxelType->FabInfos->AddCondition(t,7,10);   // 10 Blackrock Gris
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,31,20);   // 20 Galet gris argent

                // Mosaique
                t=VoxelType->FabInfos->AddTransformation();    // T5
                  VoxelType->FabInfos->AddCondition(t,1,4);    // 4  Blackrock Green
                  VoxelType->FabInfos->AddCondition(t,2,10);   // 10 Blackrock Orange
                  VoxelType->FabInfos->AddCondition(t,8,10);   // 10 Blackrock Rose
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,32,20);   // 20 Mosaique

                // Dallage labyrinthe
                t=VoxelType->FabInfos->AddTransformation();    // T6
                  VoxelType->FabInfos->AddCondition(t,1,4);    // 4  Blackrock Green
                  VoxelType->FabInfos->AddCondition(t,2,10);   // 10 Blackrock Orange
                  VoxelType->FabInfos->AddCondition(t,9,10);   // 10 Blackrock Blanc
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,36,20);   // 20 Dallage labyrinthe

                // Dallage labyrinthe sans bordure
                t=VoxelType->FabInfos->AddTransformation();    // T7
                  VoxelType->FabInfos->AddCondition(t,1,4);    // 4  Blackrock Green
                  VoxelType->FabInfos->AddCondition(t,3,10);   // 10 Blackrock Sky Blue
                  VoxelType->FabInfos->AddCondition(t,9,10);   // 10 Blackrock Blanc
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,37,20);   // 20 Dallage labyrinthe sans bordure

                // Beton beige
                t=VoxelType->FabInfos->AddTransformation();    // T8
                  VoxelType->FabInfos->AddCondition(t,1,4);    // 4  Blackrock Green
                  VoxelType->FabInfos->AddCondition(t,4,10);   // 10 Blackrock Jaune
                  VoxelType->FabInfos->AddCondition(t,8,10);   // 10 Blackrock Rose
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,38,20);   // 20 Beton beige

                // Dallage moucheté
                t=VoxelType->FabInfos->AddTransformation();    // T9
                  VoxelType->FabInfos->AddCondition(t,3,4);    // 4  Blackrock Sky Blue
                  VoxelType->FabInfos->AddCondition(t,5,10);   // 10 Blackrock Rouge
                  VoxelType->FabInfos->AddCondition(t,6,10);   // 10 Blackrock Vert citron
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,39,20);   // 20 Dallage moucheté

                // Dallage noir et blanc
                t=VoxelType->FabInfos->AddTransformation();    // T10
                  VoxelType->FabInfos->AddCondition(t,3,4);    // 4  Blackrock Sky Blue
                  VoxelType->FabInfos->AddCondition(t,6,10);   // 10 Blackrock Vert citron
                  VoxelType->FabInfos->AddCondition(t,7,10);   // 10 Blackrock Gris
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,41,20);   // 20 Dallage noir et blanc

                // Galet noir brillant
                t=VoxelType->FabInfos->AddTransformation();    // T11
                  VoxelType->FabInfos->AddCondition(t,7,4);    // 4  Blackrock Gris
                  VoxelType->FabInfos->AddCondition(t,8,10);   // 10 Blackrock Rose
                  VoxelType->FabInfos->AddCondition(t,9,10);   // 10 Blackrock Blanc
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,62,20);   // 20 Galet noir brillant

                // Pierre granit
                t=VoxelType->FabInfos->AddTransformation();    // T12
                  VoxelType->FabInfos->AddCondition(t,3,4);    // 4  Blackrock Sky Blue
                  VoxelType->FabInfos->AddCondition(t,7,10);   // 10 Blackrock Gris
                  VoxelType->FabInfos->AddCondition(t,9,10);   // 10 Blackrock Blanc
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,63,20);   // 20 Pierre granit

                // Pierre qui roule
                t=VoxelType->FabInfos->AddTransformation();    // T13
                  VoxelType->FabInfos->AddCondition(t,1,4);    // 4  Blackrock Green
                  VoxelType->FabInfos->AddCondition(t,3,10);   // 10 Blackrock Sky Blue
                  VoxelType->FabInfos->AddCondition(t,4,10);   // 10 Blackrock Jaune
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,84,20);   // 20 Pierre qui roule

                // Airport Runway (Update 2.0)
                t=VoxelType->FabInfos->AddTransformation();    // T14
                  VoxelType->FabInfos->AddCondition(t,10,10);  // 10 Sand
                  VoxelType->FabInfos->AddCondition(t,11,10);  // 10 Blue Crystal
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,240,2);   // 2  Airport Runway

                  break;

      case 198: VoxelType = new ZVoxelType_Sequencer(i);             break;
      case 204: VoxelType = new ZVoxelType_Picker(i);                break;
      case 209: VoxelType = new ZVoxelType_Mover(i);                 break;

      // Printing Unit
      case 210: VoxelType = new ZVoxelType_FabMachine(i);
                VoxelType->FabInfos = new ZFabInfos;
                VoxelType->FabInfos->AddMaterial(  1);  // Slot 0 : Blackrock Blue
                VoxelType->FabInfos->AddMaterial(  3);  // Slot 1 : Blackrock Green
                VoxelType->FabInfos->AddMaterial(  2);  // Slot 2 : Blackrock Orange
                VoxelType->FabInfos->AddMaterial(  5);  // Slot 3 : Blackrock Sky Blue
                VoxelType->FabInfos->AddMaterial(  6);  // Slot 4 : Blackrock Jaune
                VoxelType->FabInfos->AddMaterial(  4);  // Slot 5 : Blackrock Rouge
                VoxelType->FabInfos->AddMaterial(  8);  // Slot 6 : Blackrock Vert citron
                VoxelType->FabInfos->AddMaterial(  9);  // Slot 7 : Blackrock Gris
                VoxelType->FabInfos->AddMaterial(  7);  // Slot 8 : Blackrock Rose
                VoxelType->FabInfos->AddMaterial( 10);  // Slot 9 : Blackrock Blanc

                VoxelType->FabInfos->SetPurgeCondition(0,2); // if this slot go above limit, go to purge state mode.
                VoxelType->FabInfos->SetValidationCondition(0,1); // If condition reached, validate input and allow transformation


                // Symbole "Alignment Voxel"
                t=VoxelType->FabInfos->AddTransformation();    // T0
                  VoxelType->FabInfos->AddCondition(t,1,16);   // 16  Blackrock Green
                  VoxelType->FabInfos->AddCondition(t,2,3);    // 3  Blackrock Orange
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,208,16);  // 16 Alignment Voxels

                // Symbole "Out"
                t=VoxelType->FabInfos->AddTransformation();    // T1
                  VoxelType->FabInfos->AddCondition(t,1,8);    // 8  Blackrock Green
                  VoxelType->FabInfos->AddCondition(t,2,3);    // 3  Blackrock Orange
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,206,16);  // 16 Symboles In

                // Symbole "In"
                t=VoxelType->FabInfos->AddTransformation();    // T2
                  VoxelType->FabInfos->AddCondition(t,1,8);    // 8  Blackrock Green
                  VoxelType->FabInfos->AddCondition(t,2,2);    // 2  Blackrock Orange
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,205,16);  // 16 Symboles In

                // Symbole "End"
                t=VoxelType->FabInfos->AddTransformation();    // T3
                  VoxelType->FabInfos->AddCondition(t,1,8);    // 8  Blackrock Green
                  VoxelType->FabInfos->AddCondition(t,2,1);    // 1  Blackrock Orange
                  VoxelType->FabInfos->AddCondition(t,0,1);    // 1  BlackRock Blue (Validation)
                  VoxelType->FabInfos->SetResult(t,0,199,16);  // 16 Symboles End
                   break;
      case 212: VoxelType = new ZVoxelType_RTFM(i);                  break;
      case 213: VoxelType = new ZVoxelType_Egmy_T1(i);               break;
      case 214 ... 215: VoxelType = new ZVoxelType_SimpleButton(i);  break;
      case 216 ... 231: VoxelType = new ZVoxelType_IndicatorLight(i);break;
      case 235: VoxelType = new ZVoxelType_Concrete(i);              break;
      case 236:
      case 237: VoxelType = new ZVoxelType_ProgRobot_Asm(i);         break;
      case 238: VoxelType = new ZVoxelType_Example(i);               break;
      case 239: VoxelType = new ZVoxelType_PlaneZ1(i);               break;
      case 241: VoxelType = new ZVoxelType_SPS(i);                   break;
      case 254: VoxelType = new ZVoxelType_WirelessTransmitter(i);   break;
      case 255: VoxelType = new ZVoxelType_WirelessReceiver(i);      break;
      case 256 ... 259 : VoxelType = new ZVoxelType_ProgRobot_Remote(i);break;

      default:  VoxelType = new ZVoxelType(i);                       break;
    }

    VoxelType->SetGameEnv(GameEnv);
    VoxelType->SetManager(this);
    if (VoxelType->LoadTexture())
    {
      AddVoxelType(i, VoxelType);
      VoxelType->LoadVoxelInformations();
      ActiveTable->Set(i,VoxelType->Is_Active);
      LoadedTexturesCount++;
    }
    else {delete VoxelType; break;}
  }

  // User textures

  for (i=32768;i<65535;i++)
  {
    VoxelType = new ZVoxelType(i);
    VoxelType->SetGameEnv(GameEnv);
    if (VoxelType->LoadTexture())
    {
      AddVoxelType(i, VoxelType);
      VoxelType->LoadVoxelInformations();
      ActiveTable->Set(i,VoxelType->Is_Active);
    }
    else {delete VoxelType; break;}
  }

  // Universe Boundary Voxel

  VoxelType = new ZVoxelType_UniverseBorder(65535);
  VoxelType->SetGameEnv(GameEnv);
  AddVoxelType(65535,VoxelType);

  return(true);
}
