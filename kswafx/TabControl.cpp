#include <windows.h>
#include <commctrl.h>

#include <TabControl.h>


C_TabControl::C_TabControl() : C_Control()
{
}

C_TabControl::~C_TabControl()
{
}

BOOL C_TabControl::Create(const RECT& rect,C_Wnd* pParentWnd)
{
    InitCommonControls();

    m_hWnd = CreateWindow(WC_TABCONTROL,
                            "",
                            WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
                            rect.left,rect.top,
							rect.right-rect.left,
							rect.bottom-rect.top,
                            pParentWnd->GetHandle(),
                            NULL,
                            C_App::m_hInstance,
                            NULL );

	if ( m_hWnd == NULL ) return FALSE;
	m_pParentWnd = pParentWnd;

	m_hFont = (HFONT)SendMessage(m_hWnd,WM_GETFONT,0,0);
	return TRUE;
}

int C_TabControl::InsertItem(int iItem,UINT mask,LPSTR pszText,int cchTextMax)
{
    TC_ITEM tci;

    tci.mask = mask;
    tci.pszText = pszText;
    tci.cchTextMax = cchTextMax;

    WPARAM wParam = (WPARAM) (int) iItem;
    LPARAM lParam = (LPARAM) (const TC_ITEM *) &tci;

    return SendMessage(m_hWnd,TCM_INSERTITEM,wParam,lParam);
// implement later
//    int iImage;        // index to tab control's image
//    LPARAM lParam;     // application-defined data associated with tab
}

LRESULT C_TabControl::WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam )
{
    return C_Control::WndProc(iMessage,wParam,lParam);
}