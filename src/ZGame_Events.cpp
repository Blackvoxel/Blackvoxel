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
 * ZGame_Events.cpp
 *
 *  Created on: 24 juin 2011
 *      Author: laurent
 */

#include <stdio.h>
#include "ZGame_Events.h"
#include "ZGame.h"
#include "ZActorPhysics.h"
#include "ZSettings_Hardware.h"
#include <GL/glew.h>

#ifndef Z_ZVOXELGFX_TREE_H
#  include "ZVoxelGfx_Tree.h"
#endif

#ifndef Z_ZRENDER_BASIC_H
#  include "ZRender_Basic.h"
#endif

#ifndef Z_ZWORLD_STAT_H
#  include "ZWorld_Stat.h"
#endif

#ifndef Z_VOXELEXTENSIONTYPE_VOXELFLUID_H
#  include "ZVoxelExtensionType_VoxelFluid.h"
#endif

#ifndef Z_ZOS_SPECIFIC_VIEWDOC_H
#  include "ZOs_Specific_ViewDoc.h"
#endif

Bool ZGame_Events::KeyDown( UShort KeySym )
{
  ZActor * Actor;

  Keyboard_Matrix[KeySym] = 1;
  Actor = GameEnv->PhysicEngine->GetSelectedActor();

  switch(KeySym)
  {

    case SDLK_KP_DIVIDE:

    // Keyboard mouse emulation
/*
    case SDLK_c:  { Mouse_Matrix[1] = true; Actor->Action_MouseButtonClick(0); break; }
    case SDLK_v:  { Mouse_Matrix[2] = true; Actor->Action_MouseButtonClick(1); break; }
    case SDLK_b:  { Mouse_Matrix[3] = true; Actor->Action_MouseButtonClick(2); break; }
    case SDLK_x:  { Mouse_Matrix[4] = true; Actor->Action_MouseButtonClick(3); break; }
    case SDLK_n:  { Mouse_Matrix[5] = true; Actor->Action_MouseButtonClick(4); break; }
*/
    case SDLK_c:  { GameEnv->EventManager.ManualCall_MouseButtonClick(1); break; }
    case SDLK_v:  { GameEnv->EventManager.ManualCall_MouseButtonClick(2); break; }
    case SDLK_b:  { GameEnv->EventManager.ManualCall_MouseButtonClick(3); break; }
    case SDLK_x:  { GameEnv->EventManager.ManualCall_MouseButtonClick(4); break; }
    case SDLK_n:  { GameEnv->EventManager.ManualCall_MouseButtonClick(5); break; }



    case SDLK_f:  if (Keyboard_Matrix[SDLK_LSHIFT]) Actor->Action_PrevTool();
                  else                              Actor->Action_NextTool();
                  break;


    // This will Save/Load player position and state (Debug only).

    case SDLK_p:
                  if (COMPILEOPTION_DEBUGFACILITY)
                  {
                    if (Keyboard_Matrix[SDLK_LSHIFT])
                    {
                      ZStream_SpecialRamStream RamStream;
                      ZStream_File             OutStream;
                      ZString Filename, Filespec;

                      Filename << "Point_" << Actor->BuildingMaterial << ".dat";
                      Filespec = GameEnv->Path_ActualUniverse.Path(Filename);
                      OutStream.SetFileName(Filespec.String);
                      if (OutStream.OpenWrite())
                      {
                        RamStream.SetStream(&OutStream);
                        RamStream.OpenWrite();
                        Actor->Save(&RamStream);
                        RamStream.FlushBuffer();
                        RamStream.Close();
                      }
                      OutStream.Close();
                    }
                    else
                    {
                      ZStream_SpecialRamStream RamStream;
                      ZStream_File             InStream;
                      ZString Filename, Filespec;

                      Filename << "Point_" << Actor->BuildingMaterial << ".dat";
                      Filespec = GameEnv->Path_ActualUniverse.Path(Filename);
                      InStream.SetFileName(Filespec.String);
                      if (InStream.OpenRead())
                      {
                        RamStream.SetStream(&InStream);
                        RamStream.OpenRead();
                        Actor->Load(&RamStream);
                        RamStream.Close();
                      }
                      InStream.Close();
                    }
                  }
                  break;

    // J and K keys are used in school mode for getting "content pages".

    case SDLK_j:
    case SDLK_k:
                  {
                    if (!GameEnv->Settings_Hardware->Experimental_LearningMode) break;
                    ULong SlotNum = 20;
                    ZInventory * Inv = Actor->Inventory;
                    if (KeySym==SDLK_j) Actor->LearningModePage ++;
                    else                {if((Actor->LearningModePage--)==0) Actor->LearningModePage = 3; }

                    printf("Page : %d\n",Actor->LearningModePage);
                    switch (Actor->LearningModePage)
                    {
                      default: Actor->LearningModePage = 1;
                              // Robots and co...
                      case 1: Inv->SetSlot(SlotNum++, 108, 8192);
                              Inv->SetSlot(SlotNum++, 236, 8192);
                              Inv->SetSlot(SlotNum++, 49 , 8192);
                              Inv->SetSlot(SlotNum++, 214, 8192);
                              Inv->SetSlot(SlotNum++, 216, 8192);
                              Inv->SetSlot(SlotNum++, 208, 8192);
                              Inv->SetSlot(SlotNum++, 103, 8192);
                              Inv->SetSlot(SlotNum++, 104, 8192);
                              Inv->SetSlot(SlotNum++, 105, 8192);
                              Inv->SetSlot(SlotNum++, 106, 8192);

                              Inv->SetSlot(SlotNum++,  94, 8192);
                              Inv->SetSlot(SlotNum++, 95, 8192);
                              Inv->SetSlot(SlotNum++, 88, 8192);
                              Inv->SetSlot(SlotNum++, 87, 8192);
                              Inv->SetSlot(SlotNum++, 92, 8192);
                              Inv->SetSlot(SlotNum++,198, 8192);
                              Inv->SetSlot(SlotNum++,204, 8192);
                              Inv->SetSlot(SlotNum++,209, 8192);
                              Inv->SetSlot(SlotNum++,96, 8192);
                              Inv->SetSlot(SlotNum++,239, 8192);
//                            Inv->SetSlot(SlotNum++,0, 0);
                              break;
                      case 2:
                              Inv->SetSlot(SlotNum++, 90, 8192);
                              Inv->SetSlot(SlotNum++,99 , 8192);
                              Inv->SetSlot(SlotNum++,100, 8192);
                              Inv->SetSlot(SlotNum++,101, 8192);
                              Inv->SetSlot(SlotNum++,102, 8192);
                              Inv->SetSlot(SlotNum++,97,  8192);
                              Inv->SetSlot(SlotNum++,98,  8192);
                              Inv->SetSlot(SlotNum++,77 , 16);
                              Inv->SetSlot(SlotNum++,78 , 16);
                              Inv->SetSlot(SlotNum++,205, 8192);
                              Inv->SetSlot(SlotNum++,206, 8192);
                              Inv->SetSlot(SlotNum++,199, 8192);
                              Inv->SetSlot(SlotNum++,240, 8192);
                              Inv->SetSlot(SlotNum++,241, 8192);

                              while(SlotNum<40) Inv->SetSlot(SlotNum++, 0, 0);
                              break;
                      case 3: Inv->SetSlot(SlotNum++, 1, 8192);
                              Inv->SetSlot(SlotNum++, 2, 8192);
                              Inv->SetSlot(SlotNum++, 3, 8192);
                              Inv->SetSlot(SlotNum++, 4, 8192);
                              Inv->SetSlot(SlotNum++, 5, 8192);
                              Inv->SetSlot(SlotNum++, 6, 8192);
                              Inv->SetSlot(SlotNum++, 7, 8192);
                              Inv->SetSlot(SlotNum++, 8, 8192);
                              Inv->SetSlot(SlotNum++, 9, 8192);
                              Inv->SetSlot(SlotNum++,10, 8192);
                              while(SlotNum<40) Inv->SetSlot(SlotNum++, 0, 0);
                              break;
                    }
                    break;

                  }

    // Programmable robot kill switch...

    case SDLK_u:  GameEnv->Stop_Programmable_Robots = true; break;

    case SDLK_F1:
                  {
                    ZVoxelLocation Loc;
                    Long x,y,z;
                    ZActor_Player * Player;

                    SDL_WM_GrabInput(SDL_GRAB_OFF); SDL_ShowCursor(SDL_ENABLE);

                    Player = (ZActor_Player *)GameEnv->PhysicEngine->GetSelectedActor();
                    x = Player->PointedVoxel.PointedVoxel.x;
                    y = Player->PointedVoxel.PointedVoxel.y;
                    z = Player->PointedVoxel.PointedVoxel.z;

                    if (GameEnv->World->GetVoxelLocation(&Loc, x,y,z))
                    {
                      ZViewDoc::ViewDocPage(GameEnv->VoxelTypeManager.VoxelTable[ Loc.Sector->Data[Loc.Offset] ]->Documentation_PageNum,false);
                    }
                    else ZViewDoc::ViewDocPage(0,false);

                    ZString Message;
                    Message = "Click to Resume";
                    GameEnv->GameWindow_ResumeRequest->SetGameEnv(GameEnv);
                    GameEnv->GameWindow_ResumeRequest->SetMessage((char *)"CLICK TO RESUME GAME");
                    GameEnv->GameWindow_ResumeRequest->Show();
                  }

                  break;

    // THE "FEAR KEY" : Always the best way to destroy your world...
    case SDLK_F10:
                  if ( Keyboard_Matrix[SDLK_LSHIFT])
                  {
                    if (!GameEnv->GameEventSequencer->SlotIsEventAttached(1))
                    {
                      GameEnv->GameWindow_Advertising->Advertise("YOU PRESSED THE FEAR KEY", ZGameWindow_Advertising::VISIBILITY_HIGH,0,3000,3000);
                      GameEnv->GameWindow_Advertising->Advertise("SOME VOXELS WILL COME IN 1 MINUTES", ZGameWindow_Advertising::VISIBILITY_MEDIUM,0,3000,3000);
                      GameEnv->GameWindow_Advertising->Advertise("THEY MIGHT APPRECIATE YOU", ZGameWindow_Advertising::VISIBILITY_MEDIUM,0,3000,3000);
                      GameEnv->GameEventSequencer->AddEvent(60000,20000,1,false,0);
                    }
                  }
                  break;



  }
  return(true);
}

Bool ZGame_Events::KeyUp( UShort KeySym )
{
  //ZActor_Player * Actor;

  Keyboard_Matrix[KeySym] = 0;
  //Actor = (ZActor_Player *)GameEnv->PhysicEngine->GetSelectedActor();

  switch(KeySym)
  {
    /*
    case SDLK_c:  { Mouse_Matrix[1] = false; Actor->Action_MouseButtonRelease(0); break; }
    case SDLK_v:  { Mouse_Matrix[2] = false; Actor->Action_MouseButtonRelease(1); break; }
    case SDLK_b:  { Mouse_Matrix[3] = false; Actor->Action_MouseButtonRelease(2); break; }
    case SDLK_x:  { Mouse_Matrix[4] = false; Actor->Action_MouseButtonRelease(3); break; }
    case SDLK_n:  { Mouse_Matrix[5] = false; Actor->Action_MouseButtonRelease(4); break; }*/
    case SDLK_c:  { GameEnv->EventManager.ManualCall_MouseButtonRelease(1); break; }
    case SDLK_v:  { GameEnv->EventManager.ManualCall_MouseButtonRelease(2); break; }
    case SDLK_b:  { GameEnv->EventManager.ManualCall_MouseButtonRelease(3); break; }
    case SDLK_x:  { GameEnv->EventManager.ManualCall_MouseButtonRelease(4); break; }
    case SDLK_n:  { GameEnv->EventManager.ManualCall_MouseButtonRelease(5); break; }
    case SDLK_u:  { GameEnv->Stop_Programmable_Robots = false; break; }

  }
  return(true);
}

Bool ZGame_Events::MouseMove ( Short Relative_x, Short Relative_y, UShort Absolute_x, UShort Absolute_y)
{
  ZActor * Actor;

  if (EnableMouseEvents)
  {
    Actor = GameEnv->PhysicEngine->GetSelectedActor();
    if (Actor)
    {
      Actor->Action_MouseMove( Relative_x, Relative_y);
    }
  }
  return(true);
}

Bool ZGame_Events::MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y)
{
  ZActor * Actor;

  Mouse_Matrix[nButton] = true;
  if (EnableMouseEvents)
  {
    Actor = GameEnv->PhysicEngine->GetSelectedActor();
    if (Actor)
    {
      Actor->Action_MouseButtonClick( nButton - 1);
    }
  }

  return(true);
}

Bool ZGame_Events::MouseButtonRelease(UShort nButton, Short Absolute_x, Short Absolute_y)
{
  ZActor * Actor;

  Mouse_Matrix[nButton] = false;

  if (EnableMouseEvents)
  {
    Actor = GameEnv->PhysicEngine->GetSelectedActor();
    if (Actor)
    {
      Actor->Action_MouseButtonRelease( nButton - 1);
    }
  }
  return(true);
}

void ZGame_Events::Process_StillEvents()
{
  ZActor * Actor;
  ZSettings_Hardware * Settings_Hardware;

  Settings_Hardware = GameEnv->Settings_Hardware;
  Actor = GameEnv->PhysicEngine->GetSelectedActor();

  // Process actor Still event

  if (Actor) if (! Actor->Action_StillEvents( Mouse_Matrix, Keyboard_Matrix)) return;

  // Process default still events

  if (Actor)
  {

    if ( Keyboard_Matrix[SDLK_LSHIFT] || Keyboard_Matrix[SDLK_CAPSLOCK] ){ Actor->Flag_ActivateAntiFall = true; }
    else                                                                 { Actor->Flag_ActivateAntiFall = false; }
    if ( Keyboard_Matrix[Settings_Hardware->Setting_Key_MoveLeft] )      { Actor->Action_GoLeftStraff(); }
    if ( Keyboard_Matrix[Settings_Hardware->Setting_Key_MoveRight] )     { Actor->Action_GoRightStraff(); }
    if ( Keyboard_Matrix[Settings_Hardware->Setting_Key_MoveForward] )   { Actor->Action_GoForward(); }
    if ( Keyboard_Matrix[Settings_Hardware->Setting_Key_MoveBackward] )  { Actor->Action_GoBackward(); }
    if ( Keyboard_Matrix[Settings_Hardware->Setting_Key_MoveUp]   && (COMPILEOPTION_DEBUGFACILITY || GameEnv->Settings_Hardware->Experimental_LearningMode))      { Actor->Action_GoUp(); }
    if ( Keyboard_Matrix[Settings_Hardware->Setting_Key_MoveDown] && (COMPILEOPTION_DEBUGFACILITY || GameEnv->Settings_Hardware->Experimental_LearningMode))      { Actor->Action_GoDown(); }
    if ( Keyboard_Matrix[Settings_Hardware->Setting_Key_Jump])           { Actor->Action_Jump();}
    if ( Keyboard_Matrix[SDLK_e])                                        { Actor->Action_GetInOutOfVehicle();}
    //if ( Keyboard_Matrix[SDLK_a] )                                     { Actor->Action_GoUp(GameEnv->Time_GameLoop  * 1.5); }
    if ( Keyboard_Matrix[SDLK_h] && COMPILEOPTION_DEBUGFACILITY )        { Actor->Action_GoFastForward(500.0); }
    if ( Keyboard_Matrix[SDLK_DELETE] && !COMPILEOPTION_DEBUGFACILITY ) { Actor->LifePoints = 0.0; }
    if ( Keyboard_Matrix[SDLK_KP0] && (COMPILEOPTION_DEBUGFACILITY || GameEnv->Settings_Hardware->Experimental_LearningMode))       { Actor->Action_SetActorMode(0);}
    /* if ( Keyboard_Matrix[SDLK_KP1] && COMPILEOPTION_DEBUGFACILITY)       { Actor->Action_SetActorMode(1);} */
    if ( Keyboard_Matrix[SDLK_KP2] && COMPILEOPTION_DEBUGFACILITY)       { Actor->Action_SetActorMode(2);}
    if ( Keyboard_Matrix[SDLK_KP3] && (COMPILEOPTION_DEBUGFACILITY || GameEnv->Settings_Hardware->Experimental_LearningMode))       { Actor->Action_SetActorMode(3);}
    if ( Keyboard_Matrix[SDLK_KP4] && COMPILEOPTION_DEBUGFACILITY)       { Actor->Action_SetActorMode(4);}
    if ( Keyboard_Matrix[SDLK_KP5] && COMPILEOPTION_DEBUGFACILITY)
    {
      if (!Keyboard_Matrix[SDLK_LSHIFT])
      {
        ZVector3d Position; Position = 0.0; Position.y = 2048000*1.0 ; Actor->SetPosition(Position); Actor->ViewDirection.pitch = 360.0 - 90.0; Actor->ViewDirection.roll = 0.0; Actor->ViewDirection.yaw = 0.0;
      }
      else
      {
        ZVector3d Position; Position = 0.0; Actor->SetPosition(Position); Actor->ViewDirection.pitch =0.0; Actor->ViewDirection.roll = 0.0; Actor->ViewDirection.yaw = 0.0;
      }
    }

    /*if ( Keyboard_Matrix[SDLK_KP8] && COMPILEOPTION_DEBUGFACILITY)       { ZVector3d Position; Position = 0.0; Position.y = 6000.0 * 256.0; Actor->SetPosition(Position); }*/

    if ( Keyboard_Matrix[SDLK_KP7] && COMPILEOPTION_DEBUGFACILITY)
    {
      //GameEnv->Sound->PlaySound(1);
      GameEnv->Sound->Start_PlaySound(1,false,0.125,0);
      Keyboard_Matrix[SDLK_KP7] = 0;
      printf("x,y,z : %lf,%lf,%lf\n",Actor->Location.x, Actor->Location.y, Actor->Location.z);

      {
        UShort VoxelType;
        ULong * Table,x,y,z,i, SectorCount, VoxelCount, NonVoidCount;

        Table = new ULong[65536];
        for (i=0;i<65536;i++) Table[i]=0;


        ZVoxelSector * Sector;
        Sector = GameEnv->World->SectorList;

        SectorCount = 0;
        VoxelCount = 0;
        NonVoidCount = 0;
        while (Sector)
        {
          for (z=0;z<ZVOXELBLOCSIZE_Z;z++)
            for (x=0;x<ZVOXELBLOCSIZE_X;x++)
              for (y=0;y<ZVOXELBLOCSIZE_Y;y++)
              {
                VoxelType = Sector->GetCube(x,y,z);
                if (VoxelType !=0) { Table[VoxelType]++; NonVoidCount ++;}
                VoxelCount++;
              }
          SectorCount++;
          Sector = Sector->GlobalList_Next;
        }

        printf("--------------------------------Stats-------------------------------\n");
        printf("Scanned sectors : %d\n",SectorCount);
        printf("Voxels per sector : %d\n", ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_Z);
        printf("Total Voxel Count : %d\n",VoxelCount);
        printf("Non empty : %d\n",NonVoidCount);
        double PercentOfVoxel;
        PercentOfVoxel = 100.0 / ((double)NonVoidCount);

        double Time_ToFindAFilon;
        for (i=0;i<65536;i++)
        {
          if (Table[i]>0)
          {
            Time_ToFindAFilon = ((double)NonVoidCount) / ((double)Table[i]) / 3.0 * 1.25 / 60.0;
            printf("[%d]=%d : %s (%lf %%) TMoy :%lf Min \n",i,Table[i], GameEnv->VoxelTypeManager.VoxelTable[i]->VoxelTypeName.String, ((double)Table[i]) * PercentOfVoxel, Time_ToFindAFilon);
          }
        }

        delete [] Table;

        ZWorld_Stat Stat;
        Stat.SetWorld(GameEnv->World);
        // Stat.Get_Stat_DistanceToMineForVoxelType(74);

      }

    }



    if ( Keyboard_Matrix[SDLK_j] && COMPILEOPTION_DEBUGFACILITY && false)
    {
      Keyboard_Matrix[SDLK_j] = 0;

      ZActor * Actor;
      ZInventory * Inventory;
      ULong c=0;

      Actor = GameEnv->PhysicEngine->GetSelectedActor();
      if (Actor != 0)
      {
        Inventory = Actor->Inventory;
        if (Inventory!=0)
        {
          Inventory->SetSlot(c++,1,4096);  // 01
          Inventory->SetSlot(c++,2,4096);  // 02
          Inventory->SetSlot(c++,3,4096);  // 03
          Inventory->SetSlot(c++,4,4096);  // 04
          Inventory->SetSlot(c++,5,4096);  // 05
          Inventory->SetSlot(c++,6,4096);  // 06
          Inventory->SetSlot(c++,7,4096);  // 07
          Inventory->SetSlot(c++,8,4096);  // 08
          Inventory->SetSlot(c++,9,4096);  // 09
          Inventory->SetSlot(c++,10,4096); // 10
          Inventory->SetSlot(c++,11,4096); // 11
          Inventory->SetSlot(c++,12,4096); // 12
          //Inventory->SetSlot(c++,13,4096);
          Inventory->SetSlot(c++,14,4096); // 13
          // Inventory->SetSlot(c++,15,4096);
          Inventory->SetSlot(c++,16,4096); // 14
          Inventory->SetSlot(c++,17,4096); // 15
          Inventory->SetSlot(c++,18,4096); // 16
          Inventory->SetSlot(c++,19,4096); // 17
          Inventory->SetSlot(c++,38,4096); // 18
          Inventory->SetSlot(c++,39,4096); // 19
          Inventory->SetSlot(c++,20,4096); // 20
          Inventory->SetSlot(c++,21,4096); // 21
          Inventory->SetSlot(c++,22,4096); // 22
          Inventory->SetSlot(c++,80,4096); // 23
          Inventory->SetSlot(c++,81,4096); // 24
          Inventory->SetSlot(c++,82,4096); // 25
          // Inventory->SetSlot(c++,23,4096);
          // Inventory->SetSlot(c++,24,4096);
          // Inventory->SetSlot(c++,25,4096);
          Inventory->SetSlot(c++,26,4096); // 26
          Inventory->SetSlot(c++,27,4096); // 27
          Inventory->SetSlot(c++,28,4096); // 28
          // Inventory->SetSlot(c++,29,4096);
          Inventory->SetSlot(c++,30,4096); // 29
          Inventory->SetSlot(c++,31,4096); // 30
          Inventory->SetSlot(c++,32,4096); // 31
          Inventory->SetSlot(c++,33,4096); // 32
          Inventory->SetSlot(c++,34,4096); // 33
          Inventory->SetSlot(c++,35,4096); // 34
          Inventory->SetSlot(c++,36,4096); // 35
          Inventory->SetSlot(c++,37,4096); // 36

          Inventory->SetSlot(c++,46,4096);   // 01
          Inventory->SetSlot(c++,47,4096);   // 02
          Inventory->SetSlot(c++,53,4096);   // 04
          //Inventory->SetSlot(c++,42,4096);

          Inventory->SetSlot(c++,44,4096); // 40
          //Inventory->SetSlot(c++,45,4096);


          c = 40;
          Inventory->SetSlot(c++,77,1);
          Inventory->SetSlot(c++,75,1);
          Inventory->SetSlot(c++,42,1);
          Inventory->SetSlot(c++,76,1);
          Inventory->SetSlot(c++,78,1);
          Inventory->SetSlot(c++,79,1);
          c = 50;
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);


        }

      }
    }

    /*
    if ( Keyboard_Matrix[SDLK_k] && COMPILEOPTION_DEBUGFACILITY && false)
    {
      Keyboard_Matrix[SDLK_k] = 0;

      ZActor * Actor;
      ZInventory * Inventory;
      ULong c=0;

      Actor = GameEnv->PhysicEngine->GetSelectedActor();
      if (Actor != 0)
      {
        Inventory = Actor->Inventory;
        if (Inventory!=0)
        {
          Inventory->SetSlot(c++,40,4096); // 37

          Inventory->SetSlot(c++,41,4096); // 38
          Inventory->SetSlot(c++,43,4096); // 39

          //Inventory->SetSlot(c++,48,4096);
          // Inventory->SetSlot(c++,50,4096);
          // Inventory->SetSlot(c++,51,4096);
          Inventory->SetSlot(c++,52,4096);   // 03

          Inventory->SetSlot(c++,54,4096);   // 05
          Inventory->SetSlot(c++,55,4096);   // 06
          //Inventory->SetSlot(c++,56,4096);
          //Inventory->SetSlot(c++,57,4096);
          //Inventory->SetSlot(c++,58,4096);
          //Inventory->SetSlot(c++,59,4096);
          Inventory->SetSlot(c++,60,4096);   // 07
          Inventory->SetSlot(c++,61,4096);   // 08
          Inventory->SetSlot(c++,62,4096);   // 09
          Inventory->SetSlot(c++,63,4096);   // 10
          Inventory->SetSlot(c++,64,4096);   // 11
          //Inventory->SetSlot(c++,65,4096);
          Inventory->SetSlot(c++,66,4096);   // 12
          Inventory->SetSlot(c++,67,4096);   // 13
          Inventory->SetSlot(c++,68,4096);   // 14
          Inventory->SetSlot(c++,69,4096);   // 15
          Inventory->SetSlot(c++,70,4096);   // 16
          // Inventory->SetSlot(c++,71,4096);
          Inventory->SetSlot(c++,72,4096);   // 17
          //Inventory->SetSlot(c++,73,4096);
          Inventory->SetSlot(c++,74,4096);   // 18
          // Inventory->SetSlot(c++,75,4096);
          // Inventory->SetSlot(c++,76,4096);
          // Inventory->SetSlot(c++,77,4096);
          // Inventory->SetSlot(c++,78,4096);
          // Inventory->SetSlot(c++,79,4096);

          Inventory->SetSlot(c++,83,4096);   // 19

          // Inventory->SetSlot(c++,86,4096);
          Inventory->SetSlot(c++,109,4096);
          Inventory->SetSlot(c++,110,4096);
          Inventory->SetSlot(c++,111,4096);
          Inventory->SetSlot(c++,112,4096);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          c = 40;
          Inventory->SetSlot(c++,77,1);
          Inventory->SetSlot(c++,75,1);
          Inventory->SetSlot(c++,42,1);
          Inventory->SetSlot(c++,76,1);
          Inventory->SetSlot(c++,78,1);
          Inventory->SetSlot(c++,79,1);
          c = 50;
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);

        }

      }
    }
    */

    /*
    if ( Keyboard_Matrix[SDLK_l] && COMPILEOPTION_DEBUGFACILITY)
    {
      Keyboard_Matrix[SDLK_l] = 0;

      ZActor * Actor;
      ZInventory * Inventory;
      ULong c=0;

      Actor = GameEnv->PhysicEngine->GetSelectedActor();
      if (Actor != 0)
      {
        Inventory = Actor->Inventory;
        if (Inventory!=0)
        {
          Inventory->SetSlot(c++,99,4096);
          Inventory->SetSlot(c++,100,4096);
          Inventory->SetSlot(c++,101,4096);
          Inventory->SetSlot(c++,102,4096);
          // Inventory->SetSlot(c++,89,4096);
          Inventory->SetSlot(c++,90,4096);
          Inventory->SetSlot(c++,52,4096);
          // Inventory->SetSlot(c++,91,4096);
          Inventory->SetSlot(c++,87,4096);
          Inventory->SetSlot(c++,92,4096);
          Inventory->SetSlot(c++,85,4096);
          Inventory->SetSlot(c++,88,4096);
          // Inventory->SetSlot(c++,93,4096);
          Inventory->SetSlot(c++,94,4096);
          Inventory->SetSlot(c++,95,4096);
          Inventory->SetSlot(c++,84,4096);
          Inventory->SetSlot(c++,97,4096);
          Inventory->SetSlot(c++,98,4096);
          Inventory->SetSlot(c++,49,4096);
          Inventory->SetSlot(c++,96,4096);
          Inventory->SetSlot(c++,108,4096);
          Inventory->SetSlot(c++,107,4096);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,103,4096);
          Inventory->SetSlot(c++,104,4096);
          Inventory->SetSlot(c++,105,4096);
          Inventory->SetSlot(c++,106,4096);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);

          c = 40;
          Inventory->SetSlot(c++,77,1);
          Inventory->SetSlot(c++,75,1);
          Inventory->SetSlot(c++,42,1);
          Inventory->SetSlot(c++,76,1);
          Inventory->SetSlot(c++,78,1);
          Inventory->SetSlot(c++,79,1);
          c = 50;
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);
          Inventory->SetSlot(c++,0,0);

        }

      }
    }
*/
    //if ( Keyboard_Matrix[SDLK_w] )                                       { Actor->Action_GoDown(GameEnv->Time_GameLoop  * 1.5); }
    if ( Keyboard_Matrix[SDLK_g] && COMPILEOPTION_DEBUGFACILITY )                                       { Actor->Velocity.y = 15000.0; }
    if ( Keyboard_Matrix[SDLK_KP_PLUS] && COMPILEOPTION_DEBUGFACILITY)
    {
      if ( Keyboard_Matrix[SDLK_LSHIFT] ) {for (ULong i=0;i<10;i++) Actor->Action_NextBuildingMaterial();}
      else                                Actor->Action_NextBuildingMaterial();
      Keyboard_Matrix[SDLK_KP_PLUS] = 0;
    }
    if ( Keyboard_Matrix[SDLK_KP_MINUS] && COMPILEOPTION_DEBUGFACILITY)
    {
      if ( Keyboard_Matrix[SDLK_LSHIFT] ) {for (ULong i=0;i<10;i++) Actor->Action_PrevBuildingMaterial();}
      Actor->Action_PrevBuildingMaterial();
      Keyboard_Matrix[SDLK_KP_MINUS] = 0;
    }
    if ( Keyboard_Matrix[SDLK_t] && COMPILEOPTION_DEBUGFACILITY )        { ZInventory::Entry * Entry = Actor->Inventory->GetSlotRef(Actor->Inventory->GetActualItemSlotNum()); Entry->VoxelType = Actor->BuildingMaterial; Entry->Quantity = 8192*1; }

    if ( Keyboard_Matrix[SDLK_F2] )
    {
      if (!SDL_WM_GrabInput(SDL_GRAB_QUERY))
      {
        SDL_WM_GrabInput(SDL_GRAB_ON); SDL_ShowCursor(SDL_DISABLE);
      }
      else
      {
        SDL_WM_GrabInput(SDL_GRAB_OFF); SDL_ShowCursor(SDL_ENABLE);
      }
      Keyboard_Matrix[SDLK_F2] = 0;
    }

    // if ( Keyboard_Matrix[SDLK_F3] && COMPILEOPTION_DEBUGFACILITY )   { SDL_WM_GrabInput(SDL_GRAB_OFF); SDL_ShowCursor(SDL_DISABLE); }
    if ( Keyboard_Matrix[SDLK_F5] )   { GameEnv->World->Save(); }
    if ( Keyboard_Matrix[SDLK_F4] && COMPILEOPTION_DEBUGFACILITY )
    {
      ZString Infos;
      Long x,y,z;
      ZActor_Player * Player;

      Player = (ZActor_Player *)GameEnv->PhysicEngine->GetSelectedActor();
      x = Player->PointedVoxel.PointedVoxel.x;
      y = Player->PointedVoxel.PointedVoxel.y;
      z = Player->PointedVoxel.PointedVoxel.z;

      GameEnv->PointList.AddPosition( x,y,z );
      printf("-----------------Highlighted Voxel Analysis-------------\n");
      printf("Voxel Location : %d,%d,%d\n", x, y, z);
      printf("VoxelType : %d\n",GameEnv->World->GetVoxel(x,y,z) );
      ZVoxelLocation Loc;
      if (GameEnv->World->GetVoxelLocation(&Loc, x,y,z))
      {
        printf("Voxel Name : %s\n", GameEnv->VoxelTypeManager.VoxelTable[ Loc.Sector->Data[Loc.Offset] ]->VoxelTypeName.String);
        GameEnv->VoxelTypeManager.VoxelTable[ Loc.Sector->Data[Loc.Offset] ]->GetBlockInformations( &Loc, Infos );
        printf("Sector Location : %d,%d,%d\n", Loc.Sector->Pos_x , Loc.Sector->Pos_y, Loc.Sector->Pos_z);
        printf("Zone Coords : %d,%d\n",(((Loc.Sector->Pos_x) >> 4) + 32 +1 ), (((Loc.Sector->Pos_z) >> 4) + 32 +1 ));
        printf("%s\n",Infos.String);
      }

      Keyboard_Matrix[SDLK_F4] = 0;
    }


    if ( Keyboard_Matrix[SDLK_F6] )
    {
      Keyboard_Matrix[SDLK_F6] = 0;
      GameEnv->Basic_Renderer->BvProp_DisplayCrossHair = ! GameEnv->Basic_Renderer->BvProp_DisplayCrossHair;
      GameEnv->Basic_Renderer->BvProp_DisplayVoxelSelector = GameEnv->Basic_Renderer->BvProp_DisplayCrossHair;

      /* GameEnv->World->Purge(89); */
    }
    /*
    if ( Keyboard_Matrix[SDLK_F9] && COMPILEOPTION_DEBUGFACILITY )   { / * GameEnv->World->Load();* / }
    */
    /*
    if ( Keyboard_Matrix[SDLK_F7] && COMPILEOPTION_DEBUGFACILITY )
    {
      Keyboard_Matrix[SDLK_F7]=0;

      GameEnv->World->Purge(85);


      if (0) // Test code bloc
      {
        GameEnv->VoxelTypeManager.DumpInfos();
        ZVoxelSector * Sector;
        bool AllSectorsAreHere;
        Long x,y,z;

        AllSectorsAreHere = true;
        for (x=-5;x<=5;x++)
          for (y=0;y<=2;y++)
            for (z=-5;z<=5;z++)
            {
              if ( 0==GameEnv->World->FindSector(x,y,z)) { AllSectorsAreHere = false; printf("Missing:%lx,%lx,%lx\n",(UNum)x,(UNum)y,(UNum)z); GameEnv->World->RequestSector(x,y,z,4); }
            }

        if (AllSectorsAreHere)
        {
          ZVoxelGfx_Tree TreeMaker;
          printf("Entering tree creation\n");
          / *
          ZVoxelSector * NewSector;
          NewSector=new ZVoxelSector(128,128,128);
          NewSector->SetVoxelTypeManager(GameEnv->World->VoxelTypeManager);
          ZRect3L Rect;
          ZRect1d Thickness;
          Thickness.Start = 1.0; Thickness.End = 1.0;
          for (Long i=0;i<128;i+=4) { Rect.sx = i; Rect.sy=2; Rect.sz=3; Rect.ex=i; Rect.ey=2; Rect.ez=125; NewSector->Draw_safe_VoxelLine(&Rect,&Thickness, 1); }

          ZVector3L Sp,Dp,Sz;
          Sp.x = 0; Sp.y = 0; Sp.z = 0;
          Dp.x = 0; Dp.y = 0; Dp.z = 0;
          Sz.x = 16; Sz.y = 64; Sz.z = 16;

          Sector->Draw_safe_3DBlit(NewSector, &Dp, &Sp, &Sz );

  * /
          for (x=-5;x<=5;x++)
            for (y=0;y<=2;y++)
              for (z=-5;z<=5;z++)
          {
            Sector = GameEnv->World->FindSector(x,y,z);
            ZVector3d Coords;
            // Sector->Draw_safe_Tree_Type_3(&Coords );
            Coords.x = (40 ) - (Sector->Pos_x * 16);  Coords.y = -Sector->Pos_y * 64;  Coords.z = (40) - (Sector->Pos_z * 16);
            Sector->Fill(0); TreeMaker.DrawTree(Sector, &Coords);
            TreeMaker.Seed ++;
            Coords.x = (110 ) - (Sector->Pos_x * 16);  Coords.y = -Sector->Pos_y * 64;  Coords.z = (80) - (Sector->Pos_z * 16);
            TreeMaker.DrawTree(Sector, &Coords);
            TreeMaker.Seed ++;
            Coords.x = (0 ) - (Sector->Pos_x * 16);  Coords.y = -Sector->Pos_y * 64;  Coords.z = (0) - (Sector->Pos_z * 16);
            TreeMaker.DrawTree(Sector, &Coords);
            TreeMaker.Seed -=2;
          }

          for (x=-5;x<=5;x++)
            for (y=0;y<=2;y++)
              for (z=-5;z<=5;z++)
          {
            GameEnv->World->SectorUpdateFaceCulling(x,y,z,false);
            Sector = GameEnv->World->FindSector(x,y,z);
            Sector->Flag_Render_Dirty = true;
            Sector->Flag_Void_Regular = true;
            Sector->Flag_Void_Transparent = true;
          }
          TreeMaker.Seed ++;
        }
      }
    }
*/
    if ( Keyboard_Matrix[SDLK_F8] && COMPILEOPTION_DEBUGFACILITY )
    {
      ZActor * Actor;
      ZVector3L Position;
      ZVector3L Sector;
      ZVector3L Zone;

      Actor = GameEnv->PhysicEngine->GetSelectedActor();

      Position.x = Actor->Location.x/256.0; Position.y = Actor->Location.y/256.0; Position.z = Actor->Location.z/256.0;
      Sector.x = Position.x >> 4; Sector.y = Position.y >> 6; Sector.z = Position.z >> 4;
      Zone.x = Position.x >> 8; Zone.y = 0; Zone.z = Position.z >> 8;

      printf("Pos (%ld,%ld,%ld) Sector (%ld,%ld,%ld) Zone (%ld,%ld,%ld)\n",(UNum)Position.x,(UNum)Position.y,(UNum)Position.z,(UNum)Sector.x,(UNum)Sector.y,(UNum)Sector.z,(UNum)Zone.x,(UNum)Zone.y,(UNum)Zone.z);
    }
    if ( Keyboard_Matrix[SDLK_ESCAPE] )  { SDL_WM_GrabInput(SDL_GRAB_OFF); SDL_ShowCursor(SDL_ENABLE); GameEnv->Game_Run = false; }

/*
    if ( Keyboard_Matrix[SDLK_F11] )
    {
      printf("Sectors in Memory:%lu ", (UNum)GameEnv->World->Info_GetSectorsInMemory()); //World.Info_PrintHashStats();
      // Needs GL_NVX_gpu_memory_info extension
      GLint Availlable_VRam = 0;
       glGetIntegerv(/ *GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX* / 0x9049, &Availlable_VRam);
      GLint Total_Memory = 0;
      glGetIntegerv(/ *GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX* / 0x9048,&Total_Memory);
      printf("GPU MEMORY (Ko) Free/Total/Used: %d/%d/%d\n",Availlable_VRam, Total_Memory, Total_Memory - Availlable_VRam  );
      Keyboard_Matrix[SDLK_F11]=0;
    }
*/


    if (Keyboard_Matrix[SDLK_i])
    {

      Keyboard_Matrix[SDLK_i] = 0;


      if      (GameEnv->GameWindow_Storage->Is_Shown())           { GameEnv->GameWindow_Storage->Hide();}
      else if (GameEnv->GameWindow_Programmable->Is_Shown())      { GameEnv->GameWindow_Programmable->Hide();}
      else if (GameEnv->GameWindow_ProgRobot_Asm->Is_Shown())     { GameEnv->GameWindow_ProgRobot_Asm->Hide();}
      else if (GameEnv->GameWindow_UserTextureTransformer->Is_Shown()) {GameEnv->GameWindow_UserTextureTransformer->Hide();}
      else if (GameEnv->GameWindow_Sequencer->Is_Shown())         { GameEnv->GameWindow_Sequencer->Hide();}
      else if (GameEnv->GameWindow_AsmDebug->Is_Shown())          { GameEnv->GameWindow_AsmDebug->Hide(); if (GameEnv->GameWindow_AsmHardware->Is_Shown()) GameEnv->GameWindow_AsmHardware->Hide(); }
      else
      {
        GameEnv->GameWindow_Inventory->SetGameEnv(GameEnv);
        if (GameEnv->GameWindow_Inventory->Is_Shown()) GameEnv->GameWindow_Inventory->Hide();
        else                                           GameEnv->GameWindow_Inventory->Show();
      }
    }

    if (Keyboard_Matrix[SDLK_F12])
    {
      Keyboard_Matrix[SDLK_F12] = 0;

      if (GameEnv->GameWindow_DisplayInfos->Is_Shown()) { GameEnv->GameWindow_DisplayInfos->Hide(); }
      else                                              { GameEnv->GameWindow_DisplayInfos->Show(); }
    }

/*
    if (Keyboard_Matrix[SDLK_o] && 0)
    {
      Keyboard_Matrix[SDLK_o] = 0;
      GameEnv->VoxelTypeBar->SetGameEnv(GameEnv);
      if (GameEnv->VoxelTypeBar->Is_Shown()) GameEnv->VoxelTypeBar->Hide();
      else                                   GameEnv->VoxelTypeBar->Show();
    }
*/
/*
    if (Keyboard_Matrix[SDLK_y] && 0)
    {
      Keyboard_Matrix[SDLK_y] = 0;

      //GameEnv->GameWindow_Advertising->SetCompletion(50.0f);
      GameEnv->GameWindow_Advertising->Advertise("Test 1",ZGameWindow_Advertising::VISIBILITY_HIGH,0,5000.0, 2000.0);
      GameEnv->GameWindow_Advertising->Advertise("C'est un texte completement loufoque pour tester le jeu et la largeur d'ecran excessive, je raconte ma vie, mais je n'aime pas les lorem ipsum",ZGameWindow_Advertising::VISIBILITY_HIGH,0,5000.0, 2000.0);
      GameEnv->GameWindow_Advertising->Advertise("Ceci est un affichage de test MEDIUM",ZGameWindow_Advertising::VISIBILITY_MEDIUM,0,5000.0, 2000.0);
      GameEnv->GameWindow_Advertising->Advertise("Ceci est un affichage de test MEDLOW",ZGameWindow_Advertising::VISIBILITY_MEDLOW,0,5000.0, 2000.0);
      GameEnv->GameWindow_Advertising->Advertise("Ceci est un affichage de test LOW",ZGameWindow_Advertising::VISIBILITY_LOW,0,5000.0, 2000.0);
      GameEnv->GameWindow_Advertising->Advertise("Ceci est un affichage de test VERYLOW",ZGameWindow_Advertising::VISIBILITY_VERYLOW,0,5000.0, 2000.0);


      GameEnv->GameWindow_Advertising->Advertise("Ceci est un affichage de test VISIBILITY_VERYHARDTOREAD",ZGameWindow_Advertising::VISIBILITY_VERYHARDTOREAD,0,5000.0, 2000.0);


      // if (GameEnv->GameWindow_Advertising->Is_Shown()) GameEnv->GameWindow_Advertising->Hide();
      // else                                   GameEnv->GameWindow_Advertising->Show();
    }
*/

    if (Keyboard_Matrix[SDLK_END] && COMPILEOPTION_DEBUGFACILITY)
    {
      Keyboard_Matrix[SDLK_END] = 0;
      GameEnv->Enable_MVI = false;
    }
    if (Keyboard_Matrix[SDLK_HOME] && COMPILEOPTION_DEBUGFACILITY)
    {
      Keyboard_Matrix[SDLK_HOME] = 0;
      GameEnv->Enable_MVI = true;
    }

    if (Keyboard_Matrix[SDLK_PAGEDOWN] && COMPILEOPTION_DEBUGFACILITY)
    {
      Keyboard_Matrix[SDLK_PAGEDOWN] = 0;
      GameEnv->Enable_NewSectorRendering = false;
    }
    if (Keyboard_Matrix[SDLK_PAGEUP] && COMPILEOPTION_DEBUGFACILITY)
    {
      Keyboard_Matrix[SDLK_PAGEUP] = 0;
      GameEnv->Enable_NewSectorRendering = true;
    }

    if (Keyboard_Matrix[SDLK_DELETE] && COMPILEOPTION_DEBUGFACILITY)
    {
      Keyboard_Matrix[SDLK_DELETE] = 0;
      GameEnv->Enable_LoadNewSector = false;
    }
    if (Keyboard_Matrix[SDLK_INSERT] && COMPILEOPTION_DEBUGFACILITY)
    {
      Keyboard_Matrix[SDLK_INSERT] = 0;
      GameEnv->Enable_LoadNewSector = true;
    }




    // Paste Clipboard
    if (!Keyboard_Matrix[SDLK_LSHIFT])
    {
      if (Keyboard_Matrix[SDLK_KP_MULTIPLY] && COMPILEOPTION_DEBUGFACILITY)
      {
        Keyboard_Matrix[SDLK_KP_MULTIPLY] = 0;

        if (GameEnv->PointList.GetPositionCount() >= 1)
        {
          ZVoxelSector Sector;
          ZVector3L Position;

          Position = *GameEnv->PointList.GetPosition(1);
          Position.y ++;
          Sector.SetNotStandardSize();
          Sector.SetVoxelTypeManager(&GameEnv->VoxelTypeManager);
          if (Sector.Load(0, "Clipboard.dat"))
          {
            GameEnv->World->BlitZoneCopy(&Sector, &Position, Keyboard_Matrix[SDLK_LSHIFT] ? true : false);
          }
        }
      }
    }
    else
    {
    // Test routine for sector blit functions.

      if (Keyboard_Matrix[SDLK_KP_MULTIPLY] && COMPILEOPTION_DEBUGFACILITY)
      {
        Keyboard_Matrix[SDLK_KP_MULTIPLY] = 0;

        if (GameEnv->PointList.GetPositionCount() >= 1)
        {
          ZVector3L Position, Offset;
          ZVoxelLocation Loc;

          ZVoxelSector Sector;
          Position = *GameEnv->PointList.GetPosition(1);
          Sector.SetNotStandardSize();
          Sector.SetVoxelTypeManager(&GameEnv->VoxelTypeManager);
          Sector.Load(0, "Clipboard.dat");
          Offset.x = Sector.Handle_x; Offset.y = Sector.Handle_y; Offset.z = Sector.Handle_z;

          //Offset = 0;

          GameEnv->World->GetVoxelLocation(&Loc, Position.x, Position.y, Position.z );
          Offset.x = (Position.x & 0xF) - Offset.x ;
          Offset.y = (Position.y & 0x3F) - Offset.y;
          Offset.z = (Position.z & 0xF) - Offset.z ;

          Loc.Sector->BlitSector(&Sector, &Offset );

          GameEnv->World->SectorUpdateFaceCulling(Loc.Sector->Pos_x, Loc.Sector->Pos_y, Loc.Sector->Pos_z , false);

          Loc.Sector->Flag_Render_Dirty = true;
          Loc.Sector->Flag_IsDebug = true;

        }

      }
    }

    // Copy clipboard

    // Start Point, End Point, Handle Point

    if (Keyboard_Matrix[SDLK_KP_DIVIDE] && COMPILEOPTION_DEBUGFACILITY)
    {


      Keyboard_Matrix[SDLK_KP_DIVIDE] = 0;

      if (GameEnv->PointList.GetPositionCount() >= 3)
      {
        ZVector3L Start, End, Offset;
        Long tmp;

        Start = *GameEnv->PointList.GetPosition(3);
        End   = *GameEnv->PointList.GetPosition(2);
        Offset = *GameEnv->PointList.GetPosition(1);

        if ( Start.x > End.x ) {tmp = Start.x; Start.x = End.x; End.x = tmp;}
        if ( Start.y > End.y ) {tmp = Start.y; Start.y = End.y; End.y = tmp;}
        if ( Start.z > End.z ) {tmp = Start.z; Start.z = End.z; End.z = tmp;}

        Start.x +=1; Start.z+=1;
        End.x -=1; End.z -=1;
        Offset -= Start;

        ZVoxelSector * Sector;
        Sector = GameEnv->World->GetZoneCopy(&Start, &End);
        Sector->SetHandle(Offset.x, Offset.y, Offset.z);
        Sector->SetVoxelTypeManager(&GameEnv->VoxelTypeManager);

        printf("Enregistrement de la zone (%d,%d,%d) à (%d,%d,%d) taille : (%d,%d,%d) Offset : (%d,%d,%d)\n",
                Start.x,Start.y,Start.z, End.x, End.y,End.z,
                Sector->Size_x, Sector->Size_y, Sector->Size_z,
                Sector->Handle_x, Sector->Handle_y, Sector->Handle_z);

        Sector->Save(0,"Clipboard.dat");
        if (Sector) delete Sector;
      }







      // DSector = new ZVoxelSector();
/*
      DSector = GameEnv->World->FindSector(0,0,0);
      DSector->Fill(22);
      GameEnv->World->SectorUpdateFaceCulling(0,0,0, false);
      DSector->Flag_Render_Dirty = true;
      DSector->Flag_Void_Regular = true;
      DSector->Flag_Void_Transparent = true;

      DSector = GameEnv->World->FindSector(1,0,0);
      DSector->Fill(21);
      GameEnv->World->SectorUpdateFaceCulling(1,0,0, false);
      DSector->Flag_Render_Dirty = true;
      DSector->Flag_Void_Regular = true;
      DSector->Flag_Void_Transparent = true;

      Offset = 0;
      DSector->SetVoxelTypeManager(&GameEnv->VoxelTypeManager);
      DSector->BlitSector(Sector, &Offset);

      GameEnv->World->SectorUpdateFaceCulling(0,0,0, false);
      DSector->Flag_Render_Dirty = true;
      DSector->Flag_Void_Regular = true;
      DSector->Flag_Void_Transparent = true;
*/



      /*
      DSector->Flag_Render_Dirty = true;
      DSector->Flag_NeedFullCulling = true;
      DSector->Flag_Void_Regular = false;
      DSector->Flag_Void_Transparent = false;
      */



    }

  }


}
