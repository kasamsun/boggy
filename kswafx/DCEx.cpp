#include <math.h>

#include <DCEx.h>

C_DCEx::C_DCEx(HDC hDC)
{
	m_hDC = hDC;
}

int C_DCEx::AttachDC(HDC hDC)
{
	m_hDC = hDC;
	return TRUE;
}

int C_DCEx::FillGradientRect(CONST RECT *lprcClient,COLORREF cTopRGB,COLORREF cBottomRGB,short nNoOfBand)
{
	if ( m_hDC == NULL ) return FALSE;

	RECT rectFill;		// Rectangle for filling band
	float fStep;		// How large is each band?
	HBRUSH hBrush;
	int iOnBand;		// Loop index

	// 256 bands to fill for default
	fStep = (float)(lprcClient->bottom-lprcClient->top) / (float)nNoOfBand;

	BYTE nStartR = GetRValue(cTopRGB);
	BYTE nStartG = GetGValue(cTopRGB);
	BYTE nStartB = GetBValue(cTopRGB);

	BYTE nEndR = GetRValue(cBottomRGB);
	BYTE nEndG = GetGValue(cBottomRGB);
	BYTE nEndB = GetBValue(cBottomRGB);

	float fRStep = (nEndR-nStartR)/(float)nNoOfBand;
	float fGStep = (nEndG-nStartG)/(float)nNoOfBand;
	float fBStep = (nEndB-nStartB)/(float)nNoOfBand;

	for (iOnBand = 0; iOnBand < (int)nNoOfBand; iOnBand++)
	{
		// Set the location of the current band
		SetRect(&rectFill,
				lprcClient->left,            // Upper left X
				lprcClient->top + (int)(iOnBand * fStep),      // Upper left Y
				lprcClient->right,         // Lower right X
				lprcClient->top + (int)((iOnBand+1) * fStep)); // Lower right Y

		// Create a brush with the appropriate color for this band
		hBrush = CreateSolidBrush(RGB(nStartR+(fRStep*iOnBand),
									nStartG+(fGStep*iOnBand),
									nStartB+(fBStep*iOnBand) ));
		FillRect(m_hDC, &rectFill, hBrush);
		DeleteObject(hBrush);
	}
	return TRUE;
}

int C_DCEx::TransparentBlt (int xStart, int yStart,
						 int xWidth, int yHeight,
						 HDC hdcSrc,HDC hdcMsk,
						 int xPos, int yPos)
{
	if ( m_hDC == NULL ) return FALSE;

	HBITMAP    bmAndObject, bmAndMem;
	HBITMAP    bmObjectOld, bmMemOld;
	HDC        hdcMem,hdcObject, hdcTemp;
	POINT      ptSize;
	HBITMAP		hbmpTemp,bmTemp;

	hdcTemp = CreateCompatibleDC(m_hDC);

	hbmpTemp = CreateCompatibleBitmap(m_hDC,xWidth,yHeight);

	bmTemp = (HBITMAP)SelectObject(hdcTemp,hbmpTemp);

	BitBlt(hdcTemp,0,0,xWidth,yHeight,hdcSrc,xPos,yPos,SRCCOPY);

	ptSize.x = xWidth;
	ptSize.y = yHeight;
	DPtoLP(hdcTemp, &ptSize, 1);	// Convert from device
									// to logical points
	// Create some DCs to hold temporary data.
	hdcObject = CreateCompatibleDC(m_hDC);
	hdcMem    = CreateCompatibleDC(m_hDC);

	bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndMem    = CreateCompatibleBitmap(m_hDC, ptSize.x, ptSize.y);

	// Each DC must select a bitmap object to store pixel data.
	bmObjectOld = (HBITMAP) SelectObject(hdcObject, bmAndObject);
	bmMemOld    = (HBITMAP) SelectObject(hdcMem, bmAndMem);
	
	// Set proper mapping mode.
	SetMapMode(hdcTemp, GetMapMode(m_hDC));
	
	BitBlt(hdcObject,0,0,ptSize.x,ptSize.y,
			hdcMsk,xPos,yPos,SRCCOPY);
	
	// Copy the background of the main DC to the destination.
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, m_hDC, xStart, yStart,
	      SRCCOPY);
	
	// Mask out the places where the bitmap will be placed.
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);
	// XOR the bitmap with the background on the destination DC.
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);
	// Copy the destination to the screen.
	BitBlt(m_hDC, xStart, yStart, ptSize.x, ptSize.y, hdcMem, 0, 0,
	      SRCCOPY);

	DeleteObject(SelectObject(hdcObject,bmObjectOld));
	DeleteObject(SelectObject(hdcMem,bmMemOld));
	DeleteObject(SelectObject(hdcTemp,bmTemp));

	DeleteDC(hdcMem);
	DeleteDC(hdcObject);
	DeleteDC(hdcTemp);
	return TRUE;
}

HBITMAP C_DCEx::CreateMaskBitmap(HDC hdcSrc,int xWidth,int yHeight,COLORREF cTransColor)
{
	HDC hdcMsk;
	HBITMAP hbmpImgMsk,hbmpImgMskOld;

	hdcMsk = CreateCompatibleDC(hdcSrc);
	hbmpImgMsk = CreateBitmap(xWidth,yHeight, 1, 1, NULL);
	hbmpImgMskOld = (HBITMAP) SelectObject(hdcMsk,hbmpImgMsk);

	COLORREF cColor = SetBkColor(hdcSrc,cTransColor);

	// create mask picture
	BitBlt(hdcMsk,0,0,xWidth,yHeight,hdcSrc,0,0,SRCCOPY);
	SetBkColor(hdcSrc, cColor);

	// update source DC with mask picture
	HDC hdctmp2 = CreateCompatibleDC(hdcSrc);
	HBITMAP hbmptmp2 = CreateBitmap(xWidth,yHeight,1,1,NULL);
	HBITMAP hbmptmpold2 = (HBITMAP) SelectObject(hdctmp2,hbmptmp2);

	BitBlt(hdctmp2,0,0,xWidth,yHeight,hdcMsk,0,0,NOTSRCCOPY);
	// create mask around org picture with ~of mask picture
	BitBlt(hdcSrc,0,0,xWidth,yHeight,hdctmp2,0,0,SRCAND);

	DeleteObject(SelectObject(hdctmp2,hbmptmpold2));
	DeleteDC(hdctmp2);

	HBITMAP hbmpMasked;
	hbmpMasked = (HBITMAP)SelectObject(hdcMsk,hbmpImgMskOld);
	DeleteDC(hdcMsk);

	return hbmpMasked;
}

HBITMAP C_DCEx::CreateRotatedBitmap(HDC hdcSrc,int xWidth,int yHeight,int nAngle)
{
	double rad;
	POINT pos[5];
	double pi = 3.14159265359;
	double sinsetha,cossetha;
	int x,y,startx,endx;
	int i;
	int dxlt,dxlb,dxrt,dxrb,dylt,dylb,dyrt,dyrb;
	double m12,m24,m13,m34;
	int rx,ry;
	int p1,p2,p3,p4;

	rad = (double)pi/180*nAngle;
	sinsetha = sin(rad);
	cossetha = cos(rad);
	x = xWidth+1;
	y = yHeight+1;

	pos[1].x = 0;				    // -0*cossetha - 0*sinsetha;
	pos[1].y = 0;				    //  0*sinsetha - 0*cossetha;
	pos[2].x = (long)(x*cossetha);		    //  x*cossetha - 0*sinsetha;
	pos[2].y = (long)(-x*sinsetha);		    // -x*sinsetha - 0*cossetha;
	pos[3].x = (long)(y*sinsetha);		    // -0*cossetha + y*sinsetha;
	pos[3].y = (long)(y*cossetha);		    //  0*sinsetha + y*cossetha;
	pos[4].x = pos[2].x + pos[3].x;	//  x*cossetha + y*sinsetha;
	pos[4].y = pos[2].y + pos[3].y;	// -x*sinsetha + y*cossetha;

	p1 = 2;
	p2 = 1;
	p3 = 4;
	p4 = 3;
	dxlt = pos[p1].x-pos[p2].x;
	dxlb = pos[p4].x-pos[p2].x;
	dxrt = pos[p3].x-pos[p1].x;
	dxrb = pos[p3].x-pos[p4].x;
	dylt = pos[p2].y-pos[p1].y;
	dylb = pos[p4].y-pos[p2].y;
	dyrt = pos[p3].y-pos[p1].y;
	dyrb = pos[p4].y-pos[p3].y;
	m12 = (double)dxlt/dylt;
	m24 = (double)dxlb/dylb;
	m13 = (double)dxrt/dyrt;
	m34 = (double)dxrb/dyrb;
	rx = 0;
	ry = -dylt;

    // create DC for output
	HDC hdcDest = CreateCompatibleDC(hdcSrc);
	HBITMAP hbmpDest = CreateCompatibleBitmap(hdcSrc,dxlt+dxrt,dylt+dylb);
	HBITMAP hbmpTmp = (HBITMAP)SelectObject(hdcDest,hbmpDest);

    RECT rc = {0,0,dxlt+dxrt,dylt+dylb};
    HBRUSH hbrush = CreateSolidBrush(RGB(0,128,128));
    FillRect(hdcDest,&rc,hbrush);
    DeleteObject(hbrush);
//	BitBlt(hdcDest,0,0,dxlt+dxrt,dylt+dylb,NULL,0,0,BLACKNESS);

	for ( y = 0 ; y <= dylt+dylb ; y++ )
	{
		if ( y <= dylt )
		{
			startx = dxlt - (int)(m12*y);
		}
		else
		{
			startx = (int)(m24*(y-dylt));
		}
		if ( y <= dyrt )
		{
			endx = dxlt + (int)(m13*y);
		}
		else
		{
			endx = dxlb + dxrb - (int)(m34*(y-dyrt));
		}
		// map pixel of dest. image
		for ( i = startx; i<=endx ; i++)
		{
			SetPixel(hdcDest,i,y,GetPixel(hdcSrc,
					(int)((i+rx)*cossetha-(y+ry)*sinsetha),
					(int)((i+rx)*sinsetha+(y+ry)*cossetha)));
		}
	}

	HBITMAP hbmpRotated;
	hbmpRotated = (HBITMAP)SelectObject(hdcDest,hbmpTmp);
	DeleteDC(hdcDest);

    return hbmpRotated;
}

// ShadeRect
// hDC    : the DC on which the area is to be shaded
// lpRect : the coordinates within which to shade
BOOL C_DCEx::ShadeRect(HDC hDC,LPRECT lpRect,COLORREF crShade)
{
	if ( m_hDC == NULL ) return FALSE;

	COLORREF  crOldBkColor, crOldTextColor;
	HBRUSH    hBrush, hOldBrush;
	HBITMAP   hBitmap, hBrushBitmap, hOldMemBitmap;
	int       OldBkMode, nWidth, nHeight;
	HDC       hMemDC;
	RECT      rcRect = { 0, 0, 0, 0};
	// The bitmap bits are for a monochrome "every-other-pixel"
	//     bitmap (for a pattern brush)
	WORD      Bits[8] = { 0x0055, 0x00aa, 0x0055, 0x00aa,
						 0x0055, 0x00aa, 0x0055, 0x00aa };

	// The Width and Height of the target area
	nWidth = lpRect->right - lpRect->left + 1;
	nHeight = lpRect->bottom - lpRect->top + 1;

	// Need a pattern bitmap
	hBrushBitmap = CreateBitmap( 8, 8, 1, 1, &Bits );
	// Need to store the original image
	hBitmap = CreateCompatibleBitmap( hDC, nWidth, nHeight );
	// Need a memory DC to work in
	hMemDC = CreateCompatibleDC( hDC );
	// Create the pattern brush
	hBrush = CreatePatternBrush( hBrushBitmap );

	// Has anything failed so far? If so, abort!
	if( (hBrushBitmap==NULL) || (hBitmap==NULL) ||
	   (hMemDC==NULL) || (hBrush==NULL) )
	{
	  if( hBrushBitmap != NULL ) DeleteObject(hBrushBitmap);
	  if( hBitmap != NULL ) DeleteObject( hBitmap );
	  if( hMemDC != NULL ) DeleteDC( hMemDC );
	  if( hBrush != NULL ) DeleteObject( hBrush );
	  return FALSE;
	}

	// Select the bitmap into the memory DC
	hOldMemBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap );

	// How wide/tall is the original?
	rcRect.right = nWidth;
	rcRect.bottom = nHeight;

	// Lay down the pattern in the memory DC
	FillRect( hMemDC, &rcRect, hBrush );

	// Fill in the non-color pixels with the original image
	BitBlt( hMemDC, 0, 0, nWidth, nHeight, hDC,
	lpRect->left, lpRect->top, SRCAND );

	// Set the color scheme
	crOldTextColor = SetTextColor( hDC, crShade );
	crOldBkColor = SetBkColor( hDC, RGB(0,0,0) );
	OldBkMode = SetBkMode( hDC, OPAQUE );

	// Select the pattern brush
	hOldBrush = (HBRUSH)SelectObject( hDC, hBrush );
	// Fill in the color pixels, and set the others to black
	FillRect( hDC, lpRect, hBrush );
	// Fill in the black ones with the original image
	BitBlt( hDC, lpRect->left, lpRect->top, nWidth, nHeight,
		   hMemDC, 0, 0, SRCPAINT );

	// Restore target DC settings
	SetBkMode( hDC, OldBkMode );
	SetBkColor( hDC, crOldBkColor );
	SetTextColor( hDC, crOldTextColor );

	// Clean up
	SelectObject( hMemDC, hOldMemBitmap );
	DeleteObject( hBitmap );
	DeleteDC( hMemDC );
	DeleteObject( hBrushBitmap );
	SelectObject( hDC, hOldBrush );
	DeleteObject( hBrush );

	return TRUE;
}


