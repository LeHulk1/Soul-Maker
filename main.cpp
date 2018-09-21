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

#include "resource.h"

#include "MapData.h"
#include "MapDataTools.h"
#include "ROMCheck.h"


#define ID_FILE_EXIT 9001
#define ID_STUFF_GO 9002

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/* Declare Dialog procedure */
BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");


/* Global fstream */
static fstream gROMFile;

/* Global Map Data container */
static MapData gMapData;

static bool bMapLoaded = false;


using namespace std;

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

                    if(GetOpenFileName(&ofn)) {
                        // Do something usefull with the filename stored in szFileName
                        gROMFile.open(szFileName, ios::in | ios::out | ios::binary | ios::ate);
                        if (!gROMFile.is_open()) {
                            cout << "ROM file \"" << szFileName << "\" is not found!\n";
                            return 1;
                        }
//                        unsigned char FileStartSample[5];
//                        ROMFile.seekg(0, ios::beg);
//                        ROMFile.read((char*)(&(FileStartSample[0])), 1);
//                        ROMFile.read((char*)(&(FileStartSample[1])), 1);
//                        ROMFile.read((char*)(&(FileStartSample[2])), 1);
//                        ROMFile.read((char*)(&(FileStartSample[3])), 1);
//                        FileStartSample[4] = '\0';

//                        LPCSTR FileStartSampleSTR[100] = {0};
//                        for (int i = 0; i < 4; i++) {
//                            FileStartSampleSTR << hex << setw(2) << setfill('0') << int(FileStartSample[i]) << ' ';
//                        }
//                        MessageBox(hwnd, (LPCSTR)FileStartSample, "Notice", MB_OK | MB_ICONINFORMATION);
//
//                        LPSTR pszFileText;
//                        pszFileText = (char*)GlobalAlloc(GPTR, 10);
//                        for (int i = 0; i < 4; i++) {pszFileText[i] = FileStartSample[i];}
//                        SetWindowText(hwnd, pszFileText);

                        ROMStatus Status = CheckOriginalROM(gROMFile);

                        if (Status != UNKNOWN) {
                            //MessageBox(hwnd, (Status == UNHEADERED ? "Un-headered ROM." : "Headered ROM."), "Notice", MB_OK | MB_ICONINFORMATION);


                            //MapDataTools::Decompress(gROMFile, gMapData, 0xF653B, 0);

                            MapDataTools::GetMapArrangementData(gROMFile, gMapData, 0xF18E2); /*(10)*/

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

                            MapDataTools::GetTilesetData(gROMFile, gMapData, 0x48200); /*(20)*/

//                            cout << endl;
//                            cout << endl;
//                            for (int i = 0; i < (int)gMapData.Tile16Data.size(); i++) {
//                                for (int j = 0; j < 4; j++) {
//                                    cout << hex << setw(2) << setfill('0')
//                                         << gMapData.Tile16Data[i].HorizontalFlip[j] << ' ' << gMapData.Tile16Data[i].Tile8Data[j] << ' ';
//                                }
//                                cout << endl;
//                            }

                            MapDataTools::GetGraphicsData(gROMFile, gMapData, 0x706EF, true); /*(80)*/

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

                            MapDataTools::GetPaletteData(gROMFile, gMapData, 0xF3A6B); /*(40)*/

//                            cout << endl;
//                            cout << endl;
//                            for (int ColorIdx = 0; ColorIdx < (int)gMapData.PaletteData.size(); ColorIdx++) {
//                                cout << hex << setw(2) << setfill('0') << gMapData.PaletteData[ColorIdx].Red*8 << '-'
//                                            << setw(2) << setfill('0') << gMapData.PaletteData[ColorIdx].Green*8 << '-'
//                                            << setw(2) << setfill('0') << gMapData.PaletteData[ColorIdx].Blue*8 << endl;
//                            }

                            bMapLoaded = true;
                        }
//


                    }
                }
                break;
                case ID_FILE_EXIT:
                    PostMessage(hwnd, WM_CLOSE, 0, 0);
                break;
                case ID_STUFF_GO:
                {
                    int ret = DialogBox(GetModuleHandle(NULL),
                        MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutDlgProc);
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


                RECT rect;
                GetClientRect(hwnd, &rect); // To get size of window
                int win_width = rect.right - rect.left;
                int win_height = rect.bottom + rect.left;

                HDC hDCMem = CreateCompatibleDC(hdc); // Create a device context in memory
                //int savedDC = SaveDC(hDCMem);
                HBITMAP Membitmap = CreateCompatibleBitmap(hdc, win_width, win_height);
                SelectObject(hDCMem, Membitmap);

                int CurrentTile16, Tile8Index, CurrentPixel;
                int x, y, ScreenX, ScreenY, Tile16X, Tile16Y, Tile8InTile16, Palette;
                bool Hflip, Vflip;
                Tile8 CurrentTile8;

                for (ScreenX = 0; ScreenX < gMapData.NbScreensX; ScreenX++) {
                    for (ScreenY = 0; ScreenY < gMapData.NbScreensY; ScreenY++) {
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
                                            SetPixelV(hDCMem,
                                                     256*ScreenX + 16*Tile16X + x + ((Tile8InTile16 & 0x01) ? 8 : 0),
                                                     256*ScreenY + 16*Tile16Y + y + ((Tile8InTile16 & 0x02) ? 8 : 0),
                                                     RGB(gMapData.PaletteData[CurrentPixel].Red,
                                                         gMapData.PaletteData[CurrentPixel].Green,
                                                         gMapData.PaletteData[CurrentPixel].Blue));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                // Now BitBlt the stuff from memory buffer to screen
                BitBlt(hdc, 0, 0, win_width, win_height, hDCMem, 0, 0, SRCCOPY);

                // Cleanup
                DeleteObject(Membitmap);
                DeleteDC    (hDCMem);
                DeleteDC    (hdc);
            }

            EndPaint(hwnd, &ps);
            break;
        }
        case WM_LBUTTONDOWN:
        {
//            char szFileName[MAX_PATH];
//            HINSTANCE hInstance = GetModuleHandle(NULL);
//
//            GetModuleFileName(hInstance, szFileName, MAX_PATH);
//            MessageBox(hwnd, szFileName, "This program is:", MB_OK | MB_ICONINFORMATION);
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

        return TRUE;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                    EndDialog(hwnd, IDOK);
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
