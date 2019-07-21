// ���� ���� �Լ��� �־�δ� ���̺귯�� ����
#include "stdafx.h"
#include "Library.h"

void GetLoadPath(std::wstring &dir)
{
	wchar_t p[255];
	GetModuleFileNameW(hInst, p, 255);
	std::wstring f = p;
	dir = f.substr(0, f.rfind(L'\\'));
}

void GetLoadPath(std::wstring &dir, std::wstring path)
{
	GetLoadPath(dir);
	dir += path;
}

bool GetEZTPath(std::wstring &dir)
{
	if (GetEZTPathFromINI(dir)) return true;
	if (GetEZTPathFromREG(dir)) return true;
	return false;
}

bool GetEZTPath(std::wstring &dir, std::wstring path)
{
	if (GetEZTPath(dir))
	{
		dir += path;
		return true;
	}
	return false;
}

// ������Ʈ������ ����Ʈ���� ��θ� ���ɴϴ�
bool GetEZTPathFromREG(std::wstring &dir)
{
	wchar_t szPath[255];
	HKEY key;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 255;

	RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\ChangShin\\ezTrans", 0, KEY_READ, &key);

	if (RegQueryValueEx(key, L"FilePath", 0, &dwType, (LPBYTE)szPath, &dwSize) != ERROR_SUCCESS) return false;

	dir = szPath;
	return true;
}

// �������� ����Ʈ���� ��θ� ���ɴϴ�
bool GetEZTPathFromINI(std::wstring &dir)
{
	if (Cl.Config->GetEzTransPath()[0] != 0x00)
	{
		FILE *fp;
		std::wstring path = Cl.Config->GetEzTransPath();
		path += L"\\J2KEngine.dll";
		if (_wfopen_s(&fp, path.c_str(), L"rb") != 0)
		{
			//MessageBox(0, L"����Ʈ���� DLL�� ã�� �� �����ϴ�", 0, 0);
			return false;
		}
		fclose(fp);

		dir = Cl.Config->GetEzTransPath();

		return true;
	}

	return false;
}

bool ReadINI_Str(const wchar_t *key, const wchar_t *section, wchar_t *buf, wchar_t *file)
{
	int n = GetPrivateProfileString(section, key, NULL, buf, 255, file);
	if (n == 0) {
		//WriteLog(L"������ �ҷ����� | ���ǵ��� ���� �μ� | %ls\n", key);
		return false;
	}
	//WriteLog(L"%ls = \"%ls\"\n", key, buf);
	return true;
}


bool WriteINI_Str(const wchar_t *key, const wchar_t *section, wchar_t *buf, wchar_t *file)
{
	int n = WritePrivateProfileString(section, key, buf, file);
	if (n == 0) {
		//WriteLog(L"������ �ҷ����� | ���ǵ��� ���� �μ� | %ls\n", key);
		return false;
	}
	//WriteLog(L"%ls = \"%ls\"\n", key, buf);
	return true;
}

/* UCS2toUTF8
* converts UCS2 array to UTF8 string
*/
int UCS2toUTF8(char *dst, wchar_t *ucs2, int count)
{
	unsigned short unicode;
	unsigned char bytes[4];
	int nbytes;
	int i, j;
	int len = 0;

	for (i = 0; i<count; i++) {
		unicode = ucs2[i];
		if (unicode < 0x80) {
			nbytes = 1;
			bytes[0] = (unsigned char)unicode;
		}
		else if (unicode < 0x800) {
			nbytes = 2;
			bytes[1] = (unicode & 0x3f) | 0x80;
			bytes[0] = ((unicode << 2) & 0xcf00 | 0xc000) >> 8;
		}
		else {
			nbytes = 3;
			bytes[2] = (unicode & 0x3f) | 0x80;
			bytes[1] = ((unicode << 2) & 0x3f00 | 0x8000) >> 8;
			bytes[0] = ((unicode << 4) & 0x3f0000 | 0xe00000) >> 16;
		}

		for (j = 0; j<nbytes; j++) {
			dst[len] = bytes[j];
			len++;
		}
	}
	dst[len] = '\0';
	return len;
}

/* UTF8toUCS2
* converts UTF8 string to UCS2 array
*/
int UTF8toUCS2(wchar_t *ucs2, char *src)
{
	unsigned char c;
	unsigned short unicode;
	int count = 0;
	int len;
	int i;

	len = strlen(src);
	for (i = 0; i<len;) {
		c = src[i] & 0xe0;
		if (c < 0x80) {
			unicode = (unsigned short)src[i];
			i++;
		}
		else if (c < 0xe0) {
			unicode = (unsigned short)src[i] & 0x1f;
			i++;
			unicode = unicode << 6;
			unicode = unicode | ((unsigned short)src[i] & 0x3f);
			i++;
		}
		else if (c < 0xf0) {
			unicode = (unsigned short)src[i] & 0x0f;
			i++;
			unicode = unicode << 6;
			unicode = unicode | ((unsigned short)src[i] & 0x3f);
			i++;
			unicode = unicode << 6;
			unicode = unicode | ((unsigned short)src[i] & 0x3f);
			i++;
		}
		ucs2[count] = unicode;
		count++;
	}

	return count;
}

std::wstring replaceAll(const std::wstring &str, const std::wstring &pattern, const std::wstring &replace)
{
	std::wstring result = str;
	std::wstring::size_type pos = 0;
	std::wstring::size_type offset = 0;

	while ((pos = result.find(pattern, offset)) != std::string::npos)
	{
		result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);
		offset = pos + replace.size();
	}

	return result;
}

static BOOL CALLBACK OnGetWindowByProcess(HWND hwnd, LPARAM lParam)
{
	WINDOWPROCESSINFO *infoPtr = (WINDOWPROCESSINFO *)lParam;
	DWORD check = 0;
	BOOL br = TRUE;
	GetWindowThreadProcessId(hwnd, &check);
	//WriteLog(L"%x %x %x\n", hwnd, check, infoPtr->pid);
	if (check == infoPtr->pid)
	{
		//WriteLog(L"found window %x for process id %x\n", hwnd, check);
		infoPtr->hwnd = hwnd;
		br = FALSE;
	}
	return br;
}

void ExecuteFile(const std::wstring &filename)
{
	SHELLEXECUTEINFO sxi = { 0 };
	sxi.cbSize = sizeof(sxi);
	sxi.nShow = SW_SHOWNORMAL;

	sxi.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_NOASYNC | SEE_MASK_WAITFORINPUTIDLE;
	sxi.lpVerb = L"open";
	sxi.lpFile = filename.c_str();
	if (!ShellExecuteEx(&sxi))
		//WriteLog(L"ShellExecuteEx: %d\n", GetLastError());
	{
	}
	else
	{
		WINDOWPROCESSINFO info;
		info.pid = GetProcessId(sxi.hProcess); // SPECIFY PID
		info.hwnd = 0;
		AllowSetForegroundWindow(info.pid);
		EnumWindows(OnGetWindowByProcess, (LPARAM)&info);
		if (info.hwnd != 0)
		{
			SetForegroundWindow(info.hwnd);
			SetActiveWindow(info.hwnd);
		}
		CloseHandle(sxi.hProcess);

	}
}

//bool ColorDialog(HWND hWnd, CHOOSECOLOR &cc, DWORD ColorVar)
//{
//	static COLORREF acrCustClr[16]; // array of custom colors
//	wchar_t w_rgbZero[2] = L"0";
//
//	int alpha, c1, c2, c3;
//
//	alpha = (ColorVar >> 24) & 0xFF;
//	c1 = (ColorVar >> 16) & 0xFF;
//	c2 = (ColorVar >> 8) & 0xFF;
//	c3 = (ColorVar)& 0xFF;
//
//	COLORREF rgbCurrent = ((COLORREF)(((BYTE)(c1) | (((BYTE)(c2)) << 8)) | (((BYTE)(c3)) << 16)));
//
//	// Initialize CHOOSECOLOR 
//	ZeroMemory(&cc, sizeof(cc));
//	cc.lStructSize = sizeof(cc);
//	cc.hwndOwner = hWnd;
//	cc.lpCustColors = (LPDWORD)acrCustClr;
//	cc.rgbResult = rgbCurrent;
//	cc.lCustData = alpha;
//	cc.Flags = CC_FULLOPEN | CC_RGBINIT | CC_ENABLEHOOK;
//	cc.lpfnHook = CCHookProc;
//
//	if (ChooseColor(&cc) == TRUE)
//	{
//		return true;
//	}
//	else return false;
//}
//
//bool FontDialog(HWND hWnd, CHOOSEFONT &cf, LOGFONT &lf)
//{
//	ZeroMemory(&cf, sizeof(cf));
//	cf.lStructSize = sizeof(cf);
//	cf.hwndOwner = hWnd;
//	cf.hDC = (HDC)NULL;
//	cf.lpLogFont = &lf;
//	cf.iPointSize = 10;
//	cf.Flags = CF_SCREENFONTS | CF_NOVERTFONTS | CF_INITTOLOGFONTSTRUCT | CF_NOSCRIPTSEL | CF_ENABLEHOOK;
//	cf.rgbColors = RGB(0, 0, 0);
//	cf.lCustData = 0L;
//	cf.lpfnHook = CFHookProc;
//	cf.hInstance = (HINSTANCE)NULL;
//	cf.nFontType = SCREEN_FONTTYPE;
//	cf.nSizeMin = 0;
//	cf.nSizeMax = 0;
//
//	if (ChooseFont(&cf) == TRUE)
//	{
//		FontFamily fontFamily(lf.lfFaceName);
//		if (!fontFamily.IsAvailable())
//		{
//			MessageBox(hWnd, L"����� �� ���� �۲��Դϴ�.", 0, MB_ICONERROR);
//			return false;
//		}
//		return true;
//	}
//	return false;
//}