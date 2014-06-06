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
 * ZInventory.cpp
 *
 *  Created on: 18 juin 2011
 *      Author: laurent
 */

#include "ZInventory.h"
#include <stdio.h>


bool ZInventory::Save(ZStream_SpecialRamStream * Stream)
{
  /*
  FILE * fh;

  fh = fopen(FileName,"wb");  if (!fh) return(false);

  if (1!=fwrite(SlotTable, sizeof(Entry) * SlotCount, 1, fh)) {fclose(fh); return(false);}

  fclose (fh);
*/

  Stream->PutZone(SlotTable, sizeof(Entry) * SlotCount);



  return(true);
}

bool ZInventory::Load(ZStream_SpecialRamStream * Stream)
{
  /*
  FILE * fh;

  fh = fopen(FileName,"rb");  if (!fh) return(false);

  if (1!=fread(SlotTable, sizeof(Entry) * SlotCount, 1, fh)) {fclose(fh); return(false);}

  fclose (fh);
*/
  return(Stream->GetZone(SlotTable,sizeof(Entry) * SlotCount));
}


