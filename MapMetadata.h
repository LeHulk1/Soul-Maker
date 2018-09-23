#ifndef __MAPMETADATA_H__
#define __MAPMETADATA_H__

#include <vector>


using namespace std;

class MapMetadata {
public:
    MapMetadata(int aMapArrangementAddress,
                int aTilesetAddress,
                int aGraphicsAddress,
                int aPaletteAddress);
    ~MapMetadata();

    int MapArrangementAddress;
    int TilesetAddress;
    int GraphicsAddress;
    int PaletteAddress;
};


void InsertMapMetadata(vector<MapMetadata> &MapMetadataList,
                       int aMapArrangementAddress,
                       int aTilesetAddress,
                       int aGraphicsAddress,
                       int aPaletteAddress);

void GetMapMetadata(vector<MapMetadata> &MapMetadataList);

#endif // __MAPMETADATA_H__
