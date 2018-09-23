#ifndef __MAPDATATOOLS_H__
#define __MAPDATATOOLS_H__

#include <fstream>
#include <iostream>
#include <vector>

#include "MapData.h"
#include "MapMetadata.h"


using namespace std;

typedef enum {SUCCESS = 0, FAILURE = 1} Status;

namespace MapDataTools {
    void Compress(bool MapMode);
    //void Decompress(fstream &ROMFile, MapData &aMapData, int MapDataAddress, int length);

    Status GetMapData(fstream &ROMFile,
                      MapMetadata &MapMetadata,
                      MapData &aMapData);
    void GetMapArrangementData(fstream &ROMFile,
                               MapData &aMapData,
                               int MapDataAddress);
    void GetTilesetData(fstream &ROMFile,
                        MapData &aMapData,
                        int TilesetAddress);
    void GetGraphicsData(fstream &ROMFile,
                         MapData &aMapData,
                         int GraphicsAddress,
                         bool bDataIsCompressed);
    void GetPaletteData(fstream &ROMFile,
                        MapData &aMapData,
                        int PaletteAddress,
                        int NbColors);

    void Decompress(fstream &ROMFile,
                    vector<unsigned char> &DecompressedData,
                    int Address,
                    int DataLength);
}

#endif // __MAPDATATOOLS_H__
