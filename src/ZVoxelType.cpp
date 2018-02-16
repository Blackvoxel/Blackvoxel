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
 * ZVoxelType.cpp
 *
 *  Created on: 18 févr. 2011
 *      Author: laurent
 */

#include "ZVoxelType.h"
#include <stdio.h>
#include "z/ZString.h"

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif

#ifndef Z_ZSTREAMS_FILE_H
#  include "z/ZStream_File.h"
#endif

#ifndef Z_ZFABMACHINEINFOS_H
#  include "ZFabMachineInfos.h"
#endif

#ifndef Z_ZTOOLSPOINTER_H
#  include "ZToolsPointer.h"
#endif






ZVoxelType::ZVoxelType(UShort VoxelType)
{
  VoxelTypeManager = 0;
  MainTexture = 0;
  Is_PlayerCanPassThrough = false;
  this->VoxelType = VoxelType;
  Draw_TransparentRendering = false;
  Draw_FullVoxelOpacity = true;
  DrawInfo = ZVOXEL_DRAWINFO_DRAWFULLVOXELOPACITY;
  Draw_LinearInterpolation = true;
  ExtensionType = 0;
  Is_VoxelExtension = false;
  Is_HasAllocatedMemoryExtension = false;
  MiningHardness = 1000.0;
  MiningType = 2;
  Is_NoType = false;
  Is_UserTypeTransformable = true;
  Is_Harming = false;
  FrictionCoef = 0.001;
  Grip_Horizontal = 0.9;
  Grip_Vertical = 0.0;
  Is_SpaceGripType = false;
  Is_KeepControlOnJumping = true;
  HarmingLifePointsPerSecond = 0.0;
  Is_Active = false;
  Is_CanBeReplacedBy_Water = false;
  Is_CanBeReplacedBy_GreenAcid = false;
  Is_CanBeReplacedBy_MustardGaz = false;
  Is_CombinableWith_GreenAcid = true;
  Is_CanTriggerBomb = false;
  Is_Liquid = false;
  Is_Gaz = false;
  Is_Interface_StoreBlock = false;
  Is_Interface_PushBlock = false;
  Is_Interface_PullBlock = false;
  Is_Interface_GetInfo = false;
  Is_Interface_SetInfo = false;
  Is_Interface_SetLocation = false;
  Is_Pumpable_ByPump_T1 = false;
  Is_Pumpable_ByPump_T2 = false;
  Is_Loadable_ByLoader_L1 = true;
  BvProp_MoveableByTreadmill = true;
  BvProp_FastMoving = false;
  Is_Rideable = false;
  Is_HasHelpingMessage = false;
  Is_Power = false;
  BvProp_CanBePickedUpByRobot = true;
  BvProp_XrRobotPickMinLevel = 1;
  BvProp_PrRobotReplaceMinLevel = 0;
  BvProp_PrRobotPickMinLevel = 0;
  BvProp_PrRobotMoveMinLevel = 0;
  BvProp_AtomicFireResistant = false;
  BvProp_EgmyT1Resistant = false;
  BvProp_AccelerateOnFall = false;
  BvProp_UseInventoryImage=false;
  LiquidDensity = 0.0;
  BlastResistance = 1;
  OpenGl_TextureRef = 0;
  GameEnv = 0;
  FabInfos = 0;
  Documentation_PageNum = 0;
}

ZVoxelType::~ZVoxelType()
{
  if (MainTexture) delete MainTexture;
  if (FabInfos) delete FabInfos;
}


Bool ZVoxelType::LoadTexture()
{
  ZBitmapImage * Image;

  // Get the right folder path

  ZString FileSpec,FileName;
  if (VoxelType<32768)
  {
    FileName << "voxeltexture_" << (ULong)VoxelType << ".bmp";
    FileSpec.AddToPath(COMPILEOPTION_DATAFILESPATH).AddToPath("VoxelTypes").AddToPath(FileName);
  }
  else
  {
    FileName << "voxeltexture_" << (ULong)(VoxelType - 32767) << ".bmp";
    if (COMPILEOPTION_USEHOMEDIRSTORAGE) { FileSpec = ZStream_File::Get_Directory_UserData();  }
    FileSpec.AddToPath(COMPILEOPTION_SAVEFOLDERNAME);
    FileSpec.AddToPath("UserTextures");
    FileSpec.AddToPath(FileName);
  }

//  if (VoxelType<32768) sprintf(Buffer, "VoxelTypes/voxeltexture_%u.bmp", VoxelType);
//  else                 sprintf(Buffer, "UserTextures/voxeltexture_%u.bmp", VoxelType-32767);

  Image = new ZBitmapImage();
  if (!Image->LoadBMP(FileSpec.String)) { delete Image; return(false); }

  // Low Res texture for little devices

  #if COMPILEOPTION_LOWRESTEXTURING>0
  if (Image->Width > 128) Image->ReduceSize();
  if (COMPILEOPTION_LOWRESTEXTURING>2) while(Image->Width > 128)Image->ReduceSize();
  if (COMPILEOPTION_LOWRESTEXTURING>3) while(Image->Width > 64) Image->ReduceSize();

  #endif
  MainTexture = Image;
  if (Image->BytesPerPixel !=4)
  {
    if (VoxelType<32768) printf("Warning : Image %d has no alpha channel, this can cause crashes.\n", VoxelType);
    else                 printf("Warning : User defined image %d has no alpha channel, this can cause crashes.\n", VoxelType - 32767);
  }
  return(true);
}

Bool ZVoxelType::LoadVoxelInformations()
{
  //char Buffer[1024];
  ZString File, Line, Token;

  // Get the right folder path

  ZString FileSpec,FileName;
  if (VoxelType<32768)
  {
    FileName << "voxelinfo_" << (ULong)VoxelType << ".txt";
    FileSpec.AddToPath(COMPILEOPTION_DATAFILESPATH).AddToPath("VoxelTypes").AddToPath("voxelinfo").AddToPath(FileName);

  }
  else
  {
    FileName << "voxelinfo_" << (ULong)(VoxelType - 32767) << ".txt";
    if (COMPILEOPTION_USEHOMEDIRSTORAGE) { FileSpec = ZStream_File::Get_Directory_UserData();  }
    FileSpec.AddToPath(COMPILEOPTION_SAVEFOLDERNAME);
    FileSpec.AddToPath("UserTextures").AddToPath("voxelinfo").AddToPath(FileName);
  }


  //if (VoxelType<32768) sprintf(Buffer, "VoxelTypes/voxelinfo/voxelinfo_%u.txt", VoxelType);
  //else                 sprintf(Buffer, "UserTextures/voxelinfo/voxelinfo_%u.txt", VoxelType-32767);

  if (!File.LoadFromFile(FileSpec.String)) return(false);

  DrawInfo = 0;
  while ( File.Split(0x0a, Line) )
  {
    Line.StripAll(0x0d);
    if (Line.Split('=', Token))
    {
      Token.StripAll(' ');
      // if (Token=="Draw_TransparentRendering") Draw_TransparentRendering = Line.GetULong()>0 ? true:false;
      // if (Token=="Draw_FullVoxelOpacity")     Draw_FullVoxelOpacity     = Line.GetULong()>0 ? true:false;

      if (Token=="Draw_TransparentRendering")
      {
        if (Line.GetULong()>0) {DrawInfo |= ZVOXEL_DRAWINFO_DRAWTRANSPARENTRENDERING; Draw_TransparentRendering = true;}
        else {DrawInfo &= 0xFF^ZVOXEL_DRAWINFO_DRAWTRANSPARENTRENDERING; Draw_TransparentRendering = false;}
      }
      if (Token=="Draw_FullVoxelOpacity")
      {
        if (Line.GetULong()>0) {DrawInfo |= ZVOXEL_DRAWINFO_DRAWFULLVOXELOPACITY; Draw_FullVoxelOpacity = true;}
        else                   {DrawInfo &= 0xFF^ZVOXEL_DRAWINFO_DRAWFULLVOXELOPACITY; Draw_FullVoxelOpacity = false;}
      }
      if (Token=="Draw_SpecialRender")        { if (Line.GetULong()>0) DrawInfo |= ZVOXEL_DRAWINFO_SPECIALRENDERING; else DrawInfo &= 0xFF^ZVOXEL_DRAWINFO_SPECIALRENDERING; }

      if (Token=="VoxelTypeName")             VoxelTypeName             = Line;

      if (Token == "ExtensionType")           { ExtensionType = Line.GetULong(); }
      if (Token == "MiningHardness")          { MiningHardness = (double)Line.GetULong(); }
      if (Token == "MiningType")              { MiningType     = Line.GetULong(); }
      if (Token == "BvProp_PlayerCanPassThrough") { Is_PlayerCanPassThrough = (Line.GetULong()!=0) ? true:false; }
      if (Token == "BvProp_Harming")              { Is_Harming              = (Line.GetULong()!=0) ? true:false; }
      if (Token == "BvProp_Active")               { Is_Active               = (Line.GetULong()!=0) ? true:false; }
      if (Token == "FrictionCoef")            { FrictionCoef            = Line.GetDouble(); if (FrictionCoef == 0.0)    FrictionCoef =1.0; }
      if (Token == "Grip_Horizontal")         { Grip_Horizontal         = Line.GetDouble(); if (Grip_Horizontal == 0.0) Grip_Horizontal =1.0; }
      if (Token == "Grip_Vertical")           { Grip_Vertical           = Line.GetDouble(); if (Grip_Vertical == 0.0)   Grip_Vertical =1.0; }
      if (Token == "BvProp_KeepControlOnJumping") { Is_KeepControlOnJumping = (Line.GetULong()!=0) ? true:false; }
      if (Token == "HarmingLifePointsPerSecond") { HarmingLifePointsPerSecond = Line.GetDouble(); }
      if (Token == "BvProp_CombinableWith_GreenAcid") { Is_CombinableWith_GreenAcid = (Line.GetULong()!=0) ? true:false; }
      if (Token == "BvProp_CanBeReplacedBy_MustardGaz") { Is_CanBeReplacedBy_MustardGaz = (Line.GetULong()!=0) ? true:false; }
      if (Token == "BvProp_CanTriggerBomb")       { Is_CanTriggerBomb = (Line.GetULong()!=0) ? true:false; }
      if (Token == "BlastResistance")         { BlastResistance = (Line.GetLong()); }
      if (Token == "BvProp_Liquid")               { Is_Liquid = (Line.GetULong()!=0) ? true:false; }
      if (Token == "LiquidDensity")           { LiquidDensity            = Line.GetDouble();  }
      if (Token == "BvProp_Gaz")                  { Is_Gaz = (Line.GetULong()!=0) ? true:false; }
      if (Token == "BvProp_Pumpable_ByPump_T1")   { Is_Pumpable_ByPump_T1 = (Line.GetULong()!=0) ? true:false; }
      if (Token == "BvProp_Pumpable_ByPump_T2")   { Is_Pumpable_ByPump_T2 = (Line.GetULong()!=0) ? true:false; }
      if (Token == "BvProp_Loadable_ByLoader_L1") { Is_Loadable_ByLoader_L1 = (Line.GetULong()!=0) ? true:false; }
      if (Token == "BvProp_FastMoving")           { BvProp_FastMoving = (Line.GetULong()!=0) ? true:false; }
      if (Token == "BvProp_CanBePickedUpByRobot") { BvProp_CanBePickedUpByRobot = (Line.GetULong()!=0) ? true:false; }
      if (Token == "BvProp_XrRobotPickMinLevel")  { BvProp_XrRobotPickMinLevel = (UByte)Line.GetULong(); }
      if (Token == "Documentation_PageNum")       { Documentation_PageNum      = Line.GetULong(); }
      if (Token == "BvProp_AtomicFireResistant")  { BvProp_AtomicFireResistant = (Line.GetULong()!=0) ? true:false; }
      if (Token == "BvProp_EgmyT1Resistant")      { BvProp_EgmyT1Resistant = (Line.GetULong()!=0) ? true:false; }
      if (Token == "BvProp_AccelerateOnFall")     { BvProp_AccelerateOnFall = (Line.GetULong()!=0) ? true:false; }
      if (Token == "BvProp_UseInventoryImage")    { BvProp_UseInventoryImage = (Line.GetULong()!=0) ? true:false; }
      if (Token == "Draw_LinearInterpolation")    { Draw_LinearInterpolation = (Line.GetULong()!=0) ? true:false; }


      // if (Token == "Is_Interface_StoreBlock") {Is_Interface_StoreBlock = (Line.GetULong()!=0) ? true:false; }

    }
  }
  return(true);
}

void ZVoxelType::DeleteVoxelExtension(ZMemSize VoxelExtension, bool IsUnloadingPhase)
{
  if (Is_HasAllocatedMemoryExtension && VoxelExtension) delete (ZVoxelExtension *)VoxelExtension;
}

bool ZVoxelType::Interface_SetPointingLocation(ZVoxelLocation * VLoc, ZToolsPointer * ToolsPointer, ULong Slot, ZVector3L * Location, ZString * OutMessage )
{
  switch(Slot)
  {
    case 0: ToolsPointer->StartLocation = *Location;
            *OutMessage = "STARTING LOCATION SET";
            break;
    case 1: ToolsPointer->StartLocation = 0;
            ToolsPointer->EndLocation = 0;
            *OutMessage = "LOCATION RESET";
            break;
    case 2: ToolsPointer->EndLocation = *Location;
            *OutMessage = "END LOCATION SET";
            break;
  }

  return(true);
}



