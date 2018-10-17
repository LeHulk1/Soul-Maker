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

    void InsertMapDataIntoROM(fstream &ROMFile,
                              MapMetadata &aMapMetadata,
                              MapData &aMapData);

    void Compress(vector<unsigned char> &DataToCompress,
                  vector<unsigned char> &CompressedData);
    void Decompress(fstream &ROMFile,
                    vector<unsigned char> &DecompressedData,
                    int Address,
                    int DecompressedDataLength,
                    int &CompressedDataLength);
}

#endif // __MAPDATATOOLS_H__
