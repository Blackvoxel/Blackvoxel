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
#include "ZWorld.h"
#include "stdio.h"
#include "math.h"
#include "SDL/SDL.h"

// Sector deblocking coordinates

UByte STableX[] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2};
UByte STableZ[] = {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6};
UByte STableY[] = {9,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                   0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                   0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                   0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                   0,18};

UShort OfTableY []= {63, 0,1,2,3, 4,5,6,7, 8,9,10,11, 12,13,14,15,
                       16,17,18,19, 20,21,22,23, 24,25,26,27, 28,29,30,31,
                       32,33,34,35, 36,37,38,39, 40,41,42,43, 44,45,46,47,
                       48,49,50,51, 52,53,54,55, 56,57,58,59, 60,61,62,63,
                     0};
UShort OfTableX []= {  15*ZVOXELBLOCSIZE_Y,
                        0*ZVOXELBLOCSIZE_Y,  1*ZVOXELBLOCSIZE_Y,  2*ZVOXELBLOCSIZE_Y, 3*ZVOXELBLOCSIZE_Y,
                        4*ZVOXELBLOCSIZE_Y,  5*ZVOXELBLOCSIZE_Y,  6*ZVOXELBLOCSIZE_Y, 7*ZVOXELBLOCSIZE_Y,
                        8*ZVOXELBLOCSIZE_Y,  9*ZVOXELBLOCSIZE_Y, 10*ZVOXELBLOCSIZE_Y,11*ZVOXELBLOCSIZE_Y,
                       12*ZVOXELBLOCSIZE_Y, 13*ZVOXELBLOCSIZE_Y, 14*ZVOXELBLOCSIZE_Y,15*ZVOXELBLOCSIZE_Y,
                        0*ZVOXELBLOCSIZE_Y };
UShort OfTableZ []= {  15*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X,
                       0 *ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X, 1 *ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X, 2*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X, 3*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X,
                       4 *ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X, 5 *ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X, 6*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X, 7*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X,
                       8 *ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X, 9 *ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X,10*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X,11*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X,
                       12*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X, 13*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X,14*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X,15*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X,
                       0 *ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X};


ZVector3B ZVoxelWorld::ProxLoc[6];

ZVector3B_Init ProxLoc_Init[6]=
{
    { 0, 0, 1 }, // 0 = Front
    { 1, 0, 0 }, // 1 = Right
    { 0, 0,-1 }, // 2 = Back
    {-1, 0, 0 }, // 3 = Left
    { 0, 1, 0 }, // 4 = Above
    { 0,-1, 0 }  // 5 = Below
};


ZVoxelWorld::ZVoxelWorld()
{
  ZMemSize i;

  SectorEjectList = new ZSectorRingList(65536);

  SectorTable = 0;

  Size_x = 64;
  Size_y = 64;
  Size_z = 64;

  TableSize = Size_x * Size_y * Size_z;
  SectorTable = new ZVoxelSector * [TableSize];
  for(i=0;i<TableSize;i++) SectorTable[i]=0;

  WorkingFullSector    = new ZVoxelSector;
  WorkingFullSector->Fill(0x0001);
  WorkingEmptySector   = new ZVoxelSector;
  WorkingEmptySector->Fill(0);
  WorkingScratchSector = new ZVoxelSector;

  SectorLoader = 0;
  SectorList = 0;
  UniverseNum = 1;
  VoxelTypeManager = 0;

  for(i=0;i<6;i++) ProxLoc[i]=ProxLoc_Init[i];

}

ZVoxelWorld::~ZVoxelWorld()
{
  ZVoxelSector * Sector, * NewSector;

  Sector = SectorList;

  while (Sector)
  {
    NewSector = Sector->GlobalList_Next;
    if (COMPILEOPTION_ALLOWSAVE)
    {
#if COMPILEOPTION_SAVEONLYMODIFIED==1
      if (Sector->IsMustBeSaved())
#endif
      {
        Sector->Save(UniverseNum);
      }
    }
    delete Sector;
    Sector = NewSector;
  }

  if (SectorTable) { delete [] SectorTable; TableSize = 0; }


  if (WorkingFullSector)   { delete WorkingFullSector;    WorkingFullSector = 0;   }
  if (WorkingEmptySector)  { delete WorkingEmptySector;   WorkingEmptySector = 0;  }
  if (WorkingScratchSector){ delete WorkingScratchSector; WorkingScratchSector = 0;}
  SectorList = 0;
  UniverseNum = 0;
  if (SectorEjectList) delete SectorEjectList;
  SectorEjectList = 0;
}

ZVoxelSector * ZVoxelWorld::FindSector (Long x, Long y, Long z)
{
  Long xs,ys,zs, Offset;
  ZVoxelSector * SectorPointer;

// Unoptimized version
/*
  xs = x % Size_x;
  ys = y % Size_y;
  zs = z % Size_z;

  xs &= 0xff;
  ys &= 0xf;
  zs &= 0xff;

  Offset = xs + ys * Size_x + (zs * Size_x * Size_y);

*/

  xs = x & 63;
  ys = y & 15;
  zs = z & 63;

  Offset = xs + (ys << 6) + (zs << 12);

  SectorPointer = SectorTable[Offset];
  while (SectorPointer)
  {
    if ( (SectorPointer->Pos_x == x) && (SectorPointer->Pos_y == y) && (SectorPointer->Pos_z == z) ) return(SectorPointer);
    SectorPointer = SectorPointer->Next;
  }
  return(0);
}

ZVoxelSector * ZVoxelWorld::FindSector_Secure(Long x, Long y, Long z) // Create sector if not in memory.
{
  Long xs,ys,zs, Offset;
  ZVoxelSector * SectorPointer;

  xs = x % Size_x;
  ys = y % Size_y;
  zs = z % Size_z;

  xs &= 0xff;
  ys &= 0xf;
  zs &= 0xff;

  Offset = xs + ys * Size_x + (zs * Size_x * Size_y);

  while (true)
  {
    SectorPointer = SectorTable[Offset];
    while (SectorPointer)
    {
      if ( (SectorPointer->Pos_x == x) && (SectorPointer->Pos_y == y) && (SectorPointer->Pos_z == z) ) return(SectorPointer);
      SectorPointer = SectorPointer->Next;
    }
    RequestSector(x,y,z,5);
    SDL_Delay(2);
    ProcessNewLoadedSectors();
  }
  return(0);
}

void ZVoxelWorld::AddSector( ZVoxelSector * Sector )
{
  Long x,y,z,Offset;
  ZVoxelSector * SectorPointer;


  // Adding to fast access hash

  x = (Sector->Pos_x % Size_x) & 0xff;
  y = (Sector->Pos_y % Size_y) & 0xf;
  z = (Sector->Pos_z % Size_z) & 0xff;

  Offset = x + y * Size_x + (z * Size_x * Size_y);

  if ( SectorTable[Offset] == 0 )
  {
    SectorTable[Offset] = Sector; Sector->Next = 0; Sector->Pred = 0;
  }
  else
  {
    SectorPointer = SectorTable[Offset];
    while ( SectorPointer->Next ) SectorPointer = SectorPointer->Next;
    SectorPointer->Next = Sector;
    Sector->Next = 0;
    Sector->Pred = SectorPointer;
  }

  // Adding to sequential access global list

  if (SectorList == 0)
  {
    SectorList = Sector;
    Sector->GlobalList_Next = 0;
    Sector->GlobalList_Pred = 0;
  }
  else
  {
    Sector->GlobalList_Next = SectorList;
    Sector->GlobalList_Pred = 0;
    SectorList->GlobalList_Pred = Sector;
    SectorList = Sector;
  }
}


void ZVoxelWorld::RemoveSector( ZVoxelSector * Sector )
{
  Long x,y,z,Offset;
  ZVoxelSector * SectorPointer;

  // Finding sector in hash

  x = (Sector->Pos_x % Size_x) & 0xff;
  y = (Sector->Pos_y % Size_y) & 0xf;
  z = (Sector->Pos_z % Size_z) & 0xff;
  Offset = x + y * Size_x + (z * Size_x * Size_y);
  SectorPointer = SectorTable[Offset];
  while (SectorPointer != Sector)
  {
    if (!SectorPointer) return;
    SectorPointer=SectorPointer->Next;
  }

  // Remove from hash
  if (SectorPointer == SectorTable[Offset]) SectorTable[Offset] = Sector->Next;
  else                                      Sector->Pred->Next = Sector->Next;
  if (Sector->Next) Sector->Next->Pred = Sector->Pred;

  // Remove from global list

  if (Sector->GlobalList_Pred) Sector->GlobalList_Pred->GlobalList_Next = Sector->GlobalList_Next;
  else (SectorList = Sector->GlobalList_Next);
  if (Sector->GlobalList_Next) Sector->GlobalList_Next->GlobalList_Pred = Sector->GlobalList_Pred;

  // Zeroing fields

  Sector->Next = 0; Sector->Pred = 0; Sector->GlobalList_Next = 0; Sector->GlobalList_Pred = 0;

}


void ZVoxelWorld::RequestSector(Long x, Long y, Long z, Long Priority)
{
  if (!SectorLoader) return;

  SectorLoader->Request_Sector(x,y,z,Priority);
}

void ZVoxelWorld::ProcessNewLoadedSectors()
{
  ZVoxelSector * Sector, * AdjSector;
  if (!SectorLoader) return;

  while( (Sector = SectorLoader->GetRequested()) )
  {
    if   (!FindSector(Sector->Pos_x, Sector->Pos_y, Sector->Pos_z))
    {
      AddSector(Sector);
      // SectorUpdateFaceCulling(Sector->Pos_x, Sector->Pos_y, Sector->Pos_z, true);

      Sector->Flag_Void_Regular = false;
      Sector->Flag_Void_Transparent = false;
      Sector->Flag_Render_Dirty = true;
      //printf("AddSector: %ld,%ld,%ld\n",Sector->Pos_x, Sector->Pos_y, Sector->Pos_z);

      // Partial face culing for adjacent sectors

      AdjSector = FindSector(Sector->Pos_x-1, Sector->Pos_y, Sector->Pos_z); if (AdjSector) { AdjSector->PartialCulling |= DRAWFACE_LEFT;}
      AdjSector = FindSector(Sector->Pos_x+1, Sector->Pos_y, Sector->Pos_z); if (AdjSector) { AdjSector->PartialCulling |= DRAWFACE_RIGHT;}
      AdjSector = FindSector(Sector->Pos_x, Sector->Pos_y, Sector->Pos_z-1); if (AdjSector) { AdjSector->PartialCulling |= DRAWFACE_AHEAD;}
      AdjSector = FindSector(Sector->Pos_x, Sector->Pos_y, Sector->Pos_z+1); if (AdjSector) { AdjSector->PartialCulling |= DRAWFACE_BEHIND;}
      AdjSector = FindSector(Sector->Pos_x, Sector->Pos_y-1, Sector->Pos_z); if (AdjSector) { AdjSector->PartialCulling |= DRAWFACE_BELOW;}
      AdjSector = FindSector(Sector->Pos_x, Sector->Pos_y+1, Sector->Pos_z); if (AdjSector) { AdjSector->PartialCulling |= DRAWFACE_ABOVE;}

    }
    else { delete Sector; printf("Loading already used sector***\n"); }
  }
}

ULong debug_ejectpass = 0;

void ZVoxelWorld::ProcessOldEjectedSectors()
{
  ZVoxelSector * Sector;

  if (!SectorLoader) return;

  while ( SectorLoader->Is_EjectFileNotFull())
  {
    if (!(Sector = SectorEjectList->PullFromList())) break;



    // printf("EjectPass : %lx %lu\n",Sector,++debug_ejectpass);
    RemoveSector(Sector);
    if (Sector->DisplayData) {delete Sector->DisplayData; Sector->DisplayData = 0; }
    this->SectorLoader->Eject_Sector(Sector);
  }
}

void ZVoxelWorld::SetVoxelTypeManager( ZVoxelTypeManager * Manager )
{
  VoxelTypeManager = Manager;
}

void ZVoxelWorld::SetSectorLoader(ZSectorLoader * SectorLoader)
{
  this->SectorLoader = SectorLoader;
}

void ZVoxelWorld::SetVoxel(Long x, Long y, Long z, UShort VoxelValue)
{
  ZVoxelSector * Sector;

  Sector = FindSector( x>>ZVOXELBLOCSHIFT_X , y>>ZVOXELBLOCSHIFT_Y , z>>ZVOXELBLOCSHIFT_Z );

  if (!Sector) return;

  Sector->SetCube(x,y,z, VoxelValue);
}


void ZVoxelSector::DebugOut( const char * FileName )
{
  Long x,y,z;
  UShort Voxel;
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
        Voxel = Data[ y + ( x*Size_y )+ (z * (Size_y*Size_x)) ];
        if (Voxel == 0) Car = '.';
        else if (Voxel == 1) Car = '#';
        else Car = '@';
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

void ZVoxelWorld::CreateDemoWorld()
{
  Long x,y,z;
  ZVoxelSector * Sector;
return;
  for (x=-1;x<1;x++)
  {
    for (y=-1;y<1;y++)
    {
      for (z=-1;z<1;z++)
      {
        Sector = new ZVoxelSector();
        Sector->SetVoxelTypeManager(VoxelTypeManager);
        Sector->SetPos(x,y,z);
        Sector->MakeSector();

        AddSector(Sector);
      }
    }
  }
}

void ZVoxelWorld::WorldUpdateFaceCulling()
{
  ZVoxelSector * Sector;

  Sector = SectorList;

  while (Sector)
  {
    SectorUpdateFaceCulling(Sector->Pos_x, Sector->Pos_y, Sector->Pos_z );
    Sector = Sector->GlobalList_Next;
  }
}

void ZVoxelWorld::SectorUpdateFaceCulling2(Long x, Long y, Long z, bool Isolated)
{
  ZVoxelSector * SectorTable[27];
  ZVoxelType ** VoxelTypeTable;
  ZVoxelSector * MissingSector;

  UByte * BlocMatrix[3];
  void * tmpp;
  ULong i;

  BlocMatrix[0] = new UByte[9];
  BlocMatrix[1] = new UByte[9];
  BlocMatrix[2] = new UByte[9];


  if (Isolated) MissingSector = WorkingEmptySector;
  else          MissingSector = WorkingFullSector;

  for (i=0;i<27;i++) SectorTable[i] = MissingSector;
  SectorTable[0] = FindSector(x,y,z);    if (!SectorTable[0] ) {SectorTable[0]  = MissingSector;}
  SectorTable[1] = FindSector(x-1,y,z);  if (!SectorTable[1] ) {SectorTable[1]  = MissingSector;}
  SectorTable[2] = FindSector(x+1,y,z);  if (!SectorTable[2] ) {SectorTable[2]  = MissingSector;}
  SectorTable[3] = FindSector(x,y,z-1);  if (!SectorTable[3] ) {SectorTable[3]  = MissingSector;}
  SectorTable[6] = FindSector(x,y,z+1);  if (!SectorTable[6] ) {SectorTable[6]  = MissingSector;}
  SectorTable[9] = FindSector(x,y-1,z);  if (!SectorTable[9] ) {SectorTable[9]  = MissingSector;}
  SectorTable[18]= FindSector(x,y+1,z);  if (!SectorTable[18]) {SectorTable[18] = MissingSector;}


  Long xc,yc,zc;
  Long xp,yp,zp;
  Long xpp,ypp,zpp;
  UByte info;
  ZVoxelType * Vt[6];
  Bool TransparentVoxel;

  SectorTable[0]->Flag_Void_Regular = true;
  SectorTable[0]->Flag_Void_Transparent = true;
  VoxelTypeTable = this->VoxelTypeManager->VoxelTable;

  for ( xc=0 ; xc<ZVOXELBLOCSIZE_X ; xc++ )
  {
    xp = xc+1; xpp= xc+2;
    for ( zc=0 ; zc<ZVOXELBLOCSIZE_Z ; zc++ )
    {
      zp = zc+1;zpp=zc+2;

      // Prefetching the bloc matrix (only 2 rows)
      BlocMatrix[1][0] = SectorTable[(STableX[xc ]+STableY[0]+STableZ[zc ])]->Data[OfTableX[xc]+OfTableY[0]+OfTableZ[zc]];
      BlocMatrix[1][1] = SectorTable[(STableX[xp ]+STableY[0]+STableZ[zc ])]->Data[OfTableX[xp]+OfTableY[0]+OfTableZ[zc]];
      BlocMatrix[1][2] = SectorTable[(STableX[xpp]+STableY[0]+STableZ[zc ])]->Data[OfTableX[xpp]+OfTableY[0]+OfTableZ[zc]];
      BlocMatrix[1][3] = SectorTable[(STableX[xc ]+STableY[0]+STableZ[zp ])]->Data[OfTableX[xc]+OfTableY[0]+OfTableZ[zp]];
      BlocMatrix[1][4] = SectorTable[(STableX[xp ]+STableY[0]+STableZ[zp ])]->Data[OfTableX[xp]+OfTableY[0]+OfTableZ[zp]];
      BlocMatrix[1][5] = SectorTable[(STableX[xpp]+STableY[0]+STableZ[zp ])]->Data[OfTableX[xpp]+OfTableY[0]+OfTableZ[zp]];
      BlocMatrix[1][6] = SectorTable[(STableX[xc ]+STableY[0]+STableZ[zpp])]->Data[OfTableX[xc]+OfTableY[0]+OfTableZ[zpp]];
      BlocMatrix[1][7] = SectorTable[(STableX[xp ]+STableY[0]+STableZ[zpp])]->Data[OfTableX[xp]+OfTableY[0]+OfTableZ[zpp]];
      BlocMatrix[1][8] = SectorTable[(STableX[xpp]+STableY[0]+STableZ[zpp])]->Data[OfTableX[xpp]+OfTableY[0]+OfTableZ[zpp]];

      BlocMatrix[2][0] = SectorTable[(STableX[xc ]+STableY[1]+STableZ[zc ])]->Data[OfTableX[xc ]+OfTableY[1]+OfTableZ[zc ]];
      BlocMatrix[2][1] = SectorTable[(STableX[xp ]+STableY[1]+STableZ[zc ])]->Data[OfTableX[xp ]+OfTableY[1]+OfTableZ[zc ]];
      BlocMatrix[2][2] = SectorTable[(STableX[xpp]+STableY[1]+STableZ[zc ])]->Data[OfTableX[xpp]+OfTableY[1]+OfTableZ[zc ]];
      BlocMatrix[2][3] = SectorTable[(STableX[xc ]+STableY[1]+STableZ[zp ])]->Data[OfTableX[xc ]+OfTableY[1]+OfTableZ[zp ]];
      BlocMatrix[2][4] = SectorTable[(STableX[xp ]+STableY[1]+STableZ[zp ])]->Data[OfTableX[xp ]+OfTableY[1]+OfTableZ[zp ]];
      BlocMatrix[2][5] = SectorTable[(STableX[xpp]+STableY[1]+STableZ[zp ])]->Data[OfTableX[xpp]+OfTableY[1]+OfTableZ[zp ]];
      BlocMatrix[2][6] = SectorTable[(STableX[xc ]+STableY[1]+STableZ[zpp])]->Data[OfTableX[xc ]+OfTableY[1]+OfTableZ[zpp]];
      BlocMatrix[2][7] = SectorTable[(STableX[xp ]+STableY[1]+STableZ[zpp])]->Data[OfTableX[xp ]+OfTableY[1]+OfTableZ[zpp]];
      BlocMatrix[2][8] = SectorTable[(STableX[xpp]+STableY[1]+STableZ[zpp])]->Data[OfTableX[xpp]+OfTableY[1]+OfTableZ[zpp]];

      for ( yc=0 ; yc< ZVOXELBLOCSIZE_Y ; yc++ )
      {
        yp = yc+1; ypp=yc+2;

        // Scrolling bloc matrix by exchangingypp references.
        tmpp = BlocMatrix[0];
        BlocMatrix[0] = BlocMatrix[1];
        BlocMatrix[1] = BlocMatrix[2];
        BlocMatrix[2] = (UByte *) tmpp;

        // Fetching a new bloc of data slice;

        BlocMatrix[2][0] = SectorTable[(STableX[xc ]+STableY[ypp]+STableZ[zc ])]->Data[OfTableX[xc ]+OfTableY[ypp]+OfTableZ[zc ]];
        BlocMatrix[2][1] = SectorTable[(STableX[xp ]+STableY[ypp]+STableZ[zc ])]->Data[OfTableX[xp ]+OfTableY[ypp]+OfTableZ[zc ]];
        BlocMatrix[2][2] = SectorTable[(STableX[xpp]+STableY[ypp]+STableZ[zc ])]->Data[OfTableX[xpp]+OfTableY[ypp]+OfTableZ[zc ]];
        BlocMatrix[2][3] = SectorTable[(STableX[xc ]+STableY[ypp]+STableZ[zp ])]->Data[OfTableX[xc ]+OfTableY[ypp]+OfTableZ[zp ]];
        BlocMatrix[2][4] = SectorTable[(STableX[xp ]+STableY[ypp]+STableZ[zp ])]->Data[OfTableX[xp ]+OfTableY[ypp]+OfTableZ[zp ]];
        BlocMatrix[2][5] = SectorTable[(STableX[xpp]+STableY[ypp]+STableZ[zp ])]->Data[OfTableX[xpp]+OfTableY[ypp]+OfTableZ[zp ]];
        BlocMatrix[2][6] = SectorTable[(STableX[xc ]+STableY[ypp]+STableZ[zpp])]->Data[OfTableX[xc ]+OfTableY[ypp]+OfTableZ[zpp]];
        BlocMatrix[2][7] = SectorTable[(STableX[xp ]+STableY[ypp]+STableZ[zpp])]->Data[OfTableX[xp ]+OfTableY[ypp]+OfTableZ[zpp]];
        BlocMatrix[2][8] = SectorTable[(STableX[xpp]+STableY[ypp]+STableZ[zpp])]->Data[OfTableX[xpp]+OfTableY[ypp]+OfTableZ[zpp]];

        // Compute face culling info
/*
        if ( (x==0) && (y==0) && (z==0) && (yc==0) && (xc==0) && (zc==0) )
        {
          printf("#HERE#\n");
          printf("x=%ld, y=%ld, z=%ld\n",xc,yc,zc);
          printf("Offset for 0 : %d\n", OfTableX[xp ]+OfTableY[ypp]+OfTableZ[zp ] );
          printf("Sector for 0 : %ld\n", (STableX[xp ]+STableY[ypp]+STableZ[zp ]));
          printf("Top Voxel : %lx\n", (ULong)SectorTable[(STableX[xp ]+STableY[ypp]+STableZ[zp ])]->Data[OfTableX[xp ]+OfTableY[ypp]+OfTableZ[zp ]]);
          printf("Top Voxel : %lx\n", (ULong)SectorTable[0]->Data[1]);
          printf("STableX[xp ]=%ld STableY[ypp]= %ld STableZ[zp ]=%ld xp=%ld, ypp=%ld, zp=%ld\n", (ULong)STableX[xp ], (ULong)STableY[ypp],(ULong)STableZ[zp ], xp,ypp,zp);
          for (i=0;i<3;i++)
          {
            for (j=0;j<3;j++) { printf("%02x",BlocMatrix[0][j+i*3]); }
            printf(" ");
            for (j=0;j<3;j++) { printf("%02x",BlocMatrix[1][j+i*3]); }
            printf(" ");
            for (j=0;j<3;j++) { printf("%02x",BlocMatrix[2][j+i*3]); }
            printf("\n");
          }
          printf("\n");

          for (i=0;i<27;i++) {printf( "%ld:%lx", i,SectorTable[i]); if (SectorTable[i]==WorkingFullSector) printf("*"); printf(" ");}
          printf("--------------------\n");
        }
*/
/*
        info = 0;
        info |= ( (BlocMatrix[1][1] == 0) || (BlocMatrix[1][1] & 0x8000)) ? DRAWFACE_AHEAD : 0;
        info |= ( (BlocMatrix[1][7] == 0) || (BlocMatrix[1][7] & 0x8000)) ? DRAWFACE_BEHIND : 0;
        info |= ( (BlocMatrix[1][3] == 0) || (BlocMatrix[1][3] & 0x8000)) ? DRAWFACE_LEFT : 0;
        info |= ( (BlocMatrix[1][5] == 0) || (BlocMatrix[1][5] & 0x8000)) ? DRAWFACE_RIGHT : 0;
        info |= ( (BlocMatrix[0][4] == 0) || (BlocMatrix[0][4] & 0x8000)) ? DRAWFACE_BELOW : 0;
        info |= ( (BlocMatrix[2][4] == 0) || (BlocMatrix[2][4] & 0x8000)) ? DRAWFACE_ABOVE : 0;
*/
/*
        info = 0;
        info |= ( VoxelTypeTable[ BlocMatrix[1][1] ]->Draw_FullVoxelOpacity ) ? 0 : DRAWFACE_AHEAD;
        info |= ( VoxelTypeTable[ BlocMatrix[1][7] ]->Draw_FullVoxelOpacity ) ? 0 : DRAWFACE_BEHIND;
        info |= ( VoxelTypeTable[ BlocMatrix[1][3] ]->Draw_FullVoxelOpacity ) ? 0 : DRAWFACE_LEFT;
        info |= ( VoxelTypeTable[ BlocMatrix[1][5] ]->Draw_FullVoxelOpacity ) ? 0 : DRAWFACE_RIGHT;
        info |= ( VoxelTypeTable[ BlocMatrix[0][4] ]->Draw_FullVoxelOpacity ) ? 0 : DRAWFACE_BELOW;
        info |= ( VoxelTypeTable[ BlocMatrix[2][4] ]->Draw_FullVoxelOpacity ) ? 0 : DRAWFACE_ABOVE;
*/


        info = 0;
        if (BlocMatrix[1][4] > 0)
        {
          Vt[0] = VoxelTypeTable[BlocMatrix[1][1]];
          Vt[1] = VoxelTypeTable[BlocMatrix[1][7]];
          Vt[2] = VoxelTypeTable[BlocMatrix[1][3]];
          Vt[3] = VoxelTypeTable[BlocMatrix[1][5]];
          Vt[4] = VoxelTypeTable[BlocMatrix[0][4]];
          Vt[5] = VoxelTypeTable[BlocMatrix[2][4]];
          TransparentVoxel = VoxelTypeTable[BlocMatrix[1][4]]->Draw_TransparentRendering;

          info |= ( Vt[0]->Draw_FullVoxelOpacity || (TransparentVoxel && Vt[0]->Draw_TransparentRendering) ) ? 0 : DRAWFACE_AHEAD;
          info |= ( Vt[1]->Draw_FullVoxelOpacity || (TransparentVoxel && Vt[1]->Draw_TransparentRendering) ) ? 0 : DRAWFACE_BEHIND;
          info |= ( Vt[2]->Draw_FullVoxelOpacity || (TransparentVoxel && Vt[2]->Draw_TransparentRendering) ) ? 0 : DRAWFACE_LEFT;
          info |= ( Vt[3]->Draw_FullVoxelOpacity || (TransparentVoxel && Vt[3]->Draw_TransparentRendering) ) ? 0 : DRAWFACE_RIGHT;
          info |= ( Vt[4]->Draw_FullVoxelOpacity || (TransparentVoxel && Vt[4]->Draw_TransparentRendering) ) ? 0 : DRAWFACE_BELOW;
          info |= ( Vt[5]->Draw_FullVoxelOpacity || (TransparentVoxel && Vt[5]->Draw_TransparentRendering) ) ? 0 : DRAWFACE_ABOVE;
        }

        // if ( (y==-1) && (yc==63) ) info = 255;
        if (BlocMatrix[1][4]>0)
        {
          if (VoxelTypeTable[BlocMatrix[1][4]]->Draw_TransparentRendering) SectorTable[0]->Flag_Void_Transparent = false;
          else                                                             SectorTable[0]->Flag_Void_Regular = false;
        }

        // Write face culling info to face culling table

        SectorTable[0]->FaceCulling[OfTableX[xp]+OfTableY[yp]+OfTableZ[zp]] = info;

      }
    }
  }

}




bool ZVoxelWorld::RayCast2(double x, double y, double z, double yaw, double pitch, double roll, ZVoxelCoords & PointedCube, ZVoxelCoords CubeBeforePointed  )
{
  ZVector3d Delta_h;
  ZVector3d Offset_h;
  ZVector3d Norm_h;
  ZVector3d Collision_h;
  Long ActualCube_x,ActualCube_y,ActualCube_z;
  Long NewCube_x,NewCube_y,NewCube_z;

  ULong i;


  // Delta_h.x = tan((- yaw)/57.295779513);
  Delta_h.y = tan(-pitch/57.295779513);
  Delta_h.z = tan(( yaw + 90.0  )/57.295779513);



  Collision_h.x = (floor(x / 256.0) + 1.0)*256.0;
  Collision_h.y = (Collision_h.x - x) * Delta_h.y + y;
  Collision_h.z = (Collision_h.x - x) * Delta_h.z + z;

  if (yaw>=0.0 && yaw<180.0)  Offset_h.x = 256.0;
  else                        Offset_h.x = -256.0;
  Offset_h.y = Delta_h.y * 256.0;
  Offset_h.z = Delta_h.z * 256.0;

  Norm_h.x = Offset_h.x / 256.0;
  Norm_h.y = Offset_h.y / 256.0;
  Norm_h.z = Offset_h.z / 256.0;

  // printf("Angle (y:%lf p:%lf) XYZ:(%lf %lf %lf) Off(%lf %lf %lf) Coll(%lf %lf %lf)\n", yaw,pitch,x,y,z, Offset_h.x, Offset_h.y, Offset_h.z, Collision_h.x, Collision_h.y, Collision_h.z);

  for (i=0;i<50;i++)
  {
    ActualCube_x = (Long)((Collision_h.x - Norm_h.x) / 256.0);
    ActualCube_y = (Long)((Collision_h.y - Norm_h.y) / 256.0);
    ActualCube_z = (Long)((Collision_h.z - Norm_h.z) / 256.0);
    NewCube_x = (Long)floor((Collision_h.x + Norm_h.x) / 256.0);
    NewCube_y = (Long)floor((Collision_h.y + Norm_h.y) / 256.0);
    NewCube_z = (Long)floor((Collision_h.z + Norm_h.z) / 256.0);

    Collision_h.x += Offset_h.x; Collision_h.y += Offset_h.y; Collision_h.z += Offset_h.z;

    if (GetVoxel( NewCube_x, NewCube_y, NewCube_z)>0)
    {
      CubeBeforePointed.x = ActualCube_x; CubeBeforePointed.y = ActualCube_y; CubeBeforePointed.z = ActualCube_z;
      PointedCube.x = NewCube_x; PointedCube.y = NewCube_y; PointedCube.z = NewCube_z;
      //printf("MATCH: %ld %ld %ld POS %lf %lf %lf\n",PointedCube.x, PointedCube.y, PointedCube.z, Collision_h.x, Collision_h.y, Collision_h.z);
      return(true);
    }

  }
  // printf("\n");
  return(false);
  //printf("first_h_x : %lf first_h_y %lf\n",first_h_x,first_h_y);
}


bool ZVoxelWorld::RayCast(const ZRayCast_in * In, ZRayCast_out * Out )
{
  ZVector4d Delta_h,Delta_v,Delta_s;
  ZVector4d Offset_h, Offset_v, Offset_s;
  ZVector3d Norm_h, Norm_v, Norm_s;
  ZVector4d Collision_h, Collision_v, Collision_s;

  /*
  Offset_h.x = Offset_h.y = Offset_h.z = Offset_h.w = 0.0;
  Offset_v.x = Offset_v.y = Offset_v.z = Offset_v.w = 0.0;
  Offset_s.x = Offset_s.y = Offset_s.z = Offset_s.w = 0.0;
  Collision_h.x = Collision_h.y = Collision_h.z = Collision_h.w = 0.0;
  Collision_v.x = Collision_v.y = Collision_v.z = Collision_v.w = 0.0;
  Collision_s.x = Collision_s.y = Collision_s.z = Collision_s.w = 0.0;
  */


  Long ActualCube_x,ActualCube_y,ActualCube_z;
  Long NewCube_x,NewCube_y,NewCube_z;
  bool Collide_X, Collide_Y, Collide_Z;
  ULong i;

  ZVector3d Norm;
  ZVector3d Tmp;

  Norm.x = 0; Norm.y = 0; Norm.z = 1;

  // X axis rotation
  Tmp.y = Norm.y * cos(-In->Camera->Pitch/57.295779513) - Norm.z * sin(-In->Camera->Pitch/57.295779513);
  Tmp.z = Norm.y * sin(-In->Camera->Pitch/57.295779513) + Norm.z * cos(-In->Camera->Pitch/57.295779513);
  Norm.y = Tmp.y; Norm.z = Tmp.z;
  // Y axis rotation
  Tmp.x = Norm.z*sin(In->Camera->Yaw/57.295779513) + Norm.x * cos(In->Camera->Yaw/57.295779513);
  Tmp.z = Norm.z*cos(In->Camera->Yaw/57.295779513) - Norm.x * sin(In->Camera->Yaw/57.295779513);
  Norm.x = Tmp.x; Norm.z = Tmp.z;
/*
  // Z axis rotation
  Tmp.x = Norm.x * cos(roll/57.295779513) - Norm.y * sin(roll/57.295779513);
  Tmp.y = Norm.x * sin(roll/57.295779513) + Norm.y * cos(roll/57.295779513);
  Norm.x = Tmp.x; Norm.y = Tmp.y;
*/
  // Delta_h.x = tan((- yaw)/57.295779513);

  Collide_X = Collide_Y = Collide_Z = false;

  if (In->Camera->Yaw >= 1.0 && In->Camera->Yaw <= 179.0)
  {
    Collide_X = true;
    Delta_h.y = Norm.y / -Norm.x;
    Delta_h.z = Norm.z / -Norm.x;
    Delta_h.w = 1.0 / Norm.x;
    Collision_h.x = (floor(In->Camera->x / 256.0) + 1.0)*256.0;
    Collision_h.y = (Collision_h.x - In->Camera->x) * Delta_h.y + In->Camera->y;
    Collision_h.z = (Collision_h.x - In->Camera->x) * Delta_h.z + In->Camera->z;
    Collision_h.w = (Collision_h.x - In->Camera->x) * Delta_h.w;

    Offset_h.x = 256.0;
    Offset_h.y = Delta_h.y * 256.0;
    Offset_h.z = Delta_h.z * 256.0;
    Offset_h.w = Delta_h.w * 256.0;
    Norm_h.x = Offset_h.x / 128.0;
    Norm_h.y = Offset_h.y / 128.0;
    Norm_h.z = Offset_h.z / 128.0;
  }
  else if (In->Camera->Yaw >= 181.0 && In->Camera->Yaw <= 359.0)
  {
    Collide_X = true;

    Delta_h.y = Norm.y / Norm.x;
    Delta_h.z = Norm.z / Norm.x;
    Delta_h.w = 1.0 / fabs(Norm.x);

    Collision_h.x = (floor(In->Camera->x / 256.0))*256.0;
    Collision_h.y = (In->Camera->x - Collision_h.x) * Delta_h.y + In->Camera->y;
    Collision_h.z = (In->Camera->x - Collision_h.x) * Delta_h.z + In->Camera->z;
    Collision_h.w = (In->Camera->x - Collision_h.x) * Delta_h.w;
    Offset_h.x = -256.0;
    Offset_h.y = Delta_h.y * 256.0;
    Offset_h.z = Delta_h.z * 256.0;
    Offset_h.w = Delta_h.w * 256.0;
    Norm_h.x = Offset_h.x / 128.0;
    Norm_h.y = Offset_h.y / 128.0;
    Norm_h.z = Offset_h.z / 128.0;
  }

  if (In->Camera->Pitch >=1.0 && In->Camera->Pitch <= 179.0)
  {
    Collide_Y = true;
    Delta_v.x = Norm.x / Norm.y;
    Delta_v.z = Norm.z / - Norm.y;
    Delta_v.w = 1 / Norm.y;
    Collision_v.y = (floor(In->Camera->y / 256.0)) * 256.0;
    Collision_v.x = (In->Camera->y - Collision_v.y) * Delta_v.x + In->Camera->x;
    Collision_v.z = (In->Camera->y - Collision_v.y) * Delta_v.z + In->Camera->z;
    Collision_v.w = (In->Camera->y - Collision_v.y) * Delta_v.w;
    Offset_v.y = -256.0;
    Offset_v.x = Delta_v.x * 256.0;
    Offset_v.z = Delta_v.z * 256.0;
    Offset_v.w = Delta_v.w * 256.0;
    Norm_v.x = Offset_v.x / 128.0;
    Norm_v.y = Offset_v.y / 128.0;
    Norm_v.z = Offset_v.z / 128.0;
  }
  else if (In->Camera->Pitch >=181.0 && In->Camera->Pitch <= 359.0)
  {
    Collide_Y = true;
    Delta_v.x = Norm.x / -Norm.y;
    Delta_v.z = Norm.z / + Norm.y;
    Delta_v.w = 1.0 / -Norm.y;
    Collision_v.y = (floor(In->Camera->y / 256.0)+1) * 256.0;
    Collision_v.x = (Collision_v.y - In->Camera->y) * Delta_v.x + In->Camera->x;
    Collision_v.z = (Collision_v.y - In->Camera->y) * Delta_v.z + In->Camera->z;
    Collision_v.w = (Collision_v.y - In->Camera->y) * Delta_v.w;

    Offset_v.y = 256.0;
    Offset_v.x = Delta_v.x * 256.0;
    Offset_v.z = Delta_v.z * 256.0;
    Offset_v.w = Delta_v.w * 256.0;
    Norm_v.x = Offset_v.x / 128.0;
    Norm_v.y = Offset_v.y / 128.0;
    Norm_v.z = Offset_v.z / 128.0;
  }

  if (In->Camera->Yaw >= 91.0 && In->Camera->Yaw <= 269.0)
  {
    Collide_Z = true;
    Delta_s.x = Norm.x / -Norm.z;
    Delta_s.y = Norm.y / Norm.z;
    Delta_s.w = 1.0 / -Norm.z;
    Collision_s.z = (floor(In->Camera->z / 256.0) + 1.0)*256.0;
    Collision_s.x = (Collision_s.z - In->Camera->z) * Delta_s.x + In->Camera->x;
    Collision_s.y = (Collision_s.z - In->Camera->z) * Delta_s.y + In->Camera->y;
    Collision_s.w = (Collision_s.z - In->Camera->z) * Delta_s.w;

    Offset_s.z = 256.0;
    Offset_s.x = Delta_s.x * 256.0;
    Offset_s.y = Delta_s.y * 256.0;
    Offset_s.w = Delta_s.w * 256.0;
    Norm_s.x = Offset_s.x / 128.0;
    Norm_s.y = Offset_s.y / 128.0;
    Norm_s.z = Offset_s.z / 128.0;
  }
  else if (In->Camera->Yaw <= 89 || In->Camera->Yaw >= 271.0)
  {
    Collide_Z = true;
    Delta_s.x = Norm.x / +Norm.z;
    Delta_s.y = Norm.y / -Norm.z;
    Delta_s.w = 1.0 / Norm.z;
    Collision_s.z = (floor(In->Camera->z / 256.0) )*256.0;
    Collision_s.x = (In->Camera->z - Collision_s.z) * Delta_s.x + In->Camera->x;
    Collision_s.y = (In->Camera->z - Collision_s.z) * Delta_s.y + In->Camera->y;
    Collision_s.w = (In->Camera->z - Collision_s.z) * Delta_s.w;
    Offset_s.z = - 256.0;
    Offset_s.x = Delta_s.x * 256.0;
    Offset_s.y = Delta_s.y * 256.0;
    Offset_s.w = Delta_s.w * 256.0;
    Norm_s.x = Offset_s.x / 128.0;
    Norm_s.y = Offset_s.y / 128.0;
    Norm_s.z = Offset_s.z / 128.0;
  }



//  printf("yaw: %04lf pitch: %lf Offset_y:%lf Offset_z:%lf xyz:%lf %lf %lf NXYZ:%lf %lf %lf Dxyz:%lf %lf %lf", yaw,pitch, Delta_h.y, Delta_h.z,x,y,z, Norm_h.x, Norm_h.y, Norm_h.z, Delta_h.x, Delta_h.y, Delta_h.z);
 //printf("Angle (y:%lf p:%lf) XYZ:(%lf %lf %lf) Off(%lf %lf %lf %lf) Coll(%lf %lf %lf %lf) Norm(%lg %lg %lf) :\n", yaw,pitch,x,y,z, Offset_s.x, Offset_s.y, Offset_s.z, Offset_s.w, Collision_s.x, Collision_s.y, Collision_s.z, Collision_s.w, Norm_s.x,Norm_s.y, Norm_s.z);

  Long Match_h = 0;
  Long Match_s = 0;
  Long Match_v = 0;
  Long Cycle = 1;
  double MinW = 1000000.0;

  for (i=0;i<150;i++)
  {

    // Horizontal X axis.
    if (Collide_X)
    {
      if (Match_h==0 && Collision_h.w < MinW)
      {
        ActualCube_x = (Long)floor((Collision_h.x - Norm_h.x) / 256.0); ActualCube_y = (Long)floor((Collision_h.y - Norm_h.y) / 256.0); ActualCube_z = (Long)floor((Collision_h.z - Norm_h.z) / 256.0);
        NewCube_x = (Long)floor((Collision_h.x + Norm_h.x) / 256.0); NewCube_y = (Long)floor((Collision_h.y + Norm_h.y) / 256.0); NewCube_z = (Long)floor((Collision_h.z + Norm_h.z) / 256.0);
        if (GetVoxel( NewCube_x, NewCube_y, NewCube_z)>0)
        {
          Out->PredPointedVoxel.x = ActualCube_x; Out->PredPointedVoxel.y = ActualCube_y; Out->PredPointedVoxel.z = ActualCube_z;
          Out->PointedVoxel.x = NewCube_x; Out->PointedVoxel.y = NewCube_y; Out->PointedVoxel.z = NewCube_z;
          // printf(" MATCH_H: %lf\n",Collision_h.w);
          Match_h = Cycle;
          MinW = Collision_h.w;
        }
      }
    }

    // Horizontal Z axis.

    if (Collide_Z)
    {
      if (Match_s == 0 && Collision_s.w < MinW)
      {
        ActualCube_x = (Long)floor((Collision_s.x - Norm_s.x) / 256.0); ActualCube_y = (Long)floor((Collision_s.y - Norm_s.y) / 256.0); ActualCube_z = (Long)floor((Collision_s.z - Norm_s.z) / 256.0);
        NewCube_x = (Long)floor((Collision_s.x + Norm_s.x) / 256.0); NewCube_y = (Long)floor((Collision_s.y + Norm_s.y) / 256.0); NewCube_z = (Long)floor((Collision_s.z + Norm_s.z) / 256.0);
        if (GetVoxel( NewCube_x, NewCube_y, NewCube_z)>0)
        {
          Out->PredPointedVoxel.x = ActualCube_x; Out->PredPointedVoxel.y = ActualCube_y; Out->PredPointedVoxel.z = ActualCube_z;
          Out->PointedVoxel.x = NewCube_x; Out->PointedVoxel.y = NewCube_y; Out->PointedVoxel.z = NewCube_z;
          // printf(" MATCH_S: %lf\n",Collision_s.w);
          Match_s = Cycle;
          MinW = Collision_s.w;
        }
      }
    }

    // Vertical Y axis.

    if (Collide_Y)
    {
      if (Match_v==0 && Collision_v.w < MinW)
      {
        ActualCube_x = (Long)floor((Collision_v.x - Norm_v.x) / 256.0);   ActualCube_y = (Long)floor((Collision_v.y - Norm_v.y) / 256.0);   ActualCube_z = (Long)floor((Collision_v.z - Norm_v.z) / 256.0);
        NewCube_x = (Long)floor((Collision_v.x + Norm_v.x) / 256.0); NewCube_y = (Long)floor((Collision_v.y + Norm_v.y) / 256.0); NewCube_z = (Long)floor((Collision_v.z + Norm_v.z) / 256.0);
        if (GetVoxel( NewCube_x, NewCube_y, NewCube_z)>0)
        {
          Out->PredPointedVoxel.x = ActualCube_x; Out->PredPointedVoxel.y = ActualCube_y; Out->PredPointedVoxel.z = ActualCube_z;
          Out->PointedVoxel.x = NewCube_x; Out->PointedVoxel.y = NewCube_y; Out->PointedVoxel.z = NewCube_z;
          // printf(" MATCH_V: %lf\n",Collision_v.w);
          Match_v = Cycle;
          MinW = Collision_v.w;
        }
      }
    }

      //printf(" Match (H:%lf S:%lf V:%lf) \n", Collision_h.w, Collision_s.w, Collision_v.w);
      if (Match_h>0 && (Match_h - Cycle)<-100) return(true);
      if (Match_s>0 && (Match_s - Cycle)<-100) return(true);
      if (Match_v>0 && (Match_v - Cycle)<-100) return(true);

    Collision_h.x += Offset_h.x; Collision_h.y += Offset_h.y; Collision_h.z += Offset_h.z; Collision_h.w += Offset_h.w;
    Collision_v.x += Offset_v.x; Collision_v.y += Offset_v.y; Collision_v.z += Offset_v.z; Collision_v.w += Offset_v.w;
    Collision_s.x += Offset_s.x; Collision_s.y += Offset_s.y; Collision_s.z += Offset_s.z; Collision_s.w += Offset_s.w;
    Cycle ++;
  }
  // printf("\n");
  return(false);
  //printf("first_h_x : %lf first_h_y %lf\n",first_h_x,first_h_y);
}


/*

void ZVoxelWorld::SetVoxel_WithCullingUpdate2(Long x, Long y, Long z, UShort VoxelValue)
{
  UShort * Voxel_Address[7];
  ULong  Offset[7];
  UByte * FaceCulling_Address[7];
  UShort VoxelClear[7];
  ZVoxelSector * Sector[7];

  //Long Sector_x,Sector_y,Sector_z;

  //Sector_x = x >> ZVOXELBLOCSHIFT_X; Sector_y = ZVOXELBLOCSHIFT_Y; Sector_z = ZVOXELBLOCSHIFT_Z;

  Sector[VOXEL_LEFT]    = FindSector( (x-1) >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z );
  Sector[VOXEL_RIGHT]   = FindSector( (x+1) >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z );
  Sector[VOXEL_INFRONT] = FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z-1) >> ZVOXELBLOCSHIFT_Z );
  Sector[VOXEL_BEHIND]  = FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z+1) >> ZVOXELBLOCSHIFT_Z );
  Sector[VOXEL_ABOVE]   = FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y + 1) >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z );
  Sector[VOXEL_BELOW]   = FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y - 1) >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z );
  Sector[VOXEL_INCENTER]= FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z );

  if (Sector[VOXEL_LEFT])
  {
    Offset[VOXEL_LEFT] = (y & ZVOXELBLOCMASK_Y) + ( ((x - 1) & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_LEFT] = Sector[VOXEL_LEFT]->Data + Offset[VOXEL_LEFT];
    VoxelClear[VOXEL_LEFT] = (*Voxel_Address[VOXEL_LEFT]  > 0) ? 0: 255;
    FaceCulling_Address[VOXEL_LEFT] = Sector[VOXEL_LEFT]->FaceCulling + Offset[VOXEL_LEFT];
  } else VoxelClear[VOXEL_LEFT]=0;

  if (Sector[VOXEL_RIGHT])
  {
    Offset[VOXEL_RIGHT] = (y & ZVOXELBLOCMASK_Y) + ( ((x + 1) & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_RIGHT] = Sector[VOXEL_RIGHT]->Data + Offset[VOXEL_RIGHT];
    VoxelClear[VOXEL_RIGHT] = (*Voxel_Address[VOXEL_RIGHT]  > 0) ? 0 : 255;
    FaceCulling_Address[VOXEL_RIGHT] = Sector[VOXEL_RIGHT]->FaceCulling + Offset[VOXEL_RIGHT];
  } else VoxelClear[VOXEL_RIGHT]=0;

  if (Sector[VOXEL_INFRONT])
  {
    Offset[VOXEL_INFRONT] = (y & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ (((z - 1) & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_INFRONT] = Sector[VOXEL_INFRONT]->Data + Offset[VOXEL_INFRONT];
    VoxelClear[VOXEL_INFRONT] = (*Voxel_Address[VOXEL_INFRONT]  > 0) ? 0 : 255;
    FaceCulling_Address[VOXEL_INFRONT] = Sector[VOXEL_INFRONT]->FaceCulling + Offset[VOXEL_INFRONT];
  } else VoxelClear[VOXEL_INFRONT]=0;

  if (Sector[VOXEL_BEHIND])
  {
    Offset[VOXEL_BEHIND] = (y & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ (((z + 1) & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_BEHIND] = Sector[VOXEL_BEHIND]->Data + Offset[VOXEL_BEHIND];
    VoxelClear[VOXEL_BEHIND] = (*Voxel_Address[VOXEL_BEHIND]  > 0) ? 0 : 255;
    FaceCulling_Address[VOXEL_BEHIND] = Sector[VOXEL_BEHIND]->FaceCulling + Offset[VOXEL_BEHIND];
  } else VoxelClear[VOXEL_BEHIND]=0;

  if (Sector[VOXEL_ABOVE])
  {
    Offset[VOXEL_ABOVE] = ((y + 1) & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_ABOVE] = Sector[VOXEL_ABOVE]->Data + Offset[VOXEL_ABOVE];
    VoxelClear[VOXEL_ABOVE] = (*Voxel_Address[VOXEL_ABOVE]  > 0) ? 0 : 255;
    FaceCulling_Address[VOXEL_ABOVE] = Sector[VOXEL_ABOVE]->FaceCulling + Offset[VOXEL_ABOVE];
  } else VoxelClear[VOXEL_ABOVE]=0;

  if (Sector[VOXEL_BELOW])
  {
    Offset[VOXEL_BELOW] = ((y - 1) & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_BELOW] = Sector[VOXEL_BELOW]->Data + Offset[VOXEL_BELOW];
    VoxelClear[VOXEL_BELOW] = (*Voxel_Address[VOXEL_BELOW]  > 0) ? 0 : 255;
    FaceCulling_Address[VOXEL_BELOW] = Sector[VOXEL_BELOW]->FaceCulling + Offset[VOXEL_BELOW];
  } else VoxelClear[VOXEL_BELOW]=0;

  if (Sector[VOXEL_INCENTER])
  {
    Offset[VOXEL_INCENTER] = (y & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_INCENTER] = Sector[VOXEL_INCENTER]->Data + Offset[VOXEL_INCENTER];
    *Voxel_Address[VOXEL_INCENTER] = VoxelValue;
    //printf("xyz:%ld,%ld,%ld Offset:%ld Setting %lx with %d\n",x,y,z,Offset[VOXEL_INCENTER],Voxel_Address[VOXEL_INCENTER], VoxelValue);
    VoxelClear[VOXEL_INCENTER] = (*Voxel_Address[VOXEL_INCENTER] > 0) ? 0 : 255;
    FaceCulling_Address[VOXEL_INCENTER] = Sector[VOXEL_INCENTER]->FaceCulling + Offset[VOXEL_INCENTER];
  } else VoxelClear[VOXEL_INCENTER]=0;

  if (Sector[VOXEL_INCENTER])  *FaceCulling_Address[VOXEL_INCENTER] =   (VoxelClear[VOXEL_LEFT]    & DRAWFACE_LEFT)
                                                                      | (VoxelClear[VOXEL_RIGHT]   & DRAWFACE_RIGHT)
                                                                      | (VoxelClear[VOXEL_INFRONT] & DRAWFACE_AHEAD)
                                                                      | (VoxelClear[VOXEL_BEHIND]  & DRAWFACE_BEHIND)
                                                                      | (VoxelClear[VOXEL_ABOVE]   & DRAWFACE_ABOVE)
                                                                      | (VoxelClear[VOXEL_BELOW]   & DRAWFACE_BELOW);
  if (Sector[VOXEL_LEFT])      {*FaceCulling_Address[VOXEL_LEFT] &= 255 ^ DRAWFACE_RIGHT;
                                *FaceCulling_Address[VOXEL_LEFT] |= VoxelClear[VOXEL_INCENTER] & DRAWFACE_RIGHT; }
  if (Sector[VOXEL_RIGHT])     {*FaceCulling_Address[VOXEL_RIGHT] &= 255 ^ DRAWFACE_LEFT;
                                *FaceCulling_Address[VOXEL_RIGHT] |= VoxelClear[VOXEL_INCENTER] & DRAWFACE_LEFT; }
  if (Sector[VOXEL_INFRONT])   {*FaceCulling_Address[VOXEL_INFRONT] &= 255 ^ DRAWFACE_BEHIND;
                                *FaceCulling_Address[VOXEL_INFRONT] |= VoxelClear[VOXEL_INCENTER] & DRAWFACE_BEHIND; }
  if (Sector[VOXEL_BEHIND])    {*FaceCulling_Address[VOXEL_BEHIND] &= 255 ^ DRAWFACE_AHEAD;
                                *FaceCulling_Address[VOXEL_BEHIND] |= VoxelClear[VOXEL_INCENTER] & DRAWFACE_AHEAD; }
  if (Sector[VOXEL_ABOVE])     {*FaceCulling_Address[VOXEL_ABOVE] &= 255 ^ DRAWFACE_BELOW;
                                *FaceCulling_Address[VOXEL_ABOVE] |= VoxelClear[VOXEL_INCENTER] & DRAWFACE_BELOW; }
  if (Sector[VOXEL_ABOVE])     {*FaceCulling_Address[VOXEL_ABOVE] &= 255 ^ DRAWFACE_BELOW;
                                *FaceCulling_Address[VOXEL_ABOVE] |= VoxelClear[VOXEL_INCENTER] & DRAWFACE_BELOW; }
  if (Sector[VOXEL_BELOW])     {*FaceCulling_Address[VOXEL_BELOW] &= 255 ^ DRAWFACE_ABOVE;
                                *FaceCulling_Address[VOXEL_BELOW] |= VoxelClear[VOXEL_INCENTER] & DRAWFACE_ABOVE; }

  Sector[VOXEL_INCENTER]->Flag_Render_Dirty = true;
  Sector[VOXEL_LEFT    ]->Flag_Render_Dirty = true;
  Sector[VOXEL_RIGHT   ]->Flag_Render_Dirty = true;
  Sector[VOXEL_INFRONT ]->Flag_Render_Dirty = true;
  Sector[VOXEL_BEHIND  ]->Flag_Render_Dirty = true;
  Sector[VOXEL_ABOVE   ]->Flag_Render_Dirty = true;
  Sector[VOXEL_BELOW   ]->Flag_Render_Dirty = true;
  Sector[VOXEL_INCENTER]->Flag_Void_Regular = false;
  Sector[VOXEL_LEFT    ]->Flag_Void_Regular = false;
  Sector[VOXEL_RIGHT   ]->Flag_Void_Regular = false;
  Sector[VOXEL_INFRONT ]->Flag_Void_Regular = false;
  Sector[VOXEL_BEHIND  ]->Flag_Void_Regular = false;
  Sector[VOXEL_ABOVE   ]->Flag_Void_Regular = false;
  Sector[VOXEL_BELOW   ]->Flag_Void_Regular = false;
  Sector[VOXEL_INCENTER]->Flag_Void_Transparent = false;
  Sector[VOXEL_LEFT    ]->Flag_Void_Transparent = false;
  Sector[VOXEL_RIGHT   ]->Flag_Void_Transparent = false;
  Sector[VOXEL_INFRONT ]->Flag_Void_Transparent = false;
  Sector[VOXEL_BEHIND  ]->Flag_Void_Transparent = false;
  Sector[VOXEL_ABOVE   ]->Flag_Void_Transparent = false;
  Sector[VOXEL_BELOW   ]->Flag_Void_Transparent = false;

}
*/

bool ZVoxelWorld::RayCast_Vector(const ZVector3d & Pos, const ZVector3d & Vector, const ZRayCast_in * In, ZRayCast_out * Out, bool InvertCollision )
{
  ZVector4d Delta_h,Delta_v,Delta_s;
  ZVector4d Offset_h, Offset_v, Offset_s;
  ZVector3d Norm_h, Norm_v, Norm_s;
  ZVector4d Collision_h, Collision_v, Collision_s;

  Long ActualCube_x,ActualCube_y,ActualCube_z;
  Long NewCube_x,NewCube_y,NewCube_z;
  bool Collide_X, Collide_Y, Collide_Z;
  ULong i;
  UByte Face_h, Face_s,Face_v;
  UShort VoxelType;

  ZVector3d Norm;
  ZVector3d Tmp;

  double Vector_Len;

  // Normalize input vector.
  Vector_Len = sqrt( Vector.x * Vector.x + Vector.y * Vector.y + Vector.z * Vector.z);
  Norm = Vector / Vector_Len;

  // Norm = Vector;

  //printf("Norm:%lf %lf %lf\n",Norm.x,Norm.y,Norm.z);
  Collide_X = Collide_Y = Collide_Z = false;
  Face_h = Face_s = Face_v = 0;

  if (Norm.x > 0.00000001)
  {
    Face_h = 2;
    Collide_X = true;
    Delta_h.y = Norm.y / Norm.x;
    Delta_h.z = Norm.z / Norm.x;
    Delta_h.w = 1.0 / Norm.x;
    Collision_h.x = (floor(Pos.x / 256.0) + 1.0)*256.0;
    Collision_h.y = (Collision_h.x - Pos.x) * Delta_h.y + Pos.y;
    Collision_h.z = (Collision_h.x - Pos.x) * Delta_h.z + Pos.z;
    Collision_h.w = (Collision_h.x - Pos.x) * Delta_h.w;

    Offset_h.x = 256.0;
    Offset_h.y = Delta_h.y * 256.0;
    Offset_h.z = Delta_h.z * 256.0;
    Offset_h.w = Delta_h.w * 256.0;
    Norm_h.x = Offset_h.x / 128.0;
    Norm_h.y = Offset_h.y / 128.0;
    Norm_h.z = Offset_h.z / 128.0;
  }
  else if (Norm.x < -0.00000001)
  {
    Face_h = 3;
    Collide_X = true;
    Delta_h.y = Norm.y / -Norm.x;
    Delta_h.z = Norm.z / -Norm.x;
    Delta_h.w = 1.0 / fabs(Norm.x);

    Collision_h.x = (floor(Pos.x / 256.0))*256.0 ; // - 1.0
    Collision_h.y = (Pos.x - Collision_h.x) * Delta_h.y + Pos.y;
    Collision_h.z = (Pos.x - Collision_h.x) * Delta_h.z + Pos.z;
    Collision_h.w = (Pos.x - Collision_h.x) * Delta_h.w;
    Offset_h.x = -256.0;
    Offset_h.y = Delta_h.y * 256.0;
    Offset_h.z = Delta_h.z * 256.0;
    Offset_h.w = Delta_h.w * 256.0;
    Norm_h.x = Offset_h.x / 128.0;
    Norm_h.y = Offset_h.y / 128.0;
    Norm_h.z = Offset_h.z / 128.0;
  }

  if (Norm.y > 0.00000001)
  {
    Face_v = 5;
    Collide_Y = true;
    Delta_v.x = Norm.x / Norm.y;
    Delta_v.z = Norm.z / Norm.y;
    Delta_v.w = 1 / Norm.y;
    Collision_v.y = (floor(Pos.y / 256.0) +1.0) * 256.0;
    Collision_v.x = (Collision_v.y - Pos.y) * Delta_v.x + Pos.x;
    Collision_v.z = (Collision_v.y - Pos.y) * Delta_v.z + Pos.z;
    Collision_v.w = (Collision_v.y - Pos.y) * Delta_v.w;
    Offset_v.y = 256.0;
    Offset_v.x = Delta_v.x * 256.0;
    Offset_v.z = Delta_v.z * 256.0;
    Offset_v.w = Delta_v.w * 256.0;
    Norm_v.x = Offset_v.x / 128.0;
    Norm_v.y = Offset_v.y / 128.0;
    Norm_v.z = Offset_v.z / 128.0;
  }
  else if (Norm.y < -0.00000001)
  {
    Face_v = 4;
    Collide_Y = true;
    Delta_v.x = Norm.x / -Norm.y;
    Delta_v.z = Norm.z / -Norm.y;
    Delta_v.w = 1.0 / -Norm.y;
    Collision_v.y = (floor(Pos.y / 256.0)) * 256.0 ; // - 1.0
    Collision_v.x = (Pos.y - Collision_v.y) * Delta_v.x + Pos.x;
    Collision_v.z = (Pos.y - Collision_v.y) * Delta_v.z + Pos.z;
    Collision_v.w = fabs((Collision_v.y - Pos.y) * Delta_v.w);

    Offset_v.y = -256.0;
    Offset_v.x = Delta_v.x * 256.0;
    Offset_v.z = Delta_v.z * 256.0;
    Offset_v.w = Delta_v.w * 256.0;
    Norm_v.x = Offset_v.x / 128.0;
    Norm_v.y = Offset_v.y / 128.0;
    Norm_v.z = Offset_v.z / 128.0;
  }

  if (Norm.z > 0.00000001)
  {
    Face_s = 0;
    Collide_Z = true;
    Delta_s.x = Norm.x / Norm.z;
    Delta_s.y = Norm.y / Norm.z;
    Delta_s.w = 1.0 / Norm.z;
    Collision_s.z = (floor(Pos.z / 256.0 + 1.0) )*256.0;
    Collision_s.x = (Collision_s.z - Pos.z) * Delta_s.x + Pos.x;
    Collision_s.y = (Collision_s.z - Pos.z) * Delta_s.y + Pos.y;
    Collision_s.w = (Collision_s.z - Pos.z) * Delta_s.w;

    Offset_s.z = 256.0;
    Offset_s.x = Delta_s.x * 256.0;
    Offset_s.y = Delta_s.y * 256.0;
    Offset_s.w = Delta_s.w * 256.0;
    Norm_s.x = Offset_s.x / 128.0;
    Norm_s.y = Offset_s.y / 128.0;
    Norm_s.z = Offset_s.z / 128.0;
  }
  else if (Norm.z < -0.00000001)
  {
    Face_s = 1;
    Collide_Z = true;
    Delta_s.x = Norm.x / -Norm.z;
    Delta_s.y = Norm.y / -Norm.z;
    Delta_s.w = 1.0 / -Norm.z;
    Collision_s.z = (floor(Pos.z / 256.0) )*256.0 ; // - 1.0
    Collision_s.x = (Pos.z - Collision_s.z) * Delta_s.x + Pos.x;
    Collision_s.y = (Pos.z - Collision_s.z) * Delta_s.y + Pos.y;
    Collision_s.w = (Pos.z - Collision_s.z) * Delta_s.w;
    Offset_s.z = - 256.0;
    Offset_s.x = Delta_s.x * 256.0;
    Offset_s.y = Delta_s.y * 256.0;
    Offset_s.w = Delta_s.w * 256.0;
    Norm_s.x = Offset_s.x / 128.0;
    Norm_s.y = Offset_s.y / 128.0;
    Norm_s.z = Offset_s.z / 128.0;
  }
/*
   printf ("Loc(%lf %lf %lf) Norm(%lf %lf %lf) Col(%lf %lf %lf %lf) Off(%lf %lf %lf %lf) C(%d,%d,%d)\n", Pos.x, Pos.y, Pos.z, Norm.x,Norm.y, Norm.z, Collision_s.x, Collision_s.y, Collision_s.z, Collision_s.w, Offset_s.x,Offset_s.y, Offset_s.z,Offset_s.w
       ,(UShort)((Collide_X==true)? 1:0) ,(UShort)((Collide_Y==true)? 1:0), (UShort)((Collide_Z==true)? 1:0));
*/
//  printf("yaw: %04lf pitch: %lf Offset_y:%lf Offset_z:%lf xyz:%lf %lf %lf NXYZ:%lf %lf %lf Dxyz:%lf %lf %lf", yaw,pitch, Delta_h.y, Delta_h.z,x,y,z, Norm_h.x, Norm_h.y, Norm_h.z, Delta_h.x, Delta_h.y, Delta_h.z);
 //printf("Angle (y:%lf p:%lf) XYZ:(%lf %lf %lf) Off(%lf %lf %lf %lf) Coll(%lf %lf %lf %lf) Norm(%lg %lg %lf) :\n", yaw,pitch,x,y,z, Offset_s.x, Offset_s.y, Offset_s.z, Offset_s.w, Collision_s.x, Collision_s.y, Collision_s.z, Collision_s.w, Norm_s.x,Norm_s.y, Norm_s.z);

  Long Match_h=0;
  Long Match_s=0;
  Long Match_v=0;
  Long Cycle = 1;
  double MinW = 10000000.0;

  for (i=0;i < In->MaxCubeIterations;i++)
  {
    // if  ( (Collision_h.w > In->MaxDetectionDistance) || (Collision_s.w > In->MaxDetectionDistance) || (Collision_v.w > In->MaxDetectionDistance)) { Out->Collided = false; return(false); }
    // Horizontal X axis.
    if (Collide_X)
    {
      if (Match_h==0 && Collision_h.w < MinW)
      {
        ActualCube_x = (Long)floor((Collision_h.x - Norm_h.x) / 256.0); ActualCube_y = (Long)floor((Collision_h.y - Norm_h.y) / 256.0); ActualCube_z = (Long)floor((Collision_h.z - Norm_h.z) / 256.0);
        NewCube_x = (Long)floor((Collision_h.x) / 256.0); NewCube_y = (Long)floor((Collision_h.y) / 256.0); NewCube_z = (Long)floor((Collision_h.z) / 256.0);
        //ActualCube_x = (Long)floor((Collision_h.x - Norm_h.x) / 256.0); ActualCube_y = (Long)floor((Collision_h.y - Norm_h.y) / 256.0); ActualCube_z = (Long)floor((Collision_h.z - Norm_h.z) / 256.0);
        //NewCube_x = (Long)floor((Collision_h.x + Norm_h.x) / 256.0); NewCube_y = (Long)floor((Collision_h.y + Norm_h.y) / 256.0); NewCube_z = (Long)floor((Collision_h.z + Norm_h.z) / 256.0);
        if (Face_h == 3) NewCube_x--;

        VoxelType = GetVoxel       ( NewCube_x, NewCube_y, NewCube_z);

        if (!VoxelTypeManager->VoxelTable[VoxelType]->Is_PlayerCanPassThrough ^ InvertCollision)
        {
          Out->PredPointedVoxel.x = ActualCube_x; Out->PredPointedVoxel.y = ActualCube_y; Out->PredPointedVoxel.z = ActualCube_z;
          Out->PointedVoxel.x = NewCube_x; Out->PointedVoxel.y = NewCube_y; Out->PointedVoxel.z = NewCube_z;
          Out->CollisionPoint.x = Collision_h.x; Out->CollisionPoint.y = Collision_h.y; Out->CollisionPoint.z = Collision_h.z; Out->CollisionDistance = Collision_h.w;
          Out->CollisionAxe = 0; Out->CollisionFace = Face_h;
          Out->PointInCubeFace.x = fmod(Out->CollisionPoint.z, 256.0); Out->PointInCubeFace.y = fmod(Out->CollisionPoint.y, 256.0);
          //printf(" MATCH_H: %lf (%ld %ld %ld) C:%ld\n",Collision_h.w, NewCube_x, NewCube_y, NewCube_z, Cycle);
          Match_h = Cycle;
          MinW = Collision_h.w;
        }
      }
    }

    // Horizontal Z axis.

    if (Collide_Z)
    {
      if (Match_s == 0 && Collision_s.w < MinW)
      {
        ActualCube_x = (Long)floor((Collision_s.x - Norm_s.x) / 256.0); ActualCube_y = (Long)floor((Collision_s.y - Norm_s.y) / 256.0); ActualCube_z = (Long)floor((Collision_s.z - Norm_s.z) / 256.0);
        NewCube_x = (Long)floor((Collision_s.x) / 256.0); NewCube_y = (Long)floor((Collision_s.y) / 256.0); NewCube_z = (Long)floor((Collision_s.z) / 256.0);
        //ActualCube_x = (Long)floor((Collision_s.x - Norm_s.x) / 256.0); ActualCube_y = (Long)floor((Collision_s.y - Norm_s.y) / 256.0); ActualCube_z = (Long)floor((Collision_s.z - Norm_s.z) / 256.0);
        //NewCube_x = (Long)floor((Collision_s.x + Norm_s.x) / 256.0); NewCube_y = (Long)floor((Collision_s.y + Norm_s.y) / 256.0); NewCube_z = (Long)floor((Collision_s.z + Norm_s.z) / 256.0);
        if (Face_s == 1) NewCube_z--;

        VoxelType = GetVoxel       ( NewCube_x, NewCube_y, NewCube_z);

        if (!VoxelTypeManager->VoxelTable[VoxelType]->Is_PlayerCanPassThrough ^ InvertCollision)
        {
          Out->PredPointedVoxel.x = ActualCube_x; Out->PredPointedVoxel.y = ActualCube_y; Out->PredPointedVoxel.z = ActualCube_z;
          Out->PointedVoxel.x = NewCube_x; Out->PointedVoxel.y = NewCube_y; Out->PointedVoxel.z = NewCube_z;
          Out->CollisionPoint.x = Collision_s.x; Out->CollisionPoint.y = Collision_s.y; Out->CollisionPoint.z = Collision_s.z; Out->CollisionDistance = Collision_s.w;
          Out->CollisionAxe = 2; Out->CollisionFace = Face_s;
          Out->PointInCubeFace.x = fmod(Out->CollisionPoint.x, 256.0); Out->PointInCubeFace.y = fmod(Out->CollisionPoint.y, 256.0);

          //printf(" MATCH_S: %lf (%ld %ld %ld) C:%ld\n",Collision_s.w, NewCube_x, NewCube_y, NewCube_z, Cycle);
          Match_s = Cycle;
          MinW = Collision_s.w;
        }
      }
    }

    // Vertical Y axis.

    if (Collide_Y)
    {
      if (Match_v==0 && Collision_v.w < MinW)
      {
        ActualCube_x = (Long)floor((Collision_v.x - Norm_v.x) / 256.0);   ActualCube_y = (Long)floor((Collision_v.y - Norm_v.y) / 256.0);   ActualCube_z = (Long)floor((Collision_v.z - Norm_v.z) / 256.0);
        NewCube_x = (Long)floor((Collision_v.x) / 256.0); NewCube_y = (Long)floor((Collision_v.y) / 256.0); NewCube_z = (Long)floor((Collision_v.z) / 256.0);
        //ActualCube_x = (Long)floor((Collision_v.x - Norm_v.x) / 256.0);   ActualCube_y = (Long)floor((Collision_v.y - Norm_v.y) / 256.0);   ActualCube_z = (Long)floor((Collision_v.z - Norm_v.z) / 256.0);
        //NewCube_x = (Long)floor((Collision_v.x + Norm_v.x) / 256.0); NewCube_y = (Long)floor((Collision_v.y + Norm_v.y) / 256.0); NewCube_z = (Long)floor((Collision_v.z + Norm_v.z) / 256.0);
        if (Face_v == 4) NewCube_y--;

        VoxelType = GetVoxel       ( NewCube_x, NewCube_y, NewCube_z);

        if (!VoxelTypeManager->VoxelTable[VoxelType]->Is_PlayerCanPassThrough ^ InvertCollision)
        {
          Out->PredPointedVoxel.x = ActualCube_x; Out->PredPointedVoxel.y = ActualCube_y; Out->PredPointedVoxel.z = ActualCube_z;
          Out->PointedVoxel.x = NewCube_x; Out->PointedVoxel.y = NewCube_y; Out->PointedVoxel.z = NewCube_z;
          Out->CollisionPoint.x = Collision_v.x; Out->CollisionPoint.y = Collision_v.y; Out->CollisionPoint.z = Collision_v.z; Out->CollisionDistance = Collision_v.w;
          Out->CollisionAxe = 1; Out->CollisionFace = Face_v;
          Out->PointInCubeFace.x = fmod(Out->CollisionPoint.x, 256.0); Out->PointInCubeFace.y = fmod(Out->CollisionPoint.z, 256.0);

          //printf(" MATCH_V: %lf (%ld %ld %ld) C:%ld\n",Collision_v.w, NewCube_x, NewCube_y, NewCube_z,Cycle );
          Match_v = Cycle;
          MinW = Collision_v.w;
        }
      }
    }

      //printf(" Match (H:%lf S:%lf V:%lf) \n", Collision_h.w, Collision_s.w, Collision_v.w);
      if (Match_h>0 && (Cycle - Match_h)>In->PlaneCubeDiff) { Out->Collided = true; return(true); }
      if (Match_s>0 && (Cycle - Match_s)>In->PlaneCubeDiff) { Out->Collided = true; return(true); }
      if (Match_v>0 && (Cycle - Match_v)>In->PlaneCubeDiff) { Out->Collided = true; return(true); }

    Collision_h.x += Offset_h.x; Collision_h.y += Offset_h.y; Collision_h.z += Offset_h.z; Collision_h.w += Offset_h.w;
    Collision_v.x += Offset_v.x; Collision_v.y += Offset_v.y; Collision_v.z += Offset_v.z; Collision_v.w += Offset_v.w;
    Collision_s.x += Offset_s.x; Collision_s.y += Offset_s.y; Collision_s.z += Offset_s.z; Collision_s.w += Offset_s.w;
    Cycle ++;
  }
  Out->Collided = false;
  Out->CollisionAxe = 0;
  Out->CollisionFace = 0;
  Out->CollisionDistance = 0.0;
  Out->CollisionPoint = 0.0;
  return(false);
  //printf("first_h_x : %lf first_h_y %lf\n",first_h_x,first_h_y);
}

/*

void ZVoxelWorld::SetVoxel_WithCullingUpdate(Long x, Long y, Long z, UShort VoxelValue)
{
  UShort * Voxel_Address[7];
  ULong  Offset[7];
  UByte * FaceCulling_Address[7];
  UShort VoxelClear[7];
  UShort VoxelNotTransparent[7];
  UShort VoxelNotFullOpacity[7];
  UByte  VoxelState[7];
  UShort Voxel;
  ZVoxelSector * Sector[7];
  ZVoxelType ** VoxelTypeTable;
  ZVoxelType * VoxelType;

  VoxelTypeTable = VoxelTypeManager->VoxelTable;

  Long Sector_x,Sector_y,Sector_z;

  Sector_x = x >> ZVOXELBLOCSHIFT_X; Sector_y = ZVOXELBLOCSHIFT_Y; Sector_z = ZVOXELBLOCSHIFT_Z;

  Sector[VOXEL_LEFT]    = FindSector( (x-1) >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z );
  Sector[VOXEL_RIGHT]   = FindSector( (x+1) >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z );
  Sector[VOXEL_INFRONT] = FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z-1) >> ZVOXELBLOCSHIFT_Z );
  Sector[VOXEL_BEHIND]  = FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z+1) >> ZVOXELBLOCSHIFT_Z );
  Sector[VOXEL_ABOVE]   = FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y + 1) >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z );
  Sector[VOXEL_BELOW]   = FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y - 1) >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z );
  Sector[VOXEL_INCENTER]= FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z );

  if (Sector[VOXEL_LEFT])
  {
    Offset[VOXEL_LEFT] = (y & ZVOXELBLOCMASK_Y) + ( ((x - 1) & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_LEFT] = Sector[VOXEL_LEFT]->Data + Offset[VOXEL_LEFT];
    VoxelClear[VOXEL_LEFT] = (*Voxel_Address[VOXEL_LEFT]  > 0) ? 0: 255;
    VoxelNotTransparent[VOXEL_LEFT] = VoxelTypeTable[*Voxel_Address[VOXEL_LEFT]]->Draw_TransparentRendering ? 0 : 255;
    VoxelNotFullOpacity[VOXEL_LEFT] = VoxelTypeTable[*Voxel_Address[VOXEL_LEFT]]->Draw_FullVoxelOpacity ? 0 : 255;
    FaceCulvoid ZVoxelWorld::SetVoxel_WithCullingUpdate(Long x, Long y, Long z, UShort VoxelValue)
{
  UShort * Voxel_Address[7];
  ULong  Offset[7];
  UByte * FaceCulling_Address[7];
  UShort VoxelClear[7];
  UShort VoxelNotTransparent[7];
  UShort VoxelNotFullOpacity[7];
  UByte  VoxelState[7];
  UShort Voxel;
  ZVoxelSector * Sector[7];
  ZVoxelType ** VoxelTypeTable;
  ZVoxelType * VoxelType;

  VoxelTypeTable = VoxelTypeManager->VoxelTable;

  Long Sector_x,Sector_y,Sector_z;

  Sector_x = x >> ZVOXELBLOCSHIFT_X; Sector_y = ZVOXELBLOCSHIFT_Y; Sector_z = ZVOXELBLOCSHIFT_Z;

  Sector[VOXEL_LEFT]    = FindSector( (x-1) >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z );
  Sector[VOXEL_RIGHT]   = FindSector( (x+1) >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z );
  Sector[VOXEL_INFRONT] = FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z-1) >> ZVOXELBLOCSHIFT_Z );
  Sector[VOXEL_BEHIND]  = FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z+1) >> ZVOXELBLOCSHIFT_Z );
  Sector[VOXEL_ABOVE]   = FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y + 1) >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z );
  Sector[VOXEL_BELOW]   = FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y - 1) >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z );
  Sector[VOXEL_INCENTER]= FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z );

  if (Sector[VOXEL_LEFT])
  {
    Offset[VOXEL_LEFT] = (y & ZVOXELBLOCMASK_Y) + ( ((x - 1) & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_LEFT] = Sector[VOXEL_LEFT]->Data + Offset[VOXEL_LEFT];
    VoxelClear[VOXEL_LEFT] = (*Voxel_Address[VOXEL_LEFT]  > 0) ? 0: 255;
    VoxelNotTransparent[VOXEL_LEFT] = VoxelTypeTable[*Voxel_Address[VOXEL_LEFT]]->Draw_TransparentRendering ? 0 : 255;
    VoxelNotFullOpacity[VOXEL_LEFT] = VoxelTypeTable[*Voxel_Address[VOXEL_LEFT]]->Draw_FullVoxelOpacity ? 0 : 255;
    FaceCulling_Address[VOXEL_LEFT] = Sector[VOXEL_LEFT]->FaceCulling + Offset[VOXEL_LEFT];

    Voxel = *Voxel_Address[VOXEL_LEFT];
    VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_LEFT] = ((Voxel==0) ? 1 : 0) & ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) & ( VoxelType->Draw_TransparentRendering ? 4 : 0 );

  } else { VoxelClear[VOXEL_LEFT]=0; VoxelState[VOXEL_LEFT] = 2; }

  if (Sector[VOXEL_RIGHT])
  {
    Offset[VOXEL_RIGHT] = (y & ZVOXELBLOCMASK_Y) + ( ((x + 1) & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_RIGHT] = Sector[VOXEL_RIGHT]->Data + Offset[VOXEL_RIGHT];
    VoxelClear[VOXEL_RIGHT] = (*Voxel_Address[VOXEL_RIGHT]  > 0) ? 0 : 255;
    VoxelNotTransparent[VOXEL_RIGHT] = VoxelTypeTable[*Voxel_Address[VOXEL_RIGHT]]->Draw_TransparentRendering ? 0 : 255;
    VoxelNotFullOpacity[VOXEL_RIGHT] = VoxelTypeTable[*Voxel_Address[VOXEL_RIGHT]]->Draw_FullVoxelOpacity ? 0 : 255;
    FaceCulling_Address[VOXEL_RIGHT] = Sector[VOXEL_RIGHT]->FaceCulling + Offset[VOXEL_RIGHT];

    Voxel = *Voxel_Address[VOXEL_RIGHT];
    VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_RIGHT] = ((Voxel==0) ? 1 : 0) & ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) & ( VoxelType->Draw_TransparentRendering ? 4 : 0 );

  } else { VoxelClear[VOXEL_RIGHT]=0; VoxelState[VOXEL_RIGHT] = 2; }

  if (Sector[VOXEL_INFRONT])
  {
    Offset[VOXEL_INFRONT] = (y & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ (((z - 1) & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_INFRONT] = Sector[VOXEL_INFRONT]->Data + Offset[VOXEL_INFRONT];
    VoxelClear[VOXEL_INFRONT] = (*Voxel_Address[VOXEL_INFRONT]  > 0) ? 0 : 255;
    VoxelNotTransparent[VOXEL_INFRONT] = VoxelTypeTable[*Voxel_Address[VOXEL_INFRONT]]->Draw_TransparentRendering ? 0 : 255;
    VoxelNotFullOpacity[VOXEL_INFRONT] = VoxelTypeTable[*Voxel_Address[VOXEL_INFRONT]]->Draw_FullVoxelOpacity ? 0 : 255;
    FaceCulling_Address[VOXEL_INFRONT] = Sector[VOXEL_INFRONT]->FaceCulling + Offset[VOXEL_INFRONT];

    Voxel = *Voxel_Address[VOXEL_INFRONT];
    VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_INFRONT] = ((Voxel==0) ? 1 : 0) & ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) & ( VoxelType->Draw_TransparentRendering ? 4 : 0 );

  } else { VoxelClear[VOXEL_INFRONT]=0; VoxelState[VOXEL_INFRONT] = 2; }

  if (Sector[VOXEL_BEHIND])
  {
    Offset[VOXEL_BEHIND] = (y & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ (((z + 1) & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_BEHIND] = Sector[VOXEL_BEHIND]->Data + Offset[VOXEL_BEHIND];
    VoxelClear[VOXEL_BEHIND] = (*Voxel_Address[VOXEL_BEHIND]  > 0) ? 0 : 255;
    VoxelNotTransparent[VOXEL_BEHIND] = VoxelTypeTable[*Voxel_Address[VOXEL_BEHIND]]->Draw_TransparentRendering ? 0 : 255;
    VoxelNotFullOpacity[VOXEL_BEHIND] = VoxelTypeTable[*Voxel_Address[VOXEL_BEHIND]]->Draw_FullVoxelOpacity ? 0 : 255;
    FaceCulling_Address[VOXEL_BEHIND] = Sector[VOXEL_BEHIND]->FaceCulling + Offset[VOXEL_BEHIND];

    Voxel = *Voxel_Address[VOXEL_BEHIND];
    VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_BEHIND] = ((Voxel==0) ? 1 : 0) & ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) & ( VoxelType->Draw_TransparentRendering ? 4 : 0 );

  } else { VoxelClear[VOXEL_BEHIND]=0; VoxelState[VOXEL_BEHIND] = 2; }

  if (Sector[VOXEL_ABOVE])
  {
    Offset[VOXEL_ABOVE] = ((y + 1) & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_ABOVE] = Sector[VOXEL_ABOVE]->Data + Offset[VOXEL_ABOVE];
    VoxelClear[VOXEL_ABOVE] = (*Voxel_Address[VOXEL_ABOVE]  > 0) ? 0 : 255;
    VoxelNotTransparent[VOXEL_ABOVE] = VoxelTypeTable[*Voxel_Address[VOXEL_ABOVE]]->Draw_TransparentRendering ? 0 : 255;
    VoxelNotFullOpacity[VOXEL_ABOVE] = VoxelTypeTable[*Voxel_Address[VOXEL_ABOVE]]->Draw_FullVoxelOpacity ? 0 : 255;
    FaceCulling_Address[VOXEL_ABOVE] = Sector[VOXEL_ABOVE]->FaceCulling + Offset[VOXEL_ABOVE];
    VoxelNotFullOpacity[VOXEL_ABOVE] = VoxelTypeTable[*Voxel_Address[VOXEL_ABOVE]]->Draw_FullVoxelOpacity ? 0 : 255;

    Voxel = *Voxel_Address[VOXEL_BEHIND];
    VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_BEHIND] = ((Voxel==0) ? 1 : 0) & ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) & ( VoxelType->Draw_TransparentRendering ? 4 : 0 );

  } else { VoxelClear[VOXEL_ABOVE]=0; VoxelState[VOXEL_BEHIND] = 2; }

  if (Sector[VOXEL_BELOW])
  {
    Offset[VOXEL_BELOW] = ((y - 1) & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_BELOW] = Sector[VOXEL_BELOW]->Data + Offset[VOXEL_BELOW];
    VoxelClear[VOXEL_BELOW] = (*Voxel_Address[VOXEL_BELOW]  > 0) ? 0 : 255;
    VoxelNotTransparent[VOXEL_BELOW] = VoxelTypeTable[*Voxel_Address[VOXEL_BELOW]]->Draw_TransparentRendering ? 0 : 255;
    VoxelNotFullOpacity[VOXEL_BELOW] = VoxelTypeTable[*Voxel_Address[VOXEL_BELOW]]->Draw_FullVoxelOpacity ? 0 : 255;
    FaceCulling_Address[VOXEL_BELOW] = Sector[VOXEL_BELOW]->FaceCulling + Offset[VOXEL_BELOW];

    Voxel = *Voxel_Address[VOXEL_BELOW];
    VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_BELOW] = ((Voxel==0) ? 1 : 0) & ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) & ( VoxelType->Draw_TransparentRendering ? 4 : 0 );

  } else { VoxelClear[VOXEL_BELOW]=0; VoxelState[VOXEL_BELOW] = 2; }

  if (Sector[VOXEL_INCENTER])
  {
    Offset[VOXEL_INCENTER] = (y & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_INCENTER] = Sector[VOXEL_INCENTER]->Data + Offset[VOXEL_INCENTER];
    *Voxel_Address[VOXEL_INCENTER] = VoxelValue;
    //printf("xyz:%ld,%ld,%ld Offset:%ld Setting %lx with %d\n",x,y,z,Offset[VOXEL_INCENTER],Voxel_Address[VOXEL_INCENTER], VoxelValue);
    VoxelClear[VOXEL_INCENTER] = (*Voxel_Address[VOXEL_INCENTER] > 0) ? 0 : 255;
    VoxelNotTransparent[VOXEL_INCENTER] = VoxelTypeTable[*Voxel_Address[VOXEL_INCENTER]]->Draw_TransparentRendering ? 0 : 255;
    VoxelNotFullOpacity[VOXEL_INCENTER] = VoxelTypeTable[*Voxel_Address[VOXEL_INCENTER]]->Draw_FullVoxelOpacity ? 0 : 255;
    FaceCulling_Address[VOXEL_INCENTER] = Sector[VOXEL_INCENTER]->FaceCulling + Offset[VOXEL_INCENTER];

    Voxel = *Voxel_Address[VOXEL_INCENTER];
    VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_INCENTER] = ((Voxel==0) ? 1 : 0) & ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) & ( VoxelType->Draw_TransparentRendering ? 4 : 0 );

  } else { VoxelClear[VOXEL_INCENTER]=0; VoxelState[VOXEL_INCENTER] = 2; }

  if (VoxelClear[VOXEL_INCENTER])
  {
    // Voxel is void

    if (Sector[VOXEL_INCENTER])  *FaceCulling_Address[VOXEL_INCENTER] = 0;
    if (Sector[VOXEL_LEFT])      {*FaceCulling_Address[VOXEL_LEFT]    &= 255 ^ DRAWFACE_RIGHT;  *FaceCulling_Address[VOXEL_LEFT]    |= (VoxelClear[VOXEL_LEFT]    ^ 255) & DRAWFACE_RIGHT;  }
    if (Sector[VOXEL_RIGHT])     {*FaceCulling_Address[VOXEL_RIGHT]   &= 255 ^ DRAWFACE_LEFT;   *FaceCulling_Address[VOXEL_RIGHT]   |= (VoxelClear[VOXEL_RIGHT]   ^ 255) & DRAWFACE_LEFT;   }
    if (Sector[VOXEL_INFRONT])   {*FaceCulling_Address[VOXEL_INFRONT] &= 255 ^ DRAWFACE_BEHIND; *FaceCulling_Address[VOXEL_INFRONT] |= (VoxelClear[VOXEL_INFRONT] ^ 255) & DRAWFACE_BEHIND; }
    if (Sector[VOXEL_BEHIND])    {*FaceCulling_Address[VOXEL_BEHIND]  &= 255 ^ DRAWFACE_AHEAD;  *FaceCulling_Address[VOXEL_BEHIND]  |= (VoxelClear[VOXEL_BEHIND]  ^ 255) & DRAWFACE_AHEAD;  }
    if (Sector[VOXEL_ABOVE])     {*FaceCulling_Address[VOXEL_ABOVE]   &= 255 ^ DRAWFACE_BELOW;  *FaceCulling_Address[VOXEL_ABOVE]   |= (VoxelClear[VOXEL_ABOVE]   ^ 255) & DRAWFACE_BELOW;  }
    if (Sector[VOXEL_BELOW])     {*FaceCulling_Address[VOXEL_BELOW]   &= 255 ^ DRAWFACE_ABOVE;  *FaceCulling_Address[VOXEL_BELOW]   |= (VoxelClear[VOXEL_BELOW]   ^ 255) & DRAWFACE_ABOVE;  }


    printf("Null\n");
  }
  else if (VoxelNotTransparent[VOXEL_INCENTER])
  {
    printf("Opaque\n");
    // Voxel opaque
    if (Sector[VOXEL_INCENTER])  *FaceCulling_Address[VOXEL_INCENTER] =   ( (VoxelClear[VOXEL_LEFT]   | VoxelNotFullOpacity[VOXEL_LEFT]   ) & DRAWFACE_LEFT)
                                                                        | ( (VoxelClear[VOXEL_RIGHT]  | VoxelNotFullOpacity[VOXEL_RIGHT]  ) & DRAWFACE_RIGHT)
                                                                        | ( (VoxelClear[VOXEL_INFRONT]| VoxelNotFullOpacity[VOXEL_INFRONT]) & DRAWFACE_AHEAD)
                                                                        | ( (VoxelClear[VOXEL_BEHIND] | VoxelNotFullOpacity[VOXEL_BEHIND] ) & DRAWFACE_BEHIND)
                                                                        | ( (VoxelClear[VOXEL_ABOVE]  | VoxelNotFullOpacity[VOXEL_ABOVE]  ) & DRAWFACE_ABOVE)
                                                                        | ( (VoxelClear[VOXEL_BELOW]  | VoxelNotFullOpacity[VOXEL_BELOW]  ) & DRAWFACE_BELOW);

    if (Sector[VOXEL_LEFT])      {*FaceCulling_Address[VOXEL_LEFT] &= 255 ^ DRAWFACE_RIGHT;     *FaceCulling_Address[VOXEL_LEFT]    |= (VoxelNotFullOpacity[VOXEL_LEFT])   & DRAWFACE_RIGHT; }
    if (Sector[VOXEL_RIGHT])     {*FaceCulling_Address[VOXEL_RIGHT] &= 255 ^ DRAWFACE_LEFT;     *FaceCulling_Address[VOXEL_RIGHT]   |= (VoxelNotFullOpacity[VOXEL_RIGHT])  & DRAWFACE_LEFT; }
    if (Sector[VOXEL_INFRONT])   {*FaceCulling_Address[VOXEL_INFRONT] &= 255 ^ DRAWFACE_BEHIND; *FaceCulling_Address[VOXEL_INFRONT] |= (VoxelNotFullOpacity[VOXEL_INFRONT])& DRAWFACE_BEHIND; }
    if (Sector[VOXEL_BEHIND])    {*FaceCulling_Address[VOXEL_BEHIND] &= 255 ^ DRAWFACE_AHEAD;   *FaceCulling_Address[VOXEL_BEHIND]  |= (VoxelNotFullOpacity[VOXEL_BEHIND]) & DRAWFACE_AHEAD; }
    if (Sector[VOXEL_ABOVE])     {*FaceCulling_Address[VOXEL_ABOVE] &= 255 ^ DRAWFACE_BELOW;    *FaceCulling_Address[VOXEL_ABOVE]   |= (VoxelNotFullOpacity[VOXEL_ABOVE])  & DRAWFACE_BELOW; }
    if (Sector[VOXEL_BELOW])     {*FaceCulling_Address[VOXEL_BELOW] &= 255 ^ DRAWFACE_ABOVE;    *FaceCulling_Address[VOXEL_BELOW]   |= (VoxelNotFullOpacity[VOXEL_BELOW])  & DRAWFACE_ABOVE; }


  }
  else
  {
    printf("Transparent\n");

    // Voxel transparent
    if (Sector[VOXEL_INCENTER])  *FaceCulling_Address[VOXEL_INCENTER] =   (VoxelClear[VOXEL_LEFT]   & DRAWFACE_LEFT)
                                                                        | (VoxelClear[VOXEL_RIGHT]  & DRAWFACE_RIGHT)
                                                                        | (VoxelClear[VOXEL_INFRONT]& DRAWFACE_AHEAD)
                                                                        | (VoxelClear[VOXEL_BEHIND] & DRAWFACE_BEHIND)
                                                                        | (VoxelClear[VOXEL_ABOVE]  & DRAWFACE_ABOVE)
                                                                        | (VoxelClear[VOXEL_BELOW]  & DRAWFACE_BELOW);
  }


  UShort CenterNotOpaque = VoxelClear[VOXEL_INCENTER] | VoxelNotFullOpacity[VOXEL_INCENTER];
  if (Sector[VOXEL_LEFT])      {*FaceCulling_Address[VOXEL_LEFT] &= 255 ^ DRAWFACE_RIGHT;
                                *FaceCulling_Address[VOXEL_LEFT] |= (CenterNotOpaque  & VoxelNotTransparent[VOXEL_LEFT]) & DRAWFACE_RIGHT; }
  if (Sector[VOXEL_RIGHT])     {*FaceCulling_Address[VOXEL_RIGHT] &= 255 ^ DRAWFACE_LEFT;
                                *FaceCulling_Address[VOXEL_RIGHT] |= (CenterNotOpaque & VoxelNotTransparent[VOXEL_RIGHT]) & DRAWFACE_LEFT; }
  if (Sector[VOXEL_INFRONT])   {*FaceCulling_Address[VOXEL_INFRONT] &= 255 ^ DRAWFACE_BEHIND;
                                *FaceCulling_Address[VOXEL_INFRONT] |= (CenterNotOpaque & VoxelNotTransparent[VOXEL_INFRONT])& DRAWFACE_BEHIND; }
  if (Sector[VOXEL_BEHIND])    {*FaceCulling_Address[VOXEL_BEHIND] &= 255 ^ DRAWFACE_AHEAD;
                                *FaceCulling_Address[VOXEL_BEHIND] |= (CenterNotOpaque & VoxelNotTransparent[VOXEL_BEHIND]) & DRAWFACE_AHEAD; }
  if (Sector[VOXEL_ABOVE])     {*FaceCulling_Address[VOXEL_ABOVE] &= 255 ^ DRAWFACE_BELOW;
                                *FaceCulling_Address[VOXEL_ABOVE] |= (CenterNotOpaque & VoxelNotTransparent[VOXEL_ABOVE]) & DRAWFACE_BELOW; }
  if (Sector[VOXEL_BELOW])     {*FaceCulling_Address[VOXEL_BELOW] &= 255 ^ DRAWFACE_ABOVE;
                                *FaceCulling_Address[VOXEL_BELOW] |= (CenterNotOpaque & VoxelNotTransparent[VOXEL_BELOW])& DRAWFACE_ABOVE; }



  Sector[VOXEL_INCENTER]->Flag_Render_Dirty = true;
  Sector[VOXEL_LEFT    ]->Flag_Render_Dirty = true;
  Sector[VOXEL_RIGHT   ]->Flag_Render_Dirty = true;
  Sector[VOXEL_INFRONT ]->Flag_Render_Dirty = true;
  Sector[VOXEL_BEHIND  ]->Flag_Render_Dirty = true;
  Sector[VOXEL_ABOVE   ]->Flag_Render_Dirty = true;
  Sector[VOXEL_BELOW   ]->Flag_Render_Dirty = true;
  Sector[VOXEL_INCENTER]->Flag_Void_Regular = false;
  Sector[VOXEL_LEFT    ]->Flag_Void_Regular = false;
  Sector[VOXEL_RIGHT   ]->Flag_Void_Regular = false;
  Sector[VOXEL_INFRONT ]->Flag_Void_Regular = false;
  Sector[VOXEL_BEHIND  ]->Flag_Void_Regular = false;
  Sector[VOXEL_ABOVE   ]->Flag_Void_Regular = false;
  Sector[VOXEL_BELOW   ]->Flag_Void_Regular = false;
  Sector[VOXEL_INCENTER]->Flag_Void_Transparent = false;
  Sector[VOXEL_LEFT    ]->Flag_Void_Transparent = false;
  Sector[VOXEL_RIGHT   ]->Flag_Void_Transparent = false;
  Sector[VOXEL_INFRONT ]->Flag_Void_Transparent = false;
  Sector[VOXEL_BEHIND  ]->Flag_Void_Transparent = false;
  Sector[VOXEL_ABOVE   ]->Flag_Void_Transparent = false;
  Sector[VOXEL_BELOW   ]->Flag_Void_Transparent = false;

}ling_Address[VOXEL_LEFT] = Sector[VOXEL_LEFT]->FaceCulling + Offset[VOXEL_LEFT];

    Voxel = *Voxel_Address[VOXEL_LEFT];
    VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_LEFT] = ((Voxel==0) ? 1 : 0) & ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) & ( VoxelType->Draw_TransparentRendering ? 4 : 0 );

  } else { VoxelClear[VOXEL_LEFT]=0; VoxelState[VOXEL_LEFT] = 2; }

  if (Sector[VOXEL_RIGHT])
  {
    Offset[VOXEL_RIGHT] = (y & ZVOXELBLOCMASK_Y) + ( ((x + 1) & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_RIGHT] = Sector[VOXEL_RIGHT]->Data + Offset[VOXEL_RIGHT];
    VoxelClear[VOXEL_RIGHT] = (*Voxel_Address[VOXEL_RIGHT]  > 0) ? 0 : 255;
    VoxelNotTransparent[VOXEL_RIGHT] = VoxelTypeTable[*Voxel_Address[VOXEL_RIGHT]]->Draw_TransparentRendering ? 0 : 255;
    VoxelNotFullOpacity[VOXEL_RIGHT] = VoxelTypeTable[*Voxel_Address[VOXEL_RIGHT]]->Draw_FullVoxelOpacity ? 0 : 255;
    FaceCulling_Address[VOXEL_RIGHT] = Sector[VOXEL_RIGHT]->FaceCulling + Offset[VOXEL_RIGHT];

    Voxel = *Voxel_Address[VOXEL_RIGHT];
    VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_RIGHT] = ((Voxel==0) ? 1 : 0) & ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) & ( VoxelType->Draw_TransparentRendering ? 4 : 0 );

  } else { VoxelClear[VOXEL_RIGHT]=0; VoxelState[VOXEL_RIGHT] = 2; }

  if (Sector[VOXEL_INFRONT])
  {
    Offset[VOXEL_INFRONT] = (y & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ (((z - 1) & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_INFRONT] = Sector[VOXEL_INFRONT]->Data + Offset[VOXEL_INFRONT];
    VoxelClear[VOXEL_INFRONT] = (*Voxel_Address[VOXEL_INFRONT]  > 0) ? 0 : 255;
    VoxelNotTransparent[VOXEL_INFRONT] = VoxelTypeTable[*Voxel_Address[VOXEL_INFRONT]]->Draw_TransparentRendering ? 0 : 255;
    VoxelNotFullOpacity[VOXEL_INFRONT] = VoxelTypeTable[*Voxel_Address[VOXEL_INFRONT]]->Draw_FullVoxelOpacity ? 0 : 255;
    FaceCulling_Address[VOXEL_INFRONT] = Sector[VOXEL_INFRONT]->FaceCulling + Offset[VOXEL_INFRONT];

    Voxel = *Voxel_Address[VOXEL_INFRONT];
    VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_INFRONT] = ((Voxel==0) ? 1 : 0) & ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) & ( VoxelType->Draw_TransparentRendering ? 4 : 0 );

  } else { VoxelClear[VOXEL_INFRONT]=0; VoxelState[VOXEL_INFRONT] = 2; }

  if (Sector[VOXEL_BEHIND])
  {
    Offset[VOXEL_BEHIND] = (y & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ (((z + 1) & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_BEHIND] = Sector[VOXEL_BEHIND]->Data + Offset[VOXEL_BEHIND];
    VoxelClear[VOXEL_BEHIND] = (*Voxel_Address[VOXEL_BEHIND]  > 0) ? 0 : 255;
    VoxelNotTransparent[VOXEL_BEHIND] = VoxelTypeTable[*Voxel_Address[VOXEL_BEHIND]]->Draw_TransparentRendering ? 0 : 255;
    VoxelNotFullOpacity[VOXEL_BEHIND] = VoxelTypeTable[*Voxel_Address[VOXEL_BEHIND]]->Draw_FullVoxelOpacity ? 0 : 255;
    FaceCulling_Address[VOXEL_BEHIND] = Sector[VOXEL_BEHIND]->FaceCulling + Offset[VOXEL_BEHIND];

    Voxel = *Voxel_Address[VOXEL_BEHIND];
    VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_BEHIND] = ((Voxel==0) ? 1 : 0) & ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) & ( VoxelType->Draw_TransparentRendering ? 4 : 0 );

  } else { VoxelClear[VOXEL_BEHIND]=0; VoxelState[VOXEL_BEHIND] = 2; }

  if (Sector[VOXEL_ABOVE])
  {
    Offset[VOXEL_ABOVE] = ((y + 1) & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_ABOVE] = Sector[VOXEL_ABOVE]->Data + Offset[VOXEL_ABOVE];
    VoxelClear[VOXEL_ABOVE] = (*Voxel_Address[VOXEL_ABOVE]  > 0) ? 0 : 255;
    VoxelNotTransparent[VOXEL_ABOVE] = VoxelTypeTable[*Voxel_Address[VOXEL_ABOVE]]->Draw_TransparentRendering ? 0 : 255;
    VoxelNotFullOpacity[VOXEL_ABOVE] = VoxelTypeTable[*Voxel_Address[VOXEL_ABOVE]]->Draw_FullVoxelOpacity ? 0 : 255;
    FaceCulling_Address[VOXEL_ABOVE] = Sector[VOXEL_ABOVE]->FaceCulling + Offset[VOXEL_ABOVE];
    VoxelNotFullOpacity[VOXEL_ABOVE] = VoxelTypeTable[*Voxel_Address[VOXEL_ABOVE]]->Draw_FullVoxelOpacity ? 0 : 255;

    Voxel = *Voxel_Address[VOXEL_BEHIND];
    VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_BEHIND] = ((Voxel==0) ? 1 : 0) & ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) & ( VoxelType->Draw_TransparentRendering ? 4 : 0 );

  } else { VoxelClear[VOXEL_ABOVE]=0; VoxelState[VOXEL_BEHIND] = 2; }

  if (Sector[VOXEL_BELOW])
  {
    Offset[VOXEL_BELOW] = ((y - 1) & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_BELOW] = Sector[VOXEL_BELOW]->Data + Offset[VOXEL_BELOW];
    VoxelClear[VOXEL_BELOW] = (*Voxel_Address[VOXEL_BELOW]  > 0) ? 0 : 255;
    VoxelNotTransparent[VOXEL_BELOW] = VoxelTypeTable[*Voxel_Address[VOXEL_BELOW]]->Draw_TransparentRendering ? 0 : 255;
    VoxelNotFullOpacity[VOXEL_BELOW] = VoxelTypeTable[*Voxel_Address[VOXEL_BELOW]]->Draw_FullVoxelOpacity ? 0 : 255;
    FaceCulling_Address[VOXEL_BELOW] = Sector[VOXEL_BELOW]->FaceCulling + Offset[VOXEL_BELOW];

    Voxel = *Voxel_Address[VOXEL_BELOW];
    VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_BELOW] = ((Voxel==0) ? 1 : 0) & ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) & ( VoxelType->Draw_TransparentRendering ? 4 : 0 );

  } else { VoxelClear[VOXEL_BELOW]=0; VoxelState[VOXEL_BELOW] = 2; }

  if (Sector[VOXEL_INCENTER])
  {
    Offset[VOXEL_INCENTER] = (y & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )+ ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
    Voxel_Address[VOXEL_INCENTER] = Sector[VOXEL_INCENTER]->Data + Offset[VOXEL_INCENTER];
    *Voxel_Address[VOXEL_INCENTER] = VoxelValue;
    //printf("xyz:%ld,%ld,%ld Offset:%ld Setting %lx with %d\n",x,y,z,Offset[VOXEL_INCENTER],Voxel_Address[VOXEL_INCENTER], VoxelValue);
    VoxelClear[VOXEL_INCENTER] = (*Voxel_Address[VOXEL_INCENTER] > 0) ? 0 : 255;
    VoxelNotTransparent[VOXEL_INCENTER] = VoxelTypeTable[*Voxel_Address[VOXEL_INCENTER]]->Draw_TransparentRendering ? 0 : 255;
    VoxelNotFullOpacity[VOXEL_INCENTER] = VoxelTypeTable[*Voxel_Address[VOXEL_INCENTER]]->Draw_FullVoxelOpacity ? 0 : 255;
    FaceCulling_Address[VOXEL_INCENTER] = Sector[VOXEL_INCENTER]->FaceCulling + Offset[VOXEL_INCENTER];

    Voxel = *Voxel_Address[VOXEL_INCENTER];
    VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_INCENTER] = ((Voxel==0) ? 1 : 0) & ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) & ( VoxelType->Draw_TransparentRendering ? 4 : 0 );

  } else { VoxelClear[VOXEL_INCENTER]=0; VoxelState[VOXEL_INCENTER] = 2; }

  if (VoxelClear[VOXEL_INCENTER])
  {
    // Voxel is void

    if (Sector[VOXEL_INCENTER])  *FaceCulling_Address[VOXEL_INCENTER] = 0;
    if (Sector[VOXEL_LEFT])      {*FaceCulling_Address[VOXEL_LEFT]    &= 255 ^ DRAWFACE_RIGHT;  *FaceCulling_Address[VOXEL_LEFT]    |= (VoxelClear[VOXEL_LEFT]    ^ 255) & DRAWFACE_RIGHT;  }
    if (Sector[VOXEL_RIGHT])     {*FaceCulling_Address[VOXEL_RIGHT]   &= 255 ^ DRAWFACE_LEFT;   *FaceCulling_Address[VOXEL_RIGHT]   |= (VoxelClear[VOXEL_RIGHT]   ^ 255) & DRAWFACE_LEFT;   }
    if (Sector[VOXEL_INFRONT])   {*FaceCulling_Address[VOXEL_INFRONT] &= 255 ^ DRAWFACE_BEHIND; *FaceCulling_Address[VOXEL_INFRONT] |= (VoxelClear[VOXEL_INFRONT] ^ 255) & DRAWFACE_BEHIND; }
    if (Sector[VOXEL_BEHIND])    {*FaceCulling_Address[VOXEL_BEHIND]  &= 255 ^ DRAWFACE_AHEAD;  *FaceCulling_Address[VOXEL_BEHIND]  |= (VoxelClear[VOXEL_BEHIND]  ^ 255) & DRAWFACE_AHEAD;  }
    if (Sector[VOXEL_ABOVE])     {*FaceCulling_Address[VOXEL_ABOVE]   &= 255 ^ DRAWFACE_BELOW;  *FaceCulling_Address[VOXEL_ABOVE]   |= (VoxelClear[VOXEL_ABOVE]   ^ 255) & DRAWFACE_BELOW;  }
    if (Sector[VOXEL_BELOW])     {*FaceCulling_Address[VOXEL_BELOW]   &= 255 ^ DRAWFACE_ABOVE;  *FaceCulling_Address[VOXEL_BELOW]   |= (VoxelClear[VOXEL_BELOW]   ^ 255) & DRAWFACE_ABOVE;  }


    printf("Null\n");
  }
  else if (VoxelNotTransparent[VOXEL_INCENTER])
  {
    printf("Opaque\n");
    // Voxel opaque
    if (Sector[VOXEL_INCENTER])  *FaceCulling_Address[VOXEL_INCENTER] =   ( (VoxelClear[VOXEL_LEFT]   | VoxelNotFullOpacity[VOXEL_LEFT]   ) & DRAWFACE_LEFT)
                                                                        | ( (VoxelClear[VOXEL_RIGHT]  | VoxelNotFullOpacity[VOXEL_RIGHT]  ) & DRAWFACE_RIGHT)
                                                                        | ( (VoxelClear[VOXEL_INFRONT]| VoxelNotFullOpacity[VOXEL_INFRONT]) & DRAWFACE_AHEAD)
                                                                        | ( (VoxelClear[VOXEL_BEHIND] | VoxelNotFullOpacity[VOXEL_BEHIND] ) & DRAWFACE_BEHIND)
                                                                        | ( (VoxelClear[VOXEL_ABOVE]  | VoxelNotFullOpacity[VOXEL_ABOVE]  ) & DRAWFACE_ABOVE)
                                                                        | ( (VoxelClear[VOXEL_BELOW]  | VoxelNotFullOpacity[VOXEL_BELOW]  ) & DRAWFACE_BELOW);

    if (Sector[VOXEL_LEFT])      {*FaceCulling_Address[VOXEL_LEFT] &= 255 ^ DRAWFACE_RIGHT;     *FaceCulling_Address[VOXEL_LEFT]    |= (VoxelNotFullOpacity[VOXEL_LEFT])   & DRAWFACE_RIGHT; }
    if (Sector[VOXEL_RIGHT])     {*FaceCulling_Address[VOXEL_RIGHT] &= 255 ^ DRAWFACE_LEFT;     *FaceCulling_Address[VOXEL_RIGHT]   |= (VoxelNotFullOpacity[VOXEL_RIGHT])  & DRAWFACE_LEFT; }
    if (Sector[VOXEL_INFRONT])   {*FaceCulling_Address[VOXEL_INFRONT] &= 255 ^ DRAWFACE_BEHIND; *FaceCulling_Address[VOXEL_INFRONT] |= (VoxelNotFullOpacity[VOXEL_INFRONT])& DRAWFACE_BEHIND; }
    if (Sector[VOXEL_BEHIND])    {*FaceCulling_Address[VOXEL_BEHIND] &= 255 ^ DRAWFACE_AHEAD;   *FaceCulling_Address[VOXEL_BEHIND]  |= (VoxelNotFullOpacity[VOXEL_BEHIND]) & DRAWFACE_AHEAD; }
    if (Sector[VOXEL_ABOVE])     {*FaceCulling_Address[VOXEL_ABOVE] &= 255 ^ DRAWFACE_BELOW;    *FaceCulling_Address[VOXEL_ABOVE]   |= (VoxelNotFullOpacity[VOXEL_ABOVE])  & DRAWFACE_BELOW; }
    if (Sector[VOXEL_BELOW])     {*FaceCulling_Address[VOXEL_BELOW] &= 255 ^ DRAWFACE_ABOVE;    *FaceCulling_Address[VOXEL_BELOW]   |= (VoxelNotFullOpacity[VOXEL_BELOW])  & DRAWFACE_ABOVE; }


  }
  else
  {
    printf("Transparent\n");

    // Voxel transparent
    if (Sector[VOXEL_INCENTER])  *FaceCulling_Address[VOXEL_INCENTER] =   (VoxelClear[VOXEL_LEFT]   & DRAWFACE_LEFT)
                                                                        | (VoxelClear[VOXEL_RIGHT]  & DRAWFACE_RIGHT)
                                                                        | (VoxelClear[VOXEL_INFRONT]& DRAWFACE_AHEAD)
                                                                        | (VoxelClear[VOXEL_BEHIND] & DRAWFACE_BEHIND)
                                                                        | (VoxelClear[VOXEL_ABOVE]  & DRAWFACE_ABOVE)
                                                                        | (VoxelClear[VOXEL_BELOW]  & DRAWFACE_BELOW);
  }


  UShort CenterNotOpaque = VoxelClear[VOXEL_INCENTER] | VoxelNotFullOpacity[VOXEL_INCENTER];
  if (Sector[VOXEL_LEFT])      {*FaceCulling_Address[VOXEL_LEFT] &= 255 ^ DRAWFACE_RIGHT;
                                *FaceCulling_Address[VOXEL_LEFT] |= (CenterNotOpaque  & VoxelNotTransparent[VOXEL_LEFT]) & DRAWFACE_RIGHT; }
  if (Sector[VOXEL_RIGHT])     {*FaceCulling_Address[VOXEL_RIGHT] &= 255 ^ DRAWFACE_LEFT;
                                *FaceCulling_Address[VOXEL_RIGHT] |= (CenterNotOpaque & VoxelNotTransparent[VOXEL_RIGHT]) & DRAWFACE_LEFT; }
  if (Sector[VOXEL_INFRONT])   {*FaceCulling_Address[VOXEL_INFRONT] &= 255 ^ DRAWFACE_BEHIND;
                                *FaceCulling_Address[VOXEL_INFRONT] |= (CenterNotOpaque & VoxelNotTransparent[VOXEL_INFRONT])& DRAWFACE_BEHIND; }
  if (Sector[VOXEL_BEHIND])    {*FaceCulling_Address[VOXEL_BEHIND] &= 255 ^ DRAWFACE_AHEAD;
                                *FaceCulling_Address[VOXEL_BEHIND] |= (CenterNotOpaque & VoxelNotTransparent[VOXEL_BEHIND]) & DRAWFACE_AHEAD; }
  if (Sector[VOXEL_ABOVE])     {*FaceCulling_Address[VOXEL_ABOVE] &= 255 ^ DRAWFACE_BELOW;
                                *FaceCulling_Address[VOXEL_ABOVE] |= (CenterNotOpaque & VoxelNotTransparent[VOXEL_ABOVE]) & DRAWFACE_BELOW; }
  if (Sector[VOXEL_BELOW])     {*FaceCulling_Address[VOXEL_BELOW] &= 255 ^ DRAWFACE_ABOVE;
                                *FaceCulling_Address[VOXEL_BELOW] |= (CenterNotOpaque & VoxelNotTransparent[VOXEL_BELOW])& DRAWFACE_ABOVE; }



  Sector[VOXEL_INCENTER]->Flag_Render_Dirty = true;
  Sector[VOXEL_LEFT    ]->Flag_Render_Dirty = true;
  Sector[VOXEL_RIGHT   ]->Flag_Render_Dirty = true;
  Sector[VOXEL_INFRONT ]->Flag_Render_Dirty = true;
  Sector[VOXEL_BEHIND  ]->Flag_Render_Dirty = true;
  Sector[VOXEL_ABOVE   ]->Flag_Render_Dirty = true;
  Sector[VOXEL_BELOW   ]->Flag_Render_Dirty = true;
  Sector[VOXEL_INCENTER]->Flag_Void_Regular = false;
  Sector[VOXEL_LEFT    ]->Flag_Void_Regular = false;
  Sector[VOXEL_RIGHT   ]->Flag_Void_Regular = false;
  Sector[VOXEL_INFRONT ]->Flag_Void_Regular = false;
  Sector[VOXEL_BEHIND  ]->Flag_Void_Regular = false;
  Sector[VOXEL_ABOVE   ]->Flag_Void_Regular = false;
  Sector[VOXEL_BELOW   ]->Flag_Void_Regular = false;
  Sector[VOXEL_INCENTER]->Flag_Void_Transparent = false;
  Sector[VOXEL_LEFT    ]->Flag_Void_Transparent = false;
  Sector[VOXEL_RIGHT   ]->Flag_Void_Transparent = false;
  Sector[VOXEL_INFRONT ]->Flag_Void_Transparent = false;
  Sector[VOXEL_BEHIND  ]->Flag_Void_Transparent = false;
  Sector[VOXEL_ABOVE   ]->Flag_Void_Transparent = false;
  Sector[VOXEL_BELOW   ]->Flag_Void_Transparent = false;

}
*/


UShort ExtFaceStateTable[][8] =
{
  { // State 0: Clear = no FullOpaque = no TranspRend = no
    255 , // Clear = 0 FullOpaque = 0 TranspRend = 0
    0   , // Clear = 1 FullOpaque = 0 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 0
    0   , // Clear = 1 FullOpaque = 1 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 0 TranspRend = 1
    0   , // Clear = 1 FullOpaque = 0 TranspRend = 1

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 1
    0   , // Clear = 1 FullOpaque = 1 TranspRend = 1
  },
  { // State 1: Clear = yes FullOpaque = no TranspRend = no
    255 , // Clear = 0 FullOpaque = 0 TranspRend = 0
    0   , // Clear = 1 FullOpaque = 0 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 0
    0   , // Clear = 1 FullOpaque = 1 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 0 TranspRend =  Long Sector_x,Sector_y,Sector_z; 1
    0   , // Clear = 1 FullOpaque = 0 TranspRend = 1

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 1
    0   , // Clear = 1 FullOpaque = 1 TranspRend = 1
  },
  { // State 2: Clear = no FullOpaque = yes TranspRend = no
    255 , // Clear = 0 FullOpaque = 0 TranspRend = 0
    0   , // Clear = 1 FullOpaque = 0 TranspRend = 0

    0   , // Clear = 0 FullOpaque = 1 TranspRend = 0
    0   , // Clear = 1 FullOpaque = 1 TranspRend = 0

    0   , // Clear = 0 FullOpaque = 0 TranspRend = 1
    0   , // Clear = 1 FullOpaque = 0 TranspRend = 1

    0   , // Clear = 0 FullOpaque = 1 TranspRend = 1
    0   , // Clear = 1 FullOpaque = 1 TranspRend = 1
  },
  { // State 3 : Clear = yes FullOpaque = yes TranspRend = no
    255 , // Clear = 0 FullOpaque = 0 TranspRend = 0
    0   , // Clear = 1 FullOpaque = 0 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 0
    0   , // Clear = 1 FullOpaque = 1 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 0 TranspRend = 1
    0   , // Clear = 1 FullOpaque = 0 TranspRend = 1

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 1
    0   , // Clear = 1 FullOpaque = 1 TranspRend = 1
  },
  { // State 4 : Clear = no FullOpaque = no TranspRend = yes
    255 , // Clear = 0 FullOpaque = 0 TranspRend = 0
    0   , // Clear = 1 FullOpaque = 0 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 0
    0   , // Clear = 1 FullOpaque = 1 TranspRend = 0

    0   , // Clear = 0 FullOpaque = 0 TranspRend = 1
    0   , // Clear = 1 FullOpaque = 0 TranspRend = 1

    0   , // Clear = 0 FullOpaque = 1 TranspRend = 1
    0   , // Clear = 1 FullOpaque = 1 TranspRend = 1
  },
  { // State 5: Clear = yes FullOpaque = no TranspRend = yes
    255 , // Clear = 0 FullOpaque = 0 TranspRend = 0
    0   , // Clear = 1 FullOpaque = 0 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 0
    0   , // Clear = 1 FullOpaque = 1 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 0 TranspRend = 1
    0   , // Clear = 1 FullOpaque = 0 TranspRend = 1

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 1
    0   , // Clear = 1 FullOpaque = 1 TranspRend = 1
  },
  { // State 6: Clear = no FullOpaque = yes TranspRend = yes
    255 , // Clear = 0 FullOpaque = 0 TranspRend = 0
    0   , // Clear = 1 FullOpaque = 0 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 0
    0   , // Clear = 1 FullOpaque = 1 TranspRend = 0

    0   , // Clear = 0 FullOpaque = 0 TranspRend = 1
    0   , // Clear = 1 FullOpaque = 0 TranspRend = 1

    0   , // Clear = 0 FullOpaque = 1 TranspRend = 1
    0   , // Clear = 1 FullOpaque = 1 TranspRend = 1
  },
  { // State 7: Clear = yes FullOpaque = yes TranspRend = yes
    255 , // Clear = 0 FullOpaque = 0 TranspRend = 0
    0   , // Clear = 1 FullOpaque = 0 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 0
    0   , // Clear = 1 FullOpaque = 1 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 0 TranspRend = 1
    0   , // Clear = 1 FullOpaque = 0 TranspRend = 1

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 1
    0   , // Clear = 1 FullOpaque = 1 TranspRend = 1
  }
};

UShort IntFaceStateTable[][8] =
{
  { // State 0: Clear = no FullOpaque = no TranspRend = no
    255 , // Clear = 0 FullOpaque = 0 TranspRend = 0
    255 , // Clear = 1 FullOpaque = 0 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 0
    255 , // Clear = 1 FullOpaque = 1 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 0 TranspRend = 1
    255 , // Clear = 1 FullOpaque = 0 TranspRend = 1

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 1
    255 , // Clear = 1 FullOpaque = 1 TranspRend = 1
  },
  { // State 1: Clear = yes FullOpaque = no TranspRend = no
    0   , // Clear = 0 FullOpaque = 0 TranspRend = 0
    0   , // Clear = 1 FullOpaque = 0 TranspRend = 0

    0   , // Clear = 0 FullOpaque = 1 TranspRend = 0
    0   , // Clear = 1 FullOpaque = 1 TranspRend = 0

    0   , // Clear = 0 FullOpaque = 0 TranspRend = 1
    0   , // Clear = 1 FullOpaque = 0 TranspRend = 1

    0   , // Clear = 0 FullOpaque = 1 TranspRend = 1
    0   , // Clear = 1 FullOpaque = 1 TranspRend = 1
  },
  { // State 2: Clear = no FullOpaque = yes TranspRend = no
    255 , // Clear = 0 FullOpaque = 0 TranspRend = 0
    255 , // Clear = 1 FullOpaque = 0 TranspRend = 0

    0   , // Clear = 0 FullOpaque = 1 TranspRend = 0
    255 , // Clear = 1 FullOpaque = 1 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 0 TranspRend = 1
    255 , // Clear = 1 FullOpaque = 0 TranspRend = 1

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 1
    255 , // Clear = 1 FullOpaque = 1 TranspRend = 1
  },
  { // State 3 : Clear = yes FullOpaque = yes TranspRend = no
    255 , // Clear = 0 FullOpaque = 0 TranspRend = 0
    255 , // Clear = 1 FullOpaque = 0 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 0
    255 , // Clear = 1 FullOpaque = 1 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 0 TranspRend = 1
    255 , // Clear = 1 FullOpaque = 0 TranspRend = 1

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 1
    255 , // Clear = 1 FullOpaque = 1 TranspRend = 1
  },
  { // State 4 : Clear = no FullOpaque = no TranspRend = yes
    255 , // Clear = 0 FullOpaque = 0 TranspRend = 0
    255 , // Clear = 1 FullOpaque = 0 TranspRend = 0

    0   , // Clear = 0 FullOpaque = 1 TranspRend = 0
    255 , // Clear = 1 FullOpaque = 1 TranspRend = 0

    0   , // Clear = 0 FullOpaque = 0 TranspRend = 1
    255 , // Clear = 1 FullOpaque = 0 TranspRend = 1

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 1
    255 , // Clear = 1 FullOpaque = 1 TranspRend = 1
  },
  { // State 5: Clear = yes FullOpaque = no TranspRend = yes
    255 , // Clear = 0 FullOpaque = 0 TranspRend = 0
    255 , // Clear = 1 FullOpaque = 0 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 0
    255 , // Clear = 1 FullOpaque = 1 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 0 TranspRend = 1
    255 , // Clear = 1 FullOpaque = 0 TranspRend = 1

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 1
    255 , // Clear = 1 FullOpaque = 1 TranspRend = 1
  },
  { // State 6: Clear = no FullOpaque = yes TranspRend = yes
    255 , // Clear = 0 FullOpaque = 0 TranspRend = 0
    255 , // Clear = 1 FullOpaque = 0 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 0
    255 , // Clear = 1 FullOpaque = 1 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 0 TranspRend = 1
    255 , // Clear = 1 FullOpaque = 0 TranspRend = 1

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 1
    255 , // Clear = 1 FullOpaque = 1 TranspRend = 1
  },
  { // State 7: Clear = yes FullOpaque = yes TranspRend = yes
    255 , // Clear = 0 FullOpaque = 0 TranspRend = 0
    255 , // Clear = 1 FullOpaque = 0 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 0
    255 , // Clear = 1 FullOpaque = 1 TranspRend = 0

    255 , // Clear = 0 FullOpaque = 0 TranspRend = 1
    255 , // Clear = 1 FullOpaque = 0 TranspRend = 1

    255 , // Clear = 0 FullOpaque = 1 TranspRend = 1
    255 , // Clear = 1 FullOpaque = 1 TranspRend = 1
  }
};




bool ZVoxelWorld::SetVoxel_WithCullingUpdate(Long x, Long y, Long z, UShort VoxelValue, UByte ImportanceFactor, bool CreateExtension, ZVoxelLocation * Location)
{
  UShort * Voxel_Address[7];
  ULong  Offset[7];
  UByte * FaceCulling_Address[7];
  UShort VoxelState[7];
  UShort Voxel;
  ZVoxelSector * Sector[7];
  ZVoxelType ** VoxelTypeTable;
  ZVoxelType * VoxelType;

  UShort * ExtFaceState;
  UShort * IntFaceState;
  ZMemSize OtherInfos;

  VoxelTypeTable = VoxelTypeManager->VoxelTable;

  // Fetching sectors

  if ( 0== (Sector[VOXEL_LEFT]    = FindSector( (x-1) >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z ) ) ) Sector[VOXEL_LEFT]    = this->WorkingScratchSector;
  if ( 0== (Sector[VOXEL_RIGHT]   = FindSector( (x+1) >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z ) ) ) Sector[VOXEL_RIGHT]   = this->WorkingScratchSector;
  if ( 0== (Sector[VOXEL_INFRONT] = FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z-1) >> ZVOXELBLOCSHIFT_Z ) ) ) Sector[VOXEL_INFRONT] = this->WorkingScratchSector;
  if ( 0== (Sector[VOXEL_BEHIND]  = FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z+1) >> ZVOXELBLOCSHIFT_Z ) ) ) Sector[VOXEL_BEHIND]  = this->WorkingScratchSector;
  if ( 0== (Sector[VOXEL_ABOVE]   = FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y + 1) >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z ) ) ) Sector[VOXEL_ABOVE]   = this->WorkingScratchSector;
  if ( 0== (Sector[VOXEL_BELOW]   = FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y - 1) >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z ) ) ) Sector[VOXEL_BELOW]   = this->WorkingScratchSector;
  if ( 0== (Sector[VOXEL_INCENTER]= FindSector( (x)   >> ZVOXELBLOCSHIFT_X , (y)     >> ZVOXELBLOCSHIFT_Y , (z)   >> ZVOXELBLOCSHIFT_Z ) ) ) return(false);

  // Computing memory offsets from sector start

  Offset[VOXEL_LEFT]     = (y & ZVOXELBLOCMASK_Y)       + ( ((x - 1) & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y ) + ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
  Offset[VOXEL_RIGHT]    = (y & ZVOXELBLOCMASK_Y)       + ( ((x + 1) & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y ) + ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
  Offset[VOXEL_INFRONT]  = (y & ZVOXELBLOCMASK_Y)       + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )       + (((z - 1) & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
  Offset[VOXEL_BEHIND]   = (y & ZVOXELBLOCMASK_Y)       + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )       + (((z + 1) & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
  Offset[VOXEL_ABOVE]    = ((y + 1) & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )       + ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
  Offset[VOXEL_BELOW]    = ((y - 1) & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )       + ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));
  Offset[VOXEL_INCENTER] = (y & ZVOXELBLOCMASK_Y)       + ( (x & ZVOXELBLOCMASK_X)<<ZVOXELBLOCSHIFT_Y )       + ((z & ZVOXELBLOCMASK_Z) << (ZVOXELBLOCSHIFT_Y+ZVOXELBLOCSHIFT_X));

  // Computing absolute memory pointer of blocks

  Voxel_Address[VOXEL_LEFT]     = Sector[VOXEL_LEFT]->Data + Offset[VOXEL_LEFT];
  Voxel_Address[VOXEL_RIGHT]    = Sector[VOXEL_RIGHT]->Data + Offset[VOXEL_RIGHT];
  Voxel_Address[VOXEL_INFRONT]  = Sector[VOXEL_INFRONT]->Data + Offset[VOXEL_INFRONT];
  Voxel_Address[VOXEL_BEHIND]   = Sector[VOXEL_BEHIND]->Data + Offset[VOXEL_BEHIND];
  Voxel_Address[VOXEL_ABOVE]    = Sector[VOXEL_ABOVE]->Data + Offset[VOXEL_ABOVE];
  Voxel_Address[VOXEL_BELOW]    = Sector[VOXEL_BELOW]->Data + Offset[VOXEL_BELOW];
  Voxel_Address[VOXEL_INCENTER] = Sector[VOXEL_INCENTER]->Data + Offset[VOXEL_INCENTER];

  // Computing absolute

  FaceCulling_Address[VOXEL_LEFT]     = Sector[VOXEL_LEFT]->FaceCulling + Offset[VOXEL_LEFT];
  FaceCulling_Address[VOXEL_RIGHT]    = Sector[VOXEL_RIGHT]->FaceCulling + Offset[VOXEL_RIGHT];
  FaceCulling_Address[VOXEL_INFRONT]  = Sector[VOXEL_INFRONT]->FaceCulling + Offset[VOXEL_INFRONT];
  FaceCulling_Address[VOXEL_BEHIND]   = Sector[VOXEL_BEHIND]->FaceCulling + Offset[VOXEL_BEHIND];
  FaceCulling_Address[VOXEL_ABOVE]    = Sector[VOXEL_ABOVE]->FaceCulling + Offset[VOXEL_ABOVE];
  FaceCulling_Address[VOXEL_BELOW]    = Sector[VOXEL_BELOW]->FaceCulling + Offset[VOXEL_BELOW];
  FaceCulling_Address[VOXEL_INCENTER] = Sector[VOXEL_INCENTER]->FaceCulling + Offset[VOXEL_INCENTER];

  // Fetching Voxels and computing voxel state

  Voxel = *Voxel_Address[VOXEL_LEFT];    VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_LEFT] = ((Voxel==0) ? 1 : 0) | ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) | ( VoxelType->Draw_TransparentRendering ? 4 : 0 );
  Voxel = *Voxel_Address[VOXEL_RIGHT];   VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_RIGHT] = ((Voxel==0) ? 1 : 0) | ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) | ( VoxelType->Draw_TransparentRendering ? 4 : 0 );
  Voxel = *Voxel_Address[VOXEL_INFRONT]; VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_INFRONT] = ((Voxel==0) ? 1 : 0) | ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) | ( VoxelType->Draw_TransparentRendering ? 4 : 0 );
  Voxel = *Voxel_Address[VOXEL_BEHIND];  VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_BEHIND] = ((Voxel==0) ? 1 : 0) | ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) | ( VoxelType->Draw_TransparentRendering ? 4 : 0 );
  Voxel = *Voxel_Address[VOXEL_ABOVE];   VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_ABOVE] = ((Voxel==0) ? 1 : 0) | ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) | ( VoxelType->Draw_TransparentRendering ? 4 : 0 );
  Voxel = *Voxel_Address[VOXEL_BELOW];   VoxelType = VoxelTypeTable[Voxel];
    VoxelState[VOXEL_BELOW] = ((Voxel==0) ? 1 : 0) | ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) | ( VoxelType->Draw_TransparentRendering ? 4 : 0 );

  // Delete Old voxel extended informations if any

  Voxel = *Voxel_Address[VOXEL_INCENTER];
  OtherInfos = *(Sector[VOXEL_INCENTER]->OtherInfos + Offset[VOXEL_INCENTER]);

  if (OtherInfos)
  {
    VoxelType = VoxelTypeTable[Voxel];
    if (VoxelType->Is_HasAllocatedMemoryExtension) VoxelType->DeleteVoxelExtension(OtherInfos);
  }

  // Storing Extension

  VoxelType = VoxelTypeTable[VoxelValue];
  if (CreateExtension)
  {
    *Voxel_Address[VOXEL_INCENTER] = 0; // Temporary set to 0 to prevent VoxelReactor for crashing while loading the wrong extension.
    *(Sector[VOXEL_INCENTER]->OtherInfos + Offset[VOXEL_INCENTER]) =(ZMemSize)VoxelType->CreateVoxelExtension();
  }

  // Storing Voxel

  *Voxel_Address[VOXEL_INCENTER] = VoxelValue;
  VoxelState[VOXEL_INCENTER] = ((VoxelValue==0) ? 1 : 0) | ( VoxelType->Draw_FullVoxelOpacity ? 2 : 0 ) | ( VoxelType->Draw_TransparentRendering ? 4 : 0 );


  if (VoxelTypeTable[VoxelValue]->Is_Active) Sector[VOXEL_INCENTER]->Flag_IsActiveVoxels = true;

  // Filling VoxelLocation if any

  if ((Location))
  {
    Location->Sector = Sector[VOXEL_INCENTER];
    Location->Offset = Offset[VOXEL_INCENTER];
  }

  // Getting case subtables.

  ExtFaceState = &ExtFaceStateTable[VoxelState[VOXEL_INCENTER]][0];
  IntFaceState = &IntFaceStateTable[VoxelState[VOXEL_INCENTER]][0];

  // Computing face culling for center main stored voxel.

  *FaceCulling_Address[VOXEL_INCENTER] =   (IntFaceState[VoxelState[VOXEL_LEFT]]   & DRAWFACE_LEFT)
                                         | (IntFaceState[VoxelState[VOXEL_RIGHT]]   & DRAWFACE_RIGHT)
                                         | (IntFaceState[VoxelState[VOXEL_INFRONT]] & DRAWFACE_AHEAD)
                                         | (IntFaceState[VoxelState[VOXEL_BEHIND]]  & DRAWFACE_BEHIND)
                                         | (IntFaceState[VoxelState[VOXEL_ABOVE]]   & DRAWFACE_ABOVE)
                                         | (IntFaceState[VoxelState[VOXEL_BELOW]]   & DRAWFACE_BELOW);

  // Computing face culling for nearboring voxels faces touching center voxel.

  *FaceCulling_Address[VOXEL_LEFT]    &= 255 ^ DRAWFACE_RIGHT;  *FaceCulling_Address[VOXEL_LEFT]    |= ExtFaceState[ VoxelState[VOXEL_LEFT]   ]  & DRAWFACE_RIGHT;
  *FaceCulling_Address[VOXEL_RIGHT]   &= 255 ^ DRAWFACE_LEFT;   *FaceCulling_Address[VOXEL_RIGHT]   |= ExtFaceState[ VoxelState[VOXEL_RIGHT]  ]  & DRAWFACE_LEFT;
  *FaceCulling_Address[VOXEL_INFRONT] &= 255 ^ DRAWFACE_BEHIND; *FaceCulling_Address[VOXEL_INFRONT] |= ExtFaceState[ VoxelState[VOXEL_INFRONT]]  & DRAWFACE_BEHIND;
  *FaceCulling_Address[VOXEL_BEHIND]  &= 255 ^ DRAWFACE_AHEAD;  *FaceCulling_Address[VOXEL_BEHIND]  |= ExtFaceState[ VoxelState[VOXEL_BEHIND] ]  & DRAWFACE_AHEAD;
  *FaceCulling_Address[VOXEL_ABOVE]   &= 255 ^ DRAWFACE_BELOW;  *FaceCulling_Address[VOXEL_ABOVE]   |= ExtFaceState[ VoxelState[VOXEL_ABOVE]  ]  & DRAWFACE_BELOW;
  *FaceCulling_Address[VOXEL_BELOW]   &= 255 ^ DRAWFACE_ABOVE;  *FaceCulling_Address[VOXEL_BELOW]   |= ExtFaceState[ VoxelState[VOXEL_BELOW]  ]  & DRAWFACE_ABOVE;

  // printf("State[Center]:%x [Left]%x [Right]%x [INFRONT]%x [BEHIND]%x [ABOVE]%x [BELOW]%x\n",VoxelState[VOXEL_INCENTER],VoxelState[VOXEL_LEFT],VoxelState[VOXEL_RIGHT],VoxelState[VOXEL_INFRONT],VoxelState[VOXEL_BEHIND],VoxelState[VOXEL_ABOVE],VoxelState[VOXEL_BELOW]);

  // Updating sector status rendering flag status

  Sector[VOXEL_INCENTER]->Flag_Render_Dirty = true;
  Sector[VOXEL_LEFT    ]->Flag_Render_Dirty = true;
  Sector[VOXEL_RIGHT   ]->Flag_Render_Dirty = true;
  Sector[VOXEL_INFRONT ]->Flag_Render_Dirty = true;
  Sector[VOXEL_BEHIND  ]->Flag_Render_Dirty = true;
  Sector[VOXEL_ABOVE   ]->Flag_Render_Dirty = true;
  Sector[VOXEL_BELOW   ]->Flag_Render_Dirty = true;
  //Sector[VOXEL_INCENTER]->Flag_Void_Regular = false;
  //Sector[VOXEL_LEFT    ]->Flag_Void_Regular = false;
  //Sector[VOXEL_RIGHT   ]->Flag_Void_Regular = false;
  //Sector[VOXEL_INFRONT ]->Flag_Void_Regular = false;
  //Sector[VOXEL_BEHIND  ]->Flag_Void_Regular = false;
  //Sector[VOXEL_ABOVE   ]->Flag_Void_Regular = false;
  //Sector[VOXEL_BELOW   ]->Flag_Void_Regular = false;
  //Sector[VOXEL_INCENTER]->Flag_Void_Transparent = false;
  //Sector[VOXEL_LEFT    ]->Flag_Void_Transparent = false;
  //Sector[VOXEL_RIGHT   ]->Flag_Void_Transparent = false;
  //Sector[VOXEL_INFRONT ]->Flag_Void_Transparent = false;
  //Sector[VOXEL_BEHIND  ]->Flag_Void_Transparent = false;
  //Sector[VOXEL_ABOVE   ]->Flag_Void_Transparent = false;
  //Sector[VOXEL_BELOW   ]->Flag_Void_Transparent = false;
  Sector[VOXEL_INCENTER]->Flag_IsModified |= ImportanceFactor;
            //Sector[VOXEL_LEFT    ]->Flag_IsModified = true;
            //Sector[VOXEL_RIGHT   ]->Flag_IsModified = true;
            //Sector[VOXEL_INFRONT ]->Flag_IsModified = true;
            //Sector[VOXEL_BEHIND  ]->Flag_IsModified = true;
            //Sector[VOXEL_ABOVE   ]->Flag_IsModified = true;
            //Sector[VOXEL_BELOW   ]->Flag_IsModified = true;
  return(true);
}


void ZVoxelWorld::Purge(UShort VoxelType)
{
  ZVoxelSector * Sector;

  for (Sector = SectorList; (Sector) ; Sector = Sector->GlobalList_Next )
  {
    Sector->Purge(VoxelType);
    Sector->Flag_Render_Dirty = true;
  }
}


bool ZVoxelWorld::Save()
{
  ZVoxelSector * Sector;
  Bool Result;

  Result = true;
  Sector = SectorList;

  while(Sector)
  {
#if COMPILEOPTION_SAVEONLYMODIFIED==1
      if (Sector->IsMustBeSaved())
#endif
      {
        if (!Sector->Save(UniverseNum)) Result = false;
      }

    Sector=Sector->GlobalList_Next;
  }
  return(Result);
}

bool ZVoxelWorld::Load()
{
  ZVoxelSector * Sector;
  Bool Result;
  Result = true;
  Sector = SectorList;

  while(Sector)
  {
    if (!Sector->Load(UniverseNum)) Result = false;
    Sector=Sector->GlobalList_Next;
  }
  return(Result);

}

bool ZVoxelWorld::Old_Load()
{
  ZVoxelSector * Sector;
  Bool Result;
  Result = true;
  Sector = SectorList;
  while(Sector)
  {
    if (!Sector->Old_Load(UniverseNum)) Result = false;
    SectorUpdateFaceCulling(Sector->Pos_x,Sector->Pos_y,Sector->Pos_z,false);
    Sector->Flag_NeedFullCulling = true;
    Sector=Sector->GlobalList_Next;
  }

  Sector = SectorList;
  while(Sector)
  {
    SectorUpdateFaceCulling(Sector->Pos_x,Sector->Pos_y,Sector->Pos_z,false);
    Sector=Sector->GlobalList_Next;
  }

  return(Result);

}

ULong ZVoxelWorld::SectorUpdateFaceCulling_Partial(Long x, Long y, Long z, UByte FacesToDraw, bool Isolated)
{
  ZVoxelSector * SectorTable[27];
  ZVoxelType ** VoxelTypeTable;
  ZVoxelSector * MissingSector;
  ZVoxelSector * Sector_In, * Sector_Out;

  ULong i, CuledFaces;
  ULong Off_Ip, Off_In, Off_Op , Off_Out, Off_Aux;
  UShort * VoxelData_In, * VoxelData_Out;
  UByte  * VoxelFC_In;

  UByte FaceState;


  if (Isolated) MissingSector = WorkingEmptySector;
  else          MissingSector = WorkingFullSector;

  for (i=0;i<27;i++) SectorTable[i] = MissingSector;
  SectorTable[0] = FindSector(x,y,z);    if (!SectorTable[0] ) {SectorTable[0]  = MissingSector;}
  SectorTable[1] = FindSector(x-1,y,z);  if (!SectorTable[1] ) {SectorTable[1]  = MissingSector;}
  SectorTable[2] = FindSector(x+1,y,z);  if (!SectorTable[2] ) {SectorTable[2]  = MissingSector;}
  SectorTable[3] = FindSector(x,y,z-1);  if (!SectorTable[3] ) {SectorTable[3]  = MissingSector;}
  SectorTable[6] = FindSector(x,y,z+1);  if (!SectorTable[6] ) {SectorTable[6]  = MissingSector;}
  SectorTable[9] = FindSector(x,y-1,z);  if (!SectorTable[9] ) {SectorTable[9]  = MissingSector;}
  SectorTable[18]= FindSector(x,y+1,z);  if (!SectorTable[18]) {SectorTable[18] = MissingSector;}


  VoxelTypeTable = this->VoxelTypeManager->VoxelTable;

  Sector_In  = FindSector(x,y,z); if (!Sector_In) return(0);
  CuledFaces = 0;

  // Top Side

  if (FacesToDraw & DRAWFACE_ABOVE)
    if ( (Sector_Out = FindSector(x,y+1,z)))
    {
      VoxelData_In  = Sector_In->Data;
      VoxelData_Out = Sector_Out->Data;
      VoxelFC_In    = Sector_In->FaceCulling;

      for ( Off_Ip=ZVOXELBLOCSIZE_Y-1, Off_Op=0 ; Off_Ip < (ZVOXELBLOCSIZE_Y * 16) ; Off_Ip+=ZVOXELBLOCSIZE_Y, Off_Op+=ZVOXELBLOCSIZE_Y ) // x (0..15)
      {
        for ( Off_Aux=0; Off_Aux < (ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Y * 16) ; Off_Aux+=(ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Y) ) // z (0..15)
        {
          Off_In = Off_Ip + Off_Aux;
          Off_Out= Off_Op + Off_Aux;
          FaceState = IntFaceStateTable[ VoxelTypeTable[ VoxelData_In[Off_In] ]->DrawInfo & ZVOXEL_DRAWINFO_CULLINGBITS ][ VoxelTypeTable[ VoxelData_Out[Off_Out] ]->DrawInfo & ZVOXEL_DRAWINFO_CULLINGBITS ];
          if (FaceState) VoxelFC_In[Off_In] |= DRAWFACE_ABOVE; else VoxelFC_In[Off_In] &= ~DRAWFACE_ABOVE;
        }
      }
      CuledFaces |= DRAWFACE_ABOVE;
    }

  // Bottom Side

  if (FacesToDraw & DRAWFACE_BELOW)
    if ((Sector_Out = FindSector(x,y-1,z)))
    {



      VoxelData_In  = Sector_In->Data;
      VoxelData_Out = Sector_Out->Data;
      VoxelFC_In    = Sector_In->FaceCulling;

      for ( Off_Ip=0, Off_Op=ZVOXELBLOCSIZE_Y-1 ; Off_Ip < (ZVOXELBLOCSIZE_Y * 16) ; Off_Ip+=ZVOXELBLOCSIZE_Y, Off_Op+=ZVOXELBLOCSIZE_Y ) // x (0..15)
      {
        for ( Off_Aux=0; Off_Aux < (ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Y * 16) ; Off_Aux+=(ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Y) ) // z (0..15)
        {
          Off_In = Off_Ip + Off_Aux;
          Off_Out= Off_Op + Off_Aux;
          UShort Voxel_In = VoxelData_In[Off_In];
          UShort Voxel_Out = VoxelData_Out[Off_Out];
          //ZVoxelType * VtIn =  VoxelTypeTable[ Voxel_In ];
          //ZVoxelType * VtOut = VoxelTypeTable[ Voxel_Out ];


          FaceState = IntFaceStateTable[ VoxelTypeTable[ Voxel_In ]->DrawInfo & ZVOXEL_DRAWINFO_CULLINGBITS ][ VoxelTypeTable[ Voxel_Out ]->DrawInfo & ZVOXEL_DRAWINFO_CULLINGBITS ];

          //FaceState = IntFaceStateTable[ VoxelTypeTable[ VoxelData_In[Off_In] ]->DrawInfo & ZVOXEL_DRAWINFO_CULLINGBITS ][ VoxelTypeTable[ VoxelData_Out[Off_Out] ]->DrawInfo & ZVOXEL_DRAWINFO_CULLINGBITS ];
          if (FaceState) VoxelFC_In[Off_In] |= DRAWFACE_BELOW; else VoxelFC_In[Off_In] &= ~DRAWFACE_BELOW;
        }
      }
      CuledFaces |= DRAWFACE_BELOW;
    }

  // Left Side

  if (FacesToDraw & DRAWFACE_LEFT)
    if ((Sector_Out = FindSector(x-1,y,z)))
    {
      VoxelData_In  = Sector_In->Data;
      VoxelData_Out = Sector_Out->Data;
      VoxelFC_In    = Sector_In->FaceCulling;
      // VoxelData_In[63]=1;
      // VoxelData_In[63 + ZVOXELBLOCSIZE_Y*15 ]=14; // x
      // VoxelData_In[63 + ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_X * 15] = 13; // z

      for ( Off_Ip=0, Off_Op=ZVOXELBLOCSIZE_Y * (ZVOXELBLOCSIZE_X-1) ; Off_Ip < (ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Z) ; Off_Ip+=(ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_X), Off_Op+=(ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_X) ) // z (0..15)
      {
        for ( Off_Aux=0; Off_Aux < ZVOXELBLOCSIZE_Y ; Off_Aux++  ) // y (0..63)
        {
          Off_In = Off_Ip + Off_Aux;
          Off_Out= Off_Op + Off_Aux;
          //VoxelData_In[Off_In]=1; VoxelData_Out[Off_Out]=14;
          FaceState = IntFaceStateTable[ VoxelTypeTable[ VoxelData_In[Off_In] ]->DrawInfo & ZVOXEL_DRAWINFO_CULLINGBITS ][ VoxelTypeTable[ VoxelData_Out[Off_Out] ]->DrawInfo & ZVOXEL_DRAWINFO_CULLINGBITS ];
          if (FaceState) VoxelFC_In[Off_In] |= DRAWFACE_LEFT; else VoxelFC_In[Off_In] &= ~DRAWFACE_LEFT;


        }
      }
      CuledFaces |= DRAWFACE_LEFT;
    }

  // Right Side

  if (FacesToDraw & DRAWFACE_RIGHT)
    if ((Sector_Out = FindSector(x+1,y,z)))
    {
      VoxelData_In  = Sector_In->Data;
      VoxelData_Out = Sector_Out->Data;
      VoxelFC_In    = Sector_In->FaceCulling;

      for ( Off_Ip=ZVOXELBLOCSIZE_Y * (ZVOXELBLOCSIZE_X-1), Off_Op=0 ; Off_Op < (ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Z) ; Off_Ip+=(ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_X), Off_Op+=(ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_X) ) // z (0..15)
      {
        for ( Off_Aux=0; Off_Aux < ZVOXELBLOCSIZE_Y ; Off_Aux++  ) // y (0..63)
        {
          Off_In = Off_Ip + Off_Aux;
          Off_Out= Off_Op + Off_Aux;
          FaceState = IntFaceStateTable[ VoxelTypeTable[ VoxelData_In[Off_In] ]->DrawInfo & ZVOXEL_DRAWINFO_CULLINGBITS ][ VoxelTypeTable[ VoxelData_Out[Off_Out] ]->DrawInfo & ZVOXEL_DRAWINFO_CULLINGBITS ];
          if (FaceState) VoxelFC_In[Off_In] |= DRAWFACE_RIGHT; else VoxelFC_In[Off_In] &= ~DRAWFACE_RIGHT;
        }
      }
      CuledFaces |= DRAWFACE_RIGHT;
    }

  // Front Side

  if (FacesToDraw & DRAWFACE_AHEAD)
    if ((Sector_Out = FindSector(x,y,z-1)))
    {
      VoxelData_In  = Sector_In->Data;
      VoxelData_Out = Sector_Out->Data;
      VoxelFC_In    = Sector_In->FaceCulling;
      for ( Off_Ip=0, Off_Op= (ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_X * (ZVOXELBLOCSIZE_Z-1)) ; Off_Ip < (ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_X) ; Off_Ip+=ZVOXELBLOCSIZE_Y, Off_Op+=ZVOXELBLOCSIZE_Y ) // x (0..15)
      {
        for ( Off_Aux=0; Off_Aux < ZVOXELBLOCSIZE_Y ; Off_Aux++  ) // y (0..63)
        {
          Off_In = Off_Ip + Off_Aux;
          Off_Out= Off_Op + Off_Aux;
          FaceState = IntFaceStateTable[ VoxelTypeTable[ VoxelData_In[Off_In] ]->DrawInfo & ZVOXEL_DRAWINFO_CULLINGBITS ][ VoxelTypeTable[ VoxelData_Out[Off_Out] ]->DrawInfo & ZVOXEL_DRAWINFO_CULLINGBITS ];
          if (FaceState) VoxelFC_In[Off_In] |= DRAWFACE_AHEAD; else VoxelFC_In[Off_In] &= ~DRAWFACE_AHEAD;
        }
      }
      CuledFaces |= DRAWFACE_AHEAD;
    }

  // Back Side

  if (FacesToDraw & DRAWFACE_BEHIND)
    if ((Sector_Out = FindSector(x,y,z+1)))
    {
      VoxelData_In  = Sector_In->Data;
      VoxelData_Out = Sector_Out->Data;
      VoxelFC_In    = Sector_In->FaceCulling;

      for ( Off_Ip=(ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_X * (ZVOXELBLOCSIZE_Z-1)) , Off_Op=0 ; Off_Op < (ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_X) ; Off_Ip+=ZVOXELBLOCSIZE_Y, Off_Op+=ZVOXELBLOCSIZE_Y ) // x (0..15)
      {
        for ( Off_Aux=0; Off_Aux < ZVOXELBLOCSIZE_Y ; Off_Aux++  ) // y (0..63)
        {
          Off_In = Off_Ip + Off_Aux;
          Off_Out= Off_Op + Off_Aux;
          FaceState = IntFaceStateTable[ VoxelTypeTable[ VoxelData_In[Off_In] ]->DrawInfo & ZVOXEL_DRAWINFO_CULLINGBITS ][ VoxelTypeTable[ VoxelData_Out[Off_Out] ]->DrawInfo & ZVOXEL_DRAWINFO_CULLINGBITS ];
          if (FaceState) VoxelFC_In[Off_In] |= DRAWFACE_BEHIND; else VoxelFC_In[Off_In] &= ~DRAWFACE_BEHIND;
        }
      }
      CuledFaces |= DRAWFACE_BEHIND;
    }

  return(CuledFaces);
}



void ZVoxelWorld::SectorUpdateFaceCulling(Long x, Long y, Long z, bool Isolated)
{
  ZVoxelSector * SectorTable[27];
  ZVoxelType ** VoxelTypeTable;
  ZVoxelSector * MissingSector;

  UByte * BlocMatrix[3];
  void * tmpp;
  ULong i;

  UByte s1[9];
  UByte s2[9];
  UByte s3[9];

  BlocMatrix[0] = s1;
  BlocMatrix[1] = s2;
  BlocMatrix[2] = s3;


  if (Isolated) MissingSector = WorkingEmptySector;
  else          MissingSector = WorkingFullSector;

/*
  if (x==0 && y== 0 && z==0)
  {
    printf("Entering..");
  }
*/

 // (DRAWFACE_ABOVE | DRAWFACE_BELOW | DRAWFACE_LEFT | DRAWFACE_RIGHT | DRAWFACE_AHEAD | DRAWFACE_BEHIND);
  for (i=0;i<27;i++) SectorTable[i] = MissingSector;
  SectorTable[0] = FindSector(x,y,z);    if (!SectorTable[0] ) {return;}
  SectorTable[1] = FindSector(x-1,y,z);  if (!SectorTable[1] ) {SectorTable[1]  = MissingSector; SectorTable[0]->PartialCulling |= DRAWFACE_LEFT;}
  SectorTable[2] = FindSector(x+1,y,z);  if (!SectorTable[2] ) {SectorTable[2]  = MissingSector; SectorTable[0]->PartialCulling |= DRAWFACE_RIGHT;}
  SectorTable[3] = FindSector(x,y,z-1);  if (!SectorTable[3] ) {SectorTable[3]  = MissingSector; SectorTable[0]->PartialCulling |= DRAWFACE_AHEAD;}
  SectorTable[6] = FindSector(x,y,z+1);  if (!SectorTable[6] ) {SectorTable[6]  = MissingSector; SectorTable[0]->PartialCulling |= DRAWFACE_BEHIND;}
  SectorTable[9] = FindSector(x,y-1,z);  if (!SectorTable[9] ) {SectorTable[9]  = MissingSector; SectorTable[0]->PartialCulling |= DRAWFACE_BELOW;}
  SectorTable[18]= FindSector(x,y+1,z);  if (!SectorTable[18]) {SectorTable[18] = MissingSector; SectorTable[0]->PartialCulling |= DRAWFACE_ABOVE;}


  Long xc,yc,zc;
  Long xp,yp,zp;
  Long xpp,ypp,zpp;
  UByte info, MainVoxelDrawInfo;

  //SectorTable[0]->Flag_Void_Regular = true;
  //SectorTable[0]->Flag_Void_Transparent = true;
  VoxelTypeTable = this->VoxelTypeManager->VoxelTable;

  for ( xc=0 ; xc<ZVOXELBLOCSIZE_X ; xc++ )
  {
    xp = xc+1; xpp= xc+2;
    for ( zc=0 ; zc<ZVOXELBLOCSIZE_Z ; zc++ )
    {
      zp = zc+1;zpp=zc+2;

      // Prefetching the bloc matrix (only 2 rows)
//    BlocMatrix[1][0] = SectorTable[(STableX[xc ]+STableY[0]+STableZ[zc ])]->Data[OfTableX[xc]+OfTableY[0]+OfTableZ[zc]];
      BlocMatrix[1][1] = SectorTable[(STableX[xp ]+STableY[0]+STableZ[zc ])]->Data[OfTableX[xp]+OfTableY[0]+OfTableZ[zc]];
//    BlocMatrix[1][2] = SectorTable[(STableX[xpp]+STableY[0]+STableZ[zc ])]->Data[OfTableX[xpp]+OfTableY[0]+OfTableZ[zc]];
      BlocMatrix[1][3] = SectorTable[(STableX[xc ]+STableY[0]+STableZ[zp ])]->Data[OfTableX[xc]+OfTableY[0]+OfTableZ[zp]];
      BlocMatrix[1][4] = SectorTable[(STableX[xp ]+STableY[0]+STableZ[zp ])]->Data[OfTableX[xp]+OfTableY[0]+OfTableZ[zp]];
      BlocMatrix[1][5] = SectorTable[(STableX[xpp]+STableY[0]+STableZ[zp ])]->Data[OfTableX[xpp]+OfTableY[0]+OfTableZ[zp]];
//    BlocMatrix[1][6] = SectorTable[(STableX[xc ]+STableY[0]+STableZ[zpp])]->Data[OfTableX[xc]+OfTableY[0]+OfTableZ[zpp]];
      BlocMatrix[1][7] = SectorTable[(STableX[xp ]+STableY[0]+STableZ[zpp])]->Data[OfTableX[xp]+OfTableY[0]+OfTableZ[zpp]];
//    BlocMatrix[1][8] = SectorTable[(STableX[xpp]+STableY[0]+STableZ[zpp])]->Data[OfTableX[xpp]+OfTableY[0]+OfTableZ[zpp]];

//    BlocMatrix[2][0] = SectorTable[(STableX[xc ]+STableY[1]+STableZ[zc ])]->Data[OfTableX[xc ]+OfTableY[1]+OfTableZ[zc ]];
      BlocMatrix[2][1] = SectorTable[(STableX[xp ]+STableY[1]+STableZ[zc ])]->Data[OfTableX[xp ]+OfTableY[1]+OfTableZ[zc ]];
//    BlocMatrix[2][2] = SectorTable[(STableX[xpp]+STableY[1]+STableZ[zc ])]->Data[OfTableX[xpp]+OfTableY[1]+OfTableZ[zc ]];
      BlocMatrix[2][3] = SectorTable[(STableX[xc ]+STableY[1]+STableZ[zp ])]->Data[OfTableX[xc ]+OfTableY[1]+OfTableZ[zp ]];
      BlocMatrix[2][4] = SectorTable[(STableX[xp ]+STableY[1]+STableZ[zp ])]->Data[OfTableX[xp ]+OfTableY[1]+OfTableZ[zp ]];
      BlocMatrix[2][5] = SectorTable[(STableX[xpp]+STableY[1]+STableZ[zp ])]->Data[OfTableX[xpp]+OfTableY[1]+OfTableZ[zp ]];
//    BlocMatrix[2][6] = SectorTable[(STableX[xc ]+STableY[1]+STableZ[zpp])]->Data[OfTableX[xc ]+OfTableY[1]+OfTableZ[zpp]];
      BlocMatrix[2][7] = SectorTable[(STableX[xp ]+STableY[1]+STableZ[zpp])]->Data[OfTableX[xp ]+OfTableY[1]+OfTableZ[zpp]];
//    BlocMatrix[2][8] = SectorTable[(STableX[xpp]+STableY[1]+STableZ[zpp])]->Data[OfTableX[xpp]+OfTableY[1]+OfTableZ[zpp]];

      for ( yc=0 ; yc< ZVOXELBLOCSIZE_Y ; yc++ )
      {
        yp = yc+1; ypp=yc+2;

        // Scrolling bloc matrix by exchangingypp references.
        tmpp = BlocMatrix[0];
        BlocMatrix[0] = BlocMatrix[1];
        BlocMatrix[1] = BlocMatrix[2];
        BlocMatrix[2] = (UByte *) tmpp;

        // Fetching a new bloc of data slice;

//      BlocMatrix[2][0] = SectorTable[(STableX[xc ]+STableY[ypp]+STableZ[zc ])]->Data[OfTableX[xc ]+OfTableY[ypp]+OfTableZ[zc ]];
        BlocMatrix[2][1] = SectorTable[(STableX[xp ]+STableY[ypp]+STableZ[zc ])]->Data[OfTableX[xp ]+OfTableY[ypp]+OfTableZ[zc ]];
//      BlocMatrix[2][2] = SectorTable[(STableX[xpp]+STableY[ypp]+STableZ[zc ])]->Data[OfTableX[xpp]+OfTableY[ypp]+OfTableZ[zc ]];
        BlocMatrix[2][3] = SectorTable[(STableX[xc ]+STableY[ypp]+STableZ[zp ])]->Data[OfTableX[xc ]+OfTableY[ypp]+OfTableZ[zp ]];
        BlocMatrix[2][4] = SectorTable[(STableX[xp ]+STableY[ypp]+STableZ[zp ])]->Data[OfTableX[xp ]+OfTableY[ypp]+OfTableZ[zp ]];
        BlocMatrix[2][5] = SectorTable[(STableX[xpp]+STableY[ypp]+STableZ[zp ])]->Data[OfTableX[xpp]+OfTableY[ypp]+OfTableZ[zp ]];
//      BlocMatrix[2][6] = SectorTable[(STableX[xc ]+STableY[ypp]+STableZ[zpp])]->Data[OfTableX[xc ]+OfTableY[ypp]+OfTableZ[zpp]];
        BlocMatrix[2][7] = SectorTable[(STableX[xp ]+STableY[ypp]+STableZ[zpp])]->Data[OfTableX[xp ]+OfTableY[ypp]+OfTableZ[zpp]];
//      BlocMatrix[2][8] = SectorTable[(STableX[xpp]+STableY[ypp]+STableZ[zpp])]->Data[OfTableX[xpp]+OfTableY[ypp]+OfTableZ[zpp]];

        // Compute face culling info
/*
        if (x==0 && y== 0 && z==0)
        {
          if (xc == 15 && yc == 0 && zc ==0)
          {
            printf("Gotcha\n");
          }

        }
*/
        info = 0;
        if (BlocMatrix[1][4] > 0)
        {
          MainVoxelDrawInfo = VoxelTypeTable[BlocMatrix[1][4]]->DrawInfo;
          UShort * SubTable = (UShort *)&IntFaceStateTable[MainVoxelDrawInfo];


          // {
          /*
          UByte Bm = BlocMatrix[1][1];
          ZVoxelType * Vt = VoxelTypeTable[Bm];
          UByte Di = Vt->DrawInfo;
          UShort st = SubTable[ Di ];
          info |= ( st ) & DRAWFACE_AHEAD;
          */
          // }

          info |= ( SubTable[ VoxelTypeTable[BlocMatrix[1][1]]->DrawInfo ] ) & DRAWFACE_AHEAD;
          info |= ( SubTable[ VoxelTypeTable[BlocMatrix[1][7]]->DrawInfo ] ) & DRAWFACE_BEHIND;
          info |= ( SubTable[ VoxelTypeTable[BlocMatrix[1][3]]->DrawInfo ] ) & DRAWFACE_LEFT;
          info |= ( SubTable[ VoxelTypeTable[BlocMatrix[1][5]]->DrawInfo ] ) & DRAWFACE_RIGHT;
          info |= ( SubTable[ VoxelTypeTable[BlocMatrix[0][4]]->DrawInfo ] ) & DRAWFACE_BELOW;
          info |= ( SubTable[ VoxelTypeTable[BlocMatrix[2][4]]->DrawInfo ] ) & DRAWFACE_ABOVE;
        }

        // Write face culling info to face culling table

        SectorTable[0]->FaceCulling[OfTableX[xp]+OfTableY[yp]+OfTableZ[zp]] = info;

      }
    }
  }

}

// The quad sampling version has been developped for cornering deplacement


bool ZVoxelWorld::RayCast_Vector_special(const ZVector3d & Pos, const ZVector3d & Vector, const ZRayCast_in * In, ZRayCast_out * Out, bool InvertCollision)
{
  ZVector4d Delta_h,Delta_v,Delta_s;
  ZVector4d Offset_h, Offset_v, Offset_s;
  ZVector3d Norm_h, Norm_v, Norm_s;
  ZVector4d Collision_h, Collision_v, Collision_s;

  Long ActualCube_x,ActualCube_y,ActualCube_z;
  Long NewCube_x,NewCube_y,NewCube_z;
  bool Collide_X, Collide_Y, Collide_Z;
  ULong i;
  UByte Face_h, Face_s,Face_v;
  UShort VoxelType;

  ZVector3d Norm;
  ZVector3d Tmp;

  double Vector_Len;

  bool Armed, DeferArmed;

  Armed = false; DeferArmed = false;


  NewCube_x = (Long)floor((Pos.x) / 256.0); NewCube_y = (Long)floor((Pos.y) / 256.0); NewCube_z = (Long)floor((Pos.z) / 256.0);
  VoxelType = GetVoxel( NewCube_x, NewCube_y, NewCube_z);
  if (VoxelTypeManager->VoxelTable[VoxelType]->Is_PlayerCanPassThrough != InvertCollision) Armed = true;



  // Normalize input vector.
  Vector_Len = sqrt( Vector.x * Vector.x + Vector.y * Vector.y + Vector.z * Vector.z);
  Norm = Vector / Vector_Len;

  // Norm = Vector;

  //printf("Norm:%lf %lf %lf\n",Norm.x,Norm.y,Norm.z);
  Collide_X = Collide_Y = Collide_Z = false;
  Face_h = Face_s = Face_v = 0;

  if (Norm.x > 0.00000001)
  {
    Face_h = 2;
    Collide_X = true;
    Delta_h.y = Norm.y / Norm.x;
    Delta_h.z = Norm.z / Norm.x;
    Delta_h.w = 1.0 / Norm.x;
    Collision_h.x = (floor(Pos.x / 256.0) + 1.0)*256.0;
    Collision_h.y = (Collision_h.x - Pos.x) * Delta_h.y + Pos.y;
    Collision_h.z = (Collision_h.x - Pos.x) * Delta_h.z + Pos.z;
    Collision_h.w = (Collision_h.x - Pos.x) * Delta_h.w;

    Offset_h.x = 256.0;
    Offset_h.y = Delta_h.y * 256.0;
    Offset_h.z = Delta_h.z * 256.0;
    Offset_h.w = Delta_h.w * 256.0;
    Norm_h.x = Offset_h.x / 128.0;
    Norm_h.y = Offset_h.y / 128.0;
    Norm_h.z = Offset_h.z / 128.0;
  }
  else if (Norm.x < -0.00000001)
  {
    Face_h = 3;
    Collide_X = true;
    Delta_h.y = Norm.y / -Norm.x;
    Delta_h.z = Norm.z / -Norm.x;
    Delta_h.w = 1.0 / fabs(Norm.x);

    Collision_h.x = (floor(Pos.x / 256.0))*256.0 ; // - 1.0
    Collision_h.y = (Pos.x - Collision_h.x) * Delta_h.y + Pos.y;
    Collision_h.z = (Pos.x - Collision_h.x) * Delta_h.z + Pos.z;
    Collision_h.w = (Pos.x - Collision_h.x) * Delta_h.w;
    Offset_h.x = -256.0;
    Offset_h.y = Delta_h.y * 256.0;
    Offset_h.z = Delta_h.z * 256.0;
    Offset_h.w = Delta_h.w * 256.0;
    Norm_h.x = Offset_h.x / 128.0;
    Norm_h.y = Offset_h.y / 128.0;
    Norm_h.z = Offset_h.z / 128.0;
  }

  if (Norm.y > 0.00000001)
  {
    Face_v = 5;
    Collide_Y = true;
    Delta_v.x = Norm.x / Norm.y;
    Delta_v.z = Norm.z / Norm.y;
    Delta_v.w = 1 / Norm.y;
    Collision_v.y = (floor(Pos.y / 256.0) +1.0) * 256.0;
    Collision_v.x = (Collision_v.y - Pos.y) * Delta_v.x + Pos.x;
    Collision_v.z = (Collision_v.y - Pos.y) * Delta_v.z + Pos.z;
    Collision_v.w = (Collision_v.y - Pos.y) * Delta_v.w;
    Offset_v.y = 256.0;
    Offset_v.x = Delta_v.x * 256.0;
    Offset_v.z = Delta_v.z * 256.0;
    Offset_v.w = Delta_v.w * 256.0;
    Norm_v.x = Offset_v.x / 128.0;
    Norm_v.y = Offset_v.y / 128.0;
    Norm_v.z = Offset_v.z / 128.0;
  }
  else if (Norm.y < -0.00000001)
  {
    Face_v = 4;
    Collide_Y = true;
    Delta_v.x = Norm.x / -Norm.y;
    Delta_v.z = Norm.z / -Norm.y;
    Delta_v.w = 1.0 / -Norm.y;
    Collision_v.y = (floor(Pos.y / 256.0)) * 256.0 ; // - 1.0
    Collision_v.x = (Pos.y - Collision_v.y) * Delta_v.x + Pos.x;
    Collision_v.z = (Pos.y - Collision_v.y) * Delta_v.z + Pos.z;
    Collision_v.w = fabs((Collision_v.y - Pos.y) * Delta_v.w);

    Offset_v.y = -256.0;
    Offset_v.x = Delta_v.x * 256.0;
    Offset_v.z = Delta_v.z * 256.0;
    Offset_v.w = Delta_v.w * 256.0;
    Norm_v.x = Offset_v.x / 128.0;
    Norm_v.y = Offset_v.y / 128.0;
    Norm_v.z = Offset_v.z / 128.0;
  }

  if (Norm.z > 0.00000001)
  {
    Face_s = 0;
    Collide_Z = true;
    Delta_s.x = Norm.x / Norm.z;
    Delta_s.y = Norm.y / Norm.z;
    Delta_s.w = 1.0 / Norm.z;
    Collision_s.z = (floor(Pos.z / 256.0 + 1.0) )*256.0;
    Collision_s.x = (Collision_s.z - Pos.z) * Delta_s.x + Pos.x;
    Collision_s.y = (Collision_s.z - Pos.z) * Delta_s.y + Pos.y;
    Collision_s.w = (Collision_s.z - Pos.z) * Delta_s.w;

    Offset_s.z = 256.0;
    Offset_s.x = Delta_s.x * 256.0;
    Offset_s.y = Delta_s.y * 256.0;
    Offset_s.w = Delta_s.w * 256.0;
    Norm_s.x = Offset_s.x / 128.0;
    Norm_s.y = Offset_s.y / 128.0;
    Norm_s.z = Offset_s.z / 128.0;
  }
  else if (Norm.z < -0.00000001)
  {
    Face_s = 1;
    Collide_Z = true;
    Delta_s.x = Norm.x / -Norm.z;
    Delta_s.y = Norm.y / -Norm.z;
    Delta_s.w = 1.0 / -Norm.z;
    Collision_s.z = (floor(Pos.z / 256.0) )*256.0 ; // - 1.0
    Collision_s.x = (Pos.z - Collision_s.z) * Delta_s.x + Pos.x;
    Collision_s.y = (Pos.z - Collision_s.z) * Delta_s.y + Pos.y;
    Collision_s.w = (Pos.z - Collision_s.z) * Delta_s.w;
    Offset_s.z = - 256.0;
    Offset_s.x = Delta_s.x * 256.0;
    Offset_s.y = Delta_s.y * 256.0;
    Offset_s.w = Delta_s.w * 256.0;
    Norm_s.x = Offset_s.x / 128.0;
    Norm_s.y = Offset_s.y / 128.0;
    Norm_s.z = Offset_s.z / 128.0;
  }
/*
   printf ("Loc(%lf %lf %lf) Norm(%lf %lf %lf) Col(%lf %lf %lf %lf) Off(%lf %lf %lf %lf) C(%d,%d,%d)\n", Pos.x, Pos.y, Pos.z, Norm.x,Norm.y, Norm.z, Collision_s.x, Collision_s.y, Collision_s.z, Collision_s.w, Offset_s.x,Offset_s.y, Offset_s.z,Offset_s.w
       ,(UShort)((Collide_X==true)? 1:0) ,(UShort)((Collide_Y==true)? 1:0), (UShort)((Collide_Z==true)? 1:0));
*/
//  printf("yaw: %04lf pitch: %lf Offset_y:%lf Offset_z:%lf xyz:%lf %lf %lf NXYZ:%lf %lf %lf Dxyz:%lf %lf %lf", yaw,pitch, Delta_h.y, Delta_h.z,x,y,z, Norm_h.x, Norm_h.y, Norm_h.z, Delta_h.x, Delta_h.y, Delta_h.z);
 //printf("Angle (y:%lf p:%lf) XYZ:(%lf %lf %lf) Off(%lf %lf %lf %lf) Coll(%lf %lf %lf %lf) Norm(%lg %lg %lf) :\n", yaw,pitch,x,y,z, Offset_s.x, Offset_s.y, Offset_s.z, Offset_s.w, Collision_s.x, Collision_s.y, Collision_s.z, Collision_s.w, Norm_s.x,Norm_s.y, Norm_s.z);

  Long Match_h=0;
  Long Match_s=0;
  Long Match_v=0;
  Long Cycle = 1;
  double MinW = 10000000.0;

  for (i=0;i < In->MaxCubeIterations;i++)
  {
    // if  ( (Collision_h.w > In->MaxDetectionDistance) || (Collision_s.w > In->MaxDetectionDistance) || (Collision_v.w > In->MaxDetectionDistance)) { Out->Collided = false; return(false); }
    // Horizontal X axis.
    if (Collide_X)
    {
      if (Match_h==0 && Collision_h.w < MinW)
      {
        ActualCube_x = (Long)floor((Collision_h.x - Norm_h.x) / 256.0); ActualCube_y = (Long)floor((Collision_h.y - Norm_h.y) / 256.0); ActualCube_z = (Long)floor((Collision_h.z - Norm_h.z) / 256.0);
        NewCube_x = (Long)floor((Collision_h.x) / 256.0); NewCube_y = (Long)floor((Collision_h.y) / 256.0); NewCube_z = (Long)floor((Collision_h.z) / 256.0);
        //ActualCube_x = (Long)floor((Collision_h.x - Norm_h.x) / 256.0); ActualCube_y = (Long)floor((Collision_h.y - Norm_h.y) / 256.0); ActualCube_z = (Long)floor((Collision_h.z - Norm_h.z) / 256.0);
        //NewCube_x = (Long)floor((Collision_h.x + Norm_h.x) / 256.0); NewCube_y = (Long)floor((Collision_h.y + Norm_h.y) / 256.0); NewCube_z = (Long)floor((Collision_h.z + Norm_h.z) / 256.0);
        if (Face_h == 3) NewCube_x--;
        VoxelType = GetVoxel( NewCube_x, NewCube_y, NewCube_z);
        if (!VoxelTypeManager->VoxelTable[VoxelType]->Is_PlayerCanPassThrough ^ InvertCollision)
        {
          if (Armed)
          {
            Out->PredPointedVoxel.x = ActualCube_x; Out->PredPointedVoxel.y = ActualCube_y; Out->PredPointedVoxel.z = ActualCube_z;
            Out->PointedVoxel.x = NewCube_x; Out->PointedVoxel.y = NewCube_y; Out->PointedVoxel.z = NewCube_z;
            Out->CollisionPoint.x = Collision_h.x; Out->CollisionPoint.y = Collision_h.y; Out->CollisionPoint.z = Collision_h.z; Out->CollisionDistance = Collision_h.w;
            Out->CollisionAxe = 0; Out->CollisionFace = Face_h;
            Out->PointInCubeFace.x = fmod(Out->CollisionPoint.z, 256.0); Out->PointInCubeFace.y = fmod(Out->CollisionPoint.y, 256.0);
            //printf(" MATCH_H: %lf (%ld %ld %ld) C:%ld\n",Collision_h.w, NewCube_x, NewCube_y, NewCube_z, Cycle);
            Match_h = Cycle;
            MinW = Collision_h.w;
          }
          else DeferArmed = true;
        }
      }
    }

    // Horizontal Z axis.

    if (Collide_Z)
    {
      if (Match_s == 0 && Collision_s.w < MinW)
      {
        ActualCube_x = (Long)floor((Collision_s.x - Norm_s.x) / 256.0); ActualCube_y = (Long)floor((Collision_s.y - Norm_s.y) / 256.0); ActualCube_z = (Long)floor((Collision_s.z - Norm_s.z) / 256.0);
        NewCube_x = (Long)floor((Collision_s.x) / 256.0); NewCube_y = (Long)floor((Collision_s.y) / 256.0); NewCube_z = (Long)floor((Collision_s.z) / 256.0);
        //ActualCube_x = (Long)floor((Collision_s.x - Norm_s.x) / 256.0); ActualCube_y = (Long)floor((Collision_s.y - Norm_s.y) / 256.0); ActualCube_z = (Long)floor((Collision_s.z - Norm_s.z) / 256.0);
        //NewCube_x = (Long)floor((Collision_s.x + Norm_s.x) / 256.0); NewCube_y = (Long)floor((Collision_s.y + Norm_s.y) / 256.0); NewCube_z = (Long)floor((Collision_s.z + Norm_s.z) / 256.0);
        if (Face_s == 1) NewCube_z--;
        VoxelType = GetVoxel( NewCube_x, NewCube_y, NewCube_z);
        if (!VoxelTypeManager->VoxelTable[VoxelType]->Is_PlayerCanPassThrough ^ InvertCollision)
        {
          if (Armed)
          {
            Out->PredPointedVoxel.x = ActualCube_x; Out->PredPointedVoxel.y = ActualCube_y; Out->PredPointedVoxel.z = ActualCube_z;
            Out->PointedVoxel.x = NewCube_x; Out->PointedVoxel.y = NewCube_y; Out->PointedVoxel.z = NewCube_z;
            Out->CollisionPoint.x = Collision_s.x; Out->CollisionPoint.y = Collision_s.y; Out->CollisionPoint.z = Collision_s.z; Out->CollisionDistance = Collision_s.w;
            Out->CollisionAxe = 2; Out->CollisionFace = Face_s;
            Out->PointInCubeFace.x = fmod(Out->CollisionPoint.x, 256.0); Out->PointInCubeFace.y = fmod(Out->CollisionPoint.y, 256.0);

            //printf(" MATCH_S: %lf (%ld %ld %ld) C:%ld\n",Collision_s.w, NewCube_x, NewCube_y, NewCube_z, Cycle);
            Match_s = Cycle;
            MinW = Collision_s.w;
          } else DeferArmed = true;
        }
      }
    }

    // Vertical Y axis.

    if (Collide_Y)
    {
      if (Match_v==0 && Collision_v.w < MinW)
      {
        ActualCube_x = (Long)floor((Collision_v.x - Norm_v.x) / 256.0);   ActualCube_y = (Long)floor((Collision_v.y - Norm_v.y) / 256.0);   ActualCube_z = (Long)floor((Collision_v.z - Norm_v.z) / 256.0);
        NewCube_x = (Long)floor((Collision_v.x) / 256.0); NewCube_y = (Long)floor((Collision_v.y) / 256.0); NewCube_z = (Long)floor((Collision_v.z) / 256.0);
        //ActualCube_x = (Long)floor((Collision_v.x - Norm_v.x) / 256.0);   ActualCube_y = (Long)floor((Collision_v.y - Norm_v.y) / 256.0);   ActualCube_z = (Long)floor((Collision_v.z - Norm_v.z) / 256.0);
        //NewCube_x = (Long)floor((Collision_v.x + Norm_v.x) / 256.0); NewCube_y = (Long)floor((Collision_v.y + Norm_v.y) / 256.0); NewCube_z = (Long)floor((Collision_v.z + Norm_v.z) / 256.0);
        if (Face_v == 4) NewCube_y--;
        VoxelType = GetVoxel( NewCube_x, NewCube_y, NewCube_z);
        if (!VoxelTypeManager->VoxelTable[VoxelType]->Is_PlayerCanPassThrough ^ InvertCollision)
        {
          if (Armed)
          {
            Out->PredPointedVoxel.x = ActualCube_x; Out->PredPointedVoxel.y = ActualCube_y; Out->PredPointedVoxel.z = ActualCube_z;
            Out->PointedVoxel.x = NewCube_x; Out->PointedVoxel.y = NewCube_y; Out->PointedVoxel.z = NewCube_z;
            Out->CollisionPoint.x = Collision_v.x; Out->CollisionPoint.y = Collision_v.y; Out->CollisionPoint.z = Collision_v.z; Out->CollisionDistance = Collision_v.w;
            Out->CollisionAxe = 1; Out->CollisionFace = Face_v;
            Out->PointInCubeFace.x = fmod(Out->CollisionPoint.x, 256.0); Out->PointInCubeFace.y = fmod(Out->CollisionPoint.z, 256.0);

            //printf(" MATCH_V: %lf (%ld %ld %ld) C:%ld\n",Collision_v.w, NewCube_x, NewCube_y, NewCube_z,Cycle );
            Match_v = Cycle;
            MinW = Collision_v.w;
          } else DeferArmed = true;
        }
      }
    }

      //printf(" Match (H:%lf S:%lf V:%lf) \n", Collision_h.w, Collision_s.w, Collision_v.w);
      if (Match_h>0 && (Cycle - Match_h)>In->PlaneCubeDiff) { Out->Collided = true; return(true); }
      if (Match_s>0 && (Cycle - Match_s)>In->PlaneCubeDiff) { Out->Collided = true; return(true); }
      if (Match_v>0 && (Cycle - Match_v)>In->PlaneCubeDiff) { Out->Collided = true; return(true); }

    Collision_h.x += Offset_h.x; Collision_h.y += Offset_h.y; Collision_h.z += Offset_h.z; Collision_h.w += Offset_h.w;
    Collision_v.x += Offset_v.x; Collision_v.y += Offset_v.y; Collision_v.z += Offset_v.z; Collision_v.w += Offset_v.w;
    Collision_s.x += Offset_s.x; Collision_s.y += Offset_s.y; Collision_s.z += Offset_s.z; Collision_s.w += Offset_s.w;
    Cycle ++;
    if (DeferArmed) Armed = true;
  }
  Out->Collided = false;
  Out->CollisionAxe = 0;
  Out->CollisionFace = 0;
  Out->CollisionDistance = 0.0;
  Out->CollisionPoint = 0.0;
  return(false);
  //printf("first_h_x : %lf first_h_y %lf\n",first_h_x,first_h_y);
}

bool ZVoxelWorld::BlitBox(ZVector3L &SCoords, ZVector3L &DCoords, ZVector3L &Box)
{
  ZVector3L S,E, Step;
  Long x,y,z;

  if (SCoords.x > DCoords.x) { S.x = SCoords.x; E.x = S.x + Box.x; Step.x = 1;}
  else                       { E.x = SCoords.x; S.x = E.x + Box.x; Step.x = -1;}
  if (SCoords.y > DCoords.y) { S.y = SCoords.y; E.y = S.y + Box.y; Step.y = 1;}
  else                       { E.y = SCoords.y; S.y = E.y + Box.y; Step.y = -1;}
  if (SCoords.z > DCoords.z) { S.z = SCoords.z; E.z = S.z + Box.z; Step.z = 1;}
  else                       { E.z = SCoords.z; S.z = E.z + Box.z; Step.z = -1;}

  for (x = S.x; x < E.x ; x+=Step.x)
  for (z = S.z; z < E.z ; z+=Step.z)
  for (y = S.y; y < E.y ; y+=Step.y)
  {


  }

  // §§§ To finish

  return(true);
}


