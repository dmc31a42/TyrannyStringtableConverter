#include "dllExport.h"
#include "Anemone.h"
#include "wchar.h"
#include "string.h"

__declspec(dllexport) int fnWin32Project2(int value) {

	PrevMain(hInst);
	return value;
}

__declspec(dllexport) const wchar_t* Anemone_TranslateText(wchar_t* inputChar) {
	std::wstring::size_type nprev = 0;
	std::wstring::size_type npos = -1;
	std::wstring input = inputChar;
	std::list<std::wstring> list_org, list_trans, list;
	std::wstring output, trans_str;
	int i = 0, length = input.length();
	std::wstring empty = L"Abort";
	std::wstring temp_str;
	std::list<std::wstring>::iterator iter, iter_trans;
	int nStatus = 1;

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
	std::vector<int> v_line_info;

	if (list_org.size() > div)
	{
		std::wstring line;
		std::list<std::wstring> list2 = list_org;
		int j = 1;
		int k = 0;
		for (i = 1, iter = list2.begin(); iter != list2.end(); iter++, i++)
		{
			if ((float)i > (float)(list2.size() * j / div))
			{
				list.push_back(line);
				line = L"";
				j++;

				v_line_info.push_back(k);
				k = 0;
			}

			line += (*iter);
			k++;

			if (i == list2.size())
			{
				list.push_back(line);

				line = L"";
				v_line_info.push_back(k);
			}

		}
	}
	else
	{
		list = list_org;
	}

	for (i = 0, iter = list.begin(); iter != list.end() /*&& v_line_info.size() != 0*/; iter++, i++)
	{
		if (nStatus == 2)
		{
			nStatus = 0;
			MessageBox(NULL, L"텍스트 번역을 중단했습니다.", L"아네모네", MB_ICONINFORMATION);
			return L"";
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
					if ((*iter).find(L"\r\n"))
						output += L" \r\n|:_";
					else output += L" \n|:_";
				}
				else if (strRes.find(L"_|:_") != std::string::npos && trans_str.find(L"_|:_") == std::string::npos)
				{
					output += L" ";
				}
				else if (strRes.find(L"_|:_") != std::string::npos)
					output += strRes.substr(0, strRes.find(L"_|:_"));
				else
					output += trans_str;

			} while (npos != std::string::npos);
		}
		else output += trans_str;
	}

	nStatus = 0;

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
		nStatus = 0;
		MessageBox(NULL, L"번역 중 오류가 발생했습니다.", L"아네모네", MB_ICONERROR);
		return L"";
	}

	std::wstringstream outputstream;

	//for (iter = list_org.begin(), iter_trans = list_trans.begin(); iter != list_org.end(); iter++, iter_trans++)
	//{
	//	//if (FT->WriteType != 0)
	//	if (Cl.Config->GetFileTransOutput() != 0)
	//	{
	//		// 개행만 있는 라인 번역 안함 옵션 선택시 개행만 넣어줌
	//		//if (FT->NoTransLineFeed && (*iter_trans == L"\n" || *iter_trans == L"\r\n") || 
	//		if (Cl.Config->GetFileTransNoTransLineFeed() && (*iter_trans == L"\n" || *iter_trans == L"\r\n") ||
	//			std::next(iter, 1) == list_org.end() && *iter_trans == L"")
	//		{
	//			//fwrite((*iter).c_str(), sizeof(wchar_t), wcslen((*iter).c_str()), fpw);
	//			outputstream << *iter;
	//		}
	//		// 이 라인이 마지막 라인인 경우 원문에 \r\n을 붙여준다
	//		else if (std::next(iter, 1) == list_org.end())
	//		{
	//			//fwrite((*iter).c_str(), sizeof(wchar_t), wcslen((*iter).c_str()), fpw);
	//			//fwrite(L"\r\n", sizeof(wchar_t), wcslen(L"\r\n"), fpw);
	//			//fwrite((*iter_trans).c_str(), sizeof(wchar_t), wcslen((*iter_trans).c_str()), fpw);
	//			outputstream << *iter;
	//			outputstream << L"\r\n";
	//			outputstream << *iter_trans;
	//			break;
	//		}
	//		else
	//		{
	//			//fwrite((*iter).c_str(), sizeof(wchar_t), wcslen((*iter).c_str()), fpw);
	//			//fwrite((*iter_trans).c_str(), sizeof(wchar_t), wcslen((*iter_trans).c_str()), fpw);
	//			outputstream << *iter;
	//			outputstream << *iter_trans;

	//			// 출력 설정이 원문/번역문 + 개행인 경우 개행 처리
	//			//if (FT->WriteType == 2) fwrite(L"\r\n", sizeof(wchar_t), wcslen(L"\r\n"), fpw);
	//			if (Cl.Config->GetFileTransOutput() == 2)
	//				//fwrite(L"\r\n", sizeof(wchar_t), wcslen(L"\r\n"), fpw);
	//				outputstream << L"\r\n";
	//		}
	//	}
	//	else
	//		//fwrite((*iter_trans).c_str(), sizeof(wchar_t), wcslen((*iter_trans).c_str()), fpw);
	//		outputstream << *iter_trans;
	//}
	for (iter = list_org.begin(), iter_trans = list_trans.begin(); iter != list_org.end(); iter++, iter_trans++) {
		outputstream << *iter_trans;
	}
	std::wstring outputString = outputstream.str();
	wchar_t* outputChar = new wchar_t[outputString.size()];
	outputString.copy(outputChar, outputString.size());
	outputChar[outputString.size()] = L'\0';
	return outputChar;
	//return outputString.c_str();

}