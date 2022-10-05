#include "GenericWnd.h"

char C_GenericWnd::m_szClassName[] = "";

void C_GenericWnd::Register( UINT style,
							HICON hIcon,
							HCURSOR hCursor,
							HBRUSH hBkgBrush,
							LPCTSTR lpszMenuName,
							LPCTSTR lpszClassName )
{
	WNDCLASS wndclass;
	wndclass.style         = style;
	wndclass.lpfnWndProc   = ::WndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = sizeof(C_GenericWnd *);
	wndclass.hInstance     = C_App::m_hInstance;
	wndclass.hIcon         = hIcon;
	wndclass.hCursor       = hCursor;
	wndclass.hbrBackground = hBkgBrush;
	wndclass.lpszMenuName  = lpszMenuName;
	wndclass.lpszClassName = lpszClassName;

	strcpy(m_szClassName,lpszClassName);

	 if ( ! RegisterClass( &wndclass ) )
		  exit( FALSE );
}

C_GenericWnd::C_GenericWnd(C_Wnd * pParentWnd)
{
	m_pParentWnd = pParentWnd;
}

C_GenericWnd::~C_GenericWnd( void )
{
}

LRESULT C_GenericWnd::WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	return DefWindowProc(m_hWnd,iMessage,wParam,lParam);
}

