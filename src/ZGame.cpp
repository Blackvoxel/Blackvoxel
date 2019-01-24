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
 * ZGame.cpp
 *
 *  Created on: 13 mai 2011
 *      Author: laurent
 */

#include "ZGame.h"
#include <GL/glew.h>
#include <math.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include <GL/glew.h>
#include "ZRender_Basic.h"

#ifndef Z_ZSTREAMS_FILE_H
#  include "z/ZStream_File.h"
#endif

#ifndef Z_ZGAME_DEVHELPERS_H
#  include "ZGame_DevHelpers.h"
#endif

#ifndef Z_ZTOOLS_H
#  include "ZTools.h"
#endif

#ifndef Z_ZTOOL_CONSTRUCTOR_H
#  include "ZTool_Constructor.h"
#endif

#ifndef Z_ZTOOL_SCAN_H
#  include "ZTool_Scan.h"
#endif

#ifndef Z_ZTOOL_ROTATE_H
#  include "ZTool_Rotate.h"
#endif

#ifndef Z_ZTOOL_ORESCAN_H
#  include "ZTool_OreScan.h"
#endif

#ifndef Z_ZTOOL_WIRELESSLINKER_H
#  include "ZTool_Linker.h"
#endif

#ifndef Z_ZLOADINGSCREEN_H
#  include "ZLoadingScreen.h"
#endif

#ifndef Z_ZGAMEWINDOW_ZPROGROBOT_REMOTE_H
#  include "ZGameWindow_ZProgRobot_Remote.h"
#endif

#ifndef Z_ZGAMEWINDOW_RESUMEREQUEST_LITTLE_H
#  include "ZGameWindow_ResumeRequest_Little.h"
#endif


bool ZGame::Init_UserDataStorage(ZLog * InitLog)
{
  ZString ErrorMsg;

  InitLog->Log( 1, ZLog::INFO, "Starting : UserDataStorage Initialization");

  // Directory for Dev Helper out

  Path_HelperFilesOut = "Out";

  // Directory for user data

  if (COMPILEOPTION_USEHOMEDIRSTORAGE)
  {
    Path_UserData = ZStream_File::Get_Directory_UserData();
  }
  Path_UserData.AddToPath(COMPILEOPTION_SAVEFOLDERNAME);
  if ( !ZStream_File::Directory_CreateIfNotExists(Path_UserData.String)) { ErrorMsg << "ERROR *** : Can't create application user directory [" << Path_UserData << "]."; InitLog->Log(3, ZLog::FAIL, ErrorMsg); puts(ErrorMsg.String); return(false); }

  // Directory for game files

  Path_GameFiles = COMPILEOPTION_DATAFILESPATH;

  // Subdirectories

  Path_Universes = Path_UserData; Path_Universes.AddToPath("Universes");
  Path_UserTextures = Path_UserData; Path_UserTextures.AddToPath("UserTextures");

  if (!ZStream_File::Directory_CreateIfNotExists(Path_Universes.String))    { ErrorMsg << "ERROR *** : Can't create universes directory [" << Path_Universes << "]."; InitLog->Log(4, ZLog::FAIL, ErrorMsg); puts(ErrorMsg.String); return(false); }
  if (!ZStream_File::Directory_CreateIfNotExists(Path_UserTextures.String)) { ErrorMsg << "ERROR *** : Can't create user textures directory [" << Path_UserTextures << "]."; InitLog->Log(5, ZLog::FAIL, ErrorMsg); puts(ErrorMsg.String); return(false); }

  Initialized_UserDataStorage = true;
  InitLog->Log(2, ZLog::INFO, "Ended OK : UserDataStorage Initialization");
  return(true);
}

bool ZGame::Init_HardwareDetection(ZLog * InitLog)
{
  ZString Text;
  InitLog->Log( 1, ZLog::INFO, "Starting : Hardware Detection");
  HardwareInfo = new ZHardwareInfo();
  HardwareInfo->DetectHardware();
  (Text = "Info : Selected Hardware Profile : ") << HardwareInfo->GetProfileText();
  InitLog->Log( 2, ZLog::INFO, Text);
  Initialized_HardwareInfo = true;
  InitLog->Log( 3, ZLog::INFO, "Ended OK : Hardware Detection");
  return(true);
}

bool ZGame::Cleanup_HardwareDetection(ZLog * InitLog)
{
  InitLog->Log( 0, ZLog::INFO, "Cleanup : Hardware Detection");
  if (Initialized_HardwareInfo) {delete(HardwareInfo); Initialized_HardwareInfo = false;}
  return(true);
}

bool ZGame::Cleanup_UserDataStorage(ZLog * InitLog)
{
  InitLog->Log(0, ZLog::INFO, "Cleanup : UserDataStorage");
  return(false);
}

bool ZGame::Init_Settings(ZLog * InitLog)
{
  bool Res;
  ZString Text;

  InitLog->Log(1, ZLog::INFO, "Starting : Settings");

  Settings_Hardware = new ZSettings_Hardware;
  Res = Settings_Hardware->Load(); // If loading fail, continue anyway with default.

  if (Res) InitLog->Log(3, ZLog::INFO, "Info : Hardware Settings Loaded From File");
  else     InitLog->Log(4, ZLog::INFO, "Info : Can't load Settings from file, default settings taken.");

  // Some settings are now computed from recorded settings and/or hardware info.

  Settings_Hardware->AdjustForRealHardware(HardwareInfo);

  (Text = "Info : Rendering distance(in sectors) : [") << Settings_Hardware->RenderingDistance_Horizontal << "," << Settings_Hardware->RenderingDistance_Vertical << "]";
  InitLog->Log(2, ZLog::INFO, Text);

  Initialized_Settings = true;

  InitLog->Log(3, ZLog::INFO, "Ended OK : Settings");
  return(true);
}

bool ZGame::Cleanup_Settings(ZLog * InitLog)
{
  InitLog->Log(0, ZLog::INFO, "Cleanup : Settings");
  if (Settings_Hardware) { delete Settings_Hardware; Settings_Hardware = 0; }
  return(true);
}

bool ZGame::Init_SDL(ZLog * InitLog)
{
  int Result;

  InitLog->Log(1, ZLog::INFO, "Starting : SDL");

  Result = SDL_Init( SDL_INIT_EVERYTHING ); if (Result == -1) {ZString ErrorMsg; ErrorMsg = "*** ERROR : SDL library SDL_Init Failled. SDL Report : "; ErrorMsg << SDL_GetError(); InitLog->Log( 3, ZLog::FAIL, ErrorMsg); return(false); }
  Initialized_SDL = true;

  InitLog->Log(2, ZLog::INFO, "Ended OK : SDL");
  return(true);
}

bool ZGame::Cleanup_SDL(ZLog * InitLog)
{
  InitLog->Log(0, ZLog::INFO, "Cleanup : SDL");
  SDL_Quit();
  return(true);
}

bool ZGame::Init_GraphicMode(ZLog * InitLog)
{
  ZString LogMsg;

  InitLog->Log(1, ZLog::INFO, "Starting : Graphics Mode Init");

  // OpenGl Attibutes

  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

  // Getting system info

  const SDL_VideoInfo* info = NULL;
  info = SDL_GetVideoInfo( );
  DesktopResolution.x = info->current_w;
  DesktopResolution.y = info->current_h;

  LogMsg << "Info : SDL Infos [current_w:"<<info->current_w<<"][current_h:"<<info->current_h<<"][BitsPerPixel:"<<info->vfmt->BitsPerPixel<<"]";
  InitLog->Log(3, ZLog::IMINFO, LogMsg);

  // Flags like Fullscreen attributes

  Uint32 Flags = SDL_OPENGL;
  if (Settings_Hardware->Setting_FullScreen) Flags |= SDL_FULLSCREEN;
  // else                                    Flags |= SDL_RESIZABLE;

  // §§§ Todo : Write better code.
  // Windows High DPI Quick Workaround in automatic mode.
  // That's an emergency workaround preventing Blackvoxel to fail on high DPI screens.
  // It could also mitigate some problems with multiple screens.
  // Doing a better work will need some interface rework and experimentations with high DPI screens we don't have at this time.
  // Note we use a bad way for detection as compiling also lacking windows 8 headers at this time.

#ifdef ZENV_OS_WINDOWS
  if ( (DesktopResolution.x >= 2560 && DesktopResolution.y >= 1600) &&
       (Settings_Hardware->Setting_Resolution_h == 0 && Settings_Hardware->Setting_Resolution_v == 0))
  {
    // Divide resolutions by 2 because it will be upscalled by Windows 8
    DesktopResolution.x >>= 1;
    DesktopResolution.y >>= 1;

    // Force windowed mode because full screen is broken with windows GUI scalling.
    Flags &= ~ SDL_FULLSCREEN;
  }
#endif

  // Compute Hardware Resolution

  if (Settings_Hardware->Setting_Resolution_h == 0 && Settings_Hardware->Setting_Resolution_v == 0)
  {
    HardwareResolution.x = DesktopResolution.x;
    HardwareResolution.y = DesktopResolution.y;
    // If not full screen, lower resolution to take account of borders.
    if (!(Flags & SDL_FULLSCREEN))
    {
      HardwareResolution.x -= 50;HardwareResolution.y -= 100;

      // Raspberry Pi Have very limited fillrate capability, so clip size to avoid too big display size.
      #if COMPILEOPTION_PLATFORM_RASPBERRY_PI==1
      if ( HardwareResolution.x > 1024 && HardwareResolution.y > 600)
      {
        HardwareResolution.x = 1024;
        HardwareResolution.y = 600;
      }

      #endif

    }

  }
  else
  {
    HardwareResolution.x = Settings_Hardware->Setting_Resolution_h;
    HardwareResolution.y = Settings_Hardware->Setting_Resolution_v;
  }

  // Fix problem with vertical resolution mode

  if (DesktopResolution.x < DesktopResolution.y)
  {
    if ( Settings_Hardware->Setting_FullScreen )
    {
      Flags &= ~ SDL_FULLSCREEN;
    }
    if (Settings_Hardware->Setting_Resolution_h == 0 && Settings_Hardware->Setting_Resolution_v == 0)
    {
      HardwareResolution.x = DesktopResolution.x;
      HardwareResolution.y = (Long)(((double)DesktopResolution.x) / 1.77);
    }
  }

  // No frame window (Specific uses like video recording).

  if (Settings_Hardware->Setting_NoframeWindow)
  {
    Flags |= SDL_NOFRAME;
  }

  // Starting video mode

  SDL_WM_SetCaption("BlackVoxel", NULL);
  screen = SDL_SetVideoMode(HardwareResolution.x, HardwareResolution.y, 32, Flags );
  if ( screen == NULL )
  {
    ZString ErrorMsg; ErrorMsg << "*** ERROR : SDL library : Unable to init display mode [" << SDL_GetError() << "]";
    InitLog->Log(4, ZLog::FAIL, ErrorMsg);

    // Enforce failsafe procedure...

    ErrorMsg = "SDL library : Enforce failsafe screen mode";
    InitLog->Log(4, ZLog::INFO, ErrorMsg);

    HardwareResolution.x = DesktopResolution.x;
    HardwareResolution.y = DesktopResolution.y;
    screen = SDL_SetVideoMode(HardwareResolution.x, HardwareResolution.y, 32, Flags );

    // If failsafe failled, resign to surrender...

    if ( screen == NULL )
    {
      ErrorMsg.Clear();
      ErrorMsg << "*** ERROR : SDL library : Init display mode failsafe procedure fail [" << SDL_GetError() << "]";
      InitLog->Log(4, ZLog::FAIL, ErrorMsg);

      return(false);
    }
  }

  // Setting OpenGl ViewPort

  if (Settings_Hardware->Setting_ViewPort_Size_x == 0 && Settings_Hardware->Setting_ViewPort_Size_y ==0)
  {
    ScreenResolution.x = HardwareResolution.x;
    ScreenResolution.y = HardwareResolution.y;
    glViewport( 0, 0, ScreenResolution.x,ScreenResolution.y  );
  }
  else
  {
    ScreenResolution.x = Settings_Hardware->Setting_ViewPort_Size_x;
    ScreenResolution.y = Settings_Hardware->Setting_ViewPort_Size_y;
    glViewport( Settings_Hardware->Setting_ViewPort_Offset_x, Settings_Hardware->Setting_ViewPort_Offset_y, Settings_Hardware->Setting_ViewPort_Size_x, Settings_Hardware->Setting_ViewPort_Size_y);
  }

  if (!COMPILEOPTION_NOMOUSECAPTURE) SDL_ShowCursor(SDL_DISABLE);

  Initialized_GraphicMode = true;
  InitLog->Log(2, ZLog::INFO, "Ended Ok : Graphics Mode Init");
  return(true);
}

bool ZGame::Cleanup_GraphicMode(ZLog * InitLog)
{
  InitLog->Log(0, ZLog::INFO, "Cleanup : Graphics Mode Init");
  return(true);
}

bool ZGame::Init_TextureManager(ZLog * InitLog)
{
  bool Result;
  ZString Path;
  ZString Err, ErrMsg;
  bool Ol,Hs, Pi;

  InitLog->Log(1, ZLog::INFO, "Starting : Texture Manager Init");


  Result = true;
  ErrMsg = "*** ERROR : Missing gui texture file ";

  Ol = Settings_Hardware->Setting_OldLooked;
  Hs = COMPILEOPTION_HELP_SCREEN==1 ? true:false;
  Pi = COMPILEOPTION_PLATFORM_RASPBERRY_PI ? true:false;
  if (Ol) {Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/title_1_2.bmp");              Result = TextureManager.LoadBMPTexture(Path.String,0 ,false,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(7, ZLog::FAIL, Err); return(false); } }
  else    {Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/title_2_0.bmp");              Result = TextureManager.LoadBMPTexture(Path.String,0 ,true ,false,32);      if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(3, ZLog::FAIL, Err); return(false); } }
           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/blackvoxel_title_1_3.bmp");   Result = TextureManager.LoadBMPTexture(Path.String,1 ,!Pi  ,false,128);    if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(4, ZLog::FAIL, Err); return(false); }
           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/panel_2.bmp");                Result = TextureManager.LoadBMPTexture(Path.String,2 ,true ,true,32 );      if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(5, ZLog::FAIL, Err); return(false); }
           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/font_1_1.bmp");               Result = TextureManager.LoadBMPTexture(Path.String,3 ,false,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(6, ZLog::FAIL, Err); return(false); }
           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/title_1_2.bmp");              Result = TextureManager.LoadBMPTexture(Path.String,4 ,false,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(7, ZLog::FAIL, Err); return(false); }
  if (Ol) {Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/tile_1_1.bmp");               Result = TextureManager.LoadBMPTexture(Path.String,5 ,false,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(8, ZLog::FAIL, Err); return(false); }}
  else    {Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/screen_button_settings.bmp"); Result = TextureManager.LoadBMPTexture(Path.String,5 ,false,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(8, ZLog::FAIL, Err); return(false); }}
           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/tile_2_1.bmp");               Result = TextureManager.LoadBMPTexture(Path.String,6 ,false,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(9, ZLog::FAIL, Err); return(false); }
           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/gui_tiles_1_3.bmp");          Result = TextureManager.LoadBMPTexture(Path.String,7 ,false,false,64);     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(10,ZLog::FAIL, Err); return(false); }
           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/dialog_background_1_2.bmp");  Result = TextureManager.LoadBMPTexture(Path.String,8 ,false,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(11,ZLog::FAIL, Err); return(false); }
           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/dialog_voxeltype_1_1.bmp");   Result = TextureManager.LoadBMPTexture(Path.String,9 ,false,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(12,ZLog::FAIL, Err); return(false); }
           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/button_1_5.bmp");             Result = TextureManager.LoadBMPTexture(Path.String,10,false,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(13,ZLog::FAIL, Err); return(false); }
  if (Ol) {Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/tile_2_2.bmp");               Result = TextureManager.LoadBMPTexture(Path.String,11,false,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(14,ZLog::FAIL, Err); return(false); }}
  else    {Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/slot_inactive.bmp");          Result = TextureManager.LoadBMPTexture(Path.String,11,!Pi,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(14,ZLog::FAIL, Err); return(false); }}
  if (Ol) {Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/tile_3_1.bmp");               Result = TextureManager.LoadBMPTexture(Path.String,12,false,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(15,ZLog::FAIL, Err); return(false); }}
  else    {Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/slot_active.bmp");            Result = TextureManager.LoadBMPTexture(Path.String,12,!Pi,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(15,ZLog::FAIL, Err); return(false); }}
  if (Hs) {Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/contribute_1_1.bmp");         Result = TextureManager.LoadBMPTexture(Path.String,13,false,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(16,ZLog::FAIL, Err); return(false); }}
           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/background_black.bmp");       Result = TextureManager.LoadBMPTexture(Path.String,14,false,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(17,ZLog::FAIL, Err); return(false); }
           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/dialog_modalbox.bmp");        Result = TextureManager.LoadBMPTexture(Path.String,15,false,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(18,ZLog::FAIL, Err); return(false); }
           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/font_symbols.bmp");           Result = TextureManager.LoadBMPTexture(Path.String,16,false,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(19,ZLog::FAIL, Err); return(false); }
           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/buttonpushed.bmp");           Result = TextureManager.LoadBMPTexture(Path.String,17,false,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(20,ZLog::FAIL, Err); return(false); }
  if (!Ol){Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/universe_choice_back.bmp");   Result = TextureManager.LoadBMPTexture(Path.String,18,!Pi  ,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(20,ZLog::FAIL, Err); return(false); }}
  if (!Ol){Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/settings_background.bmp");    Result = TextureManager.LoadBMPTexture(Path.String,19,true ,true,32 );      if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(20,ZLog::FAIL, Err); return(false); }}

           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/gametype_back.bmp");          Result = TextureManager.LoadBMPTexture(Path.String,20,true ,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(20,ZLog::FAIL, Err); return(false); }
           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/BV_factory_eau_09_001.bmp");  Result = TextureManager.LoadBMPTexture(Path.String,21,!Pi ,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(20,ZLog::FAIL, Err); return(false); }
#ifdef COMPILEOPTION_ONLYSCHOOLMODE
           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/BV_grands_arbre2_06_001.bmp");Result = TextureManager.LoadBMPTexture(Path.String,22,!Pi  ,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(20,ZLog::FAIL, Err); return(false); }
#else
           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/BV_grands_arbre_06_001.bmp");Result = TextureManager.LoadBMPTexture(Path.String,22,!Pi  ,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(20,ZLog::FAIL, Err); return(false); }
#endif
           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/BV_schoolmode1_04_001.bmp");  Result = TextureManager.LoadBMPTexture(Path.String,23,!Pi  ,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(20,ZLog::FAIL, Err); return(false); }
           Path = COMPILEOPTION_DATAFILESPATH; Path.AddToPath("gui/BV_schoolmode2_11_001.bmp");  Result = TextureManager.LoadBMPTexture(Path.String,24,!Pi  ,true,32 );     if(!Result) { Err.Clear() << ErrMsg << Path; InitLog->Log(20,ZLog::FAIL, Err); return(false); }



  Initialized_TextureManager = true;
  InitLog->Log(2, ZLog::INFO, "Ended Ok : Texture Manager Init");
  return(true);
}

bool ZGame::Cleanup_TextureManager(ZLog * InitLog)
{
  InitLog->Log(0,ZLog::INFO, "Cleanup : Texture Manager Init");
  return(true);
}

bool ZGame::Init_EventManager(ZLog * InitLog)
{
  InitLog->Log(1,ZLog::INFO, "Starting : Event Manager Init");
  if (!Initialized_SDL) { ZString Err; Err = "*** ERROR :Can't init EventManager : SDL init not completed"; InitLog->Log(3, ZLog::FAIL, Err); return(false);  }

  Initialized_EventManager = true;
  InitLog->Log(2,ZLog::INFO, "Ended Ok : Event Manager Init");
  return(true);
}

bool ZGame::Init_GuiManager(ZLog * InitLog)
{
  ZString Err;
  InitLog->Log(1, ZLog::INFO, "Starting : GuiManager Init");
  if (!Initialized_GraphicMode)    {Err = "Can't init GuiManager : GraphicMode init not completed";    InitLog->Log(3, ZLog::FAIL, Err); return(false);}
  if (!Initialized_TextureManager) {Err = "Can't init GuiManager : TextureManager init not completed"; InitLog->Log(4, ZLog::FAIL, Err); return(false);}
  if (!Initialized_EventManager)   {Err = "Can't init GuiManager : EventManager init not completed";   InitLog->Log(5, ZLog::FAIL, Err); return(false);}

  GuiManager.SetTextureManager(&TextureManager);
  GuiManager.SetEventManager(&EventManager);
  GuiManager.SetScreenDimensions(0,0,ScreenResolution.x,ScreenResolution.y);
  EventManager.AddConsumer_ToTail(&GuiManager);

  Initialized_GuiManager = true;
  InitLog->Log(2, ZLog::INFO, "Ended Ok : GuiManager Init");
  return(true);
}



bool ZGame::Init_TileSetsAndFonts(ZLog * InitLog)
{


  InitLog->Log(1, ZLog::INFO, "Starting : Tilesets and Fonts Init");
  if (!Initialized_TextureManager) { ZString Err = "Can't init TileSets : TextureManager init not completed\n"; InitLog->Log(3, ZLog::FAIL, Err); return(false);}

  TileSetStyles = new ZTileSetStyles;

  // Tile 1 : Font 1 Little Old

  Font_1 = new ZTileSet;
  Font_1->SetTextureManager(&TextureManager);
  Font_1->SetTextureNum(3);
  Font_1->SetTextureSize(128,128);
  Font_1->SetTileSlotSize(8,8);
  Font_1->SetTileSize(8,8);
  Font_1->SetTileOffset(0,0);
  Font_1->SetTilesPerLine(16);
  Font_1->ComputeTileCoords();

  // Tile : Font Symbols

  Font_Symbols = new ZTileSet;
  Font_Symbols->SetTextureManager(&TextureManager);
  Font_Symbols->SetTextureNum(16);
  Font_Symbols->SetTextureSize(128,128);
  Font_Symbols->SetTileSlotSize(8,8);
  Font_Symbols->SetTileSize(8,8);
  Font_Symbols->SetTileOffset(0,0);
  Font_Symbols->SetTilesPerLine(16);
  Font_Symbols->ComputeTileCoords();

  // Tile 2 Gui Tileset

  GuiTileset = new ZTileSet;
  GuiTileset->SetTextureManager(&TextureManager);
  GuiTileset->SetTextureNum(7);
  GuiTileset->SetTextureSize(512,512);
  GuiTileset->SetTileSlotSize(32,32);
  GuiTileset->SetTileSize(32,32);
  GuiTileset->SetTileOffset(0,0);
  GuiTileset->SetTilesPerLine(16);
  GuiTileset->ComputeTileCoords();

  // Font Styles

  TileSetStyles->CreateStyle(0,Font_1,1.0,1.0,0.0,0.0);
  TileSetStyles->CreateStyle(1,Font_1,4.0,4.0,0.0,0.0);
  TileSetStyles->CreateStyle(2,Font_1,5.0,4.0,0.0,0.0);
  TileSetStyles->CreateStyle(3,Font_1,2.0,2.0,0.0,0.0);
  TileSetStyles->CreateStyle(4,Font_1,3.0,3.0,0.0,0.0);

  // Variable Size Font style

  double Fx,Fy;
  Fx = ((double)HardwareResolution.x)/1920.0;
  Fy = ((double)HardwareResolution.y)/1080.0;

  TileSetStyles->CreateStyle( 5,Font_1, Fx * 1.0, Fy * 1.0 ,0.0,0.0);
  TileSetStyles->CreateStyle( 6,Font_1, Fx * 2.0, Fy * 2.0 ,0.0,0.0);
  TileSetStyles->CreateStyle( 7,Font_1, Fx * 3.0, Fy * 3.0 ,0.0,0.0);
  TileSetStyles->CreateStyle( 8,Font_1, Fx * 4.0, Fy * 4.0 ,0.0,0.0);
  TileSetStyles->CreateStyle( 9,Font_1, Fx * 5.0, Fy * 5.0 ,0.0,0.0);
  TileSetStyles->CreateStyle(10,Font_1, Fx * 6.0, Fy * 6.0 ,0.0,0.0);
  TileSetStyles->CreateStyle(11,Font_1, Fx * 7.0, Fy * 7.0 ,0.0,0.0);
  TileSetStyles->CreateStyle(12,Font_1, Fx * 8.0, Fy * 8.0 ,0.0,0.0);
  TileSetStyles->CreateStyle(13,Font_1, Fx * 9.0, Fy * 9.0 ,0.0,0.0);
  TileSetStyles->CreateStyle(14,Font_1, Fx *10.0, Fy *10.0 ,0.0,0.0);

  Initialized_TileSetsAndFonts = true;
  InitLog->Log(2, ZLog::INFO, "Ended Ok : Tilesets and Fonts Init");
  return(true);
}



bool ZGame::Cleanup_GuiManager(ZLog * InitLog)
{
  InitLog->Log(0, ZLog::INFO, "Cleanup : GUIManager");
  return(true);
}

bool ZGame::Init_OpenGLGameSettings(ZLog * InitLog)
{
  InitLog->Log(1, ZLog::INFO, "Starting : OpenGL Game Settings");
  if (!Initialized_GraphicMode)  { ZString Err = "Can't init OpenGLGameSettings : GraphicMode init not completed"; InitLog->Log(3, ZLog::FAIL, Err); return(false); }
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LINE_SMOOTH);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glEnable (GL_BLEND);
  glEnable(GL_ALPHA_TEST);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glAlphaFunc(GL_GREATER, 0.05);

  Initialized_OpenGLGameSettings = true;
  InitLog->Log(2, ZLog::INFO, "Ended Ok : OpenGL Game Settings");
  return(true);
}

bool ZGame::Cleanup_OpenGLGameSettings(ZLog * InitLog)
{
  InitLog->Log(0, ZLog::INFO, "Cleanup : OpenGL Game Settings");
  Initialized_OpenGLGameSettings = false;
  return(true);
}

bool ZGame::Init_Glew(ZLog * InitLog)
{
  GLenum err;

  InitLog->Log(1, ZLog::INFO, "Starting : Glew");
  if (!Initialized_GraphicMode)    {ZString Err = "Can't init Glew : GraphicMode init not completed"; InitLog->Log(3, ZLog::FAIL, Err); return(false);}
  if ((err = glewInit()) != GLEW_OK)
  {
    printf("Can't init Glew : %s\n", glewGetErrorString(err));
    return(false);
  }

  InitLog->Log(0, ZLog::INFO, "Ended Ok : Glew");
  Initialized_Glew = true;
  return(true);
}

bool ZGame::Cleanup_Glew(ZLog * InitLog)
{
  InitLog->Log(0, ZLog::INFO, "Cleanup : Glew");
  Initialized_Glew = false;
  return(true);
}

bool ZGame::Init_LoadingScreen(ZLog * InitLog)
{
  InitLog->Log(1, ZLog::INFO, "Starting : LoadingScreen");
  ZLoadingScreen::Display(HardwareResolution.x, HardwareResolution.y);
  InitLog->Log(0, ZLog::INFO, "Ended Ok : LoadingScreen");
  Initialized_LoadingScreen = true;
  return(true);
}

bool ZGame::Cleanup_LoadingScreen(ZLog * InitLog)
{
  InitLog->Log(0, ZLog::INFO, "Cleanup : LoadingScreen");
  Initialized_LoadingScreen = false;
  return(true);
}

bool ZGame::Init_VoxelTypeManager(ZLog * InitLog)
{
  ZString Msg;
  InitLog->Log(1, ZLog::INFO, "Starting : VoxelTypeManager Init");
  if (!Initialized_GraphicMode)    {ZString Err ="Can't init VoxelTypeManager : GraphicMode init not completed"; InitLog->Log(3, ZLog::FAIL, Err); return(false);}
  if (!Initialized_Glew)           {ZString Err ="Can't init VoxelTypeManager : Glew init not completed"; InitLog->Log(4, ZLog::FAIL, Err); return(false);}
  VoxelTypeManager.SetGameEnv(this);
  if (!VoxelTypeManager.LoadVoxelTypes()) { ZString Err ="Can't init VoxelTypeManager."; InitLog->Log(5, ZLog::FAIL, Err); return(false); }
  Msg.Clear() << "Loaded " << VoxelTypeManager.GetTexturesCount() << " Voxel Textures."; InitLog->Log(6, ZLog::INFO, Msg);
  if (VoxelTypeManager.GetTexturesCount() < 212) { ZString Err; Err << "Missing Texture files (count : " << VoxelTypeManager.GetTexturesCount()<< ")"; InitLog->Log(7, ZLog::FAIL, Err); return(false); }
  if ( 0 != VoxelTypeManager.GetVoxelType(50) )  VoxelTypeManager.FillZeroSlots(50);
  else                                           return(false);
  Initialized_VoxelTypeManager = true;
  InitLog->Log(2, ZLog::INFO, "Ended Ok : VoxelTypeManager Init");
  return(true);
}

bool ZGame::Cleanup_VoxelTypeManager(ZLog * InitLog)
{
  InitLog->Log(0, ZLog::INFO, "Cleanup : VoxelTypeManager Init");
  Initialized_VoxelTypeManager = false;
  return(true);
}

bool ZGame::Cleanup_EventManager(ZLog * InitLog)
{
  InitLog->Log(0, ZLog::INFO, "Cleanup : EventManager Init");
  Initialized_EventManager = false;
  return(true);
}

bool ZGame::Cleanup_TileSetsAndFonts(ZLog * InitLog)
{
  InitLog->Log(0, ZLog::INFO, "Cleanup : Tilesets and Fonts Init");
  if (Font_1) {delete Font_1; Font_1=0;}
  if (TileSetStyles) {delete TileSetStyles; TileSetStyles = 0;}

  Initialized_TileSetsAndFonts = false;
  return(true);
}


bool ZGame::Start_Game_Events()
{
  Game_Events = new ZGame_Events;

  if (!Game_Events) return(false);

  Game_Events->SetEventManager(&EventManager);
  Game_Events->SetGameEnv(this);

  EventManager.AddConsumer_ToTail(Game_Events);

  Initialized_Game_Events = true;
  return(true);
}

bool ZGame::End_Game_Events()
{
  if (Initialized_Game_Events) EventManager.RemoveConsumer(Game_Events);
  if (Game_Events) delete Game_Events;
  Game_Events = 0;
  Initialized_Game_Events = false;

  return(true);
}

bool ZGame::Init_Renderer(ZLog * InitLog)
{
  InitLog->Log(1, ZLog::INFO, "Starting : Renderer Init");
  //ZRender_Basic Basic_Renderer;

  Basic_Renderer = new ZRender_Basic;

  Basic_Renderer->SetGameEnv(this);
  Basic_Renderer->Init();
  //Basic_Renderer.SetCamera(&Player);
  Basic_Renderer->SetVoxelTypeManager( &VoxelTypeManager);
  Basic_Renderer->SetTextureManager(&TextureManager);
  Basic_Renderer->LoadVoxelTexturesToGPU();
  Basic_Renderer->LoadTexturesToGPU();

  Initialized_Renderer = true;
  InitLog->Log(2, ZLog::INFO, "Ended Ok : Renderer Init");
  return(true);
}

bool ZGame::Cleanup_Renderer(ZLog * InitLog)
{
  InitLog->Log(0, ZLog::INFO, "Cleanup : Renderer Init");
  if (Basic_Renderer) delete Basic_Renderer;
  Basic_Renderer = 0;
  Initialized_Renderer = false;
  return(true);
}

bool ZGame::Init_Sound(ZLog * InitLog)
{
  ZString Msg;
  InitLog->Log(1, ZLog::INFO, "Starting : Sound Init");

  // New Sound Object

  if (!(Sound = new ZSound)) return(false);

  // Load the sound sample files.

  Sound->LoadSoundFiles();
  Msg.Clear() << "Loaded " << Sound->GetSampleCount() << " Sound samples."; InitLog->Log(3, ZLog::INFO, Msg);
  if (Sound->GetSampleCount() < 8) { ZString Err; Err << "Missing Sound Sample Files (count : " << Sound->GetSampleCount()<< ")"; InitLog->Log(4, ZLog::FAIL, Err); return(false); }

  // Get the volume from settings.

  double Vol = Settings_Hardware->Setting_SoundVolume/100.0;

  // Adjust sample volume. This is precomputed to reduce overhead.

  for (ULong i=0; i < Sound->GetSampleCount() ; i++)
  {
    switch (i)
    {
      case 2:    Sound->SampleModify_Volume(2,0.5 * Vol);  break;
      case 3:    Sound->SampleModify_Volume(3,0.5 * Vol);  break;
      case 4:    Sound->SampleModify_Volume(4,0.5 * Vol);  break;
      case 5:    Sound->SampleModify_Volume(5,0.03* Vol);  break; // Vrilleuse d'oreilles (0.1)
      case 6:    Sound->SampleModify_Volume(6,0.3 * Vol);  break; // Bloc Break (0.3)
      case 7:    Sound->SampleModify_Volume(7,0.3 * Vol);  break; // Bloc Place (0.3)
      default:   if (Vol<1.0) Sound->SampleModify_Volume(1,1.0 * Vol);  break;
    }
  }

  // Activate sound depending on settings.

  Sound->Activate(Settings_Hardware->Setting_SoundEnabled);


  Initialized_Sound = true;
  InitLog->Log(2, ZLog::INFO, "Ended Ok : Sound Init");
  return(true);
}

bool ZGame::Cleanup_Sound(ZLog * InitLog)
{
  InitLog->Log(0, ZLog::INFO, "Cleanup : Sound Init");
  Sound->EndAudio();
  if (Sound) delete Sound;
  Sound = 0;
  Initialized_Sound = false;
  return(true);
}

bool ZGame::Start_Game_Stats()
{
  GameStat = new ZGameStat();
  if (!GameStat) { Initialized_GameStats = false; return(false); }

  return( Initialized_GameStats = GameStat->Start() );
}

bool ZGame::End_Game_Stats()
{
  if (GameStat)
  {
    GameStat->End();
    delete GameStat;
    Initialized_GameStats = false;
    return(true);
  }
  return(false);
}

bool ZGame::Start_GameEventSequencer()
{
  GameEventSequencer = new ZGameEventSequencer();

  // GameEventSequencer->AddEvent(30000,10000,1,true, 600000);

  return(Initialized_GameEventSequencer = GameEventSequencer ? true : false);
}

bool ZGame::End_GameEventSequencer()
{
  if ( GameEventSequencer ) { delete GameEventSequencer; GameEventSequencer = 0; }
  return(true);
}

bool ZGame::Start_WorldInfo()
{
  // Create the WorldInfo

  if ((WorldInfo)) delete WorldInfo;
  WorldInfo = new ZWorldInfo;



  // Load WorldInfo if exists

  WorldInfo->Load(Path_ActualUniverse.Path("WorldInfo.dat").String);

  Initialized_WorldInfo = true;
  return(Initialized_WorldInfo);
}

bool ZGame::End_WorldInfo()
{
  bool Result;

  Result = true;
  Initialized_WorldInfo = false;
  if ((WorldInfo))
  {
#if COMPILEOPTION_ALLOWSAVE == 1
    Result = WorldInfo->Save(Path_ActualUniverse.Path("WorldInfo.dat").String);
#endif
    delete WorldInfo;
    WorldInfo = 0;
    return Result;
  }
  return(false);
}

bool ZGame::Start_World()
{
  World = new ZVoxelWorld;
  if (!World) return(false);

  World->SetUniverseNum(UniverseNum);
  World->SetVoxelTypeManager(&VoxelTypeManager);
  World->CreateDemoWorld();
  World->SetVoxel(0,0,0,1);// World.SetVoxel(0,1,0,1); World.SetVoxel(1,0,0,1); World.SetVoxel(0,2,0,2); World.SetVoxel(0,2,1,2);
  //World.SetVoxel(0,2,-1,2);World.SetVoxel(10,0,10,1); World.SetVoxel(10,1,10,1); World.SetVoxel(10,2,10,1); World.SetVoxel(5,3,0,1); World.SetVoxel(0,3,5,2);
  World->Load();
  // GameEnv.VoxelTypeManager.DumpInfos();
  World->WorldUpdateFaceCulling();


  Initialized_World = true;
  return(true);
}

bool ZGame::End_World()
{
  if (World) delete World;
  World = 0;
  Initialized_World = false;
  return(true);
}

bool ZGame::Start_PhysicEngine()
{
  ZVector3d InitialLocation;
  ZActor_Player  * ActorPlayer;

  PhysicEngine = new ZActorPhysicEngine;

  InitialLocation.x = 425.0;
  InitialLocation.y = 0.0;
  InitialLocation.z = 1975.0;
  ActorPlayer = new ZActor_Player;

  ActorPlayer->SetPosition(InitialLocation);
  ActorPlayer->SetGameEnv(this);
  PhysicEngine->SetWorld(World);
  PhysicEngine->AddActor(ActorPlayer);
  PhysicEngine->SelectActor(ActorPlayer);
  PhysicEngine->SetVoxelTypeManager(&VoxelTypeManager);
  PhysicEngine->SetToolManager(ToolManager);

  // Load actor if save exists

  ZStream_SpecialRamStream Stream;
  ZStream_File FileStream;
  ZString FileName, UNum;

  // Load Physic engine actors

  if (COMPILEOPTION_USEHOMEDIRSTORAGE)
  {
    FileName = ZStream_File::Get_Directory_UserData();
  }
  FileName.AddToPath(COMPILEOPTION_SAVEFOLDERNAME);
  UNum = UniverseNum;
  FileName.AddToPath("Universes").AddToPath(UNum).AddToPath("PlayerInfo.dat");
  FileStream.SetFileName(FileName.String);
  if (FileStream.OpenRead())
  {
    Stream.SetStream(&FileStream);
    Stream.OpenRead();

    if (ActorPlayer->Load(&Stream)) printf("Player Loaded Ok\n");
    else                            printf("Player Loading error\n");
    Stream.Close();
    FileStream.Close();
  } printf("Player created\n");


  // Inventory Loading
  /*
  char Buffer[1024];
  sprintf(Buffer, "Universes/%ld/PlayerInfo.dat", UniverseNum);
  ActorPlayer->Inventory->Load(Buffer);
*/

  Initialized_PhysicEngine = true;
  return(true);
}

bool ZGame::End_PhysicEngine()
{
  ZStream_SpecialRamStream Stream;
  ZStream_File FileStream;
  ZString FileName, UNum;

  // Save Physic engine actors

  if (COMPILEOPTION_USEHOMEDIRSTORAGE)
  {
    FileName = ZStream_File::Get_Directory_UserData();
  }
  FileName.AddToPath(COMPILEOPTION_SAVEFOLDERNAME);
  UNum = UniverseNum;
#if COMPILEOPTION_ALLOWSAVEPLAYERSTATE == 1
  FileName.AddToPath("Universes").AddToPath(UNum).AddToPath("PlayerInfo.dat");
  FileStream.SetFileName(FileName.String);
  if (FileStream.OpenWrite())
  {
    Stream.SetStream(&FileStream);

    PhysicEngine->GetSelectedActor()->Save(&Stream);
    Stream.FlushBuffer();
    Stream.Close();
    FileStream.Close();
  }
#endif
/*
  // Inventory Saving
  char Buffer[1024];
  sprintf(Buffer, "%s/%ld/PlayerInfo.dat", UniverseNum);
  PhysicEngine->GetSelectedActor()->Inventory->Save(Buffer);
*/


  if (PhysicEngine) delete PhysicEngine;
  PhysicEngine = 0;
  Initialized_PhysicEngine = false;
  return(true);
}

bool ZGame::Start_SectorLoader()
{
  SectorLoader = new ZFileSectorLoader;
  if (!SectorLoader) return(false);
  SectorLoader->SetVoxelTypeManager(&VoxelTypeManager);
  SectorLoader->SetUniverseNum(UniverseNum);
  SectorLoader->SetWorldType(GameInfo.GameType);
  if (!SectorLoader->Init()) return(false);
  World->SetSectorLoader(SectorLoader);
  Initialized_SectorLoader = true;
  return(true);
}

bool ZGame::End_SectorLoader()
{
  if (SectorLoader)
  {
    SectorLoader->Cleanup();
    delete SectorLoader;
    Initialized_SectorLoader = false;
    SectorLoader = 0;
  }
  return(true);
}

bool ZGame::Start_VoxelProcessor()
{
  // Compute sector ejection distance.

  double EjectionDistance, h,v;

  h = (double)Settings_Hardware->RenderingDistance_Horizontal;
  v = (double)Settings_Hardware->RenderingDistance_Vertical;
  EjectionDistance = sqrt( (h*h) + (h*h) + (v*v) ) * 1.5;

  // Init the voxel processor

  VoxelProcessor = new ZVoxelProcessor;

  if (!VoxelProcessor) return(false);

  VoxelProcessor->SetWorld(World);
  VoxelProcessor->SetPlayerPosition(0,0,0);
  VoxelProcessor->SetSectorEjectDistance(EjectionDistance);
  VoxelProcessor->SetGameEnv(this);
  VoxelProcessor->Start();

  Initialized_VoxelProcessor = true;
  return(true);
}

bool ZGame::End_VoxelProcessor()
{
  if (VoxelProcessor)
  {
    VoxelProcessor->End();
    delete VoxelProcessor;
    Initialized_VoxelProcessor = false;
    VoxelProcessor = 0;
  }
  return(true);
}

bool ZGame::Start_RendererSettings()
{
  Basic_Renderer->SetRenderSectorRadius(Settings_Hardware->RenderingDistance_Horizontal,Settings_Hardware->RenderingDistance_Vertical);
  Basic_Renderer->SetWorld(World);
  Basic_Renderer->SetCamera(&PhysicEngine->GetSelectedActor()->Camera);
  Basic_Renderer->SetPointedVoxel(&((ZActor_Player *)PhysicEngine->GetSelectedActor())->PointedVoxel);
  Basic_Renderer->SetViewportResolution(ScreenResolution);
  Basic_Renderer->SetPixelAspectRatio(Settings_Hardware->PixelAspectRatio);
  Basic_Renderer->SetSectorCullingOptimisationFactor(Settings_Hardware->Opt_SectCFactor);
  Initialized_RendererSettings = true;
  return(true);
}

bool ZGame::End_RendererSettings()
{
  Basic_Renderer->SetWorld(0);
  Basic_Renderer->SetCamera(0);
  Basic_Renderer->SetPointedVoxel(0);
  Initialized_RendererSettings = false;
  return(false);
}

bool ZGame::Start_GameWindows()
{
  GameWindow_Inventory   = new ZGameWindow_Inventory;     GameWindow_Inventory->SetGameEnv(this);
  VoxelTypeBar           = new ZGameWindow_VoxelTypeBar;  VoxelTypeBar->SetGameEnv(this);
  GameWindow_Storage     = new ZGameWindow_Storage;       GameWindow_Storage->SetGameEnv(this);
  GameWindow_Programmable= new ZGameWindow_Programmable;  GameWindow_Programmable->SetGameEnv(this);
  GameWindow_ProgRobot_Asm=new ZGameWindow_ProgRobot_Asm; GameWindow_ProgRobot_Asm->SetGameEnv(this);
  GameWindow_ProgRobot_Remote=new ZGameWindow_ProgRobot_Remote; GameWindow_ProgRobot_Remote->SetGameEnv(this);
  GameProgressBar        = new ZGameWindow_ProgressBar;   GameProgressBar->SetGameEnv(this);
  GameWindow_Advertising = new ZGameWindow_Advertising;   GameWindow_Advertising->SetGameEnv(this);
  GameWindow_UserTextureTransformer = new ZGameWindow_UserTextureTransformer; GameWindow_UserTextureTransformer->SetGameEnv(this);
  GameWindow_DisplayInfos= new ZGameWindow_DisplayInfos;  GameWindow_DisplayInfos->SetGameEnv(this);
  GameWindow_Sequencer   = new ZGameWindow_Sequencer;     GameWindow_Sequencer->SetGameEnv(this);
  GameWindow_AsmDebug    = new ZGameWindow_AsmDebug;      GameWindow_AsmDebug->SetGameEnv(this);
  GameWindow_AsmHardware = new ZGameWindow_AsmHardware;   GameWindow_AsmHardware->SetGameEnv(this);
  GameWindow_AsmExtendedRegisters = new ZGameWindow_AsmExtendedRegisters; GameWindow_AsmExtendedRegisters->SetGameEnv(this);
  GameWindow_Compilation_Result = new ZGameWindow_Compilation_Result;     GameWindow_Compilation_Result->SetGameEnv(this);
  GameWindow_ResumeRequest = new ZGameWindow_ResumeRequest;               GameWindow_ResumeRequest->SetGameEnv(this);
  GameWindow_ResumeRequest_Little = new ZGameWindow_ResumeRequest_Little; GameWindow_ResumeRequest_Little->SetGameEnv(this);
  GameWindow_SPS           = new ZGameWindow_SPS;         GameWindow_SPS->SetGameEnv(this);
  GameWindow_Scan          = new ZGameWindow_Scan;        GameWindow_Scan->SetGameEnv(this);
  GameWindow_RTFM          = new ZGameWindow_RTFM;        GameWindow_RTFM->SetGameEnv(this);
  GameWindow_Advertising->Show();

  Initialized_GameWindows = true;
  return(true);
}

bool ZGame::End_GameWindows()
{
  GuiManager.RemoveAllFrames();

  if (VoxelTypeBar)       {delete VoxelTypeBar; VoxelTypeBar = 0; }
  if (GameWindow_Storage) {delete GameWindow_Storage; GameWindow_Storage = 0; }
  if (GameWindow_UserTextureTransformer) { delete GameWindow_UserTextureTransformer; GameWindow_UserTextureTransformer = 0; }
  if (GameWindow_Inventory) { delete GameWindow_Inventory; GameWindow_Inventory = 0; }
  if (GameWindow_ResumeRequest_Little) {delete GameWindow_ResumeRequest_Little; GameWindow_ResumeRequest_Little = 0;}
  if (GameProgressBar)      {delete GameProgressBar; GameProgressBar=0;}
  if (GameWindow_SPS)       {delete GameWindow_SPS; GameWindow_SPS = 0;}
  if (GameWindow_Scan)      {delete GameWindow_Scan; GameWindow_Scan = 0;}
  if (GameWindow_ProgRobot_Remote) {delete GameWindow_ProgRobot_Remote; GameWindow_ProgRobot_Remote = 0;}

  Initialized_GameWindows = false;
  return(true);
}

bool ZGame::Start_ToolManager()
{
  ToolManager = new ZToolManager; if (!ToolManager) return(false);
  ToolManager->SetGameEnv(this);

  // Adding Tools
  ToolManager->AddTool(42, new ZTool_Constructor);
  ToolManager->AddTool(76, new ZTool_Constructor_P2);
  ToolManager->AddTool(77, new ZTool_Constructor_P3);
  ToolManager->AddTool(78, new ZTool_Constructor_P4);
  ToolManager->AddTool(79, new ZTool_Constructor_P5);
  ToolManager->AddTool(242,new ZTool_Scan);
  ToolManager->AddTool(243,new ZTool_Rotate);
  ToolManager->AddTool(251,new ZTool_OreScan);
  ToolManager->AddTool(253,new ZTool_Linker);

  Initialized_ToolManager = true;
  return(true);
}

bool ZGame::End_ToolManager()
{
  if (ToolManager) delete ToolManager;

  Initialized_ToolManager = false;

  return(true);
}

bool ZGame::LoadGameInfo()
{
  // Init the path to actual universe

    Path_ActualUniverse = Path_Universes; Path_ActualUniverse.AddToPath(UniverseNum);
    Path_UserScripts    = Path_ActualUniverse; Path_UserScripts.AddToPath("Scripts");
    Path_UserScripts_Squirrel = Path_UserScripts; Path_UserScripts_Squirrel.AddToPath("Squirrel");
    Path_UserScripts_Asm_1    = Path_UserScripts; Path_UserScripts_Asm_1.AddToPath("Asm_1");
    Path_UserScripts_UserData= Path_UserScripts; Path_UserScripts_UserData.AddToPath("UserData");

    // Create the directory for this universe if not exists.

    ZStream_File::Directory_CreateIfNotExists(Path_ActualUniverse.String);
    ZStream_File::Directory_CreateIfNotExists(Path_UserScripts.String);
    ZStream_File::Directory_CreateIfNotExists(Path_UserScripts_Squirrel.String);
    ZStream_File::Directory_CreateIfNotExists(Path_UserScripts_Asm_1.String);
    ZStream_File::Directory_CreateIfNotExists(Path_UserScripts_UserData.String);

    // Loadind the worldinfo

    return(GameInfo.Load(Path_ActualUniverse.Path("WorldInfo.dat").String));

}

bool ZGame::SaveGameInfo()
{
  GameInfo.Save(Path_ActualUniverse.Path("WorldInfo.dat").String);

  /*
  ZStream_File Stream;
  ZString FileSpec;

  Stream.SetFileName(Path_ActualUniverse.Path("WorldInfo.dat").String);

  if (!Stream.OpenWrite()) return(false);

  Stream<<(ULong)0xB14C08E1;
  Stream<<(ULong)COMPILEOPTION_VERSIONNUM; // Game Version
  Stream<<(ULong)2;                        // Compatibility class;
  Stream<<Machine_Serial; // Next serial number for machines, robot, etc.
  Stream.Close();
  */

  return(true);
}

void ZGame::InitGameSession()
{
  GameInfo.GameType = 1;
  GameInfo.RobotNextSerial = 1;
  GameInfo.Previous_GameVersion = 0;

  Stop_Programmable_Robots = false;
  Service_RemoteRobotServerStarted = false;
  Enable_MVI = true;
  Enable_LoadNewSector = true;
  Enable_NewSectorRendering = true;
}

bool ZGame::OutputHelperFiles()
{
  bool Ok;

  Ok = true;
  Ok &= ZGame_DevHelpers::OutputHelpers(Path_HelperFilesOut);

  return(Ok);
}
