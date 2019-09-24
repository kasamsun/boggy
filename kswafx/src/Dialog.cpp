#include <Dialog.h>

C_Dialog::C_Dialog()
{
}


C_Dialog::~C_Dialog()
{
}


BOOL C_Dialog::Create( UINT nIDTemplate, C_Wnd* pParentWnd)
{
	m_nDialogID = nIDTemplate;
	m_pParentWnd = pParentWnd;
	return TRUE;
}

int C_Dialog::DoModal()
{
	return DialogBoxParam(C_App::m_hInstance,
						MAKEINTRESOURCE(m_nDialogID),
						m_pParentWnd->GetHandle(),
						(DLGPROC)::WndProc,
						(LPARAM)this);
}

LRESULT C_Dialog::WndProc( UINT iMessage, WPARAM wParam, LPARAM )
{
	switch ( iMessage )
    {
	case WM_COMMAND:
		// default command
    	switch ( wParam )
        {
        case IDOK:
        case IDCANCEL:
        	EndDialog(m_hWnd,0);
            return TRUE;
        }
		break;
	}
	return FALSE;
}

