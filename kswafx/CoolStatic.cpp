#include <CoolStatic.h>

BEGIN_HANDLE_MSG(C_CoolStatic,C_Static)
    HANDLE_MSG(WM_PAINT,OnPaint)
END_HANDLE_MSG()

C_CoolStatic::C_CoolStatic() : C_Static()
{
}

C_CoolStatic::~C_CoolStatic()
{
}

BOOL C_CoolStatic::Create(LPCTSTR lpszCaption,DWORD dwExStyle,DWORD dwStyle,
						const RECT& rect,C_Wnd* pParentWnd,
						UINT nID)
{
	int nRetVal = C_Static::Create(lpszCaption,dwExStyle,dwStyle,rect,pParentWnd,nID);

    if ( nRetVal == FALSE )
        return FALSE;
    else
    	return C_CoolStatic::SubclassControl(m_hWnd,pParentWnd);
}

int C_CoolStatic::SubclassControl(HWND hWnd,C_Wnd *pParentWnd)
{
	int nRetVal = C_Control::SubclassControl(hWnd,pParentWnd);
	if ( nRetVal )
	{
		LONG lStyle = GetWindowLong(hWnd,GWL_STYLE);
		lStyle |= WS_CLIPSIBLINGS;
		SetWindowLong(hWnd,GWL_STYLE,lStyle);
	}
	return nRetVal;
}

int C_CoolStatic::PaintText(HDC hDC)
{
	LONG lCtlStyle = GetWindowLong(m_hWnd,GWL_STYLE);
	RECT rc;

	GetClientRect(m_hWnd,&rc);
    SetBkMode(m_hDC,TRANSPARENT);

	HFONT hTmpFont = (HFONT)SelectObject(hDC,GetFont());
	UINT dwFormat = 0;
	if ( lCtlStyle & SS_CENTERIMAGE )
		dwFormat |= DT_VCENTER|DT_SINGLELINE;
	else
		dwFormat |= DT_WORDBREAK;
	switch ( lCtlStyle & 0x0f )
	{
	case SS_LEFT:
		dwFormat |= DT_LEFT;
		break;
	case SS_CENTER:
		dwFormat |= DT_CENTER;
		break;
	case SS_RIGHT:
		dwFormat |= DT_RIGHT;
		break;
	case SS_SIMPLE:
		dwFormat |= DT_LEFT|DT_SINGLELINE;
		break;
	case SS_LEFTNOWORDWRAP:
		dwFormat |= DT_LEFT|DT_SINGLELINE;
		break;
	default:
		dwFormat |= DT_LEFT|DT_SINGLELINE;
		break;
	}

	int nBufLen = GetWindowTextLength(m_hWnd);
	char * pszBuf = new char[nBufLen+1];
	GetWindowText(m_hWnd,pszBuf,nBufLen+1);
	DrawTextEx( hDC,pszBuf,nBufLen,&rc,dwFormat,NULL);
	delete[] pszBuf;

	SelectObject(hDC,hTmpFont);
	return TRUE;
}

int C_CoolStatic::PaintIcon(HDC hDC)
{
	DrawIcon(hDC,0,0,(HICON)SendMessage(m_hWnd,STM_GETIMAGE,IMAGE_ICON,0));
	return TRUE;
}

int C_CoolStatic::PaintBitmap(HDC hDC)
{
    HDC hdcTmp = CreateCompatibleDC(hDC);
    HBITMAP hbmpPic = (HBITMAP)SendMessage(m_hWnd,STM_GETIMAGE,IMAGE_BITMAP,0);
	HBITMAP hbmpTmp = (HBITMAP)SelectObject(hdcTmp,hbmpPic);

    BITMAP bm;
    GetObject(hbmpPic,sizeof(bm),&bm);

	BitBlt(hDC,0,0,bm.bmWidth,bm.bmHeight,hdcTmp,0,0,SRCCOPY);

    SelectObject(hdcTmp,hbmpTmp);
    DeleteDC(hdcTmp);
	return TRUE;
}

LRESULT C_CoolStatic::OnPaint(WPARAM ,LPARAM )
{
    PAINTSTRUCT ps;
    m_hDC = BeginPaint( m_hWnd, &ps );

	LONG lCtlStyle = GetWindowLong(m_hWnd,GWL_STYLE);

	if ( (lCtlStyle&0x0000000f) == SS_ICON )
		PaintIcon(m_hDC);
	else if ( (lCtlStyle&0x0000000f) == SS_BITMAP )
    	PaintBitmap(m_hDC);
    else
		PaintText(m_hDC);

    EndPaint(m_hWnd,&ps);
	return 0;
}