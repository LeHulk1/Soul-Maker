#ifndef __SELECTEDTILE_H__
#define __SELECTEDTILE_H__


#include <windows.h>

#include "MapData.h"


class SelectedTile16 {
public:
    SelectedTile16();
    ~SelectedTile16();
    void FillTileInfo(int aScreenX, int aScreenY, int aX, int aY);
    int  AbsX(void);
    int  AbsY(void);

    int ScreenX;
    int ScreenY;
    int X;
    int Y;
};

class TileSelection {
public:
    TileSelection();
    ~TileSelection();
    int TopLeftTileX(void);
    int TopLeftTileY(void);
    int BottomRightTileX(void);
    int BottomRightTileY(void);
    int SelectionWidth(void);
    int SelectionHeight(void);
    int GetTilesetX(void);
    int GetSelectedTileID(void);
    void SetTilesetX(int aTilesetX);
    void SetTile_i(int aScreenX, int aScreenY, int aX, int aY);
    void SetTile_j(int aScreenX, int aScreenY, int aX, int aY);
    void SetTileID(int TileID);
    void PasteSelectionTiles(MapData &aMapData, int AbsTileX, int AbsTileY);
    void RedrawNewTiles(HWND hwnd, int AbsTileX, int AbsTileY);
    void RedrawSelection(HWND hwnd);

    bool SelectionExists;
    bool SelectionInTileset;

private:
    SelectedTile16 SelectedTile_i;
    SelectedTile16 SelectedTile_j;
    int SelectedTileID;
    int TilesetX;
};

#endif // __SELECTEDTILE_H__
