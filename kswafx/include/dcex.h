#ifndef _C_DCEX_H_
#define _C_DCEX_H_

#include <windows.h>

class C_DCEx
{
public:
	C_DCEx(HDC hDC=NULL);

	int AttachDC(HDC hDC);

	int FillGradientRect(CONST RECT *lprcClient,
                        COLORREF cTopRGB,
                        COLORREF cBottomRGB,
                        short nNoOfBand=256);

	int TransparentBlt(int xStart, int yStart,
						int xWidth, int yHeight,
						HDC hdcSrc,HDC hdcMsk,
						int xPos, int yPos);
	HBITMAP CreateMaskBitmap(HDC hdcSrc,
						int xWidth,int yHeight,
						COLORREF cTransColor);
    HBITMAP CreateRotatedBitmap(HDC hdcSrc,
                        int xWidth,int yHeight,
                        int nAngle);
	BOOL ShadeRect(HDC hDC,LPRECT lpRect,COLORREF crShade);

private:
	HDC m_hDC;
};

#endif