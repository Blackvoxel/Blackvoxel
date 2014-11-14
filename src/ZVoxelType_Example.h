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
 * ZVoxelType_Example.h
 *
 *  Created on: 11 nov. 2014
 *      Author: laurent
 */

#ifndef Z_ZVOXELTYPE_EXAMPLE_H
#define Z_ZVOXELTYPE_EXAMPLE_H

//#ifndef Z_ZVOXELTYPE_EXAMPLE_H
//#  include "ZVoxelType_Example.h"
//#endif

#ifndef Z_ZVOXELTYPE_H
#  include "ZVoxelType.h"
#endif

// Include your voxel extension class for storage if you have any.

#ifndef Z_ZVOXELEXTENSION_EXAMPLE_H
#  include "ZVoxelExtension_Example.h"
#endif

#ifndef Z_ZACTIVEVOXELINTERFACE_H
#  include "ZActiveVoxelInterface.h"
#endif

class ZVoxelType_Example : public ZVoxelType
{
  public:

    ZVoxelType_Example(ULong VoxelType) : ZVoxelType(VoxelType)
    {
      Is_VoxelExtension              = true;   // If there is a voxel extension
      Is_HasAllocatedMemoryExtension = true;   // If the voxel extension is a pointer to stored data.
      Is_Interface_PushBlock         = false;  // If it support
      Is_Interface_StoreBlock        = false;  //
      Is_Interface_PullBlock         = false;  // Interface Pullblock is
      Is_HasHelpingMessage           = false;  // If true, help message will be displayed when voxel is pointed.
      HelpingMessage = "Your help message";    // The message to display.
      Is_Active = true;
    }

    // Voxel Extensions

    virtual ZVoxelExtension * CreateVoxelExtension(bool IsLoadingPhase = true)
    {
      // If your VoxelType need storage, there is two way to do it.
      //
      // (1) You can use up to 32 bits of direct data storage in the voxel
      //     extension data (otherinfo) grid.
      // (2) You can create an extension class storing all your data.
      //     A pointer to the extension will be stored in the data
      //     (otherinfo) grid.

      // case (1) put your extension type name after new.
      // case (2) delete these lines.

      if (Is_HasAllocatedMemoryExtension)
      {
        return(new ZVoxelExtension_Example());
      }

      // case (1) dont do anything
      // case (2) put your initialisation value here.

      return(0);
    }


    virtual void  ActiveProcess( ZActiveVoxelInterface * AvData)
    {
      ZVector3L Loc, Loc2;
      VoxelLocation Location;

      Loc = Loc2 = AvData->Coords;
      Loc.x ++;
      Loc2.x --;

      AvData->MoveThis(&Loc, ZActiveVoxelInterface::CHANGE_IMPORTANT);
      //AvData->ExchangeVoxels(&Loc, &Loc2, ZActiveVoxelInterface::CHANGE_IMPORTANT);
      //AvData->MoveVoxel(&Loc, &Loc2, ZActiveVoxelInterface::CHANGE_IMPORTANT);
      //AvData->SetVoxel(&Loc, 49, ZActiveVoxelInterface::CHANGE_IMPORTANT);
      //AvData->SetVoxelExt(&Loc, 49, ZActiveVoxelInterface::CHANGE_IMPORTANT,&Location);
      //printf("%d\n",AvData->GetVoxel(&Loc));

    };

    //virtual void  DeleteVoxelExtension(ZMemSize VoxelExtension, bool IsUnloadingPhase = false)


    // The user click the activation button(Middle) on the voxel to activate special functions.
    //virtual void  UserAction_Activate(ZMemSize VoxelInfo, Long x, Long y, Long z);

    //virtual ULong  Interface_PushBlock_Push    ( VoxelLocation * DestLocation, UShort VoxelType, ULong Count );
    //virtual ULong  Interface_PushBlock_Pull    ( VoxelLocation * DestLocation,  UShort * VoxelType, ULong Count );
    //virtual ULong  Interface_PushBlock_PullTest( VoxelLocation * DestLocation,  UShort * VoxelType, ULong Count );

};

#endif /* Z_ZVOXELTYPE_EXAMPLE_H */
