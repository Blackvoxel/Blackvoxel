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
 * ZGame.h
 *
 *  Created on: 13 mai 2011
 *      Author: laurent
 */

#ifndef Z_ZGAME_H
#define Z_ZGAME_H

//#ifndef Z_ZGAME_H
//#  include "ZGame.h"
//#endif

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif

#ifndef _SDL_H
#  include "SDL/SDL.h"
#endif

#ifndef Z_ZVOXELTYPE_H
#  include "ZVoxelType.h"
#endif


#ifndef Z_ZTEXTUREMANAGER_H
#  include "ZTextureManager.h"
#endif


#ifndef Z_ZGUI_H
#  include "ZGui.h"
#endif

#ifndef Z_ZTILESETS_H
#  include "ZTileSets.h"
#endif

#ifndef Z_ZSETTINGS_HARDWARE_H
#  include "ZSettings_Hardware.h"
#endif

#ifndef Z_EVENTDISPATCH_H
#  include "ZEventManager.h"
#endif

#ifndef Z_ZGAME_EVENTS_H
#  include "ZGame_Events.h"
#endif

#ifndef Z_ZSOUND_H
#  include "ZSound.h"
#endif

#ifndef Z_ZWORLD_H
#  include "ZWorld.h"
#endif

#ifndef Z_ZACTOR_PLAYER_H
#  include "ZActor_Player.h"
#endif

#ifndef Z_ZSECTORSTREAMLOADER_H
#  include "ZSectorStreamLoader.h"
#endif

#ifndef Z_ZVOXELPROCESSOR_H
#  include "ZVoxelProcessor.h"
#endif

#ifndef Z_ZGAME_GUI_H
#  include "ZGameWindow_Inventory.h"
#endif

#ifndef Z_ZGAMEWINDOW_VOXELTYPEBAR_H
#  include "ZGameWindow_VoxelTypeBar.h"
#endif

#ifndef Z_ZGAMEWINDOW_STORAGE_H
#  include "ZGameWindow_Storage.h"
#endif

#ifndef Z_ZGAMEWINDOW_PROGRAMMABLE_H
#  include "ZGameWindow_Programmable.h"
#endif

#ifndef Z_ZGAMEWINDOW_USERTEXTURETRANSFORMER_H
#  include "ZGameWindow_UserTextureTransformer.h"
#endif

#ifndef Z_ZGAMEWINDOW_PROGRESSBAR_H
#  include "ZGameWindow_ProgressBar.h"
#endif

#ifndef Z_ZGAMEWINDOW_ASMDEBUG_H
#  include "ZGameWindow_AsmDebug.h"
#endif

#ifndef Z_ZGAMEWINDOW_ASMHARDWARE_H
#  include "ZGameWindow_AsmHardware.h"
#endif

#ifndef Z_ZGAMEWINDOW_ASMEXTENDEDREGISTERS_H
#include "ZGameWindow_AsmExtendedRegisters.h"
#endif

#ifndef ZGAMEWINDOW_COMPILATION_RESULT_H
#  include "ZGameWindow_Compilation_Result.h"
#endif

#ifndef Z_ZGAMEWINDOW_RESUMEREQUEST_H
#  include "ZGameWindow_ResumeRequest.h"
#endif

#ifndef Z_ZGAMEWINDOW_ADVERTISING_H
#  include "ZGameWindow_Advertising.h"
#endif

#ifndef Z_GAMEWINDOW_DISPLAYINFOS_H
#  include "ZGameWindow_DisplayInfos.h"
#endif

#ifndef Z_GAMEWINDOW_SEQUENCER_H
#  include "ZGameWindow_Sequencer.h"
#endif

#ifndef Z_ZGAMEWINDOW_ZPROGROBOT_ASM_H
#  include "ZGameWindow_ZProgRobot_Asm.h"
#endif

#ifndef Z_ZGAMEWINDOW_SPS_H
#  include "ZGameWindow_SPS.h"
#endif

#ifndef Z_ZGAMEWINDOW_SCAN_H
#  include "ZGameWindow_Scan.h"
#endif

#ifndef Z_ZGAMEWINDOW_RTFM_H
#  include "ZGameWindow_RTFM.h"
#endif

#ifndef Z_ZGAMESTAT_H
#  include "ZGameStat.h"
#endif

#ifndef Z_WORLDINFO_H
#  include "ZWorldInfo.h"
#endif

#ifndef Z_ZFASTRANDOM_H
#  include "ZFastRandom.h"
#endif

#ifndef Z_ZPOINTLIST_H
#  include "ZPointList.h"
#endif

#ifndef Z_ZLOG_H
#  include "ZLog.h"
#endif

#ifndef Z_ZGAMEEVENTSEQUENCER_H
#  include "ZGameEventSequencer.h"
#endif

#ifndef Z_ZHARDWAREDEPENDENT_H
#  include "ZOs_Specific_HardwareDependent.h"
#endif

#ifndef Z_ZGAMEINFO_H
#  include "ZGameInfo.h"
#endif


class ZRender_Basic;
class ZGameWindow_ProgRobot_Remote;
class ZGameWindow_ResumeRequest_Little;

class ZGame
{
  public:

   ZVector3L ShipCenter; // Test

   ZGame() {
             Initialized_SDL =
             Initialized_GraphicMode =
             Initialized_TextureManager =
             Initialized_GuiManager =
             Initialized_OpenGLGameSettings =
             Initialized_Glew =
             Initialized_LoadingScreen =
             Initialized_VoxelTypeManager =
             Initialized_EventManager =
             Initialized_TileSetsAndFonts =
             Initialized_Settings =
             Initialized_Renderer =
             Initialized_Game_Events =
             Initialized_Sound =
             Initialized_World =
             Initialized_PhysicEngine =
             Initialized_SectorLoader =
             Initialized_VoxelProcessor=
             Initialized_RendererSettings=
             Initialized_GameWindows=
             Initialized_ToolManager =
             Initialized_UserDataStorage =
             Initialized_WorldInfo =
             Initialized_GameEventSequencer =
             Initialized_HardwareInfo =
             Initialized_GameStats =
             false;
             TileSetStyles = 0 ; Font_1 =  0; GuiTileset = 0;
             Settings_Hardware = 0;
             UniverseNum = 1;
             Game_Events = 0;
             Basic_Renderer = 0;
             World = 0;
             PhysicEngine = 0;
             SectorLoader = 0;
             VoxelProcessor = 0;
             Time_GameLoop = 16.0;
             VoxelTypeBar = 0;
             ToolManager = 0;
             VoxelTypeBar = 0;
             GameWindow_Storage = 0;
             GameWindow_Programmable = 0;
             GameWindow_Inventory = 0;
             GameWindow_DisplayInfos = 0;
             GameWindow_ProgRobot_Asm = 0;
             GameWindow_ProgRobot_Remote = 0;
             GameWindow_AsmDebug = 0;
             GameWindow_AsmHardware = 0;
             GameWindow_AsmExtendedRegisters = 0;
             GameWindow_Compilation_Result = 0;
             GameWindow_ResumeRequest = 0;
             GameWindow_ResumeRequest_Little = 0;
             GameWindow_SPS = 0;
             GameWindow_Scan = 0;
             GameProgressBar = 0;
             GameWindow_RTFM = 0;
             Game_Run = false;
             screen = 0;
             GameWindow_Advertising = 0;
             Sound = 0;
             GameWindow_UserTextureTransformer = 0;
             GameWindow_Sequencer = 0;
             GameStat = 0;
             WorldInfo = 0;
             ShipCenter = 0;

             GameEventSequencer = 0;
             Time_FrameTime = 20;
             Time_GameElapsedTime = 0;
             VFov = 63.597825649;

             HardwareInfo = 0;


             // Game Info : Contains all data that must be reset between games.

             InitGameSession();

   }
  ~ZGame() { UniverseNum = 0; }

  ZLog InitLog;

  //

  ZLightSpeedRandom Random;
  ZPointList PointList;

  // Usefull directory

  ZString Path_GameFiles;
  ZString Path_UserData;
  ZString Path_HelperFilesOut;

  ZString Path_Universes;
  ZString Path_ActualUniverse;
  ZString Path_UserTextures;
  ZString Path_UserScripts;
  ZString Path_UserScripts_Squirrel;
  ZString Path_UserScripts_Asm_1;
  ZString Path_UserScripts_UserData;

  // Flags

  bool Enable_MVI;           // Enable or disable massive voxel interraction and animation processing.
  bool Enable_LoadNewSector; // Enable new sector loading and rendering. Disable Locks to only loaded sectors.
  bool Enable_NewSectorRendering; // Enable to make display lists for new incoming sectors.
  bool Stop_Programmable_Robots; // This flag signal to user programmable robots to stop running as soon as possible.

  // Hardware detection infos

  ZHardwareInfo * HardwareInfo;


  // Game Loop continue flag

  bool Game_Run;

  // Game objects

  ZSettings_Hardware * Settings_Hardware;
  ZTextureManager      TextureManager;
  ZGraphicUserManager  GuiManager;
  ZVoxelTypeManager    VoxelTypeManager;
  ZEventManager        EventManager;
  ZTileSetStyles       * TileSetStyles;
  ZRender_Basic        * Basic_Renderer;
  ZSound               * Sound;


  // Jeu proprement dit

  ZGame_Events         * Game_Events;
  ZVoxelWorld          * World;
  ZActorPhysicEngine   * PhysicEngine;
  ZFileSectorLoader    * SectorLoader;
  ZVoxelProcessor      * VoxelProcessor;
  ZToolManager         * ToolManager;
  ZGameStat            * GameStat;
  ZWorldInfo           * WorldInfo;
  ZGameEventSequencer  * GameEventSequencer;

  // Game Windows
  ZGameWindow_Inventory              * GameWindow_Inventory;
  ZGameWindow_VoxelTypeBar           * VoxelTypeBar;
  ZGameWindow_Storage                * GameWindow_Storage;
  ZGameWindow_Programmable           * GameWindow_Programmable;
  ZGameWindow_UserTextureTransformer * GameWindow_UserTextureTransformer;
  ZGameWindow_ProgRobot_Asm          * GameWindow_ProgRobot_Asm;
  ZGameWindow_ProgRobot_Remote          * GameWindow_ProgRobot_Remote;
  ZGameWindow_ProgressBar            * GameProgressBar;
  ZGameWindow_Advertising            * GameWindow_Advertising;
  ZGameWindow_DisplayInfos           * GameWindow_DisplayInfos;
  ZGameWindow_Sequencer              * GameWindow_Sequencer;
  ZGameWindow_AsmDebug               * GameWindow_AsmDebug;
  ZGameWindow_AsmHardware            * GameWindow_AsmHardware;
  ZGameWindow_AsmExtendedRegisters   * GameWindow_AsmExtendedRegisters;
  ZGameWindow_Compilation_Result     * GameWindow_Compilation_Result;
  ZGameWindow_ResumeRequest          * GameWindow_ResumeRequest;
  ZGameWindow_ResumeRequest_Little   * GameWindow_ResumeRequest_Little;
  ZGameWindow_SPS                    * GameWindow_SPS;
  ZGameWindow_Scan                   * GameWindow_Scan;
  ZGameWindow_RTFM                   * GameWindow_RTFM;


  bool Initialized_UserDataStorage;
  bool Initialized_Settings;
  bool Initialized_HardwareInfo;
  bool Initialized_Glew;
  bool Initialized_LoadingScreen;
  bool Initialized_SDL;
  bool Initialized_GraphicMode;
  bool Initialized_TextureManager;
  bool Initialized_EventManager;
  bool Initialized_GuiManager;
  bool Initialized_OpenGLGameSettings;
  bool Initialized_VoxelTypeManager;
  bool Initialized_TileSetsAndFonts;
  bool Initialized_Renderer;
  bool Initialized_Game_Events;
  bool Initialized_Sound;
  bool Initialized_World;
  bool Initialized_PhysicEngine;
  bool Initialized_SectorLoader;
  bool Initialized_VoxelProcessor;
  bool Initialized_RendererSettings;
  bool Initialized_GameWindows;
  bool Initialized_ToolManager;
  bool Initialized_GameStats;
  bool Initialized_WorldInfo;
  bool Initialized_GameEventSequencer;

  // Services

  bool Service_RemoteRobotServerStarted;

  // Screen Informations

  SDL_Surface * screen;
  ZVector2L ScreenResolution;   // Taille réelle de la zone d'affichage.
  ZVector2L HardwareResolution; // Resolution qui est demandée à SDL.
  ZVector2L DesktopResolution;  // Résolution du bureau.
  double    VFov;               // Vertical Fov;

  // timers

  double Time_GameLoop;
  UELong Time_FrameTime; // Same as Time_GameLoop but in integer format;
  UELong Time_GameElapsedTime;

  // Game Options

  ZGameInfo GameInfo;

// General Inits

  bool Init_UserDataStorage(ZLog * InitLog);
  bool Init_HardwareDetection(ZLog * InitLog);
  bool Init_Settings(ZLog * InitLog);
  bool Init_SDL(ZLog * InitLog);
  bool Init_GraphicMode(ZLog * InitLog);
  bool Init_Glew(ZLog * InitLog);
  bool Init_LoadingScreen(ZLog * InitLog);
  bool Init_VoxelTypeManager(ZLog * InitLog);
  bool Init_TextureManager(ZLog * InitLog);
  bool Init_OpenGLGameSettings(ZLog * InitLog);
  bool Init_EventManager(ZLog * InitLog);
  bool Init_GuiManager(ZLog * InitLog);
  bool Init_TileSetsAndFonts(ZLog * InitLog);
  bool Init_Renderer(ZLog * InitLog);
  bool Init_Sound(ZLog * InitLog);

  bool Cleanup_UserDataStorage(ZLog * InitLog);
  bool Cleanup_HardwareDetection(ZLog * InitLog);
  bool Cleanup_Settings(ZLog * InitLog);
  bool Cleanup_SDL(ZLog * InitLog);
  bool Cleanup_GraphicMode(ZLog * InitLog);
  bool Cleanup_TextureManager(ZLog * InitLog);
  bool Cleanup_VoxelTypeManager(ZLog * InitLog);
  bool Cleanup_GuiManager(ZLog * InitLog);
  bool Cleanup_EventManager(ZLog * InitLog);
  bool Cleanup_OpenGLGameSettings(ZLog * InitLog);
  bool Cleanup_LoadingScreen(ZLog * InitLog);
  bool Cleanup_Glew(ZLog * InitLog);
  bool Cleanup_TileSetsAndFonts(ZLog * InitLog);
  bool Cleanup_Renderer(ZLog * InitLog);
  bool Cleanup_Sound(ZLog * InitLog);

// Specific game Settings.

  bool Start_GameEventSequencer();
  bool Start_WorldInfo();
  bool Start_Game_Stats();
  bool Start_Game_Events();
  bool Start_World();
  bool Start_PhysicEngine();
  bool Start_SectorLoader();
  bool Start_VoxelProcessor();
  bool Start_RendererSettings();
  bool Start_GameWindows();
  bool Start_ToolManager();


  bool End_WorldInfo();
  bool End_Game_Events();
  bool End_World();
  bool End_PhysicEngine();
  bool End_SectorLoader();
  bool End_VoxelProcessor();
  bool End_RendererSettings();
  bool End_GameWindows();
  bool End_ToolManager();
  bool End_Game_Stats();
  bool End_GameEventSequencer();



  bool Init()
  {
    bool result;

    result = Init_UserDataStorage(InitLog.Sec(1000));    if (!result) return(false);
    result = Init_HardwareDetection(InitLog.Sec(1010));  if (!result) return(false);
    result = Init_Settings(InitLog.Sec(1020));           if (!result) return(false);
    result = Init_SDL(InitLog.Sec(1030));                if (!result) return(false);
    result = Init_GraphicMode(InitLog.Sec(1040));        if (!result) return(false);
    result = Init_Glew(InitLog.Sec(1050));               if (!result) return(false);
    result = Init_LoadingScreen(InitLog.Sec(1055));      if (!result) return(false);
    result = Init_VoxelTypeManager(InitLog.Sec(1060));   if (!result) return(false);
    result = Init_TextureManager(InitLog.Sec(1070));     if (!result) return(false);
    result = Init_OpenGLGameSettings(InitLog.Sec(1080)); if (!result) return(false);
    result = Init_EventManager(InitLog.Sec(1090));       if (!result) return(false);
    result = Init_GuiManager(InitLog.Sec(1100));         if (!result) return(false);
    result = Init_TileSetsAndFonts(InitLog.Sec(1110));   if (!result) return(false);
    result = Init_Renderer(InitLog.Sec(1120));           if (!result) return(false);
    result = Init_Sound(InitLog.Sec(1130));              if (!result) return(false);
    return(true);
  }

  bool Start_Game()
  {
    bool result;

    //result = Start_PersistGameEnv();     if(!result) return(false);
    result = Start_GameEventSequencer(); if(!result) return(false);
    //result = Start_WorldInfo();          if(!result) return(false);
    result = Start_Game_Stats();         if(!result) return(false);
    result = Start_Game_Events();        if(!result) return(false);
    result = Start_World();              if(!result) return(false);
    result = Start_ToolManager();        if(!result) return(false);
    result = Start_PhysicEngine();       if(!result) return(false);
    result = Start_SectorLoader();       if(!result) return(false);
    result = Start_VoxelProcessor();     if(!result) return(false);
    result = Start_RendererSettings();   if(!result) return(false);
    result = Start_GameWindows();        if(!result) return(false);

    return(true);
  }

  bool End_Game()
  {
    if (Initialized_GameWindows)       End_GameWindows();
    if (Initialized_RendererSettings)  End_RendererSettings();
    if (Initialized_VoxelProcessor)    End_VoxelProcessor();
    if (Initialized_SectorLoader)      End_SectorLoader();
    if (Initialized_PhysicEngine)      End_PhysicEngine();
    if (Initialized_ToolManager)       End_ToolManager();
    if (Initialized_World)             End_World();
    if (Initialized_Game_Events)       End_Game_Events();
    if (Initialized_GameStats)         End_Game_Stats();
//    if (Initialized_WorldInfo)         End_WorldInfo();
    if (GameEventSequencer)            End_GameEventSequencer();
//                                     End_PersistGameEnv();
    Sound->Stop_AllSounds();

    return(true);
  }


  bool End()
  {
    if (Initialized_Sound)              Cleanup_Sound(InitLog.Sec(2130));
    if (Initialized_Renderer)           Cleanup_Renderer(InitLog.Sec(2120));
    if (Initialized_GuiManager)         Cleanup_GuiManager(InitLog.Sec(2100));
    if (Initialized_EventManager)       Cleanup_EventManager(InitLog.Sec(2090));
    if (Initialized_OpenGLGameSettings) Cleanup_OpenGLGameSettings(InitLog.Sec(2080));
    if (Initialized_TextureManager)     Cleanup_TextureManager(InitLog.Sec(2070));
    if (Initialized_VoxelTypeManager)   Cleanup_VoxelTypeManager(InitLog.Sec(2060));
    if (Initialized_Glew)               Cleanup_Glew(InitLog.Sec(2050));
    if (Initialized_GraphicMode)        Cleanup_GraphicMode(InitLog.Sec(2040));
    if (Initialized_SDL)                Cleanup_SDL(InitLog.Sec(2030));
    if (Initialized_TileSetsAndFonts)   Cleanup_TileSetsAndFonts(InitLog.Sec(2110));
    if (Initialized_Settings)           Cleanup_Settings(InitLog.Sec(2020));
    if (Initialized_HardwareInfo)       Cleanup_HardwareDetection(InitLog.Sec(2010));
    if (Initialized_UserDataStorage)    Cleanup_UserDataStorage(InitLog.Sec(2000));

    return(true);
  }

  // TileSets

  ZTileSet * Font_1;
  ZTileSet * Font_Symbols;
  ZTileSet * GuiTileset;

  enum {FONTSIZE_1 = 0,
        FONTSIZE_2 = 3,
        FONTSIZE_3 = 4,
        FONTSIZE_4 = 1,
        FONTSIZE_5 = 2,
        FONTSIZE_VAR1 = 5,
        FONTSIZE_VAR2 = 6,
        FONTSIZE_VAR3 = 7,
        FONTSIZE_VAR4 = 8,
        FONTSIZE_VAR5 = 9,
        FONTSIZE_VAR6 = 10,
        FONTSIZE_VAR7 = 11,
        FONTSIZE_VAR8 = 12,
        FONTSIZE_VAR9 = 13,
        FONTSIZE_VAR10= 14
  };


  // InGame

  ULong UniverseNum;

  void MoveShip()
  {
    ZVector3L VoxelCoords, Vx;
    ZVoxelLocation Loc;
    ZVector3d * Location, NewLocation;

    if (ShipCenter.x == 0 && ShipCenter.y == 0 && ShipCenter.z == 0) return;
    VoxelCoords = ShipCenter;



    for ( Vx.x = VoxelCoords.x - 5; Vx.x < VoxelCoords.x + 5; Vx.x ++ )
      for (Vx.z = VoxelCoords.z - 5; Vx.z < VoxelCoords.z + 5; Vx.z ++)
        for (Vx.y = VoxelCoords.y; Vx.y < VoxelCoords.y +5; Vx.y ++ )
        {
          World->MoveVoxel(Vx.x, Vx.y, Vx.z, Vx.x, Vx.y, Vx.z - 1, 0, true);
          World->GetVoxelLocation( &Loc, Vx.x, Vx.y, Vx.z );
          Loc.Sector->Flag_HighPriorityRefresh = true;
          World->GetVoxelLocation( &Loc, Vx.x, Vx.y, Vx.z-1 );
          Loc.Sector->Flag_HighPriorityRefresh = true;
        }
/*
    World->GetVoxelLocation( &Loc, Vx.x+5, Vx.y, Vx.z );
    Loc.Sector->Flag_HighPriorityRefresh = true;
    World->GetVoxelLocation( &Loc, Vx.x-5, Vx.y, Vx.z );
    Loc.Sector->Flag_HighPriorityRefresh = true;
    World->GetVoxelLocation( &Loc, Vx.x, Vx.y, Vx.z+5 );
    Loc.Sector->Flag_HighPriorityRefresh = true;
    World->GetVoxelLocation( &Loc, Vx.x, Vx.y, Vx.z-5 );
    Loc.Sector->Flag_HighPriorityRefresh = true;
*/
    Location = &PhysicEngine->GetSelectedActor()->Location;

    NewLocation = *Location;
    NewLocation.z -= 256.0;
    PhysicEngine->GetSelectedActor()->SetPosition(NewLocation);
  }

  bool OutputHelperFiles();


  bool LoadGameInfo();
  bool SaveGameInfo();
  void InitGameSession(); // Init ZGame data that must be reset beween games.

};

#endif /* Z_ZGAME_H */
