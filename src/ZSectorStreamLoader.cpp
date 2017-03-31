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
 * ZSectorStreamLoader.cpp
 *
 *  Created on: 8 mars 2011
 *      Author: laurent
 */

#include "ZSectorStreamLoader.h"
#include "SDL/SDL.h"

ULong debug_deletecount = 0;

ZMonoSizeMemoryPool ZSectorTagHash::DefaultMemoryPool;

UByte ZFileSectorLoader::STableX[] = { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2 };
UByte ZFileSectorLoader::STableZ[] = { 3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6 };
UByte ZFileSectorLoader::STableY[] = { 9,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                       0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                       0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                       0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                       0,18 };

UShort ZFileSectorLoader::OfTableY []= { 63, 0,1,2,3, 4,5,6,7, 8,9,10,11, 12,13,14,15,
                                         16,17,18,19, 20,21,22,23, 24,25,26,27, 28,29,30,31,
                                         32,33,34,35, 36,37,38,39, 40,41,42,43, 44,45,46,47,
                                         48,49,50,51, 52,53,54,55, 56,57,58,59, 60,61,62,63,
                                         0 };
UShort ZFileSectorLoader::OfTableX []= {  15*ZVOXELBLOCSIZE_Y,
                                          0*ZVOXELBLOCSIZE_Y,  1*ZVOXELBLOCSIZE_Y,  2*ZVOXELBLOCSIZE_Y, 3*ZVOXELBLOCSIZE_Y,
                                          4*ZVOXELBLOCSIZE_Y,  5*ZVOXELBLOCSIZE_Y,  6*ZVOXELBLOCSIZE_Y, 7*ZVOXELBLOCSIZE_Y,
                                          8*ZVOXELBLOCSIZE_Y,  9*ZVOXELBLOCSIZE_Y, 10*ZVOXELBLOCSIZE_Y,11*ZVOXELBLOCSIZE_Y,
                                          12*ZVOXELBLOCSIZE_Y, 13*ZVOXELBLOCSIZE_Y, 14*ZVOXELBLOCSIZE_Y,15*ZVOXELBLOCSIZE_Y,
                                          0*ZVOXELBLOCSIZE_Y };
UShort ZFileSectorLoader::OfTableZ []= {  15*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X,
                                          0 *ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X, 1 *ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X, 2*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X, 3*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X,
                                          4 *ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X, 5 *ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X, 6*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X, 7*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X,
                                          8 *ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X, 9 *ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X,10*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X,11*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X,
                                          12*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X, 13*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X,14*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X,15*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X,
                                          0 *ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X };


int ZFileSectorLoader::thread_func(void * Data)
{
  ZFileSectorLoader * SectorLoader = (ZFileSectorLoader *) Data;
  while (SectorLoader->ThreadContinue)
  {
    if (!SectorLoader->MakeTasks()) SDL_Delay(10);
  }

  return(0);
}

ZSectorLoader::ZSectorLoader()
{

}

ZSectorLoader::~ZSectorLoader()
{
}

ZFileSectorLoader::ZFileSectorLoader()
{
  ReadySectorList   = new ZSectorRingList(1024*1024);
  EjectedSectorList = new ZSectorRingList(1024*1024);
  SectorRecycling   = new ZSectorRingList(1024*1024);
  VoxelTypeManager = 0;
  UniverseNum = 1;
  WorkingEmptySector = new ZVoxelSector;
  WorkingEmptySector->Fill(0);
  WorkingFullSector = new ZVoxelSector;
  WorkingFullSector->Fill(1);
  Thread = 0;
  ThreadContinue = false;
}

ZFileSectorLoader::~ZFileSectorLoader()
{
  if (WorkingEmptySector)
  {
    delete WorkingEmptySector;
    WorkingEmptySector = 0;
  }
  if (WorkingFullSector)
  {
    delete WorkingFullSector;
    WorkingFullSector = 0;
  }

  ZVoxelSector * Sector;

  if (ReadySectorList)
  {
    while (( Sector = ReadySectorList->PullFromList() ))
    {
      if (COMPILEOPTION_ALLOWSAVE) Sector->Save(UniverseNum);
      delete Sector;
    }
    delete ReadySectorList;
    ReadySectorList = 0;
  }
  if (EjectedSectorList)
  {
    EjectedSectorList->FreeRemainingContent();
    delete EjectedSectorList;
    EjectedSectorList = 0;
  }
  UniverseNum = 0;
}

bool ZFileSectorLoader::Init()
{
  ThreadContinue = true;
  Thread = (SDL_Thread * )SDL_CreateThread(thread_func, this);
  if (!SectorCreator.LoadTemplateImages()) return(false);
  return(true);
}

bool ZFileSectorLoader::LoadSector(Long x, Long y, Long z)
{
  ZVoxelSector * NewSector;
  bool Redo, TryLoad;

  if ( !RequestTag.find(x,y,z) )
  {
    NewSector = SectorRecycling->PullFromList(); // Try recycling some old used sector.
    if (!NewSector) {NewSector = new ZVoxelSector; }

    TryLoad = true;
    do
    {
      Redo = false;

      NewSector->Pos_x = x; NewSector->Pos_y = y; NewSector->Pos_z = z;
      NewSector->SetVoxelTypeManager(VoxelTypeManager);

      if (TryLoad && NewSector->Load(UniverseNum))
      {
        // Does this sector must be regenerated ?
        if ( 0==(NewSector->Flag_IsModified & ZVoxelSector::CHANGE_SAVEMASK) )
        {
          if (COMPILEOPTION_ALLOWSAVE) NewSector->DeleteSave(UniverseNum);
          NewSector->ReinitSector();
          Redo = true;
          TryLoad = false;
          continue;
        }
      }
      else
      {
        SectorCreator.GenerateSector(NewSector);
      }
    } while (Redo);

    // Set the options for further edge faceculling.

    NewSector->Flag_NeedFullCulling = false;
    NewSector->PartialCulling = DRAWFACE_ALL;

    // Add it in the tag

    RequestTag.Add(x,y,z);

    // Partial face culling on the sector. Edges must be completed later when other sectors will be availlable.

    LimitedUpdateFaceCulling(NewSector );
    //NoDrawFaceCulling(NewSector);

    // Push it to the list for integration in the world on the main thread.

    ReadySectorList->PushToList(NewSector);
    return(true);
  }
  return(false);
}

ULong CarZCounter=0;

bool ZFileSectorLoader::MakeTasks()
{
  Long x,y,z;
  UByte Pri;
  bool WorkRemain;

  // Sector Loading

  WorkRemain = false;
  while (1)
  {
    if      (RequestList[5].PullFromList(x,y,z)) {Pri = 4; }
    else if (RequestList[4].PullFromList(x,y,z)) {Pri = 4; }
    else if (RequestList[3].PullFromList(x,y,z)) {Pri = 3; }
    else if (RequestList[2].PullFromList(x,y,z)) {Pri = 2; }
    else if (RequestList[1].PullFromList(x,y,z)) {Pri = 1; }
    else if (RequestList[0].PullFromList(x,y,z)) {Pri = 0; }
    else break;

    if (LoadSector(x,y,z) && Pri<4) { WorkRemain = true; break; }
  }

  // Sector Unloading

  ZVoxelSector * Sector;

  while ( (Sector = EjectedSectorList->PullFromList()) )
  {
    RequestTag.Remove(Sector->Pos_x, Sector->Pos_y, Sector->Pos_z);
    //printf("Deleted : %lx, %lu L2 Start:%lu End:%lu nEntries:%lu\n", Sector,++debug_deletecount,EjectedSectorList->debug_getstart(),EjectedSectorList->debug_GetEnd(),EjectedSectorList->debug_GetnEntries() );
    if (COMPILEOPTION_ALLOWSAVE)
    {
#if COMPILEOPTION_SAVEONLYMODIFIED==1
      if (Sector->IsMustBeSaved())
#endif
      {
        Sector->Save(UniverseNum);
      }
    }
    //delete Sector;

     Sector->ReinitSector();
     SectorRecycling->PushToList(Sector);
  }

  return(WorkRemain);
}

void ZFileSectorLoader::Request_Sector(Long x, Long y, Long z, Long Priority)
{
  RequestList[Priority].PushToList(x,y,z);
  //printf("Request :%ld,%ld,%ld\n",x,y,z);
}

void ZFileSectorLoader::Eject_Sector( ZVoxelSector * Sector)
{
  if (!EjectedSectorList->PushToList(Sector)) printf ("Ejection Stall\n");
}

ZVoxelSector * ZFileSectorLoader::GetRequested()
{
  return( ReadySectorList->PullFromList() );
}

void ZFileSectorLoader::Cleanup()
{
  ThreadContinue = false;
  if (Thread) SDL_WaitThread((SDL_Thread*)Thread, NULL);
}

void ZFileSectorLoader::LimitedUpdateFaceCulling(ZVoxelSector * Sector )
{
  ZVoxelSector * SectorTable[27];
  ZVoxelType **  VoxelTypeTable;

  UShort Temp;
  UShort * BlocMatrix[3];
  void * tmpp;
  ULong i;

  BlocMatrix[0] = new UShort[9];
  BlocMatrix[1] = new UShort[9];
  BlocMatrix[2] = new UShort[9];

  for (i=0;i<27;i++) SectorTable[i] = WorkingFullSector;
  SectorTable[0] = Sector;
  SectorTable[1] = WorkingFullSector;
  SectorTable[2] = WorkingFullSector;
  SectorTable[3] = WorkingFullSector;
  SectorTable[6] = WorkingFullSector;
  SectorTable[9] = WorkingFullSector;
  SectorTable[18]= WorkingFullSector;

  Long xc,yc,zc;
  Long xp,yp,zp;
  Long xpp,ypp,zpp;
  UByte info;
  ZVoxelType * Vt[6];
  Bool TransparentVoxel;

  VoxelTypeTable = VoxelTypeManager->VoxelTable;

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

        // Scrolling bloc matrix by exchanging references.
        tmpp = BlocMatrix[0];
        BlocMatrix[0] = BlocMatrix[1];
        BlocMatrix[1] = BlocMatrix[2];
        BlocMatrix[2] = (UShort *) tmpp;

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

        info = 0;
        if (BlocMatrix[1][4] > 0)
        {
          Vt[0] = VoxelTypeTable[BlocMatrix[1][1]];
          Vt[1] = VoxelTypeTable[BlocMatrix[1][7]];
          Vt[2] = VoxelTypeTable[BlocMatrix[1][3]];
          Vt[3] = VoxelTypeTable[BlocMatrix[1][5]];
          Vt[4] = VoxelTypeTable[BlocMatrix[0][4]];
          Vt[5] = VoxelTypeTable[BlocMatrix[2][4]];
          Temp = BlocMatrix[1][4];
          TransparentVoxel = VoxelTypeTable[Temp]->Draw_TransparentRendering;

          info |= ( Vt[0]->Draw_FullVoxelOpacity || (TransparentVoxel && Vt[0]->Draw_TransparentRendering) ) ? 0 : DRAWFACE_AHEAD;
          info |= ( Vt[1]->Draw_FullVoxelOpacity || (TransparentVoxel && Vt[1]->Draw_TransparentRendering) ) ? 0 : DRAWFACE_BEHIND;
          info |= ( Vt[2]->Draw_FullVoxelOpacity || (TransparentVoxel && Vt[2]->Draw_TransparentRendering) ) ? 0 : DRAWFACE_LEFT;
          info |= ( Vt[3]->Draw_FullVoxelOpacity || (TransparentVoxel && Vt[3]->Draw_TransparentRendering) ) ? 0 : DRAWFACE_RIGHT;
          info |= ( Vt[4]->Draw_FullVoxelOpacity || (TransparentVoxel && Vt[4]->Draw_TransparentRendering) ) ? 0 : DRAWFACE_BELOW;
          info |= ( Vt[5]->Draw_FullVoxelOpacity || (TransparentVoxel && Vt[5]->Draw_TransparentRendering) ) ? 0 : DRAWFACE_ABOVE;
        }

        // if ( (y==-1) && (yc==63) ) info = 255;
        /*
        if (BlocMatrix[1][4]>0)
        {
          if (VoxelTypeTable[BlocMatrix[1][4]]->Draw_TransparentRendering) SectorTable[0]->Flag_Void_Transparent = false;
          else                                                             SectorTable[0]->Flag_Void_Regular = false;
        }
        */
        // Write face culling info to face culling table

        SectorTable[0]->FaceCulling[OfTableX[xp]+OfTableY[yp]+OfTableZ[zp]] = info;

      }
    }
  }

  delete [] BlocMatrix[0];
  delete [] BlocMatrix[1];
  delete [] BlocMatrix[2];

}


void ZFileSectorLoader::NoDrawFaceCulling(ZVoxelSector * Sector )
{
  ULong i;

  for (i=0;i < (ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_Z); i++)
  {
    Sector->FaceCulling[i] = DRAWFACE_NONE;
  }
}
