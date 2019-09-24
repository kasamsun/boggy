#include <Button.h>


C_Button::C_Button() : C_Control()
{
}

C_Button::~C_Button()
{
}

HANDLE C_Button::GetImage(WORD fImageType)
{
	return (HANDLE)SendMessage(m_hWnd,BM_GETIMAGE,(WPARAM)fImageType,0L);
}

// fImageType : IMAGE_BITMAP , IMAGE_ICON
HANDLE C_Button::SetImage(WORD fImageType,HANDLE hImage)
{
	return (HANDLE)SendMessage(m_hWnd,BM_SETIMAGE,(WPARAM)fImageType,(LPARAM)hImage);
}

void C_Button::SetCheck(WORD fCheck)
{
	SendMessage(m_hWnd,BM_SETCHECK,fCheck,0);
}

// return	BST_CHECKED = Button is checked. 
//			BST_INDETERMINATE = Button is grayed, indicating an indeterminate state (applies only if the button has the BS_3STATE or BS_AUTO3STATE style). 
//			BST_UNCHECKED = Button is unchecked 
LRESULT C_Button::GetCheck()
{
	return SendMessage(m_hWnd,BM_GETCHECK,0,0);
}

BOOL C_Button::Create(LPCTSTR lpszCaption,DWORD dwExStyle,DWORD dwStyle,
						const RECT& rect,C_Wnd* pParentWnd,
						UINT nID)
{
    // checking for style, must be WS_CHILD
    dwStyle |= WS_CHILD|WS_CLIPSIBLINGS;
    //
	m_hWnd = CreateWindowEx(dwExStyle,"button",lpszCaption,dwStyle,
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

LRESULT C_Button::WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam )
{
    return C_Control::WndProc(iMessage,wParam,lParam);
}
