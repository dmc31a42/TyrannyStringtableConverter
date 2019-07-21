#include "dllExport.h"
#include "Anemone.h"

__declspec(dllexport) int fnWin32Project2(int value) {

	PrevMain(hInst);
	return value;
}