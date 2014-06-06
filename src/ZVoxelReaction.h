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
#ifndef Z_ZVOXELREACTION_H
#define Z_ZVOXELREACTION_H

//#ifndef Z_ZVOXELREACTION_H
//#  include "ZVoxelReaction.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif



class ZVoxelReaction
{
  public:
    struct ZReaction
    {
      UShort ActorTrans;
      UShort DrivedTrans;
    };

    ZReaction * TransformationTable;

    void Set_Default(UShort ActorTrans, UShort DrivedTrans)
    {
      ULong i;

      for (i=0;i<65536;i++) { TransformationTable[i].ActorTrans = ActorTrans; TransformationTable[i].DrivedTrans = DrivedTrans; }

    }

    void SetReaction(UShort VoxelType, UShort ActorTrans, UShort DrivedTrans) { TransformationTable[VoxelType].ActorTrans = ActorTrans; TransformationTable[VoxelType].DrivedTrans = DrivedTrans; }



    ZVoxelReaction(UShort ActorTrans, UShort DrivedTrans)
    {
      TransformationTable = new ZReaction[65536];

      Set_Default(ActorTrans, DrivedTrans);
    }

    inline ZReaction * GetReaction(UShort VoxelType) { return( &TransformationTable[VoxelType] ); }

    ZVoxelReaction()
    {
      TransformationTable = new ZReaction[65536];
    }

    ~ZVoxelReaction()
    {
      if (TransformationTable) { delete [] TransformationTable; TransformationTable = 0; }
    }

};

#endif /* ZVOXELREACTION_H_ */
