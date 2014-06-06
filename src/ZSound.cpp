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
 * ZSounds.cpp
 *
 *  Created on: 3 mars 2011
 *      Author: laurent
 */

#include "ZSound.h"
#include <cmath>

ZSound::Sound * ZSound::PlayingSoundList;

Long SoundBuffer[65536];

void ZSound::mixaudio(void *unused, Uint8 *stream, int len)
{
  Sound * Snd, * OldSound;
  ZMemSize PlayLen,i,Buffer_Len, Buffer_RemainLen;

  Short * PlayPos;
  ULong nSounds;



  Buffer_Len = len;



  for (i=0;i<Buffer_Len;i++) SoundBuffer[i]=0;
  nSounds = 0;

  Snd = PlayingSoundList;
  while (Snd)
  {
    if (Snd->UseIntCompute)
    {
      Buffer_RemainLen = Buffer_Len;

      while (Buffer_RemainLen)
      {
        PlayLen = (Snd->RLen > Buffer_RemainLen) ? Buffer_RemainLen : Snd->RLen;
        PlayPos = (Short * )(Snd->Data + Snd->Pos);
        if (PlayLen == 0)
        {

          if(Snd->Repeat)
          {
            Snd->Pos = Snd->RepeatPos;
            Snd->RLen = Snd->Len - Snd->RepeatPos;
          }
          else
          {
            if (Snd->FlagOnEnd) *Snd->FlagOnEnd=true;
            Snd->DeletePending = true;
            break;
          }
        }
        else
        {
          for (i=0;i<PlayLen/2;i++) SoundBuffer[i]+= (Long)PlayPos[i];
          Buffer_RemainLen -= PlayLen;
          Snd->RLen -= PlayLen;
          Snd->Pos  += PlayLen;
        }
      }
      //SDL_MixAudio(stream, Snd->Data + Snd->Pos, PlayLen, SDL_MIX_MAXVOLUME);
      //for (i=0;i<PlayLen-2;i+=2) {((UShort *)stream)[i]=0x0;((UShort *)stream)[i+1]=0x8000;}
    }
    else
    {
      double  Coef, Temp;
      ZMemSize BufferPos;
      ZMemSize MaxOffset;
      Buffer_RemainLen = Buffer_Len >> 1;
      BufferPos = 0;
      MaxOffset = (Snd->Len >> 1) - 1;

      double S1,S2, Ir;
      ZMemSize Offset;



      while (Buffer_RemainLen && (!Snd->DeletePending))
      {
        Coef   = modf(Snd->DPos, &Temp);
        Offset = Temp;
        if (Offset < MaxOffset )
        {
          S1 = ((Short *)Snd->Data)[ Offset ];
          S2 = ((Short *)Snd->Data)[ Offset + 1 ];
          Ir = S1 * (1.0 - Coef) + (S2 * Coef);
          SoundBuffer[BufferPos++] += (Long)Ir;
          Buffer_RemainLen--;
          Snd->DPos += Snd->FrequencyVar;
        }
        else
        {
          if (Snd->Repeat)
          {
            Snd->DPos = Snd->DRepeatPos;
          }
          else
          {
            if (Snd->FlagOnEnd) *Snd->FlagOnEnd=true;
            Snd->DeletePending = true;
            break;
          }
        }
      }
    }

    OldSound = Snd;
    Snd = Snd->NextSound;

    // If sound must be deleted

    if (OldSound->DeletePending)
    {
      if (OldSound->PrevSound == 0) PlayingSoundList = OldSound->NextSound;
      else                          OldSound->PrevSound->NextSound = OldSound->NextSound;
      if (OldSound->NextSound != 0) OldSound->NextSound->PrevSound = OldSound->PrevSound;
      OldSound->NextSound = 0;
      OldSound->PrevSound = 0;
      delete OldSound;
    }


    nSounds++;
  }

  // if (nSounds) for (i=0;i<Buffer_Len/2;i++) ((Short *)stream) [i] = (Short)(SoundBuffer[i]/nSounds );
  if (nSounds) for (i=0;i<Buffer_Len/2;i++) ((Short *)stream) [i] = (Short)(SoundBuffer[i] );

}
