#include <string.h>

#include "MapData.h"


/*** Color implementation */

Color::Color() {
    Red   = 0;
    Green = 0;
    Blue  = 0;
}

Color::Color(unsigned char ColorByte1, unsigned char ColorByte2) {

    /* Implementation of the SNES 15-bit colors stored in ?BBBBBGG GGGRRRRR format */
    Red   = ColorByte2 & 0x1F;
    Green = ((ColorByte1 & 0x03) << 3) + ((ColorByte2 & 0xE0) >> 5);
    Blue  = (ColorByte1 & 0x7C) >> 2;

    /* Multiply by 8 to get each color on 8 bits */
    Red   *= 8;
    Green *= 8;
    Blue  *= 8;
}

Color::~Color() {}


/*** Tile8 implementation */

static unsigned char BitMask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

Tile8::Tile8() {
    memset(PixelData, 0, PIXELS_PER_TILE8);
}

Tile8::Tile8(unsigned char Tile8ByteData[32]) {

    /* Implementation of the SNES 4bpp graphics calculation */
    int x, y;
    for (x = 0; x < 8; x++) {
        for (y = 0; y < 8; y++) {
            PixelData[8*y + x] = ( (Tile8ByteData[2*y]      & BitMask[x]) >> (7-x) ) * 0x01
                               + ( (Tile8ByteData[2*y + 1]  & BitMask[x]) >> (7-x) ) * 0x02
                               + ( (Tile8ByteData[2*y + 16] & BitMask[x]) >> (7-x) ) * 0x04
                               + ( (Tile8ByteData[2*y + 17] & BitMask[x]) >> (7-x) ) * 0x08;
        }
    }
}

Tile8::~Tile8() {}

int Tile8::Pixel(int x, int y) {
    return PixelData[8*y + x];
}


/*** Tile16 implementation */

Tile16::Tile16() {
    Tile8Data[0] = 0;
    Tile8Data[1] = 0;
    Tile8Data[2] = 0;
    Tile8Data[3] = 0;
    HorizontalFlip[0] = 0;
    HorizontalFlip[1] = 0;
    HorizontalFlip[2] = 0;
    HorizontalFlip[3] = 0;
    VerticalFlip[0] = 0;
    VerticalFlip[1] = 0;
    VerticalFlip[2] = 0;
    VerticalFlip[3] = 0;
    Palette[0] = 0;
    Palette[1] = 0;
    Palette[2] = 0;
    Palette[3] = 0;
//    TransparentZero[0] = 0;
//    TransparentZero[1] = 0;
//    TransparentZero[2] = 0;
//    TransparentZero[3] = 0;
}

Tile16::Tile16(unsigned char Tile16ByteData[8]) {

    /* The Tile8 data for one Tile16 is defined by 4 groups of 2 bytes,
       each group for one Tile8 (top left, top right, bottom left, bottom right).

       Detail of one 2-byte Tile8 data in binary: abcdefgh xxxxxxxx
        - a: vertical flip
        - b: horizontal flip
        - c: ???
        - def: 3-bit number to pick which palette to apply
        - g: ???
        - h: ???
        - xxxxxxxx: index of the Tile8 in the graphics data */

    Tile8Data[0] = Tile16ByteData[1];
    Tile8Data[1] = Tile16ByteData[3];
    Tile8Data[2] = Tile16ByteData[5];
    Tile8Data[3] = Tile16ByteData[7];
    HorizontalFlip[0] = Tile16ByteData[0] & 0x40;
    HorizontalFlip[1] = Tile16ByteData[2] & 0x40;
    HorizontalFlip[2] = Tile16ByteData[4] & 0x40;
    HorizontalFlip[3] = Tile16ByteData[6] & 0x40;
    VerticalFlip[0] = Tile16ByteData[0] & 0x80;
    VerticalFlip[1] = Tile16ByteData[2] & 0x80;
    VerticalFlip[2] = Tile16ByteData[4] & 0x80;
    VerticalFlip[3] = Tile16ByteData[6] & 0x80;
    Palette[0] = (Tile16ByteData[0] & 0x1C) >> 2;
    Palette[1] = (Tile16ByteData[2] & 0x1C) >> 2;
    Palette[2] = (Tile16ByteData[4] & 0x1C) >> 2;
    Palette[3] = (Tile16ByteData[6] & 0x1C) >> 2;
//    TransparentZero[0] = Tile16ByteData[0] & 0x02;
//    TransparentZero[1] = Tile16ByteData[2] & 0x02;
//    TransparentZero[2] = Tile16ByteData[4] & 0x02;
//    TransparentZero[3] = Tile16ByteData[6] & 0x02;
}

Tile16::~Tile16() {}


/*** MapData implementation */

MapData::~MapData() {
    MapTiles.clear();
    Tile16Data.clear();
    Tile8Data.clear();
    PaletteData.clear();
}

int MapData::GetTile16(int ScreenX, int ScreenY, int X, int Y) {
    return MapTiles[256*NbScreensX*ScreenY + 256*ScreenX + 16*Y + X];
}

void MapData::SetTile16(int ScreenX, int ScreenY, int X, int Y, int NewTile16ID) {
    if (ScreenX < NbScreensX && ScreenY < NbScreensY && X < 16 && Y < 16) {
        MapTiles[256*NbScreensX*ScreenY + 256*ScreenX + 16*Y + X] = NewTile16ID;
    }
}

void MapData::InsertTile16Data(unsigned char Tile16ByteData[8]) {
    Tile16 NewTile16(Tile16ByteData);
    Tile16Data.push_back(NewTile16);
}

void MapData::InsertTile8Data(unsigned char Tile8ByteData[32]) {
    Tile8 NewTile8(Tile8ByteData);
    Tile8Data.push_back(NewTile8);
}

void MapData::InsertColorData(unsigned char ColorByte1, unsigned char ColorByte2) {
    Color NewColor(ColorByte1, ColorByte2);
    PaletteData.push_back(NewColor);
}

