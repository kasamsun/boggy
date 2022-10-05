#include <MCI.h>

BEGIN_HANDLE_MSG(C_MCI,C_GenericWnd)
	HANDLE_MSG(MM_MCINOTIFY,OnMCINotify)

END_HANDLE_MSG()

C_MCI::C_MCI()
{
}

C_MCI::~C_MCI()
{
}

BOOL C_MCI::Create(LPCTSTR lpszCaption,
						DWORD dwExStyle,
						DWORD dwStyle,
						const RECT& rect,
						C_Wnd* pParentWnd)
{
	m_hWnd = CreateWindowEx(  dwExStyle,
    						m_szClassName,
                            lpszCaption,
                            dwStyle,
                            rect.left,
                            rect.top,
                            rect.right-rect.left,
                            rect.bottom-rect.top,
                            pParentWnd->GetHandle(),
                            NULL,
                            C_App::m_hInstance,
                            (LPSTR) this );
	 if ( ! m_hWnd )
		  exit(FALSE);
	m_pParentWnd = pParentWnd;

	return TRUE;
}

LRESULT C_MCI::OnMCINotify(WPARAM wFlags,LONG lDevID)
{
	switch (wFlags)
	{
	case MCI_NOTIFY_ABORTED:
		this->OnNotifyAborted(lDevID);
		break;
	case MCI_NOTIFY_FAILURE:
		this->OnNotifyFailure(lDevID);
		break;
	case MCI_NOTIFY_SUCCESSFUL:
		this->OnNotifySuccessful(lDevID);
		break;
	case MCI_NOTIFY_SUPERSEDED:
		this->OnNotifySuperseded(lDevID);
		break;
	}
	return 0;
}

DWORD C_MCI::PlayMidiFile(LPSTR lpszMIDIFileName)
{
	// Plays a specified MIDI file by using MCI_OPEN and MCI_PLAY. Returns
	// as soon as playback begins. The window procedure function for the
	// specified window will be notified when playback is complete.
	// Returns 0L on success; otherwise, it returns an MCI error code.
	UINT wDeviceID;
	DWORD dwReturn;
	MCI_OPEN_PARMS mciOpenParms;
	MCI_PLAY_PARMS mciPlayParms;
	MCI_STATUS_PARMS mciStatusParms;
//	MCI_SEQ_SET_PARMS mciSeqSetParms;

    // Open the device by specifying the device and filename.
    // MCI will attempt to choose the MIDI mapper as the output port.
    mciOpenParms.lpstrDeviceType = "sequencer";
    mciOpenParms.lpstrElementName = lpszMIDIFileName;
    if ((dwReturn = mciSendCommand(NULL, MCI_OPEN,
        MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
        (DWORD)(LPVOID) &mciOpenParms))!=0)
    {
        // Failed to open device. Don't close it; just return error.
        return (dwReturn);
    }

    // The device opened successfully; get the device ID.
    wDeviceID = mciOpenParms.wDeviceID;

    // Check if the output port is the MIDI mapper.
    mciStatusParms.dwItem = MCI_SEQ_STATUS_PORT;
    if ((dwReturn = mciSendCommand(wDeviceID, MCI_STATUS,
        MCI_STATUS_ITEM, (DWORD)(LPVOID) &mciStatusParms))!=0)
    {
        mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
        return (dwReturn);
    }

    // Begin playback. The window procedure function for the parent
    // window will be notified with an MM_MCINOTIFY message when
    // playback is complete. At this time, the window procedure closes
    // the device.
    mciPlayParms.dwCallback = (DWORD) m_hWnd;
	if ((dwReturn = mciSendCommand(wDeviceID, MCI_PLAY,
			NULL,
			(DWORD)(LPVOID) &mciPlayParms))!=0)
    {
        mciSendCommand(wDeviceID,MCI_CLOSE,0,NULL);
        return (dwReturn);
    }
    return (0L);
}
