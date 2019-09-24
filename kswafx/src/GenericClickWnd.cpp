#include <windows.h>
#include <commctrl.h>
#include <GenericClickWnd.h>

BEGIN_HANDLE_MSG(C_GenericClickWnd,C_GenericWnd)
	HANDLE_MSG(WM_LBUTTONDOWN,OnLButtonDown)
	HANDLE_MSG(WM_LBUTTONUP,OnLButtonUp)
	HANDLE_MSG(WM_RBUTTONDOWN,OnRButtonDown)
	HANDLE_MSG(WM_RBUTTONUP,OnRButtonUp)
	HANDLE_MSG(WM_MOUSEMOVE,OnMouseMove)
    HANDLE_MSG(WM_KILLFOCUS,OnKillFocus)
    HANDLE_MSG(WM_CHAR,OnChar)
	HANDLE_MSG(WM_NCCALCSIZE,OnNCCalcSize)
    HANDLE_MSG(WM_ERASEBKGND,OnEraseBkgnd)

END_HANDLE_MSG()

C_GenericClickWnd::C_GenericClickWnd(C_Wnd * pParentWnd) : C_GenericWnd(pParentWnd)
{
	m_hActiveCursor = NULL;
	m_hClickCursor = NULL;
    m_hDragCursor = NULL;
	m_hTmpCursor = NULL;

	m_bMouseAbove = FALSE;
	m_bLPressed = FALSE;
	m_bRPressed = FALSE;

	m_bDragWindow = FALSE;
    m_bKBMsgParentNotify = FALSE;
    m_bClickToTop  = FALSE;
	m_bClickNotify = FALSE;

	m_bMoveWithNoBitCopy = FALSE;
	m_bMoveBkgWindow = FALSE;
    m_bInitBkg = FALSE;
}

C_GenericClickWnd::~C_GenericClickWnd()
{
	if ( m_bMoveBkgWindow )
	   	ClearBkgImage();
}

void C_GenericClickWnd::SetActiveCursor(HCURSOR hActiveCursor)
{
	m_hActiveCursor = hActiveCursor;
}

void C_GenericClickWnd::SetClickCursor(HCURSOR hClickCursor)
{
	m_hClickCursor = hClickCursor;
}

void C_GenericClickWnd::SetDragCursor(HCURSOR hDragCursor)
{
	m_hDragCursor = hDragCursor;
}

void C_GenericClickWnd::EnableDragWindow(int bFlag)
{
	m_bDragWindow = bFlag;
}

void C_GenericClickWnd::EnableKBMsgParentNotify(int bFlag)
{
	m_bKBMsgParentNotify = bFlag;
}

void C_GenericClickWnd::EnableClickToTop(int bFlag)
{
    m_bClickToTop = bFlag;
}

void C_GenericClickWnd::EnableClickNotify(int bFlag)
{
    m_bClickNotify = bFlag;
}

void C_GenericClickWnd::EnableMoveBkgWindow(int bFlag)
{
	if ( m_bMoveBkgWindow == bFlag )
    	return;

	if ( !m_bMoveBkgWindow )
		SetBkgImage();
	else
		ClearBkgImage();

    m_bMoveBkgWindow = bFlag;
}

void C_GenericClickWnd::MoveBkgWindow(int left,int top)
{
	RECT rc;
	int stx,sty,enx,eny;

	GetClientRect(m_hWnd,&rc);

	int xSize = rc.right-rc.left;
	int ySize = rc.bottom-rc.top;
	int nWidth = rc.right-rc.left;
	int nHeight = rc.bottom-rc.top;

	GetWindowRect(m_hWnd,&rc);

	int difx = rc.left - left;
	int dify = rc.top - top;

    ScreenToClient(m_pParentWnd->GetHandle(),(LPPOINT)&rc.left);

    int goright = FALSE;
    int godown = FALSE;
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
        goright = TRUE;
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
        godown = TRUE;
	}

	if ( abs(difx) > nWidth ||
			abs(dify) > nHeight )
	{
		// no collapse, move windows with nocopybits
        POINT p;
        p.x = left;
        p.y = top;

	    ScreenToClient(m_pParentWnd->GetHandle(),&p);
		m_bMoveWithNoBitCopy = TRUE;
		MoveWindow(m_hWnd,p.x,p.y,xSize,ySize,TRUE);
        Refresh();
		//TRACE("move with nobitcopy to %ld,%ld\n",left,top);
		m_bMoveWithNoBitCopy = FALSE;

		// BUG....... didn't update hdcBkg ... find the way!
		//hDC = GetDC(hWnd);
		//BitBlt(hdcBkg,0,0,32*nScreenSize,32*nScreenSize,hDC,0,0,SRCCOPY);
		//ReleaseDC(hWnd,hDC);
	}
	else
	{
		// collapse, move windows with copybits
		m_bMoveWithNoBitCopy = FALSE;

		HDC hdcsave;
		HBITMAP hbmpsave,bmsave;
		HDC hdcsave1;
		HBITMAP hbmpsave1,bmsave1;

		m_hDC = GetDC(m_hWnd);
		hdcsave = CreateCompatibleDC(m_hDC);
		hbmpsave = CreateCompatibleBitmap(m_hDC,xSize+abs(difx),ySize+abs(dify));
		hdcsave1 = CreateCompatibleDC(m_hDC);
		hbmpsave1 = CreateCompatibleBitmap(m_hDC,xSize,ySize);
		bmsave = (HBITMAP)SelectObject(hdcsave,hbmpsave);
		bmsave1 = (HBITMAP)SelectObject(hdcsave1,hbmpsave1);

		// move windows by combination of org and dest
		MoveWindow(m_hWnd,rc.left-stx,rc.top-sty,xSize+abs(difx),ySize+abs(dify),FALSE);
		// restore old bkg
		BitBlt(hdcsave,0,0,nWidth,nHeight,m_hDC,0,0,SRCCOPY);
		BitBlt(hdcsave,stx,sty,xSize,ySize,m_hdcBkg,0,0,SRCCOPY);
		// save new bkg
		BitBlt(m_hdcBkg,0,0,xSize,ySize,hdcsave,enx,eny,SRCCOPY);
        if ( goright || godown ) //////////////////////////
        {
            Refresh();
        } ////////////////////////////////////////////////////
		// make another bkg copy to use in trans blt
		BitBlt(hdcsave1,0,0,xSize,ySize,m_hdcBkg,0,0,SRCCOPY);
        Paint(hdcsave1,0,0);
//		BoggyTransparentBlt (hdcsave1,0,0,xSize,ySize,m_pCurAction->m_hdcImg,
//						(m_pCurAction->m_nCurFrame-1)*32*m_nScreenSize,
//						m_pCurAction->m_nDirection*32*m_nScreenSize);
		// put new boggy image to dest
		BitBlt(hdcsave,enx,eny,xSize,ySize,hdcsave1,0,0,SRCCOPY);
		BitBlt(m_hDC,0,0,nWidth,nHeight,hdcsave,0,0,SRCCOPY);
		// move windows to destination
		MoveWindow(m_hWnd,rc.left-stx+enx,rc.top-sty+eny,xSize,ySize,TRUE);

		DeleteObject(SelectObject(hdcsave,bmsave));
		DeleteObject(SelectObject(hdcsave1,bmsave1));
		DeleteDC(hdcsave);
		DeleteDC(hdcsave1);

		ReleaseDC(m_hWnd,m_hDC);
	}
}

void C_GenericClickWnd::SetBkgImage()
{
	RECT rc;
    GetClientRect(m_hWnd,&rc);

	int nWidth = rc.right-rc.left;
    int nHeight = rc.bottom-rc.top;

	m_hDC = GetDC(m_hWnd);
	m_hbmpBkg = CreateCompatibleBitmap(m_hDC,nWidth,nHeight);
	m_hdcBkg = CreateCompatibleDC(m_hDC);
	m_hbmpBkgOld = (HBITMAP) SelectObject(m_hdcBkg,m_hbmpBkg);
	ReleaseDC(m_hWnd,m_hDC);

}

void C_GenericClickWnd::Refresh()
{
	RECT rc;
    GetClientRect(m_hWnd,&rc);

	int nWidth = rc.right-rc.left;
    int nHeight = rc.bottom-rc.top;

	m_bInitBkg = FALSE;
    RECT rMyRect;
    GetClientRect(m_hWnd,&rMyRect);

    ClientToScreen(m_hWnd,(LPPOINT)&rMyRect.left);
    ClientToScreen(m_hWnd,(LPPOINT)&rMyRect.right);
    ScreenToClient(m_pParentWnd->GetHandle(),(LPPOINT)&rMyRect.left);
    ScreenToClient(m_pParentWnd->GetHandle(),(LPPOINT)&rMyRect.right);
    InvalidateRect(m_pParentWnd->GetHandle(),&rMyRect,FALSE);

    BitBlt(m_hdcBkg,0,0,nWidth,nHeight,m_hDC,0,0,SRCCOPY);
}

void C_GenericClickWnd::ClearBkgImage()
{
	DeleteObject(SelectObject(m_hdcBkg,m_hbmpBkgOld));
	DeleteDC(m_hdcBkg);
}

//////////////////// Event Handling /////////////////////
LRESULT C_GenericClickWnd::OnLButtonDown(WPARAM wParam,LPARAM lParam)
{
	if ( !m_bMouseAbove ) OnMouseMove(wParam,lParam);

	if ( m_bClickToTop )
    {
	    BringWindowToTop(m_hWnd);
    	UpdateWindow(m_hWnd);
    }

    if ( m_bDragWindow )
    {
        // dragging mode
        if ( m_hDragCursor != NULL )
        {
            SetClassLong(m_hWnd,GCL_HCURSOR,(long)m_hDragCursor);
            SetCursor(m_hDragCursor);
        }
    	// clip cursor , when drag , and store point clicked
    	RECT rcp;
	    GetClientRect(m_pParentWnd->GetHandle(),&rcp);
	    ClientToScreen(m_pParentWnd->GetHandle(),(LPPOINT)&rcp.left);
    	ClientToScreen(m_pParentWnd->GetHandle(),(LPPOINT)&rcp.right);
		m_nClickPosX = (int)LOWORD(lParam)+rcp.left;
		m_nClickPosY = (int)HIWORD(lParam)+rcp.top;
		m_nPosX = (int)LOWORD(lParam);
		m_nPosY = (int)HIWORD(lParam);

        RECT rc;
        GetClientRect(m_hWnd,&rc);

        rcp.left += (int)LOWORD(lParam);
        rcp.right -= rc.right-rc.left-(int)LOWORD(lParam);
        rcp.top += (int)HIWORD(lParam);
        rcp.bottom -= rc.bottom-rc.top-(int)HIWORD(lParam);
	    ClipCursor(&rcp);

        ClientToScreen(m_hWnd,(LPPOINT)&rc.left);
        ScreenToClient(m_pParentWnd->GetHandle(),(LPPOINT)&rc.left);

		// before drag, we must store bkg first
		if ( m_bMoveBkgWindow )
			Refresh();
        // notify begin drag to parent
        if ( m_pParentWnd != NULL )
        	PostMessage(m_pParentWnd->GetHandle(),WM_GENERICCLICKWND_BEGIN_DRAG,
            			MAKEWPARAM((WORD)rc.left,(WORD)rc.top),(LPARAM)this);
    }
    else
    {
        // normal mode
        if ( m_hClickCursor != NULL && !m_bRPressed )
        {
            SetClassLong(m_hWnd,GCL_HCURSOR,(long)m_hClickCursor);
            SetCursor(m_hClickCursor);
        }
    }
	m_bLPressed = TRUE;
	return 0;
}

LRESULT C_GenericClickWnd::OnLButtonUp(WPARAM wParam,LPARAM lParam)
{
	if ( !m_bMouseAbove ) OnMouseMove(wParam,lParam);

    if ( m_bDragWindow )
    {
        // dragging mode
        if ( m_hDragCursor != NULL )
        {
			if ( m_hActiveCursor != NULL )
			{
	            SetClassLong(m_hWnd,GCL_HCURSOR,(long)m_hActiveCursor);
		        SetCursor(m_hActiveCursor);
			}
			else
			{
			    SetClassLong(m_hWnd,GCL_HCURSOR,(long)m_hTmpCursor);
				SetCursor(m_hTmpCursor);
			}
        }
	    ClipCursor(NULL);

		RECT rc;
		GetWindowRect(m_hWnd,&rc);
		POINT point = {(WORD)LOWORD(lParam),(WORD)HIWORD(lParam)};
		// make sure that hiword of x,y points be clear
		point.x &= 0x0000ffff;
		point.y &= 0x0000ffff;
		ClientToScreen(m_hWnd,&point);
		if ( m_bMoveBkgWindow )
			MoveBkgWindow(point.x-m_nPosX,point.y-m_nPosY);
    	else
			MoveWindow(m_hWnd,point.x-m_nClickPosX,point.y-m_nClickPosY,
						rc.right-rc.left,rc.bottom-rc.top,TRUE);

        // notify end drag to parent
        if ( m_pParentWnd != NULL )
        {
			RECT rcp;
			GetClientRect(m_hWnd,&rcp);
			ClientToScreen(m_hWnd,(LPPOINT)&rcp.left);
            ScreenToClient(m_pParentWnd->GetHandle(),(LPPOINT)&rcp.left);
        	PostMessage(m_pParentWnd->GetHandle(),WM_GENERICCLICKWND_END_DRAG,
            			MAKEWPARAM((WORD)rcp.left,(WORD)rcp.top),(LPARAM)this);
        }
    }
    else
    {
        // normal mode
        if ( m_hClickCursor != NULL && !m_bRPressed )
        {
			if ( m_hActiveCursor != NULL )
			{
	            SetClassLong(m_hWnd,GCL_HCURSOR,(long)m_hActiveCursor);
		        SetCursor(m_hActiveCursor);
			}
			else
			{
			    SetClassLong(m_hWnd,GCL_HCURSOR,(long)m_hTmpCursor);
				SetCursor(m_hTmpCursor);
			}
        }
 		if ( m_pParentWnd && m_bClickNotify )
			PostMessage(m_pParentWnd->GetHandle(),
					WM_GENERICCLICKWND_LBUTTONCLICK,
					0,(LPARAM)this);
   }
	m_bLPressed = FALSE;
	return 0;
}

LRESULT C_GenericClickWnd::OnRButtonDown(WPARAM wParam,LPARAM lParam)
{
	if ( !m_bMouseAbove ) OnMouseMove(wParam,lParam);

    if ( m_hClickCursor != NULL && !m_bLPressed )
    {
        SetClassLong(m_hWnd,GCL_HCURSOR,(long)m_hClickCursor);
        SetCursor(m_hClickCursor);
    }
	m_bRPressed = TRUE;
	return 0;
}

LRESULT C_GenericClickWnd::OnRButtonUp(WPARAM wParam,LPARAM lParam)
{
	if ( !m_bMouseAbove ) OnMouseMove(wParam,lParam);

    if ( m_hClickCursor != NULL && !m_bLPressed )
    {
		if ( m_hActiveCursor != NULL )
		{
	        SetClassLong(m_hWnd,GCL_HCURSOR,(long)m_hActiveCursor);
		    SetCursor(m_hActiveCursor);
		}
		else
		{
			SetClassLong(m_hWnd,GCL_HCURSOR,(long)m_hTmpCursor);
			SetCursor(m_hTmpCursor);
		}
    }
	if ( m_pParentWnd && m_bClickNotify )
		PostMessage(m_pParentWnd->GetHandle(),
				WM_GENERICCLICKWND_RBUTTONCLICK,
				0,(LPARAM)this);
	m_bRPressed = FALSE;
	return 0;
}

LRESULT C_GenericClickWnd::OnMouseMove(WPARAM ,LPARAM lParam)
{
	RECT rc;
	GetWindowRect(m_hWnd,&rc);
	POINT point = {(WORD)LOWORD(lParam),(WORD)HIWORD(lParam)};
    // make sure that hiword of x,y points be clear
    point.x &= 0x0000ffff;
    point.y &= 0x0000ffff;

    // ================================================================
    // remark : dragable window must be child window only.........
    // ================================================================
	if ( m_bLPressed && m_bDragWindow )
	{
		// draging
		ClientToScreen(m_hWnd,&point);
		if ( m_bMoveBkgWindow )
        {
        	// move with transparent to background
			MoveBkgWindow(point.x-m_nPosX,point.y-m_nPosY);
//			MoveBkgWindow(point.x-m_nClickPosX,point.y-m_nClickPosY);
        }
    	else
        {
        	// normal move
			MoveWindow(m_hWnd,point.x-m_nClickPosX,point.y-m_nClickPosY,
						rc.right-rc.left,rc.bottom-rc.top,TRUE);
        }
		return 0;
 	}

    HWND hWndFocus,hWndChildFocus;
	hWndChildFocus = ChildWindowFromPoint(m_hWnd,point);
//	TRACE("%d,%d hwnd = %d",point.x,point.y,hWndMainFocus);
	ClientToScreen(m_hWnd,&point);
	ScreenToClient(GetParentWnd()->GetHandle(),&point);
	hWndFocus = ChildWindowFromPoint(GetParentWnd()->GetHandle(),point);
//	TRACE(",%d\n",hWndFocus);

	if ( hWndFocus != m_hWnd || hWndChildFocus != m_hWnd )
	{	// mouse move to outside 
		SetClassLong(m_hWnd,GCL_HCURSOR,(long)m_hTmpCursor);
		SetCursor(m_hTmpCursor);
		//m_hTmpCursor = NULL;
		m_bMouseAbove = FALSE;
    	m_bLPressed = FALSE;
		m_bRPressed = FALSE;
        ReleaseCapture();
        if ( m_bDragWindow )
            ClipCursor(NULL);
	}
	else
	{	// mouse move out from this windows
    	// check that is it on any child

        ///////////////////////////////
		if ( !m_bMouseAbove )
		{
            // Add to enable trapping WM_KILLFOCUS
            SetFocus(m_hWnd);
			// change cursor to active cursor
			if ( m_hActiveCursor != NULL )
			{
				m_hTmpCursor = (HCURSOR)SetClassLong(m_hWnd,GCL_HCURSOR,(long)m_hActiveCursor);
				SetCursor(m_hActiveCursor);
			}
			else
				m_hTmpCursor = (HCURSOR)GetClassLong(m_hWnd,GCL_HCURSOR);
			m_bMouseAbove = TRUE;
            m_bLPressed = FALSE;
			m_bRPressed = FALSE;
			SetCapture(m_hWnd);
		}
	}
	return 0;
}

LRESULT C_GenericClickWnd::OnKillFocus(WPARAM ,LPARAM )
{
	m_bMouseAbove = FALSE;
	m_bLPressed = FALSE;

    if ( m_hActiveCursor != NULL )
    {
        SetClassLong(m_hWnd,GCL_HCURSOR,(long)m_hTmpCursor);
        SetCursor(m_hTmpCursor);
    }
    //InvalidateRect(m_hWnd,NULL,FALSE);
	return 0;
}

LRESULT C_GenericClickWnd::OnChar(WPARAM wParam,LPARAM lParam)
{
	// Notify WM_CHAR to parent
	if ( m_bKBMsgParentNotify && m_pParentWnd != NULL )
    {
        SendMessage(m_pParentWnd->GetHandle(),WM_CHAR,wParam,lParam);
    }
    return DefWindowProc(m_hWnd,WM_CHAR,wParam,lParam);
}

LRESULT C_GenericClickWnd::OnNCCalcSize(WPARAM wParam,LPARAM )
{
	if ( !(BOOL)wParam )
		return 0;
	else
		return WVR_VALIDRECTS;
}

LRESULT C_GenericClickWnd::OnEraseBkgnd(WPARAM wParam,LPARAM lParam)
{
	if ( m_bMoveBkgWindow )
    {
	    if ( !m_bInitBkg )
    	{
        	RECT rc;
            GetClientRect(m_hWnd,&rc);
            int nWidth = rc.right-rc.left;
            int nHeight = rc.bottom-rc.top;

            BitBlt(m_hdcBkg,0,0,nWidth,nHeight,m_hDC,0,0,SRCCOPY);
            m_bInitBkg = TRUE;
        }
        return 1;//DefWindowProc(m_hWnd,WM_ERASEBKGND,wParam,lParam);
    }
    return DefWindowProc(m_hWnd,WM_ERASEBKGND,wParam,lParam);
}
