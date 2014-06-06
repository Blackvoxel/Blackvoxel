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
 * ZRenderSorter.h
 *
 *  Created on: 16 mai 2013
 *      Author: laurent
 */

#ifndef Z_ZRENDER_SORTER_H
#define Z_ZRENDER_SORTER_H

//#ifndef Z_ZRENDER_SORTER_H
//#  include "ZRender_Sorter.h"
//#endif

#ifndef Z_ZVOXELSECTOR_H
#  include "ZVoxelSector.h"
#endif

class ZRender_Sorter
{
  public:

    class RenderEntry
    {
      public:
        ULong PackedInfos;
    };


    class RenderBucket
    {
      public:
        UShort VoxelType;
        UShort VoxelCount;
        RenderEntry RenderTable[ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_Z];
    };

    class HashEntry
    {
      public:
        ULong CycleNum;
        RenderBucket * Bucket;
    };

  protected:

    HashEntry BucketHash[65536];

    ULong CycleNum;

    RenderBucket * BucketTable[ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_Z];
    ULong UsedBuckets;
    ULong Render_ActualBucket;

  public:


  ZRender_Sorter()
  {
    ULong i;

    CycleNum = 0;
    UsedBuckets = 0;
    Render_ActualBucket = 0;
    for (i=0;i<65536;i++) { BucketHash[i].CycleNum = 0; BucketHash[i].Bucket = 0; }
    for (i=0;i<ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_Z;i++) { BucketTable[i] = 0; }
  }

  ~ZRender_Sorter()
  {
    ULong i;

    CycleNum = 0;
    UsedBuckets = 0;
    Render_ActualBucket = 0;
    for (i=0;i<65536;i++) { BucketHash[i].CycleNum = 0; BucketHash[i].Bucket = 0; }
    for (i=0;i<ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_Z;i++) { if (BucketTable[i]) { delete BucketTable[i]; BucketTable[i]=0; } }
  }

  protected:

  void Start()
  {
    CycleNum++;
    UsedBuckets = 0;
  }


  inline void AddVoxel(const UShort VoxelType, const Long x, const Long y, const Long z, const UByte CullingInfos)
  {
    RenderBucket * Bucket;

    if (!VoxelType) return;
    if (CullingInfos == DRAWFACE_NONE) return;
    if ( BucketHash[VoxelType].CycleNum != CycleNum )
    {
      if (!BucketTable[UsedBuckets]) BucketTable[UsedBuckets] = new RenderBucket();
      BucketHash[VoxelType].CycleNum = CycleNum;
      Bucket = BucketHash[VoxelType].Bucket = BucketTable[UsedBuckets++];
      Bucket->VoxelCount = 0;
      Bucket->VoxelType = VoxelType;
    }
    else
    {
      Bucket = BucketHash[VoxelType].Bucket;
    }

    register ULong PackedInfos;

    PackedInfos =  ( ( ( ( (x << 8) | y) << 8) | z) << 8 ) | CullingInfos;
    Bucket->RenderTable[Bucket->VoxelCount++].PackedInfos = PackedInfos;
  }

  public:

  void ProcessSector(ZVoxelSector * Sector)
  {
    register ULong x,y,z, Offset;

    Start();

    Offset = 0;
    for ( z=0 ; z < ZVOXELBLOCSIZE_Z ; z++ )
    {
      for ( x=0 ; x < ZVOXELBLOCSIZE_X ; x++ )
      {
        for ( y=0 ; y < ZVOXELBLOCSIZE_Y ; y++ )
        {
          AddVoxel( Sector->Data[Offset], x,y,z,Sector->FaceCulling[Offset]);
          Offset ++;
        }
      }
    }
  }

  void Rendering_Start()
  {
    Render_ActualBucket = 0;
  }

  RenderBucket * Rendering_GetNewBucket()
  {
    if (Render_ActualBucket >= UsedBuckets) return(0);
    return(BucketTable[Render_ActualBucket++]);
  }

  ULong GetBucketCount() { return(UsedBuckets); }


};





#endif /* Z_ZRENDER_SORTER_H */
