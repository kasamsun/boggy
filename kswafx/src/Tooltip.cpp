#include <Tooltip.h>

C_Tooltip::C_Tooltip() : C_Control()
{
}

C_Tooltip::~C_Tooltip()
{
}

BOOL C_Tooltip::Create(C_Wnd * pParentWnd)
{
    InitCommonControls();

	m_hWnd = CreateWindow(
    				TOOLTIPS_CLASS,
	    			(LPSTR) NULL,
		    		TTS_ALWAYSTIP,//|TTS_NOPREFIX
			    	CW_USEDEFAULT,CW_USEDEFAULT,
				    CW_USEDEFAULT,CW_USEDEFAULT,
    				pParentWnd->GetHandle(),
	    			(HMENU)NULL,
		    		C_App::m_hInstance,
			    	NULL);

	if (m_hWnd == NULL)
		return FALSE;
    m_pParentWnd = pParentWnd;
    return TRUE;
}

BOOL C_Tooltip::AddTool(HWND hWnd,LPTSTR lpszTipText)
{
    TOOLINFO ti;
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_IDISHWND|TTF_SUBCLASS;
	ti.hwnd = hWnd;
	ti.uId = (UINT)hWnd;
	ti.hinst = C_App::m_hInstance;
	ti.lpszText = lpszTipText;

	return SendMessage(m_hWnd,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti);
}

void C_Tooltip::DelTool(HWND hWnd)
{
    TOOLINFO ti;
	ti.cbSize = sizeof(TOOLINFO);
	ti.hwnd = hWnd;
	ti.uId = (UINT)hWnd;

	SendMessage(m_hWnd,TTM_DELTOOL,0,0);
}

// TTDT_AUTOMATIC
// TTDT_AUTOPOP	Sets the length of time before the tooltip window is hidden if the cursor remains stationary in the tool's bounding rectangle after the tooltip window has appeared.
// TTDT_INITIAL	Sets the length of time that the cursor must remain stationary within the bounding rectangle of a tool before the tooltip window is displayed.
// TTDT_RESHOW	Sets the length of the delay before subsequent tooltip windows are displayed when the cursor is moved from one tool to another.
void C_Tooltip::SetDelayTime(UINT uFlag,int nDelay)
{
    SendMessage(m_hWnd,TTM_SETDELAYTIME,(WPARAM)uFlag,(LPARAM)nDelay);
}

void C_Tooltip::UpdateTipText(HWND hWnd,LPTSTR lpszTipText)
{
    TOOLINFO ti;
	ti.cbSize = sizeof(TOOLINFO);
	ti.hwnd = hWnd;
	ti.uId = (UINT)hWnd;
	ti.hinst = C_App::m_hInstance;
	ti.lpszText = lpszTipText;

	SendMessage(m_hWnd,TTM_UPDATETIPTEXT,0,(LPARAM)(LPTOOLINFO)&ti);
}


