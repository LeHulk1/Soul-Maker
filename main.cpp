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

#include "DrawingTools.h"
#include "MapData.h"
#include "MapDataTools.h"
#include "MapMetadata.h"
#include "ROMCheck.h"
#include "SelectedTile.h"


#define ID_FILE_EXIT 9001
#define ID_STUFF_GO 9002







using namespace std;

/* Declare Windows procedure */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/* Declare Dialog procedure */
BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");


HWND hWndComboBox;       /* Combobox handle */



/* Global fstream */
static fstream gROMFile;

/* Global Map Data container */
static MapData gMapData;

/* Global Map Metadata container */
static vector<MapMetadata> gMapMetadata;

/* Global Tile Selection object */
static TileSelection gTileSelection;

/* Global Map ID */
static int gMapID = 0;

static bool gMapLoaded = false;
static bool gReloadMap = true;
static bool gLeftClickDown = false;





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
            break;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case ID_FILE_OPEN:
                {
                    OPENFILENAME ofn;
                    char FileName[MAX_PATH] = "";

                    ZeroMemory(&ofn, sizeof(ofn));

                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hwnd;
                    ofn.lpstrFilter = "smc Files (*.smc)\0*.smc\0All Files (*.*)\0*.*\0";
                    ofn.lpstrFile = FileName;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
                    ofn.lpstrDefExt = "txt";

                    if(!GetOpenFileName(&ofn)) {
                        break;
                    }

                    gROMFile.open(FileName, ios::in | ios::out | ios::binary | ios::ate);
                    if (!gROMFile.is_open()) {
                        cout << "ROM file \"" << FileName << "\" is not found!\n";
                        return 1;
                    }

                    ROMStatus Status = CheckOriginalROM(gROMFile);

                    if (Status == UNKNOWN) {
                        MessageBox(hwnd, "ROM unknown!", "Notice", MB_OK | MB_ICONINFORMATION);
                        break;
                    }

                    /* Get Map metadata */
                    GetMapMetadata(gMapMetadata);

                    /* Enable map selection menu and draw map */
                    EnableMenuItem(GetMenu(hwnd), ID_STUFF_GO, MF_BYCOMMAND | MF_ENABLED);
                    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);

                }
                break;
                case ID_FILE_EXIT:
                    PostMessage(hwnd, WM_CLOSE, 0, 0);
                break;
                case ID_STUFF_GO:
                {
                    int ret = DialogBox(GetModuleHandle(NULL),
                                        MAKEINTRESOURCE(IDD_SELECTMAP),
                                        hwnd,
                                        AboutDlgProc);

                    if(ret == IDOK){
                        RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                    }
                }
                break;
            }
        break;
        case WM_PAINT:
        {
            if (gReloadMap) {

                /* Get data for requested map */
                int MapLoadStatus = MapDataTools::GetMapData(gROMFile, gMapMetadata[gMapID], gMapData);
                gReloadMap = false;

                if (MapLoadStatus == FAILURE) {
                   MessageBox(hwnd, "Failed to read map metadata!", "Notice", MB_OK | MB_ICONINFORMATION);
                   break;
                }
                gMapLoaded = true;

                /* Store x-position of the left side of the tileset */
                gTileSelection.SetTilesetX(gMapData.NbScreensX*256 + 20);
            }

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            if (gMapLoaded) {

                /* Prepare for drawing */
                HDC hDCMem = CreateCompatibleDC(hdc); // Create a device context in memory
                unsigned char* lpBitmapBits;
                int BitmapWidth = gMapData.NbScreensX*256 + 276;
                int BitmapHeight = gMapData.NbScreensY*256;

                BITMAPINFO bi;
                ZeroMemory(&bi, sizeof(BITMAPINFO));
                bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                bi.bmiHeader.biWidth = BitmapWidth;
                bi.bmiHeader.biHeight = -BitmapHeight; // Negative so (0,0) is at top left
                bi.bmiHeader.biPlanes = 1;
                bi.bmiHeader.biBitCount = 32;

                HBITMAP bitmap = ::CreateDIBSection(hDCMem, &bi, DIB_RGB_COLORS, (VOID**)&lpBitmapBits, NULL, 0);
                HGDIOBJ oldbmp = ::SelectObject(hDCMem, bitmap);

                BitBlt(hDCMem, 0, 0, BitmapWidth, BitmapHeight, hdc, 0, 0, SRCCOPY);


                /*** Draw Map ***/
                DrawingTools::DrawMap(gMapData, lpBitmapBits, BitmapWidth);

                /*** Draw tileset ***/
                DrawingTools::DrawTileset(gMapData, lpBitmapBits, BitmapWidth);

                /*** Draw selection ***/
                DrawingTools::DrawSelection(gTileSelection, lpBitmapBits, BitmapWidth);

                /* Now use BitBlt to print the memory buffer contents on the window */
                BitBlt(hdc, 0, 0, BitmapWidth, BitmapHeight, hDCMem, 0, 0, SRCCOPY);

                /* Cleanup */
                SelectObject(hDCMem,oldbmp);
                DeleteDC(hDCMem);
                DeleteObject(bitmap);
            }

            EndPaint(hwnd, &ps);
            break;
        }

        case WM_LBUTTONDOWN:
        {
            if (gMapLoaded) {
                POINT P;
                int ScreenX, ScreenY, X, Y;
                if (GetCursorPos(&P) && ScreenToClient(hwnd, &P)){

                    /* Is mouse is in Map area? */
                    if (P.x < gMapData.NbScreensX*256 && P.y < gMapData.NbScreensY*256) {
                        ScreenX = P.x / 256;
                        ScreenY = P.y / 256;
                        X = (P.x % 256) / 16;
                        Y = (P.y % 256) / 16;
//                        cout << "Clicked screen (" << ScreenX << ", " << ScreenY << ") tile ("
//                                                   << X << ", " << Y << ")." << endl
//                                                   << "   --> Tile ID = " << gMapData.GetTile16(ScreenX, ScreenY, X, Y) << "." << endl << endl;

                        /* Remove previous selection */
                        gTileSelection.RedrawSelection(hwnd);

                        gLeftClickDown = true;
                        gTileSelection.SetTile_i(ScreenX, ScreenY, X, Y);
                        gTileSelection.SetTile_j(ScreenX, ScreenY, X, Y);
                        gTileSelection.SelectionInTileset = false;
                        gTileSelection.SelectionExists = true;
                    }
                    /* Is mouse in Tileset area? */
                    else if (P.x > gMapData.NbScreensX*256 + 20 && P.x < gMapData.NbScreensX*256 + 276
                             && P.y < 256) {
                        cout << "Clicked tile ID " << (P.y/16)*16 + (P.x - (gMapData.NbScreensX*256 + 20))/16 << "." << endl << endl;


                        /* Remove previous selection */
                        gTileSelection.RedrawSelection(hwnd);

                        gTileSelection.SetTileID((P.y/16)*16 + (P.x - (gMapData.NbScreensX*256 + 20))/16);
                        gTileSelection.SelectionInTileset = true;
                        gTileSelection.SelectionExists = true;

                        /* Draw new selection */
                        gTileSelection.RedrawSelection(hwnd);
                    }
                }
            }
            break;
        }
        case WM_LBUTTONUP:
        {
            if (gLeftClickDown) {
                POINT P;
                int ScreenX, ScreenY, X, Y;
                if (GetCursorPos(&P) && ScreenToClient(hwnd, &P)){

                    /* Is mouse is in Map area? */
                    if (P.x < gMapData.NbScreensX*256 && P.y < gMapData.NbScreensY*256) {
                        ScreenX = P.x / 256;
                        ScreenY = P.y / 256;
                        X = (P.x % 256) / 16;
                        Y = (P.y % 256) / 16;
//                        cout << "Released click on screen (" << ScreenX << ", " << ScreenY << ") tile ("
//                                                             << X << ", " << Y << ")." << endl
//                                                             << "   --> Tile ID = " << gMapData.GetTile16(ScreenX, ScreenY, X, Y) << "." << endl << endl;
                        gTileSelection.SetTile_j(ScreenX, ScreenY, X, Y);
                    }

                    /* Remove previous selection */
                    gTileSelection.RedrawSelection(hwnd);
                }
            }
            gLeftClickDown = false;
            break;
        }

        case WM_RBUTTONUP:
        {
            if (gMapLoaded && gTileSelection.SelectionExists) {
                POINT P;
                if (GetCursorPos(&P) && ScreenToClient(hwnd, &P)){

                    /* Is mouse is in Map area? */
                    if (P.x < gMapData.NbScreensX*256 && P.y < gMapData.NbScreensY*256) {

                        /* Paste selection into the map and update it */
                        int AbsTileX = P.x / 16;
                        int AbsTileY = P.y / 16;
                        gTileSelection.PasteSelectionTiles(gMapData, AbsTileX, AbsTileY);
                        gTileSelection.RedrawNewTiles(hwnd, AbsTileX, AbsTileY);
                    }
                }
            }
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

            if (!hWndComboBox) {
                MessageBox(hwnd, "Combobox creation failed!", "Notice", MB_OK);
            }

            /* Fill combobox items */
            FillMapCombobox(hwnd, IDC_COMBOBOX_TEXT);

            /* Create and set font */
            HFONT hFont = CreateFont(16,0,0,0,FW_DONTCARE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, VARIABLE_PITCH,TEXT("Calibri"));
            SendMessage(hWndComboBox, WM_SETFONT, (WPARAM)hFont, (LPARAM)MAKELONG(TRUE, 0));

            /* Send user selection */
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
                        gReloadMap = true;
                        gTileSelection.SelectionExists = false;
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
