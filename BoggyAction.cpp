#include "BoggyAction.h"

#include "BoggyApp.h"
#include "BoggyWnd.h"


C_BoggyAction::C_BoggyAction()
{
}

C_BoggyAction::C_BoggyAction(int totalframe,int delay,int movestep,int ID,int nMode)
{
	m_nTotalFrame = totalframe;
	m_nDelay = delay;
	m_nMoveStep = movestep;
	m_nResID = ID;
	CreateBoggyBitmap(nMode);
}

C_BoggyAction::~C_BoggyAction()
{
	DeleteBoggyBitmap();
}

void C_BoggyAction::CreateBoggyBitmap(int nMode)
{
	HDC hdctmp = CreateCompatibleDC(NULL);
	HBITMAP hbmptmp = LoadBitmap(C_App::m_hInstance,MAKEINTRESOURCE(m_nResID));
	HBITMAP hbmptmpold = (HBITMAP)SelectObject(hdctmp,hbmptmp);

	BITMAP bm;
	GetObject(hbmptmp,sizeof(BITMAP),&bm);
	int nImgWd = bm.bmWidth*nMode;
	int nImgHg = bm.bmHeight*nMode;

	m_hdcImg = CreateCompatibleDC(NULL);
	m_hbmpImg = CreateCompatibleBitmap(hdctmp,nImgWd,nImgHg);
	m_hbmpImgOld = (HBITMAP) SelectObject(m_hdcImg,m_hbmpImg);

	// load and stretch from source picture
	StretchBlt(m_hdcImg,0,0,nImgWd,nImgHg,
				hdctmp,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);

	DeleteObject(SelectObject(hdctmp,hbmptmpold));
	DeleteDC(hdctmp);
}

void C_BoggyAction::DeleteBoggyBitmap()
{
	DeleteObject(SelectObject(m_hdcImg,m_hbmpImgOld));
	DeleteDC(m_hdcImg);
}

int C_BoggyAction::IsFinish()
{
	if ( m_nCurFrame > m_nTotalFrame && m_nDelayCounter == 1 )
		return TRUE;
	else
		return FALSE;
}

void C_BoggyAction::StartAction(int nDirection)
{
	m_nCurFrame = 1;
	m_nDelayCounter = 1;
	m_nDirection = nDirection;
}

int C_BoggyAction::DoAction()
{
	if ( m_nDelay == 1 )
	{
		if ( m_nMoveStep )
			return MOVE;
		else return STILL;
	}
	else if ( m_nDelay > 1 )
	{
		if ( m_nDelayCounter == 1 )
			return STILL;
		else return NOTHING;
	}
	return STILL;
}

void C_BoggyAction::ResumeAction()
{
	m_nDelayCounter++;
	if ( m_nDelayCounter > m_nDelay )
	{
		m_nDelayCounter = 1;
		m_nCurFrame++;
	}
}
