#include "BkgWnd.h"

#include "resource.h"
#include "BoggyWnd.h"

BEGIN_HANDLE_MSG(C_BkgWnd,C_GenericWnd)
#ifdef _DEBUG
	HANDLE_MSG(WM_PAINT,OnPaint)
#endif
	HANDLE_MSG(WM_DESTROY,OnDestroy)
END_HANDLE_MSG()

C_BkgWnd::C_BkgWnd()
{
	m_hWnd = CreateWindow(
                        m_szClassName,
                        "Boggy",
                        WS_OVERLAPPEDWINDOW,
                        0,
                        0,
                        256,
                        256,
                        NULL,
                        NULL,
                        C_App::m_hInstance,
                        (LPSTR) this );

    if ( ! m_hWnd )
        exit( FALSE );

#ifdef _DEBUG
    ShowWindow(m_hWnd, C_App::m_nCmdShow ); // hide , we won't see icon
#else
    ShowWindow(m_hWnd, SW_HIDE );
#endif
    //Update();

	pBoggyWnd = new C_BoggyWnd(this);
}

C_BkgWnd::~C_BkgWnd( void )
{
	delete pBoggyWnd;
}

///////////////////// Event Handler //////////////////
#ifdef _DEBUG
LRESULT C_BkgWnd::OnPaint(WPARAM wParam,LPARAM lParam)
{
    PAINTSTRUCT ps;
    m_hDC = BeginPaint( m_hWnd, &ps );
	BitBlt(m_hDC,0,0,32*pBoggyWnd->m_nScreenSize,32*pBoggyWnd->m_nScreenSize,
			pBoggyWnd->m_hdcBkg,0,0,SRCCOPY);
	int l = 0;
	char tmpbuf[32];
	sprintf(tmpbuf,"bLPressed = %d",pBoggyWnd->m_bLPressed);
	TextOut(m_hDC,32*pBoggyWnd->m_nScreenSize,l,tmpbuf,strlen(tmpbuf));
	sprintf(tmpbuf,"CurFrame = %d / %d",pBoggyWnd->m_pCurAction->m_nCurFrame,pBoggyWnd->m_pCurAction->m_nTotalFrame);
	TextOut(m_hDC,32*pBoggyWnd->m_nScreenSize,l+=15,tmpbuf,strlen(tmpbuf));
	sprintf(tmpbuf,"Delay = %d / %d",pBoggyWnd->m_pCurAction->m_nDelayCounter,pBoggyWnd->m_pCurAction->m_nDelay);
	TextOut(m_hDC,32*pBoggyWnd->m_nScreenSize,l+=15,tmpbuf,strlen(tmpbuf));
	sprintf(tmpbuf,"Direction = %d",pBoggyWnd->m_pCurAction->m_nDirection);
	TextOut(m_hDC,32*pBoggyWnd->m_nScreenSize,l+=15,tmpbuf,strlen(tmpbuf));
	sprintf(tmpbuf,"MoveStep = %d",pBoggyWnd->m_pCurAction->m_nMoveStep);
	TextOut(m_hDC,32*pBoggyWnd->m_nScreenSize,l+=15,tmpbuf,strlen(tmpbuf));
	sprintf(tmpbuf,"ResID = %d",pBoggyWnd->m_pCurAction->m_nResID);
	TextOut(m_hDC,32*pBoggyWnd->m_nScreenSize,l+=15,tmpbuf,strlen(tmpbuf));
	sprintf(tmpbuf,"nCurAction dir=%04x,act=%04x",HIWORD(pBoggyWnd->m_pBoggyAI->m_nCurAction),LOWORD(pBoggyWnd->m_pBoggyAI->m_nCurAction));
	TextOut(m_hDC,32*pBoggyWnd->m_nScreenSize,l+=15,tmpbuf,strlen(tmpbuf));
	sprintf(tmpbuf,"howlong = %03d,mode =%03d",pBoggyWnd->m_pBoggyAI->m_nHowLong,pBoggyWnd->m_pBoggyAI->m_nBoggyMode);
	TextOut(m_hDC,32*pBoggyWnd->m_nScreenSize,l+=15,tmpbuf,strlen(tmpbuf));
    EndPaint( m_hWnd, &ps );
	return 0;
}
#endif

LRESULT C_BkgWnd::OnDestroy(WPARAM wParam,LPARAM lParam)
{
	PostQuitMessage( 0 );
	return 0;
}

