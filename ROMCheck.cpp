#include "ROMCheck.h"

#define HEADER_OFFSET 0x200


bool CheckOriginalROM(fstream &ROMFile,
                      bool    &bROMIsHeadered,
                      bool    &bROMIsExtended) {

    int ROMStartOffset = 0;
    unsigned char FileStartSample[4];
    ROMFile.seekg(0, ios::beg);
    ROMFile.read((char*)(&(FileStartSample[0])), 4);

    /* Read first bytes to identify if this ROM is the headered or the un-headered one.
       I should do this with an actual hash or checksum later. */
    if (FileStartSample[0] == 0x78 &&
        FileStartSample[1] == 0xD8 &&
        FileStartSample[2] == 0x18 &&
        FileStartSample[3] == 0xFB) {
        bROMIsHeadered = false;
    }
    else if (FileStartSample[0] == 0x80 &&
             FileStartSample[1] == 0x00 &&
             FileStartSample[2] == 0x00 &&
             FileStartSample[3] == 0x00) {
        bROMIsHeadered = true;
        ROMStartOffset = HEADER_OFFSET;
    }

    /* Check if this is really the SOUL BLAZER U.S. version */
    unsigned char FileTitleSample[24];
    ROMFile.seekg(0x7FC0 + ROMStartOffset, ios::beg);
    ROMFile.read((char*)(&(FileTitleSample[0])), 24);

    if (FileTitleSample[0]  == 0x53 && /* S */
        FileTitleSample[1]  == 0x4F && /* O */
        FileTitleSample[2]  == 0x55 && /* U */
        FileTitleSample[3]  == 0x4C && /* L */
        FileTitleSample[4]  == 0x42 && /* B */
        FileTitleSample[5]  == 0x4C && /* L */
        FileTitleSample[6]  == 0x41 && /* A */
        FileTitleSample[7]  == 0x5A && /* Z */
        FileTitleSample[8]  == 0x45 && /* E */
        FileTitleSample[9]  == 0x52 && /* R */
        FileTitleSample[10] == 0x20 && /*   */
        FileTitleSample[11] == 0x2D && /* - */
        FileTitleSample[12] == 0x20 && /*   */
        FileTitleSample[13] == 0x31 && /* 1 */
        FileTitleSample[14] == 0x20 && /*   */
        FileTitleSample[15] == 0x55 && /* U */
        FileTitleSample[16] == 0x53 && /* S */
        FileTitleSample[17] == 0x41) { /* A */

        /* ROM is correct; let's check if it's already extended */
        if (FileTitleSample[23] == 0x0A) {
            /* Not extended */
            bROMIsExtended = false;
            return true;
        }
        else if (FileTitleSample[23] == 0x0B) {
            /* Extended to 2 Mo */
            bROMIsExtended = true;
            return true;
        }

        /* Unsupported extension type */
        return false;
    }

    /* Incorrect ROM file */
    return false;
}
