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
 * ZSectorSphere.cpp
 *
 *  Created on: 26 oct. 2012
 *      Author: laurent
 */

#include "ZSectorSphere.h"
#include <stdio.h>
#include <string.h>


void ZSectorSphere::PartSort(ULong Start, ULong ItemCount, ZSphereEntry * SortBuffer)
{
  ULong i,FirstPartCount, SecondPartCount, FirstPartStart, SecondPartStart, EndPart;
  if (ItemCount <= 1 )
  {
    return;
  }
  SecondPartCount = ItemCount/2;
  FirstPartCount = ItemCount - SecondPartCount;
  FirstPartStart = Start;
  SecondPartStart = Start + FirstPartCount;
  EndPart = FirstPartStart + ItemCount;

  // Sort subtables

  PartSort(FirstPartStart , FirstPartCount , SortBuffer);
  PartSort(SecondPartStart, SecondPartCount, SortBuffer);

  // Copy first partition into buffer

  for (i=Start;i<SecondPartStart; i++)
  {
    SortBuffer[i]=SectorList[i];
  }

  // Make partition fusion

  for(i=Start;i<EndPart;i++)
  {
    if (FirstPartCount > 0 && SecondPartCount > 0)
    {
      if (SortBuffer[FirstPartStart].SectorDistance <= SectorList[SecondPartStart].SectorDistance) { SectorList[i] = SortBuffer[FirstPartStart++]; FirstPartCount--; }
      else                                                                                         { SectorList[i] = SectorList[SecondPartStart++]; SecondPartCount--; }
    }
    else
    {
      if (FirstPartCount) { SectorList[i] = SortBuffer[FirstPartStart++];  FirstPartCount--;  }
      else                { SectorList[i] = SectorList[SecondPartStart++]; SecondPartCount--; }
    }
  }
}

void ZSectorSphere::Sort()
{

  ZSphereEntry * SortBuffer;

  SortBuffer = new ZSphereEntry[nSlots];
  if (nSlots>0) PartSort(0,nSlots, SortBuffer);
  delete SortBuffer;
}

ZSectorSphere::ZSectorSphere()
{
  SectorList = 0;
  nSlots = 0;
}

ZSectorSphere::~ZSectorSphere()
{
  if (SectorList) delete [] SectorList;
}

void ZSectorSphere::Init(Long Render_Distance_h, Long Render_Distance_v)
{
  Long x,y,z;
  ZMemSize Offset;

  double dist_x,dist_y,dist_z;

  nSlots = (Render_Distance_h * 2 + 1 ) * (Render_Distance_h * 2 + 1 ) * (Render_Distance_v * 2 + 1);

  if ((SectorList)) delete [] SectorList;
  SectorList = new ZSphereEntry[nSlots];

  Offset = 0;
  for ( x = - Render_Distance_h; x <= Render_Distance_h; x++  )
    for ( y = - Render_Distance_v; y <= Render_Distance_v; y++  )
      for ( z = - Render_Distance_h; z <= Render_Distance_h; z++  )
      {
        SectorList[Offset].x = x;
        SectorList[Offset].y = y;
        SectorList[Offset].z = z;
        dist_x = ((double) ((((ELong)x) << 12) ));
        dist_y = ((double) ((((ELong)y) << 14) ));
        dist_z = ((double) ((((ELong)z) << 12) ));
        SectorList[Offset].SectorDistance = sqrt( dist_x * dist_x + dist_y * dist_y + dist_z * dist_z );
        Offset++;
      }

  // Sort the list;

  Sort();

}

void ZSectorSphere::debugout(char * FileSpec)
{
  FILE * fh;
  char Buffer[4096];
  ULong i;

  fh = fopen(FileSpec,"wb");
  if ((fh))
  {
    for (i=0; i<nSlots;i++)
    {
      sprintf(Buffer,"X:%d Y:%d Z:%d Dist: %lf\n",SectorList[i].x, SectorList[i].y, SectorList[i].z, SectorList[i].SectorDistance );
      fwrite(Buffer, strlen(Buffer), 1, fh);
    }
    fclose(fh);
  }
}
