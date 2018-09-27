#ifndef __DRAWINGTOOLS_H__
#define __DRAWINGTOOLS_H__


#include "MapData.h"
#include "SelectedTile.h"

namespace DrawingTools {

    void DrawMap(MapData &aMapData, unsigned char* aBitmapBits, int BitmapWidth);
    void DrawTileset(MapData &aMapData, unsigned char* aBitmapBits, int BitmapWidth);
    void DrawSelection(int TopLeftX, int TopLeftY, int BottomRightX, int BottomRightY, unsigned char* aBitmapBits, int BitmapWidth);
}


#endif // __DRAWINGTOOLS_H__
