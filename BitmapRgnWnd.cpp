#include "BitmapRgnWnd.h"

#include "BoggyApp.h"
#include "resource.h"

BEGIN_HANDLE_MSG(C_BitmapRgnWnd,C_GenericWnd)
	HANDLE_MSG(WM_PAINT,OnPaint)
	HANDLE_MSG(WM_LBUTTONDOWN,OnLButtonDown)
	HANDLE_MSG(WM_MOUSEMOVE,OnMouseMove)
	HANDLE_MSG(WM_LBUTTONUP,OnLButtonUp)
	HANDLE_MSG(WM_RBUTTONUP,OnRButtonUp)
	HANDLE_MSG(WM_DESTROY,OnDestroy)
END_HANDLE_MSG()

HRGN BitmapToRegion (HBITMAP hBmp, COLORREF cTransparentColor=0x008080, COLORREF cTolerance=0x101010);

C_BitmapRgnWnd::C_BitmapRgnWnd( HWND hwndParent,int nResID )
{
	m_hbmpDialog = LoadBitmap(C_App::m_hInstance,MAKEINTRESOURCE(nResID));
	BITMAP bmpDialog;
	GetObject(m_hbmpDialog,sizeof(BITMAP),&bmpDialog);
	m_hWnd = CreateWindow(
		m_szClassName,
        m_szClassName,
        WS_POPUP|WS_VISIBLE,
        GetSystemMetrics(SM_CXFULLSCREEN)/2-bmpDialog.bmWidth/2,
        GetSystemMetrics(SM_CYFULLSCREEN)/2-bmpDialog.bmHeight/2,
        bmpDialog.bmWidth,
        bmpDialog.bmHeight,
        hwndParent,
        NULL,
        C_App::m_hInstance,
        (LPSTR) this );	//Pass obj pointer,To store when WM_CREATE

    if ( ! m_hWnd )
        exit( FALSE );

	m_hDC = GetDC(m_hWnd);
	m_hdcDialog = CreateCompatibleDC(m_hDC);
	m_hbmpDialogOld = (HBITMAP) SelectObject(m_hdcDialog,m_hbmpDialog);
	ReleaseDC(m_hWnd,m_hDC);

	HBITMAP hbmpMsk = LoadBitmap(C_App::m_hInstance,MAKEINTRESOURCE(nResID));
	HRGN MyRgn = BitmapToRegion(hbmpMsk);
	SetWindowRgn(m_hWnd,MyRgn,TRUE);

	// =================================
	m_nFlags = 0;
	m_bLPressed = FALSE;
}

C_BitmapRgnWnd::~C_BitmapRgnWnd( void )
{
	DeleteObject(SelectObject(m_hdcDialog,m_hbmpDialogOld));
	DeleteDC(m_hdcDialog);
}

HRGN C_BitmapRgnWnd::CreateBitmapRgn(HBITMAP hbmpMsk,COLORREF cTransparentColor)
{
	BITMAP bmpMsk;
	GetObject(hbmpMsk,sizeof(BITMAP),&bmpMsk);

	//TRACE("bmType = %d\n",bmpMsk.bmType);
	//TRACE("bmWidth = %d\n",bmpMsk.bmWidth);
	//TRACE("bmHeight = %d\n",bmpMsk.bmHeight);
	//TRACE("bmWidthBytes = %d\n",bmpMsk.bmWidthBytes);
	//TRACE("bmPlanes = %d\n",bmpMsk.bmPlanes);
	//TRACE("bmBitsPixel = %d\n",bmpMsk.bmBitsPixel);

	int nStartX=0,nStartY=0;
	int nRunX=0,nRunY=0;
	int nPrvX=0,nPrvY=0;

	while ( GetPixel(m_hdcDialog,nRunX,nRunY) == cTransparentColor )
	{
		nRunX++;
		if ( nRunX == bmpMsk.bmWidth )
		{
			nRunY++;
			if ( nRunY == bmpMsk.bmHeight )
				break;
			nRunX = 0;
		}
	}

	// if starting point not found 
	if ( nRunX == bmpMsk.bmWidth && nRunY == bmpMsk.bmHeight )
		return CreateRectRgn(0,0,bmpMsk.bmWidth,bmpMsk.bmHeight);

	int nDir;
	int nPrvDir;

	nStartX = nRunX;
	nStartY = nRunY;
	nPrvX = nRunX-1;
	nPrvY = nRunY;
	nDir = 4;   // 3 2 1
				// 4 x 0
				// 5 6 7
	nPrvDir = -1;

//	TRACE("nStart X %d,Y %d\n",nStartX,nStartY);
	POINT p[251];	// MAX_POINT_BUF 
	int nPointCount = 0;

	p[0].x = nRunX;
	p[0].y = nRunY;
	int cnt = 0;

	while ( nPointCount<250 /* MAX_POINT_BUF */)
	{
		nRunX = nPrvX;
		nRunY = nPrvY;
		cnt = 0;
		do
		{
			do 
			{
				cnt++;
				if ( cnt > 8 )
					goto ExitFindEdge;
				if ( nDir < 7 ) nDir++; else nDir=0;

				switch(nDir)
				{
				case 0:case 1:nRunY--;break;
				case 2:case 3:nRunX--;break;
				case 4:case 5:nRunY++;break;
				case 6:case 7:nRunX++;break;
				}
			}
			while ( (nRunX <0 || nRunX >= bmpMsk.bmWidth ||
					nRunY <0 || nRunY >= bmpMsk.bmHeight) ); // && cnt < 8
		}
		while ( GetPixel(m_hdcDialog,nRunX,nRunY) == cTransparentColor ); // & cnt < 8
		//if ( cnt >= 8 ) break;
		if ( nRunX == nStartX && nRunY == nStartY )
			break;

		nPrvX = p[nPointCount].x;
		nPrvY = p[nPointCount].y;
		if ( nPrvDir != nDir )
		{
			nPointCount++;
			nPrvDir = nDir;
//			TRACE("\n");
		}
		p[nPointCount].x = nRunX;
		p[nPointCount].y = nRunY;
		nPrvDir = nDir;
//		TRACE("p%d(%d,%d)",nPointCount,p[nPointCount].x,p[nPointCount].y);
		// change direction
		if ( nDir >= 4 ) nDir-=4; else nDir+=4;
	}
ExitFindEdge:
	nPointCount++;
//	TRACE("end of process, use %d point\n",nPointCount);
	return CreatePolygonRgn(p,nPointCount,WINDING);
}

/////////////////////////////////////////////////////////////////
#define WM_KICKIDLE         0x036A  // (params unused) causes idles to kick in
#define WF_MODALLOOP        0x0008  // currently in modal loop
#define WF_CONTINUEMODAL    0x0010  // modal loop should continue running

int C_BitmapRgnWnd::RunModalLoop()
{
	if ( !IsWindow(m_hWnd)) // window must be created
		return -1;
	if ( m_nFlags & WF_MODALLOOP ) // window must not already be in modal state
		return -1;
	// for tracking the idle time state
	BOOL bIdle = TRUE;
	LONG lIdleCount = 0;
	BOOL bShowIdle = TRUE;//(dwFlags & MLF_SHOWONIDLE) && !(GetStyle() & WS_VISIBLE); // must be visible
	HWND hWndParent = GetParent(m_hWnd);
	m_nFlags |= (WF_MODALLOOP|WF_CONTINUEMODAL);

	MSG msg;
	// acquire and dispatch messages until the modal state is done
	for (;;)
	{
		//ASSERT(ContinueModal());
		// phase1: check to see if we can do idle work
		while (bIdle &&
			!PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
		{
			//ASSERT(ContinueModal());

			// show the dialog when the message queue goes idle
			if (bShowIdle)
			{
				ShowWindow(m_hWnd,SW_SHOWNORMAL);
				UpdateWindow(m_hWnd);
				bShowIdle = FALSE;
			}

			// call OnIdle while in bIdle state
			if ( hWndParent != NULL && lIdleCount == 0)
				// send WM_ENTERIDLE to the parent
				SendMessage(hWndParent, WM_ENTERIDLE, MSGF_DIALOGBOX, (LPARAM)m_hWnd);
			if ( !SendMessage(m_hWnd,WM_KICKIDLE, MSGF_DIALOGBOX, lIdleCount++) )
				// stop idle processing next time
				bIdle = FALSE;
		}

		// phase2: pump messages while available
		do
		{
			//ASSERT(ContinueModal());

			// pump message, but quit on WM_QUIT
			if ( !PumpMessage() )
			{
				PostQuitMessage(0);
				return -1;
			}

			// show the window when certain special messages rec'd
			if (bShowIdle &&
				(msg.message == 0x118 || msg.message == WM_SYSKEYDOWN))
			{
				ShowWindow(m_hWnd,SW_SHOWNORMAL);
				UpdateWindow(m_hWnd);
				bShowIdle = FALSE;
			}

			if (!ContinueModal())
				goto ExitModal;

			// reset "no idle" state after pumping "normal" message
			if ( IsIdleMessage(&msg) )
			{
				bIdle = TRUE;
				lIdleCount = 0;
			}

		} while (PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE));
	}
ExitModal:
	m_nFlags &= ~(WF_MODALLOOP|WF_CONTINUEMODAL);
	return m_nModalResult;
}

BOOL C_BitmapRgnWnd::ContinueModal()
{
	return m_nFlags & WF_CONTINUEMODAL;
}

void C_BitmapRgnWnd::EndModalLoop()
{
	//ASSERT(IsWindow(hWnd));
	// make sure a message goes through to exit the modal loop
	if (m_nFlags & WF_CONTINUEMODAL)
	{
		//m_nFlags &= ~WF_CONTINUEMODAL;
		m_nFlags = 0;
		PostMessage(m_hWnd,0,0,0);
	}
}

BOOL C_BitmapRgnWnd::PumpMessage()
{
	MSG msg;
	if (!GetMessage(&msg, NULL, NULL, NULL))
		return FALSE;
	// process this message
	if (msg.message != WM_KICKIDLE ) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return TRUE;
}

BOOL C_BitmapRgnWnd::IsIdleMessage(MSG* pMsg)
{
	// Return FALSE if the message just dispatched should _not_
	// cause OnIdle to be run.  Messages which do not usually
	// affect the state of the user interface and happen very
	// often are checked for.

	// redundant WM_MOUSEMOVE and WM_NCMOUSEMOVE
	if (pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_NCMOUSEMOVE)
	{
		// mouse move at same position as last mouse move?
		if ( m_ptCursorLast.x == pMsg->pt.x && 
				m_ptCursorLast.y == pMsg->pt.y &&
				pMsg->message == m_nMsgLast )
			return FALSE;
		m_ptCursorLast.x = pMsg->pt.x;  // remember for next time
		m_ptCursorLast.y = pMsg->pt.y;
		m_nMsgLast = pMsg->message;
		return TRUE;
	}
	// WM_PAINT and WM_SYSTIMER (caret blink)
	return pMsg->message != WM_PAINT && pMsg->message != 0x0118;
}


/////////////// Event handler ///////////////
LRESULT C_BitmapRgnWnd::OnPaint(WPARAM wParam,LPARAM lParam)
{
    PAINTSTRUCT ps;
    m_hDC = BeginPaint( m_hWnd, &ps );
	BITMAP bmpDialog;
	GetObject(m_hbmpDialog,sizeof(BITMAP),&bmpDialog);
	BitBlt(m_hDC,0,0,bmpDialog.bmWidth,bmpDialog.bmHeight,
		   m_hdcDialog,0,0,SRCCOPY);
    EndPaint( m_hWnd, &ps );
	return 0;
}

LRESULT C_BitmapRgnWnd::OnLButtonDown(WPARAM wParam,LPARAM lParam)
{
	m_bLPressed = TRUE;
	SetCapture(m_hWnd);
	m_nPosX = (int)LOWORD(lParam);
	m_nPosY = (int)HIWORD(lParam);
	return 0;
}

LRESULT C_BitmapRgnWnd::OnMouseMove(WPARAM wParam,LPARAM lParam)
{
	if ( m_bLPressed )
	{
		POINT p = {(int)LOWORD(lParam),(int)HIWORD(lParam)};
		ClientToScreen(m_hWnd,&p);
		// * i don't know why x,y over 65535 , so i fix////
		p.x &= 0x0000ffff;
		p.y &= 0x0000ffff;
		///////////////////////////////////////////////////
		MoveWindow(m_hWnd,p.x-m_nPosX,p.y-m_nPosY,256,256,TRUE);
 	}
	return 0;
}

LRESULT C_BitmapRgnWnd::OnLButtonUp(WPARAM wParam,LPARAM lParam)
{
	ReleaseCapture();
	m_bLPressed = FALSE;
	return 0;
}

LRESULT C_BitmapRgnWnd::OnRButtonUp(WPARAM wParam,LPARAM lParam)
{
	DestroyWindow(m_hWnd);
	return 0;
}

LRESULT C_BitmapRgnWnd::OnDestroy(WPARAM wParam,LPARAM lParam)
{
	// if child call postquitmessage,
	// didn't call here
	//PostQuitMessage(0);
	EndModalLoop();
	return 0;
}




//============================================================
//	BitmapToRegion :	Create a region from the "non-transparent" pixels of a bitma
//	Author :		Jean-Edouard Lachand-Robert (http://www.geocities.com/Paris/LeftBank/1160/resume.htm), June 1998
//	hBmp :			Source bitma
//	cTransparentColor :	Color base for the "transparent" pixels (default is black
//	cTolerance :		Color tolerance for the "transparent" pixels
//	A pixel is assumed to be transparent if the value of each of its 3 components (blue, green and red) is
//	greater or equal to the corresponding value in cTransparentColor and is lower or equal to the
//	corresponding value in cTransparentColor + cTolerance
HRGN BitmapToRegion (HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance)
{
	HRGN hRgn = NULL;

	//ASSERT(hBmp);
	if (hBmp)
	{
		// Create a memory DC inside which we will scan the bitmap conten
		HDC hMemDC = CreateCompatibleDC(NULL);
		//ASSERT(hMemDC);
		if (hMemDC)
		{
			// Get bitmap siz
			BITMAP bm;
			GetObject(hBmp, sizeof(bm), &bm);

			// Create a 32 bits depth bitmap and select it into the memory DC
			BITMAPINFOHEADER RGB32BITSBITMAPINFO = {	
					sizeof(BITMAPINFOHEADER),	// biSize
					bm.bmWidth,					// biWidth;
					bm.bmHeight,				// biHeight;
					1,							// biPlanes;
					32,							// biBitCount
					BI_RGB,						// biCompression;
					0,							// biSizeImage;
					0,							// biXPelsPerMeter;
					0,							// biYPelsPerMeter;
					0,							// biClrUsed;
					0							// biClrImportant;
			};
			VOID * pbits32; 
			HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
			//ASSERT(hbm32);
			if (hbm32)
			{
				HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);

				// Create a DC just to copy the bitmap into the memory D
				HDC hDC = CreateCompatibleDC(hMemDC);
				//ASSERT(hDC);
				if (hDC)
				{
					// Get how many bytes per row we have for the bitmap bits (rounded up to 32 bits
					BITMAP bm32;
					GetObject(hbm32, sizeof(bm32), &bm32);
					while (bm32.bmWidthBytes % 4)
						bm32.bmWidthBytes++;

					// Copy the bitmap into the memory D
					HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
					BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);

					// For better performances, we will use the ExtCreateRegion() function to create th
					// region. This function take a RGNDATA structure on entry. We will add rectangles b
					// amount of ALLOC_UNIT number in this structure
					#define ALLOC_UNIT	100
					DWORD maxRects = ALLOC_UNIT;
					HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
					RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
					pData->rdh.dwSize = sizeof(RGNDATAHEADER);
					pData->rdh.iType = RDH_RECTANGLES;
					pData->rdh.nCount = pData->rdh.nRgnSize = 0;
					SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

					// Keep on hand highest and lowest values for the "transparent" pixel
					BYTE lr = GetRValue(cTransparentColor);
					BYTE lg = GetGValue(cTransparentColor);
					BYTE lb = GetBValue(cTransparentColor);
					BYTE hr = min(0xff, lr + GetRValue(cTolerance));
					BYTE hg = min(0xff, lg + GetGValue(cTolerance));
					BYTE hb = min(0xff, lb + GetBValue(cTolerance));

					// Scan each bitmap row from bottom to top (the bitmap is inverted vertically
					BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
					for (int y = 0; y < bm.bmHeight; y++)
					{
						// Scan each bitmap pixel from left to righ
						for (int x = 0; x < bm.bmWidth; x++)
						{
							// Search for a continuous range of "non transparent pixels"
							int x0 = x;
							LONG *p = (LONG *)p32 + x;
							while (x < bm.bmWidth)
							{
								BYTE b = GetRValue(*p);
								if (b >= lr && b <= hr)
								{
									b = GetGValue(*p);
									if (b >= lg && b <= hg)
									{
										b = GetBValue(*p);
										if (b >= lb && b <= hb)
											// This pixel is "transparent"
											break;
									}
								}
								p++;
								x++;
							}

							if (x > x0)
							{
								// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the regio
								if (pData->rdh.nCount >= maxRects)
								{
									GlobalUnlock(hData);
									maxRects += ALLOC_UNIT;
									hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
									pData = (RGNDATA *)GlobalLock(hData);
									//ASSERT(pData);
								}
								RECT *pr = (RECT *)&pData->Buffer;
								SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
								if (x0 < pData->rdh.rcBound.left)
									pData->rdh.rcBound.left = x0;
								if (y < pData->rdh.rcBound.top)
									pData->rdh.rcBound.top = y;
								if (x > pData->rdh.rcBound.right)
									pData->rdh.rcBound.right = x;
								if (y+1 > pData->rdh.rcBound.bottom)
									pData->rdh.rcBound.bottom = y+1;
								pData->rdh.nCount++;

								// On Windows98, ExtCreateRegion() may fail if the number of rectangles is to
								// large (ie: > 4000). Therefore, we have to create the region by multiple steps
								if (pData->rdh.nCount == 2000)
								{
									HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
									//ASSERT(h);
									if (hRgn)
									{
										CombineRgn(hRgn, hRgn, h, RGN_OR);
										DeleteObject(h);
									}
									else
										hRgn = h;
									pData->rdh.nCount = 0;
									SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
								}
							}
						}

						// Go to next row (remember, the bitmap is inverted vertically
						p32 -= bm32.bmWidthBytes;
					}

					// Create or extend the region with the remaining rectangle
					HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
					//ASSERT(h);
					if (hRgn)
					{
						CombineRgn(hRgn, hRgn, h, RGN_OR);
						DeleteObject(h);
					}
					else
						hRgn = h;

					// Clean u
					GlobalFree(hData);
					SelectObject(hDC, holdBmp);
					DeleteDC(hDC);
				}

				DeleteObject(SelectObject(hMemDC, holdBmp));
			}

			DeleteDC(hMemDC);
		}	
	}

	return hRgn;
}