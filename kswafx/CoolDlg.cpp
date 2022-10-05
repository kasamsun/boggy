#include "CoolDlg.h"

#include <commctrl.h>
#include <MainAfx.h>
#include <DCEx.h>
#include <CoolButton.h>
#include <CoolStatic.h>

// ===========================================================
//  Restriction : can use only Modal CoolDlg like STACK 
//				 create first , destroy last ......
// ===========================================================

HHOOK hCoolDlgHook = NULL;
MyList<C_CoolDlg> pCoolDlgList;

HDC C_CoolDlg::m_hdcCoolDlgImage = NULL;
HBITMAP C_CoolDlg::m_hbmpCoolDlgImage = NULL;
HBITMAP C_CoolDlg::m_hbmpTmp = NULL;
int C_CoolDlg::m_bFading = TRUE;

int WINAPI C_CoolDlg::CoolDlgSetImage(HINSTANCE hInstance,LPCTSTR lpszName)
{
	HBITMAP hbmpImage;

	if ( hInstance == NULL )
		hbmpImage = (HBITMAP)LoadImage(NULL,lpszName,
							IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	else
		hbmpImage = (HBITMAP)LoadBitmap(hInstance,lpszName);

	if ( hbmpImage == NULL ) return FALSE;

	CoolDlgClearImage();
    m_hbmpCoolDlgImage = hbmpImage;

	HDC hDC = GetDC(GetDesktopWindow());
	m_hdcCoolDlgImage = CreateCompatibleDC(hDC);
	m_hbmpTmp = (HBITMAP)SelectObject(m_hdcCoolDlgImage,m_hbmpCoolDlgImage);
	ReleaseDC(GetDesktopWindow(),hDC);

	return TRUE;
}

int WINAPI C_CoolDlg::CoolDlgClearImage()
{
	if ( m_hdcCoolDlgImage != NULL )
	{
		DeleteObject(SelectObject(m_hdcCoolDlgImage,m_hbmpTmp));
		DeleteDC(m_hdcCoolDlgImage);
		m_hdcCoolDlgImage = NULL;
	}
	return TRUE;
}

void WINAPI C_CoolDlg::CoolDlgEnableFading(int bFlag)
{
	m_bFading = bFlag;
}

C_CoolDlg::C_CoolDlg()
{
	m_lpfnDefCoolDlgProc = NULL;
}

C_CoolDlg::~C_CoolDlg()
{
}

int C_CoolDlg::CoolDlgSubclassDialog()
{
	SetWindowLong(m_hWnd,DWL_USER,(LONG)this);

    OSVERSIONINFO osver;
    osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if ( GetVersionEx(&osver) == 0 )
        return FALSE;
    if ( osver.dwPlatformId == VER_PLATFORM_WIN32_NT )
        // for NT
	    m_lpfnDefCoolDlgProc = (DLGPROC)SetWindowLong(m_hWnd,DWL_DLGPROC,(LONG)::CoolDlgProc);
    else
        // for 95,98
        m_lpfnDefCoolDlgProc = (DLGPROC)SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)::CoolDlgProc);
	return TRUE;
}

int C_CoolDlg::CoolDlgSubclassControl(HWND hDlg)
{
    char szClass[64];
    MyList<HWND> hWndList;

	m_CtlList.DeleteObjects();

	for ( HWND hWnd = GetTopWindow(hDlg) ; hWnd != NULL ; hWnd = GetNextWindow(hWnd,GW_HWNDNEXT) )
    {
		int bDup = FALSE;
	    for ( int i=0; i < hWndList.Count() ; i++ )
    		if ( *hWndList.Items(i) == hWnd )
            {
            	bDup = TRUE;
            	break;
            }

        if ( bDup )
        	continue;
        else
            hWndList.Add(new HWND(hWnd));

//======testing==== removable when no need trace
//		char tmpbuf[99];
//		GetWindowText(hWnd,tmpbuf,sizeof(tmpbuf));
//		LONG lStyle = GetWindowLong(hWnd,GWL_STYLE);
//=================
        GetClassName(hWnd,szClass,sizeof(szClass));

        if ( strcmp(szClass,"Button") == 0 )
        {
//	        TRACE("hwnd[%x][%s] style[%8x]\n",hWnd,tmpbuf,lStyle);
            C_CoolButton * pCoolButton = new C_CoolButton;
            pCoolButton->SubclassControl(hWnd,this);
            m_CtlList.Add(pCoolButton);
        }
        else if ( strcmp(szClass,"Static") == 0 )
        {
//			TRACE("hwnd[%x][%s] style[%8x]\n",hWnd,tmpbuf,lStyle);
            C_CoolStatic * pCoolStatic = new C_CoolStatic;
            pCoolStatic->SubclassControl(hWnd,this);
            m_CtlList.Add(pCoolStatic);
        }
	}
    hWndList.DeleteObjects();
    return TRUE;
}

void C_CoolDlg::InitializeCoolDlg()
{
    RECT rc;

	GetClientRect(m_hWnd,&rc);
	HDC hDC = GetDC(m_hWnd);
    m_hdcCoolBkMem = CreateCompatibleDC(hDC);
    m_hbmpCoolBkMem = CreateCompatibleBitmap(hDC,rc.right-rc.left,rc.bottom-rc.top);
    m_hbmpCoolBkTmp = (HBITMAP)SelectObject(m_hdcCoolBkMem,m_hbmpCoolBkMem);

	// if set image , use this image as background
	if ( m_hdcCoolDlgImage != NULL )
	{
		BITMAP bm;
		GetObject(m_hbmpCoolDlgImage,sizeof(bm),&bm);
		for ( int i = 0 ; i < rc.right-rc.left ; i+=bm.bmWidth )
			for ( int j = 0 ; j < rc.bottom-rc.top ; j+=bm.bmHeight )
				BitBlt(m_hdcCoolBkMem,i,j,bm.bmWidth,bm.bmHeight,
						m_hdcCoolDlgImage,0,0,SRCCOPY);
		ReleaseDC(m_hWnd,hDC);
		return;
	}

	// if no custom image present
	if ( m_bFading )
	{
	    C_DCEx * pDCEx = new C_DCEx(m_hdcCoolBkMem);
		pDCEx->FillGradientRect(&rc,GetSysColor(COLOR_3DFACE),
			                        GetSysColor(COLOR_3DHILIGHT));
		delete pDCEx;
	}
	else
	{
        HBRUSH hb;
        hb = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
        HBRUSH hbtmp = (HBRUSH)SelectObject(m_hdcCoolBkMem,hb);
        FillRect(m_hdcCoolBkMem,&rc,hb);
        DeleteObject(SelectObject(m_hdcCoolBkMem,hbtmp));
	}
    ReleaseDC(m_hWnd,hDC);
}

void C_CoolDlg::Paint(HDC hDC)
{
	RECT rc;
	GetClientRect(m_hWnd,&rc);
	BitBlt(hDC,0,0,rc.right-rc.left,rc.bottom-rc.top,m_hdcCoolBkMem,0,0,SRCCOPY);
}

void C_CoolDlg::UninitializeCoolDlg()
{
	DeleteObject(SelectObject(m_hdcCoolBkMem,m_hbmpCoolBkTmp));
	DeleteDC(m_hdcCoolBkMem);
	m_CtlList.DeleteObjects();
}


void C_CoolDlg::SetCoolDlgRgn()
{
	LONG lStyle = GetWindowLong(m_hWnd,GWL_STYLE);
	if ( !(lStyle & DS_MODALFRAME) ) return;
	RECT rc;
	GetWindowRect(m_hWnd,&rc);
	POINT pnt[8];
	POINT *p = pnt;
	int nWidth = rc.right-rc.left;
	int nHeight = rc.bottom-rc.top;
	p->x = 0;p->y = 5;		p++;
	p->x = 0;p->y = nHeight-5;	p++;
	p->x = 5;p->y = nHeight;	p++;
	p->x = nWidth-5;p->y = nHeight;	p++;
	p->x = nWidth;p->y = nHeight-5;	p++;
	p->x = nWidth;p->y = 5;	p++;
	p->x = nWidth-5;p->y = 0;	p++;
	p->x = 5;p->y = 0;		p++;
	HRGN hrgn = CreatePolygonRgn(pnt,8,WINDING);
	SetWindowRgn(m_hWnd,hrgn,FALSE);
}

void C_CoolDlg::DrawCoolDlgFrame(HWND hWnd,HDC hDC)
{
	// draw frame with bevel
	RECT rc;
	GetWindowRect(hWnd,&rc);
	int nWidth = rc.right-rc.left-1;
	int nHeight = rc.bottom-rc.top-1;

	HPEN hp = CreatePen(PS_SOLID,1,GetSysColor(COLOR_3DFACE));
	DeleteObject(SelectObject(hDC,hp));
	MoveToEx(hDC,nWidth,0,NULL);
	LineTo(hDC,0,0);
	LineTo(hDC,0,nHeight);

	hp = CreatePen(PS_SOLID,1,RGB(0,0,0));
	DeleteObject(SelectObject(hDC,hp));
	MoveToEx(hDC,0,nHeight,NULL);
	LineTo(hDC,nWidth+1,nHeight);
	MoveToEx(hDC,nWidth,nHeight,NULL);
	LineTo(hDC,nWidth,0);

	nWidth--;nHeight--;
	hp = CreatePen(PS_SOLID,1,GetSysColor(COLOR_3DHILIGHT));
	DeleteObject(SelectObject(hDC,hp));
	MoveToEx(hDC,nWidth,1,NULL);
	LineTo(hDC,1,1);
	LineTo(hDC,1,nHeight);

	hp = CreatePen(PS_SOLID,1,GetSysColor(COLOR_3DSHADOW));
	DeleteObject(SelectObject(hDC,hp));
	MoveToEx(hDC,1,nHeight,NULL);
	LineTo(hDC,nWidth+1,nHeight);
	MoveToEx(hDC,nWidth,nHeight,NULL);
	LineTo(hDC,nWidth,1);

	nWidth--;nHeight--;
	hp = CreatePen(PS_SOLID,1,GetSysColor(COLOR_3DFACE));
	DeleteObject(SelectObject(hDC,hp));
	MoveToEx(hDC,nWidth,2,NULL);
	LineTo(hDC,2,2);
	LineTo(hDC,2,nHeight);

	MoveToEx(hDC,2,nHeight,NULL);
	LineTo(hDC,nWidth+1,nHeight);
	MoveToEx(hDC,nWidth,nHeight,NULL);
	LineTo(hDC,nWidth,2);
}

void C_CoolDlg::DrawCoolDlgCaption(HWND hWnd,HDC hDC)
{
	RECT rc;
	GetWindowRect(hWnd,&rc);

	rc.right = rc.right-rc.left-GetSystemMetrics(SM_CXDLGFRAME);
	rc.left = GetSystemMetrics(SM_CXDLGFRAME);

	rc.bottom = GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYDLGFRAME);
	rc.top = GetSystemMetrics(SM_CYDLGFRAME);

	NONCLIENTMETRICS ncMetric;
	ncMetric.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
					sizeof(NONCLIENTMETRICS),&ncMetric,0);
	int nYCaption = GetSystemMetrics(SM_CYCAPTION);
//	int nYFrame = GetSystemMetrics(SM_CYDLGFRAME);
	LONG lStyle = GetWindowLong(hWnd,GWL_STYLE);
    int nWidth = rc.right-rc.left;

	// draw background & sysmenu button ( if exists )
	HBRUSH hb = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	if ( lStyle & WS_SYSMENU )
	{
		nWidth -= ncMetric.iCaptionWidth;
	}
	FillRect(hDC,&rc,hb);
	if ( lStyle & WS_SYSMENU )
	{
		RECT rcClsButton = { rc.right-(ncMetric.iCaptionWidth-1),rc.top+2,rc.right-2,rc.top+ncMetric.iCaptionHeight-2 };
		DrawFrameControl(hDC,&rcClsButton,DFC_CAPTION,DFCS_CAPTIONCLOSE);
	}
	DeleteObject(hb);
	SetBkMode(hDC,TRANSPARENT);
	char tmpbuf[256];
	GetWindowText(hWnd,tmpbuf,sizeof(tmpbuf));

	HFONT hFont = CreateFontIndirect(&ncMetric.lfCaptionFont);
	HFONT hTmpFont = (HFONT)SelectObject(hDC,hFont);

	SIZE s;
	GetTextExtentPoint32(hDC,tmpbuf,strlen(tmpbuf),&s);

	// draw caption text and caption face
	HPEN hp = CreatePen(PS_SOLID,1,GetSysColor(COLOR_3DHILIGHT));
	DeleteObject(SelectObject(hDC,hp));

	int i;
	nYCaption = (nYCaption-4)/3;
    for ( i=0 ; i<nYCaption ; i++ )
    {
		MoveToEx(hDC,s.cx+5,i*3+5,NULL);
		LineTo(hDC,nWidth,i*3+5);
    }
	hp = CreatePen(PS_SOLID,1,GetSysColor(COLOR_3DSHADOW));
	DeleteObject(SelectObject(hDC,hp));
    for ( i=0 ; i<nYCaption ; i++ )
    {
		MoveToEx(hDC,s.cx+5,i*3+6,NULL);
		LineTo(hDC,nWidth,i*3+6);
    }

    if ( IsWindowEnabled(m_hWnd) )
    	SetTextColor(hDC,GetSysColor(COLOR_ACTIVECAPTION));
    else
    	SetTextColor(hDC,GetSysColor(COLOR_INACTIVECAPTION));
    TextOut(hDC,5,5,tmpbuf,strlen(tmpbuf));
    if ( IsWindowEnabled(m_hWnd) )
    	SetTextColor(hDC,GetSysColor(COLOR_CAPTIONTEXT));
    else
    	SetTextColor(hDC,GetSysColor(COLOR_INACTIVECAPTIONTEXT));
    TextOut(hDC,4,4,tmpbuf,strlen(tmpbuf));
	DeleteObject(SelectObject(hDC,hTmpFont));
}


LRESULT C_CoolDlg::WndProc(UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	LRESULT lRetVal;

	switch (iMessage)
    {
	case WM_INITDIALOG:
		// =====================================================
		// checking for dialog created by kswafx
		// prevent recursive WM_INITDIALOG calling
		// at ::WndProc
		// =====================================================
        // subclass all controls
		CoolDlgSubclassControl(m_hWnd);
		if ( GetWindowLong(m_hWnd,GWL_USERDATA) == 0 )
			lRetVal = CallWindowProc((WNDPROC)m_lpfnDefCoolDlgProc,m_hWnd,iMessage, wParam, lParam);
		InitializeCoolDlg();
//		SetCoolDlgRgn();
        return lRetVal;
    case WM_PAINT:
    	m_hDC = BeginPaint(m_hWnd,&ps);
		Paint(m_hDC);
        EndPaint(m_hWnd,&ps);
		return TRUE;
	case WM_NCDESTROY:
		lRetVal = CallWindowProc((WNDPROC)m_lpfnDefCoolDlgProc,m_hWnd,iMessage, wParam, lParam);
		UninitializeCoolDlg();
        return lRetVal;
    case WM_NCACTIVATE:
    case WM_NCPAINT:
		// need original paint because it's to repaint
		// normal state's cool button. in case of click to 
		// popup new window 
		lRetVal = CallWindowProc((WNDPROC)m_lpfnDefCoolDlgProc,m_hWnd,iMessage, wParam, lParam);
	    m_hDC = GetWindowDC(m_hWnd);
		DrawCoolDlgFrame(m_hWnd,m_hDC);
		DrawCoolDlgCaption(m_hWnd,m_hDC);
		ReleaseDC(m_hWnd,m_hDC);
		return lRetVal;
	case WM_SYSCOMMAND:
		// redraw caption when press system button on the caption
		m_hDC = GetWindowDC(m_hWnd);
		DrawCoolDlgCaption(m_hWnd,m_hDC);
		ReleaseDC(m_hWnd,m_hDC);
		return CallWindowProc((WNDPROC)m_lpfnDefCoolDlgProc,m_hWnd,iMessage, wParam, lParam);
	}
	return CallWindowProc((WNDPROC)m_lpfnDefCoolDlgProc,m_hWnd,iMessage, wParam, lParam);
}


// ============================================================
// CoolDlg API
// ============================================================
int WINAPI CoolDlgAutoSubclass()
{
	if (hCoolDlgHook != NULL) return FALSE;
    hCoolDlgHook = SetWindowsHookEx(WH_CALLWNDPROC,(HOOKPROC)::CoolDlgHook,C_App::m_hInstance,
                            (DWORD)GetCurrentThreadId() );
    if (hCoolDlgHook == NULL) return FALSE;
    return TRUE;
}

int WINAPI CoolDlgTerminate()
{
	if ( hCoolDlgHook != NULL )
	{
		if ( UnhookWindowsHookEx( hCoolDlgHook ) )
		{
			hCoolDlgHook = NULL;
			return TRUE;
		}
	}
	return TRUE;
}

LRESULT CoolDlgProc( HWND hWnd,UINT iMessage, WPARAM wParam, LPARAM lParam )
{
    C_Wnd *pWindow = (C_Wnd*) GetWindowLong(hWnd,DWL_USER);

	if (  iMessage == WM_NCDESTROY )
	{
		LRESULT lRetVal = pWindow->WndProc(iMessage,wParam,lParam);
		// search in CoolDlg to destroy the desired one
		for ( int i = 0 ; i < pCoolDlgList.Count() ; i++ )
		{
			if ( pCoolDlgList.Items(i)->GetHandle() == hWnd )
			{
				delete pCoolDlgList.Items(i);
				pCoolDlgList.Remove(i);
				break;
			}
		}
		return lRetVal;
	}
	else
		return pWindow->WndProc( iMessage, wParam, lParam );
}

LRESULT WINAPI CoolDlgHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
	{
		CWPSTRUCT * pwp = (CWPSTRUCT*)lParam;
		if ( pwp->message == WM_INITDIALOG )
		{
            C_CoolDlg * pCoolDlg = new C_CoolDlg; 
            pCoolDlgList.Add(pCoolDlg);
            pCoolDlg->SetHandle(pwp->hwnd);
			pCoolDlgList.Items(pCoolDlgList.Count()-1)->CoolDlgSubclassDialog();
        }
    }
	return CallNextHookEx(hCoolDlgHook,nCode,wParam,lParam);
}

