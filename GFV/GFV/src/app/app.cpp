#include "app.h"
#include "graphics/drawable/image/imagedrawable.h"
#include "resource.h"

#define ID_MENU_FILE_OPEN 2444
#define ID_MENU_FILE_EXIT 2445
#define ID_MENU_FILE_DETAILS 2446
#define ID_MENU_HELP_ABOUT 2447
// TODO: HACER QUE UNA IMAGEN ESTE SIEMPRE CENTRADO (ver referencia en imagedrawable.cpp)
/*
...Configuracion...
init()
*/

ImageDrawable* img = nullptr;

bool isGifFile(const wchar_t* filePath);

App::App() : WindowAdapter(RectI(CW_USEDEFAULT, CW_USEDEFAULT, 800, 400), L"App")
{}

void App::OnCreate()
{
	HMENU menu,submenu;
	menu = CreateMenu();
	submenu = CreatePopupMenu();

	AppendMenu(submenu, MF_STRING, ID_MENU_FILE_OPEN, L"Abrir");
	AppendMenu(submenu, MF_STRING, ID_MENU_FILE_DETAILS, L"Detalles");
	AppendMenu(submenu, MF_STRING, ID_MENU_FILE_EXIT, L"Salir");
	AppendMenu(menu, MF_STRING | MF_POPUP, (UINT)submenu, L"Archivo");

	submenu = CreatePopupMenu();

	AppendMenu(submenu, MF_STRING, ID_MENU_HELP_ABOUT, L"Acerca");
	AppendMenu(menu, MF_STRING | MF_POPUP,(UINT)submenu, L"Ayuda");
	SetMenu(m_hwnd, menu);

	m_viewer.init(m_hwnd);

	img = new ImageDrawable(MAKEINTRESOURCE(IDR_IMAGE1), IDRAW_TYPE_LOAD::FILEPATH);

}

void App::Start(const wchar_t* filePath)
{
	if (isGifFile(filePath))
	{
		//m_viewer.Start(filePath);
		//RedrawWindow(m_hwnd, NULL, NULL, RDW_ALLCHILDREN | RDW_FRAME | RDW_INTERNALPAINT | RDW_UPDATENOW);
		//InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

bool isGifFile(const wchar_t* filePath)
{
	std::wstring copy = filePath;
	try
	{
		copy = copy.substr(copy.find_last_of('.'));
	}
	catch (std::out_of_range)
	{
		std::cout << "Usted dropeó algo que NO es un archivo" << std::endl;
	}

	return !copy.compare(L".gif");
}

void App::OnCommandExecute(const UINT& id)
{
	switch (id)
	{
	case ID_MENU_FILE_EXIT:
		PostQuitMessage(0);
		break;
	case ID_MENU_FILE_OPEN:
	{
		wchar_t filePath[MAX_PATH];

		SecureZeroMemory(filePath, ARRAYSIZE(filePath));

		OPENFILENAME ofn;
		RtlZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hwnd;
		ofn.lpstrTitle = L"Selecciona el archivo para seleccionar...";
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFilter = L"*.gif\0*.gif\0";
		ofn.lpstrFile = filePath;
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

		if (GetOpenFileName(&ofn))
		{
			SecureZeroMemory(&ofn, sizeof(OPENFILENAME));
			this->Start(filePath);
		}
	}
		break;
	}
}

void App::OnPaint()
{
	m_graphics->Clear(D2D1::ColorF(D2D1::ColorF::Red));
	m_graphics->Draw(img);
}

void App::OnResize()
{
	m_viewer.SetPredefinedPosition(PredefinedPosition::CenterMedium);
}