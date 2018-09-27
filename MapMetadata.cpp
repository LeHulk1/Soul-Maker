#include "MapMetadata.h"



/* Map names */
const char* MapNameList[] = {
    "[00 01] - Master's Shrine - Grass Valley",
    "[01 01] - Grass Valley",
    "[01 02] - Grass Valley - Child's secret passage",
    "[01 03] - Grass Valley - Treasure room",
    "[01 04] - Grass Valley - Chief's room",
    "[02 01] - Grass Valley - Underground Castle, west",
    "[02 02] - Grass Valley - Underground Castle, east",
    "[02 03] - Lisa's dream",
    "[03 01] - Grass Valley - Leo's Painting, room 1",
    "[03 02] - Grass Valley - Leo's Painting, room 2",
    "[03 03] - Grass Valley - Leo's Painting, room 3",
    "[03 04] - Grass Valley - Tulip's dream",
    "[03 05] - Grass Valley - Leo's Painting, room 4",
    "[03 06] - Grass Valley - Solid Arm",
    "[00 08] - Trial Room",
    "[00 09] - Overworld",
    "[00 0A] - *** UNKNOWN - Intro screen? ***",
    "[00 0B] - Ending with Lisa",
    "[00 0C] - *** UNKNOWN ***",
    "[00 0D] - *** UNKNOWN ***",
    "[00 02] - Master's Shrine - GreenWood",
    "[04 01] - GreenWood",
    "[04 02] - GreenWood - Mole tunnels",
    "[04 03] - GreenWood - Squirrel's house",
    "[04 04] - GreenWood - Psycho Sword Squirrel's house",
    "[04 05] - GreenWood - Mole tunnels (bird's dream)",
    "[04 06] - GreenWood (stump's dream)",
    "[05 01] - GreenWood - Lostside Marsh",
    "[05 02] - GreenWood - Water Shrine F1",
    "[05 03] - GreenWood - Water Shrine B1",
    "[05 04] - GreenWood - Water Shrine B2",
    "[05 05] - GreenWood - Fire Shrine F1",
    "[05 06] - GreenWood - Fire Shrine B1",
    "[05 07] - GreenWood - Fire Shrine B2",
    "[05 08] - GreenWood - Light Shrine F1",
    "[05 09] - GreenWood - Light Shrine B1",
    "[05 0A] - GreenWood - Elemental Statues",
    "[00 03] - Master's Shrine - St Elles",
    "[06 01] - St Elles",
    "[07 01] - St Elles - South Seabed",
    "[07 02] - St Elles - Secret Cave",
    "[07 03] - St Elles - Lue's Room",
    "[07 04] - St Elles - North Seabed",
    "[07 05] - St Elles - North Seabed - dolphin's dream",
    "[08 01] - St Elles - Southerta",
    "[08 02] - St Elles - Rockbird",
    "[08 03] - St Elles - Durean",
    "[08 04] - St Elles - Blester",
    "[09 01] - St Elles - Ghost Ship",
    "[09 02] - St Elles - Ghost Ship - dolphin's dream",
    "[00 04] - Master's Shrine - Mountain of Souls",
    "[0A 01] - Mountain of Souls",
    "[0B 01] - Mountain of Souls - Aurora Ridge, south",
    "[0B 02] - Mountain of Souls - Aurora Ridge, north",
    "[0B 03] - Mountain of Souls - Aurora Ridge, north - Old man's dream",
    "[0B 04] - Mountain of Souls - Lune Passage",
    "[0B 05] - Mountain of Souls - Lune",
    "[0B 06] - Mountain of Souls - Lune - Mushroom's dream",
    "[0B 07] - Mountain of Souls - Poseidon",
    "[0C 01] - Mountain of Souls - Summit",
    "[0C 02] - Mountain of Souls - Ice Hill",
    "[0C 03] - Mountain of Souls - Laynole",
    "[00 05] - Master's Shrine - Leo's Lab",
    "[0D 01] - Leo's Lab - F1",
    "[0D 02] - Leo's Lab - F2",
    "[0D 03] - Leo's Lab - Attic",
    "[0D 04] - Leo's Lab - Mousehole",
    "[0D 05] - Leo's Lab - F1 - Cat's dream",
    "[0E 01] - Leo's Lab - B1",
    "[0E 02] - Leo's Lab - B2",
    "[0E 03] - Leo's Lab - Power Plant",
    "[0E 04] - Leo's Lab - Tin Doll",
    "[0E 05] - Leo's Lab - B2 (Duplicated?)",
    "[0F 01] - Leo's Lab - Model Town 1",
    "[0F 02] - Leo's Lab - Model Town 2",
    "[00 06] - Master's Shrine - Magridd Castle",
    "[10 01] - Magridd Castle",
    "[11 01] - Magridd Castle - Torture Chamber",
    "[11 02] - Magridd Castle - B1",
    "[11 03] - Magridd Castle - B2",
    "[11 04] - Magridd Castle - B2 (Duplicated?)",
    "[12 01] - Magridd Castle - Left Tower F1",
    "[12 02] - Magridd Castle - Left Tower F2",
    "[12 03] - Magridd Castle - Left Tower F3",
    "[12 04] - Magridd Castle - Right Tower F1",
    "[12 05] - Magridd Castle - Right Tower F2",
    "[12 06] - Magridd Castle - Right Tower F3",
    "[12 07] - Magridd Castle - Right Tower F4",
    "[13 01] - Magridd Castle - Airship Dock",
    "[13 02] - *** UNKNOWN ***",
    "[13 03] - *** UNKNOWN ***",
    "[00 07] - Master's Shrine - World of Evil",
    "[14 01] - World of Evil - First Space",
    "[14 02] - World of Evil - Second Space",
    "[14 03] - World of Evil - Dazzling Space",
    "[15 01] - World of Evil - Deathtoll's Shrine",
    "[15 02] - World of Evil - Final Battle"
};


using namespace std;

MapMetadata::MapMetadata(int aMapArrangementAddress,
                         int aTilesetAddress,
                         int aGraphicsAddress,
                         int aPaletteAddress) {
    MapArrangementAddress = aMapArrangementAddress;
    TilesetAddress = aTilesetAddress;
    GraphicsAddress = aGraphicsAddress;
    PaletteAddress = aPaletteAddress;
}

MapMetadata::~MapMetadata() {}


void InsertMapMetadata(vector<MapMetadata> &MapMetadataList,
                       int aMapArrangementAddress,
                       int aTilesetAddress,
                       int aGraphicsAddress,
                       int aPaletteAddress) {

    MapMetadata NewMapMetadata(aMapArrangementAddress,
                               aTilesetAddress,
                               aGraphicsAddress,
                               aPaletteAddress);
    MapMetadataList.push_back(NewMapMetadata);
}


void FillMapCombobox(HWND hwnd, int ComboboxID) {

    for (int i = 0; i < NB_MAPS; i++) {
        SendDlgItemMessage(hwnd, ComboboxID, CB_ADDSTRING, 0, (LONG)MapNameList[i]);
    }
}


void GetMapMetadata(vector<MapMetadata> &MapMetadataList) {

    /* 28003 - Map [00 01] - Master's Shrine - Grass Valley */
    InsertMapMetadata(MapMetadataList, 0x28026, 0x2801E, 0x28017, 0x28007);

    /* 2803D - Map [01 01] - Grass Valley */
    InsertMapMetadata(MapMetadataList, 0x28043, 0x28051, 0x2804A, 0x28059);

    /* 280A8 - Map [01 02] - Grass Valley - Child's secret passage */
    InsertMapMetadata(MapMetadataList, 0x280AC, 0x28051, 0x2804A, 0x28059);

    /* 280B4 - Map [01 03] - Grass Valley - Treasure room */
    InsertMapMetadata(MapMetadataList, 0x280B8, 0x28051, 0x2804A, 0x28059);

    /* 280C0 - Map [01 04] - Grass Valley - Chief's room */
    InsertMapMetadata(MapMetadataList, 0x280C4, 0x28051, 0x2804A, 0x28059);

    /* 280CC - Map [02 01] - Grass Valley - Underground Castle, west */
    InsertMapMetadata(MapMetadataList, 0x280D0, 0x280F5, 0x280EE, 0x280FD);

    /* 28125 - Map [02 02] - Grass Valley - Underground Castle, east */
    InsertMapMetadata(MapMetadataList, 0x28129, 0x280F5, 0x280EE, 0x280FD);

    /* 28131 - Map [02 03] - Lisa's dream */
    InsertMapMetadata(MapMetadataList, 0x28129, 0x280F5, 0x280EE, 0x280FD);

    /* 28152 - Map [03 01] - Grass Valley - Leo's Painting, room 1 */
    InsertMapMetadata(MapMetadataList, 0x28156, 0x2817B, 0x28174, 0x28183);

    /* 281B1 - Map [03 02] - Grass Valley - Leo's Painting, room 2 */
    InsertMapMetadata(MapMetadataList, 0x281B5, 0x2817B, 0x28174, 0x28183);

    /* 281BD - Map [03 03] - Grass Valley - Leo's Painting, room 3 */
    InsertMapMetadata(MapMetadataList, 0x281C1, 0x2817B, 0x28174, 0x28183);

    /* 281C9 - Map [03 04] - Grass Valley - Tulip's dream */
    InsertMapMetadata(MapMetadataList, 0x281CD, 0x2817B, 0x28174, 0x28183);

    /* 281EA - Map [03 05] - Grass Valley - Leo's Painting, room 4 */
    InsertMapMetadata(MapMetadataList, 0x281EE, 0x2817B, 0x28174, 0x28183);

    /* 281F6 - Map [03 06] - Grass Valley - Solid Arm */
    InsertMapMetadata(MapMetadataList, 0x281FA, 0x2817B, 0x28174, 0x28183);

    /* 28217 - Map [00 08] - Trial Room */
    InsertMapMetadata(MapMetadataList, 0x28231, 0x28222, 0x2821B, 0x2822A);

    /* 28252 - Map [00 09] - Overworld */
    InsertMapMetadata(MapMetadataList, 0x28281, 0x28272, 0x2826B, 0x2827A);

    /* 282A7 - Map [00 0A] - *** UNKNOWN - Intro screen? *** */
    InsertMapMetadata(MapMetadataList, 0x282C0, 0, 0, 0);

    /* 282EE - Map [00 0B] - Ending with Lisa */
    InsertMapMetadata(MapMetadataList, 0x28308, 0x282F9, 0x282F2, 0x28301);

    /* 2833C - Map [00 0C] - *** UNKNOWN *** */
    InsertMapMetadata(MapMetadataList, 0, 0, 0, 0);

    /* 2834F - Map [00 0D] - *** UNKNOWN *** */
    InsertMapMetadata(MapMetadataList, 0, 0, 0, 0);

    /* 28364 - Map [00 02] - Master's Shrine - GreenWood */
    InsertMapMetadata(MapMetadataList, 0x28026, 0x2801E, 0x28017, 0x28368);

    /* 28372 - Map [04 01] - GreenWood */
    InsertMapMetadata(MapMetadataList, 0x28376, 0x2838D, 0x28386, 0x2837D);

    /* 283D1 - Map [04 02] - GreenWood - Mole tunnels */
    InsertMapMetadata(MapMetadataList, 0x283DC, 0x2838D, 0x28386, 0x283D5);

    /* 283E4 - Map [04 03] - GreenWood - Squirrel's house */
    InsertMapMetadata(MapMetadataList, 0x283EF, 0x2838D, 0x28386, 0x283E8);

    /* 283F7 - Map [04 04] - GreenWood - Psycho Sword Squirrel's house */
    InsertMapMetadata(MapMetadataList, 0x28402, 0x2838D, 0x28386, 0x283FB);

    /* 2840A - Map [04 05] - GreenWood - Mole tunnels (bird's dream) */
    InsertMapMetadata(MapMetadataList, 0x2840E, 0x2838D, 0x28386, 0x283D5);

    /* 28416 - Map [04 06] - GreenWood (stump's dream) */
    InsertMapMetadata(MapMetadataList, 0x2841A, 0x2838D, 0x28386, 0x2837D);

    /* 28422 - Map [05 01] - GreenWood - Lostside Marsh */
    InsertMapMetadata(MapMetadataList, 0x2843C, 0x2842D, 0x28426, 0x28435);

    /* 28471 - Map [05 02] - GreenWood - Water Shrine F1 */
    InsertMapMetadata(MapMetadataList, 0x28475, 0x28488, 0x28481, 0x28490);

    /* 284C2 - Map [05 03] - GreenWood - Water Shrine B1 */
    InsertMapMetadata(MapMetadataList, 0x284C6, 0x28488, 0x28481, 0x28490);

    /* 284D3 - Map [05 04] - GreenWood - Water Shrine B2 */
    InsertMapMetadata(MapMetadataList, 0x284D7, 0x28488, 0x28481, 0x28490);

    /* 284E4 - Map [05 05] - GreenWood - Fire Shrine F1 */
    InsertMapMetadata(MapMetadataList, 0x284E8, 0x284F6, 0x284EF, 0x284FE);

    /* 28535 - Map [05 06] - GreenWood - Fire Shrine B1 */
    InsertMapMetadata(MapMetadataList, 0x28539, 0x284F6, 0x284EF, 0x284FE);

    /* 28541 - Map [05 07] - GreenWood - Fire Shrine B2 */
    InsertMapMetadata(MapMetadataList, 0x28545, 0x284F6, 0x284EF, 0x284FE);

    /* 2854D - Map [05 08] - GreenWood - Light Shrine F1 */
    InsertMapMetadata(MapMetadataList, 0x28551, 0x28576, 0x2856F, 0x2857E);

    /* 285A1 - Map [05 09] - GreenWood - Light Shrine B1 */
    InsertMapMetadata(MapMetadataList, 0x285A5, 0x28576, 0x2856F, 0x2857E);

    /* 285B2 - Map [05 0A] - GreenWood - Elemental Statues */
    InsertMapMetadata(MapMetadataList, 0x285B6, 0x28576, 0x2856F, 0x2857E);

    /* 285D3 - Map [00 03] - Master's Shrine - St Elles */
    InsertMapMetadata(MapMetadataList, 0x28026, 0x2801E, 0x28017, 0x285D7);

    /* 285E1 - Map [06 01] - St Elles */
    InsertMapMetadata(MapMetadataList, 0x285E5, 0x285F3, 0x285EC, 0x285FB);

    /* 2863E - Map [07 01] - St Elles - South Seabed */
    InsertMapMetadata(MapMetadataList, 0x28642, 0x28667, 0x28660, 0x2866F);

    /* 28697 - Map [07 02] - St Elles - Secret Cave */
    InsertMapMetadata(MapMetadataList, 0x2869B, 0x28667, 0x28660, 0x2866F);

    /* 286A3 - Map [07 03] - St Elles - Lue's Room */
    InsertMapMetadata(MapMetadataList, 0x286A7, 0x285F3, 0x285EC, 0x285FB);

    /* 286AF - Map [07 04] - St Elles - North Seabed */
    InsertMapMetadata(MapMetadataList, 0x286B3, 0x28667, 0x28660, 0x2866F);

    /* 286D0 - Map [07 05] - St Elles - North Seabed - dolphin's dream */
    InsertMapMetadata(MapMetadataList, 0x286D4, 0x28667, 0x28660, 0x2866F);

    /* 286DC - Map [08 01] - St Elles - Southerta */
    InsertMapMetadata(MapMetadataList, 0x286EF, 0x286E0, 0x286FD, 0x286E8);

    /* 2872F - Map [08 02] - St Elles - Rockbird */
    InsertMapMetadata(MapMetadataList, 0x28742, 0x28733, 0x286FD, 0x2873B);

    /* 2874A - Map [08 03] - St Elles - Durean */
    InsertMapMetadata(MapMetadataList, 0x2875D, 0x2874E, 0x286FD, 0x28756);

    /* 28765 - Map [08 04] - St Elles - Blester */
    InsertMapMetadata(MapMetadataList, 0x28778, 0x28769, 0x286FD, 0x28771);

    /* 28785 - Map [09 01] - St Elles - Ghost Ship */
    InsertMapMetadata(MapMetadataList, 0x287B6, 0x287A7, 0x287A0, 0x287AF);

    /* 287E2 - Map [09 02] - St Elles - Ghost Ship - dolphin's dream */
    InsertMapMetadata(MapMetadataList, 0x287B6, 0x287A7, 0x287A0, 0x287AF);

    /* 287FE - Map [00 04] - Master's Shrine - Mountain of Souls */
    InsertMapMetadata(MapMetadataList, 0x28026, 0x2801E, 0x28017, 0x28802);

    /* 2880C - Map [0A 01] - Mountain of Souls */
    InsertMapMetadata(MapMetadataList, 0x28826, 0x28817, 0x28810, 0x2881F);

    /* 28853 - Map [0B 01] - Mountain of Souls - Aurora Ridge, south */
    InsertMapMetadata(MapMetadataList, 0x2885E, 0x28897, 0x28890, 0x28857);

    /* 288AC - Map [0B 02] - Mountain of Souls - Aurora Ridge, north */
    InsertMapMetadata(MapMetadataList, 0x288B7, 0x28897, 0x28890, 0x288B0);

    /* 288D3 - Map [0B 03] - Mountain of Souls - Aurora Ridge, north - Old man's dream */
    InsertMapMetadata(MapMetadataList, 0x288DE, 0x28897, 0x28890, 0x288D7);

    /* 2890F - Map [0B 04] - Mountain of Souls - Lune Passage */
    InsertMapMetadata(MapMetadataList, 0x28913, 0x28946, 0x2893F, 0x28936);

    /* 28970 - Map [0B 05] - Mountain of Souls - Lune */
    InsertMapMetadata(MapMetadataList, 0x28974, 0x28946, 0x2893F, 0x28936);

    /* 28981 - Map [0B 06] - Mountain of Souls - Lune - Mushroom's dream */
    InsertMapMetadata(MapMetadataList, 0x28985, 0x28946, 0x2893F, 0x28936);

    /* 289A7 - Map [0B 07] - Mountain of Souls - Poseidon */
    InsertMapMetadata(MapMetadataList, 0x289B2, 0x28946, 0x2893F, 0x289AB);

    /* 289D4 - Map [0C 01] - Mountain of Souls - Summit */
    InsertMapMetadata(MapMetadataList, 0x289DF, 0x28A02, 0x289FB, 0x289D8);

    /* 28A20 - Map [0C 02] - Mountain of Souls - Ice Hill */
    InsertMapMetadata(MapMetadataList, 0x28A2B, 0x28A02, 0x289FB, 0x28A24);

    /* 28A4D - Map [0C 03] - Mountain of Souls - Laynole */
    InsertMapMetadata(MapMetadataList, 0x28A58, 0x28A02, 0x289FB, 0x28A51);

    /* 28A7A - Map [00 05] - Master's Shrine - Leo's Lab */
    InsertMapMetadata(MapMetadataList, 0x28026, 0x2801E, 0x28017, 0x28A7E);

    /* 28A88 - Map [0D 01] - Leo's Lab - F1 */
    InsertMapMetadata(MapMetadataList, 0x28A8C, 0x28AB1, 0x28AAA, 0x28AB9);

    /* 28AE7 - Map [0D 02] - Leo's Lab - F2 */
    InsertMapMetadata(MapMetadataList, 0x28AEB, 0x28AB1, 0x28AAA, 0x28AB9);

    /* 28AF3 - Map [0D 03] - Leo's Lab - Attic */
    InsertMapMetadata(MapMetadataList, 0x28AF7, 0x28AB1, 0x28AAA, 0x28AB9);

    /* 28AFF - Map [0D 04] - Leo's Lab - Mousehole */
    InsertMapMetadata(MapMetadataList, 0x28B03, 0x28AB1, 0x28AAA, 0x28AB9);

    /* 28B0B - Map [0D 05] - Leo's Lab - F1 - Cat's dream */
    InsertMapMetadata(MapMetadataList, 0x28B24, 0x28AB1, 0x28AAA, 0x28AB9);

    /* 28B2C - Map [0E 01] - Leo's Lab - B1 */
    InsertMapMetadata(MapMetadataList, 0x28B30, 0x28B5A, 0x28B53, 0x28B62);

    /* 28B8B - Map [0E 02] - Leo's Lab - B2 */
    InsertMapMetadata(MapMetadataList, 0x28B8F, 0x28B5A, 0x28B53, 0x28B62);

    /* 28B9C - Map [0E 03] - Leo's Lab - Power Plant */
    InsertMapMetadata(MapMetadataList, 0x28BA0, 0x28B5A, 0x28B53, 0x28B62);

    /* 28BAD - Map [0E 04] - Leo's Lab - Tin Doll */
    InsertMapMetadata(MapMetadataList, 0x28BB1, 0x28B5A, 0x28B53, 0x28B62);

    /* 28BCE - Map [0E 05] - Leo's Lab - B2 (Duplicated?) */
    InsertMapMetadata(MapMetadataList, 0x28BD2, 0x28B5A, 0x28B53, 0x28B62);

    /* 28BF4 - Map [0F 01] - Leo's Lab - Model Town 1 */
    InsertMapMetadata(MapMetadataList, 0x28BF8, 0x28C0B, 0x28C04, 0x28C13);

    /* 28C36 - Map [0F 02] - Leo's Lab - Model Town 2 */
    InsertMapMetadata(MapMetadataList, 0x28C3A, 0x28C0B, 0x28C04, 0x28C13);

    /* 28C47 - Map [00 06] - Master's Shrine - Magridd Castle */
    InsertMapMetadata(MapMetadataList, 0x28026, 0x2801E, 0x28017, 0x28C4B);

    /* 28C55 - Map [10 01] - Magridd Castle */
    InsertMapMetadata(MapMetadataList, 0x28C6F, 0x28C60, 0x28C59, 0x28C68);

    /* 28CAA - Map [11 01] - Magridd Castle - Torture Chamber */
    InsertMapMetadata(MapMetadataList, 0x28CAE, 0x28CD3, 0x28CCC, 0x28CDB);

    /* 28CF7 - Map [11 02] - Magridd Castle - B1 */
    InsertMapMetadata(MapMetadataList, 0x28CFB, 0x28CD3, 0x28CCC, 0x28CDB);

    /* 28D08 - Map [11 03] - Magridd Castle - B2 */
    InsertMapMetadata(MapMetadataList, 0x28D0C, 0x28CD3, 0x28CCC, 0x28CDB);

    /* 28D19 - Map [11 04] - Magridd Castle - B2 (Duplicated?) */
    InsertMapMetadata(MapMetadataList, 0x28D1D, 0x28CD3, 0x28CCC, 0x28CDB);

    /* 28D3F - Map [12 01] - Magridd Castle - Left Tower F1 */
    InsertMapMetadata(MapMetadataList, 0x28D4A, 0x28D6F, 0x28D68, 0x28D43);

    /* 28D8C - Map [12 02] - Magridd Castle - Left Tower F2 */
    InsertMapMetadata(MapMetadataList, 0x28D97, 0x28D6F, 0x28D68, 0x28D90);

    /* 28DA4 - Map [12 03] - Magridd Castle - Left Tower F3 */
    InsertMapMetadata(MapMetadataList, 0x28DAF, 0x28D6F, 0x28D68, 0x28DA8);

    /* 28DB7 - Map [12 04] - Magridd Castle - Right Tower F1 */
    InsertMapMetadata(MapMetadataList, 0x28DC2, 0x28D6F, 0x28D68, 0x28DBB);

    /* 28DCF - Map [12 05] - Magridd Castle - Right Tower F2 */
    InsertMapMetadata(MapMetadataList, 0x28DDA, 0x28D6F, 0x28D68, 0x28DD3);

    /* 28DE7 - Map [12 06] - Magridd Castle - Right Tower F3 */
    InsertMapMetadata(MapMetadataList, 0x28DF2, 0x28D6F, 0x28D68, 0x28DEB);

    /* 28DFA - Map [12 07] - Magridd Castle - Right Tower F4 */
    InsertMapMetadata(MapMetadataList, 0x28E14, 0x28E05, 0x28DFE, 0x28E0D);

    /* 28E49 - Map [13 01] - Magridd Castle - Airship Dock */
    InsertMapMetadata(MapMetadataList, 0x28E90, 0x28E88, 0x28E81, 0x28E4D);

    /* 28EA8 - Map [13 02] - *** UNKNOWN *** */
    InsertMapMetadata(MapMetadataList, 0, 0, 0, 0);

    /* 28EDF - Map [13 03] - *** UNKNOWN *** */
    InsertMapMetadata(MapMetadataList, 0, 0, 0, 0);

    /* 28F01 - Map [00 07] - Master's Shrine - World of Evil */
    InsertMapMetadata(MapMetadataList, 0x28026, 0x2801E, 0x28017, 0x28F05);

    /* 28F0F - Map [14 01] - World of Evil - First Space */
    InsertMapMetadata(MapMetadataList, 0x28F1A, 0x28F2D, 0x28F26, 0x28F13);

    /* 28F60 - Map [14 02] - World of Evil - Second Space */
    InsertMapMetadata(MapMetadataList, 0x28F6B, 0x28F2D, 0x28F26, 0x28F64);

    /* 28F78 - Map [14 03] - World of Evil - Dazzling Space */
    InsertMapMetadata(MapMetadataList, 0x28F83, 0x28F2D, 0x28F26, 0x28F7C);

    /* 28F90 - Map [15 01] - World of Evil - Deathtoll's Shrine */
    InsertMapMetadata(MapMetadataList, 0x28F94, 0x28FAF, 0x28FA8, 0x28FB7);

    /* 28FE1 - Map [15 02] - World of Evil - Final Battle */
    InsertMapMetadata(MapMetadataList, 0x28FE5, 0x28FAF, 0x28FA8, 0x28FEA);
}
