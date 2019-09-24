#ifndef _C_MCI_H_
#define _C_MCI_H_

#include <windows.h>
#include <mmsystem.h>
#include <GenericWnd.h>

class C_MCI : public C_GenericWnd
{
public:
	C_MCI();
	virtual ~C_MCI();

	BOOL Create(LPCTSTR lpszCaption,
						DWORD dwExStyle,
						DWORD dwStyle,
						const RECT& rect,
						C_Wnd* pParentWnd);
	DWORD PlayMidiFile(LPSTR lpszMIDIFileName);

    // event handling function
	LRESULT WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam );

protected:
	virtual void OnNotifyAborted(LONG lDevID)=0;
	virtual void OnNotifyFailure(LONG lDevID)=0;
	virtual void OnNotifySuccessful(LONG lDevID)=0;
	virtual void OnNotifySuperseded(LONG lDevID)=0;

private:
	LRESULT OnMCINotify(WPARAM wFlags,LONG lDevID);
};

#endif