﻿#include "StdAfx.h"
#include "TextProcess.h"
#include "TextPreProcess.h"
#include "Anemone.h"

CTextProcess *CTextProcess::m_pThis = NULL;

CTextProcess::CTextProcess()
{
	m_pThis = this;
	if (!ehndSupport) LoadDictionary();
	/*StartWatchClip();
	StartHookMonitor();*/
}


//void CTextProcess::StartWatchClip()
//{
//	bClipIgnore = true;
//
//	if (IsActive == false)
//	{
//		if (Cl.Config->GetClipSwitch())
//		{
//			hWnds.Clip = SetClipboardViewer(hWnds.Main);
//		}
//	}
//	else
//	{
//		hWnds.Clip = SetClipboardViewer(hWnds.Main);
//	}
//
//	PostMessage(hWnds.Main, WM_PAINT, 0, 1);
//}

//void CTextProcess::EndWatchClip()
//{
//	ChangeClipboardChain(hWnds.Main, NULL);
//}
//
//void CTextProcess::ResetWatchClip()
//{
//	ChangeClipboardChain(hWnds.Main, NULL);
//	bClipIgnore = true;
//	hWnds.Clip = SetClipboardViewer(hWnds.Main);
//}
//
//void CTextProcess::StartHookMonitor()
//{
//	SECURITY_ATTRIBUTES ThreadAttributes;
//	ThreadAttributes.bInheritHandle = false;
//	ThreadAttributes.lpSecurityDescriptor = NULL;
//	ThreadAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
//
//	hHookMonitorThread = CreateThread(&ThreadAttributes, 0, HookMonitorProc, NULL, 0, NULL);
//	if (hHookMonitorThread == NULL)
//	{
//		MessageBox(0, L"쓰레드 생성 작업을 실패했습니다.", 0, MB_ICONERROR);
//	}
//}
//
//void CTextProcess::EndHookMonitor()
//{
//	if (hHookMonitorThread != NULL)
//	{
//		TerminateThread(hHookMonitorThread, 0);
//		hHookMonitorThread = 0;
//	}
//}
//
//DWORD CTextProcess::_HookMonitorProc(LPVOID lpParam)
//{
//	HWND hITH, hEdit, hThCombo;
//
//	std::wstring Prev_Word;
//	std::wstring Last_Word;
//	std::wstring Current_Word;
//
//	while (1)
//	{
//		hITH = 0;
//		hEdit = 0;
//
//		// ITH가 발견되지 않으면 발견될떄까지 찾는다
//		while ((hITH = FindWindow(L"ITH", 0)) == 0){ Sleep(200); }
//
//		// ITH 에디트 컨트롤을 찾는다
//		for (HWND hFind = 0; (hFind = FindWindowEx(hITH, hFind, 0, 0)) != 0; ) {
//			if (GetWindowLong(hFind, GWL_STYLE) == 0x50200144)
//			{
//				hEdit = hFind;
//			}
//		}
//
//		for (HWND hFind = 0, nCombo = 0; ((hFind = FindWindowEx(hITH, hFind, 0, 0)) != 0); )
//		{
//			// COMBOBOX
//			if (GetWindowLong(hFind, GWL_STYLE) == 0x50010303)
//			{
//				if (nCombo == 0) nCombo++;
//				else hThCombo = hFind;
//			}
//		}
//
//		DWORD nLast_TickCount = 0;
//
//		//wchar_t *Prev_Word = 0;
//		//wchar_t *Last_Word = 0;
//
//		int nStep = 0;
//		int nFStep = 0;
//		int nThNum = SendMessage(hThCombo, CB_GETCURSEL, 0, 0);
//
//		bool bChanged = false;
//		bool bWait = false;
//
//		wchar_t *buf;
//		int cch;
//
//		while ( 1 )
//		{
//			if (GetWindowLong(hEdit, GWL_STYLE) != 0x50200144) break;
//
//			// EDIT 내용을 읽음
//			cch = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
//			if (Prev_Word != L"" && cch == Prev_Word.length())
//			{
//				Sleep(5);
//				continue;
//			}
//
//			buf = (wchar_t *)malloc((cch + 2) * 2);
//			if (buf == NULL)
//			{
//				Sleep(5);
//				continue;
//			}
//			buf[0] = 0x00;
//
//			SendMessage(hEdit, WM_GETTEXT, (WPARAM)(cch + 1), (LPARAM)buf);
//			if (cch != 0 && buf[0] == 0x00)
//			{
//				Sleep(5);
//				free(buf);
//				continue;
//			}
//			Current_Word = buf;
//			free(buf);
//
//			/* 쓰레드 번호 얻어옴 */
//			int nCurThNum;
//
//			cch = SendMessage(hThCombo, WM_GETTEXTLENGTH, 0, 0);
//			buf = (wchar_t *)malloc((cch + 2) * 2);
//			if (buf == NULL)
//			{
//				Sleep(5);
//				continue;
//			}
//			buf[0] = 0x00;
//
//			SendMessage(hThCombo, WM_GETTEXT, (WPARAM)(cch + 1), (LPARAM)buf);
//
//			if (cch != 0 && buf[0] == 0x00)
//			{
//				Sleep(5);
//				free(buf);
//				continue;
//			}
//
//			buf[4] = 0x00;
//
//			nCurThNum = _wtoi(buf);
//			free(buf);
//			
//			/*
//
//			int nCurThNum = SendMessage(hThCombo, CB_GETCURSEL, 0, 0);
//			*/
//			// 0번 쓰레드는 번역하지 않는다
//			if (nCurThNum == 0)
//			{
//				OutputDebugString(L"nCurThNum == 0");
//				Sleep(5);
//				continue;
//			}
//			
//			// 초기화
//			if (Prev_Word == L"")
//				Prev_Word = Current_Word;
//
//			if (!bWait && (Current_Word.length() < Prev_Word.length()) || nCurThNum != nThNum)
//			{
//				bWait = true;
//				nThNum = nCurThNum;
//				nLast_TickCount = GetTickCount();
//				continue;
//			}
//
//			// Clear 하거나 쓰레드 변경시 초기화
//			if (bWait)
//			{
//				if (GetTickCount() - nLast_TickCount <= 500)
//					continue;
//				else
//				{
//					bWait = false;
//					bChanged = false;
//					//if (Prev_Word != 0) free(Prev_Word);
//					//if (Last_Word != 0) free(Last_Word);
//
//					Prev_Word = Current_Word;
//					Last_Word = L"";
//					nLast_TickCount = 0;
//					continue;
//				}
//			}
//
//			// 변경 최초 감지
//			if (!bChanged)
//			{
//				if (Current_Word.length() != Prev_Word.length())
//				{
//					bChanged = true;
//					Last_Word = Current_Word;
//					nLast_TickCount = GetTickCount();
//				}
//				// 달라진게 없으면 메모리 해제
//				else if (Prev_Word != Current_Word)
//				{
//				}
//			}
//
//			else if (bChanged)
//			{
//				// 일정 시간 안에 텍스트가 추가되면 기다린다
//				if (GetTickCount() - nLast_TickCount <= Cl.Config->GetHookInterval())
//				{
//					if (Current_Word.length() != Last_Word.length())
//					{
//						Last_Word = Current_Word;
//						nLast_TickCount = GetTickCount();
//					}
//					else
//					{
//					}
//
//					/*
//					// 문장 끝에 부호를 만나면 자르기
//					if (Cl.Config->GetHookTextSignCut() &&
//						(Last_Word[Last_Word.length() - 1] == L'」' ||
//						Last_Word[Last_Word.length() - 1] == L'』' ||
//						Last_Word[Last_Word.length() - 1] == L'）' ||
//						Last_Word[Last_Word.length() - 1] == L')' ||
//						Last_Word[Last_Word.length() - 1] == L'。' ||
//						Last_Word[Last_Word.length() - 1] == L'？' ||
//						Last_Word[Last_Word.length() - 1] == L'！'))
//					{
//						nLast_TickCount = GetTickCount() - 100000;
//					}*/
//				}
//
//				// 일정 시간동안 추가된 텍스트가 없을 때 출력
//				else
//				{
//					/*
//					free(Prev_Word);
//					free(buf);
//					Prev_Word = Last_Word;
//
//					bChanged = false;
//					nLast_TickCount = 0;
//					*/
//
//					if (Last_Word.length() != cch)
//					{
//						Last_Word = Current_Word;
//					}
//					
//					int nShift = 0;
//					if (Current_Word[Prev_Word.length()] == 0x0D && Current_Word[Prev_Word.length() + 1] == 0x0A)
//						nShift = 4;
//
//					wchar_t *text_buffer = (wchar_t *)Last_Word.c_str() + Prev_Word.length() + nShift;
//					wchar_t *line_buffer;
//
//					for (DWORD i = 0, j = 0; i < wcslen(text_buffer); i++)
//					{
//						if (text_buffer[i] == 0x0D && text_buffer[i + 1] == 0x0A)
//						{
//							line_buffer = (wchar_t *)malloc((i - j + 1) * 2);
//							wcsncpy(line_buffer, text_buffer + j, i - j);
//							line_buffer[i - j] = 0x00;
//							
//							SetDlgItemText(hWnds.HookCfg, IDC_HOOKCFG_EDIT1, line_buffer);
//
//							if (Cl.Config->GetHookMonitor())
//								SendMessage(hWnds.Main, WM_COMMAND, ID_HOOK_DRAWTEXT, (LPARAM)line_buffer);
//
//							free(line_buffer);
//
//							if (text_buffer[i + 2] == 0x0D && text_buffer[i + 3] == 0x0A) i += 2;
//							j = i + 2;
//							i++;
//						}
//
//						else if (text_buffer[i] == 0x0A)
//						{
//							line_buffer = (wchar_t *)malloc((i - j + 1) * 2);
//							wcsncpy(line_buffer, text_buffer + j, i - j);
//							line_buffer[i - j] = 0x00;
//
//							SetDlgItemText(hWnds.HookCfg, IDC_HOOKCFG_EDIT1, line_buffer);
//
//							if (Cl.Config->GetHookMonitor())
//								SendMessage(hWnds.Main, WM_COMMAND, ID_HOOK_DRAWTEXT, (LPARAM)line_buffer);
//
//							free(line_buffer);
//
//
//							if (text_buffer[i + 1] == 0x0A) i++;
//							j = i + 1;
//						}
//
//						else if (i + 1 == wcslen(text_buffer))
//						{
//							line_buffer = (wchar_t *)malloc((i - j + 1 + 1) * 2);
//							wcsncpy(line_buffer, text_buffer + j, i - j + 1);
//							line_buffer[i - j + 1] = 0x00;
//
//							SetDlgItemText(hWnds.HookCfg, IDC_HOOKCFG_EDIT1, line_buffer);
//
//							if (Cl.Config->GetHookMonitor())
//								SendMessage(hWnds.Main, WM_COMMAND, ID_HOOK_DRAWTEXT, (LPARAM)line_buffer);
//
//							free(line_buffer);
//
//							break;
//						}
//					}
//
//					/*
//					for (DWORD i = 0, j = 0; i < wcslen(text_buffer); i++)
//					{
//						if (i + 1 == wcslen(text_buffer))
//						{
//							wchar_t *text_buffer2 = (wchar_t *)malloc((wcslen(text_buffer) - j + 1) * 2);
//							wcscpy(text_buffer2, text_buffer + j);
//
//							SetDlgItemText(hWnds.HookCfg, IDC_HOOKCFG_EDIT1, text_buffer2);
//
//							if (Cl.Config->GetHookMonitor())
//								SendMessage(hWnds.Main, WM_COMMAND, ID_HOOK_DRAWTEXT, (LPARAM)text_buffer2);
//
//							free(text_buffer2);
//						}
//						else if (text_buffer[i] == 0x0D && text_buffer[i + 1] == 0x0A)
//						{
//							if (i - j > 1)
//							{
//								// 끝에 붙은 0x0D, 0x0A는 제외한다
//								wchar_t *text_buffer2 = (wchar_t *)malloc((i - j + 1) * 2);
//								wcsncpy(text_buffer2, text_buffer + j, i - j);
//								text_buffer2[i - j] = 0x00;
//
//								SetDlgItemText(hWnds.HookCfg, IDC_HOOKCFG_EDIT1, text_buffer2);
//
//								if (Cl.Config->GetHookMonitor())
//									SendMessage(hWnds.Main, WM_COMMAND, ID_HOOK_DRAWTEXT, (LPARAM)text_buffer2);
//
//								free(text_buffer2);
//							}
//
//							// 0x0D 0x0A가 한번 더 있는 경우 PASS
//							if (i + 3 < wcslen(text_buffer) && text_buffer[i + 2] == 0x0D && text_buffer[i + 3] == 0x0A)
//							{
//								i += 2;
//							}
//
//							j = i + 2;
//							i++;
//						}
//					}
//					*/
//					
//					/*
//					SetDlgItemText(hWnds.HookCfg, IDC_HOOKCFG_EDIT1, text_buffer);
//
//					if (Cl.Config->GetHookMonitor())
//						SendMessage(hWnds.Main, WM_COMMAND, ID_HOOK_DRAWTEXT, (LPARAM)text_buffer);
//						*/
//					//wchar_t *buf_copy = (wchar_t *)malloc((wcslen(Last_Word) + 1) * 2);
//					//wcscpy(buf_copy, Last_Word);
//
//					//free(Last_Word);
//
//					Prev_Word = Last_Word;
//					Last_Word = L"";
//
//					nLast_TickCount = 0;
//					bChanged = false;
//				}
//			}
//
//			/*
//			// 처음
//			if (nPrev_TickCount == 0)
//			{
//				nPrev_TickCount = GetTickCount();
//				Prev_Word = buf;
//				nFStep++;
//				continue;
//			}
//
//			// Prev_Word == 0 or buf < prev_word
//			if (Prev_Word == 0 || wcslen(buf) < wcslen(Prev_Word))
//			{
//				if (Prev_Word != 0) free(Prev_Word);
//				Prev_Word = buf;
//				nPrev_TickCount = GetTickCount();
//				continue;
//			}
//
//			// buf가 LastWord보다 큰경우
//			if (Last_Word == 0 || wcslen(buf) - wcslen(Last_Word) > 0)
//			{
//				if (Last_Word != 0) free(Last_Word);
//				Last_Word = buf;
//				continue;
//			}
//
//			if (wcslen(buf) != wcslen(Prev_Word))
//			{
//				std::wstringstream wss1;
//				wss1 << L"prev : ";
//				wss1 << wcslen(Prev_Word);
//				wss1 << L"\r\n";
//				wss1 << L"current : ";
//				wss1 << wcslen(buf);
//				wss1 << L"\r\n";
//				wss1 << Last_Word;
//
//				SetDlgItemText(hWnds.HookCfg, IDC_HOOKCFG_EDIT1, wss1.str().c_str());
//			}
//
//			// 인터벌 안에 새로운 단어가 들어옴 (연속되는 단어, 문장일 가능성)
//			if (GetTickCount() - nPrev_TickCount <= Cl.Config->GetHookInterval())
//			{
//				if (Last_Word != 0) free(Last_Word);
//				Last_Word = buf;
//				//nPrev_TickCount = nCurr_TickCount;
//			}
//			// 인터벌 지난 문장
//			else 
//			{
//
//				int nLength = wcslen(Last_Word) - wcslen(Prev_Word) - 4;
//				if (nLength > 0)
//				{
//					buf = (wchar_t *)malloc((nLength + 1) * 2);
//					//buf = (wchar_t *)malloc((10 + 1) * 2);
//					//wcscpy(buf, L"테스트 메세지입니다");
//					wcscpy(buf, Last_Word + wcslen(Prev_Word) + 4);
//					//memcpy(buf, Last_Word + wcslen(Prev_Word)*2, nLength * 2);
//
//					buf[wcslen(Prev_Word)] = 0x00;
//					PostMessage(hWnds.Main, WM_COMMAND, ID_HOOK_DRAWTEXT, (LPARAM)buf);
//
//					nStep++;
//					
//					std::wstringstream wss2;
//					wss2 << L"Result : ";
//					wss2 << buf;
//					wss2 << L"\r\n\r\n";
//					wss2 << L"Step : ";
//					wss2 << nFStep;
//					wss2 << L"\r\nStep : ";
//					wss2 << nStep;
//					wss2 << L"\r\nPrev_TickCount : ";
//					wss2 << nPrev_TickCount;
//					wss2 << L"\r\nCurrent_TickCount : ";
//					wss2 << GetTickCount();
//
//
//					SetDlgItemText(hWnds.HookCfg, IDC_HOOKCFG_EDIT2, wss2.str().c_str());
//
//
//					nPrev_TickCount = GetTickCount();
//
//
//					free(Prev_Word);
//					free(Last_Word);
//					Prev_Word = 0;
//					Last_Word = 0;
//				}
//			}*/
//			Sleep(1);
//		}
//		Sleep(100);
//	}
//}
/*
std::wstring CTextProcess::eztrans_mt_proc(std::wstring &input)
{
	std::hash_map<int, std::wstring> h1;
	
	int i = 0;
	std::wstring::size_type nprev = 0;
	std::wstring::size_type npos = -1;
	for (;i < 10; i++)
	{
		nprev = npos + 1;
		npos = input.find(L"\n", nprev);
		if (npos != std::string::npos)
		{
			h1[i] = input.substr(nprev, npos-nprev);
		}
		else
		{
			h1[i] = input.substr(nprev);
			//MessageBox(0, h1[1].c_str(), 0, 0);
			break;
		}
	}
	//MessageBox(0, str, 0, 0);

	std::wstring output = L"END";

	std::hash_map<int, std::wstring> *pt;
	pt = &h1;
	for (int j = 0; j < i; j++)
	{
		HANDLE hThread = NULL;
		DWORD dwThreadID = NULL;
		hThread = (HANDLE)_beginthreadex(NULL, 0, this->ThreadFunction, pt, 0, (unsigned*)&dwThreadID);
		if (hThread == 0) MessageBox(0, L"_beginthreadex Error\n", 0, 0);
	}
	
	HANDLE hThread = NULL;
	DWORD dwThreadID = NULL;
	hThread = (HANDLE)_beginthreadex(NULL, 0, this->ThreadFunction, NULL, 0, (unsigned*)&dwThreadID);
	if (hThread == 0) MessageBox(0, L"_beginthreadex Error\n", 0, 0);
	return output;
}

unsigned int WINAPI CTextProcess::ThreadFunction(void *arg)
{
	return 0;
}
*/

bool CTextProcess::TranslateText(HWND hWnd, const std::wstring &input, int nOutputType)
{
	Elapsed_Prepare = 0;
	Elapsed_Translate = 0;

	std::wstring::size_type nprev = 0;
	std::wstring::size_type npos = -1;
	std::list<std::wstring> list, list_org, list_trans;
	std::wstring output, trans_str;
	int i = 0, length = input.length();
	std::wstring empty = L"Abort";

	bool NoTransLineFeed = Cl.Config->GetTransNoTransLineFeed();
	bool TransOneGo = Cl.Config->GetTransOneGo();

	if (nStatus != 0) return false;
	nStatus = 1;

	std::list<std::wstring>::iterator iter, iter_trans;

	std::wstring temp_str;

	//SendMessage(hWnd, WM_COMMAND, ID_TRANS_START, 0);

	for (;; i++)
	{
		nprev = npos + 1;
		npos = input.find(L"\n", nprev);
		if (npos != std::string::npos)
		{
			temp_str = input.substr(nprev, npos - nprev + 1);
			temp_str += L"|:_";
			list_org.push_back(temp_str);
		}
		else
		{
			temp_str = input.substr(nprev);
			list_org.push_back(temp_str);
			break;
		}
	}

	unsigned int div = input.length() / 1024 + 1;

	if (TransOneGo && list_org.size() > div)
	{
		std::wstring line;
		std::list<std::wstring> list2 = list_org;
		int j = 1;
		for (i = 1, iter = list2.begin(); iter != list2.end(); iter++, i++)
		{
			if ((float)i > (float)(list2.size() * j / div))
			{
				list.push_back(line);
				line = L"";
				j++;
			}

			line += (*iter);

			if (i == list2.size())
			{
				list.push_back(line);
				line = L"";
			}

		}
	}
	else
	{
		list = list_org;
	}

	for (i = 0, iter = list.begin(); iter != list.end(); iter++, i++)
	{
		if (nStatus == 2)
		{
			nStatus = 0;
			std::wstringstream logstream;
			logstream << L"번역 중지 (";
			logstream << i + 1;
			logstream << L"/";
			logstream << list.size();
			logstream << L")";

			proclog = logstream.str();
			//SendMessage(hWnd, WM_COMMAND, ID_TRANS_ABORT, (LPARAM)proclog.c_str());

			std::wstring abort_msg = L"Abort";
			return false;
		}

		int nInput, nOutput;
		int nPos, nPrev;

		for (nPrev = 0, nPos = 0, nInput = 0; nPos != std::string::npos; nInput++)
		{
			nInput++;
			nPos = (*iter).find(L"\n|:_", nPrev);
			nPrev = nPos + 4;
		}

		trans_str = Cl.TextProcess->eztrans_proc(*iter);

		for (nPrev = 0, nPos = 0, nOutput = 0; nPos != std::string::npos; nOutput++)
		{
			nOutput++;
			nPos = trans_str.find(L"\n|:_", nPrev);
			nPrev = nPos + 4;
		}

		// 묶은 것을 풀어서 재번역
		if (nInput != nOutput)
		{
			std::wstring strOrg = (*iter);
			std::wstring strRes;
			npos = 0, nprev = 0;

			do
			{
				npos = (*iter).find(L"\n|:_", nprev);
				if (npos != std::string::npos)
				{
					strRes = (*iter).substr(nprev, npos - nprev + 4);
					nprev = npos + 4;
				}
				else
				{
					strRes = (*iter).substr(nprev);
					strRes += L"_|:_";
				}

				trans_str = Cl.TextProcess->eztrans_proc(strRes);

				// 태그가 있었는데 없어진 경우 (에러)
				if (strRes.find(L"\n|:_") != std::string::npos && trans_str.find(L"\n|:_") == std::string::npos)
				{
					output += L" \r\n|:_";
				}
				else if (strRes.find(L"_|:_") != std::string::npos && trans_str.find(L"_|:_") == std::string::npos)
				{
					output += L" ";
				}
				else if (strRes.find(L"_|:_") != std::string::npos)
					output += strRes.substr(0, strRes.find(L"_|:_"));
				else
					output += trans_str;

			}
			while (npos != std::string::npos);
		}
		else output += trans_str;

		/*

		for (int j = 0; j < 3; j++)
		{
			trans_str = Cl.TextProcess->eztrans_proc(*iter);

			for (nPrev = 0, nPos = 0, nOutput = 0; nPos != std::string::npos; nOutput++)
			{
				nOutput++;
				nPos = trans_str.find(L"\n|:_", nPrev);
				nPrev = nPos + 4;
			}

			if (nInput == nOutput) break;
		}

		if (nInput != nOutput)
		{
			nStatus = 0;
			std::wstringstream logstream;
			logstream << L"번역 라인이 맞지 않습니다.\r\n오류로 번역이 중단되었습니다.";

			proclog = logstream.str();
			SendMessage(hWnd, WM_COMMAND, ID_TRANS_ABORT, (LPARAM)proclog.c_str());

			std::wstring abort_msg = L"Abort";
			return false;
		}
		else
			output += trans_str;
			*/
		std::wstringstream logstream;
		logstream << L"번역중... (";
		logstream << i + 1;
		logstream << L"/";
		logstream << list.size();
		logstream << L")";
		logstream << L"\r\n";
		logstream << L"Prepare : ";
		logstream << Elapsed_Prepare;
		logstream << L"ms\r\n";
		logstream << L"Translate : ";
		logstream << Elapsed_Translate;
		logstream << L"ms";

		proclog = logstream.str();
		//SendMessage(hWnd, WM_COMMAND, ID_TRANS_PROGRESS, (LPARAM)proclog.c_str());
	}

	npos = 0, nprev = 0;

	for (iter = list_org.begin();; iter++, i++)
	{
		npos = output.find(L"\n|:_", nprev);
		if (npos != std::string::npos)
		{
			if ((*iter).size() - 3 > 0) (*iter) = (*iter).substr(0, (*iter).size() - 3);
			list_trans.push_back(output.substr(nprev, npos - nprev + 1));
			nprev = npos + 4;
		}
		else
		{
			list_trans.push_back(output.substr(nprev));
			break;
		}
	}


	if (list_org.size() != list_trans.size())
	{
		std::wstringstream logstream;
		logstream << L"번역 도중 오류가 발생했습니다.";

		nStatus = 0;

		proclog = logstream.str();
		//SendMessage(hWnd, WM_COMMAND, ID_TRANS_ABORT, (LPARAM)proclog.c_str());
		return false;
	}

	output = L"";

	for (iter = list_org.begin(), iter_trans = list_trans.begin(); iter != list_org.end(); iter++, iter_trans++)
	{
		if (nOutputType != 0)
		{
			// 개행만 있는 라인 번역 안함 옵션 선택시 개행만 넣어줌
			if (NoTransLineFeed && *iter_trans == L"\r\n" ||
				std::next(iter, 1) == list_org.end() && *iter_trans == L"" )
			{
				output += (*iter);
			}
			// 이 라인이 마지막 라인인 경우 원문에 \r\n을 붙여준다
			else if (std::next(iter, 1) == list_org.end())
			{
				output += (*iter);
				output += L"\r\n";
				output += (*iter_trans);
			}
			else 
			{
				output += (*iter);
				output += (*iter_trans);
				if (nOutputType == 2) output += L"\r\n";
			}
		}
		else output += (*iter_trans);
	}

	proclog = output;
	//SendMessage(hWnd, WM_COMMAND, ID_TRANS_COMPLETE, (LPARAM)proclog.c_str());

	nStatus = 0;

	return true;
}

void CTextProcess::TranslateAbort()
{
	if (nStatus == 1) nStatus = 2;
	return;
}

std::wstring CTextProcess::eztrans_proc(const std::wstring &input)
{
	int nBufLen;
	char *szBuff, *szBuff2;
	wchar_t *lpszBuff;
	std::wstring szContext, output;

	int start, end;
	start = GetTickCount();
	EnterCriticalSection(&cs_trans);

	szContext = HangulEncode(input);

	// 이지트랜스 오류 잡아주기
	// 「よろしければ今度２人でお話しなどできないでしょうか」
	szContext = replaceAll(szContext, L"できないでしょ", L"@X@でき@X@ないでしょ");
	szContext = replaceAll(szContext, L"きないでしょ", L"き@X@ないでしょ");
	szContext = replaceAll(szContext, L"でき@X@ないでしょ", L"できないでしょ");

	if (ehndSupport)
	{
		lpszBuff = (wchar_t *)Cl.TransEngine->J2K_TranslateMMNTW(0, (wchar_t *)szContext.c_str());
		output = lpszBuff;
		Cl.TransEngine->J2K_FreeMem(lpszBuff);
	}
	else
	{
		nBufLen = WideCharToMultiByte(932, 0, szContext.c_str(), -1, NULL, NULL, NULL, NULL);
		szBuff = (char *)HeapAlloc(AneHeap, 0, (nBufLen + 2) * 2);

		if (szBuff == NULL)
		{
			MessageBox(0, L"메모리 할당 실패", 0, 0);
			LeaveCriticalSection(&cs_trans);
			return false;
		}

		WideCharToMultiByte(932, 0, szContext.c_str(), -1, szBuff, nBufLen, NULL, NULL);
		szBuff2 = (char *)Cl.TransEngine->J2K_TranslateMMNT(0, szBuff);
		HeapFree(AneHeap, 0, szBuff);

		nBufLen = MultiByteToWideChar(949, 0, szBuff2, -1, NULL, NULL);
		lpszBuff = (wchar_t *)HeapAlloc(AneHeap, 0, (nBufLen + 2) * 2);

		if (lpszBuff == NULL)
		{
			MessageBox(0, L"메모리 할당 실패", 0, 0);
			LeaveCriticalSection(&cs_trans);
			return false;
		}

		MultiByteToWideChar(949, 0, szBuff2, -1, lpszBuff, nBufLen);

		output = lpszBuff;
		HeapFree(AneHeap, 0, lpszBuff);
		Cl.TransEngine->J2K_FreeMem(szBuff2);
	}
	output = HangulDecode(output);

	LeaveCriticalSection(&cs_trans);
	end = GetTickCount();

	Elapsed_Prepare += (end - start);
	return output;
}

std::wstring CTextProcess::HangulEncode(const std::wstring &input)
{
	std::wstring output;
	wchar_t buf[8];

	std::wstring::const_iterator it = input.begin();
	for (; it != input.end(); it++)
	{
		if (*it == L'@' ||
			(*it == '\0') ||
			(*it >= 0x1100 && *it <= 0x11FF) || (*it >= 0x3130 && *it <= 0x318F) ||
			(*it >= 0xA960 && *it <= 0xA97F) || (*it >= 0xAC00 && *it <= 0xD7AF) ||
			(*it >= 0xD7B0 && *it <= 0xD7FF))
		{
			swprintf_s(buf, L"+x%04X", *it);
			output += buf;
		}
		else
		{
			switch (*it)
			{
			case L'↔': case L'◁': case L'◀': case L'▷': case L'▶': case L'♤': case L'♠': case L'♡': case L'♥': case L'♧': \
			case L'♣': case L'⊙': case L'◈': case L'▣': case L'◐': case L'◑': case L'▒': case L'▤': case L'▥': case L'▨': \
			case L'▧': case L'▦': case L'▩': case L'♨': case L'☏': case L'☎': case L'☜': case L'☞': case L'↕': case L'↗': \
			case L'↙': case L'↖': case L'↘': case L'♩': case L'♬': case L'㉿': case L'㈜': case L'㏇': case L'™': case L'㏂': \
			case L'㏘': case L'＂': case L'＇': case L'∼': case L'ˇ': case L'˘': case L'˝': case L'¡': case L'˚': case L'˙': \
			case L'˛': case L'¿': case L'ː': case L'∏': case L'￦': case L'℉': case L'€': case L'㎕': case L'㎖': case L'㎗': \
			case L'ℓ': case L'㎘': case L'㎣': case L'㎤': case L'㎥': case L'㎦': case L'㎙': case L'㎚': case L'㎛': case L'㎟': \
			case L'㎠': case L'㎢': case L'㏊': case L'㎍': case L'㏏': case L'㎈': case L'㎉': case L'㏈': case L'㎧': case L'㎨': \
			case L'㎰': case L'㎱': case L'㎲': case L'㎳': case L'㎴': case L'㎵': case L'㎶': case L'㎷': case L'㎸': case L'㎀': \
			case L'㎁': case L'㎂': case L'㎃': case L'㎄': case L'㎺': case L'㎻': case L'㎼': case L'㎽': case L'㎾': case L'㎿': \
			case L'㎐': case L'㎑': case L'㎒': case L'㎓': case L'㎔': case L'Ω': case L'㏀': case L'㏁': case L'㎊': case L'㎋': \
			case L'㎌': case L'㏖': case L'㏅': case L'㎭': case L'㎮': case L'㎯': case L'㏛': case L'㎩': case L'㎪': case L'㎫': \
			case L'㎬': case L'㏝': case L'㏐': case L'㏓': case L'㏃': case L'㏉': case L'㏜': case L'㏆': case L'┒': case L'┑': \
			case L'┚': case L'┙': case L'┖': case L'┕': case L'┎': case L'┍': case L'┞': case L'┟': case L'┡': case L'┢': \
			case L'┦': case L'┧': case L'┪': case L'┭': case L'┮': case L'┵': case L'┶': case L'┹': case L'┺': case L'┽': \
			case L'┾': case L'╀': case L'╁': case L'╃': case L'╄': case L'╅': case L'╆': case L'╇': case L'╈': case L'╉': \
			case L'╊': case L'┱': case L'┲': case L'ⅰ': case L'ⅱ': case L'ⅲ': case L'ⅳ': case L'ⅴ': case L'ⅵ': case L'ⅶ': \
			case L'ⅷ': case L'ⅸ': case L'ⅹ': case L'½': case L'⅓': case L'⅔': case L'¼': case L'¾': case L'⅛': case L'⅜': \
			case L'⅝': case L'⅞': case L'ⁿ': case L'₁': case L'₂': case L'₃': case L'₄': case L'Ŋ': case L'đ': case L'Ħ': \
			case L'Ĳ': case L'Ŀ': case L'Ł': case L'Œ': case L'Ŧ': case L'ħ': case L'ı': case L'ĳ': case L'ĸ': case L'ŀ': \
			case L'ł': case L'œ': case L'ŧ': case L'ŋ': case L'ŉ': case L'㉠': case L'㉡': case L'㉢': case L'㉣': case L'㉤': \
			case L'㉥': case L'㉦': case L'㉧': case L'㉨': case L'㉩': case L'㉪': case L'㉫': case L'㉬': case L'㉭': case L'㉮': \
			case L'㉯': case L'㉰': case L'㉱': case L'㉲': case L'㉳': case L'㉴': case L'㉵': case L'㉶': case L'㉷': case L'㉸': \
			case L'㉹': case L'㉺': case L'㉻': case L'㈀': case L'㈁': case L'㈂': case L'㈃': case L'㈄': case L'㈅': case L'㈆': \
			case L'㈇': case L'㈈': case L'㈉': case L'㈊': case L'㈋': case L'㈌': case L'㈍': case L'㈎': case L'㈏': case L'㈐': \
			case L'㈑': case L'㈒': case L'㈓': case L'㈔': case L'㈕': case L'㈖': case L'㈗': case L'㈘': case L'㈙': case L'㈚': \
			case L'㈛': case L'ⓐ': case L'ⓑ': case L'ⓒ': case L'ⓓ': case L'ⓔ': case L'ⓕ': case L'ⓖ': case L'ⓗ': case L'ⓘ': \
			case L'ⓙ': case L'ⓚ': case L'ⓛ': case L'ⓜ': case L'ⓝ': case L'ⓞ': case L'ⓟ': case L'ⓠ': case L'ⓡ': case L'ⓢ': \
			case L'ⓣ': case L'ⓤ': case L'ⓥ': case L'ⓦ': case L'ⓧ': case L'ⓨ': case L'ⓩ': case L'①': case L'②': case L'③': \
			case L'④': case L'⑤': case L'⑥': case L'⑦': case L'⑧': case L'⑨': case L'⑩': case L'⑪': case L'⑫': case L'⑬': \
			case L'⑭': case L'⑮': case L'⒜': case L'⒝': case L'⒞': case L'⒟': case L'⒠': case L'⒡': case L'⒢': case L'⒣': \
			case L'⒤': case L'⒥': case L'⒦': case L'⒧': case L'⒨': case L'⒩': case L'⒪': case L'⒫': case L'⒬': case L'⒭': \
			case L'⒮': case L'⒯': case L'⒰': case L'⒱': case L'⒲': case L'⒳': case L'⒴': case L'⒵': case L'⑴': case L'⑵': \
			case L'⑶': case L'⑷': case L'⑸': case L'⑹': case L'⑺': case L'⑻': case L'⑼': case L'⑽': case L'⑾': case L'⑿': \
			case L'⒀': case L'⒁': case L'⒂':
				swprintf_s(buf, L"+X%04X", *it);
				output += buf;
				break;
			default:
				output += *it;
				break;
			}
		}
	}

	return output;
}
std::wstring CTextProcess::HangulDecode(const std::wstring &input)
{
	std::wstring output;
	wchar_t buf[8];
	std::wstring::const_iterator it = input.begin();
	for (DWORD count = 0; it != input.end(); it++, count++)
	{
		// @X = 삭제
		if (count + 2 < input.length() && (*it) == L'@' && *(it + 1) == L'X' && *(it + 2) == L'@')
		{
			it += 2;
			count += 2;
			continue;
		}
		else
			if (count + 5 < input.length() && *(it) == '+' && (*(it + 1) == 'x' || *(it + 1) == 'X') &&
			((*(it + 2) >= L'A' && *(it + 2) <= L'Z') || (*(it + 2) >= L'a' && *(it + 2) <= L'z') || (*(it + 2) >= L'0' && *(it + 2) <= L'9')) &&
			((*(it + 3) >= L'A' && *(it + 3) <= L'Z') || (*(it + 3) >= L'a' && *(it + 3) <= L'z') || (*(it + 3) >= L'0' && *(it + 3) <= L'9')) &&
			((*(it + 4) >= L'A' && *(it + 4) <= L'Z') || (*(it + 4) >= L'a' && *(it + 4) <= L'z') || (*(it + 4) >= L'0' && *(it + 4) <= L'9')) &&
			((*(it + 5) >= L'A' && *(it + 5) <= L'Z') || (*(it + 5) >= L'a' && *(it + 5) <= L'z') || (*(it + 5) >= L'0' && *(it + 5) <= L'9')))
		{
		buf[0] = *(it + 2);
		buf[1] = *(it + 3);
		buf[2] = *(it + 4);
		buf[3] = *(it + 5);
		buf[4] = 0x00;

		swscanf_s(buf, L"%04x", &buf[0]);

		output += buf;
		it += 5;
		count += 5;

		}

		else
		{
			output += (*it);
		}
	}
	return output;
}

bool CTextProcess::DoubleTextFix(std::wstring &input)
{
	std::wstring output;
	std::wstring text = input;
	bool nHardFix = false;

	std::wsmatch m;
	/*
	std::wregex regex(L"(?:(.*[^「『（(])([「『（(].*))$");

	if (std::regex_match(input, m, regex))
	{
		nHardFix = true;
		output = m[1];
		text = m[2];
		if (m[2] == L"") text = input;
	}
	else text = input;
	*/

	//if (text.size() < 8) return false;

	for (unsigned int i = 0; i < text.size(); i++)
	{
		if (text[i] == text[i + 1])
		{
			output += text[i];
			i++;
		}
		else if (text[i] == 0x0D || text[i] == 0x0A) 
		{
			output += text[i];
		}
		else
		{
			// 「テティィナナ 같은 경우 처리 (앞 1글자가 짤렸을 경우) / 강에서만 작동
			if (i == 0 && Cl.Config->GetRepeatTextProc() > 3)
			{
				bool IsNext = false;
				for (i = 1; ; i++)
				{
					if (i == text.size() || i == (text.size() / 2) - (text.size() % 2))
					{
						IsNext = true;
						break;
					}
					else if (text[i] == text[i + 1]) i++;
					else
					{
						i = 0;
						break;
					}
				}

				if (IsNext == true)
				{
					output = text[0];
					i = 0;
					continue;
				}
			}
			// 4자 (중복 8자)도 안되면 문제가 있을 수 있음. 무조건 리턴
			//if (i < 4 * 2) return false;

			//for (; i < text.size(); i++)
			//	output += text[i];
			return false;
		}
	}

	input = output;
	return true;
}

bool CTextProcess::DoubleSentenceFix(std::wstring &input)
{
	int nResult = 1;
	std::wstring str = input;
	int index = str.size() / 2;
	bool Is0D = false, Is0A = false;

	for (unsigned int i = 0; i < str.length(); i++, index++)
	{
		if (str[i] != str[index])
		{
			nResult = 0;
			break;
		}

		if (i == str.length() / 2)
		{
			nResult = 1;
			break;
		}
	}

	if (nResult == 0)
	{
		index = str.length() / 2 + 1;

		for (unsigned int i = 0; i < str.length(); i++, index++)
		{
			if (index == str.length() && str.length() / 2 - i < 1)
			{
				nResult = 2;
				break;
			}

			if (str[i] != str[index])
			{
				nResult = 0;

				// 「BBB」AAAAAA「BBB」같은 경우 처리

				std::wstring str_end = str.substr(str.length() / 2, str.length() / 2);
				std::wstring str_first = str.substr(0, str.length() / 2);
				
				for (unsigned int i = 0; i < str.length() / 2; i++, index--)
				{
					str_first = str_first[str_first.length() - 1] + str_first.substr(0, str_first.length() - 1);

					if (str_end == str_first)
					{
						input = str_first;
						return true;
					}
				}
				break;
			}
		}
	}

	// 문자가 세번 반복될 때 처리
	if (nResult == 0 && str.length() % 3 == 0)
	{
		index = str.length() / 3;
		nResult = 3;
		for (unsigned int i = 0; i < str.length(); i++, index++)
		{
			if (str[i] != str[index])
			{
				nResult = 0;
				break;
			}
			else if (i + 1 == str.length() / 3)
			{
				index = 0;
			}
		}
	}

	if (nResult == 1)
	{
		// 1234 12345
		if (str.length() % 2 == 1)
		{
			str = str.substr(str.length() / 2, str.length() / 2 + 1);
		}
		// 1234 1234
		else
		{
			str = str.substr(0, str.length() / 2);
		}
	}

	// 12345 1234
	else if (nResult == 2)
	{
		str = str.substr(0, str.length() / 2 + 1);
	}
	else if (nResult == 3)
	{
		str = str.substr(0, str.length() / 3);
	}

	input = str;
	return true;
}

std::wstring CTextProcess::NameSplit(int nCode, std::wstring &input)
{
	/*
	* [2013-12-13] 이름인식 정규식 by MYC Gamer
	* [2013-07-23] 이름인식 정규식 V2 (대사 끝에 붙어 있는 이름 인식, 일반 괄호 반복 인식)
	*
	*【AA】【AA】【AA】【AA】BB
	*【AA】【AA】BB
	*【AA】【AA【AABB
	*【AA】AAAABB
	*【AACAACAAC】BB
	*
	*/
	std::wregex reg_front_r1, reg_front_r2, reg_front_r3, reg_front_r4, reg_front_r5;
	std::wregex reg_tail_r1, reg_tail_r2, reg_tail_r3, reg_tail_r4, reg_tail_r5;
	
	reg_front_r1.assign(L"^【([^】]+?)(?:】(?:【\\1)+|】\\1+|(?!([^】])\\2*】)\\1*】)([^【】].*)");
	reg_front_r2.assign(L"^(?=[^「」『』（）()])([^「]+?)()([「].*[」]|[「][^」]+?)$");
	reg_front_r3.assign(L"^(?=[^「」『』（）()])([^『]+?)()([『].*[』]|[『][^』]+?)$");
	reg_front_r4.assign(L"^(?=[^「」『』（）()])([^(]+?)()([(].*[)]|[(][^)]+?)$");
	reg_front_r5.assign(L"^(?=[^「」『』（）()])([^（]+?)()([（].*[）]|[（][^）]+?)$");
	//rx_head5.assign(L"^([^「」『』（）()]+?)(?:[「『（(](?:\\1)+|[」』）)]\\1+|(?!([^」』）)])\\1*[」』）)])\\1*([「」『』（）()].*[）)]))");
	//rx_name.assign(L"【([^】]+?)(?:】(?:【\\1)+|】\\1+|(?!([^】])\\2*】)\\1*】)([^【】].*)");
	//rx_name2.assign(L"^([^「」『』（）()]+?)(「(?:[^」]+$|.+」$)|『(?:[^』]+$|.+』$)|\\(.*\\)$|（.*）$)");
	//rx_name4.assign(L"^([「」『』（）()].*[「」『』（）()])([^「」『』（）()]+?)(?:[「『（(](?:\\2)+|[」』）)]\\2+|(?!([^」』）)])\\2*[」』）)])\\2*)");

	reg_tail_r1.assign(L"^(.*[^【】])【([^】]+?)(?:】(?:【\\2】|【\\2)+|】\\2+|(?!([^】])\\3*】)\\2*】)$");
	reg_tail_r2.assign(L"^([「].*[」]|[^「」『』（）()]+?[^「][」])([^「」]+?)$");
	reg_tail_r3.assign(L"^([『].*[』]|[^「」『』（）()]+?[^『][』])([^『』]+?)$");
	reg_tail_r4.assign(L"^([(].*[)]|[^「」『』（）()]+?[^(][)])([^()]+?)$");
	reg_tail_r5.assign(L"^([（].*[）]|[^「」『』（）()]+?[^（][）])([^（）]+?)$");

	std::wsmatch m;

	std::wstring wName, wText;
	std::wstring wNameTrans, wTextTrans;
	std::wstring wSpace = L" ";
	std::wstring wTab = L"　";
	std::wstring wEmpty = L"";

	// 【이름】
	if (std::regex_match(input, m, reg_front_r1))
	{
		wName = L"";
		wName += L"【";
		wName += m[1];
		wName += L"】";
		wText = m[3];

		wName = replaceAll(wName, wSpace, wEmpty);
		wName = replaceAll(wName, wTab, wEmpty); //(이름)대사
		wName = replaceAll(wName, L"【】", wEmpty);
	}
	// 이름 처리
	else if (std::regex_match(input, m, reg_front_r2) || 
			std::regex_match(input, m, reg_front_r3) || 
			std::regex_match(input, m, reg_front_r4) || 
			std::regex_match(input, m, reg_front_r5))
	{
		wName = m[1];
		wText = m[3];
		wName = replaceAll(wName, wSpace, wEmpty);
		wName = replaceAll(wName, wTab, wEmpty);
	}
	else
	{
		wName = wEmpty;
		wText = input;

		if (Cl.Config->GetReviseName())
		{
			// 【이름】
			if (std::regex_match(input, m, reg_tail_r1))
			{
				wName = L"";
				wName += L"【";
				wName += m[2];
				wName += L"】";
				wText = m[1];

				wName = replaceAll(wName, wSpace, wEmpty);
				wName = replaceAll(wName, wTab, wEmpty); //(이름)대사
				wName = replaceAll(wName, L"【】", wEmpty);
			}
			// 이름 처리
			else if (std::regex_match(input, m, reg_tail_r2) || 
					std::regex_match(input, m, reg_tail_r3) || 
					std::regex_match(input, m, reg_tail_r4) || 
					std::regex_match(input, m, reg_tail_r5))
			{
				wName = m[2];
				wText = m[1];
				wName = replaceAll(wName, wSpace, wEmpty);
				wName = replaceAll(wName, wTab, wEmpty);
			}
		}
	}

	if (nCode == 0) return wName;
	return wText;
}

//bool CTextProcess::OnDrawClipboard()
//{
//	std::wstring wContext;
//
//	// 클립보드를 새로 등록했을 때 현재 저장되어 있는 클립보드 내용을 무시
//	if (bClipIgnore)
//	{
//		bClipIgnore = false;
//		return 0;
//	}
//
//	// 클립보드 감시가 꺼져있는지 확인
//	if (!Cl.Config->GetClipSwitch())
//	{
//		Cl.TextProcess->EndWatchClip();
//		return 0;
//	}
//
//	// 창 숨김시 클립보드 비활성화 옵션 확인
//	else if (!Cl.Config->GetWindowVisible() && !Cl.Config->GetTempWinHide() && Cl.Config->GetHideWinUnWatchClip())
//	{
//		return 0;
//	}
//
//	OpenClipboard(hWnds.Clip);
//	HANDLE hClipData = GetClipboardData(CF_UNICODETEXT);
//
//	if (hClipData == NULL)
//	{
//		//Cl.TextRenderer->Paint();
//		//MessageBox(0, L"Invaild Clipboard Data", 0, 0);
//		CloseClipboard();
//		Cl.TextProcess->ResetWatchClip();
//		return false;
//	}
//
//	wContext = (wchar_t*)GlobalLock(hClipData);
//	GlobalUnlock(hClipData);
//	CloseClipboard();
//
//	// 클립보드 인식 길이가 넘어가면 버리기
//	if (wContext.length() > (unsigned int)Cl.Config->GetClipLength())
//	{
//		//SetDlgItemText(hWnds.HookCfg, IDC_HOOKCFG_STATUS, L"Clip Max Exceed");
//		CloseClipboard();
//		return false;
//	}
//	
//	wchar_t *text_buffer = (wchar_t *)wContext.c_str();
//	
//	/*
//	wchar_t *line_buffer;
//	std::wstring line;
//	for (DWORD i = 0, j = 0; i < wcslen(text_buffer); i++)
//	{
//		if (text_buffer[i] == 0x0D && text_buffer[i + 1] == 0x0A)
//		{
//			line_buffer = (wchar_t *)malloc((i - j + 1) * 2);
//			wcsncpy(line_buffer, text_buffer + j, i - j);
//			line_buffer[i - j] = 0x00;
//
//			line = line_buffer;
//			ProcessText(line);
//
//			free(line_buffer);
//
//			if (text_buffer[i + 2] == 0x0D && text_buffer[i + 3] == 0x0A) i += 2;
//			j = i + 2;
//			i++;
//		}
//
//		else if (text_buffer[i] == 0x0A)
//		{
//			line_buffer = (wchar_t *)malloc((i - j + 1) * 2);
//			wcsncpy(line_buffer, text_buffer + j, i - j);
//			line_buffer[i - j] = 0x00;
//
//			line = line_buffer;
//			ProcessText(line);
//
//			free(line_buffer);
//
//
//			if (text_buffer[i + 1] == 0x0A) i++;
//			j = i + 1;
//		}
//
//		else if (i + 1 == wcslen(text_buffer))
//		{
//			line_buffer = (wchar_t *)malloc((i - j + 1 + 1) * 2);
//			wcsncpy(line_buffer, text_buffer + j, i - j + 1);
//			line_buffer[i - j + 1] = 0x00;
//
//			line = line_buffer;
//			ProcessText(line);
//
//			free(line_buffer);
//
//			break;
//		}
//	}
//	*/
//	//SetDlgItemText(hWnds.HookCfg, IDC_HOOKCFG_STATUS, L"ProcText");
//	ProcessText(wContext);
//	//SetDlgItemText(hWnds.HookCfg, IDC_HOOKCFG_STATUS, L"EndText");
//	return true;
//}
//
//bool CTextProcess::OnDrawClipboardByHooker(wchar_t *lpwszstr)
//{
//	std::wstring wContext;
//	wContext = lpwszstr;
//	
//	// 인식 길이를 넘는 글자가 들어오면 버리기
//	if (wContext.length() > (unsigned int)Cl.Config->GetClipLength())
//	{
//		return false;
//	}
//
//	ProcessText(wContext);
//	return true;
//}

//bool CTextProcess::ProcessText(std::wstring &wContext)
//{
//	CTextPreProcess preProcess;
//	wContext = preProcess.PreProcessText(wContext, !Cl.Config->GetMiddleBracketRecognize());
//
//	std::wstring wName, wNameT, wNameR, wText, wTextT, wTextR, wContextT;
//
//
//	if (Cl.Config->GetRepeatTextProc() > 0)
//	{
//		DoubleTextFix(wContext);
//		DoubleSentenceFix(wContext);
//	}
//
//	wName = NameSplit(0, wContext);
//	wText = NameSplit(1, wContext);
//
//	if (Cl.Config->GetRepeatTextProc() > 1)
//	{
//		DoubleTextFix(wText);
//		DoubleSentenceFix(wText);
//	}
//	if (Cl.Config->GetRepeatTextProc() > 2)
//	{
//		DoubleTextFix(wName);
//	}
//	if (Cl.Config->GetRepeatTextProc() > 3)
//	{
//		DoubleSentenceFix(wName);
//	}
//
//	// 이름 인식 범위를 넘어간 경우
//	if (wName.length() > Cl.Config->GetNameMax())
//	{
//		std::wstring temp = wName;
//		temp += wText;
//		
//		wName = L"";
//		wText = temp;
//	}
//
//	wContext = wName;
//	wContext += wText;
//
//	wNameR = wName;
//	wTextR = wText;
//
//	// 아네모네 사전 우선 적용을 켰을 경우
//	if (Cl.Config->GetForceAneDic()) ApplyForceAneDicAll(wNameR);
//	else ApplyForceAneDic(wNameR);
//	if (Cl.Config->GetForceAneDic()) ApplyForceAneDicAll(wTextR);
//	else ApplyForceAneDic(wTextR);
//
//	wNameT = eztrans_proc(wNameR);
//	wTextT = eztrans_proc(wTextR);
//
//	wContextT = wNameT;
//	wContextT += wTextT;
//
//	//Cl.TextRenderer->SetTextSet(wName.c_str(), wNameT.c_str(), wText.c_str(), wTextT.c_str());
//
//	_viewLog VL = _viewLog(wName.c_str(), wNameT.c_str(), wText.c_str(), wTextT.c_str());
//
//	viewLog.push_back(VL);
//
//	std::wstring str;
//	str += L"\r\n";
//	str += wName;
//	str += wText;
//	str += L"\r\n";
//	if (wName.length())
//	{
//		str += L"【";
//		str += wNameT;
//		str += L"】";
//	}
//	str += wTextT;
//	str += L"\r\n";
//	SetBackLogText(str.c_str());
//
//
//	for (; viewLog.size() > Cl.Config->GetReviewMax();)
//	{
//		viewLog.pop_front();
//	}
//
//	//MessageBox(0, viewLog[viewLog.size()-1].TextT, 0, 0);
//	viewLogNum = 0;
//	
//	IsActive = true;
//	PostMessage(hWnds.Main, WM_PAINT, 0, 1);
//
//	// 임시 창 숨김 상태일때 클립보드 요청이 들어오면 창을 다시 띄운다
//	if (Cl.Config->GetTempWinHide())
//	{
//		PostMessage(hWnds.Main, WM_COMMAND, ID_TEMP_WINDOW_HIDE, 0);
//		PostMessage(hWnds.Main, WM_COMMAND, ID_SETTING_CHECK, 0);
//	}
//
//	return true;
//}

void CTextProcess::ApplyForceAneDicAll(std::wstring &input)
{
	std::wstring str = input;

	// 개별사전 우선적용
	for (unsigned int i = 0; i<AneDic.size(); i++)
	{
		str = replaceAll(str, AneDic[i].wjpn, AneDic[i].wkor);
	}

	input = str;
}

void CTextProcess::ApplyForceAneDic(std::wstring &input)
{
	std::wstring str = input;

	// 개별사전 우선적용
	for (unsigned int i = 0; i<AneDic.size(); i++)
	{
		if (AneDic[i].type == 2) str = replaceAll(str, AneDic[i].wjpn, AneDic[i].wkor);
	}

	input = str;
}

void *CTextProcess::_PatchUDic(const wchar_t *dicFile)
{
	FILE *fp;
	//char buf[1024];
	int nLine = 0;
	int nLen = 0;

	// JKDIC 구조체 초기화
	//JKDIC.clear();

	if (_wfopen_s(&fp, dicFile, L"rb") != 0)
	{
		MessageBox(0, L"UserDict.jk 파일이 없습니다", 0, 0);
		return false;
	}
	//	jkdicStruct jS;
	/*
	fseek(fp, 0, SEEK_END);
	int fLength = ftell(fp);
	fseek(fp, 0, SEEK_SET);*/
	fseek(fp, 0L, SEEK_END);
	int filesize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	void *m_loc = malloc(filesize);
	fread(m_loc, sizeof(char), filesize, fp);

	/*
	while(1)
	{
	nLine++;
	if (fread(buf, sizeof(char), 1, fp) == 0) break;
	if (buf[0] == 0x00) jS.hidden = false;
	else jS.hidden = true;
	if (fread(buf, sizeof(char), 31, fp) == 0) break;
	memcpy(jS.jpword_sjis, buf, 31);
	if (fread(buf, sizeof(char), 31, fp) == 0) break;
	memcpy(jS.krword_euckr, buf, 31);
	if (fread(buf, sizeof(char), 5, fp) == 0) break;
	memcpy(jS.part_of_speech, buf, 5);
	if (fread(buf, sizeof(char), 42, fp) == 0) break;
	memcpy(jS.attributes, buf, 42);

	JKDIC.push_back(jS);
	}
	*/
	fclose(fp);


	if (_wfopen_s(&fp, dicFile, L"wb") != 0)
	{
		MessageBox(0, L"UserDict.jk 파일에 쓰기 권한이 없습니다", 0, 0);
		return false;
	}

	for (unsigned int i = 0; i<AneDic.size(); i++)
	{
		if (strstr(AneDic[i].jpn, "?") != 0) continue;

		fwrite(&AneDic[i].hidden, sizeof(char), 1, fp);
		fwrite(&AneDic[i].jpn, sizeof(char), 31, fp);
		fwrite(&AneDic[i].kor, sizeof(char), 31, fp);
		fwrite(&AneDic[i].part, sizeof(char), 5, fp);
		fwrite(&AneDic[i].attr, sizeof(char), 42, fp);
	}

	fwrite(m_loc, sizeof(char), filesize, fp);

	/*
	for (unsigned int i=0; i<JKDIC.size(); i++)
	{
	fwrite(&JKDIC[i].hidden, sizeof(char), 1, fp);
	fwrite(&JKDIC[i].jpword_sjis, sizeof(char), 31, fp);
	fwrite(&JKDIC[i].krword_euckr, sizeof(char), 31, fp);
	fwrite(&JKDIC[i].part_of_speech, sizeof(char), 5, fp);
	fwrite(&JKDIC[i].attributes, sizeof(char), 42, fp);
	}*/
	fclose(fp);

	//WriteLog(L"[DatUserDictRead] %d개의 DAT 사용자 사전 \"UserDict.jk\"를 읽었습니다.\n", UserDict.size());

	OLDFILEINFO *ret = new OLDFILEINFO;

	ret->filesize = filesize;
	ret->m_loc = m_loc;

	return ret;
}
bool CTextProcess::_UnPatchUDic(const wchar_t *dicFile, void *param)
{
	FILE *fp;
	OLDFILEINFO *offile = (OLDFILEINFO *)param;
	int nLine = 0;
	int nLen = 0;

	//DeleteFile(g_J2K_UserDic);

	if (_wfopen_s(&fp, dicFile, L"wb") != 0)
	{
		MessageBox(0, L"UserDict.jk 파일에 쓰기 권한이 없습니다", 0, 0);
		return false;
	}

	fwrite(offile->m_loc, sizeof(char), offile->filesize, fp);
	/*
	for (unsigned int i=0; i<JKDIC.size(); i++)
	{
	fwrite(&JKDIC[i].hidden, sizeof(char), 1, fp);
	fwrite(&JKDIC[i].jpword_sjis, sizeof(char), 31, fp);
	fwrite(&JKDIC[i].krword_euckr, sizeof(char), 31, fp);
	fwrite(&JKDIC[i].part_of_speech, sizeof(char), 5, fp);
	fwrite(&JKDIC[i].attributes, sizeof(char), 42, fp);
	}*/
	fclose(fp);

	free(offile->m_loc);
	delete offile;
	return true;
}

bool CTextProcess::_LoadDic(const wchar_t *dicFile)
{
	FILE *fp;
	int nLine;
	wchar_t wstr[1024];
	wchar_t wjpn[1024];
	wchar_t wkor[1024];
	wchar_t wpart[1024];
	wchar_t wattr[1024];

	char jpn[31];
	char kor[31];
	char attr[42];

	// 사전 파일 읽기

	if (_wfopen_s(&fp, dicFile, L"rt,ccs=UTF-8") != 0)
	{
		//MessageBox(0, L"사용자 사전을 열 수 없습니다", 0, 0);
		return false;
	}
	nLine = 0;
	AneDic.clear();

	// 한줄씩 읽기
	while (fgetws(wstr, 1000, fp) != NULL)
	{
		int nLength = wcslen(wstr);
		int nBom = 0;
		nLine++;

		// 주석 처리
		if (wstr[0] == L'/' && wstr[1] == L'/')
		{
			//fwprintf(out, L"[주석]\n");
			continue;
		}

		memset(wjpn, 0, 1024);
		memset(wkor, 0, 1024);
		memset(wpart, 0, 1024);
		memset(wattr, 0, 1024);


		int tab = 0;

		for (int i = 0, prev = 0; i <= nLength; i++)
		{
			/*
			// 탭을 여러개 넣었을 떄의 처리
			if (i > 0 && wstr[i - 1] == L'\t' && wstr[i] == L'\t')
			{
				prev++;
				continue;
			}*/

			// 탭을 만나거나 EOF를 만나면
			if (wstr[i] == L'\t' || wstr[i] == L'\n' || (wstr[i - 1] == L'/' && wstr[i] == L'/') || i == nLength)
			{
				switch (tab)
				{
				case 0: // jpn
					wcsncpy_s(wjpn, wstr, i);
					tab++;
					prev = i + 1;
					break;

				case 1: // kor
					wcsncpy_s(wkor, wstr + prev, i - prev);
					tab++;
					prev = i + 1;
					break;

				case 2: // part
					wcsncpy_s(wpart, wstr + prev, i - prev);
					tab++;
					prev = i + 1;
					break;

				case 3: // attr
					wcsncpy_s(wattr, wstr + prev, i - prev);
					tab++;
					prev = i + 1;
					break;
				}

				// 주석을 만나면 종료
				if (wstr[i - 1] == L'/' && wstr[i] == L'/') break;

			}
		}

		// 탭이 1개 이상 없으면 옳바르지 않은 형식
		if (tab < 2) continue;

		// 유효성 검사
		if (wjpn[0] == '\0') continue;
		/*
		// 우선 적용 단어인 경우
		if (wpart[0] == L'2')
		{
			aneFDicStruct FDIC;
			FDIC.jpn = wjpn;
			FDIC.kor = wkor;
			AneFDic.push_back(FDIC);
			continue;
		}
		*/
		aneDicStruct DIC;

		memset(DIC.wjpn, 0, sizeof(wchar_t) * 31);
		memset(DIC.wkor, 0, sizeof(wchar_t) * 31);
		memset(DIC.jpn, 0, sizeof(char) * 31);
		memset(DIC.kor, 0, sizeof(char) * 31);
		memset(DIC.part, 0, sizeof(char) * 5);
		memset(DIC.attr, 0, sizeof(char) * 42);

		int n_jpword;
		int nLen;

		if ((n_jpword = WideCharToMultiByte(932, 0, wjpn, -1, NULL, NULL, NULL, NULL)) > 31)
		{
			//WriteLog(L"[UserDictRead] 오류 | 일어 단어f의 길이는 15자 (30Byte)를 넘을 수 없습니다. 해당 단어는 무시됩니다. 현재 길이 : %dByte\n", n_jpword);
			//WriteLog(L"[UserDictRead] 오류 | [%s:%d] : %s | %s | %s | %s\n", FindFileData.cFileName, nLine, wjpword_sjis, wkrword_euckr, wpart_of_speech, wattributes);
			continue;
		}
		WideCharToMultiByte(932, 0, wjpn, -1, jpn, n_jpword, NULL, NULL);

		if ((nLen = WideCharToMultiByte(949, 0, wkor, -1, NULL, NULL, NULL, NULL)) > 31)
		{
			//WriteLog(L"[UserDictRead] 오류 | 한글 단어의 길이는 15자 (30Byte)를 넘을 수 없습니다. 해당 단어는 무시됩니다. 현재 길이 : %dByte\n", nLen);
			//WriteLog(L"[UserDictRead] 오류 | [%s:%d] : %s | %s | %s | %s\n", FindFileData.cFileName, nLine, wjpword_sjis, wkrword_euckr, wpart_of_speech, wattributes);
			continue;
		}
		WideCharToMultiByte(949, 0, wkor, -1, kor, nLen, NULL, NULL);

		int part = 0;

		//WideCharToMultiByte_REAL(932,0,wpart_of_speech,-1,part_of_speech,5,NULL,NULL);

		if ((nLen = WideCharToMultiByte(CP_ACP, 0, wattr, -1, NULL, NULL, NULL, NULL)) > 42)
		{
			//WriteLog(L"[UserDictRead] 오류 | 단어 속성은 42Byte를 넘을 수 없습니다. 해당 단어는 무시됩니다. 현재 길이 : %dByte\n", nLen);
			//WriteLog(L"[UserDictRead] 오류 | [%s:%d] : %s | %s | %s | %s\n", FindFileData.cFileName, nLine, wjpword_sjis, wkrword_euckr, wpart_of_speech, wattributes);
			continue;
		}
		WideCharToMultiByte(CP_ACP, 0, wattr, -1, attr, nLen, NULL, NULL);

		DIC.line = nLine;
		DIC.hidden = 0x00;
		strncpy_s(DIC.jpn, jpn, 31);
		strncpy_s(DIC.kor, kor, 31);
		//strncpy(jS.part_of_speech, part_of_speech, 5);
		if (wpart[0] == L'0')
		{
			DIC.type = 0;
			strncpy_s(DIC.part, "A9D0", 5); // 상용어구 (0)
		}
		else if (wpart[0] == L'2')
		{
			DIC.type = 2;
			strncpy_s(DIC.part, "A9D0", 5); // 상용어구 (0)
		}
		else
		{
			DIC.type = 1;
			strncpy_s(DIC.part, "I110", 5); // 명사 (1 또는 입력없음)
		}
		strncpy_s(DIC.attr, attr, 42);

		wcscpy_s(DIC.wjpn, wjpn);
		wcscpy_s(DIC.wkor, wkor);
		AneDic.push_back(DIC);
	}

	sort(AneDic.begin(), AneDic.end());

	fclose(fp);
	return true;
}

bool CTextProcess::LoadDictionary()
{
	std::wstring AneDic;
	std::wstring DicJK;

	GetLoadPath(AneDic, L"\\AneDic.txt");
	GetEZTPath(DicJK, L"\\Dat\\UserDict.jk");

	//if (bLoadDic == true)
	//{
	//	PostMessage(hWnds.Main, WM_COMMAND, ID_LOAD_DICTIONARY, 0);
	//	return false;
	//}

	EnterCriticalSection(&cs_trans);
	//bLoadDic = true;
	if (!ehndSupport)
	{
		if (_LoadDic(AneDic.c_str()))
		{
			OLDFILEINFO *offile = (OLDFILEINFO *)_PatchUDic(DicJK.c_str());
			Cl.TransEngine->J2K_ReloadUserDict();
			_UnPatchUDic(DicJK.c_str(), offile);
		}
	}
	else
		Cl.TransEngine->J2K_ReloadUserDict();
	LeaveCriticalSection(&cs_trans);
	//bLoadDic = false;
	return true;
}

CTextProcess::~CTextProcess()
{
	// 클립보드 감시 종료
	//EndWatchClip();
	//EndHookMonitor();
	Cl.TextProcess = 0;
}
