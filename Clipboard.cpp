// MIT License
//
// Copyright (c) 2025 Clipboard - xmc0211 <xmc0211@qq.com>xmc0211
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Clipboard.h"
#include <tchar.h>

BOOL ReadClipboardData(std::_tstring* lpData) {
    HANDLE hData;
    TCHAR* pGlobal;
    BOOL bRes = FALSE;
    if (lpData == NULL) return FALSE;
    if (!OpenClipboard(NULL)) return FALSE;
    if (!IsClipboardFormatAvailable(_TCFTEXT)) {
        *lpData = TEXT("");
        return TRUE;
    }
    hData = GetClipboardData(_TCFTEXT);
    if (hData == NULL) goto EXIT;
    pGlobal = static_cast<TCHAR*>(GlobalLock(hData));
    if (pGlobal == NULL) goto EXIT;
    *lpData = pGlobal;
    bRes = TRUE;
EXIT:
    if (hData) GlobalUnlock(hData);
    CloseClipboard();
    return bRes;
}

BOOL WriteClipboardData(std::_tstring Data) {
    BOOL bRes = FALSE;
    TCHAR* pGlobal = NULL;
    HANDLE hRes;
    if (!OpenClipboard(NULL)) return FALSE;
    EmptyClipboard();
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (Data.size() + 1) * sizeof(TCHAR));
    if (!hGlobal) goto EXIT;
    pGlobal = static_cast<TCHAR*>(GlobalLock(hGlobal));
    if (!pGlobal) goto EXIT;
    _tcscpy_s(pGlobal, Data.size() + 1, Data.c_str());
    hRes = SetClipboardData(_TCFTEXT, hGlobal);
    if (!hRes) goto EXIT;
    bRes = TRUE;
EXIT:
    CloseClipboard();
    if (pGlobal) GlobalUnlock(pGlobal);
    if (hGlobal) GlobalFree(hGlobal);
    return bRes;
}