#ifndef _C_PREFERENCEWND_H_
#define _C_PREFERENCEWND_H_

#include "BitmapRgnWnd.h"

class C_PreferenceWnd : public C_BitmapRgnWnd
{
public:
	C_PreferenceWnd(HWND hwndParent,int nResID);
	~C_PreferenceWnd();

    LRESULT WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam );
};
#endif