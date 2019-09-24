#include <process.h>

#include <MyThread.h>


C_MyThread::C_MyThread()
{
	m_nPriority	= THREAD_PRIORITY_NORMAL;
	m_hThread = NULL;
	m_uThreadId = 0;
    m_lpParameter = NULL;
}

C_MyThread::~C_MyThread()
{
    if( m_hThread != NULL )
    {
	    CloseHandle(m_hThread);
    }
}

DWORD WINAPI C_MyThread::ThreadDispatcher(LPVOID lpParameter)
{
    return ((C_MyThread*) lpParameter)->MyThreadFunction();
}

DWORD C_MyThread::MyThreadFunction()
{
	return 0;
}

int C_MyThread::Create(LPVOID lpParameter)
{
    m_lpParameter = lpParameter;

	m_hThread = CreateThread(NULL,0,ThreadDispatcher,this,0,&m_uThreadId);

	if( m_hThread )
		return FALSE;

	return TRUE;
}

void C_MyThread::Destroy()
{
	TerminateThread(m_hThread,0);
}

void C_MyThread::Suspend()
{
	SuspendThread(m_hThread);
}

void C_MyThread::Resume()
{
	ResumeThread(m_hThread);
}

void C_MyThread::Join()
{
	WaitForSingleObject(m_hThread,INFINITE);
}

int C_MyThread::GetPriority()
{
	return GetThreadPriority(m_hThread);
}

int C_MyThread::SetPriority(int nPriority)
{
	int nOldPriority = GetThreadPriority(m_hThread);

	SetThreadPriority(m_hThread, nPriority);
	return nOldPriority;
}
