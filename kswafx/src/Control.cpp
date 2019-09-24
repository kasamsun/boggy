#include <Control.h>

C_Control::C_Control()
{
	m_hWnd = NULL;
	m_pParentWnd = NULL;
    m_lpfnDefWndProc = NULL;
}

C_Control::~C_Control()
{
	if ( IsWindow(m_hWnd) )
		DestroyWindow(m_hWnd);
}

//SetFontToParentFont()
/*		WPARAM wp = (WPARAM) SendMessage(m_pParentWnd->GetHandle(),WM_GETFONT,0,0);
		LPARAM lp = MAKELPARAM(TRUE, 0);
		SendMessage(m_hWnd,WM_SETFONT,wp,lp);
*/
HFONT C_Control::GetFont()
{
	return (HFONT)SendMessage(m_hWnd,WM_GETFONT,0,0);
}

void C_Control::SetFont(HFONT hFont,BOOL bRedraw)
{
	SendMessage(m_hWnd,WM_SETFONT,(WPARAM)hFont,MAKELPARAM(bRedraw,0));
}

int C_Control::SetText(LPCTSTR lpsz)
{
	int nRetVal = SendMessage(m_hWnd,WM_SETTEXT,0,(LPARAM)lpsz);
	//if ( nRetVal )//&& m_lpfnDefWndProc != NULL )
		InvalidateParentRect(FALSE);
		InvalidateRect(m_hWnd,NULL,FALSE);

	return nRetVal;
}

BOOL C_Control::InvalidateParentRect(BOOL bErase)
{
    RECT rMyRect;
    GetClientRect(m_hWnd,&rMyRect);
    ClientToScreen(m_hWnd,(LPPOINT)&rMyRect.left);
    ClientToScreen(m_hWnd,(LPPOINT)&rMyRect.right);
    ScreenToClient(GetParent(m_hWnd),(LPPOINT)&rMyRect.left);
    ScreenToClient(GetParent(m_hWnd),(LPPOINT)&rMyRect.right);
    return InvalidateRect(GetParent(m_hWnd),&rMyRect,bErase);
}

int C_Control::SubclassControl(HWND hWnd,C_Wnd * pParentWnd)
{
	// prevent repeat subclass
    char szClassName[64];
    if ( GetClassName(hWnd,szClassName,sizeof(szClassName)) == 0 )
        return FALSE;

    if ( GetProp(hWnd,szClassName) == NULL )
    {
        m_lpfnDefWndProc = (WNDPROC)SetWindowLong(hWnd,
                       GWL_WNDPROC, (LONG)::WndProc);
        SetWindowLong(hWnd,GWL_USERDATA,(LONG)this);
        SetProp(hWnd, szClassName, (HANDLE)m_lpfnDefWndProc);
		m_pParentWnd = (C_Wnd*)GetWindowLong(GetParent(hWnd),GWL_USERDATA);//pParentWnd;
		// if dialog created by system ( message box )
		// , set parent directly
		if ( m_pParentWnd == 0 ) m_pParentWnd = pParentWnd;
        m_hWnd = hWnd;
	    return TRUE;
    }
	else
		return FALSE;
}

// =======================================================
//  Remove properties here , for subclassed control only
//  WM_NCDESTROY -> last chance to remove it
// =======================================================
LRESULT C_Control::CleanupSubclass(UINT iMessage,WPARAM wParam,LPARAM lParam)
{
    char szClassName[64];
    if ( GetClassName(m_hWnd,szClassName,sizeof(szClassName)) == 0 )
        return FALSE;
	RemoveProp(m_hWnd, szClassName);

	return CallWindowProc(m_lpfnDefWndProc,m_hWnd,iMessage,wParam,lParam);
}

// ==============================================================
// derived class should always forward WM_NCDESTROY to C_Control
// ==============================================================
LRESULT C_Control::WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam )
{
    if ( m_lpfnDefWndProc == NULL )
    {
        return DefWindowProc(m_hWnd,iMessage,wParam,lParam);
    }
    else
    {
        switch ( iMessage )
        {
        case WM_NCDESTROY:
            return CleanupSubclass(iMessage,wParam,lParam);
        default:
            return CallWindowProc(m_lpfnDefWndProc,m_hWnd,iMessage,wParam,lParam);
        }
    }
}
