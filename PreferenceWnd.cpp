#include "PreferenceWnd.h"

BEGIN_HANDLE_MSG(C_PreferenceWnd,C_BitmapRgnWnd)


END_HANDLE_MSG()

C_PreferenceWnd::C_PreferenceWnd( HWND hwndParent,int nResID)
		:C_BitmapRgnWnd(hwndParent,nResID)
{
	// enter modal loop
	RunModalLoop();
}

C_PreferenceWnd::~C_PreferenceWnd()
{
}


