#ifndef _C_BOGGYAI_H_
#define _C_BOGGYAI_H_

#include <windows.h>
class C_BoggyWnd;

class C_BoggyAI
{
#ifdef _DEBUG
public:// public on debugging to show in other window
#endif
	DWORD m_nCurAction;	// |== WORD ==|== WORD ==|
						//  direction   action no.
	int m_nHowLong;
	int m_nBoggyMode;
	int m_nPower;

public:
	C_BoggyAI(C_BoggyWnd * pBoggyWnd);
	~C_BoggyAI();
/*
	getscreensize();
	getcurrentpos();
	getcurrentaction();
*/
	C_BoggyWnd * GetBoggyWnd() { return m_pBoggyWnd; }
	int GenNextAction();
	/////////// Action ///////////////
	int RunComeAndGo();
	int RunFollowMouse();
	// WalkComeAndGo();
	// WalkFollowMouse();
	// ChangeLevel();
	// GotoBed();
	// SitForCommand();
private:
	C_BoggyWnd * m_pBoggyWnd;

};

#endif