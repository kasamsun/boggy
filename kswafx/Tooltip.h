#ifndef _C_TOOLTIP_H_
#define _C_TOOLTIP_H_

#include <windows.h>
#include <commctrl.h>

#include <Control.h>

class C_Wnd;

class C_Tooltip : public C_Control
{
public:
	C_Tooltip();
	~C_Tooltip();

	BOOL Create(C_Wnd* pParentWnd);

    // now i use hwnd as identifier. no need uid now.
    // hwnd + id as identifier , implement later
    BOOL AddTool(HWND hWnd,LPTSTR lpszTipText);
    void DelTool(HWND hWnd);
    void SetDelayTime(UINT uFlag,int nDelay);
    void UpdateTipText(HWND hWnd,LPTSTR lpszTipText);

protected:

private:

};

#endif