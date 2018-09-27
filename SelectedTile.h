#ifndef __SELECTEDTILE_H__
#define __SELECTEDTILE_H__


class SelectedTile16 {
public:
    SelectedTile16();
    ~SelectedTile16();
    void FillTileInfo(int aScreenX, int aScreenY, int aX, int aY);
    int  AbsX(void);
    int  AbsY(void);

    int ScreenX;
    int ScreenY;
    int X;
    int Y;
};

#endif // __SELECTEDTILE_H__
