#pragma once

extern "C"
{
	__declspec(dllexport) int fnWin32Project2(int value);
	__declspec(dllexport) const wchar_t* Anemone_TranslateText(wchar_t* inputChar);
}