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

class ZInventory
{
  public:

   class Entry
   {
     public:
       UShort VoxelType;
       ULong  Quantity;
   };

  protected:
    ULong SlotCount;
    Entry * SlotTable;

    ULong ActualItem;
    ULong ActualTool;

  public:

    enum { Inventory_StartSlot = 0,
           Inventory_EndSlot   = 39,
           Tools_StartSlot     = 40,
           Tools_EndSlot       = 49,
           Powers_StartSlot    = 50,
           Powers_EndSlot       = 59 };

    ZInventory()
    {
      ULong i;

      SlotCount = 60;

      ActualItem = Inventory_StartSlot;
      ActualTool = Tools_StartSlot;

      SlotTable = new Entry[SlotCount];

      for (i=0;i<SlotCount;i++) { SlotTable[i].VoxelType = 0; SlotTable[i].Quantity = 0; }
    }

    void Clear()
    {
      ULong i;

      for ( i=Inventory_StartSlot ; i<=Inventory_EndSlot ; i++ ) { SlotTable[i].VoxelType = 0; SlotTable[i].Quantity = 0; }
      for ( i=Tools_StartSlot ; i<=Tools_EndSlot ; i++ )         { SlotTable[i].VoxelType = 0; SlotTable[i].Quantity = 0; }
      for ( i=Powers_StartSlot ; i<=Powers_EndSlot ; i++ )       { SlotTable[i].VoxelType = 0; SlotTable[i].Quantity = 0; }
    }

    void SetSlot(ULong SlotNum, UShort Type, ULong Quantity)
    {
      if (SlotNum >= this->SlotCount) return;

      SlotTable[SlotNum].VoxelType = Type;
      SlotTable[SlotNum].Quantity  = Quantity;
    }

    Entry * GetSlotRef(ULong SlotNum) { return(&SlotTable[SlotNum]); }

    Entry * GetActualItemSlot()       { return(&SlotTable[ActualItem]); }
    Entry * GetActualToolSlot()       { return(&SlotTable[ActualTool]); }

    ZInventory(ULong SlotCount)
    {
      ULong i;

      ActualItem = 0;
      ActualTool = 0;

      this->SlotCount = SlotCount;
      SlotTable = new Entry[SlotCount];
      for (i=0;i<SlotCount;i++) { SlotTable[i].VoxelType = 0; SlotTable[i].Quantity = 0; }
    }

    ~ZInventory()
    {
      if (SlotTable) delete [] SlotTable;
      SlotCount = 0;
    }

    bool Save(ZStream_SpecialRamStream * Stream);
    bool Load(ZStream_SpecialRamStream * Stream);

    bool FindSlot(UShort VoxelType, ULong &Slot)
    {
      ULong i;

      for(i=0;i<SlotCount;i++)
      {
        if (SlotTable[i].VoxelType == VoxelType && SlotTable[i].Quantity>0) { Slot = i; return(true); }
      }
      return(false);
    }

    bool FindFreeSlot(ULong &Slot)
    {
      ULong i;

      for(i=0;i<SlotCount;i++)
      {
        if (SlotTable[i].Quantity == 0) {Slot = i; return(true);}
      }

      return(false);
    }

    bool StoreBlocks(UShort VoxelType, ULong VoxelQuantity)
    {
      ULong Slot;

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

    ULong UnstoreBlocks(UShort VoxelType, ULong VoxelQuantity)
    {
      ULong i, UnstoredCount, Quantity;
      Entry * Slot;

      UnstoredCount = 0;
      for (i= Inventory_StartSlot; i<=Inventory_EndSlot; i++)
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

    void Select_NextItem()     {ActualItem ++; if (ActualItem >Inventory_EndSlot) ActualItem = Inventory_StartSlot;}
    void Select_PreviousItem() {if (ActualItem == Inventory_StartSlot) ActualItem = Inventory_EndSlot; else ActualItem--;}
    ULong GetActualItemSlotNum() { return( ActualItem ); }
    void  SetActualItemSlotNum( ULong SlotNum ) { ActualItem = SlotNum; }
    ULong GetActualToolSlotNum() { return( ActualTool ); }

    ULong GetNextUsedItemSlotNum( ULong SlotNum )
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

    ULong GetPreviousUsedItemSlotNum( ULong SlotNum )
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

};


#endif /* Z_ZINVENTORY_H */
