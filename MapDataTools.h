#ifndef __MAPDATATOOLS_H__
#define __MAPDATATOOLS_H__

#include <fstream>
#include <iostream>
#include <vector>

#include "MapData.h"


using namespace std;

namespace MapDataTools {
    void Compress(bool MapMode);
    //void Decompress(fstream &ROMFile, MapData &aMapData, int MapDataAddress, int length);
    void GetPaletteData(fstream &ROMFile,
                        MapData &aMapData,
                        int PaletteAddress);
    void GetGraphicsData(fstream &ROMFile,
                         MapData &aMapData,
                         int GraphicsAddress,
                         bool bDataIsCompressed);
    void GetTilesetData(fstream &ROMFile,
                        MapData &aMapData,
                        int TilesetAddress);
    void GetMapArrangementData(fstream &ROMFile,
                               MapData &aMapData,
                               int MapDataAddress);
    void Decompress(fstream &ROMFile,
                    vector<unsigned char> &DecompressedData,
                    int Address,
                    int DataLength);
}

#endif // __MAPDATATOOLS_H__
