#include "stdafx.h"
#include "FileWatch.h"

CFileWatch *CFileWatch::m_pThis = NULL;
std::vector<std::wstring> fileList;

CFileWatch::CFileWatch()
{
	m_pThis = this;

	SECURITY_ATTRIBUTES ThreadAttributes;
	ThreadAttributes.bInheritHandle = false;
	ThreadAttributes.lpSecurityDescriptor = NULL;
	ThreadAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);

	hWatchThread = CreateThread(&ThreadAttributes, 0, FileChangeNotifyThread, NULL, 0, NULL);
	if (hWatchThread == NULL)
	{
		MessageBox(0, L"������ ���� �۾��� �����߽��ϴ�.", 0, MB_ICONERROR);
	}
}

CFileWatch::~CFileWatch()
{
	TerminateThread(hWatchThread, 0);
	Cl.FileWatch = 0;
}

void CFileWatch::TurnOn()
{
	bWatch = true;
}

void CFileWatch::TurnOff()
{
	bWatch = false;
}

bool CFileWatch::IsTurned()
{
	return bWatch;
}

DWORD CFileWatch::_FileChangeNotifyThread(LPVOID lpParam)
{
	HWND hwnd = (HWND)lpParam;
	UINT m_nTimerID;

	std::wstring Path;
	GetLoadPath(Path);

	HANDLE hDir = CreateFile(Path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
		0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0);
	CONST DWORD cbBuffer = 1024;
	BYTE pBuffer[1024];
	BOOL bWatchSubtree = FALSE;
	DWORD dwNotifyFilter = FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
		FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE |
		FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION;
	DWORD bytesReturned;

	m_nTimerID = timeSetEvent(300, 0, (LPTIMECALLBACK)FileChangeNotifyProc, 0, TIME_PERIODIC);

	wchar_t temp[MAX_PATH] = { 0 };
	for (;;)
	{
		FILE_NOTIFY_INFORMATION* pfni;
		BOOL fOk = ReadDirectoryChangesW(hDir, pBuffer, cbBuffer,
			bWatchSubtree, dwNotifyFilter, &bytesReturned, 0, 0);
		if (!fOk)
		{
			DWORD dwLastError = GetLastError();
			break;
		}
		pfni = (FILE_NOTIFY_INFORMATION*)pBuffer;

		if (m_pThis->bWatch)
		{
			std::vector<std::wstring> fileList_Temp;
			do {
				memcpy(temp, pfni->FileName, pfni->FileNameLength);
				temp[pfni->FileNameLength / 2] = 0;
				std::wstring filename(temp);
				transform(filename.begin(), filename.end(), filename.begin(), tolower);

				if (fileList_Temp.begin() == fileList_Temp.end())
				{
					fileList_Temp.push_back(filename.c_str());
				}

				std::vector<std::wstring>::iterator it = fileList_Temp.begin();
				for (; it != fileList_Temp.end(); it++)
				{
					if (it + 1 == fileList_Temp.end())
					{
						fileList_Temp.push_back(filename.c_str());
						break;
					}
					else if ((*it).compare(filename.c_str()) == 0) break;
				}

				pfni = (FILE_NOTIFY_INFORMATION*)((PBYTE)pfni + pfni->NextEntryOffset);
			} while (pfni->NextEntryOffset > 0);

			fileList = fileList_Temp;
		}
	}
	return 0;
}

MMRESULT CFileWatch::_FileChangeNotifyProc(UINT m_nTimerID, UINT uiMsg, DWORD dwUser, DWORD dw1, DWORD d2)
{
	bool c_config;
	bool c_anedic;

	c_config = false;
	c_anedic = false;

	if (fileList.begin() == fileList.end()) return 0;

	std::vector<std::wstring> fileList_Temp = fileList;
	fileList.clear();

	std::vector<std::wstring>::iterator it = fileList_Temp.begin();
	for (; it != fileList_Temp.end(); it++)
	{
		if ((*it).compare(L"anemone.ini") == 0)
		{
			c_config = true;
		}

		if ((*it).compare(L"anedic.txt") == 0)
		{
			c_anedic = true;
		}
	}
	
	// ���� ��尡 �۵����϶� ���Ϻ����
	if (c_config == true)
	{
		c_config = false;

		//MessageBox(0, 0, 0, 0);
		Cl.Config->LoadConfig();

		//PostMessage(hWnds.Main, WM_COMMAND, ID_LOAD_CONFIG, 0);

		//PostMessage(hWnds.Main, WM_COMMAND, ID_SETTING_CHECK, 0);
		//PostMessage(hSettingWnd, WM_USER, UM_REFRESH_SETTING, 0);
	}

	if (c_anedic == true)
	{
		c_anedic = false;

		// �Ƴ׸�� ���� �ٽ��б�
		Cl.TextProcess->LoadDictionary();
		//PostMessage(hWnds.Main, WM_COMMAND, ID_LOAD_DICTIONARY, 0);
	}

	return 0;
}
