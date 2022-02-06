#include <MainAfx.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "BoggyApp.h"
#include "resource.h"

#include "BoggyWnd.h"
#include "PreferenceWnd.h"

#define WS_EX_LAYERED           0x00080000

BEGIN_HANDLE_MSG(C_BoggyWnd,C_GenericWnd)
	HANDLE_MSG(WM_PAINT,OnPaint)
	HANDLE_MSG(WM_ACTIVATE,OnActivate)
	HANDLE_MSG(WM_SIZE,OnSize)
	HANDLE_MSG(WM_MOVE,OnMove)
	HANDLE_MSG(WM_NCCALCSIZE,OnNCCalcSize)
	HANDLE_MSG(WM_TIMER,OnTimer)
	HANDLE_MSG(WM_ERASEBKGND,OnEraseBkgnd)
	HANDLE_MSG(WM_LBUTTONDOWN,OnLButtonDown)
	HANDLE_MSG(WM_MOUSEMOVE,OnMouseMove)
	HANDLE_MSG(WM_LBUTTONUP,OnLButtonUp)
	HANDLE_MSG(WM_LBUTTONDBLCLK,OnLButtonDblClk)
	HANDLE_MSG(WM_RBUTTONDBLCLK,OnRButtonDblClk)
	HANDLE_MSG(WM_RBUTTONUP,OnRButtonUp)
	HANDLE_MSG(WM_DESTROY,OnDestroy)
END_HANDLE_MSG()

C_BoggyWnd::C_BoggyWnd( C_Wnd *pParentWnd ) : C_GenericWnd(pParentWnd)
{
	RANDOMIZE();
	RANDOM(RAND_MAX);

	
	m_nScreenSize = DOUBLE_SIZE;
	m_nWidth = 32*m_nScreenSize;
	m_nHeight = 32*m_nScreenSize;

	m_bFirstActivated = TRUE;
	m_bDestroy = FALSE;

	m_bLPressed = FALSE;
	m_bMoveWithNoBitCopy = FALSE;

	m_hWnd = CreateWindowEx(
                    WS_EX_LAYERED |WS_EX_TOPMOST,
                    //0,
                    m_szClassName,
                    m_szClassName,
                    WS_VISIBLE|WS_POPUP,
                    -32*m_nScreenSize,	// if don't process WM_ERASEBKG,
                                        // must run from outside screen
                    RANDOM(GetSystemMetrics(SM_CYFULLSCREEN)-32*m_nScreenSize),
                    m_nWidth,
                    m_nHeight,
                    pParentWnd->GetHandle(),
                    NULL,
                    C_App::m_hInstance,
                    (LPSTR) this );

    if ( ! m_hWnd )
        exit( FALSE );

	
	HMODULE hUser32 = GetModuleHandle("USER32.DLL");
	m_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)GetProcAddress(hUser32, "SetLayeredWindowAttributes");
	m_pSetLayeredWindowAttributes(m_hWnd,RGB(0, 128, 128),125,1);

	m_pBoggyAI = new C_BoggyAI(this);

	m_pAction[STAND]		= new C_BoggyAction(2,2,0,IDB_STAND,m_nScreenSize);
	m_pAction[RUN]		= new C_BoggyAction(6,1,5,IDB_RUN,m_nScreenSize);
	m_pAction[RUN_SLOBBER]= new C_BoggyAction(6,1,5,IDB_RUNS,m_nScreenSize);

	m_pCurAction = m_pAction[RUN];
	m_pCurAction->StartAction(RIGHT);

    ShowWindow(m_hWnd,C_App::m_nCmdShow);
    UpdateWindow(m_hWnd);
}

C_BoggyWnd::~C_BoggyWnd()
{
	delete m_pBoggyAI;

	for (int i=0 ;i<NUM_OF_ACTION; i++)
		delete m_pAction[i];

}

void C_BoggyWnd::CreateBoggySaveBitmap(int nMode)
{
	m_hDC = GetDC(m_hWnd);

	m_hbmpBkg = CreateCompatibleBitmap(m_hDC,m_nWidth,m_nHeight);
	m_hdcBkg = CreateCompatibleDC(m_hDC);
	m_hbmpBkgOld = (HBITMAP) SelectObject(m_hdcBkg,m_hbmpBkg);
	ReleaseDC(m_hWnd,m_hDC);
}

void C_BoggyWnd::DeleteBoggySaveBitmap()
{
	DeleteObject(SelectObject(m_hdcBkg,m_hbmpBkgOld));
	DeleteDC(m_hdcBkg);
}

void C_BoggyWnd::Still()
{
	HDC hdcsave;
	HBITMAP hbmpsave,bmsave;

	m_hDC = GetDC(m_hWnd);
	hdcsave = CreateCompatibleDC(m_hDC);
	hbmpsave = CreateCompatibleBitmap(m_hDC,m_nWidth,m_nHeight);
	bmsave = (HBITMAP)SelectObject(hdcsave,hbmpsave);

	BitBlt(hdcsave,0,0,m_nWidth,m_nHeight,m_hdcBkg,0,0,SRCCOPY);
	BitBlt (hdcsave,0,0,m_nWidth,m_nHeight,m_pCurAction->m_hdcImg,
			(m_pCurAction->m_nCurFrame-1)*32*m_nScreenSize,
			m_pCurAction->m_nDirection*32*m_nScreenSize, SRCCOPY);
	BitBlt(m_hDC,0,0,m_nWidth,m_nHeight,hdcsave,0,0,SRCCOPY);

	DeleteObject(SelectObject(hdcsave,bmsave));
	DeleteDC(hdcsave);
	ReleaseDC(m_hWnd,m_hDC);
}

void C_BoggyWnd::MoveBoggyWindow(int left,int top)
{
	RECT rc;
	int difx,dify;
	int stx,sty,enx,eny;
	int xSize = 32*m_nScreenSize;
	int ySize = 32*m_nScreenSize;

	GetWindowRect(m_hWnd,&rc);

	difx = rc.left - left;
	dify = rc.top - top;

	//-------set up x ---------
	if ( difx == 0 ) 
	{
		stx = 0;enx = 0;
	}
	else if ( difx > 0 ) // go left
	{
		stx = difx;enx = 0;
	}
	else // go right
	{
		stx = 0;enx = -difx;
	}
	//////////set up y/////////
	if ( dify == 0 )
	{
		sty = 0;eny = 0;
	}
	else if ( dify > 0 ) // go up
	{
		sty = dify;eny = 0;
	}
	else // go down
	{
		sty = 0;eny = -dify;
	}

	
	HDC hdcsave;
	HBITMAP hbmpsave,bmsave;
	m_hDC = GetDC(m_hWnd);
	hdcsave = CreateCompatibleDC(m_hDC);
	hbmpsave = CreateCompatibleBitmap(m_hDC,xSize,ySize);
	bmsave = (HBITMAP)SelectObject(hdcsave,hbmpsave);
	
	
	BitBlt(hdcsave,0,0,xSize,ySize,m_pCurAction->m_hdcImg,
					(m_pCurAction->m_nCurFrame-1)*32*m_nScreenSize,
					m_pCurAction->m_nDirection*32*m_nScreenSize, SRCCOPY);
	BitBlt(m_hDC,0,0,m_nWidth,m_nHeight,hdcsave,0,0,SRCCOPY);
	
	MoveWindow(m_hWnd,rc.left-stx+enx,rc.top-sty+eny,xSize,ySize,TRUE);

	DeleteObject(SelectObject(hdcsave,bmsave));
	DeleteDC(hdcsave);

	ReleaseDC(m_hWnd,m_hDC);

}


/////////////////////// Event Handler /////////////////////
LRESULT C_BoggyWnd::OnPaint(WPARAM wParam,LPARAM lParam)
{
    PAINTSTRUCT ps;
    m_hDC = BeginPaint( m_hWnd, &ps );
	if ( m_bFirstActivated )
	{
		CreateBoggySaveBitmap(TRIPLE_SIZE);
		SetTimer(m_hWnd,IDTIMER_UPDATE,TIMER_UPDATE,NULL);
		m_bFirstActivated = FALSE;
	}
	BitBlt (m_hDC,0,0,m_nWidth,m_nHeight,m_pCurAction->m_hdcImg,
			(m_pCurAction->m_nCurFrame-1)*32*m_nScreenSize,
			m_pCurAction->m_nDirection*32*m_nScreenSize, SRCCOPY);
    EndPaint( m_hWnd, &ps );
	return 0;
}

LRESULT C_BoggyWnd::OnActivate(WPARAM wParam,LPARAM lParam)
{
	if ( m_bFirstActivated )
	{
		CreateBoggySaveBitmap(TRIPLE_SIZE);
		SetTimer(m_hWnd,IDTIMER_UPDATE,TIMER_UPDATE,NULL);
		m_bFirstActivated = FALSE;
	}
	return 0;
}

LRESULT C_BoggyWnd::OnSize(WPARAM wParam,LPARAM lParam)
{
	m_nWidth = (int)LOWORD(lParam);  // width of client area
	m_nHeight = (int)HIWORD(lParam); // height of client area
	//TRACE("nWidth=%d,nHeight=%d\n",nWidth,nHeight);
	return 0;
}

LRESULT C_BoggyWnd::OnMove(WPARAM wParam,LPARAM lParam)
{
	//TRACE("wm_move\n");
	// update virtual screen object
	return 0;
}

LRESULT C_BoggyWnd::OnNCCalcSize(WPARAM wParam,LPARAM lParam)
{
	if ( !(BOOL)wParam )
		return 0;
	else
		return WVR_VALIDRECTS;
}

LRESULT C_BoggyWnd::OnTimer(WPARAM wParam,LPARAM lParam)
{
	if ( m_bLPressed )
	{
		static tmp=FALSE;
		tmp = ~tmp;
		if ( tmp )
		{
			m_pCurAction->m_nCurFrame++;
			if (m_pCurAction->m_nCurFrame>2 )
				m_pCurAction->m_nCurFrame = 1;
			Still();
		}
#ifdef _DEBUG   // update debug window
		InvalidateRect(GetParent(m_hWnd),NULL,FALSE);
#endif
		return 0;
	}
	switch(wParam)
	{
	case IDTIMER_UPDATE:
		if ( m_pCurAction->IsFinish() )
		{
			// if last action finish => generate next action
			int nNext = m_pBoggyAI->GenNextAction();
			m_pCurAction = m_pAction[0x0000ffff & nNext];
			m_pCurAction->StartAction(nNext>>16);
		}
		switch (m_pCurAction->DoAction())
		{
		case MOVE:
			{
			RECT rc;
			GetWindowRect(m_hWnd,&rc);
			if ( m_pCurAction->m_nDirection == LEFT )
			{
				MoveBoggyWindow(rc.left-(m_pCurAction->m_nMoveStep*m_nScreenSize),rc.top);
			}
			else
			{
				MoveBoggyWindow(rc.left+(m_pCurAction->m_nMoveStep*m_nScreenSize),rc.top);
			}
			////// old moving
			//Moving(pCurAction->nDirection,
			//			pCurAction->nMoveStep*nScreenSize);
			}
			break;
		case STILL:
			Still();
			break;
		case NOTHING:
			break;
		default:
			Still();
			break;
		}
		m_pCurAction->ResumeAction();
		break;
	}
#ifdef _DEBUG   // update debug window
	InvalidateRect(GetParentWnd()->GetHandle(),NULL,FALSE);
#endif
	return 0;
}

LRESULT C_BoggyWnd::OnEraseBkgnd(WPARAM wParam,LPARAM lParam)
{
	// top most called once
	// if first appearence is somewhere on screen
	BitBlt (m_hdcBkg,0,0,m_nWidth,m_nHeight,(HDC)wParam,0,0, SRCCOPY);
	return 1;
}

LRESULT C_BoggyWnd::OnLButtonDown(WPARAM wParam,LPARAM lParam)
{
	int tmp = m_pCurAction->m_nDirection;
	m_pCurAction = m_pAction[STAND];
	m_pCurAction->StartAction(tmp);
	m_bLPressed = TRUE;
	SetCapture(m_hWnd);
	m_nPosX = LOWORD(lParam);
	m_nPosY = HIWORD(lParam);
	return 0;
}
LRESULT C_BoggyWnd::OnMouseMove(WPARAM wParam,LPARAM lParam)
{
	if ( m_bLPressed )
	{
		POINT p = {(int)LOWORD(lParam),(int)HIWORD(lParam)};
		ClientToScreen(m_hWnd,&p);
		// * i don't know why x,y over 65535 , so i fix////
		p.x &= 0x0000ffff;
		p.y &= 0x0000ffff;
		///////////////////////////////////////////////////
		MoveBoggyWindow(p.x-m_nPosX,p.y-m_nPosY);
	}
	return 0;
}
//	case WM_WINDOWPOSCHANGING:
//		if ( /*bLButtonPress &&*/ bMoveWithNoBitCopy )
//			((LPWINDOWPOS) lParam)->flags |= SWP_NOCOPYBITS;
//		break;

LRESULT C_BoggyWnd::OnLButtonUp(WPARAM wParam,LPARAM lParam)
{
	ReleaseCapture();
	m_bLPressed = FALSE;
	//////////////
	//pCurAction->nCurFrame++;
	return 0;
}

LRESULT C_BoggyWnd::OnLButtonDblClk(WPARAM wParam,LPARAM lParam)
{
	static bActive = FALSE;
	if ( !bActive )
	{
		bActive = TRUE;
		C_PreferenceWnd PreferenceWnd(m_hWnd,IDB_PREFERENCE);
		bActive = FALSE;
	}
	return 0;
}

LRESULT C_BoggyWnd::OnRButtonDblClk(WPARAM wParam,LPARAM lParam)
{
	m_bDestroy = TRUE;
	return 0;
}

LRESULT C_BoggyWnd::OnRButtonUp(WPARAM wParam,LPARAM lParam)
{
	if ( m_bDestroy ) DestroyWindow(m_hWnd);
	return 0;
}

LRESULT C_BoggyWnd::OnDestroy(WPARAM wParam,LPARAM lParam)
{
 	KillTimer(m_hWnd,IDTIMER_UPDATE);
	DeleteBoggySaveBitmap();
	PostQuitMessage( 0 );
	return 0;
}
