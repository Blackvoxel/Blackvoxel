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
#include <GL/glew.h>
#include <math.h>
#include <stdio.h>
#include "SDL/SDL.h"

#  include "ZRender_Basic.h"

#ifndef Z_ZHIGHPERFTIMER_H
#  include "ZHighPerfTimer.h"
#endif

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

#ifndef Z_ZGAMESTAT_H
#  include "ZGameStat.h"
#endif


void ZRender_Basic::SetWorld( ZVoxelWorld * World )
{
  this->World = World;
}

void ZRender_Basic::SetCamera( ZCamera * Camera )
{
  this->Camera = Camera;
}

void ZRender_Basic::SetVoxelTypeManager( ZVoxelTypeManager * Manager )
{
  VoxelTypeManager = Manager;
}

void ZRender_Basic::Init()
{
  RadiusZones.SetSize(17,7,17);
  // RadiusZones.DrawZones( 5.0, 3.5, 3.0, 2.0 );
  // RadiusZones.DebugOut();
}

void ZRender_Basic::Render_DebugLine( ZVector3d & Start, ZVector3d & End)
{


  glDisable(GL_TEXTURE_2D);
  glEnable(GL_LINE_SMOOTH);

  glEnable (GL_LINE_SMOOTH);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
  glLineWidth (3.5);

  glColor3f(1.0,1.0,1.0);
  //glLineWidth(0.001f);
  //glPointSize(0.001f);
    glBegin(GL_LINES);
      glVertex3f((float)Start.x,(float)Start.y,(float)Start.z);glVertex3f((float)End.x,(float)End.y,(float)End.z);
    glEnd();

  glColor3f(1.0,1.0,1.0);
  glEnable(GL_TEXTURE_2D);
}

void ZRender_Basic::Render_VoxelSelector(ZVoxelCoords * SelectedVoxel, float r, float g, float b)
{

  //      PointedCube.x = 1;
  //      PointedCube.y = 0;
  //      PointedCube.z = 0;
  /*


        ZVector3f P1,P2,P3,P4,P5,P6,P7,P8;
        P1.x = SelectedVoxel->x * 256.0f + 0.0f;   P1.y = SelectedVoxel->y * 256.0f + 0.0f;   P1.z = SelectedVoxel->z * 256.0f + 0.0f;
        P2.x = SelectedVoxel->x * 256.0f + 0.0f;   P2.y = SelectedVoxel->y * 256.0f + 0.0f;   P2.z = SelectedVoxel->z * 256.0f + 256.0f;
        P3.x = SelectedVoxel->x * 256.0f + 256.0f; P3.y = SelectedVoxel->y * 256.0f + 0.0f;   P3.z = SelectedVoxel->z * 256.0f + 256.0f;
        P4.x = SelectedVoxel->x * 256.0f + 256.0f; P4.y = SelectedVoxel->y * 256.0f + 0.0f;   P4.z = SelectedVoxel->z * 256.0f + 0.0f;
        P5.x = SelectedVoxel->x * 256.0f + 0.0f;   P5.y = SelectedVoxel->y * 256.0f + 256.0f; P5.z = SelectedVoxel->z * 256.0f + 0.0f;
        P6.x = SelectedVoxel->x * 256.0f + 0.0f;   P6.y = SelectedVoxel->y * 256.0f + 256.0f; P6.z = SelectedVoxel->z * 256.0f + 256.0f;
        P7.x = SelectedVoxel->x * 256.0f + 256.0f; P7.y = SelectedVoxel->y * 256.0f + 256.0f; P7.z = SelectedVoxel->z * 256.0f + 256.0f;
        P8.x = SelectedVoxel->x * 256.0f + 256.0f; P8.y = SelectedVoxel->y * 256.0f + 256.0f; P8.z = SelectedVoxel->z * 256.0f + 0.0f;
*/

        glDisable(GL_TEXTURE_2D);

        //      PointedCube.x = 1;
        //      PointedCube.y = 0;
        //      PointedCube.z = 0;

              ZVector3f P1,P2,P3,P4,P5,P6,P7,P8;
              P1.x = SelectedVoxel->x * 256.0f + 0.0f;   P1.y = SelectedVoxel->y * 256.0f + 0.0f; P1.z = SelectedVoxel->z * 256.0f + 0.0f;
              P2.x = SelectedVoxel->x * 256.0f + 0.0f;   P2.y = SelectedVoxel->y * 256.0f + 0.0f; P2.z = SelectedVoxel->z * 256.0f + 256.0f;
              P3.x = SelectedVoxel->x * 256.0f + 256.0f; P3.y = SelectedVoxel->y * 256.0f + 0.0f; P3.z = SelectedVoxel->z * 256.0f + 256.0f;
              P4.x = SelectedVoxel->x * 256.0f + 256.0f; P4.y = SelectedVoxel->y * 256.0f + 0.0f; P4.z = SelectedVoxel->z * 256.0f + 0.0f;
              P5.x = SelectedVoxel->x * 256.0f + 0.0f;   P5.y = SelectedVoxel->y * 256.0f + 256.0f; P5.z = SelectedVoxel->z * 256.0f + 0.0f;
              P6.x = SelectedVoxel->x * 256.0f + 0.0f;   P6.y = SelectedVoxel->y * 256.0f + 256.0f; P6.z = SelectedVoxel->z * 256.0f + 256.0f;
              P7.x = SelectedVoxel->x * 256.0f + 256.0f; P7.y = SelectedVoxel->y * 256.0f + 256.0f; P7.z = SelectedVoxel->z * 256.0f + 256.0f;
              P8.x = SelectedVoxel->x * 256.0f + 256.0f; P8.y = SelectedVoxel->y * 256.0f + 256.0f; P8.z = SelectedVoxel->z * 256.0f + 0.0f;


              glDisable(GL_TEXTURE_2D);
              glColor3f(r,g,b);
              glEnable(GL_LINE_SMOOTH);

              glEnable (GL_LINE_SMOOTH);
              glEnable (GL_BLEND);
              glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
              glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
              glLineWidth (3.5);


              //glLineWidth(0.001f);
              //glPointSize(0.001f);
                glBegin(GL_LINES);
                  glVertex3f(P1.x,P1.y,P1.z);glVertex3f(P2.x,P2.y,P2.z);
                  glVertex3f(P2.x,P2.y,P2.z);glVertex3f(P3.x,P3.y,P3.z);
                  glVertex3f(P3.x,P3.y,P3.z);glVertex3f(P4.x,P4.y,P4.z);
                  glVertex3f(P4.x,P4.y,P4.z);glVertex3f(P1.x,P1.y,P1.z);

                  glVertex3f(P5.x,P5.y,P5.z);glVertex3f(P6.x,P6.y,P6.z);
                  glVertex3f(P6.x,P6.y,P6.z);glVertex3f(P7.x,P7.y,P7.z);
                  glVertex3f(P7.x,P7.y,P7.z);glVertex3f(P8.x,P8.y,P8.z);
                  glVertex3f(P8.x,P8.y,P8.z);glVertex3f(P5.x,P5.y,P5.z);

                  glVertex3f(P1.x,P1.y,P1.z);glVertex3f(P5.x,P5.y,P5.z);
                  glVertex3f(P2.x,P2.y,P2.z);glVertex3f(P6.x,P6.y,P6.z);
                  glVertex3f(P3.x,P3.y,P3.z);glVertex3f(P7.x,P7.y,P7.z);
                  glVertex3f(P4.x,P4.y,P4.z);glVertex3f(P8.x,P8.y,P8.z);
                glEnd();
              glColor3f(1.0,1.0,1.0);

              glEnable(GL_TEXTURE_2D);
        glEnable(GL_LINE_SMOOTH);

        glEnable (GL_LINE_SMOOTH);
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
        glLineWidth (3.5);


        //glLineWidth(0.001f);
        //glPointSize(0.001f);
          glBegin(GL_LINES);
            glVertex3f(P1.x,P1.y,P1.z);glVertex3f(P2.x,P2.y,P2.z);
            glVertex3f(P2.x,P2.y,P2.z);glVertex3f(P3.x,P3.y,P3.z);
            glVertex3f(P3.x,P3.y,P3.z);glVertex3f(P4.x,P4.y,P4.z);
            glVertex3f(P4.x,P4.y,P4.z);glVertex3f(P1.x,P1.y,P1.z);

            glVertex3f(P5.x,P5.y,P5.z);glVertex3f(P6.x,P6.y,P6.z);
            glVertex3f(P6.x,P6.y,P6.z);glVertex3f(P7.x,P7.y,P7.z);
            glVertex3f(P7.x,P7.y,P7.z);glVertex3f(P8.x,P8.y,P8.z);
            glVertex3f(P8.x,P8.y,P8.z);glVertex3f(P5.x,P5.y,P5.z);

            glVertex3f(P1.x,P1.y,P1.z);glVertex3f(P5.x,P5.y,P5.z);
            glVertex3f(P2.x,P2.y,P2.z);glVertex3f(P6.x,P6.y,P6.z);
            glVertex3f(P3.x,P3.y,P3.z);glVertex3f(P7.x,P7.y,P7.z);
            glVertex3f(P4.x,P4.y,P4.z);glVertex3f(P8.x,P8.y,P8.z);
          glEnd();
        glColor3f(1.0,1.0,1.0);
        glEnable(GL_TEXTURE_2D);

}



void ZRender_Basic::Render()
{

  ZHighPerfTimer Timer;
#if COMPILEOPTION_FINETIMINGTRACKING == 1
  ZHighPerfTimer Timer_SectorRefresh;
  ULong Time;
#endif
  ULong RenderedSectors;
  Long i;

   Timer.Start();

   Stat_RenderDrawFaces = 0;
   Stat_FaceTop = 0;
   Stat_FaceBottom = 0;
   Stat_FaceLeft = 0;
   Stat_FaceRight = 0;
   Stat_FaceFront = 0;
   Stat_FaceBack = 0;

 // Stats reset

   ZGameStat * Stat = GameEnv->GameStat;



  // Precomputing values for faster math

  ZVector3d::ZTransformParam FastCamParameters;

  FastCamParameters.SetRotation(-Camera->Yaw, Camera->Pitch, Camera->Roll);
  FastCamParameters.SetTranslation(-Camera->x, -Camera->y, -Camera->z);


   // Update per cycle.
   ULong UpdatePerCycle = 2;
   ULong n;


   if (Stat_RefreshWaitingSectorCount < 50) UpdatePerCycle = 1;
   if (Stat_RefreshWaitingSectorCount < 500) UpdatePerCycle = 2;
   else if (Stat->SectorRefresh_TotalTime <32) UpdatePerCycle = 5;
   Stat_RefreshWaitingSectorCount = 0;

   // Stat Reset

   Stat->SectorRefresh_Count = 0;
   Stat->SectorRefresh_TotalTime = 0;
   Stat->SectorRefresh_MinTime = 0;
   Stat->SectorRefresh_MaxTime = 0;
   Stat->SectorRender_Count = 0;
   Stat->SectorRender_TotalTime = 0;
   Stat->SectorRefresh_Waiting = 0;

   // Renderwaiting system

   for (i=0;i<64;i++) RefreshToDo[i] = 0;
   for (i=63;i>0;i--)
   {
     n = RefreshWaiters[i];
     if (n>UpdatePerCycle) n = UpdatePerCycle;
     UpdatePerCycle -= n;
     RefreshToDo[i] = n;
   }
   RefreshToDo[0] = UpdatePerCycle;

   for (i=0;i<64;i++) RefreshWaiters[i]=0;

  // Computing Frustum and Setting up Projection

   Aspect_Ratio = ((double)ViewportResolution.x / (double)ViewportResolution.y) * PixelAspectRatio;
   if (VerticalFOV < 5.0 ) VerticalFOV = 5.0;
   if (VerticalFOV > 160.0 ) VerticalFOV = 160.0;
   Frustum_V = tan(VerticalFOV / 2.0 * 0.017453293) * FocusDistance;
   Frustum_H = Frustum_V * Aspect_Ratio;

   Frustum_CullingLimit = ((Frustum_H > Frustum_V) ? Frustum_H : Frustum_V) * Optimisation_FCullingFactor;


   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(Frustum_H, -Frustum_H, -Frustum_V, Frustum_V, FocusDistance, 1000000.0); // Official Way
// glFrustum(50.0, -50.0, -31.0, 31.0, 50.0, 1000000.0); // Official Way

    // glFrustum(165.0, -165.0, -31.0, 31.0, 50.0, 1000000.0); // Eyefinity setting.


  // Objects of the world are translated and rotated to position camera at the right place.

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-Camera->Roll  , 0.0, 0.0, 1.0);
    glRotatef(-Camera->Pitch , 1.0, 0.0, 0.0);
    glRotatef(180-Camera->Yaw, 0.0, 1.0, 0.0);

    glTranslatef(-(float)Camera->x,-(float)Camera->y,-(float)Camera->z);

  // Clearing FrameBuffer and Z-Buffer

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glAlphaFunc(GL_GREATER, 0.2);
    glEnable(GL_ALPHA_TEST);

    // Long Start_x,Start_y,Start_z;
    Long Sector_x,Sector_y, Sector_z;
    // Long End_x, End_y, End_z;
    Long x,y,z;

    ZVoxelSector * Sector;
    Long Priority, PriorityBoost;
    ULong Sector_Refresh_Count;


  // Transforming Camera coords to sector coords. One Voxel is 256 observer units. One sector is 16x16x32.

    Sector_x = (ELong)Camera->x >> 12;
    Sector_y = (ELong)Camera->y >> 14;
    Sector_z = (ELong)Camera->z >> 12;


    // Start_x = Sector_x - hRenderRadius; End_x = Sector_x + hRenderRadius;
    // Start_y = Sector_y - vRenderRadius; End_y = Sector_y + vRenderRadius;
    // Start_z = Sector_z - hRenderRadius; End_z = Sector_z + hRenderRadius;

  // Rendering loop

    // printf("x: %lf, y: %lf, z: %lf Pitch: %lf Yaw: %lf \n",Camera->x, Camera->y, Camera->z, Camera->Pitch, Camera->Yaw);

  // Preparation and first rendering pass

    RenderedSectors = 0;
    Sector_Refresh_Count = 0;
    ZVector3d Cv, Cv2;
    ZSectorSphere::ZSphereEntry * SectorSphereEntry;
    ULong SectorsToProcess = SectorSphere.GetEntryCount();

    for (ULong Entry=0; Entry<SectorsToProcess; Entry++ )
    {
      SectorSphereEntry = SectorSphere.GetEntry(Entry);

      x = SectorSphereEntry->x + Sector_x;
      y = SectorSphereEntry->y + Sector_y;
      z = SectorSphereEntry->z + Sector_z;

      // for (x = Start_x ; x <= End_x ; x++)
      // for (y = Start_y; y <= End_y ; y++)
      // for (z = Start_z; z <= End_z ; z++)

          // try to see if sector is visible

          ZVector3d Cv, Cv2;
          bool SectorVisible;

          Cv.x = (double) ( ((ELong)x) << 12 );
          Cv.y = (double) ( ((ELong)y) << 14 );
          Cv.z = (double) ( ((ELong)z) << 12 );

          SectorVisible = false;
          Cv2.x = (0 * ZVOXELBLOCSIZE_X * 256.0); Cv2.y = (0 * ZVOXELBLOCSIZE_Y * 256.0); Cv2.z = (0 * ZVOXELBLOCSIZE_Z * 256.0); Cv2 += Cv ; SectorVisible |= Is_PointVisible(&FastCamParameters, &Cv2);
          Cv2.x = (1 * ZVOXELBLOCSIZE_X * 256.0); Cv2.y = (0 * ZVOXELBLOCSIZE_Y * 256.0); Cv2.z = (0 * ZVOXELBLOCSIZE_Z * 256.0); Cv2 += Cv ; SectorVisible |= Is_PointVisible(&FastCamParameters, &Cv2);
          Cv2.x = (1 * ZVOXELBLOCSIZE_X * 256.0); Cv2.y = (0 * ZVOXELBLOCSIZE_Y * 256.0); Cv2.z = (1 * ZVOXELBLOCSIZE_Z * 256.0); Cv2 += Cv ; SectorVisible |= Is_PointVisible(&FastCamParameters, &Cv2);
          Cv2.x = (0 * ZVOXELBLOCSIZE_X * 256.0); Cv2.y = (0 * ZVOXELBLOCSIZE_Y * 256.0); Cv2.z = (1 * ZVOXELBLOCSIZE_Z * 256.0); Cv2 += Cv ; SectorVisible |= Is_PointVisible(&FastCamParameters, &Cv2);
          Cv2.x = (0 * ZVOXELBLOCSIZE_X * 256.0); Cv2.y = (1 * ZVOXELBLOCSIZE_Y * 256.0); Cv2.z = (0 * ZVOXELBLOCSIZE_Z * 256.0); Cv2 += Cv ; SectorVisible |= Is_PointVisible(&FastCamParameters, &Cv2);
          Cv2.x = (1 * ZVOXELBLOCSIZE_X * 256.0); Cv2.y = (1 * ZVOXELBLOCSIZE_Y * 256.0); Cv2.z = (0 * ZVOXELBLOCSIZE_Z * 256.0); Cv2 += Cv ; SectorVisible |= Is_PointVisible(&FastCamParameters, &Cv2);
          Cv2.x = (1 * ZVOXELBLOCSIZE_X * 256.0); Cv2.y = (1 * ZVOXELBLOCSIZE_Y * 256.0); Cv2.z = (1 * ZVOXELBLOCSIZE_Z * 256.0); Cv2 += Cv ; SectorVisible |= Is_PointVisible(&FastCamParameters, &Cv2);
          Cv2.x = (0 * ZVOXELBLOCSIZE_X * 256.0); Cv2.y = (1 * ZVOXELBLOCSIZE_Y * 256.0); Cv2.z = (1 * ZVOXELBLOCSIZE_Z * 256.0); Cv2 += Cv ; SectorVisible |= Is_PointVisible(&FastCamParameters, &Cv2);

          Sector = World->FindSector(x,y,z);
          Priority      = RadiusZones.GetZone(x-Sector_x,y-Sector_y,z-Sector_z);
          PriorityBoost = (SectorVisible && Priority <= 2) ? 1 : 0;
          // Go = true;

          if (Sector)
          {
            Sector->Flag_IsVisibleAtLastRendering = SectorVisible || Priority>=4;
            // Display lists preparation
            if (Sector->Flag_Render_Dirty && GameEnv->Enable_NewSectorRendering)
            {
              if (Sector->Flag_IsDebug)
              {
                printf("Debug\n");
                //Sector->Flag_IsDebug = false;
              }

              // if (Sector_Refresh_Count < 5 || Priority==4)
              if ((RefreshToDo[Sector->RefreshWaitCount]) || Sector->Flag_HighPriorityRefresh )
              {

                #if COMPILEOPTION_FINETIMINGTRACKING == 1
                  Timer_SectorRefresh.Start();
                #endif

                RefreshToDo[Sector->RefreshWaitCount]--;
                Sector->Flag_HighPriorityRefresh = false;

                if (   Sector->Flag_NeedSortedRendering
                    || COMPILEOPTION_FORCE_SORTED_RENDERING ) MakeSectorRenderingData_Sorted(Sector);
                else                                          MakeSectorRenderingData(Sector);

                Sector_Refresh_Count++;
                Sector->RefreshWaitCount = 0;
                Stat->SectorRefresh_Count++;

                #if COMPILEOPTION_FINETIMINGTRACKING == 1
                  Timer_SectorRefresh.End(); Time = Timer_SectorRefresh.GetResult(); Stat->SectorRefresh_TotalTime += Time; if (Time < Stat->SectorRefresh_MinTime ) Stat->SectorRefresh_MinTime = Time; if (Time > Stat->SectorRefresh_MaxTime ) Stat->SectorRefresh_MaxTime = Time;
                #endif
              }
              else
              {
                Sector->RefreshWaitCount++;
                if (Sector->RefreshWaitCount > 31) Sector->RefreshWaitCount = 31;
                if (Priority==4) Sector->RefreshWaitCount++;
                RefreshWaiters[Sector->RefreshWaitCount]++;
                Stat_RefreshWaitingSectorCount++;
                Stat->SectorRefresh_Waiting++;
              }

            }


            // Rendering first pass
            if (   Sector->Flag_IsVisibleAtLastRendering
                && (!Sector->Flag_Void_Regular)
                && (Sector->DisplayData != 0)
                && (((ZRender_basic_displaydata *)Sector->DisplayData)->DisplayList_Regular != 0)
                )
              {

                #if COMPILEOPTION_FINETIMINGTRACKING == 1
                Timer_SectorRefresh.Start();
                #endif

                glCallList( ((ZRender_basic_displaydata *)Sector->DisplayData)->DisplayList_Regular );
                Stat->SectorRender_Count++;RenderedSectors++;

                #if COMPILEOPTION_FINETIMINGTRACKING == 1
                Timer_SectorRefresh.End(); Time = Timer_SectorRefresh.GetResult(); Stat->SectorRender_TotalTime += Time;
                #endif
              }
          }
          else
          {
            if (GameEnv->Enable_LoadNewSector) World->RequestSector(x,y,z,Priority + PriorityBoost );
          }

    }

  // Second pass rendering


    glDepthMask(GL_FALSE);

    SectorsToProcess = SectorSphere.GetEntryCount();

    for (ULong Entry=0; Entry<SectorsToProcess; Entry++ )
    {
      SectorSphereEntry = SectorSphere.GetEntry(Entry);

      x = SectorSphereEntry->x + Sector_x;
      y = SectorSphereEntry->y + Sector_y;
      z = SectorSphereEntry->z + Sector_z;

       Sector = World->FindSector(x,y,z);
          // printf("Sector : %ld %ld %ld %lu\n", x, y, z, (ULong)(Sector != 0));9
          if (Sector)
          {
            if (  Sector->Flag_IsVisibleAtLastRendering
               && (!Sector->Flag_Void_Transparent)
               && (Sector->DisplayData != 0)
               && (((ZRender_basic_displaydata *)Sector->DisplayData)->DisplayList_Transparent != 0)
               )
            {
              #if COMPILEOPTION_FINETIMINGTRACKING == 1
                Timer_SectorRefresh.Start();
              #endif

              glCallList( ((ZRender_basic_displaydata *)Sector->DisplayData)->DisplayList_Transparent );
              Stat->SectorRender_Count++;

              #if COMPILEOPTION_FINETIMINGTRACKING == 1
                Timer_SectorRefresh.End(); Time = Timer_SectorRefresh.GetResult(); Stat->SectorRender_TotalTime += Time;
              #endif
            }

          }
    }
    glDepthMask(GL_TRUE);


    // ***************************
    // Cube designation
    // ***************************

    ZRayCast_in In;

    In.Camera = Camera;
    In.MaxCubeIterations = 150;
    In.PlaneCubeDiff = 100;
    In.MaxDetectionDistance = 30000.0;

 //   if (World->RayCast( &In, PointedVoxel ))
 //   {
      // Render_VoxelSelector( &PointedVoxel->PointedVoxel, 1.0,1.0,1.0 );
      //Render_VoxelSelector( &PointedVoxel->PredPointedVoxel, 1.0, 0.0, 0.0);
//    }

    // Debug ****************************************************

    ZVector3d Norm, Tmp;
    Norm.x = 0; Norm.y = 0; Norm.z = 1;

    // X axis rotation
    Tmp.y = Norm.y * cos(-Camera->Pitch/57.295779513) - Norm.z * sin(-Camera->Pitch/57.295779513);
    Tmp.z = Norm.y * sin(-Camera->Pitch/57.295779513) + Norm.z * cos(-Camera->Pitch/57.295779513);
    Norm.y = Tmp.y; Norm.z = Tmp.z;
    // Y axis rotation
    Tmp.x = Norm.z*sin(Camera->Yaw/57.295779513) + Norm.x * cos(Camera->Yaw/57.295779513);
    Tmp.z = Norm.z*cos(Camera->Yaw/57.295779513) - Norm.x * sin(Camera->Yaw/57.295779513);
    Norm.x = Tmp.x; Norm.z = Tmp.z;
    Norm.y = Tmp.y;
    // printf("Norm(%lf %lf %lf)\n",Norm.x,Norm.y,Norm.z);

    In.MaxCubeIterations = 150;
    In.MaxDetectionDistance = (1536.0 / COMPILEOPTION_VOXELSIZEFACTOR);//1000000.0;

    ZVector3d CamPoint(Camera->x,Camera->y,Camera->z);
    ZVector3d Zp;

    Zp = PointedVoxel->CollisionPoint; Zp.y = PointedVoxel->CollisionPoint.y + 100.0;

    if (World->RayCast_Vector(ZVector3d(Camera->x,Camera->y,Camera->z),Norm , &In, PointedVoxel))
    {
      if (PointedVoxel->CollisionDistance < In.MaxDetectionDistance)
      {
        PointedVoxel->Collided = true;
        if (BvProp_DisplayVoxelSelector) Render_VoxelSelector( &PointedVoxel->PointedVoxel, 1.0,1.0,1.0 );
      }
      else PointedVoxel->Collided = false;
    }


    // ***************************
    // Réticule
    // ***************************



    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0, 1440, 900.0 , 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (BvProp_CrossHairType==1 && BvProp_DisplayCrossHair)
    {
      glDisable(GL_TEXTURE_2D);
      glBegin(GL_POLYGON);
        glColor3f(1.0,1.0,1.0);
        glVertex3f(720.0f-1.0f, 450.0f-20.0f , 0.0f);
        glVertex3f(720.0f+1.0f, 450.0f-20.0f , 0.0f);
        glVertex3f(720.0f+1.0f, 450.0f-10.0f , 0.0f);
        glVertex3f(720.0f-1.0f, 450.0f-10.0f , 0.0f);
      glEnd();

      glBegin(GL_POLYGON);
        glColor3f(1.0,1.0,1.0);
        glVertex3f(720.0f-1.0f, 450.0f+10.0f , 0.0f);
        glVertex3f(720.0f+1.0f, 450.0f+10.0f , 0.0f);
        glVertex3f(720.0f+1.0f, 450.0f+20.0f , 0.0f);
        glVertex3f(720.0f-1.0f, 450.0f+20.0f , 0.0f);
      glEnd();

      glBegin(GL_POLYGON);
        glColor3f(1.0,1.0,1.0);
        glVertex3f(720.0f-20.0f, 450.0f-1.0f , 0.0f);
        glVertex3f(720.0f-10.0f, 450.0f-1.0f , 0.0f);
        glVertex3f(720.0f-10.0f, 450.0f+1.0f , 0.0f);
        glVertex3f(720.0f-20.0f, 450.0f+1.0f , 0.0f);
      glEnd();

      glBegin(GL_POLYGON);
        glColor3f(1.0,1.0,1.0);
        glVertex3f(720.0f+20.0f, 450.0f+1.0f , 0.0f);
        glVertex3f(720.0f+10.0f, 450.0f+1.0f , 0.0f);
        glVertex3f(720.0f+10.0f, 450.0f-1.0f , 0.0f);
        glVertex3f(720.0f+20.0f, 450.0f-1.0f , 0.0f);
      glEnd();

      glColor3f(1.0,1.0,1.0);
      glEnable(GL_TEXTURE_2D);
    }

    // Voile coloré

    if (Camera->ColoredVision.Activate)
    {
      glDisable(GL_TEXTURE_2D);
      glDisable(GL_DEPTH_TEST);
      glColor4f(Camera->ColoredVision.Red,Camera->ColoredVision.Green,Camera->ColoredVision.Blue,Camera->ColoredVision.Opacity);
      glBegin(GL_POLYGON);
        glVertex3f(0.0f   , 0.0f   , 0.0f);
        glVertex3f(1440.0f, 0.0f   , 0.0f);
        glVertex3f(1440.0f, 900.0f , 0.0f);
        glVertex3f(0.0f   , 900.0f , 0.0f);
      glEnd();
      glColor3f(1.0,1.0,1.0);
      glEnable(GL_TEXTURE_2D);
      glEnable(GL_DEPTH_TEST);
    }

/*
    if (Camera->ColoredVision.Activate)
    {
      glDisable(GL_TEXTURE_2D);
      glDisable(GL_DEPTH_TEST);
      glColor4f(Camera->ColoredVision.Red,Camera->ColoredVision.Green,Camera->ColoredVision.Blue,Camera->ColoredVision.Opacity);
      glBegin(GL_POLYGON);
        glVertex3f(0.0f   , 0.0f   , 0.0f);
        glVertex3f(1440.0f, 0.0f   , 0.0f);
        glVertex3f(1440.0f, 900.0f , 0.0f);
        glVertex3f(0.0f   , 900.0f , 0.0f);
      glEnd();
      glColor3f(1.0,1.0,1.0);
      glEnable(GL_TEXTURE_2D);
      glEnable(GL_DEPTH_TEST);
    }
*/



    Timer.End();

    /*printf("Frame Time : %lu Rend Sects: %lu Draw Faces :%lu Top:%lu Bot:%lu Le:%lu Ri:%lu Front:%lu Back:%lu\n",Timer.GetResult(), RenderedSectors, Stat_RenderDrawFaces, Stat_FaceTop, Stat_FaceBottom,
           Stat_FaceLeft,Stat_FaceRight,Stat_FaceFront,Stat_FaceBack);*/

    //printf("RenderedSectors : %lu\n",RenderedSectors);
    //SDL_GL_SwapBuffers( );
}

/*
void ZRender_Basic::RenderSector2(ZVoxelSector * Sector)
{
  Long x,y,z;
  ULong info;
  UShort cube;
  ULong FlankText, TopText;
  ULong Offset;
  float cubx, cuby, cubz;
  Long Sector_Display_x, Sector_Display_y, Sector_Display_z;

  Sector_Display_x = Sector->Pos_x * Sector->Size_x * 256;
  Sector_Display_y = Sector->Pos_y * Sector->Size_y * 256;
  Sector_Display_z = Sector->Pos_z * Sector->Size_z * 256;


  for ( z=0 ; z < Sector->Size_z ; z++ )
  {
    for ( x=0 ; x < Sector->Size_x ; x++ )
    {
      for ( y=0 ; y < Sector->Size_y ; y++ )
      {
        Offset = y + ( x*Sector->Size_y )+ (z * (Sector->Size_y*Sector->Size_x));
        cube = Sector->Data[Offset];

        // info = 65535;
        info = Sector->FaceCulling[Offset];

        //if (Sector->Pos_y<0) cub0.0e = 1;

        if (cube>0 && info != DRAWFACE_NONE)
        {
          switch (cube)
          {
            case 1: FlankText = TextureName[0]; TopText = TextureName[1]; break;
            case 2: FlankText = TextureName[2]; TopText = TextureName[2]; break;
          }

          cubx = (float)(x*256 + Sector_Display_x);
          cuby = (float)(y*256 + Sector_Display_y);
          cubz = (float)(z*256 + Sector_Display_z);

          if (info & DRAWFACE_FLANK) glBindTexture(GL_TEXTURE_2D,FlankText);

          //Left
          if (info & DRAWFACE_LEFT)
          {
            glBegin(GL_POLYGON);
              glTexCoord2f(0.0,0.0); glVertex3f(cubx, cuby, cubz );
              glTexCoord2f(1.0,0.0); glVertex3f(cubx , cuby, cubz + 256.0);
              glTexCoord2f(1.0,1.0); glVertex3f(cubx , cuby +256.0, cubz + 256.0);
              glTexCoord2f(0.0,1.0); glVertex3f(cubx , cuby +256.0, cubz);
            glEnd();
          }

          // Right
          if (info & DRAWFACE_RIGHT)
          {
            glBegin(GL_POLYGON);
              glTexCoord2f(0.0,0.0); glVertex3f(cubx + 256.0 , cuby, cubz );
              glTexCoord2f(1.0,0.0); glVertex3f(cubx + 256.0, cuby, cubz + 256.0);
              glTexCoord2f(1.0,1.0); glVertex3f(cubx + 256.0, cuby +256.0, cubz + 256.0);
              glTexCoord2f(0.0,1.0); glVertex3f(cubx + 256.0, cuby +256.0, cubz);
            glEnd();
          }

          //Front
          if (info & DRAWFACE_AHEAD)
          {
            glBegin(GL_POLYGON);
              glTexCoord2f(0.0,0.0); glVertex3f(cubx, cuby, cubz);
              glTexCoord2f(1.0,0.0); glVertex3f(cubx + 256.0, cuby, cubz);
              glTexCoord2f(1.0,1.0); glVertex3f(cubx + 256.0, cuby +256.0, cubz);
              glTexCoord2f(0.0,1.0); glVertex3f(cubx , cuby +256.0, cubz);
            glEnd();
          }

          //Back
          if (info & DRAWFACE_BEHIND)
          {
            glBegin(GL_POLYGON);
              glTexCoord2f(0.0,0.0); glVertex3f(cubx, cuby, cubz + 256.0);
              glTexCoord2f(1.0,0.0); glVertex3f(cubx + 256.0, cuby, cubz + 256.0);
              glTexCoord2f(1.0,1.0); glVertex3f(cubx + 256.0, cuby +256.0, cubz + 256.0);
              glTexCoord2f(0.0,1.0); glVertex3f(cubx , cuby +256.0, cubz + 256.0);
            glEnd();
          }

          if (info & DRAWFACE_UD) glBindTexture(GL_TEXTURE_2D,TopText);

          // Top
          if (info & DRAWFACE_ABOVE)
          {
            glBegin(GL_POLYGON);
              glTexCoord2f(0.0,0.0); glVertex3f(cubx , cuby  + 256.0, cubz);
              glTexCoord2f(1.0,0.0); glVertex3f(cubx + 256.0, cuby  + 256.0, cubz);
              glTexCoord2f(1.0,1.0); glVertex3f(cubx + 256.0, cuby  + 256.0, cubz +256.0);
              glTexCoord2f(0.0,1.0); glVertex3f(cubx, cuby  + 256.0 ,cubz +256.0);
            glEnd();
          }

         // Bottom
         if (info & DRAWFACE_BELOW)
         {
           glBegin(GL_POLYGON);
             glTexCoord2f(0.0,0.0); glVertex3f(cubx, cuby, cubz);
             glTexCoord2f(1.0,0.0); glVertex3f(cubx + 256.0, cuby, cubz);
             glTexCoord2f(1.0,1.0); glVertex3f(cubx + 256.0, cuby, cubz +256.0);
             glTexCoord2f(0.0,1.0); glVertex3f(cubx, cuby,cubz +256.0);
           glEnd();
          }
        }


      }
    }
  }
}
*/
Bool ZRender_Basic::LoadVoxelTexturesToGPU()
{
  ULong i;
  ZVoxelType * VoxelType;

  for (i=0;i<65536;i++)
  {
    if ( !(VoxelType = VoxelTypeManager->VoxelTable[i])->Is_NoType)
    {
      if (VoxelType->MainTexture)
      {
        glGenTextures(1,&VoxelType->OpenGl_TextureRef);
        glBindTexture(GL_TEXTURE_2D,VoxelType->OpenGl_TextureRef);
        /*
        glTexImage2D (GL_TEXTURE_2D,      //Type : texture 2D
                      0,                 //Mipmap : none
                      GL_RGBA8,          //Format : RGBA
                      VoxelType->MainTexture->Width,         //Width
                      VoxelType->MainTexture->Height,        //Height
                      0,                 //Largeur du bord : 0     img4_m6.LoadBMP("textures/texture_cubeglow_mip_6.bmp");
                      GL_BGRA,    //Format : RGBA
                      GL_UNSIGNED_BYTE,   //Type des couleurs
                      (GLubyte *)VoxelType->MainTexture->BitmapMemory//Addresse de l'image
                     );
        */


        if (VoxelTypeManager->VoxelTable[i]->Draw_LinearInterpolation)
        {
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // GL_LINEAR GL_NEAREST
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else
        {
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_LINEAR GL_NEAREST
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }

        // if (i & 1) glTexParameteri(GL_TEXTURE_2D, 0x84FE /*TEXTURE_MAX_ANISOTROPY_EXT*/, 8);
        if (COMPILEOPTION_ANISOTROPICTEXTURINGLEVEL>0) glTexParameteri(GL_TEXTURE_2D, 0x84FE /*TEXTURE_MAX_ANISOTROPY_EXT*/, COMPILEOPTION_ANISOTROPICTEXTURINGLEVEL);
        gluBuild2DMipmaps(GL_TEXTURE_2D,      //Type : texture 2D
            GL_RGBA8,          //Format : RGBA
            VoxelType->MainTexture->Width,         //Width
            VoxelType->MainTexture->Height,        //Height
            GL_BGRA,    //Format : RGBA
            GL_UNSIGNED_BYTE,   //Type des couleurs
            (GLubyte *)VoxelType->MainTexture->BitmapMemory//Addresse de l'image
           );


          //glTexEnvf(0x8500 /* TEXTURE_FILTER_CONTROL_EXT */, 0x8501 /* TEXTURE_LOD_BIAS_EXT */,3.0);
        // if ((i & 1) ) glTexEnvf(0x8500 /* TEXTURE_FILTER_CONTROL_EXT */, 0x8501 /* TEXTURE_LOD_BIAS_EXT */,-4.25);

        glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
      }
    }
  }

  return(true);
}

void ZRender_Basic::FreeDisplayData(ZVoxelSector * Sector)
{
  ZRender_basic_displaydata * DisplayData;

  DisplayData = (ZRender_basic_displaydata *)Sector->DisplayData;

  if (DisplayData)
  {
    if (DisplayData->DisplayList_Regular)
    {
      glDeleteLists (DisplayData->DisplayList_Regular,1);
      DisplayData->DisplayList_Regular = 0;
    }
    if (DisplayData->DisplayList_Transparent)
    {
      glDeleteLists (DisplayData->DisplayList_Transparent,1);
      DisplayData->DisplayList_Transparent = 0;
    }
    delete DisplayData;
  }
}

Bool ZRender_Basic::LoadTexturesToGPU()
{
  ULong i;
  ULong TextureCount;
  ZTexture_Entry * Entry;

  if (!TextureManager) return(false);

  TextureCount = TextureManager->GetTexture_Count();

  for (i=0;i<TextureCount;i++)
  {
    if ((Entry = TextureManager->GetTextureEntry(i)))
    {
      glGenTextures(1,&Entry->OpenGl_TextureRef);
      glBindTexture(GL_TEXTURE_2D,Entry->OpenGl_TextureRef);
      if (Entry->LinearInterpolation)
      {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // GL_LINEAR GL_NEAREST
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      }
      else
      {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_LINEAR GL_NEAREST
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      }

      if (COMPILEOPTION_ANISOTROPICTEXTURINGLEVEL>0) glTexParameteri(GL_TEXTURE_2D, 0x84FE /*TEXTURE_MAX_ANISOTROPY_EXT*/, COMPILEOPTION_ANISOTROPICTEXTURINGLEVEL);
      gluBuild2DMipmaps(GL_TEXTURE_2D,      //Type : texture 2D
          GL_RGBA8,          //Format : RGBA
          Entry->Texture->Width,         //Width
          Entry->Texture->Height,        //Height
          GL_BGRA,    //Format : RGBA
          GL_UNSIGNED_BYTE,   //Type des couleurs
          (GLubyte *)Entry->Texture->BitmapMemory//Addresse de l'image
         );


          //glTexEnvf(0x8500 /* TEXTURE_FILTER_CONTROL_EXT */, 0x8501 /* TEXTURE_LOD_BIAS_EXT */,3.0);
        // if ((i & 1) ) glTexEnvf(0x8500 /* TEXTURE_FILTER_CONTROL_EXT */, 0x8501 /* TEXTURE_LOD_BIAS_EXT */,-4.25);

        glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    }
  }

  return(true);
}

void ZRender_Basic::MakeSectorRenderingData(ZVoxelSector * Sector)
{
  Long x,y,z;
  ULong info;
  UShort cube, prevcube;

  ULong Offset;
  float cubx, cuby, cubz;
  Long Sector_Display_x, Sector_Display_y, Sector_Display_z;
  ZRender_basic_displaydata * DisplayData;
  ULong Pass;
  Bool Draw;
  ZVoxelType ** VoxelTypeTable = VoxelTypeManager->VoxelTable;
  ZVector3f P0,P1,P2,P3,P4,P5,P6,P7;


  // Display list creation or reuse.

  if (Sector->DisplayData == 0) { Sector->DisplayData = new ZRender_basic_displaydata; }
  DisplayData = (ZRender_basic_displaydata *)Sector->DisplayData;
  if ( DisplayData->DisplayList_Regular == 0 )    DisplayData->DisplayList_Regular = glGenLists(1);
  if ( DisplayData->DisplayList_Transparent == 0) DisplayData->DisplayList_Transparent = glGenLists(1);

  if (Sector->Flag_Render_Dirty || 1 )
  {
    Sector_Display_x = Sector->Pos_x * Sector->Size_x * 256;
    Sector_Display_y = Sector->Pos_y * Sector->Size_y * 256;
    Sector_Display_z = Sector->Pos_z * Sector->Size_z * 256;

    Sector->Flag_Void_Regular = true;
    Sector->Flag_Void_Transparent = true;

    for (Pass=0; Pass<2; Pass++)
    {
      switch(Pass)
      {
        case 0: glNewList(DisplayData->DisplayList_Regular, GL_COMPILE); break;
        case 1: glNewList(DisplayData->DisplayList_Transparent, GL_COMPILE); break;
      }
      prevcube = 0;
      for ( z=0 ; z < Sector->Size_z ; z++ )
      {
        for ( x=0 ; x < Sector->Size_x ; x++ )
        {
          for ( y=0 ; y < Sector->Size_y ; y++ )
          {
            Offset = y + ( x*Sector->Size_y )+ (z * (Sector->Size_y*Sector->Size_x));
            cube = Sector->Data[Offset];
            info = Sector->FaceCulling[Offset];


            if (cube>0 && info != DRAWFACE_NONE)
            {
              switch(Pass)
              {
                case 0: if ( VoxelTypeTable[cube]->Draw_TransparentRendering ) { Draw = false;  Sector->Flag_Void_Transparent = false; }
                        else                                                   { Draw = true;   Sector->Flag_Void_Regular     = false; }
                        break;
                case 1: Draw = ( VoxelTypeTable[cube]->Draw_TransparentRendering ) ? true:false; break;
              }
            } else Draw = false;

            if (Draw)
            {
              // glTexEnvf(0x8500 /* TEXTURE_FILTER_CONTROL_EXT */, 0x8501 /* TEXTURE_LOD_BIAS_EXT */,VoxelTypeManager->VoxelTable[cube]->TextureLodBias);
              if (cube != prevcube) glBindTexture(GL_TEXTURE_2D, VoxelTypeManager->VoxelTable[cube]->OpenGl_TextureRef);
              prevcube = cube;
              cubx = (float)(x*256 + Sector_Display_x);
              cuby = (float)(y*256 + Sector_Display_y);
              cubz = (float)(z*256 + Sector_Display_z);

              if (VoxelTypeTable[cube]->DrawInfo & ZVOXEL_DRAWINFO_SPECIALRENDERING ) {VoxelTypeTable[cube]->SpecialRender(cubx,cuby,cubz); continue; }

              P0.x = cubx;           P0.y = cuby;          P0.z = cubz;
              P1.x = cubx + 256.0f;  P1.y = cuby;          P1.z = cubz;
              P2.x = cubx + 256.0f;  P2.y = cuby;          P2.z = cubz+256.0f;
              P3.x = cubx;           P3.y = cuby;          P3.z = cubz+256.0f;
              P4.x = cubx;           P4.y = cuby + 256.0f; P4.z = cubz;
              P5.x = cubx + 256.0f;  P5.y = cuby + 256.0f; P5.z = cubz;
              P6.x = cubx + 256.0f;  P6.y = cuby + 256.0f; P6.z = cubz + 256.0f;
              P7.x = cubx;           P7.y = cuby + 256.0f; P7.z = cubz + 256.0f;

              //Left
              if (info & DRAWFACE_LEFT)
              {
                Stat_RenderDrawFaces++;
                Stat_FaceLeft++;
                glBegin(GL_TRIANGLES);
                  glTexCoord2f(0.25,0.25); glVertex3f(P4.x, P4.y, P4.z );
                  glTexCoord2f(0.25,0.0);  glVertex3f(P0.x, P0.y, P0.z );
                  glTexCoord2f(0.50,0.0);  glVertex3f(P3.x, P3.y, P3.z );
                  glTexCoord2f(0.50,0.0);  glVertex3f(P3.x, P3.y, P3.z );
                  glTexCoord2f(0.50,0.25); glVertex3f(P7.x, P7.y, P7.z );
                  glTexCoord2f(0.25,0.25); glVertex3f(P4.x, P4.y, P4.z );
                glEnd();
              }

              // Right
              if (info & DRAWFACE_RIGHT)
              {
                Stat_RenderDrawFaces++;
                Stat_FaceRight++;
                glBegin(GL_TRIANGLES);
                  glTexCoord2f(0.25,0.50); glVertex3f(P5.x, P5.y, P5.z );
                  glTexCoord2f(0.50,0.50);  glVertex3f(P6.x, P6.y, P6.z );
                  glTexCoord2f(0.50,0.75);  glVertex3f(P2.x, P2.y, P2.z );
                  glTexCoord2f(0.50,0.75);  glVertex3f(P2.x, P2.y, P2.z );
                  glTexCoord2f(0.25,0.75); glVertex3f(P1.x, P1.y, P1.z );
                  glTexCoord2f(0.25,0.50); glVertex3f(P5.x, P5.y, P5.z );
                glEnd();
              }

              //Front
              if (info & DRAWFACE_AHEAD)
              {
                Stat_RenderDrawFaces++;
                Stat_FaceFront++;
                glBegin(GL_TRIANGLES);
                  glTexCoord2f(0.0,0.25); glVertex3f(P0.x, P0.y, P0.z );
                  glTexCoord2f(0.25,0.25);  glVertex3f(P4.x, P4.y, P4.z );
                  glTexCoord2f(0.25,0.50);  glVertex3f(P5.x, P5.y, P5.z );
                  glTexCoord2f(0.25,0.50);  glVertex3f(P5.x, P5.y, P5.z );
                  glTexCoord2f(0.0,0.50); glVertex3f(P1.x, P1.y, P1.z );
                  glTexCoord2f(0.0,0.25); glVertex3f(P0.x, P0.y, P0.z );
                glEnd();
              }

              //Back
              if (info & DRAWFACE_BEHIND)
              {
                Stat_RenderDrawFaces++;
                Stat_FaceBack++;
                glBegin(GL_TRIANGLES);
                  glTexCoord2f(0.75,0.50); glVertex3f(P2.x, P2.y, P2.z );
                  glTexCoord2f(0.50,0.50);  glVertex3f(P6.x, P6.y, P6.z );
                  glTexCoord2f(0.75,0.25);  glVertex3f(P3.x, P3.y, P3.z );
                  glTexCoord2f(0.75,0.25);  glVertex3f(P3.x, P3.y, P3.z );
                  glTexCoord2f(0.50,0.50); glVertex3f(P6.x, P6.y, P6.z );
                  glTexCoord2f(0.50,0.25); glVertex3f(P7.x, P7.y, P7.z );
                glEnd();
              }

              // Top
              if (info & DRAWFACE_ABOVE)
              {
                Stat_RenderDrawFaces++;
                Stat_FaceTop++;
                glBegin(GL_TRIANGLES);
                  glTexCoord2f(0.25,0.25); glVertex3f(P4.x, P4.y, P4.z );
                  glTexCoord2f(0.50,0.25);  glVertex3f(P7.x, P7.y, P7.z );
                  glTexCoord2f(0.25,0.50);  glVertex3f(P5.x, P5.y, P5.z );
                  glTexCoord2f(0.25,0.50);  glVertex3f(P5.x, P5.y, P5.z );
                  glTexCoord2f(0.50,0.25); glVertex3f(P7.x, P7.y, P7.z );
                  glTexCoord2f(0.50,0.50); glVertex3f(P6.x, P6.y, P6.z );
                glEnd();
              }

             // Bottom
             if (info & DRAWFACE_BELOW)
             {
               Stat_RenderDrawFaces++;
               Stat_FaceBottom++;
               glBegin(GL_TRIANGLES);
                 glTexCoord2f(1.0,0.25); glVertex3f(P0.x, P0.y, P0.z );
                 glTexCoord2f(1.0,0.50);  glVertex3f(P1.x, P1.y, P1.z );
                 glTexCoord2f(0.75,0.25);  glVertex3f(P3.x, P3.y, P3.z );
                 glTexCoord2f(0.75,0.25);  glVertex3f(P3.x, P3.y, P3.z );
                 glTexCoord2f(1.0,0.50); glVertex3f(P1.x, P1.y, P1.z );
                 glTexCoord2f(0.75,0.50); glVertex3f(P2.x, P2.y, P2.z );
               glEnd();
              }
            }


          }
        }
      }
      glEndList();

      // if in the first pass, the sector has no transparent block, the second pass is cancelled.

      if (Sector->Flag_Void_Transparent) break;
    }
    Sector->Flag_Render_Dirty = false;
  }

}























void ZRender_Basic::MakeSectorRenderingData_Sorted(ZVoxelSector * Sector)
{
  Long x,y,z;
  ULong info, i;
  UShort VoxelType, prevVoxelType;

  // ULong Offset;
  float cubx, cuby, cubz;
  Long Sector_Display_x, Sector_Display_y, Sector_Display_z;
  ZRender_basic_displaydata * DisplayData;
  ULong Pass;
  ZVoxelType ** VoxelTypeTable = VoxelTypeManager->VoxelTable;
  ZVector3f P0,P1,P2,P3,P4,P5,P6,P7;

  ZRender_Sorter::RenderBucket * RenderBucket;

  // Set flags

  Sector->Flag_Void_Regular = true;
  Sector->Flag_Void_Transparent = true;
  Sector->Flag_Render_Dirty = false;

  // Render sorter action

  RenderSorter.ProcessSector(Sector);
  if (!RenderSorter.GetBucketCount()) return;

  // Check what blocktypes

  RenderSorter.Rendering_Start();
  for (i=0;i<RenderSorter.GetBucketCount();i++)
  {
    VoxelType = RenderSorter.Rendering_GetNewBucket()->VoxelType;

    if (VoxelTypeTable[VoxelType]->Draw_TransparentRendering) Sector->Flag_Void_Transparent = false;
    else                                                      Sector->Flag_Void_Regular     = false;
  }

  // Display list creation or reuse.

  if (Sector->DisplayData == 0) { Sector->DisplayData = new ZRender_basic_displaydata; }
  DisplayData = (ZRender_basic_displaydata *)Sector->DisplayData;
  if ( (!Sector->Flag_Void_Regular)     && (DisplayData->DisplayList_Regular == 0) )    DisplayData->DisplayList_Regular = glGenLists(1);
  if ( (!Sector->Flag_Void_Transparent) && (DisplayData->DisplayList_Transparent == 0) ) DisplayData->DisplayList_Transparent = glGenLists(1);

  // Computing Sector Display coordinates;

  Sector_Display_x = (Sector->Pos_x * Sector->Size_x) << 8;
  Sector_Display_y = (Sector->Pos_y * Sector->Size_y) << 8;
  Sector_Display_z = (Sector->Pos_z * Sector->Size_z) << 8;

  for (Pass=0; Pass<2; Pass++)
  {
    if (!Pass) { if (Sector->Flag_Void_Regular)     continue; glNewList(DisplayData->DisplayList_Regular, GL_COMPILE); }
    else       { if (Sector->Flag_Void_Transparent) continue; glNewList(DisplayData->DisplayList_Transparent, GL_COMPILE); }

    prevVoxelType = 0;

    RenderSorter.Rendering_Start();

    while (( RenderBucket = RenderSorter.Rendering_GetNewBucket() ))
    {
      VoxelType = RenderBucket->VoxelType;

      // Is it the right voxel transparency type for that rendering pass ?

      if ( (Pass>0) != VoxelTypeTable[VoxelType]->Draw_TransparentRendering) continue;

      // Render one RenderBucket.

      for (i=0;i<RenderBucket->VoxelCount;i++)
      {
        register ULong Pck;

        // Gettint Voxel Informations from the table.

        Pck = RenderBucket->RenderTable[i].PackedInfos;

        // Unpacking voxel infos

        info = Pck & 0xFF;
        z    = Pck >> 8 & 0xFF;
        y    = Pck >> 16 & 0xFF;
        x    = Pck >> 24 & 0xFF;

        // Offset = y + ( x << ZVOXELBLOCSHIFT_Y )+ (z << (ZVOXELBLOCSHIFT_Y + ZVOXELBLOCSHIFT_X));

        // glTexEnvf(0x8500 /* TEXTURE_FILTER_CONTROL_EXT */, 0x8501 /* TEXTURE_LOD_BIAS_EXT */,VoxelTypeManager->VoxelTable[VoxelType]->TextureLodBias);
        if (VoxelType != prevVoxelType) glBindTexture(GL_TEXTURE_2D, VoxelTypeManager->VoxelTable[VoxelType]->OpenGl_TextureRef);
        prevVoxelType = VoxelType;
        cubx = (float)(x*256 + Sector_Display_x);
        cuby = (float)(y*256 + Sector_Display_y);
        cubz = (float)(z*256 + Sector_Display_z);

        if (VoxelTypeTable[VoxelType]->DrawInfo & ZVOXEL_DRAWINFO_SPECIALRENDERING ) {VoxelTypeTable[VoxelType]->SpecialRender(cubx,cuby,cubz); continue; }

        P0.x = cubx;           P0.y = cuby;          P0.z = cubz;
        P1.x = cubx + 256.0f;  P1.y = cuby;          P1.z = cubz;
        P2.x = cubx + 256.0f;  P2.y = cuby;          P2.z = cubz+256.0f;
        P3.x = cubx;           P3.y = cuby;          P3.z = cubz+256.0f;
        P4.x = cubx;           P4.y = cuby + 256.0f; P4.z = cubz;
        P5.x = cubx + 256.0f;  P5.y = cuby + 256.0f; P5.z = cubz;
        P6.x = cubx + 256.0f;  P6.y = cuby + 256.0f; P6.z = cubz + 256.0f;
        P7.x = cubx;           P7.y = cuby + 256.0f; P7.z = cubz + 256.0f;

        //Left
        if (info & DRAWFACE_LEFT)
        {
          Stat_RenderDrawFaces++;
          Stat_FaceLeft++;
          glBegin(GL_TRIANGLES);
            glTexCoord2f(0.25,0.25); glVertex3f(P4.x, P4.y, P4.z );
            glTexCoord2f(0.25,0.0);  glVertex3f(P0.x, P0.y, P0.z );
            glTexCoord2f(0.50,0.0);  glVertex3f(P3.x, P3.y, P3.z );
            glTexCoord2f(0.50,0.0);  glVertex3f(P3.x, P3.y, P3.z );
            glTexCoord2f(0.50,0.25); glVertex3f(P7.x, P7.y, P7.z );
            glTexCoord2f(0.25,0.25); glVertex3f(P4.x, P4.y, P4.z );
          glEnd();
        }

        // Right
        if (info & DRAWFACE_RIGHT)
        {
          Stat_RenderDrawFaces++;
          Stat_FaceRight++;
          glBegin(GL_TRIANGLES);
            glTexCoord2f(0.25,0.50); glVertex3f(P5.x, P5.y, P5.z );
            glTexCoord2f(0.50,0.50);  glVertex3f(P6.x, P6.y, P6.z );
            glTexCoord2f(0.50,0.75);  glVertex3f(P2.x, P2.y, P2.z );
            glTexCoord2f(0.50,0.75);  glVertex3f(P2.x, P2.y, P2.z );
            glTexCoord2f(0.25,0.75); glVertex3f(P1.x, P1.y, P1.z );
            glTexCoord2f(0.25,0.50); glVertex3f(P5.x, P5.y, P5.z );
          glEnd();
        }

        //Front
        if (info & DRAWFACE_AHEAD)
        {
          Stat_RenderDrawFaces++;
          Stat_FaceFront++;
          glBegin(GL_TRIANGLES);
            glTexCoord2f(0.0,0.25); glVertex3f(P0.x, P0.y, P0.z );
            glTexCoord2f(0.25,0.25);  glVertex3f(P4.x, P4.y, P4.z );
            glTexCoord2f(0.25,0.50);  glVertex3f(P5.x, P5.y, P5.z );
            glTexCoord2f(0.25,0.50);  glVertex3f(P5.x, P5.y, P5.z );
            glTexCoord2f(0.0,0.50); glVertex3f(P1.x, P1.y, P1.z );
            glTexCoord2f(0.0,0.25); glVertex3f(P0.x, P0.y, P0.z );
          glEnd();
        }

        //Back
        if (info & DRAWFACE_BEHIND)
        {
          Stat_RenderDrawFaces++;
          Stat_FaceBack++;
          glBegin(GL_TRIANGLES);
            glTexCoord2f(0.75,0.50); glVertex3f(P2.x, P2.y, P2.z );
            glTexCoord2f(0.50,0.50);  glVertex3f(P6.x, P6.y, P6.z );
            glTexCoord2f(0.75,0.25);  glVertex3f(P3.x, P3.y, P3.z );
            glTexCoord2f(0.75,0.25);  glVertex3f(P3.x, P3.y, P3.z );
            glTexCoord2f(0.50,0.50); glVertex3f(P6.x, P6.y, P6.z );
            glTexCoord2f(0.50,0.25); glVertex3f(P7.x, P7.y, P7.z );
          glEnd();
        }

        // Top
        if (info & DRAWFACE_ABOVE)
        {
          Stat_RenderDrawFaces++;
          Stat_FaceTop++;
          glBegin(GL_TRIANGLES);
            glTexCoord2f(0.25,0.25); glVertex3f(P4.x, P4.y, P4.z );
            glTexCoord2f(0.50,0.25);  glVertex3f(P7.x, P7.y, P7.z );
            glTexCoord2f(0.25,0.50);  glVertex3f(P5.x, P5.y, P5.z );
            glTexCoord2f(0.25,0.50);  glVertex3f(P5.x, P5.y, P5.z );
            glTexCoord2f(0.50,0.25); glVertex3f(P7.x, P7.y, P7.z );
            glTexCoord2f(0.50,0.50); glVertex3f(P6.x, P6.y, P6.z );
          glEnd();
        }

       // Bottom
       if (info & DRAWFACE_BELOW)
       {
         Stat_RenderDrawFaces++;
         Stat_FaceBottom++;
         glBegin(GL_TRIANGLES);
           glTexCoord2f(1.0,0.25); glVertex3f(P0.x, P0.y, P0.z );
           glTexCoord2f(1.0,0.50);  glVertex3f(P1.x, P1.y, P1.z );
           glTexCoord2f(0.75,0.25);  glVertex3f(P3.x, P3.y, P3.z );
           glTexCoord2f(0.75,0.25);  glVertex3f(P3.x, P3.y, P3.z );
           glTexCoord2f(1.0,0.50); glVertex3f(P1.x, P1.y, P1.z );
           glTexCoord2f(0.75,0.50); glVertex3f(P2.x, P2.y, P2.z );
         glEnd();
        }


      }
    }
    glEndList();
  }
}


void ZRender_Basic::ComputeAndSetAspectRatio(double VerticalFOV, double PixelAspectRatio, ZVector2L & ViewportResolution)
{
  double FocusDistance = 50.0;
  VerticalFOV = 63.597825649;
  PixelAspectRatio = 1.0;

  double Frustum_V;
  double Frustum_H;
  double Aspect_Ratio;

  Aspect_Ratio = (ViewportResolution.x / ViewportResolution.y) * PixelAspectRatio;

  // FOV Limitation to safe values.

  if (VerticalFOV < 5.0 ) VerticalFOV = 5.0;
  if (VerticalFOV > 160.0 ) VerticalFOV = 160.0;

  Frustum_V = tan(VerticalFOV / 2.0) * FocusDistance;
  Frustum_H = Frustum_V * Aspect_Ratio;

  glFrustum(Frustum_H, -Frustum_H, -Frustum_V, Frustum_V, FocusDistance, 1000000.0); // Official Way

}

