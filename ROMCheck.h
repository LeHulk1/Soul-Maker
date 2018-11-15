#ifndef __ROMCHECK_H__
#define __ROMCHECK_H__

#include <fstream>


using namespace std;

bool CheckOriginalROM(fstream &ROMFile,
                      bool    &bROMIsHeadered,
                      bool    &bROMIsExtended);


#endif // __ROMCHECK_H__
