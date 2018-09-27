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

/* Global Map ID */
static int gMapID = 0;

static bool bMapLoaded = false;
static bool bReloadMap = true;

static int  gSelectedTileID = 0;
static bool gLeftClickDown = false;
static bool gSelectionExists = false;
static bool gSelectionIsInTileset = false;

static SelectedTile16 gSelectedTile16_i;
static SelectedTile16 gSelectedTile16_j;



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
            if (bReloadMap) {

                /* Get data for requested map */
                int MapLoadStatus = MapDataTools::GetMapData(gROMFile, gMapMetadata[gMapID], gMapData);
                bReloadMap = false;

                if (MapLoadStatus == FAILURE) {
                   MessageBox(hwnd, "Failed to read map metadata!", "Notice", MB_OK | MB_ICONINFORMATION);
                   break;
                }
                bMapLoaded = true;
            }

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            if (bMapLoaded) {

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
                if (gSelectionExists) {

                    /* Get coordinates of the top left and bottom right vertices of the selection rectangle */
                    int TopLeftX, TopLeftY, BottomRightX, BottomRightY;
                    if (gSelectionIsInTileset) {
                        TopLeftX     = 256*gMapData.NbScreensX + 20 + (gSelectedTileID%16)*16;
                        TopLeftY     =                                (gSelectedTileID/16)*16;
                        BottomRightX = 256*gMapData.NbScreensX + 20 + (gSelectedTileID%16)*16 + 15;
                        BottomRightY =                                (gSelectedTileID/16)*16 + 15;
                    }
                    else {
                        TopLeftX     = (gSelectedTile16_i.AbsX() < gSelectedTile16_j.AbsX() ? gSelectedTile16_i.AbsX() : gSelectedTile16_j.AbsX())*16;
                        TopLeftY     = (gSelectedTile16_i.AbsY() < gSelectedTile16_j.AbsY() ? gSelectedTile16_i.AbsY() : gSelectedTile16_j.AbsY())*16;
                        BottomRightX = (gSelectedTile16_i.AbsX() > gSelectedTile16_j.AbsX() ? gSelectedTile16_i.AbsX() : gSelectedTile16_j.AbsX())*16 + 15;
                        BottomRightY = (gSelectedTile16_i.AbsY() > gSelectedTile16_j.AbsY() ? gSelectedTile16_i.AbsY() : gSelectedTile16_j.AbsY())*16 + 15;
                    }

                    DrawingTools::DrawSelection(TopLeftX, TopLeftY, BottomRightX, BottomRightY, lpBitmapBits, BitmapWidth);
                }

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
            if (bMapLoaded) {
                POINT P;
                int ScreenX, ScreenY, X, Y;
                if (GetCursorPos(&P) && ScreenToClient(hwnd, &P)){

                    /* Is mouse is in Map area? */
                    if (P.x < gMapData.NbScreensX*256 && P.y < gMapData.NbScreensY*256) {
                        ScreenX = P.x / 256;
                        ScreenY = P.y / 256;
                        X = (P.x % 256) / 16;
                        Y = (P.y % 256) / 16;
                        cout << "Clicked screen (" << ScreenX << ", " << ScreenY << ") tile ("
                                                   << X << ", " << Y << ")." << endl
                                                   << "   --> Tile ID = " << gMapData.GetTile16(ScreenX, ScreenY, X, Y) << "." << endl << endl;



                        /* Remove previous selection */
                        if (gSelectionExists) {
                            if (gSelectionIsInTileset) {
                                RECT RectTile;
                                RectTile.left = 256*gMapData.NbScreensX + 20 + (gSelectedTileID%16)*16;
                                RectTile.top = (gSelectedTileID/16)*16;
                                RectTile.right = 256*gMapData.NbScreensX + 20 + (gSelectedTileID%16)*16 + 16;
                                RectTile.bottom = (gSelectedTileID/16)*16 + 16;
                                RedrawWindow(hwnd, &RectTile, NULL, RDW_INVALIDATE /*| RDW_ERASE*/);
                            }
                            else {
                                int TopLeftTileX = (gSelectedTile16_i.AbsX() < gSelectedTile16_j.AbsX() ? gSelectedTile16_i.AbsX() : gSelectedTile16_j.AbsX());
                                int TopLeftTileY = (gSelectedTile16_i.AbsY() < gSelectedTile16_j.AbsY() ? gSelectedTile16_i.AbsY() : gSelectedTile16_j.AbsY());
                                int BottomRightTileX = (gSelectedTile16_i.AbsX() > gSelectedTile16_j.AbsX() ? gSelectedTile16_i.AbsX() : gSelectedTile16_j.AbsX());
                                int BottomRightTileY = (gSelectedTile16_i.AbsY() > gSelectedTile16_j.AbsY() ? gSelectedTile16_i.AbsY() : gSelectedTile16_j.AbsY());

                                int SelectionWidth  = BottomRightTileX - TopLeftTileX + 1;
                                int SelectionHeight = BottomRightTileY - TopLeftTileY + 1;
                                RECT RectTile;
                                RectTile.left = TopLeftTileX*16;
                                RectTile.top = TopLeftTileY*16;
                                RectTile.right = (TopLeftTileX + SelectionWidth)*16;
                                RectTile.bottom = (TopLeftTileY + SelectionHeight)*16;
                                RedrawWindow(hwnd, &RectTile, NULL, RDW_INVALIDATE /*| RDW_ERASE*/);
                            }
                        }



                        gLeftClickDown = true;
                        gSelectedTile16_i.FillTileInfo(ScreenX, ScreenY, X, Y);
                        gSelectedTile16_j.FillTileInfo(ScreenX, ScreenY, X, Y);
                        gSelectionIsInTileset = false;
                        gSelectionExists = true;
                    }
                    /* Is mouse in Tileset area? */
                    else if (P.x > gMapData.NbScreensX*256 + 20 && P.x < gMapData.NbScreensX*256 + 276
                             && P.y < 256) {
                        cout << "Clicked tile ID " << (P.y/16)*16 + (P.x - (gMapData.NbScreensX*256 + 20))/16 << "." << endl << endl;


                        /* Remove previous selection */
                        if (gSelectionExists) {
                            if (gSelectionIsInTileset) {
                                RECT RectTile;
                                RectTile.left = 256*gMapData.NbScreensX + 20 + (gSelectedTileID%16)*16;
                                RectTile.top = (gSelectedTileID/16)*16;
                                RectTile.right = 256*gMapData.NbScreensX + 20 + (gSelectedTileID%16)*16 + 16;
                                RectTile.bottom = (gSelectedTileID/16)*16 + 16;
                                RedrawWindow(hwnd, &RectTile, NULL, RDW_INVALIDATE /*| RDW_ERASE*/);
                            }
                            else {
                                int TopLeftTileX = (gSelectedTile16_i.AbsX() < gSelectedTile16_j.AbsX() ? gSelectedTile16_i.AbsX() : gSelectedTile16_j.AbsX());
                                int TopLeftTileY = (gSelectedTile16_i.AbsY() < gSelectedTile16_j.AbsY() ? gSelectedTile16_i.AbsY() : gSelectedTile16_j.AbsY());
                                int BottomRightTileX = (gSelectedTile16_i.AbsX() > gSelectedTile16_j.AbsX() ? gSelectedTile16_i.AbsX() : gSelectedTile16_j.AbsX());
                                int BottomRightTileY = (gSelectedTile16_i.AbsY() > gSelectedTile16_j.AbsY() ? gSelectedTile16_i.AbsY() : gSelectedTile16_j.AbsY());

                                int SelectionWidth  = BottomRightTileX - TopLeftTileX + 1;
                                int SelectionHeight = BottomRightTileY - TopLeftTileY + 1;
                                RECT RectTile;
                                RectTile.left = TopLeftTileX*16;
                                RectTile.top = TopLeftTileY*16;
                                RectTile.right = (TopLeftTileX + SelectionWidth)*16;
                                RectTile.bottom = (TopLeftTileY + SelectionHeight)*16;
                                RedrawWindow(hwnd, &RectTile, NULL, RDW_INVALIDATE /*| RDW_ERASE*/);
                            }
                        }


                        gSelectedTileID = (P.y/16)*16 + (P.x - (gMapData.NbScreensX*256 + 20))/16;
                        gSelectionIsInTileset = true;
                        gSelectionExists = true;

                        RECT RectTile;
                        RectTile.left = 256*gMapData.NbScreensX + 20 + (gSelectedTileID%16)*16;
                        RectTile.top = (gSelectedTileID/16)*16;
                        RectTile.right = 256*gMapData.NbScreensX + 20 + (gSelectedTileID%16)*16 + 16;
                        RectTile.bottom = (gSelectedTileID/16)*16 + 16;
                        RedrawWindow(hwnd, &RectTile, NULL, RDW_INVALIDATE /*| RDW_ERASE*/);
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
                        cout << "Released click on screen (" << ScreenX << ", " << ScreenY << ") tile ("
                                                             << X << ", " << Y << ")." << endl
                                                             << "   --> Tile ID = " << gMapData.GetTile16(ScreenX, ScreenY, X, Y) << "." << endl << endl;
                        gSelectedTile16_j.FillTileInfo(ScreenX, ScreenY, X, Y);
                    }

                    int TopLeftTileX = (gSelectedTile16_i.AbsX() < gSelectedTile16_j.AbsX() ? gSelectedTile16_i.AbsX() : gSelectedTile16_j.AbsX());
                    int TopLeftTileY = (gSelectedTile16_i.AbsY() < gSelectedTile16_j.AbsY() ? gSelectedTile16_i.AbsY() : gSelectedTile16_j.AbsY());
                    int BottomRightTileX = (gSelectedTile16_i.AbsX() > gSelectedTile16_j.AbsX() ? gSelectedTile16_i.AbsX() : gSelectedTile16_j.AbsX());
                    int BottomRightTileY = (gSelectedTile16_i.AbsY() > gSelectedTile16_j.AbsY() ? gSelectedTile16_i.AbsY() : gSelectedTile16_j.AbsY());

                    int SelectionWidth  = BottomRightTileX - TopLeftTileX + 1;
                    int SelectionHeight = BottomRightTileY - TopLeftTileY + 1;
                    RECT RectTile;
                    RectTile.left = TopLeftTileX*16;
                    RectTile.top = TopLeftTileY*16;
                    RectTile.right = (TopLeftTileX + SelectionWidth)*16;
                    RectTile.bottom = (TopLeftTileY + SelectionHeight)*16;
                    RedrawWindow(hwnd, &RectTile, NULL, RDW_INVALIDATE /*| RDW_ERASE*/);
                }
            }
            gLeftClickDown = false;
            break;
        }
        case WM_RBUTTONUP:
        {
            if (bMapLoaded && gSelectionExists) {
                POINT P;
                int ScreenX, ScreenY, TileX, TileY, X, Y;
                if (GetCursorPos(&P) && ScreenToClient(hwnd, &P)){

                    //cout << P.x << " - " << P.y << endl;
                    /* Is mouse is in Map area? */
                    if (P.x < gMapData.NbScreensX*256 && P.y < gMapData.NbScreensY*256) {

                        if (gSelectionIsInTileset) {
                            ScreenX = P.x / 256;
                            ScreenY = P.y / 256;
                            X = (P.x % 256) / 16;
                            Y = (P.y % 256) / 16;
                            cout << "Changed screen (" << ScreenX << ", " << ScreenY << ") tile ("
                                                       << X << ", " << Y << ")." << endl
                                                       << "   --> Tile ID " << gMapData.GetTile16(ScreenX, ScreenY, X, Y)
                                                       << " changed to " << gSelectedTileID << "." << endl << endl;

                            gMapData.SetTile16(ScreenX, ScreenY, X, Y, gSelectedTileID);


                            RECT RectTile;
                            RectTile.left = (P.x/16)*16;
                            RectTile.top = (P.y/16)*16;
                            RectTile.right = (P.x/16)*16 + 16;
                            RectTile.bottom = (P.y/16)*16 + 16;
                            RedrawWindow(hwnd, &RectTile, NULL, RDW_INVALIDATE /*| RDW_ERASE*/);
                        }
                        else {
                            int TopLeftTileX = (gSelectedTile16_i.AbsX() < gSelectedTile16_j.AbsX() ? gSelectedTile16_i.AbsX() : gSelectedTile16_j.AbsX());
                            int TopLeftTileY = (gSelectedTile16_i.AbsY() < gSelectedTile16_j.AbsY() ? gSelectedTile16_i.AbsY() : gSelectedTile16_j.AbsY());
                            int BottomRightTileX = (gSelectedTile16_i.AbsX() > gSelectedTile16_j.AbsX() ? gSelectedTile16_i.AbsX() : gSelectedTile16_j.AbsX());
                            int BottomRightTileY = (gSelectedTile16_i.AbsY() > gSelectedTile16_j.AbsY() ? gSelectedTile16_i.AbsY() : gSelectedTile16_j.AbsY());

                            int SelectionWidth  = BottomRightTileX - TopLeftTileX + 1;
                            int SelectionHeight = BottomRightTileY - TopLeftTileY + 1;

                            int TileSelection[SelectionWidth][SelectionHeight];

                            for (X = 0; X < SelectionWidth; X++) {
                                for (Y = 0; Y < SelectionHeight; Y++) {
                                    TileSelection[X][Y] =
                                        gMapData.GetTile16((TopLeftTileX+X)/16, (TopLeftTileY+Y)/16, (TopLeftTileX+X)%16, (TopLeftTileY+Y)%16);
                                }
                            }


    //                        ScreenX = P.x / 256;
    //                        ScreenY = P.y / 256;
                            TileX = P.x / 16;
                            TileY = P.y / 16;
    //                        cout << "Changed screen (" << ScreenX << ", " << ScreenY << ") tile ("
    //                                                   << TileX << ", " << TileY << ")." << endl
    //                                                   << "   --> Tile ID " << gMapData.GetTile16(ScreenX, ScreenY, TileX, TileY)
    //                                                   << " changed to " << gSelectedTile << "." << endl << endl;

                            for (X = 0; X < SelectionWidth; X++) {
                                for (Y = 0; Y < SelectionHeight; Y++) {
                                    gMapData.SetTile16((TileX+X)/16, (TileY+Y)/16, (TileX+X)%16, (TileY+Y)%16,
                                                       TileSelection[X][Y]);
                                }
                            }

                            RECT RectTile;
                            RectTile.left = TileX*16;
                            RectTile.top = TileY*16;
                            RectTile.right = (TileX + SelectionWidth)*16;
                            RectTile.bottom = (TileY + SelectionHeight)*16;
                            RedrawWindow(hwnd, &RectTile, NULL, RDW_INVALIDATE /*| RDW_ERASE*/);
                        }
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
                        bReloadMap = true;
                        gSelectionExists = false;
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
