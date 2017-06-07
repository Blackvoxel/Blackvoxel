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

    enum { Inventory_SlotCount = 60,
           Inventory_StartSlot = 0,
           Inventory_EndSlot   = 39,
           Tools_StartSlot     = 40,
           Tools_EndSlot       = 49,
           Powers_StartSlot    = 50,
           Powers_EndSlot       = 59};

    ZInventory()
    {
      ULong i;

      SlotCount = Inventory_SlotCount;

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

    // Some family of voxel will be regrouped to a single type when stored.

    UShort GetGroupLeader(UShort VoxelType)
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

    void NextTool()
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

    void PrevTool()
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

};


#endif /* Z_ZINVENTORY_H */
