#include <CoolButton.h>

BEGIN_HANDLE_MSG(C_CoolButton,C_Button)
    HANDLE_MSG(WM_MOUSEMOVE,OnMouseMove)
    HANDLE_MSG(WM_PAINT,OnPaint)
    HANDLE_MSG(WM_ERASEBKGND,OnEraseBkgnd)
    HANDLE_MSG(WM_SETFOCUS,OnSetFocus)
    HANDLE_MSG(WM_KILLFOCUS,OnKillFocus)

    HANDLE_MSG(WM_LBUTTONDOWN,OnLButtonDown)
    HANDLE_MSG(WM_LBUTTONUP,OnLButtonUp)
    HANDLE_MSG(WM_LBUTTONDBLCLK,OnLButtonDblClk)
    HANDLE_MSG(WM_KEYDOWN,OnKeyDown)
    HANDLE_MSG(WM_KEYUP,OnKeyUp)
    HANDLE_MSG(WM_CHAR,OnChar)
	HANDLE_MSG(WM_ENABLE,OnEnable)

    HANDLE_MSG(BM_SETSTATE,OnBMSetState)
    HANDLE_MSG(BM_SETCHECK,OnBMSetCheck)

END_HANDLE_MSG()


C_CoolButton::C_CoolButton() : C_Button()
{
	m_nMouseStatus = 0;
    m_bKBMsgParentNotify = FALSE;
	m_bFocusOnMouseMove = FALSE;
}

C_CoolButton::~C_CoolButton()
{
}

BOOL C_CoolButton::Create(LPCTSTR lpszCaption,DWORD dwExStyle,DWORD dwStyle,
						const RECT& rect,C_Wnd* pParentWnd,
						UINT nID)
{
	int nRetVal = C_Button::Create(lpszCaption,dwExStyle,dwStyle,rect,pParentWnd,nID);

    if ( nRetVal == FALSE )
        return FALSE;
    else
    	return C_CoolButton::SubclassControl(m_hWnd,pParentWnd);
}

int C_CoolButton::SubclassControl(HWND hWnd,C_Wnd *pParentWnd)
{
	int nRetVal = C_Control::SubclassControl(hWnd,pParentWnd);
	if ( nRetVal )
	{
		// **** warning : control in group box , parent is not dialog ,
		// but group box. ok.
		// Group box should be always transparent.
		LONG lStyle;
		lStyle=GetWindowLong(hWnd,GWL_STYLE);
		if ( (lStyle&0x0000000f) == BS_GROUPBOX )
			SetWindowLong(hWnd,GWL_EXSTYLE,
						GetWindowLong(hWnd,GWL_EXSTYLE)|WS_EX_TRANSPARENT);
	}
	return nRetVal;
}

void C_CoolButton::EnableKBMsgParentNotify(int bFlag)
{
	m_bKBMsgParentNotify = bFlag;
}

void C_CoolButton::EnableFocusOnMouseMove(int bFlag)
{
	m_bFocusOnMouseMove = bFlag;
}

// ===============================================================
// button control with group box style have own method to get style
// ===============================================================
UINT C_CoolButton::GetFormatFromStyle(long lCtlStyle)
{
   UINT dwFormat = DT_SINGLELINE;
//DT_TOP              0x00000000   BS_LEFT             0x00000100L
//DT_LEFT             0x00000000   BS_RIGHT            0x00000200L
//DT_CENTER           0x00000001   BS_CENTER           0x00000300L
//DT_RIGHT            0x00000002   BS_TOP              0x00000400L
//DT_VCENTER          0x00000004   BS_BOTTOM           0x00000800L
//DT_BOTTOM           0x00000008   BS_VCENTER          0x00000C00L
//DT_SINGLELINE       0x00000020

	// horizontal ===========================
    if ( (lCtlStyle & BS_LEFT) && (lCtlStyle & BS_RIGHT) )
        dwFormat |= DT_CENTER;
    else if ( lCtlStyle & BS_RIGHT )
        dwFormat |= DT_RIGHT;
    else if ( lCtlStyle & BS_LEFT )
    	dwFormat |= DT_LEFT;
    else
   		dwFormat |= DT_CENTER;

	// vertical ===========================
    if ( (lCtlStyle & BS_TOP) && (lCtlStyle & BS_BOTTOM) )
        dwFormat |= DT_VCENTER;
    else if ( lCtlStyle & BS_BOTTOM )
        dwFormat |= DT_BOTTOM;
    else if ( lCtlStyle & BS_TOP )
        dwFormat |= DT_TOP;
    else
        dwFormat |= DT_VCENTER;

	return dwFormat;
}

int C_CoolButton::PaintText(HDC hDC)
{
    RECT rc;
    GetClientRect(m_hWnd,&rc);

    if ( m_nMouseStatus & CURSOR_LPRESSED )
    {
        DrawEdge( hDC,&rc,BDR_SUNKENOUTER,BF_RECT);
        rc.left+=1;
        rc.top+=1;
    }
    else if ( m_nMouseStatus & CURSOR_INSIDE )
        DrawEdge( hDC,&rc,BDR_RAISEDINNER,BF_RECT);

    int nBufLen = GetWindowTextLength(m_hWnd);
	if ( nBufLen > 0 )
	{
	    HFONT hTmpFont = (HFONT)SelectObject(hDC,GetFont());
		char * pszBuf = new char[nBufLen+1];
		GetWindowText(m_hWnd,pszBuf,nBufLen+1);
		DrawTextEx( hDC,pszBuf,nBufLen,&rc,
				GetFormatFromStyle(GetWindowLong(m_hWnd,GWL_STYLE)),
				NULL);
		delete[] pszBuf;

		SelectObject(hDC,hTmpFont);
	}
	return 0;
}

int C_CoolButton::PaintIcon(HDC hDC)
{
    RECT rc;
    GetClientRect(m_hWnd,&rc);
    if ( m_nMouseStatus & CURSOR_LPRESSED )
    {
        DrawEdge( hDC,&rc,BDR_SUNKENOUTER,BF_RECT);
    	rc.left+=1;
        rc.top+=1;
    }
    else if ( m_nMouseStatus & CURSOR_INSIDE )
        DrawEdge( hDC,&rc,BDR_RAISEDINNER,BF_RECT);

	DrawIcon(hDC,rc.left,rc.top,(HICON)SendMessage(m_hWnd,BM_GETIMAGE,IMAGE_ICON,0));
	return 0;
}

int C_CoolButton::PaintBitmap(HDC hDC)
{
    HDC hdcTmp = CreateCompatibleDC(hDC);
    HBITMAP hbmpPic = (HBITMAP)SendMessage(m_hWnd,BM_GETIMAGE,IMAGE_BITMAP,0);
	HBITMAP hbmpTmp = (HBITMAP)SelectObject(hdcTmp,hbmpPic);

    BITMAP bm;
    GetObject(hbmpPic,sizeof(bm),&bm);

    RECT rc;
    GetClientRect(m_hWnd,&rc);
    if ( m_nMouseStatus & CURSOR_LPRESSED )
    {
    	rc.left+=1;
        rc.top+=1;
    }

	BitBlt(hDC,rc.left,rc.top,bm.bmWidth,bm.bmHeight,hdcTmp,0,0,SRCCOPY);

    if ( m_nMouseStatus & CURSOR_LPRESSED )
        DrawEdge( hDC,&rc,BDR_SUNKENOUTER,BF_RECT);
    else if ( m_nMouseStatus & CURSOR_INSIDE )
        DrawEdge( hDC,&rc,BDR_RAISEDINNER,BF_RECT);

    SelectObject(hdcTmp,hbmpTmp);
    DeleteDC(hdcTmp);
    return 0;
}

int C_CoolButton::PaintCheck(HDC hDC)
{
	RECT rc;
    GetClientRect(m_hWnd,&rc);
	if ( m_nMouseStatus & CURSOR_LPRESSED )
		DrawEdge( hDC,&rc,BDR_SUNKENOUTER,BF_RECT);
	else if ( m_nMouseStatus & CURSOR_INSIDE )
		DrawEdge( hDC,&rc,BDR_RAISEDINNER,BF_RECT);

	rc.left+=18;
    int nBufLen = GetWindowTextLength(m_hWnd);
	if ( nBufLen > 0 )
	{
		HFONT hTmpFont = (HFONT)SelectObject(hDC,GetFont());
	    char * pszBuf = new char[nBufLen+1];
		GetWindowText(m_hWnd,pszBuf,nBufLen+1);
		DWORD dwFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	    DrawTextEx( hDC,pszBuf,nBufLen,&rc,dwFormat,NULL);
		delete[] pszBuf;
		SelectObject(hDC,hTmpFont);
	}

    GetClientRect(m_hWnd,&rc);
	rc.top = (rc.bottom-rc.top) /2 - 6;
	rc.bottom = rc.top+13;
	rc.left = 2;
	rc.right = rc.left+13;
	switch(SendMessage(m_hWnd,BM_GETCHECK,0,0))
    {
    case BST_CHECKED://	Button is checked.
		DrawFrameControl(hDC,&rc,DFC_BUTTON,DFCS_CHECKED);
    	break;
	case BST_INDETERMINATE://	Button is grayed, indicating an indeterminate state (applies only if the button has the BS_3STATE or BS_AUTO3STATE style).
		DrawFrameControl(hDC,&rc,DFC_BUTTON,DFCS_CHECKED);
    	break;
    case BST_UNCHECKED: //	Button is unchecked
		DrawFrameControl(hDC,&rc,DFC_BUTTON,DFCS_BUTTONCHECK);
		break;
    }

	return 0;
}

int C_CoolButton::PaintRadio(HDC hDC)
{
	RECT rc;
    GetClientRect(m_hWnd,&rc);
	if ( m_nMouseStatus & CURSOR_LPRESSED )
		DrawEdge( hDC,&rc,BDR_SUNKENOUTER,BF_RECT);
	else if ( m_nMouseStatus & CURSOR_INSIDE )
		DrawEdge( hDC,&rc,BDR_RAISEDINNER,BF_RECT);

	rc.left+=18;
    int nBufLen = GetWindowTextLength(m_hWnd);
	if ( nBufLen > 0 )
	{
		HFONT hTmpFont = (HFONT)SelectObject(hDC,GetFont());
	    char * pszBuf = new char[nBufLen+1];
		GetWindowText(m_hWnd,pszBuf,nBufLen+1);
		DWORD dwFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	    DrawTextEx( hDC,pszBuf,nBufLen,&rc,dwFormat,NULL);
		delete[] pszBuf;
		SelectObject(hDC,hTmpFont);
	}

    GetClientRect(m_hWnd,&rc);
	rc.top = (rc.bottom-rc.top) /2 - 6;
	rc.bottom = rc.top+14;
	rc.left = 2;
	rc.right = rc.left+14;
	switch(SendMessage(m_hWnd,BM_GETCHECK,0,0))
    {
    case BST_CHECKED://	Button is checked.
		DrawFrameControl(hDC,&rc,DFC_BUTTON,DFCS_CHECKED|DFCS_BUTTONRADIO);
    	break;
	case BST_INDETERMINATE://	Button is grayed, indicating an indeterminate state (applies only if the button has the BS_3STATE or BS_AUTO3STATE style).
		DrawFrameControl(hDC,&rc,DFC_BUTTON,DFCS_BUTTONRADIO);
    	break;
    case BST_UNCHECKED: //	Button is unchecked
		DrawFrameControl(hDC,&rc,DFC_BUTTON,DFCS_BUTTONRADIO);
		break;
    }

	return 0;
}

int C_CoolButton::PaintGroupBox(HDC hDC)
{
    RECT rc;
    GetClientRect(m_hWnd,&rc);

    HFONT hTmpFont = (HFONT)SelectObject(hDC,GetFont());
	TEXTMETRIC tm;
	GetTextMetrics(hDC,&tm);

	rc.top+=(tm.tmHeight+tm.tmExternalLeading)/2;

	int nBufLen = GetWindowTextLength(m_hWnd);
    if ( nBufLen > 0 )
    {
        char * pszBuf = new char[nBufLen+1];
        GetWindowText(m_hWnd,pszBuf,nBufLen+1);

        // find format
        int nLeft,nRight;
       	SIZE size;

   		GetTextExtentPoint32(hDC,pszBuf,nBufLen,&size);

        LONG lCtlStyle = GetWindowLong(m_hWnd,GWL_STYLE);
        if ( (lCtlStyle & BS_LEFT) && (lCtlStyle & BS_RIGHT) )
        {
        	// center
            nLeft = (rc.right - size.cx) /2;
            nRight = nLeft + size.cx;
        }
        else if ( lCtlStyle & BS_RIGHT )
        {
        	// right
            nRight = rc.right-10 ; nLeft = nRight-size.cx;
        }
        else if ( lCtlStyle & BS_LEFT )
        {
        	// left
        	nLeft = 10; nRight = size.cx+10;
        }
        else
        {
        	// default left
        	nLeft = 10; nRight = size.cx+10;
        }
		TextOut( hDC,nLeft,0,pszBuf,nBufLen);
        delete[] pszBuf;

        // paint edge on clip rgn only ( not over the text )
        // left/right margin is 10
        HRGN hrgn;
        hrgn = CreateRectRgn(nLeft,0,nRight,size.cy);
        ExtSelectClipRgn( hDC,hrgn,RGN_DIFF );
		DrawEdge( hDC,&rc,EDGE_ETCHED,BF_RECT);
        SelectClipRgn(hDC,NULL);
        DeleteObject(hrgn);
	}
	else
	{
		DrawEdge( hDC,&rc,EDGE_ETCHED,BF_RECT);
	}
	SelectObject(hDC,hTmpFont);
	return 0;
}

// ====================== Event Handler ======================
//====================================================
// Limitation : rect of parent should not be change
//              because i use InvalidateRect(FALSE)
//              if i use TRUE, with long OnPaint duration of parent,
//              it 'll blink blink.
//=====================================================
LRESULT C_CoolButton::OnPaint(WPARAM ,LPARAM )
{
    PAINTSTRUCT ps;
	m_hDC = BeginPaint( m_hWnd, &ps );
	SetBkMode(m_hDC,TRANSPARENT);

    LONG lCtlStyle = GetWindowLong(m_hWnd,GWL_STYLE);
//                                          (WINVER >= 0x0400)
//BS_PUSHBUTTON       0x00000000L   //BS_TEXT             0x00000000L
//BS_DEFPUSHBUTTON    0x00000001L	//BS_ICON             0x00000040L
//BS_CHECKBOX         0x00000002L   //BS_BITMAP           0x00000080L
//BS_AUTOCHECKBOX     0x00000003L   //BS_LEFT             0x00000100L
//BS_RADIOBUTTON      0x00000004L   //BS_RIGHT            0x00000200L
//BS_3STATE           0x00000005L   //BS_CENTER           0x00000300L
//BS_AUTO3STATE       0x00000006L   //BS_TOP              0x00000400L
//BS_GROUPBOX         0x00000007L   //BS_BOTTOM           0x00000800L
//BS_USERBUTTON       0x00000008L   //BS_VCENTER          0x00000C00L
//BS_AUTORADIOBUTTON  0x00000009L   //BS_PUSHLIKE         0x00001000L
//BS_OWNERDRAW        0x0000000BL   //BS_MULTILINE        0x00002000L
//BS_LEFTTEXT         0x00000020L   //BS_NOTIFY           0x00004000L
					                //BS_FLAT             0x00008000L
                                    //BS_RIGHTBUTTON      BS_LEFTTEXT
	switch ( lCtlStyle&0x0000000f )
    {
    case BS_GROUPBOX:
		PaintGroupBox(m_hDC); break;
    case BS_3STATE: case BS_AUTO3STATE: case BS_AUTOCHECKBOX: case BS_CHECKBOX:
    	PaintCheck(m_hDC); break;
    case BS_AUTORADIOBUTTON:
	case BS_RADIOBUTTON:
    	PaintRadio(m_hDC); break;
    default:
        if ( lCtlStyle & BS_ICON )
        	PaintIcon(m_hDC);
        else if ( lCtlStyle & BS_BITMAP )
        	PaintBitmap(m_hDC);
        else
			PaintText(m_hDC);
        break;
	}
    EndPaint(m_hWnd,&ps);
    return 0;
}

LRESULT C_CoolButton::OnEraseBkgnd(WPARAM ,LPARAM )
{
    // prevent original button's wndproc painting
    return 1;
}

LRESULT C_CoolButton::OnMouseMove(WPARAM ,LPARAM lParam)
{
	POINT point = {(int)LOWORD(lParam),(int)HIWORD(lParam)};

	HWND hWndFocus,hWndChildFocus;
	hWndChildFocus = ChildWindowFromPoint(m_hWnd,point);

	ClientToScreen(m_hWnd,&point);
	ScreenToClient(GetParentWnd()->GetHandle(),&point);
	hWndFocus = ChildWindowFromPoint(GetParentWnd()->GetHandle(),point);

	if ( hWndFocus != m_hWnd || hWndChildFocus != m_hWnd )
	{// cursor is out side
		m_nMouseStatus &= ~(CURSOR_INSIDE|CURSOR_LPRESSED); // remove flag
		ReleaseCapture();
	    InvalidateParentRect(FALSE);
        //InvalidateRect(m_hWnd,NULL,FALSE);

        // i can't remember why setfocus. ???  it cause blink when control is in groupbox
		//SetFocus(hWndFocus);
	}
	else
	{// cursor is in side
		m_nMouseStatus |= CURSOR_INSIDE;    // set flag
        if ( GetCapture() != m_hWnd )
        {
    		SetCapture(m_hWnd);
			// temporarily change to owner draw prevent focus drawing
			// by button class
			if ( m_bFocusOnMouseMove )
			{
				LONG lTmpStyle = SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE)|BS_OWNERDRAW);
				// setfocus is to enable this control can trap wm_killfocus
				SetFocus(m_hWnd);
				SetWindowLong(m_hWnd,GWL_STYLE,lTmpStyle);
			}
// *** no need set focus , if it's not use custom cursor like genericclickwnd ***
//			LONG lTmpStyle = SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE)|BS_OWNERDRAW);
            // setfocus is to enable this control can trap wm_killfocus
//			SetFocus(m_hWnd);
//			SetWindowLong(m_hWnd,GWL_STYLE,lTmpStyle);
            //InvalidateParentRect(FALSE);
   	        InvalidateRect(m_hWnd,NULL,FALSE);
        }
	}
	return 0;
}

LRESULT C_CoolButton::OnSetFocus(WPARAM ,LPARAM )
{
	//return CallWindowProc(m_lpfnDefWndProc,m_hWnd,WM_SETFOCUS,wParam,lParam);
	return 0;
}

LRESULT C_CoolButton::OnKillFocus(WPARAM ,LPARAM )
{
    m_nMouseStatus &= ~CURSOR_INSIDE;
	InvalidateParentRect(FALSE);
	//InvalidateRect(m_hWnd,NULL,FALSE);
	return 0;
}

LRESULT C_CoolButton::OnLButtonDown(WPARAM wParam,LPARAM lParam)
{
    m_nMouseStatus |= CURSOR_LPRESSED;
    InvalidateParentRect(FALSE);
   	InvalidateRect(m_hWnd,NULL,FALSE);

	return CallWindowProc(m_lpfnDefWndProc,m_hWnd,WM_LBUTTONDOWN,wParam,lParam);
}

LRESULT C_CoolButton::OnLButtonUp(WPARAM wParam,LPARAM lParam)
{
    m_nMouseStatus &= ~CURSOR_LPRESSED;
    InvalidateParentRect(FALSE);
   	InvalidateRect(m_hWnd,NULL,FALSE);

    return CallWindowProc(m_lpfnDefWndProc,m_hWnd,WM_LBUTTONUP,wParam,lParam);
}

LRESULT C_CoolButton::OnLButtonDblClk(WPARAM ,LPARAM )
{
	// ignore dblclk that cause unwanted painting
	return 0;
}

LRESULT C_CoolButton::OnKeyDown(WPARAM ,LPARAM )
{
	// CoolButton 'll not response any keyboard message
    return 0;
}

LRESULT C_CoolButton::OnKeyUp(WPARAM ,LPARAM )
{
    return 0;
}

LRESULT C_CoolButton::OnChar(WPARAM wParam,LPARAM lParam)
{
	// Notify WM_CHAR to parent
	if ( m_bKBMsgParentNotify && m_pParentWnd != NULL )
    {
        SendMessage(m_pParentWnd->GetHandle(),WM_CHAR,wParam,lParam);
    }
    return 0;
}

LRESULT C_CoolButton::OnEnable(WPARAM ,LPARAM )
{
	return 0;
}

LRESULT C_CoolButton::OnBMSetState(WPARAM wParam,LPARAM lParam)
{
//	prevent drawing in BM_SETSTATE by temporarily change style to BS_OWNERDRAW
	LONG lTmpStyle = SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE)|BS_OWNERDRAW);
    LRESULT lRetVal = CallWindowProc(m_lpfnDefWndProc,m_hWnd,BM_SETSTATE,wParam,lParam);
	SetWindowLong(m_hWnd,GWL_STYLE,lTmpStyle);
	return lRetVal;
}

LRESULT C_CoolButton::OnBMSetCheck(WPARAM wParam,LPARAM lParam)
{
	LRESULT lRetVal = CallWindowProc(m_lpfnDefWndProc,m_hWnd,BM_SETCHECK,wParam,lParam);
	// checking for auto bm_setcheck sent by system
	// and overwrite auto paint to correct painting
	if ( GetFocus() != m_hWnd )
	{
		InvalidateParentRect(FALSE);
		InvalidateRect(m_hWnd,NULL,FALSE);
	}
	return lRetVal;
}

//WM_SETTEXT, WM_KILLFOCUS, WM_ENABLE, WM_SETFOCUS, WM_PAINT, BM_SETSTATE, and BM_SETCHECK
