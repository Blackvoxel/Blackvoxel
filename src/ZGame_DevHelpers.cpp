/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2015 Laurent Thiebaut & Olivia Merle
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
 * ZGame_DevHelpers.cpp
 *
 *  Created on: 6 janv. 2015
 *      Author: laurent
 */

#ifndef Z_ZGAME_DEVHELPERS_H
#  include "ZGame_DevHelpers.h"
#endif

#ifndef Z_ZVCPU_H
#  include "ZVCPU.h"
#endif

#ifndef Z_ZWORLDGENESIS_H
#  include "ZWorldGenesis.h"
#endif

#ifndef Z_BITMAP_IMAGE_H
#  include "z/ZBitmapImage.h"
#endif

bool ZGame_DevHelpers::OutputZoneImage(ZString & HelperDirectory)
{
  int x,y;
  char v;
  ZBitmapImage Bmp;
  ZString FileName;

  FileName = HelperDirectory;
  FileName.AddToPath("ZoneMap.bmp");

  Bmp.CreateBitmap(128,128,32);
  Bmp.Clear();

  for (y=0;y<Z_GENESISMAP_SIZE;y++)
    for (x=0;x<Z_GENESISMAP_SIZE;x++)
    {
      v = ZWorldGenesis::ZoneMap_New[x][y];
      if (v>='0' && v<='9') v=v-'0';
      if (v>='A' && v<='Z') v=v-'A'+10;
      Bmp.SetPixel(x,y,v*16,v*16,v*16,0);
    }

  Bmp.SaveBMP(FileName.String);

  return(true);
}

bool ZGame_DevHelpers::OutputZoneHeight(ZString & HelperDirectory)
{
  int x,y;
  char v;

  ZBitmapImage Bmp;
  ZString FileName;

  FileName = HelperDirectory;
  FileName.AddToPath("ZoneHeight.bmp");

  Bmp.CreateBitmap(128,128,32);
  Bmp.Clear();

  for (y=0;y<Z_GENESISMAP_SIZE;y++)
    for (x=0;x<Z_GENESISMAP_SIZE;x++)
    {
      v = ZWorldGenesis::HeightMap_New[x][y];
      if (v>='0' && v<='9') v=v-'0';
      if (v>='A' && v<='Z') v=v-'A'+10;
      Bmp.SetPixel(x,y,v*16,v*16,v*16,0);
    }

  Bmp.SaveBMP(FileName.String);

  return(true);
}



bool ZGame_DevHelpers::OutputHelpers(ZString & HelperDirectory)
{
  ZString Out, FileName;

  // Output BlackCPU V1 Opcode table

  FileName = HelperDirectory;
  FileName.AddToPath("BlackCPU_V1_OpcodeList.txt");
  Out.Clear();
  Out = BlackCPU<int>::OutputOpcodeDatabase(0);
  if (!Out.SaveToFile(FileName.String)) return(false);

  // Output BlackCPU V1 Opcode table (SQL version);

  FileName = HelperDirectory;
  FileName.AddToPath("BlackCPU_V1_OpcodeList.sql");
  Out.Clear();
  Out = BlackCPU<int>::OutputOpcodeDatabase(1);
  if (!Out.SaveToFile(FileName.String)) return(false);

  // Output BlackCPU V1 Opcode table (Compact html version)

  FileName = HelperDirectory;
  FileName.AddToPath("BlackCPU_V1_OpcodeListCompact.htm");
  Out.Clear();
  Out = BlackCPU<int>::OutputOpcodeDatabase(2);
  if (!Out.SaveToFile(FileName.String)) return(false);

  // Output Zone Map

  if(!OutputZoneImage(HelperDirectory)) return(false);

  // Output Zone Height

  if(!OutputZoneHeight(HelperDirectory)) return(false);



  return(true);
}
