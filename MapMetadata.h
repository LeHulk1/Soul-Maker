#ifndef __MAPMETADATA_H__
#define __MAPMETADATA_H__

#include <vector>
#include <windows.h>

#define NB_MAPS 97


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

void FillMapCombobox(HWND hwnd, int ComboboxID);

void GetMapMetadata(vector<MapMetadata> &MapMetadataList);

#endif // __MAPMETADATA_H__
