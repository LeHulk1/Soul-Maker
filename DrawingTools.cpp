#include "DrawingTools.h"



namespace DrawingTools {

    void DrawMap(MapData &aMapData, unsigned char* aBitmapBits, int BitmapWidth) {

        int CurrentTile16, CurrentPixel, Tile16X, Tile16Y, Tile8InTile16, Palette;
        int x, y, X, Y, ScreenX, ScreenY;
        bool Hflip, Vflip;
        Tile8 CurrentTile8;

        /* Loop on all 16x16 screens and each Tile16 in those screens */
        for (ScreenX = 0; ScreenX < aMapData.NbScreensX; ScreenX++) {
            for (ScreenY = 0; ScreenY < aMapData.NbScreensY; ScreenY++) {
                for (Tile16X = 0; Tile16X < 16; Tile16X++) {
                    for (Tile16Y = 0; Tile16Y < 16; Tile16Y++) {

                        /* Get the ID of the Tile16 at this position */
                        CurrentTile16 = aMapData.GetTile16(ScreenX, ScreenY, Tile16X, Tile16Y);

                        /* Process each of the 4 Tile8s in this Tile16 */
                        for (Tile8InTile16 = 0; Tile8InTile16 < 4; Tile8InTile16++) {

                            /* Get the data corresponding to this Tile8 */
                            Hflip = aMapData.Tile16Data[CurrentTile16].HorizontalFlip[Tile8InTile16];
                            Vflip = aMapData.Tile16Data[CurrentTile16].VerticalFlip[Tile8InTile16];
                            Palette = aMapData.Tile16Data[CurrentTile16].Palette[Tile8InTile16] - 1;
                            CurrentTile8 = aMapData.Tile8Data[ aMapData.Tile16Data[CurrentTile16].Tile8Data[Tile8InTile16] ];

                            /* Loop on each pixel of this Tile8 */
                            for (x = 0; x < 8; x++) {
                                for (y = 0; y < 8; y++) {

                                    /* Get the correct color for this pixel - apply flips and palette changes if any */
                                    CurrentPixel = CurrentTile8.Pixel((Hflip?(7-x):x), (Vflip?(7-y):y)) + 0x10*Palette;

                                    /* Get the absolute coordinates of this pixel in the bitmap */
                                    X = 256*ScreenX + 16*Tile16X + x + ((Tile8InTile16 & 0x01) ? 8 : 0);
                                    Y = 256*ScreenY + 16*Tile16Y + y + ((Tile8InTile16 & 0x02) ? 8 : 0);

                                    /* Draw the pixel */
                                    aBitmapBits[(Y*BitmapWidth + X)*4]     = aMapData.PaletteData[CurrentPixel].Blue;
                                    aBitmapBits[(Y*BitmapWidth + X)*4 + 1] = aMapData.PaletteData[CurrentPixel].Green;
                                    aBitmapBits[(Y*BitmapWidth + X)*4 + 2] = aMapData.PaletteData[CurrentPixel].Red;
                                }
                            }
                        }
                    }
                }
            }
        }
    }


    void DrawTileset(MapData &aMapData, unsigned char* aBitmapBits, int BitmapWidth) {

        int x, y, X, Y, Tile8InTile16, Palette, CurrentPixel;
        bool Hflip, Vflip;
        Tile8 CurrentTile8;
        int Tile16Index = 0;

        /* Loop on all registered Tile16 IDs - there should be 256 of them */
        while (Tile16Index < (int)aMapData.Tile16Data.size()) {

            /* Process each of the 4 Tile8s in this Tile16 */
            for (Tile8InTile16 = 0; Tile8InTile16 < 4; Tile8InTile16++) {

                /* Get the data corresponding to this Tile8 */
                Hflip = aMapData.Tile16Data[Tile16Index].HorizontalFlip[Tile8InTile16];
                Vflip = aMapData.Tile16Data[Tile16Index].VerticalFlip[Tile8InTile16];
                Palette = aMapData.Tile16Data[Tile16Index].Palette[Tile8InTile16] - 1;
                CurrentTile8 = aMapData.Tile8Data[ aMapData.Tile16Data[Tile16Index].Tile8Data[Tile8InTile16] ];

                /* Loop on each pixel of this Tile8 */
                for (x = 0; x < 8; x++) {
                    for (y = 0; y < 8; y++) {

                        /* Get the correct color for this pixel - apply flips and palette changes if any */
                        CurrentPixel = CurrentTile8.Pixel((Hflip?(7-x):x), (Vflip?(7-y):y)) + 0x10*Palette;

                        /* Get the absolute coordinates of this pixel in the bitmap */
                        X = 256*aMapData.NbScreensX + 16*(Tile16Index%16) + x + ((Tile8InTile16 & 0x01) ? 8 : 0) + 20;
                        Y = 16*(Tile16Index/16) + y + ((Tile8InTile16 & 0x02) ? 8 : 0);

                        /* Draw the pixel */
                        aBitmapBits[(Y*BitmapWidth + X)*4]     = aMapData.PaletteData[CurrentPixel].Blue;
                        aBitmapBits[(Y*BitmapWidth + X)*4 + 1] = aMapData.PaletteData[CurrentPixel].Green;
                        aBitmapBits[(Y*BitmapWidth + X)*4 + 2] = aMapData.PaletteData[CurrentPixel].Red;
                    }
                }
            }

            /* Get the next Tile16 ID */
            Tile16Index++;
        }
    }


    void DrawSelection(int TopLeftX, int TopLeftY, int BottomRightX, int BottomRightY, unsigned char* aBitmapBits, int BitmapWidth) {

        int X, Y;

//        if (gSelectionIsInTileset) {
//
//            /* Draw green square around selected tile */
//            int TopLeftX = TilesetX + (gSelectedTileID%16)*16;
//            int TopLeftY =            (gSelectedTileID/16)*16;
//
//            /* Top edge */
//            for (X = 0; X < 15; X++) {lpBitmapBits[( TopLeftY    *BitmapWidth + TopLeftX+ X)*4 + 1] = 255;}
//            /* Right edge */
//            for (Y = 0; Y < 15; Y++) {lpBitmapBits[((TopLeftY+ Y)*BitmapWidth + TopLeftX+15)*4 + 1] = 255;}
//            /* Bottom edge */
//            for (X = 15; X > 0; X--) {lpBitmapBits[((TopLeftY+15)*BitmapWidth + TopLeftX+ X)*4 + 1] = 255;}
//            /* Left edge */
//            for (Y = 15; Y > 0; Y--) {lpBitmapBits[((TopLeftY+ Y)*BitmapWidth + TopLeftX   )*4 + 1] = 255;}
//        }
//        else {

            /* Draw green rectangle around selection */
//            int TopLeftX     = (gSelectedTile16_i.AbsX() < gSelectedTile16_j.AbsX() ? gSelectedTile16_i.AbsX() : gSelectedTile16_j.AbsX())*16;
//            int TopLeftY     = (gSelectedTile16_i.AbsY() < gSelectedTile16_j.AbsY() ? gSelectedTile16_i.AbsY() : gSelectedTile16_j.AbsY())*16;
//            int BottomRightX = (gSelectedTile16_i.AbsX() > gSelectedTile16_j.AbsX() ? gSelectedTile16_i.AbsX() : gSelectedTile16_j.AbsX())*16 + 15;
//            int BottomRightY = (gSelectedTile16_i.AbsY() > gSelectedTile16_j.AbsY() ? gSelectedTile16_i.AbsY() : gSelectedTile16_j.AbsY())*16 + 15;

            int SelectionWidth  = BottomRightX - TopLeftX + 1;
            int SelectionHeight = BottomRightY - TopLeftY + 1;

            /* Top edge */
            for (X = 0; X < SelectionWidth -1; X++) {aBitmapBits[( TopLeftY   *BitmapWidth + TopLeftX + X)*4 + 1] = 255;}
            /* Right edge */
            for (Y = 0; Y < SelectionHeight-1; Y++) {aBitmapBits[((TopLeftY+Y)*BitmapWidth + BottomRightX)*4 + 1] = 255;}
            /* Bottom edge */
            for (X = SelectionWidth -1; X > 0; X--) {aBitmapBits[(BottomRightY*BitmapWidth + TopLeftX + X)*4 + 1] = 255;}
            /* Left edge */
            for (Y = SelectionHeight-1; Y > 0; Y--) {aBitmapBits[((TopLeftY+Y)*BitmapWidth + TopLeftX    )*4 + 1] = 255;}
//        }
    }

}
