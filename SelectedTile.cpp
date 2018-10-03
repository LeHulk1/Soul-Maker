#include "SelectedTile.h"


/*** SelectedTile16 implementation ***/

SelectedTile16::SelectedTile16() {
    ScreenX = 0; ScreenY = 0; X = 0; Y = 0;
}

SelectedTile16::~SelectedTile16() {}

void SelectedTile16::FillTileInfo(int aScreenX, int aScreenY, int aX, int aY) {
    ScreenX = aScreenX; ScreenY = aScreenY; X = aX; Y = aY;
}

int SelectedTile16::AbsX() {
    return 16*ScreenX + X;
}

int SelectedTile16::AbsY() {
    return 16*ScreenY + Y;
}


/*** TileSelection implementation ***/

TileSelection::TileSelection() {
    SelectedTileID = 0;
    TilesetX = 0;
    SelectionExists = false;
    SelectionInTileset = false;
}

TileSelection::~TileSelection() {}

int TileSelection::TopLeftTileX()     {return (SelectedTile_i.AbsX() < SelectedTile_j.AbsX() ? SelectedTile_i.AbsX() : SelectedTile_j.AbsX());}
int TileSelection::TopLeftTileY()     {return (SelectedTile_i.AbsY() < SelectedTile_j.AbsY() ? SelectedTile_i.AbsY() : SelectedTile_j.AbsY());}
int TileSelection::BottomRightTileX() {return (SelectedTile_i.AbsX() > SelectedTile_j.AbsX() ? SelectedTile_i.AbsX() : SelectedTile_j.AbsX());}
int TileSelection::BottomRightTileY() {return (SelectedTile_i.AbsY() > SelectedTile_j.AbsY() ? SelectedTile_i.AbsY() : SelectedTile_j.AbsY());}
int TileSelection::SelectionWidth()   {return (SelectionInTileset ? 1 : BottomRightTileX() - TopLeftTileX() + 1);}
int TileSelection::SelectionHeight()  {return (SelectionInTileset ? 1 : BottomRightTileY() - TopLeftTileY() + 1);}

int TileSelection::GetTilesetX(void) {
    return TilesetX;
}

int TileSelection::GetSelectedTileID(void) {
    return SelectedTileID;
}

void TileSelection::SetTilesetX(int aTilesetX) {
    TilesetX = aTilesetX;
}

void TileSelection::SetTile_i(int aScreenX, int aScreenY, int aX, int aY) {
    SelectedTile_i.FillTileInfo(aScreenX, aScreenY, aX, aY);
}
void TileSelection::SetTile_j(int aScreenX, int aScreenY, int aX, int aY) {
    SelectedTile_j.FillTileInfo(aScreenX, aScreenY, aX, aY);
}
void TileSelection::SetTileID(int TileID) {
    SelectedTileID = TileID;
}


void TileSelection::PasteSelectionTiles(MapData &aMapData, int AbsTileX, int AbsTileY) {

    if (SelectionInTileset) {
        /* Directly draw the tile */
        aMapData.SetTile16(AbsTileX/16, AbsTileY/16, AbsTileX%16, AbsTileY%16, SelectedTileID);
    }
    else {
        int SelectionTiles[SelectionWidth()][SelectionHeight()];
        int X, Y;

        /* Copy the selected tiles... */
        for (X = 0; X < SelectionWidth(); X++) {
            for (Y = 0; Y < SelectionHeight(); Y++) {
                SelectionTiles[X][Y] =
                    aMapData.GetTile16((TopLeftTileX()+X)/16, (TopLeftTileY()+Y)/16, (TopLeftTileX()+X)%16, (TopLeftTileY()+Y)%16);
            }
        }

        /* ...and paste them */
        for (X = 0; X < SelectionWidth(); X++) {
            for (Y = 0; Y < SelectionHeight(); Y++) {
                aMapData.SetTile16((AbsTileX+X)/16, (AbsTileY+Y)/16, (AbsTileX+X)%16, (AbsTileY+Y)%16,
                                   SelectionTiles[X][Y]);
            }
        }
    }
}


void TileSelection::RedrawNewTiles(HWND hwnd, int AbsTileX, int AbsTileY) {

    /* Make rectangle encapsulating the map change */
    RECT NewTilesRectangle;
    NewTilesRectangle.left = AbsTileX*16;
    NewTilesRectangle.top = AbsTileY*16;
    NewTilesRectangle.right = (AbsTileX + SelectionWidth())*16;
    NewTilesRectangle.bottom = (AbsTileY + SelectionHeight())*16;

    /* Invalidate the bitmap region corresponding to that rectangle */
    RedrawWindow(hwnd, &NewTilesRectangle, NULL, RDW_INVALIDATE);
}


void TileSelection::RedrawSelection(HWND hwnd) {

    if (SelectionExists) {

        /* Make selection rectangle */
        RECT SelectionRectangle;

        if (SelectionInTileset) {
            SelectionRectangle.left   = TilesetX + (SelectedTileID%16)*16;
            SelectionRectangle.top    = (SelectedTileID/16)*16;
            SelectionRectangle.right  = TilesetX + (SelectedTileID%16)*16 + 16;
            SelectionRectangle.bottom = (SelectedTileID/16)*16 + 16;
        }
        else {
            SelectionRectangle.left   = TopLeftTileX()*16;
            SelectionRectangle.top    = TopLeftTileY()*16;
            SelectionRectangle.right  = (BottomRightTileX() + 1)*16;
            SelectionRectangle.bottom = (BottomRightTileY() + 1)*16;
        }

        /* Invalidate the bitmap region corresponding to that rectangle */
        RedrawWindow(hwnd, &SelectionRectangle, NULL, RDW_INVALIDATE);
    }
}
