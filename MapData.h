#ifndef __MAPDATA_H__
#define __MAPDATA_H__

#include <vector>

#define MAX_TILE16        256
#define PIXELS_PER_TILE8  64
#define TILE8S_PER_TILE16 4


using namespace std;

class Color {
public:
    Color();
    Color(unsigned char ColorByte1, unsigned char ColorByte2);
    ~Color();

    int Red;
    int Green;
    int Blue;
};

class Tile8 {
public:
    Tile8();
    Tile8(unsigned char Tile8ByteData[32]);
    ~Tile8();
    int Pixel(int x, int y);

    int PixelData[PIXELS_PER_TILE8];
};

class Tile16 {
public:
    Tile16();
    Tile16(unsigned char Tile16ByteData[8]);
    ~Tile16();

    int  Tile8Data[TILE8S_PER_TILE16];
    bool HorizontalFlip[TILE8S_PER_TILE16];
    bool VerticalFlip[TILE8S_PER_TILE16];
    int  Palette[TILE8S_PER_TILE16];
//    bool TransparentZero[TILE8S_PER_TILE16];
};

class MapData {
public:
    MapData() : MapTiles(0),
                Tile16Data(0),
                Tile8Data(0),
                PaletteData(0)
                {NbScreensX = 0; NbScreensY = 0;}
    ~MapData();
    int  GetTile16(int ScreenX, int ScreenY, int X, int Y);
    void InsertTile16Data(unsigned char Tile16ByteData[8]);
    void InsertTile8Data(unsigned char Tile8ByteData[32]);
    void InsertColorData(unsigned char ColorByte1, unsigned char ColorByte2);

    int NbScreensX;
    int NbScreensY;
    vector<int>    MapTiles;
    vector<Tile16> Tile16Data;
    vector<Tile8>  Tile8Data;
    vector<Color>  PaletteData;
};


#endif // __MAPDATA_H__
