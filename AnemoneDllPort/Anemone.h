#pragma once
#include "stdafx.h"

extern HINSTANCE hInst;
int PrevMain(__in HINSTANCE hInstance);

void SetBackLogText(LPCWSTR Text);
void SetBackLogText(LPCWSTR Text, COLORREF crText, COLORREF crBackground);