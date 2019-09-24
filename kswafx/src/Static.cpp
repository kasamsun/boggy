#include <Static.h>

C_Static::C_Static() : C_Control()
{
}

C_Static::~C_Static()
{
}

BOOL C_Static::Create(LPCTSTR lpszCaption,DWORD dwExStyle,DWORD dwStyle,
						const RECT& rect,C_Wnd* pParentWnd,
						UINT nID)
{
    dwStyle |= WS_CHILD|WS_CLIPSIBLINGS;
	m_hWnd = CreateWindowEx(dwExStyle,"static",lpszCaption,dwStyle,
							rect.left,rect.top,
							rect.right-rect.left,
							rect.bottom-rect.top,
							pParentWnd->GetHandle(),
							(HMENU)nID,
							C_App::m_hInstance,
							NULL);

	if ( m_hWnd == NULL ) return FALSE;
	m_pParentWnd = pParentWnd;
	return TRUE;
}

LRESULT C_Static::WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam )
{
    return C_Control::WndProc(iMessage,wParam,lParam);
}
