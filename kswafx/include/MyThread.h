#ifndef _C_MYTHREAD_H_
#define _C_MYTHREAD_H_

#include <windows.h>

class C_MyThread
{
public:
	C_MyThread();
	~C_MyThread();

	virtual int Create(LPVOID lpParameter);
	virtual void Destroy();
	virtual void Suspend();
	virtual void Resume();
	virtual void Join();

	virtual int GetPriority();
	virtual int SetPriority(int nPriority);

    static DWORD WINAPI ThreadDispatcher(LPVOID lpParameter);

protected:
    LPVOID m_lpParameter;
	// overrideable thread procedure for derive class
    virtual DWORD MyThreadFunction();

private:
	int	m_nPriority;
	HANDLE m_hThread;
	unsigned long m_uThreadId;
};

#endif
