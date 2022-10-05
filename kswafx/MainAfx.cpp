#include "MainAfx.h"
#include <stdio.h>

#ifdef _DEBUG
// ======================= TRACE function ================
void _Trace(LPCTSTR lpszFormat, ...)
{
	va_list Args;
	va_start(Args,lpszFormat);

	char szMsgBuf[MAX_TRACE_BUFFER];
	vsprintf(szMsgBuf,lpszFormat,Args);
	va_end(Args);
	OutputDebugString(szMsgBuf);
}
#endif

HINSTANCE C_App::m_hInstance = 0;
HINSTANCE C_App::m_hPrevInstance = 0;
LPSTR C_App::m_lpszCmdLine = NULL;
int C_App::m_nCmdShow = 0;

// ================== Main message loop ==========================
int C_App::MessageLoop( void )
{
    MSG msg;

    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
    return msg.wParam;
}

void C_App::GetExecDirectory(char *szDirPath,int size)
{
	char szFullPath[MAX_PATH];
	char szDrive[_MAX_DRIVE];
	char szDir[_MAX_DIR];

	GetModuleFileName(NULL,szFullPath,sizeof(szFullPath));
	_splitpath(szFullPath,szDrive,szDir,NULL,NULL);
	if ( (int)(strlen(szDrive) + strlen(szDir) + 1) > size )
		sprintf(szDirPath,"\0");
	else
		sprintf(szDirPath,"%s%s\0",szDrive,szDir);
}



LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
    C_Wnd *pWindow = (C_Wnd*) GetWindowLong(hWnd,GWL_USERDATA);
	// Note ::  /////////////////////////////////////////////////////////
    // How do we know in general if the pointer pWindow is invalid?
    // Simple: Windows allocates the window extra bytes using LocalAlloc
    // which zero initializes memory; thus, pWindow will have a value of
    // zero before we set the window extra bytes to the 'this' pointer.
    // Caveat emptor: the fact that LocalAlloc will zero initialize the
    // window extra bytes is not documented; therefore, it could change
    // in the future.
	//////////////////////////////////////////////////////////////////////
    if ( pWindow == 0 )
    {
        switch ( iMessage )
		{
		case WM_CREATE:
			{
            LPCREATESTRUCT lpcs;

            lpcs = (LPCREATESTRUCT) lParam;
            pWindow = (C_Wnd *) lpcs->lpCreateParams;
			pWindow->SetHandle(hWnd);
            SetWindowLong(hWnd,GWL_USERDATA,(LONG)pWindow);
			return pWindow->WndProc( iMessage, wParam, lParam );
			}
		case WM_INITDIALOG:
			{
			pWindow = (C_Wnd*) lParam;
			pWindow->SetHandle(hWnd);
			SetWindowLong(hWnd,GWL_USERDATA,(LONG)pWindow);
			return pWindow->WndProc(iMessage,wParam,lParam);
			}
		default:
			return DefWindowProc( hWnd, iMessage, wParam, lParam );
		}
    }
    else
		return pWindow->WndProc( iMessage, wParam, lParam );
}
