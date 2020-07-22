#include "global.h"
#include "app/app.h"

#include <iostream>
#include <locale.h>
#include <thread>

using namespace std;

IWICImagingFactory* global_WICImagingFactory;
IDWriteFactory* global_IDWriteFactory;

int main()
{
	setlocale(LC_ALL, "");

	CoInitialize(nullptr);

	assert(SUCCEEDED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&global_WICImagingFactory))));

	assert(SUCCEEDED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(global_IDWriteFactory),reinterpret_cast<IUnknown**>(&global_IDWriteFactory))));

	App window;
	assert(window.init());
	
	/*
	POINT p;
	thread t([&]()->void
		{
			once_flag flag;

			while (true)
			{
				//system("cls");
				call_once(flag, []()->void {cout << "Call once" << endl; });
				GetCursorPos(&p);
				//ScreenToClient(window.GetWindowHandle(), &p);
				cout << p.x << " : " << p.y << endl;
				cout << window.GetBounds().x << " : " << window.GetBounds().y << " : " << window.GetBounds().weidth << " : " << window.GetBounds().height << endl;
				Sleep(5000);
			}
		});
	t.detach();*/

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	CoUninitialize();
	return 0;
}