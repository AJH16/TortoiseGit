// TGitInstaller.cpp : Definiert den Einstiegspunkt f�r die Anwendung.
//

#include "stdafx.h"
#include "TGitInstaller.h"
#include <propsys.h>
#include <PropKey.h>

#include "UpdateDownloader.h"
#include "Win7.h"

#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")
#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 100

#define SIGNATURE_FILE_ENDING _T(".rsa.asc")

#define WM_USER_DISPLAYSTATUS	(WM_USER + 1)
#define WM_USER_ENDDOWNLOAD		(WM_USER + 2)
#define WM_USER_FILLCHANGELOG	(WM_USER + 3)

// Globale Variablen:
HINSTANCE hInst;								// Aktuelle Instanz
TCHAR szTitle[MAX_LOADSTRING];					// Titelleistentext
TCHAR szWindowClass[MAX_LOADSTRING];			// Klassenname des Hauptfensters

// Vorw�rtsdeklarationen der in diesem Codemodul enthaltenen Funktionen:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	SetDllDirectory(L"");

	InitCommonControls();

 	// TODO: Hier Code einf�gen.
	MSG msg;
	HACCEL hAccelTable;

	// Globale Zeichenfolgen initialisieren
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TGITINSTALLER, szWindowClass, MAX_LOADSTRING);
	//MyRegisterClass(hInstance);

	// Anwendungsinitialisierung ausf�hren:
	/*if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}*/
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_CHECKFORUPDATES), nullptr, About, 0);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TGITINSTALLER));

	// Hauptnachrichtenschleife:
	/*while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}*/

	return 0;
}

void MarkWindowAsUnpinnable(HWND hWnd)
{
	typedef HRESULT(WINAPI *SHGPSFW) (HWND hwnd, REFIID riid, void** ppv);

	HMODULE hShell = AtlLoadSystemLibraryUsingFullPath(_T("Shell32.dll"));

	if (hShell) {
		SHGPSFW pfnSHGPSFW = (SHGPSFW)::GetProcAddress(hShell, "SHGetPropertyStoreForWindow");
		if (pfnSHGPSFW) {
			IPropertyStore *pps;
			HRESULT hr = pfnSHGPSFW(hWnd, IID_PPV_ARGS(&pps));
			if (SUCCEEDED(hr)) {
				PROPVARIANT var;
				var.vt = VT_BOOL;
				var.boolVal = VARIANT_TRUE;
				pps->SetValue(PKEY_AppUserModel_PreventPinning, var);
				pps->Release();
			}
		}
		FreeLibrary(hShell);
	}
}

//
//  FUNKTION: MyRegisterClass()
//
//  ZWECK: Registriert die Fensterklasse.
//
/*ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TGIT));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_TGIT));

	return RegisterClassEx(&wcex);
}*/

//
//   FUNKTION: InitInstance(HINSTANCE, int)
//
//   ZWECK: Speichert das Instanzenhandle und erstellt das Hauptfenster.
//
//   KOMMENTARE:
//
//        In dieser Funktion wird das Instanzenhandle in einer globalen Variablen gespeichert, und das
//        Hauptprogrammfenster wird erstellt und angezeigt.
//
/*BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Instanzenhandle in der globalen Variablen speichern

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}*/

//
//  FUNKTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ZWECK:  Verarbeitet Meldungen vom Hauptfenster.
//
//  WM_COMMAND	- Verarbeiten des Anwendungsmen�s
//  WM_PAINT	- Zeichnen des Hauptfensters
//  WM_DESTROY	- Beenden-Meldung anzeigen und zur�ckgeben
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Men�auswahl bearbeiten:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		/*case IDM_EXIT:
			DestroyWindow(hWnd);
			break;*/
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Hier den Zeichnungscode hinzuf�gen.
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

DWORD WINAPI CheckThread(LPVOID lpParameter)
{
	HWND hDlg = (HWND)lpParameter;

	CString sCheckURL = _T("https://versioncheck.tortoisegit.org/version.txt");

	CAutoConfig versioncheck(true);

	/*Sleep(2500);
	HWND listview = GetDlgItem(hDlg, IDC_LIST_DOWNLOADS);
	ShowWindow(listview, SW_SHOW);*/
	return 0;
}

// Meldungshandler f�r Infofeld.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		MarkWindowAsUnpinnable(hDlg);
		HWND listview = GetDlgItem(hDlg, IDC_LIST_DOWNLOADS);
		ListView_SetExtendedListViewStyle(listview, LVS_EX_DOUBLEBUFFER | LVS_EX_CHECKBOXES);
		LVCOLUMN column = { 0 };
		column.cx = 350;
		column.mask = LVCF_WIDTH;
		column.iOrder = 0;
		ListView_InsertColumn(listview, 0, &column);
		column.cx = 200;
		column.iOrder = 1;
		ListView_InsertColumn(listview, 1, &column);
		LVITEM listItem = { 0 };
		listItem.mask = LVIF_TEXT;
		listItem.iItem = 0;
		listItem.iSubItem = 0;
		listItem.pszText = L"TortoiseGit";
		ListView_InsertItem(listview, &listItem);
		listItem.mask |= LVCF_SUBITEM;
		listItem.iSubItem = 1;
		listItem.pszText = L"de";
		ListView_SetItem(listview, &listItem);
		ShowWindow(listview, SW_HIDE);

		// hide download controls
		/*m_ctrlFiles.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GROUP_DOWNLOADS)->ShowWindow(SW_HIDE);
		RECT rectWindow, rectGroupDownloads, rectOKButton;
		GetWindowRect(&rectWindow);
		GetDlgItem(IDC_GROUP_DOWNLOADS)->GetWindowRect(&rectGroupDownloads);
		GetDlgItem(IDOK)->GetWindowRect(&rectOKButton);
		LONG bottomDistance = rectWindow.bottom - rectOKButton.bottom;
		OffsetRect(&rectOKButton, 0, rectGroupDownloads.top - rectOKButton.top);
		rectWindow.bottom = rectOKButton.bottom + bottomDistance;
		MoveWindow(&rectWindow);
		::MapWindowPoints(NULL, GetSafeHwnd(), (LPPOINT)&rectOKButton, 2);
		GetDlgItem(IDOK)->MoveWindow(&rectOKButton);*/

		CAutoGeneralHandle m_eventStop = CreateEvent(nullptr, TRUE, FALSE, nullptr);
		auto m_updateDownloader = new CUpdateDownloader(hDlg, FALSE, WM_USER_DISPLAYSTATUS, m_eventStop);

		//https://msdn.microsoft.com/en-us/library/windows/desktop/ms682516%28v=vs.85%29.aspx
		CAutoGeneralHandle thread = CreateThread(nullptr, 0, CheckThread, hDlg, 0, nullptr);

	}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_USER_DISPLAYSTATUS:
		break;
	}

	return (INT_PTR)FALSE;
}

// https://stackoverflow.com/questions/10282964/how-can-i-update-a-progress-bar
//InsertColumn