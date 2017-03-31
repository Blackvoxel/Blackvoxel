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
 * ZWorldGenesis.h
 *
 *  Created on: 10 sept. 2011
 *      Author: laurent
 */

#ifndef Z_ZWORLDGENESIS_H
#define Z_ZWORLDGENESIS_H

//#ifndef Z_ZWORLDGENESIS_H
//#  include "ZWorldGenesis.h"
//#endif

#ifndef Z_ZFASTRANDOM_H
#  include "z/ZFastRandom.h"
#endif

#ifndef Z_ZVOXELSECTOR_H
#  include "ZVoxelSector.h"
#endif

#ifndef Z_ZVOXELGFX_TREE_H
#  include "ZVoxelGfx_Tree.h"
#endif

#ifndef Z_BITMAP_IMAGE_H
#  include "z/ZBitmapImage.h"
#endif

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif

#ifndef Z_ZSECTORLOADER_H
#  include "ZSectorLoader.h"
#endif

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif

//ifndef Z_ZGENERICCANVA_H
//  include "z/ZGenericCanva.h"
//endif

#ifndef Z_ZGENERICCANVA2_H
#  include "z/ZGenericCanva_2.h"
#endif

#define Z_GENESISMAP_SIZE 128

class ZWorldGenesis
{
  public:
    static const char * ZoneMap_New[];
    static const char * HeightMap_New[];
    static const char * RingNum[];
    static const Long ZoneYOffset[];

  protected:

    enum { InclusionQuantity = 32000000,
           BlackWoods_Level = -14};
    UShort         ConvCN[256];

    ZLightSpeedRandom RandomGen;
    ZFastRandom       AltRandomGen;

    double GetHeightMap(Long x, Long z);
  public:

    ZGenericCanva<UByte> Canva_1;
    ZGenericCanva<UByte> Canva_2;
    ZGenericCanva<UByte> Canva_3_1;
    ZGenericCanva<UByte> Canva_3_2;
    ZGenericCanva<UByte> Canva_3_3;
    ZGenericCanva<UByte> Canva_3_4;
    ZGenericCanva<UByte> Canva_4;


    ZVoxelSector T3dTemplate_1; // Tree
    ZVoxelSector T3dTemplate_2; // Boulder 1
    ZVoxelSector Template_Vegetation_1;
    ZVoxelSector Template_Vegetation_2;
    ZVoxelSector Template_Vegetation_3;
    ZVoxelSector * TreeTable[16];

    ZWorldGenesis()
    {
      ULong i;
      // char to num
      for (i=0;i<256;i++) ConvCN[i]=0;
      for (i=0;i<=9;i++)  ConvCN[i+'0']=i;
      for (i=0;i<26;i++)  ConvCN[i+'A']=10+i;
      for (i=0;i<26;i++)  ConvCN[i+'a']=36+i;
    }

    ~ZWorldGenesis()
    {

    }


    bool LoadTemplateImages();


    Long GetZoneHeight(Long x, Long z, UShort & ZoneType)
    {
      Long SecPos_x, SecPos_z;

      SecPos_x = x >> ZVOXELBLOCSHIFT_X;
      SecPos_z = z >> ZVOXELBLOCSHIFT_Z;
      Long XSel = (((SecPos_x) >> 4) + 64);
      Long ZSel = (((SecPos_z) >> 4) + 64);

      if (   (XSel >= 0 && XSel < 128 )
          && (ZSel >= 0 && ZSel  < 128 ) )
      {
        ZoneType = ConvCN[ (ZMemSize)ZoneMap_New[ XSel ][ ZSel ] ];
        switch (ZoneType)
        {
          default:
          case 2: //GenerateZone_MathStruct_1( VoxelSector, x,y,z ); break;
          case 3: //GenerateZone_RandomStickSea( VoxelSector, x,y,z ); break;
          case 0: return(0); break;
          case 1:
          case 4: //GenerateZone_BlackWoods( VoxelSector, x,y,z ); break;
          case 5:
                  {
                    ULong sx = (SecPos_x << ZVOXELBLOCSHIFT_X) + (x & ZVOXELBLOCMASK_X);
                    ULong sz = (SecPos_z << ZVOXELBLOCSHIFT_Z) + (z & ZVOXELBLOCMASK_Z);
                    ULong rx = sx >> 4; double Coef1 = (sx % 16) * (1.0 / 16.0);
                    ULong rz = sz >> 4; double Coef2 = (sz % 16) * (1.0 / 16.0);

                    double P1 = (RandomGen.GetNumber(rx) + RandomGen.GetNumber(rz) ) % 30 ;
                    double P2 = (RandomGen.GetNumber(rx+1) + RandomGen.GetNumber(rz) ) % 30 ;
                    double P3 = (RandomGen.GetNumber(rx) + RandomGen.GetNumber(rz+1) ) % 30 ;
                    double P4 = (RandomGen.GetNumber(rx+1) + RandomGen.GetNumber(rz+1) ) % 30 ;

                    Long height = Interpolation_2d(P1,P2,P3,P4,Coef1,Coef2) - 15 + BlackWoods_Level;
                    return(height);
                  }
                  break;
          case 6:
                  {
                    Long height;
                    double P1,P2,P3,P4;

                    height = GetHeightMap(x , z) * 16;

                    ULong sx = (SecPos_x << ZVOXELBLOCSHIFT_X) + (x & ZVOXELBLOCMASK_X);
                    ULong sz = (SecPos_z << ZVOXELBLOCSHIFT_Z) + (z & ZVOXELBLOCMASK_Z);
                    ULong rx = sx >> 4; double Coef1 = (sx % 16) * (1.0 / 16.0);
                    ULong rz = sz >> 4; double Coef2 = (sz % 16) * (1.0 / 16.0);

                    P1 = (RandomGen.GetNumber(rx) + RandomGen.GetNumber(rz) ) % 30 ;
                    P2 = (RandomGen.GetNumber(rx+1) + RandomGen.GetNumber(rz) ) % 30 ;
                    P3 = (RandomGen.GetNumber(rx) + RandomGen.GetNumber(rz+1) ) % 30 ;
                    P4 = (RandomGen.GetNumber(rx+1) + RandomGen.GetNumber(rz+1) ) % 30 ;

                    Long height2 = Interpolation_2d(P1 + height,P2+height,P3+height,P4+height,Coef1,Coef2) - 15 ;

                    if (height < 16.0) height2*= height / 16.0;
                    height = height2;
                    return(height);
                  }
                  break;
          case 13:
                  {
                    return( Canva_3_1.GetPoint_Fast( 1023 - ((x + 512) & 1023), (z + 512) & 1023 ) );
                  }
                  break;

        }
      } else return(0);
      return(0);

    }


    void GenerateSector(ZVoxelSector * VoxelSector)
    {
      Long x,y,z, HeightOffset;
      UShort ZoneType, RngNum;


      x = VoxelSector->Pos_x;
      y = VoxelSector->Pos_y;
      z = VoxelSector->Pos_z;

      VoxelSector->Flag_Void_Regular=true;VoxelSector->Flag_Void_Transparent=true;

      Long XSel = (((x) >> 4) + 64 );
      Long ZSel = (((z) >> 4) + 64 );

      if (   (XSel >= 0 && XSel < 128 )
          && (ZSel >= 0 && ZSel  < 128 ) )
      {

        ZoneType = ConvCN[ (ZMemSize) ZoneMap_New[ XSel ][ ZSel ] ];
        VoxelSector->ZoneType = ZoneType;
        RngNum = ConvCN[(ZMemSize)RingNum[ XSel ][ ZSel ] ];
        VoxelSector->RingNum = RngNum;
        HeightOffset = ZoneYOffset[ RngNum ];
        VoxelSector->ZoneVersion = 1; // Set the default version num to 1.
        VoxelSector->GeneratorVersion = 1; // Generator version. Updated at each world change.

        if (y > 64) { GenerateZone_DustField( VoxelSector, x, y, z); return; }

        switch (ZoneType)
        {
          default:
          case 0: GenerateZone_Flat2( VoxelSector, HeightOffset, x,y,z );
                  GenerateRTFM(VoxelSector);
                  // GenerateBigTrees(VoxelSector);
                  break;
          case 1: GenerateZone_NaturalMountains( VoxelSector, x,y,z ); break;
          case 2: GenerateZone_MathStruct_1( VoxelSector, HeightOffset, x,y,z );
                  GenerateBigTrees(VoxelSector);
                  break;
          case 3: GenerateZone_RandomStickSea( VoxelSector, HeightOffset, x,y,z ); break;
          case 4: GenerateZone_BlackWoods( VoxelSector, x,y,z );
                  GenerateBigTrees(VoxelSector);
                  break;
          case 5: GenerateZone_BlackWoods( VoxelSector, x,y,z );
                  GenerateBigTrees(VoxelSector);
                  GenerateAddBombs(VoxelSector, 0.002, 200, true);
                  break;
          case 6: GenerateZone_BigMountainsV2( VoxelSector, HeightOffset, x,y,z);
                  //GenerateBigTrees(VoxelSector);
                  break;
          case 7: GenerateZone_LavaRiver( VoxelSector );
                  GenerateBigTrees(VoxelSector);
                  break;
          case 8: GenerateZone_Flat2( VoxelSector, HeightOffset, x,y,z );
                  GenerateBigTrees(VoxelSector);
                  break;
          case 9: GenerateZone_AcidHills( VoxelSector, HeightOffset, x,y,z );
                  break;
          case 10:GenerateZone_WaterLands( VoxelSector, HeightOffset, x,y,z );
                  // GenerateBigTrees(VoxelSector);
                  break;
          case 11:GenerateZone_WaterMountain( VoxelSector, HeightOffset,  x,y,z );
                  break;
          case 12:GenerateZone_FlatAcidResistant( VoxelSector, HeightOffset, x,y,z );
                  break;
          case 13:GenerateZone_Generic_MappedTerrain( VoxelSector, 10, &Canva_3_1, 512 ); // Main Terrain
                  // Generate_Generic_LittleStructs( VoxelSector, &T3dTemplate_2, &Canva_3_2, 512  );
                  Generate_Generic_PlaceVoxel( VoxelSector, 87, &Canva_3_3, 512,0, true); // Water Generator
                  Generate_Generic_PlaceVoxel( VoxelSector, 88, &Canva_3_2, 512,1, true); // Water Absorber
                  VoxelSector->Flag_NeedSortedRendering = true;
                  break;
          case 14:Generate_Terrain_1536( VoxelSector, HeightOffset, x,y,z );
                  AddTreeOn(VoxelSector, TreeTable, 3, 232);
                  //AddTreeOn(VoxelSector, &Template_Vegetation_1, 232);
                  break;
          case 15:Generate_Terrain_1537( VoxelSector, HeightOffset, x,y,z );
                  break;
          case 16:Generate_Terrain_1538( VoxelSector, HeightOffset, x,y,z );
                  AddTreeOn(VoxelSector, TreeTable, 3, 232);
                  break;

        }

      } else    { GenerateZone_WaterMountain( VoxelSector, -70, x,y,z );}

    }

    void GenerateZone_WaterMountain(ZVoxelSector * VoxelSector, Long HeightOffset, Long Sector_x, Long Sector_y, Long Sector_z);
    void GenerateZone_WaterLands(ZVoxelSector * VoxelSector, Long HeightOffset, Long Sector_x, Long Sector_y, Long Sector_z);

    void GenerateZone_AcidHills(ZVoxelSector * VoxelSector, Long HeightOffset, Long Sector_x, Long Sector_y, Long Sector_z);

    void GenerateZone_LavaRiver(ZVoxelSector * VoxelSector);
    void GenerateZone_BigMountains(ZVoxelSector * VoxelSector, Long HeightOffset, Long Sector_x, Long Sector_y, Long Sector_z);
    void GenerateZone_BigMountainsV2(ZVoxelSector * VoxelSector, Long HeightOffset, Long Sector_x, Long Sector_y, Long Sector_z);
    void GenerateZone_NaturalMountains(ZVoxelSector * VoxelSector, Long Sector_x, Long Sector_y, Long Sector_z);
    void GenerateZone_BlackWoods(ZVoxelSector * VoxelSector, Long Sector_x, Long Sector_y, Long Sector_z);
    void GenerateZone_PureSinusArea(ZVoxelSector * VoxelSector, Long Sector_x, Long Sector_y, Long Sector_z);
    void GenerateZone_CuttedSinusArea(ZVoxelSector * VoxelSector, Long Sector_x, Long Sector_y, Long Sector_z);

    void GenerateZone_MathStruct_1(ZVoxelSector * VoxelSector, Long HeightOffset, Long Sector_x, Long Sector_y, Long Sector_z);

    void GenerateZone_Mountains(ZVoxelSector * VoxelSector, Long Sector_x, Long Sector_y, Long Sector_z);
    void GenerateZone_RandomStickSea(ZVoxelSector * VoxelSector, Long HeightOffset, Long Sector_x, Long Sector_y, Long Sector_z);


    void GenerateZone_Flat(ZVoxelSector * VoxelSector, Long Sector_x, Long Sector_y, Long Sector_z);
    void GenerateZone_Flat2(ZVoxelSector * VoxelSector, Long HeightOffset, Long Sector_x, Long Sector_y, Long Sector_z);
    void GenerateZone_FlatAcidResistant(ZVoxelSector * VoxelSector, Long HeightOffset, Long Sector_x, Long Sector_y, Long Sector_z);

    void GenerateZone_DustField(ZVoxelSector * VoxelSector, Long Sector_x, Long Sector_y, Long Sector_z);

    void Generate_Terrain_1536(ZVoxelSector * VoxelSector, Long HeightOffset, Long Sector_x, Long Sector_y, Long Sector_z);
    void Generate_Terrain_1537(ZVoxelSector * VoxelSector, Long HeightOffset, Long Sector_x, Long Sector_y, Long Sector_z);
    void Generate_Terrain_1538(ZVoxelSector * VoxelSector, Long HeightOffset, Long Sector_x, Long Sector_y, Long Sector_z);
    void AddTreeOn(ZVoxelSector * Sector, ZVoxelSector * TemplateTable[], ULong ModelCount, UShort VoxelType);


    void GenerateBigTrees(ZVoxelSector * Sector);
    void GenerateAddBombs(ZVoxelSector * Sector, double Probability, UShort VoxelType, bool SetActiveSector);
    void GenerateRTFM(ZVoxelSector * Sector);

    // New generic generation functions
    void GenerateZone_Generic_MappedTerrain(ZVoxelSector * VoxelSector, UShort TerrainVoxelType, ZGenericCanva<UByte> * TerrainMap, Long Offset);
    void Generate_Generic_LittleStructs(ZVoxelSector * Sector, ZVoxelSector * ObjectToPlace, ZGenericCanva<UByte> * LocationMap, Long Offset );
    void Generate_Generic_PlaceVoxel(ZVoxelSector * Sector, UShort VoxelType, ZGenericCanva<UByte> * LocationMap, Long Offset, Long RelativeHeight,  bool SetActiveSector);





    static void Debug_Out_Zonemap();


    inline double Interpolation_Lissee(double P1, double P2, double coef)
    {
      double result, inter;
      inter = (1.0 - cos(3.141592654 * coef )) / 2.0;
      result = P1 * (1.0 - inter) + P2 * inter;
      return(result);
    }

    inline double Interpolation_2d(double P1, double P2, double P3, double P4, double Coef1, double Coef2)
    {
      double Plane1,Plane2;

      Plane1 = Interpolation_Lissee(P1, P2, Coef1);
      Plane2 = Interpolation_Lissee(P3, P4, Coef1);
      return Interpolation_Lissee(Plane1, Plane2, Coef2);
    }

    // Tricubic interpolation.
    inline double Interpolation_3d(double *P, double Coef1, double Coef2, double Coef3)
    {
      double Plane1,Plane2;

      Plane1 = Interpolation_2d(P[0], P[1],P[2],P[3], Coef1, Coef2);
      Plane2 = Interpolation_2d(P[4], P[5],P[6],P[7], Coef1, Coef2);
      return Interpolation_Lissee(Plane1, Plane2, Coef3);
    }

    inline double Interpolation_bilin_3d(double *P, double Coef1, double Coef2, double Coef3)
    {
      double i1,i2,i3,i4,i5,i6;
      double iCoef1, iCoef2,iCoef3;

      iCoef1 = 1.0 - Coef1;
      iCoef2 = 1.0 - Coef2;
      iCoef3 = 1.0 - Coef3;

      i1 = P[0]*iCoef1 + P[1]*Coef1;
      i2 = P[2]*iCoef1 + P[3]*Coef1;
      i3 = P[4]*iCoef1 + P[5]*Coef1;
      i4 = P[6]*iCoef1 + P[7]*Coef1;

      i5 = i1 * iCoef2 + i2 * Coef2;
      i6 = i3 * iCoef2 + i4 * Coef2;

      return( i5 * iCoef3 + i6 * Coef3);
    }

    // Zonemap Work
    void ZoneMap_ChangeSize(const char ** Table, Long TableWidth, Long TableHeight, const char * TableDecl, Long NewTableWidth, Long NewTableHeight, UByte EmptyZonesValue, const char * OutputFile);
    void ZoneMap_Shift(const char ** Table, Long TableWidth, Long TableHeight, const char * TableDecl, Long Ring, Long Shift, const char * OutputFile, UByte IncThreshold=255, UByte IncAdd=0);
  };

#endif /* Z_ZWORLDGENESIS_H */
