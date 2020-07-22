#pragma once
#include "widgets/base/childwindow.h"
#include "graphics/drawable/image/imagedrawable.h"

class Viewer : public ChildWindow
{
public:
	Viewer() : ChildWindow(RectI(0, 0, 300, 300), L"Viewer"), m_img(nullptr) {};

	void Start(const wchar_t* filePath)
	{
		if (!m_img)
		{
			//m_img = new ImageDrawable(filePath,IDRAW_FILEPATH);
		}
		else
		{
			//m_imgfile->ChangeFile(filePath);
		}

		RECT clientParentRect;
		GetClientRect(m_hwndParent, &clientParentRect);

		SizeI sizeParent(clientParentRect.right - clientParentRect.left, clientParentRect.bottom - clientParentRect.top);

		//SetSize(m_imgfile->GetSize());
		SetPosition(PointI(sizeParent.width / 2 - m_bounds.weidth / 2, sizeParent.height / 2 - m_bounds.height / 2));
	}

	void OnPaint()
	{
		m_graphics->Clear(D2D1::ColorF(D2D1::ColorF::Blue));

		RectF rc(0,0,m_bounds.weidth,m_bounds.height);

		//m_graphics->Draw(m_imgfile);

		m_graphics->DrawRectangle(rc, D2D1::ColorF(D2D1::ColorF::Black));
	
	}
private:
	ImageDrawable* m_img;

};

