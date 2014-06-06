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
 * ZEgmyWaveManager.h
 *
 *  Created on: 21 sept. 2013
 *      Author: laurent
 */

#ifndef Z_ZEGMYTARGETMANAGER_H
#define Z_ZEGMYTARGETMANAGER_H

//#ifndef Z_ZEGMYTARGETMANAGER_H
//#  include "ZEgmyTargetManager.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

# define ZEGMYMANAGER_LISTCOUNT 1024


class ZEgmyTargetManager
{
  public:
    struct ObjectiveEntry
    {
      ZVector3L Location;
      UShort    VoxelType;
    };

    class EntryList
    {
      public:
        ULong       EntryCount;
        ObjectiveEntry ObjectiveList[ZEGMYMANAGER_LISTCOUNT];
        EntryList()
        {
          EntryCount = 0;
        }

        inline void AddObjective(ZVector3L * Objective, UShort VoxelType)
        {
          if (EntryCount >= ZEGMYMANAGER_LISTCOUNT) return;
          ObjectiveList[EntryCount].Location = *Objective;
          ObjectiveList[EntryCount].VoxelType = VoxelType;
          EntryCount++;
        }

        inline bool GetRandomObjective(ULong RandomNumber, ObjectiveEntry * &OutObjectiveEntry)
        {
          if (EntryCount == 0) return(false);
          OutObjectiveEntry = &ObjectiveList[RandomNumber % EntryCount];
          return(true);
        }

        inline void Clear()
        {
          EntryCount = 0;
        }

    };

  protected:

    EntryList * Back_WorkedList;     // The list worked on for registering Objectives.
    EntryList * Front_SupplyingList; // The list used to supply objectives on the actual stage.

  public:

    ZEgmyTargetManager()
    {
      Back_WorkedList = new EntryList;
      Front_SupplyingList = new EntryList;
    }

    // Report an objective so the Egmy's will look at it.
    void ReportObjective(ZVector3L * Objective, UShort VoxelType)
    {
      Back_WorkedList->AddObjective(Objective, VoxelType);
    }

    bool GetRandomObjective(ULong RandomNumber, ObjectiveEntry * &OutObjectiveEntry)
    {
      return (Front_SupplyingList->GetRandomObjective(RandomNumber,OutObjectiveEntry ));
    }

    void SwapList()
    {
      EntryList * Tmp;

      Tmp = Back_WorkedList;
      Back_WorkedList = Front_SupplyingList;
      Front_SupplyingList = Tmp;
      Back_WorkedList->Clear();
    }

    //

    void SetGameTime(ULong GameTime); // The gametime for getting absolute schedulling.






};


#endif /* Z_ZEGMYTARGETMANAGER_H */
