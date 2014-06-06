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
 * ZGameWindow_UserTextureTransformer.h
 *
 *  Created on: 29 févr. 2012
 *      Author: laurent
 */

#ifndef Z_ZGAMEWINDOW_USERTEXTURETRANSFORMER_H
#define Z_ZGAMEWINDOW_USERTEXTURETRANSFORMER_H

//#ifndef Z_ZGAMEWINDOW_USERTEXTURETRANSFORMER_H
//#  include "ZGameWindow_UserTextureTransformer.h"
//#endif

#ifndef Z_ZGUI_H
#  include "ZGui.h"
#endif

#ifndef Z_ZGUI_INVENTORYBOX_H
#  include "ZGui_InventoryBox.h"
#endif

#ifndef Z_ZGUI_VOXELVIEW_H
#  include "ZGui_VoxelViewBox.h"
#endif

#ifndef Z_GUI_FONTFRAME_H
#  include "ZGui_FontFrame.h"
#endif

#ifndef Z_ZGUI_PANELTEXTBUTTON_H
#  include "ZGui_PanelTextButton.h"
#endif

#ifndef Z_ZGUI_TILEFRAME_H
#  include "ZGui_TileFrame.h"
#endif

#ifndef Z_ZVOXELTYPEMANAGER_H
#  include "ZVoxelTypeManager.h"
#endif

#ifndef Z_ZVOXELEXTENSION_USERTEXTURETRANSFORMER_H
#  include "ZVoxelExtension_UserTextureTransformer.h"
#endif



class ZGame;

class ZGameWindow_UserTextureTransformer : public ZFrame
{
  protected:
    //ZFrame        MainWindow;
    ZInventoryBox IBox;
    ZInventoryBox IBox2;
    ZInventoryBox IBox3;
    ZGame * GameEnv;
    bool Flag_Shown;

    ZFrame_FontFrame MainTitle;
    ZFrame_FontFrame ActionTitle;
    ZFrame_FontFrame StorageTitle;
    ZFrame_FontFrame InventoryTitle;

    ZInventoryBox * MainStorage;

    ZTileFrame    ArrowLeft;
    ZTileFrame    ArrowRight;
    ZVoxelViewBox VoxelTypeView;
    ZPanelTextButton GetButton;

    ZString Text_MainTitle;
    ZString Text_ActionTitle;
    ZString Text_StorageTitle;
    ZString Text_InventoryTitle;
    ZString Text_Get;

    ZVoxelExtension_UserTextureTransformer * VoxelExtension_UserTextureTransformer;

    UShort i1,i2,i3;
    ULong Q1,Q2,Q3;

    // The info

  public:

  ZGameWindow_UserTextureTransformer()
  {
    MainStorage = new ZInventoryBox[128];
    Flag_Shown = false;
    Text_MainTitle = "GET USER VOXEL";
    Text_ActionTitle = "CHOOSE YOUR CUSTOM VOXEL TYPE";
    Text_StorageTitle = "WORKING ZONE : PUT MATERIALS HERE";
    Text_InventoryTitle = "INVENTORY";
    Text_Get = "GET IT";
    VoxelExtension_UserTextureTransformer = 0;
  }

  ~ZGameWindow_UserTextureTransformer()
  {
    if (MainStorage) delete [] MainStorage;
  }

  void SetGameEnv(ZGame * GameEnv) {this->GameEnv = GameEnv;}

  void SetVoxelExtension(ZVoxelExtension * Extension) { VoxelExtension_UserTextureTransformer = (ZVoxelExtension_UserTextureTransformer *)Extension; }

  void Show();
  void Hide();


  bool Is_Shown() {return(Flag_Shown);}

  // Overloaded events

  virtual Bool MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y);



};



#endif /* Z_ZGAMEWINDOW_USERTEXTURETRANSFORMER_H */
