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
 * ZVoxelSector.h
 *
 *  Created on: 14 mars 2011
 *      Author: laurent
 */

#ifndef Z_ZVOXELSECTOR_H
#define Z_ZVOXELSECTOR_H

// #ifndef Z_ZVOXELSECTOR_H
// #  include "ZVoxelSector.h"
// #endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif

#ifndef Z_ZOBJECT_H
#  include "z/ZObject.h"
#endif

#ifndef Z_ZSECTOR_MODIFTRACKER_H
#  include "ZSector_ModifTracker.h"
#endif



#define ZVOXELBLOCSIZE_X 16
#define ZVOXELBLOCSIZE_Y 64
#define ZVOXELBLOCSIZE_Z 16
#define ZVOXELBLOCSHIFT_X 4
#define ZVOXELBLOCSHIFT_Y 6
#define ZVOXELBLOCSHIFT_Z 4
#define ZVOXELBLOCMASK_X 0xf
#define ZVOXELBLOCMASK_Y 0x3f
#define ZVOXELBLOCMASK_Z 0xf

#define DRAWFACE_LEFT   1
#define DRAWFACE_RIGHT  2
#define DRAWFACE_AHEAD  4
#define DRAWFACE_BEHIND 8
#define DRAWFACE_ABOVE  16
#define DRAWFACE_BELOW  32
#define DRAWFACE_ALL    (DRAWFACE_LEFT | DRAWFACE_RIGHT | DRAWFACE_AHEAD | DRAWFACE_BEHIND | DRAWFACE_ABOVE | DRAWFACE_BELOW )
#define DRAWFACE_NONE   0
#define DRAWFACE_FLANK  (DRAWFACE_LEFT | DRAWFACE_RIGHT | DRAWFACE_AHEAD | DRAWFACE_BEHIND)
#define DRAWFACE_UD     (DRAWFACE_ABOVE | DRAWFACE_BELOW)

#define VOXEL_LEFT 0
#define VOXEL_RIGHT 1
#define VOXEL_INFRONT 2
#define VOXEL_BEHIND  3
#define VOXEL_ABOVE   4
#define VOXEL_BELOW   5
#define VOXEL_INCENTER 6


class ZVoxelTypeManager;
class ZLightSpeedRandom;

class ZVoxelSector : public ZObject
{
  public:
    static ULong SectorsInMemory;

    ZVoxelTypeManager * VoxelTypeManager;

    ZVoxelSector * Next;
    ZVoxelSector * Pred;

    ZVoxelSector * GlobalList_Next;
    ZVoxelSector * GlobalList_Pred;

    Short Handle_x, Handle_y, Handle_z;
    Long Pos_x,Pos_y,Pos_z;
    Long Size_x, Size_y, Size_z;

    // Version control : Added for handling better world evolution.

    UShort ZoneType;     // The type of the zone.
    UShort ZoneVersion;  // The version of the generator used for this zone.
    UShort GeneratorVersion; // Main generator version. Updated at world change.
    UShort RingNum;

    // Is_Modified field Values.
    enum
    {
      CHANGE_NONE = 0,         // No changes. Assigned at sector creation.
      CHANGE_SAVEMASK = 28,    // If one of theses bits are set, sector must be saved.
      CHANGE_BITSECTORMODIFIED=1, // If sector is modified for any reason, this bit is set.
      CHANGE_BITDISCARDABLE  = 2, // Change aren't important and must be discarded at saving.
      CHANGE_BITUNIMPORTANT  = 4, // Change aren't important but must be saved anyway. The sector can be discarded if changes are made to the world.
      CHANGE_BITIMPORTANT    = 8, // The changes are important and must be preserved.
      CHANGE_BITCRITICAL     = 16,// The changes are critical as they are done directly by player. These sectors must be preserved at all cost.

      // These values is what you MUST use with functions using "ImportanceFactor" field

      CHANGE_DISCARDABLE     = CHANGE_BITDISCARDABLE | CHANGE_BITSECTORMODIFIED ,  // Changes aren't important at all and MUST be discarded at saving.
      CHANGE_UNIMPORTANT     = CHANGE_BITUNIMPORTANT | CHANGE_BITSECTORMODIFIED,   // Changes aren't important BUT MUST be saved anyway. Can be discarded if a new game version change zone disposition.
      CHANGE_IMPORTANT       = CHANGE_BITIMPORTANT   | CHANGE_BITSECTORMODIFIED,   // Changes are important and bust be preserved.
      CHANGE_CRITICAL        = CHANGE_BITCRITICAL    | CHANGE_BITSECTORMODIFIED    // Critical changes : work done directly by the player. Must be preserved at all cost.
    };

    bool Flag_Void_Regular;
    bool Flag_Void_Transparent;
    bool Flag_Render_Dirty;
    bool Flag_HighPriorityRefresh;
    bool Flag_IsVisibleAtLastRendering;
    bool Flag_DeletePending;
    bool Flag_NeedFullCulling;
    bool Flag_KeepInMemory;
    UByte Flag_IsModified;        // This sector has been modified. Values are bitfields (See above).
    bool Flag_IsSlowGeneration;   // This sector was generated using a very slow algorithm. Based on processer power, it may be a good idea to save it to disk rather than redoing the generation.
    bool Flag_IsActiveVoxels;     // Active voxels in this sector needs voxel processor attention.
    bool Flag_IsActiveLowRefresh; // Voxel processor will get activity in low frequency mode. Use LowRefresh_Mask to specify frequency;
    bool Flag_IsDebug;            // Debug flag
    bool Flag_NotStandardSize;
    bool Flag_NeedSortedRendering; // Activate new rendering code for better speed in some zones.

    //bool Flag_NeedPartialCulling;
    UByte PartialCulling;

    // Data stored by block
    ZMemSize DataSize;
    UShort    * Data;
    UByte     * FaceCulling;
    ZMemSize  * OtherInfos; // Informations autres
    UShort    * TempInfos;  // Température des voxels
    ZObject   * DisplayData;
    ZSectorModifTracker ModifTracker;

    ULong RefreshWaitCount;

    ULong LowRefresh_Mask;

    ZVoxelSector();
    ZVoxelSector( const ZVoxelSector &Sector);
    ZVoxelSector(Long Size_x, Long Size_y, Long Size_z);
    ~ZVoxelSector();
    void ChangeSize(Long Size_x, Long Size_y, Long Size_z);

protected:
    void InitSector(); // Fill sector content for reuse.

    void CleanupSector(); //

    void Compress_Short_RLE(UShort * Data, void * Stream);
    void Compress_OtherInfos_RLE(ZMemSize * Data, UShort * VoxelData, void * Stream);
    void Compress_FaceCulling_RLE(UByte * Data, void  * Stream);
    void Compress_Temperatures_RLE(UShort * Data, void  * Stream);

    bool Decompress_Short_RLE(UShort * Data, void * Stream);
    bool Decompress_FaceCulling_RLE(UByte * Data, void * Stream);
    bool Decompress_OtherInfos_RLE(ZMemSize * Data, void * Stream);
    bool Decompress_Temperatures_RLE(UShort * Data, void * Stream);
public:

    void SetVoxelTypeManager(ZVoxelTypeManager * VoxelTypeManager) { this->VoxelTypeManager = VoxelTypeManager; }

    void ReinitSector() { CleanupSector(); InitSector(); }
    void SetPos(Long x, Long y, Long z)
    {
      this->Pos_x = x;
      this->Pos_y = y;
      this->Pos_z = z;
    }

    // Handle point is set relative to default point.
    void SetHandle( Short x, Short y, Short z)
    {
      Handle_x = x;
      Handle_y = y;
      Handle_z = z;
    }

    void SetNotStandardSize( bool NotStandardSize=true )
    {
      Flag_NotStandardSize = NotStandardSize;
    }

    inline void SetCube(Long x, Long y, Long z, UShort CubeValue)
    {
      Long Offset;

      Offset = (y & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)*Size_y )+ ((z & ZVOXELBLOCMASK_Z) * (Size_y*Size_x));
      Data[Offset] = CubeValue;
      OtherInfos[Offset]=0;
    }

    inline void SetCube_WithExtension(Long x, Long y, Long z, UShort CubeValue, ZMemSize Extension)
    {
      Long Offset;

      Offset = (y & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)*Size_y )+ ((z & ZVOXELBLOCMASK_Z) * (Size_y*Size_x));
      Data[Offset] = CubeValue;
      OtherInfos[Offset]= Extension;
    }

    UShort GetCube(Long x, Long y, Long z)
    {
      Long Offset;

      Offset = (y & ZVOXELBLOCMASK_Y) + ( (x & ZVOXELBLOCMASK_X)*Size_y )+ ((z & ZVOXELBLOCMASK_Z) * (Size_y*Size_x));
      return(Data[Offset]);
    }

    void MakeSector()
    {
      Long x,y,z;
      UByte Cnt;

      if (Pos_y<0) {Cnt=1;Flag_Void_Regular=false;Flag_Void_Transparent=true;}
      if (Pos_y>=0){Cnt=0;Flag_Void_Regular=true;Flag_Void_Transparent=true;}

      for ( z=0; z<Size_x; z++ )
      {
        for ( y=0; y<Size_y; y++ )
        {
          for ( x=0; x<Size_x; x++ )
          {
            SetCube( x,y,z, Cnt );
          }
        }
      }
    }

    void Fill(UShort VoxelType)
    {
      ZMemSize i;

      for ( i=0 ; i<DataSize ; i++ )
      {
        Data[i] = VoxelType;
        FaceCulling[i] = 255;
      }
    }

    // Drawing Functions

    void Draw_safe_SetVoxel(Long x, Long y, Long z, UShort VoxelType, bool DrawIfVoid = true);
    void Draw_safe_Sphere(double x, double y, double z, double Radius, UShort VoxelType, bool DrawIfVoid = true);
    void Draw_safe_VoxelLine( ZRect3L * LineCoords, ZRect1d * Thickness, UShort VoxelType );
    void Draw_safe_VoxelLine_TickCtl( ZRect3L * LineCoords, double * ThicknessTable, ULong nThickIndices, UShort VoxelType );

    void Draw_safe_VoxelLine2( ZRect3L_2 * LineCoords, ZRect1d * Thickness, UShort VoxelType ); // old
    void Draw_safe_3DBlit(ZVoxelSector * SourceSector, ZVector3L * DestinationPoint, ZVector3L * SourcePoint, ZVector3L * Size);
    void Draw_subtree_1(ZVoxelSector * Sector, ZVector3d * Point, ZPolar3d * Direction, ZLightSpeedRandom * Random, double TotalLen);
    void Draw_safe_Tree_Type_1(ZVoxelSector * SourceSector, ZVector3L * Coords);

    void Draw_subtree_2( ZVector3d * Point, ZPolar3d * Direction, ZLightSpeedRandom * Random, double TotalLen);
    void Draw_safe_Tree_Type_2( ZVector3L * Coords);

    void Draw_trunc_3 ( ZVector3d * Point, ZPolar3d * Direction, ZLightSpeedRandom * Random, double TotalLen);
    void Draw_subtree_3( ZVector3d * Point, ZPolar3d * Direction, ZLightSpeedRandom * Random, double TotalLen);
    void Draw_safe_Tree_Type_3( ZVector3L * Coords);

    void Purge(UShort VoxelType);
    void Subst(UShort Source_VoxelType, UShort Dest_VoxelType);

    void BlitSector( ZVoxelSector * Source, ZVector3L * Offset);

    // Update control

    inline bool IsMustBeSaved()
    {

      bool IsModified = /*(Flag_IsModified & CHANGE_BITSECTORMODIFIED) && */ (Flag_IsModified & CHANGE_SAVEMASK );

      return( IsModified ); // Save only if sector is modified AND if modifications are rated important enough.
    }

    // Saving

    Bool Save(ULong UniverseNum, char const * OptFileName = 0);
    Bool Load(ULong UniverseNum, char const * OptFileName = 0);
    Bool DeleteSave(ULong UniverseNum); // Delete file on disk.
    Bool static GetSectorFileName(ULong UniverseNum, Long Pos_x, Long Pos_y, Long Pos_z, ZString & BaseDirectory, char * OutBuffer);
    Bool static GetSectorBaseDirectory(ZString & OutDirectory);
    Bool static CreateSectorPathSubstructure(ZString & SectorSaveBaseDirectory, ULong UniverseNum, Long Pos_x, Long Pos_y, Long Pos_z);


    Bool Old_Load(ULong UniverseNum);

    // Debug

    void DebugOut( const char * FileName );
    void DebugOutFCInfo( const char * FileName );
};



#endif
