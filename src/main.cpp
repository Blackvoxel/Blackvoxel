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
    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>
    #include <GL/glew.h>
// #include <GL/gl.h>
    #include <GL/glext.h>
   // #include <GL/glut.h>
    #include "SDL/SDL.h"
    #include "z/ZTypes.h"
    #include "bmploader.h"

    #include "math.h"

    #include "ZCamera.h"
    #include "ZWorld.h"

#ifndef Z_ZVOXELTYPE_H
#  include "ZVoxelType.h"
#endif

    #include "ZRender_Basic.h"
    #include "ZActorPhysics.h"
    #include "ZActor_Player.h"

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif

#ifndef Z_ZSOUND_H
#  include "ZSound.h"
#endif

#ifndef Z_ZSECTORSTREAMLOADER_H
#  include "ZSectorStreamLoader.h"
#endif

#ifndef Z_ZVOXELPROCESSOR_H
#  include "ZVoxelProcessor.h"
#endif

#ifndef Z_ZTEXTUREMANAGER_H
#  include "ZTextureManager.h"
#endif

#ifndef Z_ZGUI_H
#  include "ZGui.h"
#endif

#ifndef Z_GUI_FONTFRAME_H
#  include "ZGui_FontFrame.h"
#endif

#ifndef Z_ZTILESETS_H
#  include "ZTileSets.h"
#endif

#ifndef Z_EVENTDISPATCH_H
#  include "ZEventManager.h"
#endif

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

#ifndef Z_ZSCREEN_MAIN_H
#  include "ZScreen_Main.h"
#endif

#ifndef Z_ZSCREEN_SLOTSELECTION_H
#  include "ZScreen_SlotSelection.h"
#endif

#ifndef Z_ZSCREEN_LOADING_H
#  include "ZScreen_Loading.h"
#endif

#ifndef Z_ZSCREEN_SAVING_H
#  include "ZScreen_Saving.h"
#endif

#ifndef Z_ZSCREEN_OPTIONS_H
#  include "ZScreen_Options_Display.h"
#endif

#ifndef Z_ZSCREEN_CHOOSEOPTION_H
#  include "ZScreen_ChooseOption.h"
#endif

#ifndef Z_ZSCREEN_OPTIONS_SOUND_H
#  include "ZScreen_Options_Sound.h"
#endif

#ifndef Z_ZSCREEN_OPTIONS_MOUSE_H
#  include "ZScreen_Options_Gameplay.h"
#endif

#ifndef Z_ZSCREEN_OPTIONS_KEYMAP_H
#  include "ZScreen_Options_Keymap.h"
#endif

#ifndef Z_ZSCREEN_MESSAGE_H
#  include "ZScreen_Message.h"
#endif

#ifndef Z_ZWORLDCONVERT_H
#  include "ZWorldConvert.h"
#endif

#ifndef Z_ZHIGHPERFTIMER_H
#  include "ZHighPerfTimer.h"
#endif

#ifndef Z_ZFASTRANDOM_H
#  include "z/ZFastRandom.h"
#endif

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif

#ifndef Z_ZRANDOM_MAKEGOODNUMBERS_H
#  include "ZRandom_MakeGoodNumbers.h"
#endif

#ifndef Z_ZGENERICCANVA_H
#  include "z/ZGenericCanva.h"
#endif

#ifndef Z_ZOS_SPECIFIC_VARIOUS_H
#  include "ZOs_Specific_Various.h"
#endif

#ifndef Z_ZTEST_PARTS_H
#  include "Z0Test_Parts.h"
#endif


ZGame * Ge;
double FrameTime;


int main(int argc, char *argv[])
{
  ULong Result;
  bool StartGame;

  // Windows Terminal Fix

  #ifdef ZENV_OS_WINDOWS
    Windows_DisplayConsole();
  #endif

  // Start

    printf ("Starting BlackVoxel...\n");

  // Test Code

  #if DEVELOPPEMENT_ON == 1
/*
     ZTest_Parts TestParts;
     if (!TestParts.RunTestCode()) exit(0);
*/
  #endif

  // Game main object

    ZGame GameEnv;
    Ge = &GameEnv;

  // Main Game Object Initialisation

    if (!GameEnv.Init()) return(-10);

  // Debug output for manufacturing compositions

    #if COMPILEOPTION_FABDATABASEOUTPUT == 1
      GameEnv.VoxelTypeManager.OutFabInfos();
      GameEnv.VoxelTypeManager.FindFabConflicts_V2();
      GameEnv.OutputHelperFiles();

    #endif

  // Game Menu Loop

    for ( bool GameContinue = true; GameContinue ; )
    {
      StartGame = false;


      // ***************************************** Main Title Screen ****************************************************

      SDL_ShowCursor(SDL_ENABLE);
      SDL_WM_GrabInput(SDL_GRAB_OFF);

      ZScreen_Main Screen_Main;

      for ( bool ScreenTitle_Continue = true; ScreenTitle_Continue ; )
      {
        // Display Main Screen and wait user choice;

        Result = Screen_Main.ProcessScreen(&GameEnv);

        switch(Result)
        {
          case ZScreen_Main::CHOICE_QUIT:     // Quit the game

                                              GameContinue = false;
                                              StartGame = false;
                                              ScreenTitle_Continue = false;
                                              break;

          case ZScreen_Main::CHOICE_OPTIONS:  // Option Section
                                              {
                                                ZScreen_ChooseOption Screen_ChooseOption;
                                                do
                                                {
                                                  Screen_ChooseOption.ProcessScreen(&GameEnv);
                                                  switch( Screen_ChooseOption.ResultCode )
                                                  {
                                                    case ZScreen_ChooseOption::CHOICE_DISPLAY:  { ZScreen_Options_Display Screen_Options_Display; Screen_Options_Display.ProcessScreen(&GameEnv); break; }
                                                    case ZScreen_ChooseOption::CHOICE_SOUND:    { ZScreen_Options_Sound Screen_Options_Sound;     Screen_Options_Sound.ProcessScreen(&GameEnv); break; }
                                                    case ZScreen_ChooseOption::CHOICE_MOUSE:    { ZScreen_Options_Game Screen_Options_Mouse;     Screen_Options_Mouse.ProcessScreen(&GameEnv);   break; }
                                                    case ZScreen_ChooseOption::CHOICE_KEYMAP:  { ZScreen_Options_Keymap Screen_Options_Keymap;    Screen_Options_Keymap.ProcessScreen(&GameEnv);  break; }
                                                  }
                                                } while (Screen_ChooseOption.ResultCode != ZScreen_ChooseOption::CHOICE_QUIT);
                                              }
                                              break;

          case ZScreen_Main::CHOICE_PLAYGAME: // Play the game
                                              StartGame = true;
                                              ScreenTitle_Continue = false;
                                              break;
        }
      }



      // ****************************************** Entering the game ****************************************************

      if (StartGame)
      {

        // ****************************** Slot Selection screen ******************************

          ZScreen_SlotSelection Screen_SlotSelection;

          GameEnv.UniverseNum = Screen_SlotSelection.ProcessScreen(&GameEnv);

          // ********************************** Loading Screen ******************************

          ZScreen_Loading Screen_Loading;

          Screen_Loading.ProcessScreen(&GameEnv);

          //     ***************************** THE GAME ******************************

          // Starting procedure

          if (!GameEnv.Start_Game()) {printf("Start Game Failled\n"); exit(0);}

          // Mouse grab and cursor disable for gaming.

          if (!COMPILEOPTION_NOMOUSECAPTURE)
          {
            SDL_ShowCursor(SDL_DISABLE);
            SDL_WM_GrabInput(SDL_GRAB_ON);
          }

          // Pre-Gameloop Initialisations.

          FrameTime = 20.0;
          ULong MoveShipCounter = 0;
          GameEnv.Time_FrameTime = 20000;
          ZHighPerfTimer Timer;
          double ReadableDisplayCounter = 0.0;
          GameEnv.GameEventSequencer->SetGameTime(GameEnv.PhysicEngine->GetSelectedActor()->Time_TotalGameTime);


          // *********************************** Main Game Loop **********************************************

          GameEnv.Game_Run = true;
          GameEnv.Time_FrameTime = 20;
          //ULong PerfTable[100];


          while (GameEnv.Game_Run)
          {
            Timer.Start();

            // Process Input events (Mouse, Keyboard)

            GameEnv.EventManager.ProcessEvents();       // Process incoming events.
            GameEnv.Game_Events->Process_StillEvents(); // Process repeating checked events.

            // Process incoming sectors from the make/load working thread

            GameEnv.World->ProcessNewLoadedSectors();

            // if (MoveShipCounter>125 ) {GameEnv.MoveShip(); MoveShipCounter = 0; }

            // Player physics

            GameEnv.PhysicEngine->DoPhysic(COMPILEOPTION_ADD_MIDLOOP_PHYSIC_ITERATION ? GameEnv.Time_FrameTime >> 1 : GameEnv.Time_FrameTime);

            // Voxel Processor Get Player Position.

            ZActor * Actor;
            Actor = GameEnv.PhysicEngine->GetSelectedActor();
            GameEnv.VoxelProcessor->SetPlayerPosition(Actor->Location.x,Actor->Location.y,Actor->Location.z);

            // Sector Ejection processing.

            GameEnv.World->ProcessOldEjectedSectors();

            // Advertising messages

            GameEnv.GameWindow_Advertising->Advertising_Actions((double)FrameTime);

            // Tool activation and desactivation

            GameEnv.ToolManager->ProcessAndDisplay();
            GameEnv.PhysicEngine->GetSelectedActor()->Process_Powers();

            // Rendering

            GameEnv.Basic_Renderer->Render();
            //PerfTable[8] = Timer.GetIntermediateSlice();

            GameEnv.GuiManager.Render();
            //PerfTable[9] = Timer.GetIntermediateSlice();

            // On some little devices, this extra step helps getting better reactivity
            #if COMPILEOPTION_ADD_MIDLOOP_PHYSIC_ITERATION==1
            GameEnv.EventManager.ProcessEvents();       // Process incoming events.
            GameEnv.Game_Events->Process_StillEvents(); // Process repeating checked events.
            GameEnv.PhysicEngine->DoPhysic(GameEnv.Time_FrameTime >> 1);
            #endif

            // Swapping OpenGL Surfaces.

            SDL_GL_SwapBuffers( );
            //PerfTable[10] = Timer.GetIntermediateSlice();


            // Game Events.

            GameEnv.GameEventSequencer->ProcessEvents(GameEnv.PhysicEngine->GetSelectedActor()->Time_TotalGameTime);

            // Time Functions

            Timer.End();
            FrameTime = GameEnv.Time_GameLoop = Timer.GetResult() / 1000.0;
            GameEnv.Time_FrameTime = Timer.GetResult();
            GameEnv.Time_GameElapsedTime += GameEnv.Time_FrameTime;
            if (GameEnv.Time_GameLoop > 64.0) GameEnv.Time_GameLoop = 64.0; // Game cannot make too long frames because inaccuracy. In this case, game must slow down.
            GameEnv.GameStat->FrameTime = (ULong) FrameTime;
            GameEnv.GameStat->DoLogRecord();
            MoveShipCounter += FrameTime;

            // Frametime Display;

            ReadableDisplayCounter += FrameTime;
            if (GameEnv.GameWindow_DisplayInfos->Is_Shown() )
            {
              if (ReadableDisplayCounter > 500.0)
              {
                ReadableDisplayCounter = 0.0;
                ZString As;

                As = "FPS: "; As << (ULong)( 1000.0 / FrameTime) << " FTM: " << FrameTime;
                /*
                for (ULong i=0;i<12;i++)
                {
                  As<<"\n" <<(ULong)i<<":"<<(ULong)PerfTable[i];
                }
                */
                GameEnv.GameWindow_DisplayInfos->SetText(&As);
              }
            }
          }

          // Display screen "Saving game".

          ZScreen_Saving Screen_Saving;
          Screen_Saving.ProcessScreen(&GameEnv);

          // Save game and cleanup all the mess before returning to title screen

          GameEnv.Basic_Renderer->Cleanup();
          GameEnv.End_Game();
        }

        // Relooping to the title screen
      }

      // Getting out of the game.

#if COMPILEOPTION_HELP_SCREEN==1
      ZScreen_Message Screen_Message;
      Screen_Message.ProcessScreen(&GameEnv);
#endif

      GameEnv.End();

      return 0;
    }


