// Note.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"
#include "Note.h"
#include "commctrl.h"
#include "stdio.h"
#include "commdlg.h"
#include <windows.h>
#include <fcntl.h>
#include <io.h>

#define MAX_LOADSTRING 100

#define SAVEAS 456
#define SAVE 123
#define OPEN 789
#define NEW 987
#define COPY ID_32791
#define PASTE ID_32793
#define CUT ID_33333
#define UNDO ID_32794


// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND ed, tree, butS, butSA, butO, butC, path, Toolbar;
LPRECT lr;
HFONT hfont, hfont1;
int dx, dy;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_NOTE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NOTE));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NOTE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1); 
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_NOTE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_NOTE));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	//InitCommonControlsEx(p);

   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   INITCOMMONCONTROLSEX picce;
   picce.dwSize = sizeof(INITCOMMONCONTROLSEX);
   picce.dwICC = ICC_WIN95_CLASSES;
   InitCommonControlsEx(&picce);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }


   hfont = CreateFontW(-13, 0, 0, 0, 500, 0, 0, 0, 0, 
	   OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, _T("Arial"));    // создание шрифта
    // создание контролов 
   ed = CreateWindowW(_T("EDIT"), _T(""), WS_CHILD|WS_VISIBLE|WS_BORDER|ES_MULTILINE|WS_HSCROLL|WS_VSCROLL, 200, 65, 500, 500, hWnd, (HMENU)1, hInstance, 0);
   SendMessage(ed, WM_SETFONT, (WPARAM)hfont, 0);
 
   path = CreateWindowW(_T("STATIC"), _T(""), WS_CHILD | WS_VISIBLE | ES_READONLY, 0, 45, 400, 20, hWnd, (HMENU)4, hInstance, 0);

   butS = CreateWindowW(_T("BUTTON"), _T("Сохранить"), WS_CHILD | WS_VISIBLE, 0, 65, 200, 20, hWnd, (HMENU) 123, hInstance, 0);
   butSA = CreateWindowW(_T("BUTTON"), _T("Сохранить как"), WS_CHILD | WS_VISIBLE, 0, 86, 200, 20, hWnd, (HMENU) 456, hInstance, 0);
   butO = CreateWindowW(_T("BUTTON"), _T("Открыть"), WS_CHILD | WS_VISIBLE, 0, 106, 200, 20, hWnd, (HMENU) 789, hInstance, 0);

   SendMessage(butS, WM_SETFONT, (WPARAM)hfont, 0);      // установка шрифта
   SendMessage(butSA, WM_SETFONT, (WPARAM)hfont, 0);
   SendMessage(butO, WM_SETFONT, (WPARAM)hfont, 0);
   SendMessage(path, WM_SETFONT, (WPARAM)hfont, 0);

   //***********************************************************************
   Toolbar = CreateWindowExW(0, TOOLBARCLASSNAME, NULL, WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 300, 25, hWnd, (HMENU)2, hInst, NULL); // создание тулбара
   HIMAGELIST g_hImageList = ImageList_Create(16, 16, ILC_COLOR16 | ILC_MASK, 10, 0);
   const int ImageListID = 0;
   SendMessage(Toolbar, TB_SETIMAGELIST, (WPARAM)ImageListID, (LPARAM)g_hImageList);
   SendMessage(Toolbar, TB_LOADIMAGES, (WPARAM)IDB_STD_SMALL_COLOR, (LPARAM)HINST_COMMCTRL);
   TBBUTTON tbButtons[10] =
   {// заполнение кнопок тулбара
	   { MAKELONG(STD_FILENEW, ImageListID), NEW, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"New"},
	   { MAKELONG(STD_FILEOPEN,  ImageListID), OPEN,  TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"Open" },
	   { MAKELONG(STD_FILESAVE, ImageListID), SAVE, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"Save"},
	   { MAKELONG(STD_FILESAVE, ImageListID), SAVEAS, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"SaveAs"},
	   { 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0L, 0},
	   { MAKELONG(STD_COPY,  ImageListID), COPY,  TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"Copy" },
	   { MAKELONG(STD_PASTE, ImageListID), PASTE, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"Paste"},
	   { MAKELONG(STD_CUT, ImageListID), CUT, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"Cut"},
	   { 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0L, 0},
	   { MAKELONG(STD_HELP, ImageListID), IDM_ABOUT, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"About"},
   };
   SendMessage(Toolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
   SendMessage(Toolbar, TB_ADDBUTTONS, (WPARAM)10, (LPARAM)&tbButtons); //установка кнопок
   SendMessage(Toolbar, TB_AUTOSIZE, 0, 0);
   ShowWindow(Toolbar, TRUE);
    //*************************************************************
   HKEY hKey;                                       // чтение из реестра
   WCHAR path[200] = _T("\0");
   DWORD reestr, sz = sizeof(int), type = REG_DWORD, type2 = REG_SZ, size = sizeof(path);
   int x = 0, x1 = 0, y = 0, y1 = 0;
   if (RegCreateKeyExW(HKEY_CURRENT_USER, TEXT("Software\\MyNote\\MyNoteSettings"), 0, NULL, 0, KEY_ALL_ACCESS,
	   NULL, &hKey, &reestr) == ERROR_SUCCESS)
   {
	   RegQueryValueExW(HKEY_CURRENT_USER, TEXT("L"), NULL, &type, (PBYTE)&x, &sz);
	   RegQueryValueExW(HKEY_CURRENT_USER, TEXT("T"), NULL, &type, (PBYTE)&y, &sz);
	   RegQueryValueExW(HKEY_CURRENT_USER, TEXT("R"), NULL, &type, (PBYTE)&x1, &sz);
	   RegQueryValueExW(HKEY_CURRENT_USER, TEXT("B"), NULL, &type, (PBYTE)&y1, &sz);

	   RegQueryValueExW(HKEY_CURRENT_USER, TEXT("Name"), NULL, &type2, (PBYTE)&path, &size);

	   RegCloseKey(hKey);
	   SetWindowPos(hWnd, 0, x, y, x1 - x, y1 - y, NULL);
   }
   FILE* f;
   f = _wfopen(path, _T("rb"));      // открытие файла прочитанного из реестра
   if (f) {
	   fseek(f, 0, SEEK_END);
	   int size = ftell(f);
	   fseek(f, 0, SEEK_SET);
	   WCHAR* buf = new WCHAR[size / 2 + 1];
	   fread(buf, 1, size, f);
	   buf[size / 2] = 0;
	   SetDlgItemTextW(hWnd, 1, buf);
	   SetDlgItemTextW(hWnd, 4, path);
	   delete[] buf;
	   fclose(f);
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
	case WM_CREATE:
	{
		RegisterHotKey(hWnd, NEW, MOD_CONTROL, 0x4E);
		RegisterHotKey(hWnd, OPEN, MOD_CONTROL, 0x4F);
		RegisterHotKey(hWnd, SAVE, MOD_CONTROL, 0x53);
		RegisterHotKey(hWnd, SAVEAS, MOD_CONTROL, 0x57);
		RegisterHotKey(hWnd, COPY, MOD_CONTROL, 0x43);
		RegisterHotKey(hWnd, PASTE, MOD_CONTROL, 0x56);
		RegisterHotKey(hWnd, CUT, MOD_CONTROL, 0x58);
		RegisterHotKey(hWnd, UNDO, MOD_CONTROL, 0x5A);

	}
	break;
	case WM_HOTKEY:
	{
		if (NEW == (int)wParam) { PostMessageW(hWnd, WM_COMMAND, NEW, 0); }
		if (OPEN == (int)wParam) { PostMessageW(hWnd, WM_COMMAND, OPEN, 0); }
		if (SAVE== (int)wParam) { PostMessageW(hWnd, WM_COMMAND, SAVE, 0); }
		if (SAVEAS == (int)wParam) { PostMessageW(hWnd, WM_COMMAND, SAVEAS, 0); }
		if (COPY == (int)wParam) { PostMessageW(hWnd, WM_COMMAND, COPY, 0); }
		if (PASTE == (int)wParam) { PostMessageW(hWnd, WM_COMMAND, PASTE, 0); }
		if (CUT == (int)wParam) { PostMessageW(hWnd, WM_COMMAND, CUT, 0); }
		if (UNDO == (int)wParam) { PostMessageW(hWnd, WM_COMMAND, UNDO, 0); }
	}
	break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
			case 123:                     //  клавиша "Соханить"
			{
				WCHAR path[300] = { NULL };
				GetDlgItemTextW(hWnd, 4, path, 300);
				if (path[0] != NULL) {
					FILE* f;
					int sz = GetWindowTextLength(ed);
					sz = sz * 2;
					WCHAR* buff = new WCHAR[sz];
					GetDlgItemTextW(hWnd, 1, buff, sz);
					f = _wfopen(path, _T("wb"));
					fwrite(buff, 1, sz, f);
					delete[] buff;
					fclose(f);

					MessageBox(hWnd, _T("Сохранено успешно"), _T("Сохранение..."), 0);
				}
				else {
					PostMessageW(hWnd, WM_COMMAND, 456, 0);
				}

				break;
			}
			case 456:                 // клавиша "Сохранить как"
			{
				TCHAR path[300] = _T("\0");
				OPENFILENAME filename;
				filename.lStructSize = sizeof(filename);
				filename.hwndOwner = hWnd;
				filename.lpstrFile = path;
				filename.lpstrFilter = _T("Text files(*.txt)\0*.txt\0\0");
				filename.nFilterIndex = 1;
				filename.nMaxFile = 512;
				filename.Flags =  OFN_OVERWRITEPROMPT | OFN_CREATEPROMPT;
				filename.lpstrDefExt = _T("txt");
				filename.hInstance = 0;
				filename.lpstrCustomFilter = NULL;
				filename.lpstrFileTitle = NULL;
				filename.nMaxFileTitle = 50;
				filename.lpstrInitialDir = NULL;
				filename.lpstrTitle = NULL;

				if (GetSaveFileName(&filename)) {

					FILE *f;
					int sz = GetWindowTextLength(ed);
					sz = sz * 2;
					WCHAR* buff = new WCHAR[sz];
					GetDlgItemTextW(hWnd, 1, buff, sz);
					f = _wfopen(path, _T("wb"));
					fwrite(buff, 1, sz, f);
					SetDlgItemTextW(hWnd, 4, path);
					delete[] buff;
					fclose(f);

					MessageBox(hWnd, _T("Сохранено успешно"), _T("Сохранение..."), 0);
					
				}
				break;
			}
			case 789:                 // клавиша Открыть
			{
				WCHAR path[300] = _T("\0");
				OPENFILENAME filename;
				filename.lStructSize = sizeof(filename);
				filename.hwndOwner = hWnd;
				filename.lpstrFile = path;
				filename.lpstrFilter = _T("Text files(*.txt)\0*.txt\0\0");
				filename.nFilterIndex = 1;
				filename.nMaxFile = 512;
				filename.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
				filename.lpstrDefExt = _T("txt");
				filename.hInstance = 0;
				filename.lpstrCustomFilter = NULL;
				filename.lpstrFileTitle = NULL;
				filename.nMaxFileTitle = 50;
				filename.lpstrInitialDir = _T("c:\\");
				filename.lpstrTitle = NULL;

				if (GetOpenFileName(&filename))
				{
					FILE *f;
					f = _wfopen(path, _T("rb"));
					fseek(f, 0, SEEK_END);
					int size = ftell(f);
					fseek(f, 0, SEEK_SET);
					WCHAR *buf = new WCHAR[size / 2 + 1];
					fread(buf, 1, size, f);
					buf[size / 2] = 0;
					SetDlgItemTextW(hWnd, 1, buf);
					SetDlgItemTextW(hWnd, 4, path);
					delete[] buf;
					fclose(f);
				}
				break;
			}
			case 987:       // клавиша Новый файл
			{
				WCHAR path[200];
				GetDlgItemTextW(hWnd, 4, path, 200);
				if (path != NULL) {
					int ans = MessageBox(hWnd, _T("Сохранить изменения в текущем файле перед закрытием?"), _T("Сохранять?"), MB_YESNOCANCEL);
					switch (ans) {
					case IDYES: {
						SendMessageW(hWnd, WM_COMMAND, 123, 0);
						WCHAR name[50] = _T("");
						SetDlgItemTextW(hWnd, 1, name);
						SetDlgItemTextW(hWnd, 4, name);
					}
					break;
					case IDNO: {
						WCHAR name[50] = _T("");
						SetDlgItemTextW(hWnd, 1, name);
						SetDlgItemTextW(hWnd, 4, name);
					}
					break;
					case IDCANCEL: {
						break;
					}
					}
				}
				

				break;
			}
			case ID_32791:
			{
				SendMessage(ed, WM_COPY, 0, 0);
			}
			break;
			case ID_33333:
			{
				//SendMessage(ed, WM_CUT, 0, 0);
				SendMessage(ed, WM_COPY, 0, 0);
				SendMessage(ed, WM_CLEAR, 0, 0);
			}
			break;
			case ID_32793:
			{
				if (IsClipboardFormatAvailable(CF_TEXT) != NULL) {
					SendMessage(ed, WM_PASTE, 0, 0);
				}
			}
			break;
			case ID_32794:
			{
				SendMessage(ed, EM_UNDO, 0, 0);
			}
			break;
            case IDM_EXIT:
			{
				DestroyWindow(hWnd);
				break;
			}
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_GETMINMAXINFO:
		{
			POINT a;
			a.x = 400;
			a.y = 200;
			tagMINMAXINFO *min = (LPMINMAXINFO)lParam;
			min->ptMinTrackSize = a;
		}
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_SIZE:
		{
			dx = LOWORD(lParam);
			dy = HIWORD(lParam);
			SetWindowPos(ed, 0, 200, 20, dx - 200, dy - 65, SWP_NOMOVE);
			SetWindowPos(path, 0, 0, 0, dx, 20, SWP_NOMOVE);
			SetWindowPos(Toolbar, 0, 0, 0, dx, 25, SWP_NOMOVE);
			break;
		}
    case WM_DESTROY:
		HKEY hKey;
		RECT c;
		
		DWORD reestr;
		GetWindowRect(hWnd, &c);
		if (RegCreateKeyExW(HKEY_CURRENT_USER, TEXT("Software\\MyNote\\MyNoteSettings"), 0, NULL, 0, KEY_ALL_ACCESS,
			NULL, &hKey, &reestr) == ERROR_SUCCESS)
		{
			RegSetValueExW(hKey, TEXT("L"), NULL, REG_DWORD, (PBYTE)&c.left, sizeof(c.left));     // запись в реестр 
			RegSetValueExW(hKey, TEXT("T"), NULL, REG_DWORD, (PBYTE)&c.top, sizeof(c.top));
			RegSetValueExW(hKey, TEXT("R"), NULL, REG_DWORD, (PBYTE)&c.right, sizeof(c.right));
			RegSetValueExW(hKey, TEXT("B"), NULL, REG_DWORD, (PBYTE)&c.bottom, sizeof(c.bottom));

			WCHAR* path = new WCHAR[200];
			GetDlgItemTextW(hWnd, 4, path, 400);
			RegSetValueEx(HKEY_CURRENT_USER, TEXT("Name"), NULL, REG_SZ, (LPBYTE)path, 200);
	
			delete[] path;
			RegCloseKey(hKey);
		}
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}