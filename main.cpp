#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>

#include "resource.h"

#include "MapData.h"
#include "MapDataTools.h"
#include "MapMetadata.h"
#include "ROMCheck.h"


#define ID_FILE_EXIT 9001
#define ID_STUFF_GO 9002

#define NB_MAPS 97


using namespace std;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/* Declare Dialog procedure */
BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");


HWND hWndComboBox;       /* Combobox handle */

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

/* Global fstream */
static fstream gROMFile;

/* Global Map Data container */
static MapData gMapData;

/* Global Map Metadata container */
static vector<MapMetadata> gMapMetadata;

/* Golbal Map ID */
static int gMapID = 0;

static bool bMapLoaded = false;
static bool bReloadMap = true;



int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */

    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0) > 0)
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* Close ROM file stream */
    gROMFile.close();
    gROMFile.clear();

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
//            hWndComboBox = CreateWindow("COMBOBOX",
//                                        NULL,
//                                        WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | WS_VSCROLL,
//                                        60, 62, 136, 160,
//                                        hwnd,
//                                        (HMENU)IDC_COMBOBOX_TEXT,
//                                        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
//                                        NULL);
//
//            if( !hWndComboBox )
//            {
//                MessageBox(hwnd,
//                           "Could not create the combo box",
//                           "Failed Control Creation",
//                           MB_OK);
//
//            }
//
//            SendMessage(hWndComboBox,
//                        CB_ADDSTRING,
//                        0,
//                        reinterpret_cast<LPARAM>((LPCTSTR)ComboBoxItems[0]));
//            SendMessage(hWndComboBox,
//                        CB_ADDSTRING,
//                        0,
//                        reinterpret_cast<LPARAM>((LPCTSTR)ComboBoxItems[1]));
//            SendMessage(hWndComboBox,
//                        CB_ADDSTRING,
//                        0,
//                        reinterpret_cast<LPARAM>((LPCTSTR)ComboBoxItems[2]));
//            SendMessage(hWndComboBox,
//                        CB_ADDSTRING,
//                        0,
//                        reinterpret_cast<LPARAM>((LPCTSTR)ComboBoxItems[3]));
//            SendMessage(hWndComboBox,
//                        CB_ADDSTRING,
//                        0,
//                        reinterpret_cast<LPARAM>((LPCTSTR)ComboBoxItems[4]));
            break;


        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case ID_FILE_OPEN:
                {
                    OPENFILENAME ofn;
                    char szFileName[MAX_PATH] = "";

                    ZeroMemory(&ofn, sizeof(ofn));

                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hwnd;
                    ofn.lpstrFilter = "smc Files (*.smc)\0*.smc\0All Files (*.*)\0*.*\0";
                    ofn.lpstrFile = szFileName;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
                    ofn.lpstrDefExt = "txt";

                    if(!GetOpenFileName(&ofn)) {
                        break;
                    }
                    // Do something usefull with the filename stored in szFileName
                    gROMFile.open(szFileName, ios::in | ios::out | ios::binary | ios::ate);
                    if (!gROMFile.is_open()) {
                        cout << "ROM file \"" << szFileName << "\" is not found!\n";
                        return 1;
                    }

                    ROMStatus Status = CheckOriginalROM(gROMFile);

                    if (Status == UNKNOWN) {
                        MessageBox(hwnd, "ROM unknown!", "Notice",
                            MB_OK | MB_ICONINFORMATION);
                        break;
                    }

                    /* Get Map metadata */
                    GetMapMetadata(gMapMetadata);

                    /* Get data for one map */
//                    int MapLoadStatus;
//                    MapLoadStatus = MapDataTools::GetMapData(gROMFile,
//                                                             gMapMetadata[gMapID],
//                                                             gMapData);
//
//                    if (MapLoadStatus == FAILURE) {
//                       MessageBox(hwnd, "Failed to read map metadata!", "Notice", MB_OK | MB_ICONINFORMATION);
//                    }

                    //MapDataTools::GetMapArrangementData(gROMFile, gMapData, 0x9F448); /*(10)*/

//                            cout << endl;
//                            cout << endl;
//                            cout << gMapData.NbScreensColumns << " " << gMapData.NbScreensLines << endl;
//                            cout << endl;
//                            for (int SX = 0; SX < gMapData.NbScreensX; SX++) {
//                                for (int SY = 0; SY < gMapData.NbScreensY; SY++) {
//                                    for (int X = 0; X < 16; X++) {
//                                        for (int Y = 0; Y < 16; Y++) {
//                                            cout << hex << setw(2) << setfill('0') << gMapData.GetTile16(SX, SY, X, Y) << ' ';
//                                        }
//                                        cout << endl;
//                                    }
//                                    cout << endl;
//                                }
//                            }

                    //MapDataTools::GetTilesetData(gROMFile, gMapData, 0x97A0F); /*(20)*/

//                            cout << endl;
//                            cout << endl;
//                            for (int i = 0; i < (int)gMapData.Tile16Data.size(); i++) {
//                                for (int j = 0; j < 4; j++) {
//                                    cout << hex << setw(2) << setfill('0')
//                                         << gMapData.Tile16Data[i].HorizontalFlip[j] << ' ' << gMapData.Tile16Data[i].Tile8Data[j] << ' ';
//                                }
//                                cout << endl;
//                            }

                    //MapDataTools::GetGraphicsData(gROMFile, gMapData, 0xA436B, true); /*(80)*/

//                            cout << endl;
//                            cout << endl;
//                            for (int T8Idx = 0; T8Idx < (int)gMapData.Tile8Data.size(); T8Idx++) {
//                                for (int x = 0; x < 8; x++) {
//                                    for (int y = 0; y < 8; y++) {
//                                        cout << hex << setw(1) << setfill('0') << gMapData.Tile8Data[T8Idx].Pixel(x, y) << ' ';
//                                    }
//                                    cout << endl;
//                                }
//                                cout << endl;
//                                cout << endl;
//                            }

                    //MapDataTools::GetPaletteData(gROMFile, gMapData, 0xF6E12); /*(40)*/

//                            cout << endl;
//                            cout << endl;
//                            for (int ColorIdx = 0; ColorIdx < (int)gMapData.PaletteData.size(); ColorIdx++) {
//                                cout << hex << setw(2) << setfill('0') << gMapData.PaletteData[ColorIdx].Red*8 << '-'
//                                            << setw(2) << setfill('0') << gMapData.PaletteData[ColorIdx].Green*8 << '-'
//                                            << setw(2) << setfill('0') << gMapData.PaletteData[ColorIdx].Blue*8 << endl;
//                            }

                    bMapLoaded = true;

                    EnableMenuItem(GetMenu(hwnd), ID_STUFF_GO, MF_BYCOMMAND | MF_ENABLED);

                }
                break;
                case ID_FILE_EXIT:
                    PostMessage(hwnd, WM_CLOSE, 0, 0);
                break;
                case ID_STUFF_GO:
                {
                    int ret = DialogBox(GetModuleHandle(NULL),
                        MAKEINTRESOURCE(IDD_SELECTMAP), hwnd, AboutDlgProc);





                    if(ret == IDOK){
                        MessageBox(hwnd, "Dialog exited with IDOK.", "Notice",
                            MB_OK | MB_ICONINFORMATION);
                    }
                    else if(ret == IDCANCEL){
                        MessageBox(hwnd, "Dialog exited with IDCANCEL.", "Notice",
                            MB_OK | MB_ICONINFORMATION);
                    }
                    else if(ret == -1){
                        MessageBox(hwnd, "Dialog failed!", "Error",
                            MB_OK | MB_ICONINFORMATION);
                    }
                }
                break;
            }
        break;
        case WM_PAINT:
        {

            if (bReloadMap) {
                /* Get data for one map */
                int MapLoadStatus;
                MapLoadStatus = MapDataTools::GetMapData(gROMFile,
                                                         gMapMetadata[gMapID],
                                                         gMapData);

                bReloadMap = false;

                if (MapLoadStatus == FAILURE) {
                   //MessageBox(hwnd, "Failed to read map metadata!", "Notice", MB_OK | MB_ICONINFORMATION);
                   break;
                }
            }

            PAINTSTRUCT ps;
            //RECT r;

//            GetClientRect(hwnd, &r);
//
//            if (r.bottom == 0) {
//                break;
//            }

            HDC hdc = BeginPaint(hwnd, &ps);
//            HPEN hPen = CreatePen(PS_NULL, 1, RGB(0, 0, 0));
//            HPEN holdPen = (HPEN)SelectObject(hdc, hPen);
//
////            for (int i=0; i<1000; i++) {
////
////                int x = rand() % r.right;
////                int y = rand() % r.bottom;
////                SetPixel(hdc, x, y, RGB(255, 0, 0));
////            }
//
//            HBRUSH hBrush1 = CreateSolidBrush(RGB(121, 90, 0));
//            HBRUSH hBrush2 = CreateSolidBrush(RGB(240, 63, 19));
//            HBRUSH hBrush3 = CreateSolidBrush(RGB(240, 210, 18));
//            HBRUSH hBrush4 = CreateSolidBrush(RGB(9, 189, 21));
//
//            HBRUSH holdBrush = (HBRUSH)SelectObject(hdc, hBrush1);
//
//            Rectangle(hdc, 30, 30, 100, 100);
//            SelectObject(hdc, hBrush2);
//            Rectangle(hdc, 110, 30, 180, 100);
//            SelectObject(hdc, hBrush3);
//            Rectangle(hdc, 30, 110, 100, 180);
//            SelectObject(hdc, hBrush4);
//            Rectangle(hdc, 110, 110, 180, 180);
//
//            SelectObject(hdc, holdPen);
//            SelectObject(hdc, holdBrush);
//
//            DeleteObject(hPen);
//            DeleteObject(hBrush1);
//            DeleteObject(hBrush2);
//            DeleteObject(hBrush3);
//            DeleteObject(hBrush4);


            if (bMapLoaded) {

                /* Prepare for drawing */
//                RECT rect;
//                GetClientRect(hwnd, &rect); // To get size of window
//                int win_width = rect.right - rect.left;
//                int win_height = rect.bottom + rect.left;

                HDC hDCMem = CreateCompatibleDC(hdc); // Create a device context in memory
                //int savedDC = SaveDC(hDCMem);
//                HBITMAP Membitmap = CreateCompatibleBitmap(hdc, win_width, win_height);
//                SelectObject(hDCMem, Membitmap);


                unsigned char* lpBitmapBits;

                int W = gMapData.NbScreensX*256 + 276;
                int H = gMapData.NbScreensY*256;

	BITMAPINFO bi;
	ZeroMemory(&bi, sizeof(BITMAPINFO));
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = W;//m_rcMainRect.Width();
	bi.bmiHeader.biHeight = -H;//-m_rcMainRect.Height();  //negative so (0,0) is at top left
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;


	HBITMAP bitmap = ::CreateDIBSection(hDCMem, &bi, DIB_RGB_COLORS,  (VOID**)&lpBitmapBits, NULL, 0);
	HGDIOBJ oldbmp = ::SelectObject(hDCMem, bitmap);

	BitBlt(hDCMem, 0, 0, W, H, hdc, 0, 0, SRCCOPY);


                /*** Draw Map ***/

                int NbScreensX = gMapData.NbScreensX;
                int NbScreensY = gMapData.NbScreensY;
                int CurrentTile16, Tile8Index, CurrentPixel;
                int x, y, ScreenX, ScreenY, Tile16X, Tile16Y, Tile8InTile16, Palette;
                bool Hflip, Vflip;
                Tile8 CurrentTile8;

                int X,Y,index;
                int pitch = 4*W;

                for (ScreenX = 0; ScreenX < NbScreensX; ScreenX++) {
                    for (ScreenY = 0; ScreenY < NbScreensY; ScreenY++) {
                        for (Tile16X = 0; Tile16X < 16; Tile16X++) {
                            for (Tile16Y = 0; Tile16Y < 16; Tile16Y++) {

                                CurrentTile16 = gMapData.GetTile16(ScreenX, ScreenY, Tile16X, Tile16Y);

                                for (Tile8InTile16 = 0; Tile8InTile16 < 4; Tile8InTile16++) {

                                    Tile8Index = gMapData.Tile16Data[CurrentTile16].Tile8Data[Tile8InTile16];
                                    Hflip = gMapData.Tile16Data[CurrentTile16].HorizontalFlip[Tile8InTile16];
                                    Vflip = gMapData.Tile16Data[CurrentTile16].VerticalFlip[Tile8InTile16];
                                    Palette = gMapData.Tile16Data[CurrentTile16].Palette[Tile8InTile16] - 1;
                                    CurrentTile8 = gMapData.Tile8Data[Tile8Index];

                                    for (x = 0; x < 8; x++) {
                                        for (y = 0; y < 8; y++) {
                                            CurrentPixel = CurrentTile8.Pixel((Hflip?(7-x):x), (Vflip?(7-y):y)) + 0x10*Palette;
//                                            SetPixelV(hDCMem,
//                                                      256*ScreenX + 16*Tile16X + x + ((Tile8InTile16 & 0x01) ? 8 : 0),
//                                                      256*ScreenY + 16*Tile16Y + y + ((Tile8InTile16 & 0x02) ? 8 : 0),
//                                                      RGB(gMapData.PaletteData[CurrentPixel].Red,
//                                                          gMapData.PaletteData[CurrentPixel].Green,
//                                                          gMapData.PaletteData[CurrentPixel].Blue));
                                            X = 256*ScreenX + 16*Tile16X + x + ((Tile8InTile16 & 0x01) ? 8 : 0);
                                            Y = 256*ScreenY + 16*Tile16Y + y + ((Tile8InTile16 & 0x02) ? 8 : 0);
                                            index = Y * pitch;
                                            index += X*4;
                                            lpBitmapBits[index + 0] = gMapData.PaletteData[CurrentPixel].Blue;  // blue
                                            lpBitmapBits[index + 1] = gMapData.PaletteData[CurrentPixel].Green; // green
                                            lpBitmapBits[index + 2] = gMapData.PaletteData[CurrentPixel].Red;  // red
                                        }
                                    }
                                }
                            }
                        }
                    }
                }


                /*** Draw tileset ***/

                int Tile16Index = 0;
                Tile16X = 0;
                Tile16Y = 0;

                while (Tile16Index < (int)gMapData.Tile16Data.size()) {
                    for (Tile8InTile16 = 0; Tile8InTile16 < 4; Tile8InTile16++) {

                        Tile8Index = gMapData.Tile16Data[Tile16Index].Tile8Data[Tile8InTile16];
                        Hflip = gMapData.Tile16Data[Tile16Index].HorizontalFlip[Tile8InTile16];
                        Vflip = gMapData.Tile16Data[Tile16Index].VerticalFlip[Tile8InTile16];
                        Palette = gMapData.Tile16Data[Tile16Index].Palette[Tile8InTile16] - 1;
                        CurrentTile8 = gMapData.Tile8Data[Tile8Index];

                        for (x = 0; x < 8; x++) {
                            for (y = 0; y < 8; y++) {
                                CurrentPixel = CurrentTile8.Pixel((Hflip?(7-x):x), (Vflip?(7-y):y)) + 0x10*Palette;
//                                SetPixelV(hDCMem,
//                                          256*NbScreensX + 16*Tile16X + x + ((Tile8InTile16 & 0x01) ? 8 : 0) + 20,
//                                                           16*Tile16Y + y + ((Tile8InTile16 & 0x02) ? 8 : 0),
//                                          RGB(gMapData.PaletteData[CurrentPixel].Red,
//                                              gMapData.PaletteData[CurrentPixel].Green,
//                                              gMapData.PaletteData[CurrentPixel].Blue));
                                X = 256*NbScreensX + 16*Tile16X + x + ((Tile8InTile16 & 0x01) ? 8 : 0) + 20;
                                Y = 16*Tile16Y + y + ((Tile8InTile16 & 0x02) ? 8 : 0);
                                index = Y * pitch;
                                index += X*4;
                                lpBitmapBits[index + 0] = gMapData.PaletteData[CurrentPixel].Blue;  // blue
                                lpBitmapBits[index + 1] = gMapData.PaletteData[CurrentPixel].Green; // green
                                lpBitmapBits[index + 2] = gMapData.PaletteData[CurrentPixel].Red;  // red
                            }
                        }
                    }
                    if (Tile16X == 15) {
                        Tile16X = 0;
                        Tile16Y++;
                    }
                    else {
                        Tile16X++;
                    }
                    Tile16Index++;
                }

                // Now BitBlt the stuff from memory buffer to screen
                BitBlt(hdc, 0, 0, W, H, hDCMem, 0, 0, SRCCOPY);

                SelectObject(hDCMem,oldbmp);
        DeleteDC(hDCMem);
        DeleteObject(bitmap);

                // Cleanup
                //DeleteObject(Membitmap);
//                DeleteDC    (hDCMem);
//                DeleteDC    (hdc);
            }

            EndPaint(hwnd, &ps);
            break;
        }
        case WM_LBUTTONDOWN:
        {
            break;
        }
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_INITDIALOG:
        {

            hWndComboBox = CreateWindow("COMBOBOX",
                                        NULL,
                                        WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | WS_VSCROLL,
                                        10, 12, 250, 160,
                                        hwnd,
                                        (HMENU)IDC_COMBOBOX_TEXT,
                                        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                                        NULL);

            if( !hWndComboBox )
            {
                MessageBox(hwnd,
                           "Could not create the combo box",
                           "Failed Control Creation",
                           MB_OK);

            }

            //const TCHAR* fontName = _T("Arial");
            const long nFontSize = 8;

//            HDC hdc = GetDC(hwnd);
//
//            LOGFONT logFont = {0};
//            //logFont.lfHeight = -MulDiv(nFontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
//            logFont.lfWeight = FW_DONTCARE;
//            logFont.lfFaceName[0] = '\0';
            //strncpy(logFont.lfFaceName, fontName, 5);

            //HFONT s_hFont = CreateFontIndirect(&logFont);

            HFONT hFont = CreateFont(16,0,0,0,FW_DONTCARE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, VARIABLE_PITCH,TEXT("Calibri"));
            //ReleaseDC(hwnd, hdc);

            for (int i = 0; i < NB_MAPS; i++) {
                SendDlgItemMessage(hwnd, IDC_COMBOBOX_TEXT, CB_ADDSTRING, 0, (LONG)MapNameList[i]);
            }

            SendMessage(hWndComboBox, WM_SETFONT, (WPARAM)hFont, (LPARAM)MAKELONG(TRUE, 0));
            SendDlgItemMessage(hwnd, IDC_COMBOBOX_TEXT, CB_SETCURSEL, gMapID, 0);

        }
        return TRUE;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                {
                    int NewMapID = SendDlgItemMessage(hwnd, IDC_COMBOBOX_TEXT, CB_GETCURSEL, 0, 0);
                    if (gMapID != NewMapID) {
                        gMapID = NewMapID;
                        bReloadMap = true;
                    }
                    EndDialog(hwnd, IDOK);
                }
                break;
                case IDCANCEL:
                    EndDialog(hwnd, IDCANCEL);
                break;
            }
        break;
        default:
            return FALSE;
    }
    return TRUE;
}
