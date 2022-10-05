#include <Edit.h>


C_Edit::C_Edit() : C_Control()
{
}

C_Edit::~C_Edit()
{
}

BOOL C_Edit::Create(LPCTSTR lpszCaption,DWORD dwExStyle,DWORD dwStyle,
						const RECT& rect,C_Wnd* pParentWnd,
						UINT nID)
{
    // checking for style, must be WS_CHILD
    dwStyle |= WS_CHILD|WS_CLIPSIBLINGS;
    //
	m_hWnd = CreateWindowEx(dwExStyle,"edit",lpszCaption,dwStyle,
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

int C_Edit::GetLimitText()
{
	return SendMessage(m_hWnd,EM_GETLIMITTEXT,0,0);
}

void C_Edit::SetLimitText(short cbMax)
{
	SendMessage(m_hWnd,EM_SETLIMITTEXT,(WPARAM)cbMax,0);
}

LRESULT C_Edit::WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam )
{
    return C_Control::WndProc(iMessage,wParam,lParam);
}