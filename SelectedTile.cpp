#include "SelectedTile.h"


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
