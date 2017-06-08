/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2017 Laurent Thiebaut & Olivia Merle
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
 * ZVoxelType_ProgRobot_Web.cpp
 *
 *  Created on: 22 mai 2017
 *      Author: laurent
 */

#include "ZVoxelType_ProgRobot_Web.h"

#include "ZGame.h"

#ifndef Z_ZVOXELEXTENSION_PROGROBOT_WEB_H
#  include "ZVoxelExtension_ProgRobot_Web.h"
#endif

#ifndef Z_ZACTIVEVOXELINTERFACE_H
#  include "ZActiveVoxelInterface.h"
#endif

#ifndef Z_ZSIMPLEREQUESTPARSER_H
#  include "z/ZSimpleRequestParser.h"
#endif

#ifndef Z_ZGAMEWINDOW_ZPROGROBOT_WEB_H
#  include "ZGameWindow_ZProgRobot_Web.h"
#endif


ZVoxelExtension * ZVoxelType_ProgRobot_Web::CreateVoxelExtension(bool IsLoadingPhase)
{
  ZVoxelExtension_ProgRobot_Web * NewVoxelExtension = 0;

  NewVoxelExtension = new ZVoxelExtension_ProgRobot_Web;

  return (NewVoxelExtension);
}

void ZVoxelType_ProgRobot_Web::DeleteVoxelExtension(ZMemSize VoxelExtension, bool IsUnloadingPhase)
{
  ZVoxelExtension_ProgRobot_Asm * Ext;

  Ext = (ZVoxelExtension_ProgRobot_Asm *)VoxelExtension;
  if (!Ext) return;

  // Stop Execution


  // If there is a window opened on this robot, then close it before it could displays bad memory locations (or crash).

  if (GameEnv->GameWindow_ProgRobot_Asm)
  {
    // Does we are displaying the extension data of the robot we are about to get out of memory ?
    if (GameEnv->GameWindow_ProgRobot_Asm->GetVoxelExtension() == Ext) GameEnv->GameWindow_ProgRobot_Asm->Hide();
  }

  if (GameEnv->GameWindow_AsmDebug)
  {
    // Does we are displaying the extension data of the robot we are about to get out of memory ?
    if (GameEnv->GameWindow_AsmDebug->GetVoxelExtension() == Ext) GameEnv->GameWindow_AsmDebug->Hide();
  }

  // Ok, then we can destroy extension.

  delete Ext;
}

void  ZVoxelType_ProgRobot_Web::UserAction_Activate(ZMemSize VoxelInfo, Long x, Long y, Long z)
{
  if (GameEnv->GameWindow_ProgRobot_Web->Is_Shown()) return;
  GameEnv->GameWindow_ProgRobot_Web->SetVoxelExtension((ZVoxelExtension *)VoxelInfo);
  GameEnv->GameWindow_ProgRobot_Web->Show();
}

ULong ZVoxelType_ProgRobot_Web::Interface_PushBlock_Push( ZVoxelLocation * DestLocation, UShort VoxelType, ULong Count )
{
  ZVoxelExtension_ProgRobot_Asm * Ext_Storage;

  Ext_Storage = (ZVoxelExtension_ProgRobot_Asm *)DestLocation->Sector->OtherInfos[DestLocation->Offset];

  if (Ext_Storage)
  {
    return(Ext_Storage->StoreBlocks(VoxelType, Count));
  }

  return(0);
}

ULong ZVoxelType_ProgRobot_Web::Interface_PushBlock_Pull( ZVoxelLocation * DestLocation,  UShort * VoxelType, ULong Count )
{
  ZVoxelExtension_ProgRobot_Asm * Ext_Storage;
  ULong SlotNum;
  Ext_Storage = (ZVoxelExtension_ProgRobot_Asm *)DestLocation->Sector->OtherInfos[DestLocation->Offset];

  if (Ext_Storage)
  {
    SlotNum = Ext_Storage->FindFirstUsedBlock();
    if (SlotNum== (ULong)(-1)) return(0);

    return(Ext_Storage->UnstoreBlocks(SlotNum,Count, VoxelType));
  }

  return(0);
}

ULong ZVoxelType_ProgRobot_Web::Interface_PushBlock_PullTest( ZVoxelLocation * DestLocation,  UShort * VoxelType, ULong Count )
{
  ZVoxelExtension_ProgRobot_Asm * Ext_Storage;
  ULong SlotNum;
  Ext_Storage = (ZVoxelExtension_ProgRobot_Asm *)DestLocation->Sector->OtherInfos[DestLocation->Offset];

  if (Ext_Storage)
  {
    SlotNum = Ext_Storage->FindFirstUsedBlock();
    if (SlotNum== (ULong)(-1)) return(0);

    *VoxelType = Ext_Storage->VoxelType[SlotNum];
    return( (Ext_Storage->VoxelQuantity[SlotNum] > Count) ? Count : Ext_Storage->VoxelQuantity[SlotNum] );
  }

  return(0);
}

void ZVoxelType_ProgRobot_Web::ActiveProcess( ZActiveVoxelInterface * AvData)
{
  ZVoxelExtension_ProgRobot_Web * Ext;
  ZWebRobotManager::Request * Req;
  ZString As, Bs, Cs;
  ZWebRobotManager * RobotManager;

  Ext = (ZVoxelExtension_ProgRobot_Web *)AvData->GetVoxelExtension_Main();
  Ext->SetActiveVoxelInterface(AvData);

  // Set home location if isn't already set.

  if (!Ext->HomeSet)
  {
    Ext->HomeSet = true;
    Ext->HomeLocation = AvData->Coords;
  }

  RobotManager = &AvData->GameEnv->VoxelProcessor->WebRobotManager;

  if ((Req = RobotManager->PopRequest()))
  {
    ZSimpleRequestParser Parser;


    if (Parser.ParseRequest(Req->Text))
    {
      if (   Parser.RequestType == "GET"
          /*&& Parser.Proto       == "HTTP/1.1"*/)
      {

        UShort Direction = (AvData->GetVoxelType_Main() - 256) & 3;

/*
        if (Parser.Command=="do_advance")
        {
          if (!AvData->GetNeighborVoxel(Direction))
          {
            AvData->MoveThis(Direction, ZActiveVoxelInterface::CHANGE_CRITICAL);
          }

          RobotManager->Answer_NoParameters(Parser, Req);
        }

        if (Parser.Command=="do_goback")
        {
          Direction = (Direction + 2) & 3;

          if (!AvData->GetNeighborVoxel(Direction))
          {
            AvData->MoveThis(Direction, ZActiveVoxelInterface::CHANGE_CRITICAL);
          }

          RobotManager->Answer_NoParameters(Parser, Req);
        }

        if (Parser.Command=="do_goup")
        {
          if (!AvData->GetNeighborVoxel(4))
          {
            AvData->MoveThis(4, ZActiveVoxelInterface::CHANGE_CRITICAL);
          }

          RobotManager->Answer_NoParameters(Parser, Req);
        }

        if (Parser.Command=="do_godown")
        {
          AvData->MoveThis(5, ZActiveVoxelInterface::CHANGE_CRITICAL);

          RobotManager->Answer_NoParameters(Parser, Req);
        }

        if (Parser.Command=="do_turnleft")
        {
          Direction = (Direction - 1) & 3;
          AvData->SetVoxel_Main(256+Direction, ZActiveVoxelInterface::CHANGE_CRITICAL );

          RobotManager->Answer_NoParameters(Parser, Req);
        }

        if (Parser.Command=="do_turnright")
        {
          Direction = (Direction + 1) & 3;
          AvData->SetVoxel_Main(256+Direction, ZActiveVoxelInterface::CHANGE_CRITICAL );

          RobotManager->Answer_NoParameters(Parser, Req);
        }
*/


        if (Parser.Command=="do_home")
        {
          if (!AvData->GetVoxel(&Ext->HomeLocation))
          {
            AvData->MoveThis_Abs(&Ext->HomeLocation, ZActiveVoxelInterface::CHANGE_CRITICAL);
          }


          RobotManager->Answer_NoParameters(Parser, Req);
        }

        if (Parser.Command=="do_dig")
        {

          UShort DigDirection;
          ZString DirTextParameter;

          if (Parser.FindEntryText((char *)"dir",DirTextParameter))
          {
            DigDirection = DirTextParameter.GetULong();

            if (DigDirection <= 3) DigDirection = (Direction + DigDirection) & 3;

            UShort VoxelType = AvData->GetNeighborVoxel(DigDirection);
            //printf("Dig Direction : %d\n", (ULong)DigDirection);
            //printf("VoxelType=%d\n", VoxelType);

            if (GameEnv->VoxelTypeManager.GetVoxelType(VoxelType)->BvProp_PrRobotPickMinLevel < 1)
            {
              if (Ext->StoreBlocks(VoxelType,1)==1)
              {
                AvData->SetNeighborVoxel(DigDirection, 0, ZActiveVoxelInterface::CHANGE_CRITICAL );
              }
            }

            //AvData->SetVoxel_Main(256+Direction, ZActiveVoxelInterface::CHANGE_CRITICAL );
          }

          //Parser.Dump();

          RobotManager->Answer_NoParameters(Parser, Req);
        }


        if (Parser.Command=="do_move")
        {

          UShort MoveDirection;
          ZString DirTextParameter;

          if (Parser.FindEntryText((char *)"dir",DirTextParameter))
          {
            MoveDirection = DirTextParameter.GetULong();

            if (MoveDirection <= 3) MoveDirection = (Direction + MoveDirection) & 3;

            UShort VoxelType = AvData->GetNeighborVoxel(MoveDirection);
            printf("Move Direction : %d\n", (ULong)MoveDirection);
            printf("VoxelType=%d\n", VoxelType);

            if (VoxelType == 0)
            {
              AvData->MoveThis(MoveDirection, ZActiveVoxelInterface::CHANGE_CRITICAL );
            }
          }

          //Parser.Dump();

          RobotManager->Answer_NoParameters(Parser, Req);
        }

        if (Parser.Command=="do_turn")
        {

          UShort TurnDirection;
          ZString DirTextParameter;

          if (Parser.FindEntryText((char *)"dir",DirTextParameter))
          {
            TurnDirection = DirTextParameter.GetULong();

            switch(TurnDirection)
            {
              default:
              case 0: Direction = (Direction + 1) & 3;
                      AvData->SetVoxel_Main(256+Direction, ZActiveVoxelInterface::CHANGE_CRITICAL );
                      RobotManager->Answer_NoParameters(Parser, Req);
                      break;

              case 1: Direction = (Direction - 1) & 3;
                      AvData->SetVoxel_Main(256+Direction, ZActiveVoxelInterface::CHANGE_CRITICAL );
                      RobotManager->Answer_NoParameters(Parser, Req);
                      break;

            }
          }

          //Parser.Dump();

          RobotManager->Answer_NoParameters(Parser, Req);
        }

        if (Parser.Command=="do_place")
        {

          UShort PlaceDirection, NewVoxelType;
          ZString DirTextParameter;
          ULong Slot;

          if (Parser.FindEntryText((char *)"dir",DirTextParameter))
          {
            PlaceDirection = DirTextParameter.GetULong();

            if (PlaceDirection <= 3) PlaceDirection = (Direction + PlaceDirection) & 3;

            UShort VoxelType = AvData->GetNeighborVoxel(PlaceDirection);
            printf("Place Direction : %d\n", (ULong)PlaceDirection);
            printf("VoxelType=%d\n", VoxelType);

            if (VoxelType == 0)
            {
              if (Ext->FindSlot(Ext->BuildVoxelType,Slot))
              {
                if (1==Ext->UnstoreBlocks(Slot,Ext->BuildVoxelType, &NewVoxelType ))
                {
                  AvData->SetNeighborVoxel(PlaceDirection, Ext->BuildVoxelType, ZActiveVoxelInterface::CHANGE_CRITICAL );
                }
              }
              else if (GameEnv->Settings_Hardware->Experimental_LearningMode)
              {
                AvData->SetNeighborVoxel(PlaceDirection, Ext->BuildVoxelType, ZActiveVoxelInterface::CHANGE_CRITICAL );
              }

            }

            //AvData->SetVoxel_Main(256+Direction, ZActiveVoxelInterface::CHANGE_CRITICAL );
          }

          RobotManager->Answer_NoParameters(Parser, Req);
        }


        if (Parser.Command=="get_coord")
        {

          ULong CoordType;
          ZString InfoTextParameter;
          Long Coord;

          Coord = 0;
          if (Parser.FindEntryText((char *)"info",InfoTextParameter))
          {
            CoordType = InfoTextParameter.GetULong();

            switch(CoordType)
            {
              case 0: Coord = AvData->Coords.x - Ext->HomeLocation.x; break;
              case 1: Coord = AvData->Coords.y - Ext->HomeLocation.y; break;
              case 2: Coord = AvData->Coords.z - Ext->HomeLocation.z; break;
              case 3: Coord = AvData->Coords.x;  break;
              case 4: Coord = AvData->Coords.y;  break;
              case 5: Coord = AvData->Coords.z;  break;
            }
          }

          InfoTextParameter = Coord;
          RobotManager->Answer_OneParameter(Parser, Req, InfoTextParameter);
        }

        if (Parser.Command=="get_look")
        {

          UShort Direction;
          ZString DirTextParameter, ReturnInfo;

          if (Parser.FindEntryText((char *)"dir",DirTextParameter))
          {
            Direction = (DirTextParameter.GetULong());
            if (Direction > 5) Direction = 0;
          }

          ReturnInfo = AvData->GetNeighborVoxel(Direction);
          RobotManager->Answer_OneParameter(Parser, Req, ReturnInfo);
        }

        if (Parser.Command=="get_invquantity")
        {

          UShort InvType, VoxelType;
          ZString InvTextParameter, VoxelTypeTextParameter, ReturnInfo;
          ULong Quantity,i;

          Quantity = 0;

          if (   Parser.FindEntryText((char *)"inv",InvTextParameter)
              && Parser.FindEntryText((char *)"voxeltype",VoxelTypeTextParameter) )
          {
            InvType = InvTextParameter.GetULong();
            VoxelType = VoxelTypeTextParameter.GetULong();


            switch(InvType)
            {
              case 6:
              default: for (i=0;i<ZVoxelExtension_ProgRobot_Web::Storage_NumSlots;i++)
                       {
                         if (Ext->VoxelType[i]==VoxelType) Quantity += Ext->VoxelQuantity[i];
                         printf("%d:%d=%d\n", i, Ext->VoxelType[i], Ext->VoxelQuantity[i]);
                       }
                       break;
            }

          }

          ReturnInfo = Quantity;
          RobotManager->Answer_OneParameter(Parser, Req, ReturnInfo);
        }




        if (Parser.Command=="do_push")
        {
          ZString DirTextParameter, VoxelTypeTextParameter;
          UShort PushDirection, VoxelType, GotVoxelType;
          ULong SlotNum, Quantity;

          Quantity = 1;

          if (   Parser.FindEntryText((char *)"dir",DirTextParameter)
              && Parser.FindEntryText((char *)"voxeltype",VoxelTypeTextParameter) )
          {
            PushDirection = DirTextParameter.GetULong();
            VoxelType = VoxelTypeTextParameter.GetULong();

            printf("Place Direction : %d\n", (ULong)PushDirection);
            printf("VoxelType=%d\n", VoxelType);

            if (PushDirection <= 3) PushDirection = (Direction + PushDirection) & 3;
            if (PushDirection>5) PushDirection = 0;

            if (Ext->FindSlot(VoxelType, SlotNum))
            {
              if (AvData->PushToNeighborVoxel_Try(PushDirection,VoxelType,Quantity))
              {
                Ext->UnstoreBlocks(SlotNum,1,&GotVoxelType);
                AvData->PushToNeighborVoxel_Do(PushDirection,VoxelType,Quantity);
              }
            }
          }

          RobotManager->Answer_NoParameters(Parser, Req);
        }

        if (Parser.Command=="do_pull")
        {
          ZString DirTextParameter;
          UShort PullDirection, VoxelType;
          ULong VoxelQuantity;

          if ( Parser.FindEntryText((char *)"dir",DirTextParameter) )

          {
            PullDirection = DirTextParameter.GetULong();

            if (PullDirection <= 3) PullDirection = (Direction + PullDirection) & 3;
            if (PullDirection>5) PullDirection = 0;

            VoxelQuantity = 1;

            if (AvData->PullFromNeighborVoxel_Try(PullDirection, VoxelType, VoxelQuantity))
            {
              if (1==Ext->StoreBlocks(VoxelType, VoxelQuantity))
                AvData->PullFromNeighborVoxel_Do(PullDirection, VoxelType, VoxelQuantity);
            }
          }

          RobotManager->Answer_NoParameters(Parser, Req);
        }


        if (Parser.Command=="do_voxeltype")
        {
          ZString VoxelTypeTextParameter;
          UShort VoxelType;

          if ( Parser.FindEntryText((char *)"voxeltype",VoxelTypeTextParameter) )
          {
            VoxelType = VoxelTypeTextParameter.GetULong();

            if (!VoxelTypeManager->VoxelTable[VoxelType]->Is_NoType)
            {
              Ext->BuildVoxelType = VoxelType;
            }
          }


          RobotManager->Answer_NoParameters(Parser, Req);
        }



  // Todo : Modifier les fonctions de rotation pour ne pas réinitialiser l'extension




      }
    }

    delete Req;
  }

}

