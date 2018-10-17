#include "MapDataTools.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <fstream>
#include <iostream>
#include <iomanip>


//#define MAP_FILE_NAME  "MapToCompress.txt"
//#define ROM_FILE_NAME  "Soul Blazer (U) [!] - Dev.smc"

//#define SEARCH_LOG2  8
#define SEARCH_SIZE  256
//#define LOOKAHEAD_LOG2  4
#define LOOKAHEAD_SIZE  16

#define MAX_BYTES  10000

#define MIN(x,y) (x<y?x:y)


#define COPY_BIT(bit) { \
    if (bit!=0) {CurrentOutputByte += PositionMask[CurrentOutputBit];} \
    if (CurrentOutputBit == 7) { \
        CurrentOutputBit = 0;    \
        CompressedData.push_back(CurrentOutputByte);     \
        CurrentOutputByte = 0;   \
    } else {CurrentOutputBit++;} \
}

#define COPY_4_BITS(x) { \
    bool e = x & 0x08; COPY_BIT(e); \
    bool f = x & 0x04; COPY_BIT(f); \
    bool g = x & 0x02; COPY_BIT(g); \
    bool h = x & 0x01; COPY_BIT(h); \
}

#define COPY_BYTE(x) { \
    bool a = x & 0x80; COPY_BIT(a); \
    bool b = x & 0x40; COPY_BIT(b); \
    bool c = x & 0x20; COPY_BIT(c); \
    bool d = x & 0x10; COPY_BIT(d); \
    bool e = x & 0x08; COPY_BIT(e); \
    bool f = x & 0x04; COPY_BIT(f); \
    bool g = x & 0x02; COPY_BIT(g); \
    bool h = x & 0x01; COPY_BIT(h); \
}


using namespace std;

static int PositionMask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

namespace MapDataTools {

//    static int ConvertHex(unsigned char x) {
//        switch(x) {
//        case '0': return 0x00;
//        case '1': return 0x01;
//        case '2': return 0x02;
//        case '3': return 0x03;
//        case '4': return 0x04;
//        case '5': return 0x05;
//        case '6': return 0x06;
//        case '7': return 0x07;
//        case '8': return 0x08;
//        case '9': return 0x09;
//        case 'a': return 0x0A;
//        case 'b': return 0x0B;
//        case 'c': return 0x0C;
//        case 'd': return 0x0D;
//        case 'e': return 0x0E;
//        case 'f': return 0x0F;
//        default: return 0x00;
//        }
//    }

    static int ReadPointer(unsigned char AddressBytes[3]) {
        return AddressBytes[2]*0x10000 + AddressBytes[1]*0x100 + AddressBytes[0]*0x01;
    }

//    void Compress(bool MapMode) {
//
//
//
//        // Open the file.
//        FILE *MapFile;
//        MapFile = fopen(MAP_FILE_NAME, "r");
//        if (!MapFile) {
//            printf("Error: File %s not found\n", MAP_FILE_NAME);
//            exit(1);
//        }
//
//        ofstream OutputFile("MapComp.txt");
//
//        OutputFile << "***************\n";
//        OutputFile << "* Binary dump *\n";
//        OutputFile << "***************\n";
//        OutputFile << endl;
//
//
//        // Read the bytes.
//        byte InputBytes[MAX_BYTES];
//        byte tocomp[MAX_BYTES];
//        int BufferSize = 0;
//        int Lines = 1;
//        int Columns = 0;
//        bool IncreaseColumns = true;
//        byte input1, input2;
//        while (BufferSize < MAX_BYTES) {
//            input1 = fgetc(MapFile);
//            if (input1 == '\n') {
//                /* ignore carriage return and line feed */
//                input1 = fgetc(MapFile);
//                IncreaseColumns = false;
//                Lines++;
//            }
//            input2 = fgetc(MapFile);
//            if (feof(MapFile)) {break;}
//            InputBytes[BufferSize] = ConvertHex(input1)*0x10 + ConvertHex(input2);
//            OutputFile << hex << setw(2) << setfill('0') << int(InputBytes[BufferSize]) << ' ';
//            BufferSize++;
//            if (IncreaseColumns) {Columns++;}
//
//            input1 = fgetc(MapFile); /* should be a blank */
//
//        }
//        fclose(MapFile);
//
//        OutputFile << endl;
//        OutputFile << endl;
//        OutputFile << "Lines / Columns: " << dec << Lines << " / " << Columns << ".";
//
//        OutputFile << endl;
//        OutputFile << endl;
//        OutputFile << "Number of bytes: " << BufferSize << ".";
//
//        // Re-arrange the data.
//        int i, j;
//        if (MapMode) {
//            int screenI, screenJ;
//            int NbScreensColumns = Columns / 16;
//            int NbScreensLines = Lines / 16;
//            for (screenI = 0; screenI < NbScreensLines; screenI++) {
//                for (screenJ = 0; screenJ < NbScreensColumns; screenJ++) {
//                    for (i = 0; i < 16; i++) {
//                        for (j = 0; j < 16; j++) {
//                            tocomp[256*NbScreensColumns*screenI + 256*screenJ + 16*i + j + SEARCH_SIZE] = InputBytes[256*NbScreensColumns*screenI + 16*screenJ + 16*NbScreensColumns*i + j];
//                        }
//                    }
//                }
//            }
//        }
//        else {
//            memcpy(&tocomp[SEARCH_SIZE], InputBytes, BufferSize*sizeof(byte));
//        }
//
//        OutputFile << endl;
//        OutputFile << endl;
//        OutputFile << endl;
//        OutputFile << "******************\n";
//        OutputFile << "* Arranged input *\n";
//        OutputFile << "******************\n";
//        OutputFile << endl;
//
//        for (i = 0; i < BufferSize; i++) {
//            OutputFile << hex << setw(2) << setfill('0') << int(tocomp[i+SEARCH_SIZE]) << ' ';
//        }
//
//
//
//        OutputFile << endl;
//        OutputFile << endl;
//        OutputFile << endl;
//        OutputFile << "*********************\n";
//        OutputFile << "* Compressed output *\n";
//        OutputFile << "*********************\n";
//        OutputFile << endl;
//
//
//        int currentIndex = SEARCH_SIZE;
//        int bestIndex, bestLength, compareLimit, currentLength;
//
//        // Prepare memory buffer: left-pad with with 0x20 values
//        for (i = 0; i < SEARCH_SIZE; i++) {
//            tocomp[i] = 0x20;
//        }
//        BufferSize += SEARCH_SIZE;
//        int CurrentOutputBit = 0;
//        int CurrentOutputByte = 0;
//
//        // Compressed map in byte format
//        byte OutputBytes[MAX_BYTES];
//
//        // Main compression loop.
//        while (currentIndex < BufferSize) {
//            bestIndex = 0;
//            bestLength = 0;
//
//            // Look for a match in the search buffer. (Brute force)
//            for (i = 0; i < SEARCH_SIZE; i++) {
//                // Don't compare past the end of the lookahead buffer.
//                // Don't compare past the end of the memory buffer.
//                compareLimit = MIN(
//                  LOOKAHEAD_SIZE + 1,
//                  BufferSize - currentIndex
//                );
//
//                // Compare the search buffer to the lookahead buffer.
//                // Count how many sequential bytes match (possibly zero).
//                currentLength = 0;
//                for (j = 0; j < compareLimit; j++) {
//                    if (tocomp[currentIndex - SEARCH_SIZE + i + j] == tocomp[currentIndex + j]) {
//                        currentLength += 1;
//                    }
//                    else {
//                        break;
//                    }
//                }
//
//                // Keep track of the largest match we've seen.
//                if (currentLength > bestLength) {
//                    bestIndex = currentIndex - SEARCH_SIZE + i;
//                    bestLength = currentLength;
//                }
//            }
//
//            // Write the next block of compressed output.
//            if (bestLength >= 2) {
//                // For some reason, the decompressor expects the pastcopy
//                // source values to be offset by 0xEF. I have no idea why.
//
////                OutputFile << endl;
////                OutputFile << "Pastcopy -> Index " << bestIndex % 256 << " (";
////                WRITE_BYTE_IN_BINARY((byte)((bestIndex + 0xEF) & 0xFF));
////                OutputFile << ") / Length " << bestLength << " (";
////                WRITE_BYTE_IN_BINARY((byte)(bestLength - 2));
////                OutputFile << ")\n";
////                OutputFile << endl;
//
//
//                bestIndex = (bestIndex + 0xEF) & 0xFF;
//                OutputFile << 0;
//                WRITE_BYTE_IN_BINARY((byte)(bestIndex));
//                WRITE_BYTE_IN_4_BITS((byte)(bestLength - 2));
//
//                COPY_BIT(0);
//                COPY_BYTE((byte)(bestIndex));
//                COPY_4_BITS((byte)(bestLength - 2));
//
////                OutputFile << endl;
//
//                currentIndex += bestLength;
//            }
//            else {
//
////                OutputFile << endl;
////                OutputFile << "Literal -> Byte ";
////                WRITE_BYTE_IN_BINARY(tocomp[currentIndex]);
////                OutputFile << endl;
////                OutputFile << endl;
//
//                OutputFile << 1;
//                WRITE_BYTE_IN_BINARY(tocomp[currentIndex]);
//
//                COPY_BIT(1);
//                COPY_BYTE(tocomp[currentIndex]);
//
////                OutputFile << endl;
//
//                currentIndex += 1;
//            }
//        }
//
//        OutputFile << endl;
//        OutputFile << endl;
//        for (i = 0; i < CurrentOutputByte + (CurrentOutputBit == 0 ? 0 : 1); i++) {
//            OutputFile << hex << setw(2) << setfill('0') << int(OutputBytes[i]) << ' ';
//        }
//
//        OutputFile << endl;
//        OutputFile << endl;
//        OutputFile << "Number of bytes : " << dec << CurrentOutputByte + (CurrentOutputBit == 0 ? 0 : 1) << endl;
//
//
//        OutputFile.close();
//        OutputFile.clear();
//
//    }












    Status GetMapData(fstream &ROMFile,
                      MapMetadata &aMapMetadata,
                      MapData &aMapData) {

        unsigned char Byte, NbColors;
        unsigned char AddressBytes[3];
        bool IsGraphicsDataCompressed = true;

        /* Return if one of the addresses is not provided */
        if (aMapMetadata.MapArrangementAddress == 0 ||
            aMapMetadata.TilesetAddress == 0 ||
            aMapMetadata.GraphicsAddress == 0 ||
            aMapMetadata.PaletteAddress == 0) {
            return FAILURE;
        }


        /*** Map arrangement data */
        ROMFile.seekp(aMapMetadata.MapArrangementAddress, ios::beg);
        ROMFile.read((char*)(&Byte), 1);
        if (Byte != 0x10) {
            return FAILURE;
        }
        ROMFile.seekg(1, ios::cur);
        ROMFile.read((char*)(&AddressBytes[0]), 3);

        GetMapArrangementData(ROMFile,
                              aMapData,
                              ReadPointer(AddressBytes));


        /*** Tileset data */
        ROMFile.seekp(aMapMetadata.TilesetAddress, ios::beg);
        ROMFile.read((char*)(&Byte), 1);
        if (Byte != 0x20) {
            return FAILURE;
        }
        ROMFile.seekg(4, ios::cur);
        ROMFile.read((char*)(&AddressBytes[0]), 3);

        GetTilesetData(ROMFile,
                       aMapData,
                       ReadPointer(AddressBytes));


        /*** Graphics data */
        ROMFile.seekp(aMapMetadata.GraphicsAddress, ios::beg);
        ROMFile.read((char*)(&Byte), 1);
        if (Byte != 0x80) {
            return FAILURE;
        }
        ROMFile.read((char*)(&Byte), 1);
        if (Byte == 0x80) {
            /* 0x80 means the data is clear */
            IsGraphicsDataCompressed = false;
        }
        else if (Byte != 0x00) {
            /* 0x00 means the data is compressed */
            return FAILURE;
        }
        ROMFile.seekg(2, ios::cur);
        ROMFile.read((char*)(&AddressBytes[0]), 3);

        GetGraphicsData(ROMFile,
                        aMapData,
                        ReadPointer(AddressBytes),
                        IsGraphicsDataCompressed);


        /*** Palette data */
        ROMFile.seekp(aMapMetadata.PaletteAddress, ios::beg);
        ROMFile.read((char*)(&Byte), 1);
        if (Byte != 0x40) {
            return FAILURE;
        }
        ROMFile.seekg(1, ios::cur);
        ROMFile.read((char*)(&NbColors), 1);
        ROMFile.seekg(1, ios::cur);
        ROMFile.read((char*)(&AddressBytes[0]), 3);

        GetPaletteData(ROMFile,
                       aMapData,
                       ReadPointer(AddressBytes),
                       NbColors);


        return SUCCESS;
    }



    void GetMapArrangementData(fstream &ROMFile,
                               MapData &aMapData,
                               int MapDataAddress) {

        unsigned char Width, Height, LengthHundreds, LengthUnits;
        int DataLength, CompressedDataLength, CurrentByte;

        /* Get the width/height in number of screens + length of the decompressed data */
        ROMFile.seekp(MapDataAddress, ios::beg);
        ROMFile.read((char*)(&Width), 1);
        ROMFile.read((char*)(&Height), 1);
        ROMFile.read((char*)(&LengthUnits), 1);
        ROMFile.read((char*)(&LengthHundreds), 1);
        DataLength = (0x100 * LengthHundreds) + LengthUnits;
        cout << "DataLength = " << DataLength << endl;
        cout << "Data Address = " << MapDataAddress << endl;

        /* Allocate the decompressed data buffer */
        vector<unsigned char> DecompressedData(DataLength);

        /* Decompress the map arrangement data */
        Decompress(ROMFile,
                   DecompressedData,
                   MapDataAddress + 4, /* +4 to ignore the leading width/height/length bytes */
                   DataLength,
                   CompressedDataLength);

        /* Update map data */
        aMapData.MapTiles.clear();
        aMapData.NbScreensX = (int)Width;
        aMapData.NbScreensY = (int)Height;
        aMapData.CompressedDataLength = CompressedDataLength;
        for (CurrentByte = 0; CurrentByte < DataLength; CurrentByte++) {
            aMapData.MapTiles.push_back(DecompressedData[CurrentByte]);
        }
    }


    void GetTilesetData(fstream &ROMFile,
                        MapData &aMapData,
                        int TilesetAddress) {

        unsigned char LengthHundreds, LengthUnits;
        int DataLength, CompressedDataLength, CurrentTileData, CurrentByte;

        /* Get the length of the decompressed data */
        ROMFile.seekp(TilesetAddress, ios::beg);
        ROMFile.read((char*)(&LengthUnits), 1);
        ROMFile.read((char*)(&LengthHundreds), 1);
        DataLength = (0x100 * LengthHundreds) + LengthUnits;

        /* Allocate the decompressed data buffer */
        vector<unsigned char> DecompressedData(DataLength);

        /* Decompress the map arrangement data */
        Decompress(ROMFile,
                   DecompressedData,
                   TilesetAddress + 2, /* +2 to ignore the leading length bytes */
                   DataLength,
                   CompressedDataLength);

        /* Update tileset data */
        aMapData.Tile16Data.clear();
        unsigned char Tile16ByteData[8];
        for (CurrentTileData = 0;
             CurrentTileData < DataLength/8 && CurrentTileData < MAX_TILE16;
             CurrentTileData++) {
            for (CurrentByte = 0; CurrentByte < 8; CurrentByte++) {
                Tile16ByteData[CurrentByte] = DecompressedData[8*CurrentTileData + CurrentByte];
            }
            aMapData.InsertTile16Data(Tile16ByteData);
        }
    }


    void GetGraphicsData(fstream &ROMFile,
                         MapData &aMapData,
                         int GraphicsAddress,
                         bool bDataIsCompressed) {

        int Tile8Index;
        unsigned char Tile8ByteData[32];

        /* Clear the currently stored Tile8Data now */
        aMapData.Tile8Data.clear();

        if (bDataIsCompressed) {

            unsigned char LengthHundreds, LengthUnits;
            int DataLength, CompressedDataLength;

            /* Get the length of the decompressed data */
            ROMFile.seekp(GraphicsAddress, ios::beg);
            ROMFile.read((char*)(&LengthUnits), 1);
            ROMFile.read((char*)(&LengthHundreds), 1);
            DataLength = (0x100 * LengthHundreds) + LengthUnits;

            /* Allocate the decompressed data buffer */
            vector<unsigned char> DecompressedData(DataLength);

            /* Decompress the map arrangement data */
            Decompress(ROMFile,
                       DecompressedData,
                       GraphicsAddress + 2, /* +2 to ignore the leading length bytes */
                       DataLength,
                       CompressedDataLength);

            /* Update graphics data */
            int CurrentByte;
            for (Tile8Index = 0; Tile8Index < 256; Tile8Index++) {
                for (CurrentByte = 0; CurrentByte < 32; CurrentByte++) {
                    Tile8ByteData[CurrentByte] = DecompressedData[32*Tile8Index + CurrentByte];
                }
                aMapData.InsertTile8Data(Tile8ByteData);
            }
        }
        else {

            /* Read and update graphics data directly */
            /* Uncompressed graphics seem to always be on 0x2000 bytes (256 * 32) */
            ROMFile.seekp(GraphicsAddress, ios::beg);
            for (Tile8Index = 0; Tile8Index < 256; Tile8Index++) {
                ROMFile.read((char*)(&Tile8ByteData[0]), 32);
                aMapData.InsertTile8Data(Tile8ByteData);
            }
        }

    }


    void GetPaletteData(fstream &ROMFile,
                        MapData &aMapData,
                        int PaletteAddress,
                        int NbColors) {

        /* TODO: read the actual number of bytes */
        ROMFile.seekp(PaletteAddress, ios::beg);

        /* Update palette data */
        aMapData.PaletteData.clear();
        int ColorIndex;
        unsigned char ColorData[2];
        for (ColorIndex = 0; ColorIndex < NbColors; ColorIndex++) {
            ROMFile.read((char*)(&ColorData[0]), 2);
            aMapData.InsertColorData(ColorData[1], ColorData[0]); /* Reverse the order of the two bytes */
        }
    }



    void InsertMapDataIntoROM(fstream &ROMFile,
                              MapMetadata &aMapMetadata,
                              MapData &aMapData) {

        int DataLength = aMapData.NbScreensX * aMapData.NbScreensY * 0x100;
        vector<unsigned char> DataToCompress(DataLength);
        vector<unsigned char> CompressedData;

        /* Copy Map Data into the buffer of data to compress */
        for (int CurrentByte = 0; CurrentByte < DataLength; CurrentByte++) {
            DataToCompress[CurrentByte] = (unsigned char)(aMapData.MapTiles[CurrentByte]);
        }

        /* Perform the compression */
        Compress(DataToCompress, CompressedData);

        /* Decide at which address we should write the new compressed map data.
         *   - If it is shorter or as long as the original data, overwrite it.
         *   - Otherwise, write at a certain address after 0x100000. */
        int Address;
        unsigned char Byte;
        ROMFile.seekp(aMapMetadata.MapArrangementAddress, ios::beg);
        ROMFile.seekg(2, ios::cur);
        if ((int)CompressedData.size() <= aMapData.CompressedDataLength) {
            /* Get the address of the data in ROM */
            unsigned char AddressBytes[3];
            ROMFile.read((char*)(&AddressBytes[0]), 3);
            Address = ReadPointer(AddressBytes);
        }
        else {
            /* Calculate the new address */
            Address = 0x100000;

            /* Update pointer in Map Metadata in ROM */
            Byte = Address % 0x100;
            ROMFile.write((char*)(&Byte), 1);
            Byte = (Address % 0x10000) / 0x100;
            ROMFile.write((char*)(&Byte), 1);
            Byte = Address / 0x10000;
            ROMFile.write((char*)(&Byte), 1);
        }

        /* Write into ROM */
        ROMFile.seekp(Address, ios::beg);
        Byte = (unsigned char)(aMapData.NbScreensX);
        ROMFile.write((char*)(&Byte), 1);
        Byte = (unsigned char)(aMapData.NbScreensY);
        ROMFile.write((char*)(&Byte), 1);
        Byte = (unsigned char)(DataLength % 0x100);
        ROMFile.write((char*)(&Byte), 1);
        Byte = (unsigned char)(DataLength / 0x100);
        ROMFile.write((char*)(&Byte), 1);
        ROMFile.write((char*)(&CompressedData[0]), CompressedData.size());
    }



    void Compress(vector<unsigned char> &DataToCompress,
                  vector<unsigned char> &CompressedData) {

        int CurrentIndex      = SEARCH_SIZE;
        int CurrentOutputBit  = 0;
        unsigned char CurrentOutputByte = 0;
        int BestIndex, BestLength, CompareLimit, CurrentLength, i, j;

        /* Prepare memory buffer: left-pad with with 0x20 values */
        for (i = 0; i < SEARCH_SIZE; i++) {
            DataToCompress.insert(DataToCompress.begin(), 0x20);
        }
        int InputBufferSize = DataToCompress.size();


        /* Compression algorithm */
        while (CurrentIndex < InputBufferSize) {
            BestIndex  = 0;
            BestLength = 0;

            /* Look for a match in the search buffer (brute force search) */
            for (i = 0; i < SEARCH_SIZE; i++) {

                /* Don't compare past the end of the lookahead buffer, or past the whole data buffer */
                CompareLimit = MIN(LOOKAHEAD_SIZE + 1, InputBufferSize - CurrentIndex);

                /* Compare the search buffer to the lookahead buffer and
                   count how many sequential bytes match (possibly zero) */
                CurrentLength = 0;
                for (j = 0; j < CompareLimit; j++) {
                    if (DataToCompress[CurrentIndex - SEARCH_SIZE + i + j] == DataToCompress[CurrentIndex + j]) {
                        CurrentLength += 1;
                    }
                    else {
                        break;
                    }
                }

                /* Update the largest match found yet */
                if (CurrentLength > BestLength) {
                    BestIndex  = CurrentIndex - SEARCH_SIZE + i;
                    BestLength = CurrentLength;
                }
            }

            /* Write the next block of compressed output */
            if (BestLength >= 2) {
                /* The Pastcopy source values must be offset by 0xEF for some unknown reason */
                BestIndex = (BestIndex + 0xEF) & 0xFF;
                COPY_BIT(0);
                COPY_BYTE((unsigned char)BestIndex);
                COPY_4_BITS((unsigned char)(BestLength - 2));
                CurrentIndex += BestLength;
            }
            else {
                /* No match found: this is a Literal copy case */
                COPY_BIT(1);
                COPY_BYTE(DataToCompress[CurrentIndex]);
                CurrentIndex += 1;
            }
        }

    }


    void Decompress(fstream &ROMFile,
                    vector<unsigned char> &DecompressedData,
                    int Address,
                    int DecompressedDataLength,
                    int &CompressedDataLength) {

        int InputByteIndex, OutputByteIndex;
        unsigned char InputByte, OutputByte;
        bool InputBits[8 * DecompressedDataLength];

        /* Read compressed input stream */
        ROMFile.seekp(Address, ios::beg);
        for (InputByteIndex = 0; InputByteIndex < DecompressedDataLength; InputByteIndex++) {
            ROMFile.read((char*)(&InputByte), 1);
            InputBits[0 + 8*InputByteIndex] = InputByte & 0x80;
            InputBits[1 + 8*InputByteIndex] = InputByte & 0x40;
            InputBits[2 + 8*InputByteIndex] = InputByte & 0x20;
            InputBits[3 + 8*InputByteIndex] = InputByte & 0x10;
            InputBits[4 + 8*InputByteIndex] = InputByte & 0x08;
            InputBits[5 + 8*InputByteIndex] = InputByte & 0x04;
            InputBits[6 + 8*InputByteIndex] = InputByte & 0x02;
            InputBits[7 + 8*InputByteIndex] = InputByte & 0x01;
        }


        int NbBytesToWrite, CopyOffset;
        int NbBytesWritten = 0;
        int InputBitIndex = 0;

        /* Decompression algorithm */
        while (NbBytesWritten < DecompressedDataLength) {

            if (InputBits[InputBitIndex] == 1) {
                /* 1 => Literal case: directly copy the following byte to the output stream. */
                OutputByte = 0;
                OutputByte += (InputBits[InputBitIndex + 1] * 0x80);
                OutputByte += (InputBits[InputBitIndex + 2] * 0x40);
                OutputByte += (InputBits[InputBitIndex + 3] * 0x20);
                OutputByte += (InputBits[InputBitIndex + 4] * 0x10);
                OutputByte += (InputBits[InputBitIndex + 5] * 0x08);
                OutputByte += (InputBits[InputBitIndex + 6] * 0x04);
                OutputByte += (InputBits[InputBitIndex + 7] * 0x02);
                OutputByte += (InputBits[InputBitIndex + 8] * 0x01);

                /* Copy the byte and update */
                DecompressedData[NbBytesWritten] = OutputByte;
                NbBytesWritten++;
                InputBitIndex += 9;
            }
            else {
                /* 0 => Repeat case: copy the indicated number of bytes located at the specified offset. */
                NbBytesToWrite = 0;
                NbBytesToWrite += (InputBits[InputBitIndex +  9] * 0x8);
                NbBytesToWrite += (InputBits[InputBitIndex + 10] * 0x4);
                NbBytesToWrite += (InputBits[InputBitIndex + 11] * 0x2);
                NbBytesToWrite += (InputBits[InputBitIndex + 12] * 0x1);
                NbBytesToWrite += 2;

                /* Get the offset; it starts from 0xEF for some unknown reason. */
                CopyOffset = 0;
                CopyOffset += (InputBits[InputBitIndex + 1] * 0x80);
                CopyOffset += (InputBits[InputBitIndex + 2] * 0x40);
                CopyOffset += (InputBits[InputBitIndex + 3] * 0x20);
                CopyOffset += (InputBits[InputBitIndex + 4] * 0x10);
                CopyOffset += (InputBits[InputBitIndex + 5] * 0x08);
                CopyOffset += (InputBits[InputBitIndex + 6] * 0x04);
                CopyOffset += (InputBits[InputBitIndex + 7] * 0x02);
                CopyOffset += (InputBits[InputBitIndex + 8] * 0x01);
                CopyOffset = (CopyOffset + 0x11) % 0x100;

                /* Copy the bytes and update */
                CopyOffset += ((NbBytesWritten >> 8) << 8);
                if (CopyOffset >= NbBytesWritten) {
                    CopyOffset -= 0x100;
                }
                for (OutputByteIndex = 0; OutputByteIndex < NbBytesToWrite; OutputByteIndex++) {
                    DecompressedData[NbBytesWritten + OutputByteIndex] = DecompressedData[CopyOffset + OutputByteIndex];
                }
                NbBytesWritten += NbBytesToWrite;
                InputBitIndex += 13;
            }
        }

        /* Calculate and return the length (in bytes) of the compressed data */
        CompressedDataLength = (InputBitIndex + 6) / 8;
    }

}
