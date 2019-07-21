#pragma once
#include "stdafx.h"
class CFileWatch
{
public:
	CFileWatch();
	~CFileWatch();
	void TurnOn();
	void TurnOff();
	bool IsTurned();
private:
	static CFileWatch *m_pThis;
	HANDLE hWatchThread;

	bool bWatch = true;

	DWORD _FileChangeNotifyThread(LPVOID lpParam);
	UINT _FileChangeNotifyProc(UINT m_nTimerID, UINT uiMsg, DWORD dwUser, DWORD dw1, DWORD d2);

	// Trampoline
	static DWORD WINAPI FileChangeNotifyThread(LPVOID lpParam)
	{
		return m_pThis->_FileChangeNotifyThread(lpParam);
	}

	static UINT CALLBACK FileChangeNotifyProc(UINT m_nTimerID, UINT uiMsg, DWORD dwUser, DWORD dw1, DWORD d2)
	{
		return m_pThis->_FileChangeNotifyProc(m_nTimerID, uiMsg, dwUser, dw1, d2);
	}

};

