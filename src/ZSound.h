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
 * ZSound.h
 *
 *  Created on: 2 mars 2011
 *      Author: laurent
 */

#ifndef Z_ZSOUND_H
#define Z_ZSOUND_H

// #ifndef Z_ZSOUND_H
// #  include "ZSound.h"
// #endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif

#ifndef _SDL_audio_h
#  include <SDL/SDL_audio.h>
#endif

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif

#define ZSOUND_MAX_SOUNDFILES 512

class ZSound
{
  protected:

    struct SoundFile
    {
      bool Used;
      UByte * SoundData;
      ULong SoundLen;
    };

    SoundFile SoundBank[ZSOUND_MAX_SOUNDFILES];
    ULong SampleCount;

    struct Sound
    {
      Sound * NextSound;
      Sound * PrevSound;

      ULong SoundNum;
      UByte * Data;
      ZMemSize Pos;
      ZMemSize Len;
      ZMemSize RLen;
      ULong Volume;
      ULong Calls;
      // New
      bool DeletePending;
      bool Repeat;
      ZMemSize RepeatPos;
      double   DRepeatPos;
      bool * FlagOnEnd;
      bool   UseIntCompute;
      double FrequencyVar;
      double DPos;
    };

    static Sound * PlayingSoundList;

    bool SoundActivated;


    static void mixaudio(void *unused, Uint8 *stream, int len);




    Sound * FindSound(void * SoundHandle)
    {
      Sound * Snd;
      Snd = PlayingSoundList;
      while (Snd)
      {
        if (Snd == SoundHandle)
        {
          return(Snd);
        }

        Snd = Snd->NextSound;
      }

      return(0);
    }

  public:

    ZSound()
    {
      ULong i;
      SampleCount = 0;
      SoundActivated = true; PlayingSoundList =0;
      for (i=0;i<ZSOUND_MAX_SOUNDFILES;i++) {SoundBank[i].Used = false; SoundBank[i].SoundData = 0; SoundBank[i].SoundLen =0;}
    }
    ~ZSound()
    {
      ULong i;

      for (i=0;i<ZSOUND_MAX_SOUNDFILES;i++)
      {
        if (SoundBank[i].Used)
        {
          delete [] SoundBank[i].SoundData;
          SoundBank[i].Used = false;
          SoundBank[i].SoundData = 0;
          SoundBank[i].SoundLen =0;
        }
      }
      SampleCount = 0;

    }

    ULong GetSampleCount() { return SampleCount; }

    void Activate(bool ActivateSound)
    {
      SoundActivated = ActivateSound;
    }

    void SampleModify_Volume(ULong SoundNum, double NewVolume)
    {
      ULong i, Len;
      Short * Samples;
      register double Dat;

      if (!SoundBank[SoundNum].Used) return;
      Len = SoundBank[SoundNum].SoundLen >> 1;
      Samples = (Short *)SoundBank[SoundNum].SoundData;

      for (i=0;i<Len;i++)
      {
        Dat = (double)*Samples;
        Dat*= NewVolume;
        *Samples= (UShort)Dat;
        Samples ++;
      }

    }

    Bool LoadSoundFiles()
    {
      SDL_AudioSpec FormatSpec;
      SDL_AudioCVT cvt;
      SDL_AudioSpec SndSpec;
      UByte * SndData;
      Uint32   DataLen;
      // char FileName[4096];
      ZString FileSpec, FileName;

      ULong i;



      /* Un son sereo de 16 bits à 44.1kHz */
      FormatSpec.freq = 44100;
      FormatSpec.format = AUDIO_S16;
      FormatSpec.channels = 1;
      FormatSpec.samples = 1024; /* 512; */       /*Une bonne valeur pour les jeux */
      FormatSpec.callback = mixaudio;
      FormatSpec.userdata = NULL;

      /* Ouvre le contexte audio et joue le son */
      if ( SDL_OpenAudio(&FormatSpec, NULL) < 0 ) { fprintf(stderr, "Impossible d'accéder à l'audio: %s\n", SDL_GetError()); return(false); }


      SDL_PauseAudio(0);



      for(i=0;i<ZSOUND_MAX_SOUNDFILES;i++)
      {
        memset(&FormatSpec,0, sizeof(SDL_AudioSpec));
        memset(&cvt,0, sizeof(SDL_AudioCVT));
        memset(&SndSpec,0, sizeof(SDL_AudioSpec));


        FileName.Clear();
        FileName << i << ".wav";
        FileSpec = COMPILEOPTION_DATAFILESPATH;
        FileSpec.AddToPath("Sound").AddToPath(FileName);
        // sprintf(FileName, "Sound/%lu.wav",i);

        if ( SDL_LoadWAV(FileSpec.String, &SndSpec, &SndData, &DataLen) == NULL ) break;

        SDL_BuildAudioCVT(&cvt, SndSpec.format, SndSpec.channels, SndSpec.freq, AUDIO_S16, 1, 44100);

        cvt.len = DataLen;
        cvt.buf = new UByte[DataLen*cvt.len_mult];

        memcpy(cvt.buf, SndData , DataLen );

        SDL_ConvertAudio(&cvt);

        SoundBank[i].Used = true;
        SoundBank[i].SoundData = cvt.buf;
        SoundBank[i].SoundLen  = cvt.len_cvt;

        SDL_FreeWAV(SndData);
        SampleCount ++;
      }
      return(true);
    }

    void PlaySound(ULong SoundNum)
    {
      Sound * Snd;

      if (!SoundActivated) return;
      // printf("Sound:%d\n",SoundNum);

      SDL_LockAudio();
        Snd = new Sound;
        Snd->DeletePending = false;
        Snd->Data = SoundBank[SoundNum].SoundData;
        Snd->Pos = 0;
        Snd->Len = SoundBank[SoundNum].SoundLen;
        Snd->RLen = Snd->Len;
        Snd->Volume = 100;
        Snd->Calls = 0;
        Snd->Repeat = false;
        Snd->RepeatPos = 0;
        Snd->FlagOnEnd = 0;
        Snd->UseIntCompute = true;
        Snd->FrequencyVar = 1.0;
        Snd->DPos = 0.0;
        Snd->DRepeatPos = 0.0;
        if (PlayingSoundList == 0) { Snd->NextSound = 0; Snd->PrevSound = 0; PlayingSoundList = Snd; }
        else                       { Snd->NextSound = PlayingSoundList; Snd->PrevSound = 0; PlayingSoundList->PrevSound = Snd; PlayingSoundList = Snd; }
        // printf("Start Sound :%lx\n",(ZMemSize)Snd);

      SDL_UnlockAudio();
    }

    void * Start_PlaySound(ULong SoundNum, bool Repeat = false, bool UseIntFastMath = true, double Frequency = 1.0, bool * FlagToSetAtSoundEnd=0)
    {
      Sound * Snd;

      if (!SoundActivated) return(0);
      // printf("Sound:%d\n",SoundNum);

      SDL_LockAudio();
        Snd = new Sound;
        Snd->DeletePending = false;
        Snd->Data = SoundBank[SoundNum].SoundData;
        Snd->Pos = 0;
        Snd->Len = SoundBank[SoundNum].SoundLen;
        Snd->RLen = Snd->Len;
        Snd->Volume = 100;
        Snd->Calls = 0;
        Snd->Repeat = Repeat;
        Snd->RepeatPos = 0;
        Snd->FlagOnEnd = FlagToSetAtSoundEnd;
        Snd->UseIntCompute = UseIntFastMath;
        Snd->FrequencyVar = Frequency;
        Snd->DPos = 0.0;
        Snd->DRepeatPos = 0.0;
        if (PlayingSoundList == 0) { Snd->NextSound = 0; Snd->PrevSound = 0; PlayingSoundList = Snd; }
        else                       { Snd->NextSound = PlayingSoundList; Snd->PrevSound = 0; PlayingSoundList->PrevSound = Snd; PlayingSoundList = Snd; }
      SDL_UnlockAudio();
      return(Snd);
    }

    bool IsPlayed (void * SoundHandle)
    {
      return ( ((FindSound(SoundHandle))) ? true : false );
      {
        return(true);
      }
    }

    void Stop_PlaySound(void * SoundHandle)
    {
      Sound * Snd;

      if (!SoundActivated) return;

      SDL_LockAudio();

      Snd = PlayingSoundList;
      while (Snd)
      {
        if (Snd == SoundHandle)
        {
          Snd->Repeat = false;
          Snd->RLen = 0;
          Snd->DeletePending = true;
        }

        Snd = Snd->NextSound;
      }


      SDL_UnlockAudio();
    }

    void Stop_AllSounds()
    {
      Sound * Snd;

       SDL_LockAudio();

       Snd = PlayingSoundList;
       while (Snd)
       {
         Snd->Repeat = false;
         Snd->RLen = 0;
         Snd->DeletePending = true;

         Snd = Snd->NextSound;
       }

       SDL_UnlockAudio();
    }



    void ModifyFrequency(void * SoundHandle, double NewFrequency)
    {
      Sound * Snd;

      if ((Snd = FindSound(SoundHandle)))
      {
        Snd->FrequencyVar = NewFrequency;
      }

    }
    void EndAudio()
    {
      SDL_PauseAudio(1);
      SDL_CloseAudio();
    }

};

#endif /* ZSOUND_H_ */
