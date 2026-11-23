#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include <windows.h>
#include <wchar.h>
#include "calculator.h"

static Calculator calculator;
static HWND screen, hint;
static HFONT numberFont, buttonFont, smallFont;
static HBRUSH background, screenBackground;
static void refresh(void) {
    wchar_t text[80];
    if (calculator.error) wcscpy(text, calculator.error == 1 ? L"無法除以零" : L"超出計算範圍");
    else MultiByteToWideChar(CP_UTF8, 0, calculator.value, -1, text, 80);
    SetWindowTextW(screen, text);
    const wchar_t *operation = L"";
    switch (calculator.op) {
        case '+': operation = L"+"; break; case '-': operation = L"−"; break;
        case '*': operation = L"×"; break; case '/': operation = L"÷"; break;
    }
    SetWindowTextW(hint, operation);
}
static void input(char key) {
    if ((key >= '0' && key <= '9') || key == '.') calc_digit(&calculator, key);
    else if (key == '+' || key == '-' || key == '*' || key == '/') calc_operator(&calculator, key);
    else if (key == '=') calc_equals(&calculator);
    else if (key == 'c' || key == 'C') calc_clear(&calculator);
    refresh();
}
static void button(HWND parent, const wchar_t *label, char key, int x, int y, int width) {
    HWND b = CreateWindowW(L"BUTTON", label, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
        x, y, width, 56, parent, (HMENU)(INT_PTR)key, GetModuleHandleW(NULL), NULL);
    SendMessageW(b, WM_SETFONT, (WPARAM)buttonFont, TRUE);
}
static LRESULT CALLBACK window_proc(HWND window, UINT message, WPARAM w, LPARAM l) {
    switch (message) {
        case WM_CREATE: {
            calc_clear(&calculator);
            numberFont = CreateFontW(-32,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,0,L"Segoe UI");
            buttonFont = CreateFontW(-23,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,0,L"Segoe UI");
            smallFont = CreateFontW(-14,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,0,L"Microsoft JhengHei UI");
            hint = CreateWindowW(L"STATIC",L"",WS_CHILD|WS_VISIBLE|SS_RIGHT,22,18,300,24,window,NULL,NULL,NULL);
            screen = CreateWindowW(L"STATIC",L"0",WS_CHILD|WS_VISIBLE|SS_RIGHT|SS_CENTERIMAGE,22,42,300,66,window,NULL,NULL,NULL);
            SendMessageW(screen,WM_SETFONT,(WPARAM)numberFont,TRUE);
            SendMessageW(hint,WM_SETFONT,(WPARAM)buttonFont,TRUE);
            button(window,L"C",'C',22,126,222); button(window,L"÷",'/',256,126,66);
            const wchar_t *labels[] = {L"7",L"8",L"9",L"×",L"4",L"5",L"6",L"−",L"1",L"2",L"3",L"+"};
            const char keys[] = "789*456-123+";
            for (int i=0;i<12;i++) button(window,labels[i],keys[i],22+(i%4)*78,194+(i/4)*68,66);
            button(window,L"0",'0',22,398,144); button(window,L".",'.',178,398,66); button(window,L"=",'=',256,398,66);
            HWND help = CreateWindowW(L"STATIC",L"支援鍵盤輸入 · Esc 清除",WS_CHILD|WS_VISIBLE|SS_CENTER,22,470,300,24,window,NULL,NULL,NULL);
            SendMessageW(help,WM_SETFONT,(WPARAM)smallFont,TRUE);
            return 0;
        }
        case WM_COMMAND:
            if (HIWORD(w) == BN_CLICKED) { input((char)LOWORD(w)); SetFocus(window); }
            return 0;
        case WM_CTLCOLORSTATIC: {
            HDC dc=(HDC)w; SetTextColor(dc,RGB(41,60,54));
            if ((HWND)l == screen || (HWND)l == hint) { SetBkColor(dc,RGB(237,241,231)); return (LRESULT)screenBackground; }
            SetBkColor(dc,RGB(244,243,238)); return (LRESULT)background;
        }
        case WM_DESTROY:
            DeleteObject(numberFont); DeleteObject(buttonFont); DeleteObject(smallFont);
            PostQuitMessage(0); return 0;
    }
    return DefWindowProcW(window,message,w,l);
}
int WINAPI wWinMain(HINSTANCE instance,HINSTANCE previous,PWSTR command,int show) {
    (void)previous; (void)command;
    SetProcessDPIAware();
    background=CreateSolidBrush(RGB(244,243,238));
    screenBackground=CreateSolidBrush(RGB(237,241,231));
    WNDCLASSW wc={0}; wc.lpfnWndProc=window_proc; wc.hInstance=instance;
    wc.lpszClassName=L"SimpleCalculatorWindow"; wc.hCursor=LoadCursorW(NULL,IDC_ARROW);
    wc.hIcon=LoadIconW(NULL,IDI_APPLICATION); wc.hbrBackground=background;
    if (!RegisterClassW(&wc)) return 1;
    DWORD style=WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;
    RECT rect={0,0,344,510}; AdjustWindowRect(&rect,style,FALSE);
    HWND window=CreateWindowW(wc.lpszClassName,L"簡易計算機",style,CW_USEDEFAULT,CW_USEDEFAULT,
        rect.right-rect.left,rect.bottom-rect.top,NULL,NULL,instance,NULL);
    if (!window) return 1;
    ShowWindow(window,show); UpdateWindow(window);
    MSG msg;
    while (GetMessageW(&msg,NULL,0,0)>0) {
        if (msg.message == WM_KEYDOWN && (msg.wParam == VK_RETURN || msg.wParam == VK_ESCAPE)) {
            input(msg.wParam == VK_RETURN ? '=' : 'C'); continue;
        }
        if (msg.message == WM_CHAR) {
            wchar_t key=(wchar_t)msg.wParam;
            if ((key>=L'0' && key<=L'9') || wcschr(L".+-*/=cC",key)) { input((char)key); continue; }
        }
        TranslateMessage(&msg); DispatchMessageW(&msg);
    }
    DeleteObject(background); DeleteObject(screenBackground);
    return (int)msg.wParam;
}
