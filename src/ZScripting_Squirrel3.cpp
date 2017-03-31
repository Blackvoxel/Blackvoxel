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
 * ZScripting_Squirrel3.cpp
 *
 *  Created on: 27 déc. 2012
 *      Author: laurent
 */

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif

#ifndef Z_ZSCRIPTING_SQUIRREL3_H
#  include "ZScripting_Squirrel3.h"
#endif

#ifndef Z_ZVOXELEXTENSION_PROGRAMMABLE_H
#  include "ZVoxelExtension_Programmable.h"
#endif

#ifndef Z_ZHIGHPERFTIMER_H
#  include "ZHighPerfTimer.h"
#endif

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

#ifndef Z_ZTEXTUREMANAGER_H
#  include "ZTextureManager.h"
#endif

#ifndef Z_ZGENERICCANVA2_H
#  include "ZGenericCanva_2.h"
#endif

#include <stdarg.h>
#include <stdio.h>

#include "squirrel.h"
#include "sqstdio.h"
#include "sqstdaux.h"
#include "sqstdmath.h"
#include "sqstdblob.h"
#include "sqstdsystem.h"
#include "sqstdstring.h"

extern ZGame * Ge;

class ZStoreSq3
{
  public:
    ZStoreSq3() {GameEnv=Ge; Allow_WorldManipulations = false; v=0; VoxelPosition = 0; HasMoved = false; Extension = 0;LaunchContext=0;}
    HSQUIRRELVM v;
    ZGame * GameEnv;
    ZVoxelExtension_Programmable * Extension;
    bool Allow_WorldManipulations;
    bool HasMoved;
    ULong LaunchContext;
    ZVector3L VoxelPosition;
};

void printfunc(HSQUIRRELVM v,const SQChar *s,...)
{
  va_list vl;
  va_start(vl, s);
  vfprintf(stdout, s, vl);
  fflush(stdout);
  va_end(vl);
}

void errorfunc(HSQUIRRELVM v,const SQChar *s,...)
{
  va_list vl;
  va_start(vl, s);
  vfprintf(stderr, s, vl);
  va_end(vl);
}

ZVector3L Dir_Table[6] =
{
  ZVector3L( 0, 0, 1 ),
  ZVector3L( 1, 0, 0 ),
  ZVector3L( 0, 0, -1),
  ZVector3L(-1, 0, 0 ),
  ZVector3L( 0, 1, 0 ),
  ZVector3L( 0,-1, 0 )
};

SQInteger function_GetX(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQInteger RetVal;

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if (!S->Allow_WorldManipulations) RetVal = 0;
  else                              RetVal = S->VoxelPosition.x;
  sq_pushinteger(v,RetVal);
  return(1);
}

SQInteger function_GetY(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQInteger RetVal;

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if (!S->Allow_WorldManipulations) RetVal = 0;
  else                              RetVal = S->VoxelPosition.y;
  sq_pushinteger(v,RetVal);
  return(1);
}

SQInteger function_GetZ(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQInteger RetVal;

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if (!S->Allow_WorldManipulations) RetVal = 0;
  else                              RetVal = S->VoxelPosition.z;
  sq_pushinteger(v,RetVal);
  return(1);
}

SQInteger function_GetGameTime(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger RetVal;

  // RetVal = ZHighPerfTimer::GetActualTime() / 1000;

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  RetVal = (SQInteger)(S->GameEnv->PhysicEngine->GetSelectedActor()->Time_TotalGameTime / 1000LLU);

  sq_pushinteger(v,RetVal);
  return(1);
}

SQInteger function_Look(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQInteger RetVal;
  SQInteger InVal;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &InVal);
  InVal %= 6;

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if (!S->Allow_WorldManipulations){ RetVal = -1; sq_pushinteger(v,RetVal); return(1); }

  RetVal = S->GameEnv->World->GetVoxel( S->VoxelPosition.x + Dir_Table[InVal].x, S->VoxelPosition.y + Dir_Table[InVal].y, S->VoxelPosition.z + Dir_Table[InVal].z );

  sq_pushinteger(v,RetVal);
  return(1);
}

SQInteger function_Move(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQInteger InVal;
  UShort VoxelType;
  ZVoxelLocation Loc;
  ZVector3L NewLocation;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &InVal);
  InVal %= 6;

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if ( (!S->Allow_WorldManipulations) || (S->HasMoved) ) { sq_pushbool(v,SQFalse); return(1); }

  VoxelType = S->GameEnv->World->GetVoxel( S->VoxelPosition.x + Dir_Table[InVal].x, S->VoxelPosition.y + Dir_Table[InVal].y, S->VoxelPosition.z + Dir_Table[InVal].z );
  if (!S->GameEnv->VoxelTypeManager.VoxelTable[VoxelType]->Is_PlayerCanPassThrough) { sq_pushbool(v,SQFalse); return(1); };

  NewLocation = S->VoxelPosition;
  NewLocation += Dir_Table[InVal];
  S->GameEnv->World->MoveVoxel( &S->VoxelPosition,
                                &NewLocation,
                                0, ZVoxelSector::CHANGE_IMPORTANT);
  S->GameEnv->World->GetVoxelLocation(&Loc, NewLocation.x , NewLocation.y, NewLocation.z );
  Loc.Sector->ModifTracker.Set(Loc.Offset);

  // Update voxel position

  S->VoxelPosition = NewLocation;



  S->HasMoved = true;
  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_PickVoxel(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQInteger InVal;
  UShort VoxelType;
  ZVector3L NewLocation;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &InVal);
  InVal %= 6;

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if ( (!S->Allow_WorldManipulations) || (S->HasMoved) ) { sq_pushbool(v,SQFalse); return(1); }

  NewLocation = S->VoxelPosition; NewLocation += Dir_Table[InVal];

  VoxelType = S->GameEnv->World->GetVoxel( S->VoxelPosition.x + Dir_Table[InVal].x, S->VoxelPosition.y + Dir_Table[InVal].y, S->VoxelPosition.z + Dir_Table[InVal].z );
  if (!S->GameEnv->VoxelTypeManager.VoxelTable[VoxelType]->BvProp_CanBePickedUpByRobot) { sq_pushbool(v,SQFalse); return(1); };

  if (1==S->Extension->StoreBlocks( VoxelType, 1))
  {
    S->GameEnv->World->SetVoxel_WithCullingUpdate(NewLocation.x,NewLocation.y,NewLocation.z, 0, ZVoxelSector::CHANGE_IMPORTANT , true, 0 );
    sq_pushbool(v,SQTrue);
  }
  else
  {
    sq_pushbool(v,SQFalse);
  }

  return(1);
}


SQInteger function_GetRnd(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQInteger InVal;
  SQInteger RetVal;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &InVal);

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  RetVal = S->GameEnv->Random.GetNumber();
  if(InVal) RetVal %= InVal;
  sq_pushinteger(v,RetVal);
  return(1);
}


SQInteger function_GetVoxelProp(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQInteger RetVal;
  SQInteger InVal, InVal2;
  SQInteger ArgCount;

  // Parameter input

  ArgCount = sq_gettop(v); if (ArgCount<3) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  sq_getinteger(v,2, &InVal);
  InVal &= 0xffff;
  sq_getinteger(v,3, &InVal2);

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  // if (!S->Allow_WorldManipulations){ RetVal = -1; sq_pushinteger(v,RetVal); return(1); }

  switch (InVal2)
  {
    case 0: RetVal = S->GameEnv->VoxelTypeManager.VoxelTable[InVal]->Is_PlayerCanPassThrough ? SQTrue : SQFalse; sq_pushbool(v,RetVal); break;
    case 1: RetVal = S->GameEnv->VoxelTypeManager.VoxelTable[InVal]->Is_Interface_PushBlock ? SQTrue : SQFalse; sq_pushbool(v,RetVal); break;
    case 2: RetVal = S->GameEnv->VoxelTypeManager.VoxelTable[InVal]->Is_Interface_PullBlock ? SQTrue : SQFalse; sq_pushbool(v,RetVal); break;
    default: sq_pushbool(v,SQFalse); break;

  }

  return(1);
}

SQInteger function_GetVoxelName(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQInteger InVal;
  SQInteger ArgCount;

  // Parameter input

  ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &InVal);
  InVal &= 0xffff;

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  // if (!S->Allow_WorldManipulations){ RetVal = -1; sq_pushinteger(v,RetVal); return(1); }

  sq_pushstring(v, S->GameEnv->VoxelTypeManager.VoxelTable[InVal]->VoxelTypeName.String, S->GameEnv->VoxelTypeManager.VoxelTable[InVal]->VoxelTypeName.Len );

  return(1);
}


SQInteger function_PlaceVoxel(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  ULong SlotNum;

  SQInteger InVal, InVal2;
  UShort VoxelType;
  ZVector3L NewLocation;
  bool Found;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<3) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  sq_getinteger(v,2, &InVal);
  InVal %= 6;
  sq_getinteger(v,3, &InVal2);
  InVal2 &= 0xffff;

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if ( (!S->Allow_WorldManipulations) || (S->HasMoved) ) { sq_pushbool(v,SQFalse); return(1); }

  NewLocation = S->VoxelPosition; NewLocation += Dir_Table[InVal];

  VoxelType = S->GameEnv->World->GetVoxel( S->VoxelPosition.x + Dir_Table[InVal].x, S->VoxelPosition.y + Dir_Table[InVal].y, S->VoxelPosition.z + Dir_Table[InVal].z );
  if (!S->GameEnv->VoxelTypeManager.VoxelTable[VoxelType]->Is_PlayerCanPassThrough) { sq_pushbool(v,SQFalse); return(1); };

  // Find the voxel type in the robot storage.
  for (SlotNum=0, Found = false ; SlotNum<ZVoxelExtension_Programmable::Storage_NumSlots ; SlotNum++)
  {
    if (   (S->Extension->VoxelType[SlotNum] == InVal2)
        && (S->Extension->VoxelQuantity[SlotNum]>0 )  )   { Found = true; break; }
  }

  if (!Found) { sq_pushbool(v,SQFalse); return(1); }

  S->Extension->VoxelQuantity[SlotNum]--; if (S->Extension->VoxelQuantity[SlotNum] == 0) S->Extension->VoxelType[SlotNum] = 0;
  S->GameEnv->World->SetVoxel_WithCullingUpdate(NewLocation.x,NewLocation.y,NewLocation.z, InVal2, ZVoxelSector::CHANGE_IMPORTANT, true, 0 );
  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_PlaceVoxel3D(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  ZVector3L NewLocation;

  SQInteger x,y,z, tmp;
  UShort VoxelType, OldVoxelType, tmp2;
  ZVoxelLocation Loc;
  ZVoxelType * OldVoxelTypeInfo;
  ULong SlotNum;

  // Parameter input and filtering.

  SQInteger ArgCount = sq_gettop(v); if (ArgCount < 5) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  if (sq_gettype(v,5) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 4"); //throws an exception
  sq_getinteger(v,2, &x);
  sq_getinteger(v,3, &y);
  sq_getinteger(v,4, &z);
  sq_getinteger(v,5, &tmp); VoxelType = tmp;

  // Test if we can do that operation.
  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if ( (!S->Allow_WorldManipulations) ) { sq_pushbool(v,SQFalse); return(1); }

  // Compute the plotting position
  NewLocation = S->VoxelPosition;
  NewLocation.x += x; NewLocation.y += y; NewLocation.z += z;

  // Try to find the location
  if (!S->GameEnv->World->GetVoxelLocation(&Loc, &NewLocation)) { sq_pushbool(v,SQFalse); return(1); };

  // Get The voxel at the location and see if we can replace it.
  OldVoxelType = Loc.Sector->Data[Loc.Offset];
  OldVoxelTypeInfo = S->GameEnv->VoxelTypeManager.VoxelTable[OldVoxelType];
  if (OldVoxelTypeInfo->BvProp_PrRobotReplaceMinLevel > S->Extension->RobotLevel) { sq_pushbool(v,SQFalse); return(1); };

  // Find the voxel type in the robot storage and unstore 1 unit.
  if (!S->Extension->FindSlot(VoxelType, SlotNum)) { sq_pushbool(v,SQFalse); return(1); };
  S->Extension->UnstoreBlocks(SlotNum, 1, &tmp2 );

  // Set voxel and moved flag
  S->GameEnv->World->SetVoxel_WithCullingUpdate(NewLocation.x, NewLocation.y, NewLocation.z ,VoxelType, ZVoxelSector::CHANGE_IMPORTANT, true, 0);
  Loc.Sector->ModifTracker.Set(Loc.Offset);

  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_UnofficialFastPlot(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  ZVector3L NewLocation;

  SQInteger x,y,z, tmp;
  UShort VoxelType;
  ZVoxelLocation Loc;

  // Parameter input and filtering.

  sq_getinteger(v,2, &x);
  sq_getinteger(v,3, &y);
  sq_getinteger(v,4, &z);
  sq_getinteger(v,5, &tmp); VoxelType = tmp;

  // Test if we can do that operation.
  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if ( (!S->Allow_WorldManipulations)) { sq_pushbool(v,SQFalse); return(1); }

  // Compute the plotting position
  NewLocation = S->VoxelPosition;
  NewLocation.x += x; NewLocation.y += y; NewLocation.z += z;

  // Try to find the location
  if (!S->GameEnv->World->GetVoxelLocation(&Loc, &NewLocation)) { sq_pushbool(v,SQFalse); return(1); };

  // Set voxel and moved flag
  S->GameEnv->World->SetVoxel_WithCullingUpdate(NewLocation.x, NewLocation.y, NewLocation.z ,VoxelType, ZVoxelSector::CHANGE_IMPORTANT, true, 0);
  Loc.Sector->ModifTracker.Set(Loc.Offset);

  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_PickVoxel3D(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  ZVector3L NewLocation;

  SQInteger x,y,z;
  UShort VoxelType;
  ZVoxelLocation Loc;
  ZVoxelType * VoxelTypeInfo;

  // Parameter input and filtering.

  SQInteger ArgCount = sq_gettop(v); if (ArgCount < 4) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  sq_getinteger(v,2, &x);
  sq_getinteger(v,3, &y);
  sq_getinteger(v,4, &z);

  // Test if we can do that operation.
  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if (!S->Allow_WorldManipulations) { sq_pushbool(v,SQFalse); return(1); }

  // Exclude the 0,0,0 position;

  if (x == 0 && y == 0 && z==0) { sq_pushbool(v,SQFalse); return(1); };

  // Compute the plotting position
  NewLocation = S->VoxelPosition;
  NewLocation.x += x; NewLocation.y += y; NewLocation.z += z;

  // Retrieve the voxel access info
  if (!S->GameEnv->World->GetVoxelLocation(&Loc, &NewLocation)) { sq_pushbool(v,SQFalse); return(1); };

  // Get The voxel at the location and see if we can pick it.
  VoxelType     = Loc.Sector->Data[Loc.Offset];
  VoxelTypeInfo = S->GameEnv->VoxelTypeManager.VoxelTable[VoxelType];
  if (VoxelTypeInfo->BvProp_XrRobotPickMinLevel > S->Extension->RobotLevel) { sq_pushbool(v,SQFalse); return(1); };

  // Destroy the Voxel and store it into robot's inventory.
  if (1==S->Extension->StoreBlocks( VoxelType, 1))
  {
    S->GameEnv->World->SetVoxel_WithCullingUpdate(NewLocation.x,NewLocation.y,NewLocation.z, 0, ZVoxelSector::CHANGE_IMPORTANT, true, 0 );
    sq_pushbool(v,SQTrue);
  }
  else
  {
    sq_pushbool(v,SQFalse);
  }
  return(1);
}

SQInteger function_MoveVoxel3D(HSQUIRRELVM v)
{
  // To do
  ZStoreSq3 * S;
  ZVector3L SourceLocation, DestLocation;

  SQInteger xs,ys,zs,xd,yd,zd;
  UShort DestVoxelType, SourceVoxelType;
  ZVoxelLocation LocSource,LocDest;
  ZVoxelType * DestVoxelTypeInfo, * SourceVoxelTypeInfo;

  // Parameter input and filtering.

  SQInteger ArgCount = sq_gettop(v); if (ArgCount < 5) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  if (sq_gettype(v,5) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 4"); //throws an exception
  sq_getinteger(v,2, &xs);
  sq_getinteger(v,3, &ys);
  sq_getinteger(v,4, &zs);
  sq_getinteger(v,5, &xd);
  sq_getinteger(v,6, &yd);
  sq_getinteger(v,7, &zd);

  // Test if we can do that operation.
  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if ( (!S->Allow_WorldManipulations) ) { sq_pushbool(v,SQFalse); return(1); }

  // Compute the start and end voxel position
  SourceLocation = DestLocation = S->VoxelPosition;
  SourceLocation.x += xs; SourceLocation.y += ys; SourceLocation.z += zs;
  DestLocation.x   += xd; DestLocation.y   += yd; DestLocation.z   += zd;

  // Get The voxel at the destination location and see if we can replace it.
  if (!S->GameEnv->World->GetVoxelLocation(&LocDest, &DestLocation)) { sq_pushbool(v,SQFalse); return(1); };
  DestVoxelType = LocDest.Sector->Data[LocDest.Offset];
  DestVoxelTypeInfo = S->GameEnv->VoxelTypeManager.VoxelTable[DestVoxelType];
  if (DestVoxelTypeInfo->BvProp_PrRobotReplaceMinLevel > S->Extension->RobotLevel) { sq_pushbool(v,SQFalse); return(1); };

  // Get The voxel at the source location and see if we can move it.
  if (!S->GameEnv->World->GetVoxelLocation(&LocSource, &SourceLocation)) { sq_pushbool(v,SQFalse); return(1); };
  SourceVoxelType = LocSource.Sector->Data[LocSource.Offset];
  SourceVoxelTypeInfo = S->GameEnv->VoxelTypeManager.VoxelTable[SourceVoxelType];
  if (SourceVoxelTypeInfo->BvProp_PrRobotMoveMinLevel > S->Extension->RobotLevel) { sq_pushbool(v,SQFalse); return(1); };

  // Move the voxel and set the moved flag
  if (!S->GameEnv->World->MoveVoxel(&SourceLocation, &DestLocation, 0, ZVoxelSector::CHANGE_IMPORTANT)) sq_pushbool(v,SQFalse);
  LocDest.Sector->ModifTracker.Set(LocDest.Offset);

  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_Unofficial_FastMoveVoxel3D(HSQUIRRELVM v)
{
  // To do
  ZStoreSq3 * S;
  ZVector3L SourceLocation, DestLocation;

  SQInteger xs,ys,zs,xd,yd,zd;
  ZVoxelLocation LocDest;

  // Parameter input and filtering.

  sq_getinteger(v,2, &xs);
  sq_getinteger(v,3, &ys);
  sq_getinteger(v,4, &zs);
  sq_getinteger(v,5, &xd);
  sq_getinteger(v,6, &yd);
  sq_getinteger(v,7, &zd);

  // Test if we can do that operation.
  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if ( (!S->Allow_WorldManipulations) ) { sq_pushbool(v,SQFalse); return(1); }

  // Compute the start and end voxel position
  SourceLocation = DestLocation = S->VoxelPosition;
  SourceLocation.x += xs; SourceLocation.y += ys; SourceLocation.z += zs;
  DestLocation.x   += xd; DestLocation.y   += yd; DestLocation.z   += zd;

  // Get the destination voxel location.
  if (!S->GameEnv->World->GetVoxelLocation(&LocDest, &DestLocation)) { sq_pushbool(v,SQFalse); return(1); };

  // Move the voxel and set the moved flag
  if (!S->GameEnv->World->MoveVoxel(&SourceLocation, &DestLocation, 0, ZVoxelSector::CHANGE_IMPORTANT))   sq_pushbool(v,SQFalse);
  LocDest.Sector->ModifTracker.Set(LocDest.Offset);

  sq_pushbool(v,SQTrue);
  return(1);
}


SQInteger function_Look3D(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQInteger x,y,z;
  UShort VoxelType;
  ZVector3L NewLocation;
  ZVoxelLocation Loc;

  // Parameter input and filtering.

  SQInteger ArgCount = sq_gettop(v); if (ArgCount < 4) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  sq_getinteger(v,2, &x);
  sq_getinteger(v,3, &y);
  sq_getinteger(v,4, &z);

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if (!S->Allow_WorldManipulations) { sq_pushinteger(v, -1); return(1); }

  // Compute the looking position
  NewLocation = S->VoxelPosition;
  NewLocation.x += x; NewLocation.y += y; NewLocation.z += z;

  // Look at the position and find the voxel
  if (!S->GameEnv->World->GetVoxelLocation( &Loc, &NewLocation )) {sq_pushinteger(v, -1 );return(1);}
  VoxelType = Loc.Sector->Data[Loc.Offset];

  sq_pushinteger(v,(SQInteger)VoxelType);
  return(1);
}

SQInteger function_GetInfo(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger DataType;
  ZString Nm;

  SQInteger ArgCount = sq_gettop(v); if (ArgCount < 2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &DataType);

  // Get the context pointer
  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if ( (!S->Allow_WorldManipulations) && (DataType>0 && DataType<=19) ) { sq_pushbool(v, SQFalse); return(1); }

  switch (DataType)
  {
    case 0: sq_pushinteger(v,24); break;
    case 1: sq_pushfloat(v, (SQFloat)S->GameEnv->PhysicEngine->GetSelectedActor()->Location.x );  break;
    case 2: sq_pushfloat(v, (SQFloat)S->GameEnv->PhysicEngine->GetSelectedActor()->Location.y );  break;
    case 3: sq_pushfloat(v, (SQFloat)S->GameEnv->PhysicEngine->GetSelectedActor()->Location.z );  break;
    case 4: sq_pushinteger(v, (SQInteger)  (Long)(((ELong) S->GameEnv->PhysicEngine->GetSelectedActor()->Location.x ) >> 8) );  break;
    case 5: sq_pushinteger(v, (SQInteger)  (Long)(((ELong) S->GameEnv->PhysicEngine->GetSelectedActor()->Location.y ) >> 8) );  break;
    case 6: sq_pushinteger(v, (SQInteger)  (Long)(((ELong) S->GameEnv->PhysicEngine->GetSelectedActor()->Location.z ) >> 8) );  break;
    case 7: sq_pushfloat(v, (SQFloat)S->GameEnv->PhysicEngine->GetSelectedActor()->ViewDirection.yaw * 0.017453293 );  break;
    case 8: sq_pushfloat(v, (SQFloat)S->GameEnv->PhysicEngine->GetSelectedActor()->ViewDirection.pitch * 0.017453293);  break;
    case 9: sq_pushfloat(v, (SQFloat)S->GameEnv->PhysicEngine->GetSelectedActor()->ViewDirection.roll * 0.017453293);  break;
    case 11: {ZActor * Actor = S->GameEnv->PhysicEngine->GetSelectedActor(); sq_pushfloat(v, (SQFloat)(Actor->Location.x + Actor->EyesPosition.x) ); } break;
    case 12: {ZActor * Actor = S->GameEnv->PhysicEngine->GetSelectedActor(); sq_pushfloat(v, (SQFloat)(Actor->Location.y + Actor->EyesPosition.y) ); } break;
    case 13: {ZActor * Actor = S->GameEnv->PhysicEngine->GetSelectedActor(); sq_pushfloat(v, (SQFloat)(Actor->Location.z + Actor->EyesPosition.z) ); } break;
    case 14: {ZActor * Actor = S->GameEnv->PhysicEngine->GetSelectedActor(); sq_pushinteger(v, (SQInteger) (Long)(((ELong) (Actor->Location.x + Actor->EyesPosition.x) ) >> 8) ); } break;
    case 15: {ZActor * Actor = S->GameEnv->PhysicEngine->GetSelectedActor(); sq_pushinteger(v, (SQInteger) (Long)(((ELong) (Actor->Location.y + Actor->EyesPosition.y) ) >> 8) ); } break;
    case 16: {ZActor * Actor = S->GameEnv->PhysicEngine->GetSelectedActor(); sq_pushinteger(v, (SQInteger) (Long)(((ELong) (Actor->Location.z + Actor->EyesPosition.z) ) >> 8) ); } break;
    case 17: sq_pushfloat(v, (SQFloat)S->GameEnv->PhysicEngine->GetSelectedActor()->Velocity.x );  break;
    case 18: sq_pushfloat(v, (SQFloat)S->GameEnv->PhysicEngine->GetSelectedActor()->Velocity.y );  break;
    case 19: sq_pushfloat(v, (SQFloat)S->GameEnv->PhysicEngine->GetSelectedActor()->Velocity.z );  break;
    case 20: Nm = ZString::GetPathSeparator(); sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 21: sq_pushinteger(v, (SQInteger) COMPILEOPTION_VERSIONNUM );  break;
    case 22: sq_pushinteger(v, (SQInteger) S->Extension->ScriptNum );  break;
    case 23: sq_pushinteger(v, (SQInteger) S->LaunchContext );  break;
    case 24: sq_pushinteger(v, (SQInteger) 20 );  break;
    default: sq_pushbool(v, SQFalse); break;
  }
  return(1);
}

SQInteger function_GetInfoName(HSQUIRRELVM v)
{
  SQInteger DataType;
  ZString Nm;

  SQInteger ArgCount = sq_gettop(v); if (ArgCount < 2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &DataType);


  switch (DataType)
  {
    case 0: sq_pushinteger(v,24); break;
    case 1: Nm = "Player Location (x)"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 2: Nm = "Player Location (y)"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 3: Nm = "Player Location (z)"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 4: Nm = "Player Location (x) in Voxel Units"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 5: Nm = "Player Location (y) in Voxel Units"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 6: Nm = "Player Location (z) in Voxel Units"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 7: Nm = "Player Viewing direction (yaw)"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 8: Nm = "Player Viewing direction (pitch)"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 9: Nm = "Player Viewing direction (roll)"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 11: Nm = "Player Head Location (x)"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 12: Nm = "Player Head Location (y)"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 13: Nm = "Player Head Location (z)"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 14: Nm = "Player Head Location (x) in Voxel Units"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 15: Nm = "Player Head Location (y) in Voxel Units"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 16: Nm = "Player Head Location (z) in Voxel Units"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 17: Nm = "Player Velocity (x)"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 18: Nm = "Player Velocity (y)"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 19: Nm = "Player Velocity (z)"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 20: Nm = "File path separator character"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 21: Nm = "Program Version"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 22: Nm = "Actual Script Number"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 23: Nm = "Running Context"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 24: Nm = "Robot Inventory Slot Count"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    default: Nm = "NOTHING"; sq_pushstring(v, Nm.String, Nm.Len  );  break;
  }
  return(1);
}



SQInteger function_GetRobotLevel(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  S = (ZStoreSq3 *)sq_getforeignptr(v);

  sq_pushinteger(v,(SQInteger)S->Extension->RobotLevel);
  return(1);
}

SQInteger function_GetRobotID(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if (S->Extension->RobotSerialNumber == 0) S->Extension->RobotSerialNumber = S->GameEnv->Machine_Serial++; // Assign new serial if robot has none.
  sq_pushinteger(v,(SQInteger)S->Extension->RobotSerialNumber);
  return(1);
}

SQInteger function_MovePlayer(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQFloat x,y,z;
  ZVector3d PlayerPos;


  // Parameter input and filtering.

  SQInteger ArgCount = sq_gettop(v); if (ArgCount < 4) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_FLOAT) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_FLOAT) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_FLOAT) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  sq_getfloat(v,2, &x);
  sq_getfloat(v,3, &y);
  sq_getfloat(v,4, &z);

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if (!S->Allow_WorldManipulations) { sq_pushbool(v,SQFalse); return(1); }

  // Clipping.

  if (x>10.0f)  x =  5000.0f;
  if (x<-10.0f) x = -5000.0f;
  if (y>10.0f)  y =  5000.0f;
  if (y<-10.0f) y = -5000.0f;
  if (z>10.0f)  z =  5000.0f;
  if (z<-10.0f) z = -5000.0f;

  // Move the player.

  PlayerPos = S->GameEnv->PhysicEngine->GetSelectedActor()->Location;
  PlayerPos.x += x; PlayerPos.y +=y; PlayerPos.z +=z;
  S->GameEnv->PhysicEngine->GetSelectedActor()->SetPosition(PlayerPos);

  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_PlayerAddVelocity(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQFloat x,y,z;
  ZVector3d PlayerPos;
  ZActor * Actor;


  // Parameter input and filtering.

  SQInteger ArgCount = sq_gettop(v); if (ArgCount < 4) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_FLOAT) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_FLOAT) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_FLOAT) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  sq_getfloat(v,2, &x);
  sq_getfloat(v,3, &y);
  sq_getfloat(v,4, &z);

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if (!S->Allow_WorldManipulations) { sq_pushbool(v,SQFalse); return(1); }

  // Clipping.
/*
  if (x>10.0f)  x =  10.0f;
  if (x<-10.0f) x = -10.0f;
  if (y>10.0f)  y =  10.0f;
  if (y<-10.0f) y = -10.0f;
  if (z>10.0f)  z =  10.0f;
  if (z<-10.0f) z = -10.0f;
*/
  // Accelerate the player.

  Actor = S->GameEnv->PhysicEngine->GetSelectedActor();
  Actor->Velocity.x += x; Actor->Velocity.y += y; Actor->Velocity.z += z;

  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_PlayerSetVelocity(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQFloat x,y,z;
  ZVector3d PlayerPos;
  ZActor * Actor;


  // Parameter input and filtering.

  SQInteger ArgCount = sq_gettop(v); if (ArgCount < 4) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_FLOAT) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_FLOAT) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_FLOAT) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  sq_getfloat(v,2, &x);
  sq_getfloat(v,3, &y);
  sq_getfloat(v,4, &z);

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if (!S->Allow_WorldManipulations) { sq_pushbool(v,SQFalse); return(1); }

  // Clipping.
/*
  if (x>10.0f)  x =  10.0f;
  if (x<-10.0f) x = -10.0f;
  if (y>10.0f)  y =  10.0f;
  if (y<-10.0f) y = -10.0f;
  if (z>10.0f)  z =  10.0f;
  if (z<-10.0f) z = -10.0f;
*/
  // Accelerate the player.

  Actor = S->GameEnv->PhysicEngine->GetSelectedActor();
  Actor->Velocity.x = x; Actor->Velocity.y = y; Actor->Velocity.z = z;

  sq_pushbool(v,SQTrue);
  return(1);
}


SQInteger function_GetPath(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger DataType;
  ZString Nm;

  SQInteger ArgCount = sq_gettop(v); if (ArgCount < 2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &DataType);

  // Get the context pointer
  S = (ZStoreSq3 *)sq_getforeignptr(v);

  switch (DataType)
  {
    case 0: sq_pushinteger(v,4); break;
    case 1: Nm = S->GameEnv->Path_UserData; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 2: Nm = S->GameEnv->Path_ActualUniverse; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 3: Nm = S->GameEnv->Path_UserScripts_Squirrel; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    case 4: Nm = S->GameEnv->Path_UserScripts_UserData; sq_pushstring(v, Nm.String, Nm.Len  );  break;
    default: Nm = "NOTHING"; sq_pushstring(v, Nm.String, Nm.Len  );  break;

  }
  return(1);
}


SQInteger function_PushVoxelTo(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  ULong SlotNum;

  SQInteger InVal, InVal2;
  UShort VoxelType;
  ZVector3L NewLocation;
  ZVoxelLocation Loc;
  bool Found;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<3) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  sq_getinteger(v,2, &InVal);
  InVal %= 6;
  sq_getinteger(v,3, &InVal2);
  InVal2 &= 0xffff;

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if ( (!S->Allow_WorldManipulations) || (S->HasMoved) ) { sq_pushbool(v,SQFalse); return(1); }

  NewLocation = S->VoxelPosition; NewLocation += Dir_Table[InVal];

  VoxelType = S->GameEnv->World->GetVoxel( NewLocation.x, NewLocation.y, NewLocation.z );
  if (!S->GameEnv->VoxelTypeManager.VoxelTable[VoxelType]->Is_Interface_PushBlock) { sq_pushbool(v,SQFalse); return(1); };

  // Find the voxel type in the robot storage.
  for (SlotNum=0, Found = false ; SlotNum<ZVoxelExtension_Programmable::Storage_NumSlots ; SlotNum++)
  {
    if (S->Extension->VoxelType[SlotNum] == InVal2 && S->Extension->VoxelQuantity[SlotNum]>0) { Found = true; break; }
  }

  if (!Found) { sq_pushbool(v,SQFalse); return(1); }

  S->GameEnv->World->GetVoxelLocation(&Loc, NewLocation.x, NewLocation.y, NewLocation.z );
  if (1!=S->GameEnv->VoxelTypeManager.VoxelTable[ Loc.Sector->Data[Loc.Offset] ]->Interface_PushBlock_Push(&Loc,InVal2,1)) { sq_pushbool(v,SQFalse); return(1); };
  S->Extension->VoxelQuantity[SlotNum]--; if (S->Extension->VoxelQuantity[SlotNum] == 0) S->Extension->VoxelType[SlotNum] = 0;

  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_PullVoxelFrom(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  ULong SlotNum;

  SQInteger InVal;
  UShort VoxelType;
  ZVector3L NewLocation;
  ZVoxelLocation Loc;
  bool Found;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &InVal);
  InVal %= 6;

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if ( (!S->Allow_WorldManipulations) || (S->HasMoved) ) { sq_pushbool(v,SQFalse); return(1); }

  NewLocation = S->VoxelPosition; NewLocation += Dir_Table[InVal];

  VoxelType = S->GameEnv->World->GetVoxel( NewLocation.x, NewLocation.y, NewLocation.z );
  if (!S->GameEnv->VoxelTypeManager.VoxelTable[VoxelType]->Is_Interface_PullBlock ) { sq_pushbool(v,SQFalse); return(1); };

  // Search for at least 1 empty slot
  for (SlotNum=0, Found = false ; SlotNum<ZVoxelExtension_Programmable::Storage_NumSlots ; SlotNum++)
  {
    if (S->Extension->VoxelType[SlotNum] == 0) { Found = true; break; }
  }
  if (!Found) { sq_pushbool(v,SQFalse); return(1); }

  S->GameEnv->World->GetVoxelLocation(&Loc, NewLocation.x, NewLocation.y, NewLocation.z );
  if (1!=S->GameEnv->VoxelTypeManager.VoxelTable[ Loc.Sector->Data[Loc.Offset] ]->Interface_PushBlock_Pull(&Loc,&VoxelType,1)) { sq_pushbool(v,SQFalse); return(1); };
  S->Extension->StoreBlocks(VoxelType,1);
  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_Display(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  const SQChar *  Inval;
  SQInteger Inval2, Inval3, Inval4;

  // Parameter input


  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_STRING) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getstring(v,2, &Inval);
  Inval2 = 5000;
  if (ArgCount>2)
  {
    if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
    sq_getinteger(v,3,&Inval2);
    if (Inval2 <0) Inval2= 0;
    if (Inval2 >10000) Inval2 = 10000;
  }
  Inval3 = 2;
  if (ArgCount >3)
  {
    if (sq_gettype(v,4) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
    if (ArgCount>3) sq_getinteger(v,4,&Inval3);
    if (Inval3 <0) Inval3= 0;
    if (Inval3 >5) Inval3= 5;
  }
  Inval4 = Inval2;
  if (ArgCount >4)
  {
    if (sq_gettype(v,5) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
    if (ArgCount>3) sq_getinteger(v,5,&Inval4);
    if (Inval4 > Inval2) Inval4 = Inval2;
  }

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  S->GameEnv->GameWindow_Advertising->Advertise(Inval,Inval3,0,Inval2,Inval4);

  sq_pushbool(v,SQTrue);
  return(1);
}


SQInteger function_GetQuantity(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  ULong SlotNum;

  SQInteger InVal, Quantity;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &InVal);
  InVal &= 0xffff;

  S = (ZStoreSq3 *)sq_getforeignptr(v);

  // Find the voxel type in the robot storage.
  for (SlotNum=Quantity=0; SlotNum<ZVoxelExtension_Programmable::Storage_NumSlots ; SlotNum++)
  {
    if (S->Extension->VoxelType[SlotNum] == InVal) Quantity+= S->Extension->VoxelQuantity[SlotNum];
  }
  sq_pushinteger(v,Quantity);
  return(1);
}

SQInteger function_GetSlot_Type(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQInteger InVal;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &InVal);
  if (InVal < 0 || InVal >= ZVoxelExtension_Programmable::Storage_NumSlots ) { sq_pushinteger(v,-1); return(1); }

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if (S->Extension->VoxelQuantity[InVal] > 0) sq_pushinteger(v,S->Extension->VoxelType[InVal]);
  else                                        sq_pushinteger(v,0);
  return(1);
}

SQInteger function_GetSlot_Quantity(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQInteger InVal;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &InVal);
  if (InVal < 0 || InVal >= ZVoxelExtension_Programmable::Storage_NumSlots ) { sq_pushinteger(v,0); return(1); }

  S = (ZStoreSq3 *)sq_getforeignptr(v);

  sq_pushinteger(v,S->Extension->VoxelQuantity[InVal]);
  return(1);
}

SQInteger function_GetFirstUsedSlot(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger i;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<1) return sq_throwerror(v,"invalid parameter count"); //throws an exception

  S = (ZStoreSq3 *)sq_getforeignptr(v);

  for (i=0;i<ZVoxelExtension_Programmable::Storage_NumSlots;i++)
  {
    if (S->Extension->VoxelQuantity[i]>0 && S->Extension->VoxelType!=0) {sq_pushinteger(v,i); return(1);}
  }

  sq_pushinteger(v,-1);
  return(1);
}

SQInteger function_Unf_SetSlot(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQInteger Type,Quantity, Slot;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<4) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  sq_getinteger(v,2, &Slot);
  sq_getinteger(v,3, &Type);
  sq_getinteger(v,4, &Quantity);

  S = (ZStoreSq3 *)sq_getforeignptr(v);

  if (Slot >= ZVoxelExtension_Programmable::Storage_NumSlots) {sq_pushbool(v,SQFalse); return(1);}

  S->Extension->VoxelType[Slot] = (UShort) Type;
  S->Extension->VoxelQuantity[Slot] = Quantity;

  sq_pushbool(v,SQTrue);
  return(1);
}

bool GetVarInSQStack(HSQUIRRELVM v, SQInteger StackPosition, ZVar * Var)
{
  switch(sq_gettype(v,StackPosition))
  {
    case OT_NULL:     return(false); break;
    case OT_INTEGER:  {SQInteger i; sq_getinteger(v,StackPosition, &i);Var->Set_Int(i);} break;
    case OT_FLOAT:    {SQFloat i; sq_getfloat(v,StackPosition, &i);Var->Set_Double(i);} break;
    case OT_STRING:   {SQChar * String; sq_getstring(v,StackPosition, (const SQChar **)&String); Var->Set_CString(String); } break;
    case OT_BOOL:     {SQBool i; sq_getbool(v,StackPosition, &i);Var->Set_Bool(i);} break;
    case OT_TABLE:
    case OT_ARRAY:
    case OT_USERDATA:
    case OT_CLOSURE:
    case OT_NATIVECLOSURE:
    case OT_GENERATOR:
    case OT_USERPOINTER:
    case OT_CLASS:
    case OT_INSTANCE:
    case OT_WEAKREF:
    default:           return(false); break;
  }
  return(true);
}

bool PutVarToSQStack(HSQUIRRELVM v, ZVar * Var)
{
  switch(Var->VarType)
  {
    case ZVar::TYPE_BOOL: sq_pushbool(v, (Var->Scalar.Bool) ? 1:0 ); break;
    case ZVar::TYPE_INT:  sq_pushinteger(v, Var->Scalar.Int); break;
    case ZVar::TYPE_DOUBLE:sq_pushfloat(v, Var->Scalar.doublefloat); break;
    case ZVar::TYPE_ZSTRING: sq_pushstring(v, Var->String.String, Var->String.Len); break;
    default: return(false); break;
  }
  return(true);
}

SQInteger function_voxel_getinfo(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQInteger x,y,z,Index;
  UShort VoxelType;
  ZVector3L NewLocation;
  ZVoxelLocation Loc;
  ZVar Var;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<5) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  if (sq_gettype(v,5) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 4"); //throws an exception
  sq_getinteger(v,2, &x);
  sq_getinteger(v,3, &y);
  sq_getinteger(v,4, &z);
  sq_getinteger(v,5, &Index);

  // Get access to informations
  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if (!S->Allow_WorldManipulations) { sq_pushbool(v,SQFalse); return(1); }

  // Compute communication location.
  NewLocation = S->VoxelPosition;
  NewLocation.x += x; NewLocation.y += y; NewLocation.z += z;

  // Get the Voxel Location

  if (!S->GameEnv->World->GetVoxelLocation(&Loc, NewLocation.x, NewLocation.y, NewLocation.z )) return(false);

  // Get the Voxel type

  VoxelType = Loc.Sector->Data[Loc.Offset];

  // Call getinfo interface

  if (!S->GameEnv->VoxelTypeManager.VoxelTable[ VoxelType ]->Interface_GetInfo(&Loc, Index, & Var)) { sq_pushbool(v,SQFalse); return(1); }

  // Return result to stack

  if (!PutVarToSQStack(v,&Var)) { sq_pushbool(v,SQFalse); return(1); }
  return(1);
}

SQInteger function_voxel_setinfo(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQInteger x,y,z,Index;
  UShort VoxelType;
  ZVector3L NewLocation;
  ZVoxelLocation Loc;
  ZVar Var;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<6) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  if (sq_gettype(v,5) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 4"); //throws an exception
  sq_getinteger(v,2, &x);
  sq_getinteger(v,3, &y);
  sq_getinteger(v,4, &z);
  sq_getinteger(v,5, &Index);
  if (!GetVarInSQStack(v,6,&Var)) return sq_throwerror(v,"invalid parameter type for argument 5"); //throws an exception

  // Get access to informations
  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if (!S->Allow_WorldManipulations) { sq_pushbool(v,SQFalse); return(1); }

  // Compute communication location.
  NewLocation = S->VoxelPosition;
  NewLocation.x += x; NewLocation.y += y; NewLocation.z += z;

  // Get the Voxel Location

  if (!S->GameEnv->World->GetVoxelLocation(&Loc, NewLocation.x, NewLocation.y, NewLocation.z )) return(false);

  // Get the Voxel type

  VoxelType = Loc.Sector->Data[Loc.Offset];

  // Call getinfo interface

  if (!S->GameEnv->VoxelTypeManager.VoxelTable[ VoxelType ]->Interface_SetInfo(&Loc, Index, & Var)) { sq_pushbool(v,SQFalse); return(1); }

  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_voxel_getinfodoc(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQInteger Index, DocType,tmp;
  UShort VoxelType;
  ZVar Var;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<4) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  sq_getinteger(v,2, &tmp); VoxelType = tmp;
  sq_getinteger(v,3, &Index);
  sq_getinteger(v,4, &DocType);

  // Get access to informations
  S = (ZStoreSq3 *)sq_getforeignptr(v);
  if (!S->Allow_WorldManipulations) { sq_pushbool(v,SQFalse); return(1); }

  // Call getinfo interface

  if (!S->GameEnv->VoxelTypeManager.VoxelTable[ VoxelType ]->Interface_GetInfoDoc(Index, DocType, & Var)) { sq_pushbool(v,SQFalse); return(1); }

  // Return result to stack

  if (!PutVarToSQStack(v,&Var)) { sq_pushbool(v,SQFalse); return(1); }
  return(1);
}

SQInteger function_GetFontPixel(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger c, x, y, RetVal;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<4) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  sq_getinteger(v,2, &c);
  sq_getinteger(v,3, &x);
  sq_getinteger(v,4, &y);

  // Input Validation

  if (    (c < 0 || c > 255)
       || (x > 7 || x < 0)
       || (y > 7 || y < 0)  ){ sq_pushinteger(v,0); return(1); }

  S = (ZStoreSq3 *)sq_getforeignptr(v);

  RetVal = S->GameEnv->Font_1->GetTilePixel( (UByte)c, (ULong)x, (ULong)y );

  sq_pushinteger(v,RetVal);
  return(1);
}

SQInteger function_Image_New(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger ImageNum, Width, Height;
  ZGenericCanva<ULong> * Image;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<4) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  sq_getinteger(v,2, &ImageNum);
  sq_getinteger(v,3, &Width);
  sq_getinteger(v,4, &Height);

  // Input Validation

  if ( (ImageNum < 0 || ImageNum >= 64)
       || (Width < 1)
       || (Height < 1)  ) { sq_pushbool(v,SQFalse); return(1); }

  S = (ZStoreSq3 *)sq_getforeignptr(v);

  // Delete existing image

  Image = S->Extension->ImageTable[ImageNum];
  if (Image) { delete Image; S->Extension->ImageTable[ImageNum] = 0;}

  // Create the new image

  Image = new ZGenericCanva<ULong>(Width,Height); if (!Image) { sq_pushbool(v,SQFalse); return(1); }
  Image->Clear(0);
  S->Extension->ImageTable[ImageNum] = Image;

  // Return Success

  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_Image_Exists(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger ImageNum;
  ZGenericCanva<ULong> * Image;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &ImageNum);

  // Input Validation

  if (ImageNum < 0 || ImageNum >= 64) { sq_pushbool(v,SQFalse); return(1); }

  // Retrieve the image

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  Image = S->Extension->ImageTable[ImageNum];


  // Return Success

  sq_pushbool(v, (Image) ? SQTrue : SQFalse );
  return(1);
}

SQInteger function_Image_GetPixel(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger ImageNum, x, y, RetVal;
  ZGenericCanva<ULong> * Image;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<4) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  sq_getinteger(v,2, &ImageNum);
  sq_getinteger(v,3, &x);
  sq_getinteger(v,4, &y);

  // Input validation

  if ( ImageNum < 0 || ImageNum >= 64) { sq_pushbool(v,SQFalse); return(1); }

  // Get Working image

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  Image = S->Extension->ImageTable[ImageNum]; if (!Image) { sq_pushbool(v,SQFalse); return(1); }

  // Get Pixel Information

  RetVal = Image->GetPoint_Secure(x,y);

  // Return result

  sq_pushinteger(v,RetVal);
  return(1);
}

SQInteger function_Image_SetPixel(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger ImageNum, x, y, Color;
  ZGenericCanva<ULong> * Image;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<5) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  if (sq_gettype(v,5) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 4"); //throws an exception
  sq_getinteger(v,2, &ImageNum);
  sq_getinteger(v,3, &x);
  sq_getinteger(v,4, &y);
  sq_getinteger(v,5, &Color);

  // Input validation

  if ( (ImageNum < 0 || ImageNum >= 64) || (x < 0) || (y < 0)  ) { sq_pushbool(v,SQFalse); return(1); }

  // Get Working image

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  Image = S->Extension->ImageTable[ImageNum]; if (!Image) { sq_pushbool(v,SQFalse); return(1); }

  // Set Pixel Information

  Image->SetPoint_Secure(x,y,Color);

  // Return result

  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_Image_Line(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger ImageNum, sx, sy, ex, ey, Color;
  ZGenericCanva<ULong> * Image;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<7) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  if (sq_gettype(v,5) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 4"); //throws an exception
  if (sq_gettype(v,6) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 5"); //throws an exception
  if (sq_gettype(v,7) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 6"); //throws an exception
  sq_getinteger(v,2, &ImageNum);
  sq_getinteger(v,3, &sx);
  sq_getinteger(v,4, &sy);
  sq_getinteger(v,5, &ex);
  sq_getinteger(v,6, &ey);
  sq_getinteger(v,7, &Color);

  // Input validation

  if ( (ImageNum < 0 || ImageNum >= 64) ) { sq_pushbool(v,SQFalse); return(1); }

  // Get Working image

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  Image = S->Extension->ImageTable[ImageNum]; if (!Image) { sq_pushbool(v,SQFalse); return(1); }

  // Set Pixel Information

  ZLineCoords Lc;
  Lc.Start.x = sx; Lc.Start.y = sy; Lc.End.x = ex; Lc.End.y = ey;

  /*
  Image->Polygon_Start();
  Image->Polygon_Segment(&Lc);
  Image->Polygon_Render(Color);
  */
  Image->Line(&Lc, Color);


  // Return result

  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_Image_Clear(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger ImageNum, Color;
  ZGenericCanva<ULong> * Image;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<3) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  sq_getinteger(v,2, &ImageNum);
  sq_getinteger(v,3, &Color);

  // Input validation

  if ( (ImageNum < 0 || ImageNum >= 64) ) { sq_pushbool(v,SQFalse); return(1); }

  // Get Working image

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  Image = S->Extension->ImageTable[ImageNum]; if (!Image) { sq_pushbool(v,SQFalse); return(1); }

  // Clear Image

  Image->Clear(Color);

  // Return result

  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_Image_Polygon_Start(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger ImageNum;
  ZGenericCanva<ULong> * Image;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &ImageNum);

  // Input validation

  if ( (ImageNum < 0 || ImageNum >= 64) ) { sq_pushbool(v,SQFalse); return(1); }

  // Get Working image

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  Image = S->Extension->ImageTable[ImageNum]; if (!Image) { sq_pushbool(v,SQFalse); return(1); }

  // Init Poligon Drawing

  Image->Polygon_Start();

  // Return result

  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_Image_Polygon_Segment(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger ImageNum, sx, sy, ex, ey;
  ZGenericCanva<ULong> * Image;
  ZLineCoords Lc;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  if (sq_gettype(v,5) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 4"); //throws an exception
  if (sq_gettype(v,6) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 5"); //throws an exception
  sq_getinteger(v,2, &ImageNum);
  sq_getinteger(v,3, &sx);
  sq_getinteger(v,4, &sy);
  sq_getinteger(v,5, &ex);
  sq_getinteger(v,6, &ey);

  // Input validation

  if ( (ImageNum < 0 || ImageNum >= 64) ) { sq_pushbool(v,SQFalse); return(1); }

  // Get Working image

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  Image = S->Extension->ImageTable[ImageNum]; if (!Image) { sq_pushbool(v,SQFalse); return(1); }

  // Draw Polygon segment

  Lc.Start.x = sx;
  Lc.Start.y = sy;
  Lc.End.x = ex;
  Lc.End.y = ey;
  Image->Polygon_Segment(&Lc);

  // Return result

  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_Image_Polygon_Render(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger ImageNum, Color;
  ZGenericCanva<ULong> * Image;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  sq_getinteger(v,2, &ImageNum);
  sq_getinteger(v,3, &Color);

  // Input validation

  if ( (ImageNum < 0 || ImageNum >= 64) ) { sq_pushbool(v,SQFalse); return(1); }

  // Get Working image

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  Image = S->Extension->ImageTable[ImageNum]; if (!Image) { sq_pushbool(v,SQFalse); return(1); }

  // Draw Polygon segment

  Image->Polygon_Render(Color);

  // Return result

  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_Image_Circle_Filled(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger ImageNum, Color, Intval;
  double   x,y, Radius;
  ZGenericCanva<ULong> * Image;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  if (sq_gettype(v,5) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 4"); //throws an exception
  if (sq_gettype(v,6) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 5"); //throws an exception
  sq_getinteger(v,2, &ImageNum);
  sq_getinteger(v,3, &Intval); x = Intval; x+=0.5;
  sq_getinteger(v,4, &Intval); y = Intval; y+=0.5;
  sq_getinteger(v,5, &Intval); Radius = Intval;
  sq_getinteger(v,6, &Color);

  // Input validation

  if ( (ImageNum < 0 || ImageNum >= 64) ) { sq_pushbool(v,SQFalse); return(1); }

  // Get Working image

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  Image = S->Extension->ImageTable[ImageNum]; if (!Image) { sq_pushbool(v,SQFalse); return(1); }

  // Draw Polygon segment

  Image->DrawCircleFilled(x,y,Radius,Color,1.0);

  // Return result

  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_Image_DrawBox(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger ImageNum, sx, sy, ex, ey, Color;
  ZGenericCanva<ULong> * Image;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  if (sq_gettype(v,4) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 3"); //throws an exception
  if (sq_gettype(v,5) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 4"); //throws an exception
  if (sq_gettype(v,6) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 5"); //throws an exception
  if (sq_gettype(v,7) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 6"); //throws an exception
  sq_getinteger(v,2, &ImageNum);
  sq_getinteger(v,3, &sx);
  sq_getinteger(v,4, &sy);
  sq_getinteger(v,5, &ex);
  sq_getinteger(v,6, &ey);
  sq_getinteger(v,7, &Color);

  // Input validation

  if ( (ImageNum < 0 || ImageNum >= 64) ) { sq_pushbool(v,SQFalse); return(1); }

  // Get Working image

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  Image = S->Extension->ImageTable[ImageNum]; if (!Image) { sq_pushbool(v,SQFalse); return(1); }

  // Draw Polygon segment

  Image->DrawBox(sx,sy,ex,ey,Color);

  // Return result

  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_Image_Load(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  const SQChar * Filename;
  SQInteger ImageNum;
  ZGenericCanva<ULong> * Image;
  ZBitmapImage Bitmap;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  if (sq_gettype(v,3) != OT_STRING) return sq_throwerror(v,"invalid parameter type for argument 2"); //throws an exception
  sq_getinteger(v,2, &ImageNum);
  sq_getstring(v,3, &Filename);

  // Input validation

  if ( (ImageNum < 0 || ImageNum >= 64) ) { sq_pushbool(v,SQFalse); return(1); }

  // Get Working image

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  Image = S->Extension->ImageTable[ImageNum];

  // Load File

  if (!Bitmap.LoadBMP(Filename)) { sq_pushbool(v,SQFalse); return(1); }
  if (Bitmap.Width < 1 || Bitmap.Height < 1) return sq_throwerror(v,"Invalid image size in Image_Load function"); //throws an exception
  if (Bitmap.BitsPerPixel!= 32 && Bitmap.BitsPerPixel) return sq_throwerror(v,"Invalid image color format in Image_Load function"); //throws an exception

  // Delete existing image

  if (Image) { delete Image; S->Extension->ImageTable[ImageNum] = 0;}

  // Create the new image

  Image = new ZGenericCanva<ULong>(Bitmap.Width,Bitmap.Height); if (!Image) { sq_pushbool(v,SQFalse); return(1); }
  Image->GetFromBitmap(&Bitmap,false,0);
  S->Extension->ImageTable[ImageNum] = Image;

  // Return Success

  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_GetKey(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger Keycode;
  ZString Nm;

  SQInteger ArgCount = sq_gettop(v); if (ArgCount < 2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &Keycode);

  // Get the context pointer
  S = (ZStoreSq3 *)sq_getforeignptr(v);

  // Return the key state

  sq_pushbool(v, S->GameEnv->EventManager.Is_KeyPressed(Keycode, false) ? SQTrue : SQFalse );
  return(1);
}

SQInteger function_Terminate(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQInteger RetVal;

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  RetVal = S->Extension->IsAllowedToRun ? 1 : 0;

  S->Extension->IsAllowedToRun=false;

  sq_pushinteger(v,RetVal);
  return(1);
}

SQInteger function_UnTerminate(HSQUIRRELVM v)
{
  ZStoreSq3 * S;

  SQInteger RetVal;

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  RetVal = S->Extension->IsAllowedToRun ? 1 : 0;

  S->Extension->IsAllowedToRun=true;

  sq_pushinteger(v,RetVal);
  return(1);
}

SQInteger function_Overclock(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger OverclockFactor;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &OverclockFactor);

  // Input Validation

  if (OverclockFactor < 0 || OverclockFactor >= 16) { sq_pushbool(v,SQFalse); return(1); }

  // Change the overclock factor

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  S->Extension->Overclock = OverclockFactor;

  // Return Success

  sq_pushbool(v,SQTrue);
  return(1);
}

SQInteger function_GetFab_Find(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger VoxelType, FabCount, Result;
  ZVoxelTypeManager * Vtm;
  ZVoxelType * Vt;
  ZFabInfos::ZTransformation * Transformation;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<3) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &VoxelType);
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,3, &FabCount);



  S = (ZStoreSq3 *)sq_getforeignptr(v);
  // Input Validation

  ULong VoxelTypeIndex, FabEntryIndex, ResultIndex;
  Vtm = &S->GameEnv->VoxelTypeManager;

  for (VoxelTypeIndex=0;VoxelTypeIndex<32768;VoxelTypeIndex++)
  {
    Vt = Vtm->VoxelTable[VoxelTypeIndex];
    if (!Vt) { sq_pushinteger(v,0); return(1); }
    if (Vt->Is_NoType && VoxelTypeIndex!=50) { sq_pushinteger(v,0); return(1); }

    if (Vt->FabInfos)
    {
      for (FabEntryIndex=0; FabEntryIndex < Vt->FabInfos->TransformationCount; FabEntryIndex++)
      {
        Transformation = &Vt->FabInfos->TransformationTable[FabEntryIndex];

        for (ResultIndex=0; ResultIndex < Transformation->Max_ResultVoxels; ResultIndex++)
        {
          if (!Transformation->Result_VoxelType[ResultIndex]) break;
          if (Transformation->Result_VoxelType[ResultIndex] == VoxelType)
          {
            if (FabCount) {FabCount--;break;}
            else
            {
              Result = VoxelTypeIndex;
              Result |= (FabEntryIndex<<16);

              //printf("Found : Fab %d Ind %d\n",(int)Result & 0xFFFF, (int)Result >> 16);
              sq_pushinteger(v,Result);
              return(1);
            }
          }


        }

      }
    }
  }


  // Return Success

  sq_pushinteger(v,0);
  return(1);
}

SQInteger Function_GetFab_ListType(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger MachineRef, Index;
  ZVoxelTypeManager * Vtm;
  ZVoxelType * Vt;
  ZFabInfos::ZTransformation * Transformation;
  UShort MachineType;
  ULong  EntryIndex;
  UShort VoxelType;


  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<3) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &MachineRef);
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,3, &Index);



  S = (ZStoreSq3 *)sq_getforeignptr(v);
  // Input Validation

  Vtm = &S->GameEnv->VoxelTypeManager;

  MachineType = MachineRef & 0xffff;
  EntryIndex  = MachineRef >> 16;

  Vt = Vtm->VoxelTable[MachineType];

  // Verification of the machine reference

  if (!Vt) {sq_pushinteger(v,0); return(1);}
  if (!Vt->FabInfos) {sq_pushinteger(v,0); return(1);}
  if (EntryIndex >= Vt->FabInfos->TransformationCount) {sq_pushinteger(v,0); return(1);}

  // Get info

  Transformation = &Vt->FabInfos->TransformationTable[EntryIndex];
  if (Index >= Transformation->EntryCount) {sq_pushinteger(v,0);return(1);}
  VoxelType = Vt->FabInfos->MaterialTable[ Transformation->FabList[Index].Index ].VoxelType;
  sq_pushinteger(v, VoxelType);

  return(1);
}

SQInteger Function_GetFab_ListQuantity(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger MachineRef, Index;
  ZVoxelTypeManager * Vtm;
  ZVoxelType * Vt;
  ZFabInfos::ZTransformation * Transformation;
  UShort MachineType;
  ULong  EntryIndex;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<3) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &MachineRef);
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,3, &Index);



  S = (ZStoreSq3 *)sq_getforeignptr(v);
  // Input Validation

  Vtm = &S->GameEnv->VoxelTypeManager;

  MachineType = MachineRef & 0xffff;
  EntryIndex  = MachineRef >> 16;

  Vt = Vtm->VoxelTable[MachineType];

  // Verification of the machine reference

  if (!Vt) {sq_pushinteger(v,0); return(1);}
  if (!Vt->FabInfos) {sq_pushinteger(v,0); return(1);}
  if (EntryIndex >= Vt->FabInfos->TransformationCount) {sq_pushinteger(v,0); return(1);}

  // Get info

  Transformation = &Vt->FabInfos->TransformationTable[EntryIndex];
  if (Index >= Transformation->EntryCount) {sq_pushinteger(v,0);return(1);}
  sq_pushinteger(v,Transformation->FabList[Index].Quantity);

  return(1);
}

SQInteger Function_GetFab_ResultType(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger MachineRef, Index;
  ZVoxelTypeManager * Vtm;
  ZVoxelType * Vt;
  ZFabInfos::ZTransformation * Transformation;
  UShort MachineType;
  ULong  EntryIndex;
  UShort VoxelType;


  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<3) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &MachineRef);
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,3, &Index);



  S = (ZStoreSq3 *)sq_getforeignptr(v);
  // Input Validation

  Vtm = &S->GameEnv->VoxelTypeManager;

  MachineType = MachineRef & 0xffff;
  EntryIndex  = MachineRef >> 16;

  Vt = Vtm->VoxelTable[MachineType];

  // Verification of the machine reference

  if (!Vt) {sq_pushinteger(v,0); return(1);}
  if (!Vt->FabInfos) {sq_pushinteger(v,0); return(1);}
  if (EntryIndex >= Vt->FabInfos->TransformationCount) {sq_pushinteger(v,0); return(1);}

  // Get info

  Transformation = &Vt->FabInfos->TransformationTable[EntryIndex];
  if (Index >= Transformation->Max_ResultVoxels) {sq_pushinteger(v,0);return(1);}
  VoxelType = Transformation->Result_VoxelType[Index];
  sq_pushinteger(v, VoxelType);

  return(1);
}

SQInteger Function_GetFab_ResultQuantity(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger MachineRef, Index;
  ZVoxelTypeManager * Vtm;
  ZVoxelType * Vt;
  ZFabInfos::ZTransformation * Transformation;
  UShort MachineType;
  ULong  EntryIndex;
  UShort VoxelType;


  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<3) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &MachineRef);
  if (sq_gettype(v,3) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,3, &Index);

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  // Input Validation

  Vtm = &S->GameEnv->VoxelTypeManager;

  MachineType = MachineRef & 0xffff;
  EntryIndex  = MachineRef >> 16;

  Vt = Vtm->VoxelTable[MachineType];

  // Verification of the machine reference

  if (!Vt) {sq_pushinteger(v,0); return(1);}
  if (!Vt->FabInfos) {sq_pushinteger(v,0); return(1);}
  if (EntryIndex >= Vt->FabInfos->TransformationCount) {sq_pushinteger(v,0); return(1);}

  // Get info

  Transformation = &Vt->FabInfos->TransformationTable[EntryIndex];
  if (Index >= Transformation->Max_ResultVoxels) {sq_pushinteger(v,0);return(1);}
  VoxelType = Transformation->Result_Quantity[Index];
  sq_pushinteger(v, VoxelType);

  return(1);
}

SQInteger Function_GetFab_ListCount(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger MachineRef;
  ZVoxelTypeManager * Vtm;
  ZVoxelType * Vt;
  ZFabInfos::ZTransformation * Transformation;
  UShort MachineType;
  ULong  EntryIndex;


  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &MachineRef);

  S = (ZStoreSq3 *)sq_getforeignptr(v);
  // Input Validation

  Vtm = &S->GameEnv->VoxelTypeManager;

  MachineType = MachineRef & 0xffff;
  EntryIndex  = MachineRef >> 16;

  Vt = Vtm->VoxelTable[MachineType];

  // Verification of the machine reference

  if (!Vt) {sq_pushinteger(v,0); return(1);}
  if (!Vt->FabInfos) {sq_pushinteger(v,0); return(1);}
  if (EntryIndex >= Vt->FabInfos->TransformationCount) {sq_pushinteger(v,0); return(1);}

  // Get info

  Transformation = &Vt->FabInfos->TransformationTable[EntryIndex];

  sq_pushinteger(v, Transformation->EntryCount);

  return(1);
}

SQInteger Function_GetFab_ResultCount(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger MachineRef, Index;
  ZVoxelTypeManager * Vtm;
  ZVoxelType * Vt;
  ZFabInfos::ZTransformation * Transformation;
  UShort MachineType;
  ULong  EntryIndex;


  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &MachineRef);


  S = (ZStoreSq3 *)sq_getforeignptr(v);
  // Input Validation

  Vtm = &S->GameEnv->VoxelTypeManager;

  MachineType = MachineRef & 0xffff;
  EntryIndex  = MachineRef >> 16;

  Vt = Vtm->VoxelTable[MachineType];

  // Verification of the machine reference

  if (!Vt) {sq_pushinteger(v,0); return(1);}
  if (!Vt->FabInfos) {sq_pushinteger(v,0); return(1);}
  if (EntryIndex >= Vt->FabInfos->TransformationCount) {sq_pushinteger(v,0); return(1);}

  // Get info

  Transformation = &Vt->FabInfos->TransformationTable[EntryIndex];
  for (Index=0;Index < Transformation->Max_ResultVoxels; Index++)
  {
    if (Transformation->Result_VoxelType[Index]==0) {sq_pushinteger(v, Index); return(1); }
  }

  sq_pushinteger(v,Transformation->Max_ResultVoxels);

  return(1);
}

SQInteger Function_GetFab_Machine(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger MachineRef;
  ZVoxelTypeManager * Vtm;
  ZVoxelType * Vt;
  UShort MachineType;
  ULong  EntryIndex;


  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &MachineRef);


  S = (ZStoreSq3 *)sq_getforeignptr(v);
  // Input Validation

  Vtm = &S->GameEnv->VoxelTypeManager;

  MachineType = MachineRef & 0xffff;
  EntryIndex  = MachineRef >> 16;

  Vt = Vtm->VoxelTable[MachineType];

  // Verification of the machine reference

  if (!Vt) {sq_pushinteger(v,0); return(1);}
  if (!Vt->FabInfos) {sq_pushinteger(v,0); return(1);}
  if (EntryIndex >= Vt->FabInfos->TransformationCount) {sq_pushinteger(v,0); return(1);}

  // Return the machine type

  sq_pushinteger(v,MachineType);
  return(1);
}


SQInteger function_GetFab_Count(HSQUIRRELVM v)
{
  ZStoreSq3 * S;
  SQInteger VoxelType, FabCount;
  ZVoxelTypeManager * Vtm;
  ZVoxelType * Vt;
  ZFabInfos::ZTransformation * Transformation;

  // Parameter input

  SQInteger ArgCount = sq_gettop(v); if (ArgCount<2) return sq_throwerror(v,"invalid parameter count"); //throws an exception
  if (sq_gettype(v,2) != OT_INTEGER) return sq_throwerror(v,"invalid parameter type for argument 1"); //throws an exception
  sq_getinteger(v,2, &VoxelType);



  S = (ZStoreSq3 *)sq_getforeignptr(v);
  // Input Validation

  ULong VoxelTypeIndex, FabEntryIndex, ResultIndex;
  Vtm = &S->GameEnv->VoxelTypeManager;

  FabCount = 0;
  for (VoxelTypeIndex=0;VoxelTypeIndex<32768;VoxelTypeIndex++)
  {
    Vt = Vtm->VoxelTable[VoxelTypeIndex];
    if (!Vt) { sq_pushinteger(v,0); return(1); }
    if (Vt->Is_NoType && VoxelTypeIndex!=50) { sq_pushinteger(v,FabCount); return(1); }
    if (Vt->FabInfos)
    {
      for (FabEntryIndex=0; FabEntryIndex < Vt->FabInfos->TransformationCount; FabEntryIndex++)
      {
        Transformation = &Vt->FabInfos->TransformationTable[FabEntryIndex];

        for (ResultIndex=0; ResultIndex < Transformation->Max_ResultVoxels; ResultIndex++)
        {

          if (!Transformation->Result_VoxelType[ResultIndex]) break;

          if (Transformation->Result_VoxelType[ResultIndex] == VoxelType)
          {
            FabCount++;
          }

        }

      }
    }
  }

  // Return Success

  sq_pushinteger(v,FabCount);
  return(1);
}


SQInteger print_args(HSQUIRRELVM v)
{
    SQInteger nargs = sq_gettop(v); //number of arguments
    for(SQInteger n=1;n<=nargs;n++)
    {
        printf("[arg %ld is ",(long int)n);
        switch(sq_gettype(v,n))
        {
            case OT_NULL:
                printf("null");
                break;
            case OT_INTEGER:
                printf("integer");
                break;
            case OT_FLOAT:
                printf("float");
                break;
            case OT_STRING:
                printf("string");
                break;
            case OT_TABLE:
                printf("table");
                break;
            case OT_ARRAY:
                printf("array");
                break;
            case OT_USERDATA:
                printf("userdata");
                break;
            case OT_CLOSURE:
                printf("closure(function)");
                break;
            case OT_NATIVECLOSURE:
                printf("native closure(C function)");
                break;
            case OT_GENERATOR:
                printf("generator");
                break;
            case OT_USERPOINTER:
                printf("userpointer");
                break;
            case OT_CLASS:
                printf("class");
                break;
            case OT_INSTANCE:
                printf("instance");
                break;
            case OT_WEAKREF:
                printf("weak reference");
                break;
            default:
                return sq_throwerror(v,"invalid param"); //throws an exception
        }
    }
    printf("\n");
    sq_pushinteger(v,nargs); //push the number of arguments as return value
    return 1; //1 because 1 value is returned
}

void RegisterSquirrelFunction(ZStoreSq3 * S, SQInteger (*function)(HSQUIRRELVM) , const char * FunctionName )
{
  sq_pushroottable(S->v);
  sq_pushstring(S->v,FunctionName,-1);
  sq_newclosure(S->v,function,0); //create a new function
  sq_newslot(S->v,-3,SQFalse);
  sq_pop(S->v,1); //pops the root table
}

bool ZScripting_Squirrel3::Init()
{
  // SQInteger top;

  // Create the storage structure

  IsReady = false;
  if (Ls) Cleanup();
  Ls = new ZStoreSq3;
  ZStoreSq3 * S = (ZStoreSq3 *)Ls;
  S->GameEnv = GameEnv;

  // Create Squirrel virtual machine instance

  S->v = sq_open(1024); // creates a VM with initial stack size 1024
  if (S->v == 0) return(false);

  // printf("Create %ul -> %ul\n",S,S->v);
  // Set error handlers

  sqstd_seterrorhandlers(S->v); //registers the default error handlers

  // Set print functions

  sq_setprintfunc(S->v, printfunc,errorfunc); //sets the print function


  // Store the pointer on info table in the Squirrel registry table.

  /*
  top = sq_gettop(S->v); //saves the stack size before the call
  sq_pushregistrytable(S->v);
  sq_pushinteger(S->v, 1536 );
  sq_pushuserpointer(S->v, S );
  if (!SQ_SUCCEEDED(sq_newslot(S->v, -3, SQFalse))) return(false);
  sq_settop(S->v, top);
*/
  sq_setforeignptr(S->v, S);

  // Register libraries

  sq_pushroottable(S->v);
#if COMPILEOPTION_SQUIRRELUNSAFE==1
  sqstd_register_mathlib(S->v);
  sqstd_register_bloblib(S->v);
  sqstd_register_systemlib(S->v);
  sqstd_register_iolib(S->v);
  sqstd_register_stringlib(S->v);
#endif

/*
  // Get the user pointer stored on info table stored in the Squirrel registry table.
  sq_pushregistrytable(S->v);
  sq_pushinteger(S->v, 1536);
  sq_get(S->v,-2);
  void * p;
  sq_getuserpointer(S->v,-1, &p);
*/

  // 2.0 extension

  RegisterSquirrelFunction(S, function_Terminate,            "Terminate");
  RegisterSquirrelFunction(S, function_UnTerminate,          "UnTerminate");
  RegisterSquirrelFunction(S, function_Overclock,            "Overclock");
  RegisterSquirrelFunction(S, function_GetFab_Find,          "GetFab_Find");
  RegisterSquirrelFunction(S, function_GetFab_Count,         "GetFab_Count");
  RegisterSquirrelFunction(S, Function_GetFab_ListType,      "GetFab_ListType");
  RegisterSquirrelFunction(S, Function_GetFab_ListQuantity,  "GetFab_ListQuantity");
  RegisterSquirrelFunction(S, Function_GetFab_ResultType,    "GetFab_ResultType");
  RegisterSquirrelFunction(S, Function_GetFab_ResultQuantity,"GetFab_ResultQuantity");
  RegisterSquirrelFunction(S, Function_GetFab_ListCount,     "GetFab_ListCount");
  RegisterSquirrelFunction(S, Function_GetFab_ResultCount,   "GetFab_ResultCount");
  RegisterSquirrelFunction(S, Function_GetFab_Machine,       "GetFab_Machine");

  // 1.36 extension

  RegisterSquirrelFunction(S, function_Image_New,            "Image_New");
  RegisterSquirrelFunction(S, function_Image_Clear,          "Image_Clear");
  RegisterSquirrelFunction(S, function_Image_Exists,         "Image_Exists");
  RegisterSquirrelFunction(S, function_Image_GetPixel,       "Image_GetPixel");
  RegisterSquirrelFunction(S, function_Image_SetPixel,       "Image_SetPixel");
  RegisterSquirrelFunction(S, function_Image_Line,           "Image_Line");
  RegisterSquirrelFunction(S, function_Image_Polygon_Start,  "Image_Polygon_Start");
  RegisterSquirrelFunction(S, function_Image_Polygon_Segment,"Image_Polygon_Segment");
  RegisterSquirrelFunction(S, function_Image_Polygon_Render, "Image_Polygon_Render");
  RegisterSquirrelFunction(S, function_Image_Circle_Filled,  "Image_Circle_Filled");
  RegisterSquirrelFunction(S, function_Image_DrawBox,        "Image_DrawBox");
  RegisterSquirrelFunction(S, function_Image_Load,           "Image_Load");
  RegisterSquirrelFunction(S, function_GetKey,               "GetKey");

  // 1.34 extension

  RegisterSquirrelFunction(S, function_GetFontPixel, "GetFontPixel");

  // 1.22 b3 extension
  RegisterSquirrelFunction(S, function_GetRobotLevel, "GetRobotLevel");
  RegisterSquirrelFunction(S, function_GetRobotID, "GetRobotID");
  RegisterSquirrelFunction(S, function_MovePlayer, "PlayerMove");
  RegisterSquirrelFunction(S, function_PlayerAddVelocity, "PlayerAddAccel"); // Obsolete
  RegisterSquirrelFunction(S, function_PlayerAddVelocity, "PlayerAddVelocity");
  RegisterSquirrelFunction(S, function_PlayerSetVelocity, "PlayerSetAccel"); // Obsolete
  RegisterSquirrelFunction(S, function_PlayerSetVelocity, "PlayerSetVelocity");
  RegisterSquirrelFunction(S, function_GetPath, "GetPath");
  RegisterSquirrelFunction(S, function_PlaceVoxel3D, "PlaceVoxel3D");
  RegisterSquirrelFunction(S, function_voxel_getinfo, "Voxel_GetInfo");
  RegisterSquirrelFunction(S, function_voxel_setinfo, "Voxel_SetInfo");
  RegisterSquirrelFunction(S, function_voxel_getinfodoc, "Voxel_GetInfoDoc");

  // 1.22 Extension
  RegisterSquirrelFunction(S, function_PlaceVoxel3D, "PlaceVoxel3D");
  RegisterSquirrelFunction(S, function_PickVoxel3D, "PickVoxel3D");
  RegisterSquirrelFunction(S, function_Look3D, "Look3D");
  RegisterSquirrelFunction(S, function_MoveVoxel3D, "MoveVoxel3D");
  RegisterSquirrelFunction(S, function_GetInfo, "GetInfo");
  RegisterSquirrelFunction(S, function_GetInfoName, "GetInfoName");

  RegisterSquirrelFunction(S, function_UnofficialFastPlot, "UNF_PlaceVoxel3D");
  RegisterSquirrelFunction(S, function_Unofficial_FastMoveVoxel3D, "UNF_MoveVoxel3D");
  RegisterSquirrelFunction(S, function_Unf_SetSlot, "UNF_SetSlot");

  // Functions
  RegisterSquirrelFunction(S, function_GetX, "GetX");
  RegisterSquirrelFunction(S, function_GetY, "GetY");
  RegisterSquirrelFunction(S, function_GetZ, "GetZ");
  RegisterSquirrelFunction(S, function_GetGameTime, "GetGameTime");
  RegisterSquirrelFunction(S, function_Look, "Look");
  RegisterSquirrelFunction(S, function_GetVoxelProp, "GetVoxelProp");
  RegisterSquirrelFunction(S, function_GetVoxelName, "GetVoxelName");
  RegisterSquirrelFunction(S, function_Move, "Move");
  RegisterSquirrelFunction(S, function_GetRnd, "GetRnd");
  RegisterSquirrelFunction(S, function_PickVoxel, "PickVoxel");
  RegisterSquirrelFunction(S, function_PlaceVoxel,"PlaceVoxel");
  RegisterSquirrelFunction(S, function_PushVoxelTo, "PushVoxelTo");
  RegisterSquirrelFunction(S, function_PullVoxelFrom, "PullVoxelFrom");
  RegisterSquirrelFunction(S, function_Display, "Display" );
  RegisterSquirrelFunction(S, function_GetQuantity, "GetQuantity" );
  RegisterSquirrelFunction(S, function_GetSlot_Type, "GetSlot_Type" );
  RegisterSquirrelFunction(S, function_GetSlot_Quantity, "GetSlot_Quantity" );
  RegisterSquirrelFunction(S, function_GetFirstUsedSlot,"GetFirstUsedSlot");

  RegisterSquirrelFunction(S, print_args, "print_args");

  IsReady = true;
  return(true);
}

void ZScripting_Squirrel3::Cleanup()
{
  ZStoreSq3 * S = (ZStoreSq3 *)Ls;
//  printf("Cleanup: %ul -> %ul\n",S,S->v);
  if (S)
  {
    if (S->v !=0) sq_close(S->v);
    S->v = 0;
    delete S;
    Ls = 0;
  }
  ScriptCompiledOK = false;
  IsReady = false;
}

bool ZScripting_Squirrel3::IsCompiledOk()
{
  return(ScriptCompiledOK);

}


void ZScripting_Squirrel3::RunScript(const char * FunctionName, bool AllowWorldManipulations, ULong LaunchContext)
{
    ZStoreSq3 * S = (ZStoreSq3 *)Ls;
    if (!S) return;
    if (!S->v) return;
    if (!ScriptCompiledOK) return;

    S->Allow_WorldManipulations = AllowWorldManipulations;
    S->LaunchContext = LaunchContext;
    S->HasMoved = false;
    SQInteger top = sq_gettop(S->v); //saves the stack size before the call
    sq_pushroottable(S->v); //pushes the global table
    sq_pushstring(S->v,_SC(FunctionName),-1);

    sq_setrunning(S->v,true); // Personnallized Squirrel function
    if(SQ_SUCCEEDED(sq_get(S->v,-2))) { //gets the field 'foo' from the global table
            sq_pushroottable(S->v); //push the 'this' (in this case is the global table)
            // sq_pushinteger(S->v,n);
            // sq_pushfloat(v,f);
            // sq_pushstring(v,s,-1);
            sq_call(S->v,1,SQFalse,SQTrue); //calls the function
    }
    sq_settop(S->v,top); //restores the original stack size
    sq_setrunning(S->v,false);
}

void ZScripting_Squirrel3::StopProgram()
{
  ZStoreSq3 * S = (ZStoreSq3 *)Ls;

  if (!S) return;
  if (!S->v) return;
  if (sq_isrunningprog(S->v)) sq_stoprunningprog(S->v);
}

bool ZScripting_Squirrel3::IsRunningProgram()
{
  ZStoreSq3 * S = (ZStoreSq3 *)Ls;

  if (!S) return(false);
  if (!S->v) return(false);
  return(sq_isrunningprog(S->v));
}

bool ZScripting_Squirrel3::LoadAndCompileScript()
{
  ZStoreSq3 * S;

  // Reinit virtual machine if there is yet script compiled OK in it.
  if (ScriptCompiledOK) {Cleanup(); Init();}
  S = (ZStoreSq3 *)Ls;
  sq_pushroottable(S->v);
  if(SQ_SUCCEEDED(sqstd_dofile(S->v, _SC(ScriptFilename.String), SQFalse, SQTrue))) // also prints syntax errors if any
  {
    this->ScriptCompiledOK = true;
    sq_pop(S->v,1);
    return(true);
  }
  sq_pop(S->v,1);
  return(false);
}

ZScripting_Squirrel3::ZScripting_Squirrel3()
{
  Ls = 0;
  ScriptCompiledOK = false;
  GameEnv = Ge;
  IsReady = false;
}

ZScripting_Squirrel3::~ZScripting_Squirrel3()
{
  Cleanup();
}

void ZScripting_Squirrel3::SetVoxelPosition(ZVector3L * VoxelPosition)
{
  ZStoreSq3 * S = (ZStoreSq3 *)Ls;
  if (S)
  {
    S->VoxelPosition = *VoxelPosition;
  }
}

void ZScripting_Squirrel3::SetExtension(ZVoxelExtension_Programmable * Extension)
{
  ZStoreSq3 * S = (ZStoreSq3 *)Ls;
  if (S)
  {
    S->Extension = Extension;
  }
}


