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
 * ZGameEventSequencer.h
 *
 *  Created on: 26 sept. 2013
 *      Author: laurent
 */

#ifndef Z_ZGAMEEVENTSEQUENCER_H
#define Z_ZGAMEEVENTSEQUENCER_H

//#ifndef Z_ZGAMEEVENTSEQUENCER_H
//#  include "ZGameEventSequencer.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

#ifndef Z_ZOBJECT_H
#  include "ZObject.h"
#endif

#ifndef Z_ZOBJECT_LISTS_H
#  include "ZObject_Lists.h"
#endif

#define ZGAMEEVENTSEQUENCER_SLOTCOUNT 128

class ZGameEventSequencer
{
  protected:
    ELong GameTime;

    class Event : public ZObject
    {
      public:
        ELong StartTime;
        ELong EndTime;
        ELong InitialDelay;
        ELong RepeatDelay;
        ELong Duration;
        ULong SlotNum;
        bool  AutoRepeat;
    };

    struct Slot
    {
      bool Triggered;
      bool EventOn;
      ZListItem * Item;
    };

    ZList EventList;
    Slot SlotTable[ZGAMEEVENTSEQUENCER_SLOTCOUNT];
  public:
    ZGameEventSequencer()
    {
      ULong i;

      GameTime = 0;
      for (i=0;i<ZGAMEEVENTSEQUENCER_SLOTCOUNT;i++) { SlotTable[i].EventOn = false; SlotTable[i].Triggered = false; }
    }

    void SetGameTime(ELong GameTime) { this->GameTime = GameTime; }

    void * AddEvent(ELong InHowMuchTime, ELong Duration, ULong SlotToSet, bool AutoRepeat=false, ELong RepeatDelay=0)
    {
      Event * Evn;

      Evn = new Event;
      Evn->SlotNum = SlotToSet;
      Evn->StartTime = GameTime + InHowMuchTime;
      Evn->EndTime = Evn->StartTime + Duration;
      Evn->InitialDelay = InHowMuchTime;
      Evn->Duration = Duration;
      Evn->AutoRepeat = AutoRepeat;
      if (RepeatDelay) Evn->RepeatDelay = RepeatDelay;
      else             Evn->RepeatDelay = Duration;
      SlotTable[SlotToSet].EventOn = true;
      SlotTable[SlotToSet].Triggered = false;
      return(SlotTable[SlotToSet].Item = EventList.AddToTail(Evn));
    }

    bool ProcessEvents(ELong TotalElapsedGameTime)
    {
      ZListItem * Item, * NextItem;
      Event * Evn;

      GameTime = TotalElapsedGameTime / 1000;
      // printf("GameTime : %ld\n",GameTime);
      Item = EventList.GetFirst();
      while (Item)
      {
        NextItem = EventList.GetNext(Item);
        Evn = (Event *)Item->GetObject();
        if (GameTime > Evn->StartTime)
        {
          if (!SlotTable[Evn->SlotNum].Triggered) printf("Start Event : %d at : %ld\n",Evn->SlotNum, (long int)GameTime);
          SlotTable[Evn->SlotNum].Triggered = true;
        }
        if (GameTime > Evn->EndTime)
        {
          printf("End Event : %d at : %ld\n",Evn->SlotNum, (long int)GameTime);
          SlotTable[Evn->SlotNum].Triggered = false;

          // If event is auto restartable, restart it. Else kill it.
          if (Evn->AutoRepeat)
          {
            Evn->StartTime = GameTime + Evn->RepeatDelay;
            Evn->EndTime = Evn->StartTime + Evn->Duration;
          }
          else
          {
            SlotTable[Evn->SlotNum].EventOn = false;
            EventList.Remove(Item);
          }
        }
        Item = NextItem;
      }
      return(true);
    }


    bool SlotIsActivated(ULong SlotNum) { return( SlotTable[SlotNum].Triggered ); }
    bool SlotIsEventAttached(ULong SlotNum) { return( SlotTable[SlotNum].EventOn ); }

};


#endif /* Z_ZGAMEEVENTSEQUENCER_H */
