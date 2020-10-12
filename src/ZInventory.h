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
 * ZInventory.h
 *
 *  Created on: 18 juin 2011
 *      Author: laurent
 */

#ifndef Z_ZINVENTORY_H
#define Z_ZINVENTORY_H

//#ifndef Z_ZINVENTORY_H
//#  include "ZInventory.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZSTREAM_SPECIALRAMSTREAM_H
#  include "z/ZStream_SpecialRamStream.h"
#endif

class ZVoxelTypeManager;

class ZInventory
{
  public:

   class Entry
   {
     public:
       UShort VoxelType;
       ULong  Quantity;

       ULong UnstoreBlock(ULong BlockQuantity);
       ULong StoreBlock(UShort VoxelType_ToStore, ULong BlockQuantity);
   };

  protected:
    ULong SlotCount;
    Entry * SlotTable;

    ULong ActualItem;
    ULong ActualTool;

  public:

    enum { Inventory_SlotCount = 60,
           Inventory_StartSlot = 0,
           Inventory_EndSlot   = 39,
           Tools_StartSlot     = 40,
           Tools_EndSlot       = 49,
           Powers_StartSlot    = 50,
           Powers_EndSlot       = 59};

    ZInventory();                  // Create inventory with default size (for player)
    ZInventory(ULong SlotCount);   // Create inventory with special size (for machines)
    ~ZInventory();

    // Inventory working

    void   Clear();
    void   SetSlot(ULong SlotNum, UShort VoxelType, ULong Quantity);
    bool   StoreBlocks(UShort VoxelType, ULong VoxelQuantity);
    ULong  UnstoreBlocks(UShort VoxelType, ULong VoxelQuantity);
    ULong  UnstoreBlocks_FromSlot(ULong SlotNum, ULong VoxelQuantity);
    ULong  GetQuantity(ULong SlotNum) { return(SlotTable[SlotNum].Quantity); }
    UShort GetVoxelType(ULong SlotNum){ return(SlotTable[SlotNum].VoxelType); }
    // Find slots

    bool   FindSlot(UShort VoxelType, ULong &Slot); // Find slot storing some VoxelType
    bool   FindFreeSlot(ULong &Slot);               // Find a "free" slot with nothing in it

    // Get References on slot entries.

    Entry * GetSlotRef(ULong SlotNum) { return(&SlotTable[SlotNum]); }
    Entry * GetActualItemSlot()       { return(&SlotTable[ActualItem]); }
    Entry * GetActualToolSlot()       { return(&SlotTable[ActualTool]); }

    // "Current" used slot functions.
    ULong GetActualItemSlotNum()    { return( ActualItem ); }
    void  SetActualItemSlotNum( ULong SlotNum ) { ActualItem = SlotNum; }
    void  Select_NextItem()          {ActualItem ++; if (ActualItem >Inventory_EndSlot) ActualItem = Inventory_StartSlot;}
    void  Select_PreviousItem()      {if (ActualItem == Inventory_StartSlot) ActualItem = Inventory_EndSlot; else ActualItem--;}
    ULong GetNextUsedItemSlotNum( ULong SlotNum );
    ULong GetPreviousUsedItemSlotNum( ULong SlotNum );

    // "Current" used tool.
    ULong GetActualToolSlotNum() { return( ActualTool ); }
    void  NextTool();
    void  PrevTool();

    // Save inventory

    bool Save(ZStream_SpecialRamStream * Stream);
    bool Load(ZStream_SpecialRamStream * Stream);

    // Some family of voxel will be regrouped to a single type when stored.

    UShort GetGroupLeader(UShort VoxelType);

    // Computations

    double GetWeight(ZVoxelTypeManager *  VoxelTypeManager);


};


#endif /* Z_ZINVENTORY_H */
