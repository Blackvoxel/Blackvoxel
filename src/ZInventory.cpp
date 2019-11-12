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


ZInventory::ZInventory()
{
  ULong i;

  SlotCount = Inventory_SlotCount;

  ActualItem = Inventory_StartSlot;
  ActualTool = Tools_StartSlot;

  SlotTable = new Entry[SlotCount];

  for (i=0;i<SlotCount;i++) { SlotTable[i].VoxelType = 0; SlotTable[i].Quantity = 0; }
}

ZInventory::ZInventory(ULong SlotCount)
{
  ULong i;

  ActualItem = 0;
  ActualTool = 0;

  this->SlotCount = SlotCount;
  SlotTable = new Entry[SlotCount];
  for (i=0;i<SlotCount;i++) { SlotTable[i].VoxelType = 0; SlotTable[i].Quantity = 0; }
}

ZInventory::~ZInventory()
{
  if (SlotTable) delete [] SlotTable;
  SlotTable = 0;
  SlotCount = 0;
}

ULong ZInventory::Entry::UnstoreBlock(ULong BlockQuantity)
{
  if (BlockQuantity > Quantity) BlockQuantity = Quantity;

  Quantity-=BlockQuantity;
  if (Quantity==0) VoxelType = 0;

  return(BlockQuantity);
}

ULong ZInventory::Entry::StoreBlock(UShort VoxelType_ToStore, ULong BlockQuantity)
{
  if (VoxelType == 0) {VoxelType = VoxelType_ToStore; Quantity = 0;}
  if (VoxelType_ToStore != VoxelType) return(0);

  Quantity+=BlockQuantity;

  return(BlockQuantity);
}

void ZInventory::Clear()
{
  ULong i;

  for(i=0;i<SlotCount;i++) { SlotTable[i].VoxelType = 0; SlotTable[i].Quantity = 0; }

  /*
  for ( i=Inventory_StartSlot ; i<=Inventory_EndSlot ; i++ ) { SlotTable[i].VoxelType = 0; SlotTable[i].Quantity = 0; }
  for ( i=Tools_StartSlot ; i<=Tools_EndSlot ; i++ )         { SlotTable[i].VoxelType = 0; SlotTable[i].Quantity = 0; }
  for ( i=Powers_StartSlot ; i<=Powers_EndSlot ; i++ )       { SlotTable[i].VoxelType = 0; SlotTable[i].Quantity = 0; }
  */
}

bool ZInventory::FindSlot(UShort VoxelType, ULong &Slot)
{
  ULong i;

  for(i=0;i<SlotCount;i++)
  {
    if (SlotTable[i].VoxelType == VoxelType && SlotTable[i].Quantity>0) { Slot = i; return(true); }
  }
  return(false);
}

bool ZInventory::FindFreeSlot(ULong &Slot)
{
  ULong i;

  for(i=0;i<SlotCount;i++)
  {
    if (SlotTable[i].Quantity == 0) {Slot = i; return(true);}
  }

  return(false);
}

void ZInventory::SetSlot(ULong SlotNum, UShort VoxelType, ULong Quantity)
{
  if (SlotNum >= this->SlotCount) return;

  SlotTable[SlotNum].VoxelType = VoxelType;
  SlotTable[SlotNum].Quantity  = Quantity;
}

bool ZInventory::StoreBlocks(UShort VoxelType, ULong VoxelQuantity)
{
  ULong Slot;

  VoxelType = GetGroupLeader(VoxelType);

  if (FindSlot(VoxelType,Slot))
  {
    SlotTable[Slot].Quantity += VoxelQuantity;
    return(true);
  }
  else if (FindFreeSlot(Slot))
  {
    SlotTable[Slot].VoxelType = VoxelType;
    SlotTable[Slot].Quantity  = VoxelQuantity;
    return(true);
  }

  return(false);
}

ULong ZInventory::UnstoreBlocks(UShort VoxelType, ULong VoxelQuantity)
{
  ULong i, UnstoredCount, Quantity;
  Entry * Slot;

  UnstoredCount = 0;

  for (i=0; i <= SlotCount; i++)
  {
    Slot = &SlotTable[i];
    if ( Slot->VoxelType == VoxelType)
    {
      Quantity = (Slot->Quantity >= VoxelQuantity) ? VoxelQuantity : Slot->Quantity;
      UnstoredCount += Quantity;
      Slot->Quantity -= Quantity;
      if (Slot->Quantity == 0) Slot->VoxelType = 0;
    }
    if (UnstoredCount == VoxelQuantity) return( UnstoredCount);
  }
  return(UnstoredCount);
}

ULong ZInventory::UnstoreBlocks_FromSlot(ULong SlotNum, ULong VoxelQuantity)
{
  Entry * Slot;

  if (SlotNum >= this->SlotCount) return(0);

  Slot = &SlotTable[SlotNum];

  if (VoxelQuantity > Slot->Quantity) VoxelQuantity = Slot->Quantity;
  Slot->Quantity -= VoxelQuantity;
  if (Slot->Quantity == 0) Slot->VoxelType = 0;

  return(VoxelQuantity);
}


ULong ZInventory::GetNextUsedItemSlotNum( ULong SlotNum )
{
  ULong i;

  if (SlotNum == (ULong)-1) return((ULong)-1);

  i = SlotNum;
  while ( i != SlotNum )
  {
    i++;
    if (i > Inventory_EndSlot) i = Inventory_StartSlot;
    if (SlotTable[i].VoxelType > 0 && SlotTable[i].Quantity > 0) return(i);
  }
  return(-1);
}

ULong ZInventory::GetPreviousUsedItemSlotNum( ULong SlotNum )
{
  Long i;

  if (SlotNum == (ULong)-1) return((ULong)-1);
  i = (Long)SlotNum ;
  while (--i != (Long)SlotNum)
  {
    if (i < Inventory_StartSlot) i = Inventory_EndSlot;
    if (SlotTable[i].VoxelType > 0 && SlotTable[i].Quantity > 0) return((ULong)i);
  }
  return(-1);
}

void ZInventory::NextTool()
{
  ULong i, Index;
  Entry Buffer[Tools_EndSlot - Tools_StartSlot + 1];

  // Search for next tool availlable.

  for (i=Tools_StartSlot+1;i<= Tools_EndSlot;i++)
  {
    if (SlotTable[i].VoxelType > 0 && SlotTable[i].Quantity > 0) {break;}
  }
  if (i>Tools_EndSlot) return;

  Index = i;

  // Copy in new order

  for (i=0;i <= (Tools_EndSlot - Tools_StartSlot); i++)
  {
    Buffer[i] = SlotTable[Index];
    Index++;
    if (Index > Tools_EndSlot) Index = Tools_StartSlot;
  }

  // Copy back into inventory.

  for (i=0;i <= (Tools_EndSlot - Tools_StartSlot); i++)
  {
    SlotTable[Tools_StartSlot + i ] = Buffer[i];
  }

}

void ZInventory::PrevTool()
{
  Long i, Index;
  Entry Buffer[Tools_EndSlot - Tools_StartSlot + 1];

  // Search for next tool availlable.

  for (i=Tools_EndSlot;i>=Tools_StartSlot ;i--)
  {
    if (SlotTable[i].VoxelType > 0 && SlotTable[i].Quantity > 0) {break;}
  }
  if (i<Tools_StartSlot) return;

  Index = i;

  // Copy in new order

  for (i=0;i <= (Tools_EndSlot - Tools_StartSlot); i++)
  {
    Buffer[i] = SlotTable[Index];
    Index++;
    if (Index > Tools_EndSlot) Index = Tools_StartSlot;
  }

  // Copy back into inventory.

  for (i=0;i <= (Tools_EndSlot - Tools_StartSlot); i++)
  {
    SlotTable[Tools_StartSlot + i ] = Buffer[i];
  }

}

UShort ZInventory::GetGroupLeader(UShort VoxelType)
{
  UShort NewVoxelType;

  switch(VoxelType)
  {
    default: NewVoxelType = VoxelType; break;
    case 103:
    case 104:
    case 105:
    case 106: NewVoxelType = 103; break;
    case 244:
    case 245:
    case 246:
    case 247: NewVoxelType = 244; break;
    case 256:
    case 257:
    case 258:
    case 259: NewVoxelType = 256; break;
  }
  return(NewVoxelType);
}

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


