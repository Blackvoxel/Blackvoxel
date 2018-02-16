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
 * ZVoxelSector.cpp
 *
 *  Created on: 14 mars 2011
 *      Author: laurent
 */

#include "ZVoxelSector.h"
#include "ZVoxelType.h"
#include "ZVoxelTypeManager.h"
#include "z/ZStream_SpecialRamStream.h"
#include "z/ZStream_File.h"
#include <stdio.h>
#include <sys/stat.h>
#include <math.h>
#include <string.h>

#ifndef Z_ZVOXELEXTENSION_H
#  include "ZVoxelExtension.h"
#endif

#ifndef Z_ZMEMPOOL_H
#  include "z/ZMemPool.h"
#endif

#ifndef Z_ZFASTRANDOM_H
#  include "z/ZFastRandom.h"
#endif

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif


ULong  ZVoxelSector::SectorsInMemory = 0;




ZVoxelSector::ZVoxelSector() : ModifTracker(ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_Z)
{
  VoxelTypeManager = 0;
  Size_x = 16;
  Size_y = 64;
  Size_z = 16;
  Handle_x = Handle_y = Handle_z = 0;

  DataSize = Size_x * Size_y * Size_z;
  DisplayData = 0;
  Data        = new UShort[DataSize];
  FaceCulling = new UByte [DataSize];
  OtherInfos  = new ZMemSize[DataSize];
  TempInfos   = new UShort[DataSize];

  Next = 0;
  Pred = 0;
  GlobalList_Next = 0;
  GlobalList_Pred = 0;

  InitSector();


  SectorsInMemory++;
}

ZVoxelSector::ZVoxelSector( const ZVoxelSector &Sector)
{
  ZMemSize DataSize;

  DataSize = Sector.Size_x * Sector.Size_y * Sector.Size_z;

  Data        = new UShort[DataSize];
  FaceCulling = new UByte [DataSize];
  OtherInfos  = new ZMemSize[DataSize];
  TempInfos   = new UShort[DataSize];

  memcpy(Data, Sector.Data, DataSize << 1 );
  memcpy(FaceCulling, Sector.FaceCulling, DataSize);
  memcpy(OtherInfos, Sector.OtherInfos, DataSize * sizeof(ZMemSize));
  memcpy(TempInfos, Sector.TempInfos, DataSize << 2);

  VoxelTypeManager = 0;
  Next = Pred = GlobalList_Next = GlobalList_Pred = 0;
  Handle_x = Sector.Handle_x;
  Handle_y = Sector.Handle_y;
  Handle_z = Sector.Handle_z;
  Pos_x    = Sector.Pos_x;
  Pos_y    = Sector.Pos_y;
  Pos_z    = Sector.Pos_z;
  Size_x   = Sector.Size_x;
  Size_y   = Sector.Size_y;
  Size_z   = Sector.Size_z;

  Flag_Void_Regular             = Sector.Flag_Void_Regular;
  Flag_Void_Transparent         = Sector.Flag_Void_Transparent;
  Flag_Render_Dirty             = Sector.Flag_Render_Dirty;
  Flag_HighPriorityRefresh      = Sector.Flag_HighPriorityRefresh;
  Flag_IsVisibleAtLastRendering = Sector.Flag_IsVisibleAtLastRendering;
  Flag_DeletePending            = Sector.Flag_DeletePending;
  Flag_NeedFullCulling          = Sector.Flag_NeedFullCulling;
  Flag_KeepInMemory             = Sector.Flag_KeepInMemory;
  Flag_IsModified               = Sector.Flag_IsModified;
  Flag_IsSlowGeneration         = Sector.Flag_IsSlowGeneration;
  Flag_IsActiveVoxels           = Sector.Flag_IsActiveVoxels;
  Flag_IsActiveLowRefresh       = Sector.Flag_IsActiveLowRefresh;
  Flag_IsDebug                  = Sector.Flag_IsDebug;
  Flag_NotStandardSize          = Sector.Flag_NotStandardSize;
  Flag_NeedSortedRendering      = Sector.Flag_NeedSortedRendering;
  PartialCulling                = Sector.PartialCulling;

  this->DataSize                = Sector.DataSize;
  RefreshWaitCount              = Sector.RefreshWaitCount;
  LowRefresh_Mask               = Sector.LowRefresh_Mask;

}

ZVoxelSector::ZVoxelSector(Long Size_x, Long Size_y, Long Size_z)
{
  VoxelTypeManager = 0;
  this->Size_x = Size_x;
  this->Size_y = Size_y;
  this->Size_z = Size_z;
  Handle_x = Handle_y = Handle_z = 0;

  DataSize = Size_x * Size_y * Size_z;
  DisplayData = 0;
  if (DataSize>0)
  {
    Data        = new UShort[DataSize];
    FaceCulling = new UByte [DataSize];
    OtherInfos  = new ZMemSize [DataSize];
    TempInfos   = new UShort[DataSize];
  }
  else
  {
    Data        = 0;
    FaceCulling = 0;
    OtherInfos  = 0;
    TempInfos   = 0;
  }

  Next = 0;
  Pred = 0;
  GlobalList_Next = 0;
  GlobalList_Pred = 0;

  InitSector();

  SectorsInMemory++;
}

void ZVoxelSector::ChangeSize(Long Size_x, Long Size_y, Long Size_z)
{
  ULong i;

  if (Data)        {delete [] Data;        Data = 0;        }
  if (FaceCulling) {delete [] FaceCulling; FaceCulling = 0; }
  if (DisplayData) {delete DisplayData; DisplayData = 0;    }
  if (OtherInfos)  {delete [] OtherInfos;  OtherInfos  = 0; }
  if (TempInfos)   {delete [] TempInfos;   TempInfos   = 0; }

  this->Size_x = Size_x;
  this->Size_y = Size_y;
  this->Size_z = Size_z;
  Handle_x = Handle_y = Handle_z = 0;

  DataSize = Size_x * Size_y * Size_z;
  DisplayData = 0;

  Data        = new UShort[DataSize];
  FaceCulling = new UByte [DataSize];
  OtherInfos  = new ZMemSize [DataSize];
  TempInfos   = new UShort[DataSize];

  for(i=0;i<DataSize;i++) Data[i] = 0;
  for(i=0;i<DataSize;i++) FaceCulling[i] = 0;
  for(i=0;i<DataSize;i++) OtherInfos[i] = 0;
  for (i=0;i<DataSize;i++) TempInfos[i] = 273+20;
}

void ZVoxelSector::InitSector()
{
  ULong i;

  Pos_x=0;Pos_y=0;Pos_z=0;
  Handle_x = Handle_y = Handle_z = 0;
  ZoneType    = 0;
  ZoneVersion = 0;
  GeneratorVersion = 0;
  RingNum = 65535;

  for(i=0;i<DataSize;i++) OtherInfos[i] = 0;
  for (i=0;i<DataSize;i++) TempInfos[i] = 273+20;

  Flag_Render_Dirty = true;
  Flag_HighPriorityRefresh = false;
  Flag_Void_Regular = true;
  Flag_Void_Transparent = true;
  Flag_IsVisibleAtLastRendering = false;
  Flag_DeletePending = false;
  Flag_NeedFullCulling = false;
  Flag_KeepInMemory = false;
  Flag_IsModified = CHANGE_NONE;
  Flag_IsSlowGeneration = false;
  Flag_IsActiveVoxels = false;
  Flag_IsActiveLowRefresh = false;
  Flag_IsDebug = false;
  Flag_NotStandardSize = false;
  Flag_NeedSortedRendering = false;
  PartialCulling = 0;
  RefreshWaitCount = 0;
  LowRefresh_Mask = 0x0F;
}

void ZVoxelSector::CleanupSector()
{
  ULong i;
  ZMemSize Infos;

  for(i=0;i<DataSize;i++)
  {
    if ((Infos=OtherInfos[i]))
    {
      if (VoxelTypeManager->VoxelTable[ Data[i] ]->Is_HasAllocatedMemoryExtension )
      {
        VoxelTypeManager->VoxelTable[ Data[i] ]->DeleteVoxelExtension(Infos, true);
      }
      OtherInfos[i] = 0;
    }
  }
}; //


ZVoxelSector::~ZVoxelSector()
{
  ZMemSize i;

  // Delete allocated voxel extensions

  if (VoxelTypeManager)
  {
    for (i=0;i<DataSize;i++)
    {
      if (VoxelTypeManager->VoxelTable[ Data[i] ]->Is_HasAllocatedMemoryExtension)
      {
        VoxelTypeManager->VoxelTable[ Data[i] ]->DeleteVoxelExtension( OtherInfos[i], true );
        OtherInfos[i] = 0;
      }
    }
  }

  // Delete memory zones

  if (Data)        {delete [] Data;        Data = 0;        }
  if (FaceCulling) {delete [] FaceCulling; FaceCulling = 0; }
  if (DisplayData) {delete DisplayData; DisplayData = 0;    }
  if (OtherInfos)  {delete [] OtherInfos;  OtherInfos  = 0; }
  if (TempInfos)   {delete [] TempInfos;   TempInfos   = 0; }

  SectorsInMemory--;
}

Bool ZVoxelSector::GetSectorBaseDirectory(ZString & OutDirectory)
{
  if (COMPILEOPTION_USEHOMEDIRSTORAGE)
  {
    OutDirectory = ZStream_File::Get_Directory_UserData();
  }
  OutDirectory.AddToPath(COMPILEOPTION_SAVEFOLDERNAME);
  OutDirectory.AddToPath("Universes");

  return(true);
}


Bool ZVoxelSector::GetSectorFileName(ULong UniverseNum, Long Pos_x, Long Pos_y, Long Pos_z, ZString & BaseDirectory, char * OutBuffer)
{

  sprintf( OutBuffer,
             "%s/%lu/TL1_%ld_%ld_%ld/TL2_%ld_%ld_%ld/TL3_%ld_%ld_%ld/Sector_%ld_%ld_%ld.vox",
             BaseDirectory.String,
             (UNum)UniverseNum,
             (UNum)(Pos_x >> 12), (UNum)(Pos_y >> 12), (UNum)(Pos_z >> 12),
             (UNum)(Pos_x >> 8 ), (UNum)(Pos_y >> 8 ), (UNum)(Pos_z >> 8 ),
             (UNum)(Pos_x >> 4 ), (UNum)(Pos_y >> 4 ), (UNum)(Pos_z >> 4 ),
             (UNum)(Pos_x      ), (UNum)(Pos_y      ), (UNum)(Pos_z      )
           );

  return(true);
}

Bool ZVoxelSector::DeleteSave(ULong UniverseNum) // Delete file on disk.
{
  ZString Directory;
  char * FileName[FILENAME_MAX];

  GetSectorBaseDirectory(Directory);
  GetSectorFileName(UniverseNum, Pos_x, Pos_y, Pos_z, Directory, (char *)FileName);
  return(ZStream_File::Destroy_File((char *)FileName));
}

Bool ZVoxelSector::CreateSectorPathSubstructure(ZString & SectorSaveBaseDirectory, ULong UniverseNum, Long Pos_x, Long Pos_y, Long Pos_z)
{
  char DirName[FILENAME_MAX];

#ifdef ZENV_OS_LINUX
    sprintf( DirName, "%s/%lu", SectorSaveBaseDirectory.String, (UNum)UniverseNum);
    mkdir(DirName, S_IREAD | S_IWRITE | S_IEXEC | S_IRGRP | S_IWGRP | S_IXGRP);
    sprintf( DirName, "%s/%lu/TL1_%ld_%ld_%ld", SectorSaveBaseDirectory.String, (UNum)UniverseNum, (UNum)(Pos_x >> 12), (UNum)(Pos_y >> 12), (UNum)(Pos_z >> 12) );
    mkdir(DirName, S_IREAD | S_IWRITE | S_IEXEC | S_IRGRP | S_IWGRP | S_IXGRP);
    sprintf( DirName, "%s/%lu/TL1_%ld_%ld_%ld/TL2_%ld_%ld_%ld", SectorSaveBaseDirectory.String, (UNum)UniverseNum, (UNum)(Pos_x >> 12), (UNum)(Pos_y >> 12), (UNum)(Pos_z >> 12), (UNum)(Pos_x >> 8) , (UNum)(Pos_y >> 8), (UNum)(Pos_z >> 8) );
    mkdir(DirName, S_IREAD | S_IWRITE | S_IEXEC | S_IRGRP | S_IWGRP | S_IXGRP);
    sprintf( DirName, "%s/%lu/TL1_%ld_%ld_%ld/TL2_%ld_%ld_%ld/TL3_%ld_%ld_%ld", SectorSaveBaseDirectory.String, (UNum)UniverseNum, (UNum)(Pos_x >> 12), (UNum)(Pos_y >> 12), (UNum)(Pos_z >> 12), (UNum)(Pos_x >> 8) , (UNum)(Pos_y >> 8), (UNum)(Pos_z >> 8), (UNum)(Pos_x >> 4) , (UNum)(Pos_y >> 4), (UNum)(Pos_z >> 4) );
    mkdir(DirName, S_IREAD | S_IWRITE | S_IEXEC | S_IRGRP | S_IWGRP | S_IXGRP);
#endif
#ifdef ZENV_OS_WINDOWS
    ZStream_File OutStream;
    sprintf( DirName, "%s/%lu", SectorSaveBaseDirectory.String, UniverseNum);
    OutStream.Directory_Create(DirName);
    sprintf( DirName, "%s/%lu/TL1_%ld_%ld_%ld", SectorSaveBaseDirectory.String, UniverseNum, Pos_x >> 12, Pos_y >> 12, Pos_z >> 12 );
    OutStream.Directory_Create(DirName);
    sprintf( DirName, "%s/%lu/TL1_%ld_%ld_%ld/TL2_%ld_%ld_%ld", SectorSaveBaseDirectory.String, UniverseNum, Pos_x >> 12, Pos_y >> 12, Pos_z >> 12, Pos_x >> 8 , Pos_y >> 8,  Pos_z >> 8 );
    OutStream.Directory_Create(DirName);
    sprintf( DirName, "%s/%lu/TL1_%ld_%ld_%ld/TL2_%ld_%ld_%ld/TL3_%ld_%ld_%ld", SectorSaveBaseDirectory.String, UniverseNum, Pos_x >> 12, Pos_y >> 12, Pos_z >> 12, Pos_x >> 8 , Pos_y >> 8,  Pos_z >> 8, Pos_x >> 4 , Pos_y >> 4,  Pos_z >> 4 );
    OutStream.Directory_Create(DirName);
#endif
#ifdef ZENV_OS_OSX
    sprintf( DirName, "%s/%lu", SectorSaveBaseDirectory.String, (UNum)UniverseNum);
    mkdir(DirName, S_IREAD | S_IWRITE | S_IEXEC | S_IRGRP | S_IWGRP | S_IXGRP);
    sprintf( DirName, "%s/%lu/TL1_%ld_%ld_%ld", SectorSaveBaseDirectory.String, (UNum)UniverseNum, (UNum)(Pos_x >> 12), (UNum)(Pos_y >> 12), (UNum)(Pos_z >> 12) );
    mkdir(DirName, S_IREAD | S_IWRITE | S_IEXEC | S_IRGRP | S_IWGRP | S_IXGRP);
    sprintf( DirName, "%s/%lu/TL1_%ld_%ld_%ld/TL2_%ld_%ld_%ld", SectorSaveBaseDirectory.String, (UNum)UniverseNum, (UNum)(Pos_x >> 12), (UNum)(Pos_y >> 12), (UNum)(Pos_z >> 12), (UNum)(Pos_x >> 8) , (UNum)(Pos_y >> 8), (UNum)(Pos_z >> 8) );
    mkdir(DirName, S_IREAD | S_IWRITE | S_IEXEC | S_IRGRP | S_IWGRP | S_IXGRP);
    sprintf( DirName, "%s/%lu/TL1_%ld_%ld_%ld/TL2_%ld_%ld_%ld/TL3_%ld_%ld_%ld", SectorSaveBaseDirectory.String, (UNum)UniverseNum, (UNum)(Pos_x >> 12), (UNum)(Pos_y >> 12), (UNum)(Pos_z >> 12), (UNum)(Pos_x >> 8) , (UNum)(Pos_y >> 8), (UNum)(Pos_z >> 8), (UNum)(Pos_x >> 4) , (UNum)(Pos_y >> 4), (UNum)(Pos_z >> 4) );
    mkdir(DirName, S_IREAD | S_IWRITE | S_IEXEC | S_IRGRP | S_IWGRP | S_IXGRP);
#endif
    return(true);
}

Bool ZVoxelSector::Save(ULong UniverseNum, char const * OptFileName)
{

  ZStream_File             OutStream;
  ZStream_SpecialRamStream Rs;
  ULong * Size, StartLen, i;
  ZString Directory;

  ULong DataSize;

  // Make sector path and FileName

  if (!OptFileName)
  {
    char FileName[FILENAME_MAX];
    GetSectorBaseDirectory(Directory);
    GetSectorFileName(UniverseNum, Pos_x, Pos_y, Pos_z, Directory, FileName);
    OutStream.SetFileName(FileName);
  }
  else OutStream.SetFileName(OptFileName);

  // Open filename. If it doesn't work, try creating the right directory substructure.

  if (!OutStream.OpenWrite())
  {
    if (OptFileName) return(false);
    CreateSectorPathSubstructure(Directory, UniverseNum, Pos_x, Pos_y, Pos_z);
    if (!OutStream.OpenWrite()) return(false);
  }

  DataSize = Size_x * Size_y * Size_z;

  Rs.SetStream(&OutStream);

  Rs.OpenWrite();
  Rs.PutString("BLACKSEC");

  Rs.Put( (UShort)4); // Version
  Rs.Put( (UShort)4); // Compatibility Class;

  // Sector Informations

  Rs.PutString("SECTINFO");
  Size = Rs.GetPointer_ULong();
  Rs.Put(0xA600DBEDu);
  StartLen = Rs.GetActualBufferLen();
  Rs.Put((UShort)6); // Version
  Rs.Put((UShort)Size_x); Rs.Put((UShort)Size_y); Rs.Put((UShort)Size_z); // Voxel Sector Dimensions
  Rs.Put((Short)Handle_x); Rs.Put((Short)Handle_y); Rs.Put((Short)Handle_z);
  Rs.Put( (UByte)Flag_NeedFullCulling );
  Rs.Put( PartialCulling ); // Partial Culling data
  Rs.Put( (UByte)Flag_IsModified );
  Rs.Put( (UByte)Flag_IsSlowGeneration );
  Rs.Put( (UByte)Flag_IsActiveVoxels);       // V2
  Rs.Put( (UByte)Flag_IsActiveLowRefresh);   // V5
  Rs.Put( (ULong)LowRefresh_Mask );          // V5
  Rs.Put( (UShort)ZoneType );                // V6
  Rs.Put( (UShort)ZoneVersion );             // V6
  Rs.Put( (UShort)GeneratorVersion );        // V6
  Rs.Put( (UShort)RingNum);                  // V6
  Rs.Put( (UByte) Flag_NeedSortedRendering); // V6
  Rs.Put( (UByte) Flag_NotStandardSize);     // V6
  *Size = Rs.GetActualBufferLen() - StartLen;
  Rs.FlushBuffer();

  // Voxel Data

  Rs.PutString("VOXELDAT");
  Size = Rs.GetPointer_ULong();
  Rs.Put(0xA600DBEDu);
  StartLen = Rs.GetActualBufferLen();
  Rs.Put((UShort)1); // Version
  Compress_Short_RLE(Data, &Rs);
  *Size = Rs.GetActualBufferLen() - StartLen;
  Rs.FlushBuffer();

  // Face Culling Info

  Rs.PutString("FACECULL");
  Size = Rs.GetPointer_ULong();
  Rs.Put(0xA600DBEDu);
  StartLen = Rs.GetActualBufferLen();
  Rs.Put((UShort)1); // Version
  Compress_FaceCulling_RLE(FaceCulling, &Rs);
  *Size = Rs.GetActualBufferLen() - StartLen;
  Rs.FlushBuffer();

  // Other infos Data

  Rs.PutString("OTHERINF");
  Size = Rs.GetPointer_ULong();
  Rs.Put(0xA600DBEDu);
  StartLen = Rs.GetActualBufferLen();
  Rs.Put((UShort)1); // Version
  Compress_OtherInfos_RLE(OtherInfos, Data, &Rs);
  *Size = Rs.GetActualBufferLen() - StartLen;
  Rs.FlushBuffer();

  // Temperature infos

  Rs.PutString("TEMPDATA");
  Size = Rs.GetPointer_ULong();
  Rs.Put(0xA600DBEDu);
  StartLen = Rs.GetActualBufferLen();
  Rs.Put((UShort)1); // Version
  Compress_Temperatures_RLE(TempInfos, &Rs);
  *Size = Rs.GetActualBufferLen() - StartLen;
  Rs.FlushBuffer();

  // Voxel Extensions

  UShort Voxel;
  ZVoxelType * VoxelType;
  ULong  * ExtensionCount;
  ZVoxelExtension * VoxelExtension;

  Rs.PutString("VOXELEXT");
  Size = Rs.GetPointer_ULong();
  Rs.Put(0xA600DBEDu);
  StartLen = Rs.GetActualBufferLen();
  Rs.Put((UShort)1);
  ExtensionCount = Rs.GetPointer_ULong();
  Rs.Put(0u);

  for (i=0;i<DataSize;i++)
  {
    Voxel = Data[i];
    VoxelType = VoxelTypeManager->VoxelTable[Voxel];
    if (VoxelType->Is_HasAllocatedMemoryExtension)
    {
      (*ExtensionCount)++;
      Rs.Put(i); // Voxel Offset;
      VoxelExtension = (ZVoxelExtension *)OtherInfos[i];
      Rs.Put(VoxelExtension->GetExtensionID());

      VoxelExtension->Save(&Rs);
    }
  }
  *Size = Rs.GetActualBufferLen() - StartLen;
  Rs.FlushBuffer();






    //if (DataSize != fwrite(Data, sizeof(UShort),DataSize,fh)) {fclose (fh);return(false);}
    //if (DataSize != fwrite(Data, sizeof(UByte) ,DataSize,fh)) {fclose (fh);return(false);}
    //fclose (fh);

  OutStream.Close();

  return(true);
}

/*
Bool ZVoxelSector::Save(char const * FileName)
{
  FILE * fh;
  ULong DataSize;

  fh = fopen(FileName, "wb");

  DataSize = ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_Z;

  if (DataSize != fwrite(Data, sizeof(UShort),DataSize,fh)) {fclose (fh);return(false);}
  if (DataSize != fwrite(Data, sizeof(UByte) ,DataSize,fh)) {fclose (fh);return(false);}
  fclose (fh);

  return(true);
}
*/

Bool ZVoxelSector::Old_Load(ULong UniverseNum)
{
  char FileName[1024];
  FILE * fh;
  ULong DataSize;

  printf("Loading... %ld %ld %ld\n",(UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z);

  sprintf(FileName,"Universes/%lu/OldFormat/Sector_%ld_%ld_%ld.vox",(UNum)UniverseNum,(UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z);

  fh = fopen(FileName, "rb");

  if (!fh) return(false);



  DataSize = ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_Z;

  if (DataSize != fread(Data, sizeof(UShort),DataSize,fh)) {fclose (fh);return(false);}
  if (DataSize != fread(Data, sizeof(UByte) ,DataSize,fh)) {fclose (fh);return(false);}
  fclose (fh);

  return(true);
}

Bool ZVoxelSector::Load(ULong UniverseNum, char const * OptFileName)
{
  ZTestMemoryPool         MemPool;
  ZStream_File             InStream;
  ZStream_SpecialRamStream Rs;
  ZString String;
  //String.SetMemPool(&MemPool);
  ZString SectionName;
  //SectionName.SetMemPool(&MemPool);
  //InStream.FileName.SetMemPool(&MemPool);

  bool Ok;
  ZString Directory;
  ULong i,j;

  //for(i=0;i<DataSize;i++) Data[i]=0;

  if (OptFileName)
  {
    InStream.SetFileName(OptFileName);
  }
  else
  {
    char FileName[FILENAME_MAX];

    GetSectorBaseDirectory(Directory);
    GetSectorFileName(UniverseNum, Pos_x, Pos_y, Pos_z, Directory, FileName);
    InStream.SetFileName(FileName);
  }

  if (!InStream.OpenRead()) return(false);

  Rs.SetStream(&InStream);
  Rs.OpenRead();

  //
  // Sector Header
  UShort Version;
  UShort Compatibility_Class;

  String.SetLen(8);
  Rs.GetStringFixedLen(String.String,8);
  if (String!="BLACKSEC") { printf("Sector Loading Error (%ld,%ld,%ld): Header Missing, File is not a Sector regular format.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }
  Ok = Rs.Get(Version);
  Ok&= Rs.Get(Compatibility_Class);       if (!Ok) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read header informations.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }

  if (Compatibility_Class > 4 ) { printf("Sector Loading Error (%ld,%ld,%ld): Incompatible format version.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }

  // Sector Informations


  UShort  Section_Version;
  ULong   Section_Len;
  UShort  Size_x=0, Size_y=0, Size_z=0;
  UByte   Temp_Byte;
  //bool    SectInfoReaded;

  Section_Version = 0;
  Temp_Byte = 0;
  //SectInfoReaded = false;
  SectionName.SetLen(8);
  while (Rs.GetRemainBytesToRead() != 0)
  {
    if (!Rs.GetStringFixedLen(SectionName.String,8)) { printf("Sector Loading Error (%ld,%ld,%ld): Unable to read section name.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }

    if (SectionName=="SECTINFO")
    {
      Ok = Rs.Get(Section_Len);
      Ok&= Rs.Get(Section_Version);
      if (Section_Version >= 1)
      {
        Ok&= Rs.Get(Size_x);
        Ok&= Rs.Get(Size_y);
        Ok&= Rs.Get(Size_z);
        if ( (Size_x != this->Size_x) || (Size_y != this->Size_y) || (Size_z != this->Size_z)) ChangeSize(Size_x,Size_y,Size_z);
        if (Section_Version >= 4)
        {
          Ok&= Rs.Get(Handle_x);
          Ok&= Rs.Get(Handle_y);
          Ok&= Rs.Get(Handle_z);
        }
        Ok&= Rs.Get(Temp_Byte); Flag_NeedFullCulling = (Temp_Byte) ? true:false;
        Ok&= Rs.Get(Temp_Byte); PartialCulling = Temp_Byte;
      }
      if (Section_Version >= 2)
      {
        if (Section_Version >= 6) { Ok&= Rs.Get(Flag_IsModified); Flag_IsModified &= (0xFF ^ CHANGE_BITSECTORMODIFIED ); }
        else                      { Ok&= Rs.Get(Temp_Byte); Flag_IsModified = (Temp_Byte) ? CHANGE_IMPORTANT:CHANGE_NONE;}
        Ok&= Rs.Get(Temp_Byte); Flag_IsSlowGeneration   = (Temp_Byte) ? true:false;
      }
      if (Section_Version >= 3)
      {
        Ok&= Rs.Get(Temp_Byte); Flag_IsActiveVoxels = (Temp_Byte) ? true:false;
      }

      if (Section_Version >= 5)
      {
        Ok&= Rs.Get(Temp_Byte); Flag_IsActiveLowRefresh = (Temp_Byte) ? true:false;
        Ok&= Rs.Get(LowRefresh_Mask);
      }

      if (Section_Version >= 6)
      {
        Ok&=Rs.Get(ZoneType);
        Ok&=Rs.Get(ZoneVersion);
        Ok&=Rs.Get(GeneratorVersion);
        Ok&=Rs.Get(RingNum);
        Ok&= Rs.Get(Temp_Byte); Flag_NeedSortedRendering = (Temp_Byte) ? true:false;
        Ok&= Rs.Get(Temp_Byte); Flag_NotStandardSize     = (Temp_Byte) ? true:false;
      }
      //
      if (!Ok) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read SECTOR INFO section informations.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }

      if (!Flag_NotStandardSize) if ( (Size_x != ZVOXELBLOCSIZE_X) || (Size_y != ZVOXELBLOCSIZE_Y) || (Size_z != ZVOXELBLOCSIZE_Z) ) { printf("Sector Loading Error (%ld,%ld,%ld): Incompatible sector dimension.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }
      if (Section_Version>6) { printf("Sector Loading Error (%ld,%ld,%ld): Incompatible format in SECTOR INFO section.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }
    }
    else if (SectionName=="VOXELDAT")
    {
      Ok = Rs.Get(Section_Len);
      Ok&= Rs.Get(Section_Version);
      if (!Ok) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read VOXEL DATA section informations.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }
      if (!Decompress_Short_RLE(Data,&Rs)) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read and decompress VOXEL DATA section data.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }
    }
    else if (SectionName == "FACECULL")
    {
      Ok = Rs.Get(Section_Len);
      Ok&= Rs.Get(Section_Version);
      if (!Ok) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read VOXEL DATA section informations.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }
      if (!Decompress_FaceCulling_RLE(FaceCulling,&Rs)) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read and decompress FACE CULLING section data.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }
    }
    else if (SectionName == "OTHERINF")
    {
      Ok = Rs.Get(Section_Len);
      Ok&= Rs.Get(Section_Version);
      if (!Ok) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read VOXEL DATA section informations.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }
      if (!Decompress_OtherInfos_RLE(OtherInfos,&Rs)) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read and decompress FACE CULLING section data.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }
    }
    else if (SectionName == "TEMPDATA")
    {
      Ok = Rs.Get(Section_Len);
      Ok&= Rs.Get(Section_Version);
      if (!Ok) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read VOXEL DATA section informations.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }
      if (!Decompress_Temperatures_RLE(TempInfos,&Rs)) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read and decompress FACE CULLING section data.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }
    }
    else if (SectionName =="VOXELEXT")
    {
      // Voxel Extensions

      UShort Voxel;
      ZVoxelType * VoxelType;
      ZVoxelExtension * VoxelExtension;
      ULong RemainingExtensionsInFile;
      ULong VoxelOffset,ExtensionID, ExtensionLen;
      bool  PassExtension;
      RemainingExtensionsInFile = 0;

      Ok = Rs.Get(Section_Len);
      Ok&= Rs.Get(Section_Version);
      Ok&= Rs.Get(RemainingExtensionsInFile);


      if (!Ok) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read VOXEL EXTENSION section informations.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }

      for (i=0;i<DataSize;i++)
      {
        Voxel = Data[i];
        VoxelType = VoxelTypeManager->VoxelTable[Voxel];
        if (VoxelType->Is_HasAllocatedMemoryExtension)
        {
          VoxelExtension = (ZVoxelExtension *)VoxelType->CreateVoxelExtension(true);
          if (!VoxelExtension) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read VOXEL EXTENSION section / Can't create voxel extension.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }

          do
          {
            PassExtension = false;
            Ok = Rs.Get(VoxelOffset);    if (!Ok) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read VOXEL EXTENSION section / VoxelOffset.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }
            if (!RemainingExtensionsInFile) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read VOXEL EXTENSION section / Missing voxel extensions in file.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }
            Ok = Rs.Get(ExtensionID);    if (!Ok) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read VOXEL EXTENSION section / ExtensionID.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }

            // If there is unsupported extensions in file, read them and throw there contents out.
            if (VoxelOffset!=i)
            {
              PassExtension = true; // After throwing extension, we must try next.
              Ok = Rs.Get(ExtensionLen);    if (!Ok) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read VOXEL EXTENSION section / ExtensionLen.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }
              for (j=0;j<ExtensionLen;j++) Ok&=Rs.Get(Temp_Byte);
              if (!Ok) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read VOXEL EXTENSION section / Thrown extension data.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }

            }
            else if (ExtensionID != VoxelExtension->GetExtensionID()) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read VOXEL EXTENSION section / Extension type doesn't match.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }
            RemainingExtensionsInFile--;
          } while (PassExtension);

          OtherInfos[i] = (ZMemSize)VoxelExtension;
          if (!VoxelExtension->Load(&Rs)) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read VOXEL EXTENSION section / Can't read voxel extension subdata.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }
        }
      }
      // End of Section
    }
    else // Unknow Section
    {
      Ok = Rs.Get(Section_Len);
      if (!Ok) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read UNKNOWN section informations.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }
      for (i=0; i<Section_Len;i++) if (!Rs.Get(Temp_Byte)) { printf("Sector Loading Error (%ld,%ld,%ld): Can't read UNKNOWN section data.\n", (UNum)Pos_x,(UNum)Pos_y,(UNum)Pos_z); Rs.Close(); InStream.Close(); return(false); }
    }
  }

  // Voxel Data

  /*

  Rs.PutString("VOXELDAT");
  Size = Rs.GetPointer_ULong();
  Rs.Put(0xA600DBEDUL);
  StartLen = Rs.GetActualBufferLen();
  Rs.Put((UShort)1); // Version
  Compress_Short_RLE(Data, &Rs);
  *Size = Rs.GetActualBufferLen() - StartLen;
  Rs.FlushBuffer();

  // Face Culling Info

  Rs.PutString("FACECULL");
  Size = Rs.GetPointer_ULong();
  Rs.Put(0xA600DBEDUL);
  StartLen = Rs.GetActualBufferLen();
  Rs.Put((UShort)1); // Version
  Compress_FaceCulling_RLE(FaceCulling, &Rs);
  *Size = Rs.GetActualBufferLen() - StartLen;
  Rs.FlushBuffer();

  // Other infos Data

  Rs.PutString("OTHERINF");
  Size = Rs.GetPointer_ULong();
  Rs.Put(0xA600DBEDUL);
  StartLen = Rs.GetActualBufferLen();
  Rs.Put((UShort)1); // Version
  Compress_OtherInfos_RLE(OtherInfos, &Rs);
  *Size = Rs.GetActualBufferLen() - StartLen;
  Rs.FlushBuffer();

  // Temperature infos

  Rs.PutString("TEMPDATA");
  Size = Rs.GetPointer_ULong();
  Rs.Put(0xA600DBEDUL);
  StartLen = Rs.GetActualBufferLen();
  Rs.Put((UShort)1); // Version
  Compress_Temperatures_RLE(TempInfos, &Rs);
  *Size = Rs.GetActualBufferLen() - StartLen;
  Rs.FlushBuffer();

  }

    //if (DataSize != fwrite(Data, sizeof(UShort),DataSize,fh)) {fclose (fh);return(false);}
    //if (DataSize != fwrite(Data, sizeof(UByte) ,DataSize,fh)) {fclose (fh);return(false);}
    //fclose (fh);
*/
  InStream.Close();

  return(true);


}

/*
Bool ZVoxelSector::Load(char const * FileName)
{
  FILE * fh;
  ULong DataSize;

  fh = fopen(FileName, "rb");

  if (!fh) return(false);

  DataSize = ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_Z;

  if (DataSize != fread(Data, sizeof(UShort),DataSize,fh)) {fclose (fh);return(false);}
  if (DataSize != fread(Data, sizeof(UByte) ,DataSize,fh)) {fclose (fh);return(false);}
  fclose (fh);

  return(true);
}
*/

void ZVoxelSector::DebugOutFCInfo( const char * FileName )
{
  Long x,y,z;
  UByte Voxel;
  int Car;
  FILE * fp;

  fp = fopen(FileName, "wb");
  if (!fp) return;

  for ( y=0 ; y<Size_y ; y++ )
  {
    for ( z=0 ; z<Size_x ; z++ )
    {
      for ( x=0 ; x<Size_z ; x++ )
      {
        Voxel = this->FaceCulling[ y + ( x*Size_y )+ (z * (Size_y*Size_x)) ];
        Car = Voxel + 'A';
        fputc(Car, fp);
      }
      fputc(0x0d, fp);
      fputc(0x0a, fp);
    }
    fputc(0x0d, fp);
    fputc(0x0a, fp);
    fputc(0x0d, fp);
    fputc(0x0a, fp);
  }

  fclose (fp);
}

void ZVoxelSector::Compress_Short_RLE(UShort * Data, void * Stream)
{
  ZStream_SpecialRamStream * Rs = (ZStream_SpecialRamStream *)Stream;
  UShort MagicToken = 65535;
  UShort Last, Actual;
  ULong Point = 0;
  ULong SameCount = 0;
  ULong i;
  bool Continue;

  Last = Data[Point++];
  Continue = true;
  while (Continue)
  {
    if (Point != DataSize) Actual = Data[Point++];
    else                   {Actual = Last - 1; Continue = false; }
    if (Last == Actual)
    {
      SameCount ++;
    }
    else
    {
      if (SameCount)
      {
        if (SameCount < 3)
        {
          if   (Last == MagicToken) { Rs->Put(MagicToken); Rs->Put(MagicToken); Rs->Put((UShort)(SameCount+1)); }
          else                 { for (i=0;i<=SameCount;i++) Rs->Put(Last); }
        }
        else
        {
          Rs->Put(MagicToken);
          Rs->Put(Last);
          Rs->Put((UShort)(SameCount+1));
        }
        SameCount = 0;
      }
      else
      {
        if (Last == MagicToken) {Rs->Put(MagicToken); Rs->Put(Last); Rs->Put((UShort)1); }
        else               {Rs->Put(Last);}
      }
    }
    Last = Actual;
  }

}

void ZVoxelSector::Compress_OtherInfos_RLE(ZMemSize * Data, UShort * VoxelData, void * Stream)
{
  ZStream_SpecialRamStream * Rs = (ZStream_SpecialRamStream *)Stream;
  ULong MagicToken = 0xA600DBED;
  ULong Last, Actual;
  ULong Point = 0;
  ULong SameCount = 0;
  ULong i;
  bool Continue;

  Last = Data[Point];
  if (VoxelTypeManager->VoxelTable[VoxelData[Point++]]->Is_HasAllocatedMemoryExtension) Last = 0;

  Continue = true;
  while (Continue)
  {
    if (Point != DataSize)
    {
      Actual = OtherInfos[Point];
      if (VoxelTypeManager->VoxelTable[VoxelData[Point++]]->Is_HasAllocatedMemoryExtension) Actual = 0;
    }
    else                   {Actual = Last - 1; Continue = false; }
    if (Last == Actual)
    {
      SameCount ++;
    }
    else
    {
      if (SameCount)
      {
        if (SameCount < 3)
        {
          if   (Last == MagicToken) { Rs->Put(MagicToken); Rs->Put(MagicToken); Rs->Put((UShort)(SameCount+1)); }
          else                 { for (i=0;i<=SameCount;i++) Rs->Put(Last); }
        }
        else
        {
          Rs->Put(MagicToken);
          Rs->Put(Last);
          Rs->Put((UShort)(SameCount+1));
        }
        SameCount = 0;
      }
      else
      {
        if (Last == MagicToken) {Rs->Put(MagicToken); Rs->Put(Last); Rs->Put((UShort)1); }
        else               {Rs->Put(Last);}
      }
    }
    Last = Actual;
  }
}

void ZVoxelSector::Compress_FaceCulling_RLE(UByte * Data, void  * Stream)
{
  ZStream_SpecialRamStream * Rs = (ZStream_SpecialRamStream *)Stream;
  UByte MagicToken = 0xFF;
  UByte Last, Actual;
  ULong Point = 0;
  ULong SameCount = 0;
  ULong i;
  bool Continue;

  Last = OtherInfos[Point++];

  Continue = true;
  while (Continue)
  {
    if (Point != DataSize) {Actual = OtherInfos[Point++]; }
    else                   {Actual = Last - 1; Continue = false; }
    if (Last == Actual)
    {
      SameCount ++;
    }
    else
    {
      if (SameCount)
      {
        if (SameCount < 3)
        {
          if   (Last == MagicToken) { Rs->Put(MagicToken); Rs->Put(MagicToken); Rs->Put((UShort)(SameCount+1)); }
          else                 { for (i=0;i<=SameCount;i++) Rs->Put(Last); }
        }
        else
        {
          Rs->Put(MagicToken);
          Rs->Put(Last);
          Rs->Put((UShort)(SameCount+1));
        }
        SameCount = 0;
      }
      else
      {
        if (Last == MagicToken) {Rs->Put(MagicToken); Rs->Put(Last); Rs->Put((UShort)1); }
        else               {Rs->Put(Last);}
      }
    }
    Last = Actual;
  }
}

void ZVoxelSector::Compress_Temperatures_RLE(UShort * Data, void  * Stream)
{
  ZStream_SpecialRamStream * Rs = (ZStream_SpecialRamStream *)Stream;
  UShort MagicToken = 55871;
  UShort Last, Actual;
  ULong Point = 0;
  ULong SameCount = 0;
  ULong i;
  bool Continue;

  Last = Data[Point++];
  Continue = true;
  while (Continue)
  {
    if (Point != DataSize) Actual = Data[Point++];
    else                   {Actual = Last - 1; Continue = false; }
    if (Last == Actual)
    {
      SameCount ++;
    }
    else
    {
      if (SameCount)
      {
        if (SameCount < 3)
        {
          if   (Last == MagicToken) { Rs->Put(MagicToken); Rs->Put(MagicToken); Rs->Put((UShort)(SameCount+1)); }
          else                 { for (i=0;i<=SameCount;i++) Rs->Put(Last); }
        }
        else
        {
          Rs->Put(MagicToken);
          Rs->Put(Last);
          Rs->Put((UShort)(SameCount+1));
        }
        SameCount = 0;
      }
      else
      {
        if (Last == MagicToken) {Rs->Put(MagicToken); Rs->Put(Last); Rs->Put((UShort)1); }
        else               {Rs->Put(Last);}
      }
    }
    Last = Actual;
  }

}

bool ZVoxelSector::Decompress_Short_RLE(UShort * Data, void * Stream)
{
  ZStream_SpecialRamStream * Rs = (ZStream_SpecialRamStream *)Stream;
  UShort MagicToken = 0xFFFF;
  UShort Actual;
  ULong Pointer;
  UShort nRepeat;

  Pointer = 0;
  while (Pointer<DataSize)
  {
    if (!Rs->Get(Actual)) return(false);
    if (Actual == MagicToken)
    {
      if (!Rs->Get(Actual))  return(false);
      if (!Rs->Get(nRepeat)) return(false);
      if ( ((ULong)nRepeat) > (DataSize - Pointer))
      {
        return(false);
      }

      while (nRepeat--) {Data[Pointer++] = Actual;}
    }
    else
    {
      Data[Pointer++] = Actual;
    }
  }

  return(true);
}

bool ZVoxelSector::Decompress_FaceCulling_RLE(UByte * Data, void * Stream)
{
  ZStream_SpecialRamStream * Rs = (ZStream_SpecialRamStream *)Stream;
  UByte MagicToken = 0xFF;
  UByte Actual;
  ULong Pointer;
  UShort nRepeat;

  Pointer = 0;
  while (Pointer<DataSize)
  {
    if (!Rs->Get(Actual)) return(false);
    if (Actual == MagicToken)
    {
      if (!Rs->Get(Actual))  return(false);
      if (!Rs->Get(nRepeat)) return(false);
      if ( ((ULong)nRepeat) > (DataSize - Pointer))
      {
        return(false);
      }

      while (nRepeat--) {Data[Pointer++] = Actual;}
    }
    else
    {
      Data[Pointer++] = Actual;
    }
  }

  return(true);
}


bool ZVoxelSector::Decompress_OtherInfos_RLE(ZMemSize * Data, void * Stream)
{
  ZStream_SpecialRamStream * Rs = (ZStream_SpecialRamStream *)Stream;
  ULong MagicToken = 0xA600DBED;;
  ULong Actual;
  ULong Pointer;
  UShort nRepeat;

  Pointer = 0;
  while (Pointer<DataSize)
  {
    if (!Rs->Get(Actual)) return(false);
    if (Actual == MagicToken)
    {
      if (!Rs->Get(Actual))  return(false);
      if (!Rs->Get(nRepeat)) return(false);
      if ( ((ULong)nRepeat) > (DataSize - Pointer))
      {
        return(false);
      }

      while (nRepeat--) {Data[Pointer++] = Actual;}
    }
    else
    {
      Data[Pointer++] = Actual;
    }
  }

  return(true);
}

bool ZVoxelSector::Decompress_Temperatures_RLE(UShort * Data, void * Stream)
{
  ZStream_SpecialRamStream * Rs = (ZStream_SpecialRamStream *)Stream;
  UShort MagicToken = 55871;
  UShort Actual;
  ULong Pointer;
  UShort nRepeat;

  Pointer = 0;
  while (Pointer<DataSize)
  {
    if (!Rs->Get(Actual)) return(false);
    if (Actual == MagicToken)
    {
      if (!Rs->Get(Actual))  return(false);
      if (!Rs->Get(nRepeat)) return(false);
      if ( ((ULong)nRepeat) > (DataSize - Pointer))
      {
        return(false);
      }

      while (nRepeat--) {Data[Pointer++] = Actual;}
    }
    else
    {
      Data[Pointer++] = Actual;
    }
  }

  return(true);
}

void ZVoxelSector::Draw_safe_Sphere(double x, double y, double z, double Radius, UShort VoxelType, bool DrawIfVoid)
{
  Long sx,sy,sz;
  Long ex,ey,ez;
  Long nx,ny,nz;
  double r;
  double dx,dy,dz;

  if (Radius<0) Radius = -Radius;

  sx = floor(x-Radius); ex = ceil(x+Radius);
  sy = floor(y-Radius); ey = ceil(y+Radius);
  sz = floor(z-Radius); ez = ceil(z+Radius);

  if (sx<0) sx=0; if (sx>=Size_x) sx=Size_x-1;
  if (sy<0) sy=0; if (sy>=Size_y) sy=Size_y-1;
  if (sz<0) sz=0; if (sz>=Size_z) sz=Size_z-1;

  if (ex<0) ex=0; if (ex>=Size_x) ex=Size_x-1;
  if (ey<0) ey=0; if (ey>=Size_y) ey=Size_y-1;
  if (ez<0) ez=0; if (ez>=Size_z) ez=Size_z-1;

  for (nz=sz;nz<=ez;nz++)
    for (nx=sx;nx<=ex;nx++)
      for (ny=sy;ny<=ey;ny++)
      {
        dx = x - (double)nx;
        dy = y - (double)ny;
        dz = z - (double)nz;
        r = sqrt(dx*dx + dy*dy + dz*dz);
        if (r<=Radius) Draw_safe_SetVoxel(nx,ny,nz,VoxelType, DrawIfVoid);
      }

}

void ZVoxelSector::Draw_safe_SetVoxel(Long x, Long y, Long z, UShort VoxelType, bool DrawIfVoid)
{
  ZMemSize Pointer;

  UShort OldVoxel;

  if ( x >= Size_x || y >= Size_y || z >= Size_z || x<0 || y<0 || z<0 ) return;

  Pointer = y + x * Size_y + z * (Size_y * Size_x);

  // Si le Voxel Stocké dispose d'une extension, la libérer.

  OldVoxel = Data[Pointer];

  if ( OldVoxel && !DrawIfVoid ) return;
  if ( VoxelTypeManager->VoxelTable[OldVoxel]->Is_HasAllocatedMemoryExtension)
  {
    VoxelTypeManager->VoxelTable[Data[Pointer]]->DeleteVoxelExtension(OtherInfos[Pointer]);
    OtherInfos[Pointer] = 0;
  }

  // Stocke le voxel

  Data[Pointer] = VoxelType;

  // Si le voxel a stocker comporte une partie extension, la créer et l'enregistrer.

  if ( VoxelTypeManager->VoxelTable[VoxelType]->Is_HasAllocatedMemoryExtension)
  {
    OtherInfos[Pointer] = (ZMemSize)VoxelTypeManager->VoxelTable[VoxelType]->CreateVoxelExtension();
  }
}

void ZVoxelSector::Purge(UShort VoxelType)
{
  ULong i;
  UShort Voxel;

  for (i=0;i< (ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_Z); i++ )
  {
    Voxel = this->Data[i];
    if (Voxel == VoxelType)
    {
      if ( VoxelTypeManager->VoxelTable[Voxel]->Is_HasAllocatedMemoryExtension)
      {
        VoxelTypeManager->VoxelTable[Voxel]->DeleteVoxelExtension(OtherInfos[i]);
      }
      OtherInfos[i] = 0;
      Data[i]=0;
      TempInfos[i]=0;
    }
  }
}

void ZVoxelSector::Draw_safe_VoxelLine2( ZRect3L_2 * LineCoords, ZRect1d * Thickness, UShort VoxelType )
{
  Long Dx,Dy,Dz, TempMax, NumSteps, i;

  double x,y,z, Thick, Stepx,Stepy,Stepz,StepThick;

  Dx = LineCoords->ex - LineCoords->sx;
  Dy = LineCoords->ey - LineCoords->sy;
  Dz = LineCoords->ez - LineCoords->sz;

  TempMax = (fabs(Dx) > fabs(Dy)) ? fabs(Dx) : fabs(Dy);
  NumSteps = (TempMax > fabs(Dz)) ? TempMax : fabs(Dz);
  if (NumSteps<=0) Draw_safe_SetVoxel(LineCoords->sx, LineCoords->sy, LineCoords->sz, VoxelType);

  x = LineCoords->sx; y = LineCoords->sy; z = LineCoords->sz; Thick = Thickness->Start;
  Stepx = ((double)Dx) / NumSteps; Stepy = ((double)Dy) / NumSteps; Stepz = ((double)Dz) / NumSteps; StepThick = (Thickness->End - Thickness->Start) / NumSteps;

  for (i=0;i<=NumSteps;i++)
  {
    if (Thick == 0.5) Draw_safe_SetVoxel( floor(x+0.5),floor(y+0.5),floor(z+0.5), VoxelType );
    else              Draw_safe_Sphere( x+0.5,y+0.5,z+0.5, Thick, VoxelType );

    x+=Stepx;
    y+=Stepy;
    z+=Stepz;
    Thick+=StepThick;
  }
}

void ZVoxelSector::Draw_safe_VoxelLine( ZRect3L * LineCoords, ZRect1d * Thickness, UShort VoxelType )
{
  Long Dx,Dy,Dz, TempMax, NumSteps, i;

  double x,y,z, Thick, Stepx,Stepy,Stepz,StepThick;

  // Do not draw if line is entirely outside the sector.

  Long MaxThickness;
  Long ThickSize_x, ThickSize_y, ThickSize_z;
  MaxThickness = Thickness->End;
  if (Thickness->Start > MaxThickness) MaxThickness = Thickness->Start;
  MaxThickness += MaxThickness; // Convert Ray to diameter.
  ThickSize_x = Size_x + MaxThickness;
  ThickSize_y = Size_y + MaxThickness;
  ThickSize_z = Size_z + MaxThickness;

  MaxThickness = - MaxThickness; // We needs negative value for next tests;

  if (LineCoords->Start.x < MaxThickness && LineCoords->End.x < MaxThickness ) return;
  if (LineCoords->Start.x >= ThickSize_x && LineCoords->End.x >= ThickSize_x) return;
  if (LineCoords->Start.y < MaxThickness && LineCoords->End.y < MaxThickness ) return;
  if (LineCoords->Start.y >= ThickSize_y && LineCoords->End.y >= ThickSize_y) return;
  if (LineCoords->Start.z < MaxThickness && LineCoords->End.z < MaxThickness ) return;
  if (LineCoords->Start.z >= ThickSize_z && LineCoords->End.z >= ThickSize_z) return;

  //

  Dx = LineCoords->End.x - LineCoords->Start.x;
  Dy = LineCoords->End.y - LineCoords->Start.y;
  Dz = LineCoords->End.z - LineCoords->Start.z;

  TempMax = (fabs(Dx) > fabs(Dy)) ? fabs(Dx) : fabs(Dy);
  NumSteps = (TempMax > fabs(Dz)) ? TempMax : fabs(Dz);
  if (NumSteps<=0) Draw_safe_SetVoxel(LineCoords->Start.x, LineCoords->Start.y, LineCoords->Start.z, VoxelType);

  x = LineCoords->Start.x; y = LineCoords->Start.y; z = LineCoords->Start.z; Thick = Thickness->Start;
  Stepx = ((double)Dx) / NumSteps; Stepy = ((double)Dy) / NumSteps; Stepz = ((double)Dz) / NumSteps; StepThick = (Thickness->End - Thickness->Start) / NumSteps;

  for (i=0;i<=NumSteps;i++)
  {
    if (Thick == 0.5) Draw_safe_SetVoxel( floor(x+0.5),floor(y+0.5),floor(z+0.5), VoxelType );
    else              Draw_safe_Sphere( x+0.5,y+0.5,z+0.5, Thick, VoxelType );

    x+=Stepx;
    y+=Stepy;
    z+=Stepz;
    Thick+=StepThick;
  }
}

void ZVoxelSector::Draw_safe_VoxelLine_TickCtl( ZRect3L * LineCoords, double * ThicknessTable, ULong nThickIndices, UShort VoxelType )
{
  ULong Index1,Index2;
  Long Dx,Dy,Dz, TempMax, NumSteps,i;


  double x,y,z, ThickIndex, Thick, Thick1, Thick2, Stepx,Stepy,Stepz, StepThick, Temp, Modulator;

  if (nThickIndices < 2 || (ThicknessTable == 0)) return;

  Dx = LineCoords->End.x - LineCoords->Start.x;
  Dy = LineCoords->End.y - LineCoords->Start.y;
  Dz = LineCoords->End.z - LineCoords->Start.z;

  TempMax = (fabs(Dx) > fabs(Dy)) ? fabs(Dx) : fabs(Dy);
  NumSteps = (TempMax > fabs(Dz)) ? TempMax : fabs(Dz);
  if (NumSteps<=0) Draw_safe_SetVoxel(LineCoords->Start.x, LineCoords->Start.y, LineCoords->Start.z, VoxelType);

  x = LineCoords->Start.x; y = LineCoords->Start.y; z = LineCoords->Start.z;
  Stepx = ((double)Dx) / NumSteps; Stepy = ((double)Dy) / NumSteps; Stepz = ((double)Dz) / NumSteps;
  StepThick = ((double)(nThickIndices-1)) / (NumSteps) ; ThickIndex = 0;

  for (i=0;i<=NumSteps;i++)
  {
    Modulator = modf(ThickIndex, &Temp);
    Index1 = (ULong)floor(ThickIndex);
    Index2 = (ULong)ceil(ThickIndex);
    if (Index2>=nThickIndices) Index2 = nThickIndices-1;
    Thick1 = ThicknessTable[Index1];
    Thick2 = ThicknessTable[Index2];

    Thick = (Thick1 * (1.0 - Modulator)) + (Thick2 * Modulator);

    if (Thick <= 1.0) Draw_safe_SetVoxel( x+0.5,y+0.5,z+0.5, VoxelType );
    else              Draw_safe_Sphere( x+0.5,y+0.5,z+0.5, Thick, VoxelType );

    x+=Stepx;
    y+=Stepy;
    z+=Stepz;
    ThickIndex+=StepThick;
  }
}

void ZVoxelSector::Draw_safe_3DBlit(ZVoxelSector * SourceSector, ZVector3L * DestinationPoint, ZVector3L * SourcePoint, ZVector3L * Size)
{
  ZVector3L Dp;
  ZVector3L Sp;
  ZVector3L Sz;

  Dp.x = DestinationPoint->x; Dp.y = DestinationPoint->y; Dp.z = DestinationPoint->z;
  Sp.x = SourcePoint->x; Sp.y = SourcePoint->y; Sp.z = SourcePoint->z;
  Sz.x = Size->x; Sz.y = Size->y; Sz.z = Size->z;

  // Clipping du destination point englobant le volume.

  if (Dp.x < 0) { Sp.x -= Dp.x; Sz.x -= Dp.x; Dp.x = 0; }
  if (Dp.y < 0) { Sp.y -= Dp.y; Sz.y -= Dp.y; Dp.y = 0; }
  if (Dp.z < 0) { Sp.z -= Dp.z; Sz.z -= Dp.z; Dp.z = 0; }

  if (Dp.x > this->Size_x || Dp.y > this->Size_y || Dp.z > this->Size_z) return;
  if (Sp.x > SourceSector->Size_x || Sp.y > SourceSector->Size_y || Sp.z > SourceSector->Size_z) return;
  if (Sp.x < 0 || Sp.y < 0 || Sp.z <0 ) return;

  if ( (Sp.x+Sz.x) > SourceSector->Size_x ) Sz.x -= (Sp.x + Sz.x - SourceSector->Size_x);
  if ( (Sp.y+Sz.y) > SourceSector->Size_y ) Sz.y -= (Sp.y + Sz.y - SourceSector->Size_y);
  if ( (Sp.z+Sz.z) > SourceSector->Size_z ) Sz.z -= (Sp.z + Sz.z - SourceSector->Size_z);

  Long x,y,z;
  UShort Voxel;

  for (z=0;z<Sz.z;z++)
    for (x=0;x<Sz.x;x++)
      for (y=0;y<Sz.y;y++)
      {
        Voxel = SourceSector->GetCube(Sp.x+x,Sp.y+y,Sp.z+z);
        if (Voxel!=0) Draw_safe_SetVoxel(Dp.x+x,Dp.y+y,Dp.z+z,Voxel);
      }
}

void ZVoxelSector::Draw_subtree_1(ZVoxelSector * Sector, ZVector3d * Point, ZPolar3d * Direction, ZLightSpeedRandom * Random, double TotalLen)
{
   ZVector3d BranchVector, NewPoint, NewPoint2;
   ZPolar3d  NewDirection, NewDirection2;
   double angle,angle2;
   ZRect3L_2 Rect;
   ZRect1d Thickness;

   BranchVector.x = Direction->Len * sin(Direction->yaw / 57.295779506) * cos(Direction->pitch / 57.295779506);
   BranchVector.y = Direction->Len * sin(Direction->yaw / 57.295779506) * sin(Direction->pitch / 57.295779506);
   BranchVector.z = Direction->Len * cos(Direction->yaw / 57.295779506);

   NewPoint.x = Point->x + BranchVector.x;
   NewPoint.y = Point->y + BranchVector.y;
   NewPoint.z = Point->z + BranchVector.z;

   NewDirection.Len   = Direction->Len;
   NewDirection.pitch = Direction->pitch;
   NewDirection.roll  = Direction->roll;
   NewDirection.yaw   = Direction->yaw;
   NewDirection2.Len = Direction->Len;
   NewDirection2.pitch = Direction->pitch;
   NewDirection2.roll = Direction->roll;
   NewDirection2.yaw = Direction->yaw;

   TotalLen+= Direction->Len;

   angle = (double) ((Long)Random->GetNumber()) / 23860929 / 3;
   angle2 = (double) ((Long)Random->GetNumber()) / 23860929 / 3;
   NewDirection.pitch += angle;
   NewDirection.yaw +=angle2;
   NewDirection2.pitch -= angle;
   NewDirection2.yaw -=angle2;

   Rect.sx = floor(Point->x+0.5);
   Rect.sy = floor(Point->y+0.5);
   Rect.sz = floor(Point->z+0.5);
   Rect.ex = floor(NewPoint.x + 0.5);
   Rect.ey = floor(NewPoint.y + 0.5);
   Rect.ez = floor(NewPoint.z + 0.5);
   Thickness.Start = 1;
   Thickness.End = 1;
   Draw_safe_VoxelLine2(&Rect, &Thickness, 10);

   if (TotalLen<128.0)
   {
     Draw_subtree_1( Sector, &NewPoint, &NewDirection, Random, TotalLen);
     Draw_subtree_1( Sector, &NewPoint, &NewDirection2, Random, TotalLen);
   }


}

void ZVoxelSector::Draw_safe_Tree_Type_1(ZVoxelSector * Sector, ZVector3L * Coords)
{
  ZVector3d Point;
  ZLightSpeedRandom Random;
  ZPolar3d  Direction;

  Point.x = (double)Coords->x;
  Point.y = (double)Coords->y;
  Point.z = (double)Coords->z;

  Direction.Len = 10.0;
  Direction.pitch = 90.0;
  Direction.yaw = 90.0;

  Draw_subtree_1( Sector, &Point, &Direction, &Random, 0.0);
}

void ZVoxelSector::Draw_subtree_2(ZVector3d * Point, ZPolar3d * Direction, ZLightSpeedRandom * Random, double TotalLen)
{
   ZVector3d BranchVector, NewPoint, NewPoint2;
   ZPolar3d  NewDirection, NewDirection2;
   double angle,angle2;
   ZRect3L_2 Rect;
   ZRect1d Thickness;

   BranchVector.x = Direction->Len * sin(Direction->yaw / 57.295779506) * cos(Direction->pitch / 57.295779506);
   BranchVector.y = Direction->Len * sin(Direction->yaw / 57.295779506) * sin(Direction->pitch / 57.295779506);
   BranchVector.z = Direction->Len * cos(Direction->yaw / 57.295779506);

   NewPoint.x = Point->x + BranchVector.x;
   NewPoint.y = Point->y + BranchVector.y;
   NewPoint.z = Point->z + BranchVector.z;

   NewDirection.Len   = Direction->Len ;
   NewDirection.pitch = Direction->pitch;
   NewDirection.roll  = Direction->roll;
   NewDirection.yaw   = Direction->yaw;
   NewDirection2.Len  = Direction->Len ;
   NewDirection2.pitch = Direction->pitch;
   NewDirection2.roll = Direction->roll;
   NewDirection2.yaw = Direction->yaw;

   TotalLen+= Direction->Len;

   angle = (double) ((Long)Random->GetNumber()) / 23860929 / 3;
   angle2 = (double) ((Long)Random->GetNumber()) / 23860929 / 3;
   NewDirection.pitch += angle;
   NewDirection.yaw +=angle2;
   NewDirection2.pitch -= angle;
   NewDirection2.yaw -=angle2;

   Rect.sx = floor(Point->x+0.5);
   Rect.sy = floor(Point->y+0.5);
   Rect.sz = floor(Point->z+0.5);
   Rect.ex = floor(NewPoint.x + 0.5);
   Rect.ey = floor(NewPoint.y + 0.5);
   Rect.ez = floor(NewPoint.z + 0.5);
   Thickness.Start = 1;
   Thickness.End = 1;
   Draw_safe_VoxelLine2(&Rect, &Thickness, 68);


   if (TotalLen>40.0) this->Draw_safe_Sphere( NewPoint.x, NewPoint.y, NewPoint.z, 10.0,67,false);
   if (TotalLen<64.0)
   {
     Draw_subtree_2( &NewPoint, &NewDirection, Random, TotalLen);
     Draw_subtree_2( &NewPoint, &NewDirection2, Random, TotalLen);
   }
   else
   {
     this->Draw_safe_Sphere( NewPoint.x, NewPoint.y, NewPoint.z, 10.0,67,false);
   }


}

void ZVoxelSector::Draw_safe_Tree_Type_2( ZVector3L * Coords)
{
  ZVector3d Point;
  ZLightSpeedRandom Random;
  ZPolar3d  Direction;

  Point.x = (double)Coords->x;
  Point.y = (double)Coords->y;
  Point.z = (double)Coords->z;

  Direction.Len = 10.0;
  Direction.pitch = 90.0;
  Direction.yaw = 90.0;

  this->Draw_subtree_2( &Point, &Direction, &Random, 0.0);
}

void ZVoxelSector::Draw_subtree_3(ZVector3d * Point, ZPolar3d * Direction, ZLightSpeedRandom * Random, double TotalLen)
{
   ZVector3d BranchVector, NewPoint, NewPoint2;
   ZPolar3d  NewDirection, NewDirection2;
   double angle,angle2;
   ZRect3L_2 Rect;
   ZRect1d Thickness;

   BranchVector.x = Direction->Len * sin(Direction->yaw / 57.295779506) * cos(Direction->pitch / 57.295779506);
   BranchVector.y = Direction->Len * sin(Direction->yaw / 57.295779506) * sin(Direction->pitch / 57.295779506);
   BranchVector.z = Direction->Len * cos(Direction->yaw / 57.295779506);

   NewPoint.x = Point->x + BranchVector.x;
   NewPoint.y = Point->y + BranchVector.y;
   NewPoint.z = Point->z + BranchVector.z;

   NewDirection.Len   = Direction->Len ;
   NewDirection.pitch = Direction->pitch;
   NewDirection.roll  = Direction->roll;
   NewDirection.yaw   = Direction->yaw;
   NewDirection2.Len  = Direction->Len ;
   NewDirection2.pitch = Direction->pitch;
   NewDirection2.roll = Direction->roll;
   NewDirection2.yaw = Direction->yaw;

   if (NewDirection2.Len > 10.0) NewDirection2.Len = 10.0;
   if (NewDirection.Len > 10.0) NewDirection.Len = 10.0;

   TotalLen+= Direction->Len;

   angle = (double) ((Long)Random->GetNumber()) / 23860929 / 3;
   angle2 = (double) ((Long)Random->GetNumber()) / 23860929 / 3;
   NewDirection.pitch += angle;
   NewDirection.yaw +=angle2;
   NewDirection2.pitch -= angle;
   NewDirection2.yaw -=angle2;

   Rect.sx = floor(Point->x+0.5);
   Rect.sy = floor(Point->y+0.5);
   Rect.sz = floor(Point->z+0.5);
   Rect.ex = floor(NewPoint.x + 0.5);
   Rect.ey = floor(NewPoint.y + 0.5);
   Rect.ez = floor(NewPoint.z + 0.5);
   Thickness.Start = 5 - ((TotalLen - Direction->Len) / 24);
   Thickness.End = 5 - (TotalLen / 24);
   Draw_safe_VoxelLine2(&Rect, &Thickness, 68);


   if (TotalLen>100.0) this->Draw_safe_Sphere( NewPoint.x, NewPoint.y, NewPoint.z, 10.0,70,false);
   if (TotalLen<120.0)
   {
     Draw_subtree_3( &NewPoint, &NewDirection, Random, TotalLen);
     Draw_subtree_3( &NewPoint, &NewDirection2, Random, TotalLen);
   }
   else
   {
     this->Draw_safe_Sphere( NewPoint.x, NewPoint.y, NewPoint.z, 10.0,70,false);
   }


}

void ZVoxelSector::Draw_safe_Tree_Type_3( ZVector3L * Coords)
{
  ZVector3d Point;
  ZLightSpeedRandom Random;
  ZPolar3d  Direction;

  Point.x = (double)Coords->x;
  Point.y = (double)Coords->y;
  Point.z = (double)Coords->z;

  Direction.Len = 40.0;
  Direction.pitch = 90.0;
  Direction.yaw = 90.0;

  Draw_subtree_3( &Point, &Direction, &Random, 0.0);
}


void ZVoxelSector::BlitSector( ZVoxelSector * Source, ZVector3L * Offset)
{
  ZVector3L Size;
  Long t;
  ULong xs,zs, xd,zd;
  ZVector3L SStart, SEnd,DStart,DEnd, SSize,DSize;
  ULong SOffsetStep_x, SOffsetStep_z, DOffsetStep_z, DOffsetStep_x;
  ULong So,Do, SoEnd;

  SSize.x = Source->Size_x; SSize.y = Source->Size_y; SSize.z = Source->Size_z;
  DSize.x = Size_x;         DSize.y = Size_y;         DSize.z = Size_z;

  SStart = 0;
  SEnd   = SSize;
  DStart = *Offset;
  DEnd   = SSize ; DEnd += *Offset;

  if ( (t = DEnd.x - DSize.x) > 0) { DEnd.x -= t; SEnd.x -= t; }
  if ( (t = DEnd.y - DSize.y) > 0) { DEnd.y -= t; SEnd.y -= t; }
  if ( (t = DEnd.z - DSize.z) > 0) { DEnd.z -= t; SEnd.z -= t; }

  if ( (t = -DStart.x) > 0) { DStart.x = 0; SStart.x += t; }
  if ( (t = -DStart.y) > 0) { DStart.y = 0; SStart.y += t; }
  if ( (t = -DStart.z) > 0) { DStart.z = 0; SStart.z += t; }

  if ( SStart.x >= SEnd.x) return;
  if ( SStart.y >= SEnd.y) return;
  if ( SStart.z >= SEnd.z) return;

  Size.x = Source->Size_x;
  Size.y = Source->Size_y;
  Size.z = Source->Size_z;



  SOffsetStep_x = Source->Size_y;
  SOffsetStep_z = Source->Size_y * Source->Size_x;
  DOffsetStep_x = Size_y;
  DOffsetStep_z = Size_y * Size_x;

  ULong SOffsetStart_x, SOffsetStart_y, SOffsetStart_z;
  ULong SOffsetEnd_x  , SOffsetEnd_z;
  ULong DOffsetStart_x, DOffsetStart_y, DOffsetStart_z;

  SOffsetStart_x = SStart.x * SOffsetStep_x;
  SOffsetEnd_x = (SEnd.x ) * SOffsetStep_x;
  DOffsetStart_x = DStart.x * DOffsetStep_x;

  SOffsetStart_y = SStart.y;
  DOffsetStart_y = DStart.y;

  SOffsetStart_z = SStart.z * SOffsetStep_z;
  SOffsetEnd_z = (SEnd.z ) * SOffsetStep_z;
  DOffsetStart_z = DStart.z * DOffsetStep_z;



  for (zs=SOffsetStart_z, zd=DOffsetStart_z; zs < SOffsetEnd_z; zs += SOffsetStep_z, zd += DOffsetStep_z)
  {
    for (xs=SOffsetStart_x, xd=DOffsetStart_x; xs < SOffsetEnd_x; xs += SOffsetStep_x, xd += DOffsetStep_x)
    {
      So = zs + xs + SOffsetStart_y;
      Do = zd + xd + DOffsetStart_y;
      SoEnd = zs + xs + SEnd.y;
      for ( ; So < SoEnd; So++, Do++)
      {
        register UShort VoxelType;
        VoxelType = Source->Data[So];

        if (VoxelType)
        {
          // Bound checking check
          #if COMPILEOPTION_BOUNDCHECKINGSLOW==1
            if (So > Source->DataSize ) MANUAL_BREAKPOINT;
            if (Do > DataSize) MANUAL_BREAKPOINT;
          #endif

          Data[Do] = VoxelType;
          if ((OtherInfos[Do] = Source->OtherInfos[So]))
          {
            if (VoxelTypeManager->VoxelTable[Data[So]]->Is_HasAllocatedMemoryExtension)
            {
              ZVoxelExtension * Ext;
              Ext = ((ZVoxelExtension *)OtherInfos[So])->GetNewCopy();
              OtherInfos[Do] = (ZMemSize)Ext;
            }
          }
        }

      }
    }
  }

}

void ZVoxelSector::Subst(UShort Source_VoxelType, UShort Dest_VoxelType)
{
  ZMemSize VoxelCount,i;
  if (Flag_NotStandardSize) VoxelCount = Size_x * Size_y * Size_z;
  else                      VoxelCount = ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_Z;

  for (i=0;i<VoxelCount;i++)
  {
    if (Data[i]==Source_VoxelType) Data[i]=Dest_VoxelType;
  }

}

