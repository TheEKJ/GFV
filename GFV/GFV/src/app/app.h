#pragma once
#include "os/win/winadapter.h"
#include "widgets/childwindow.h"

class App : public WindowAdapter
{
public:
	App();

	void OnCreate();
	void OnPaint();

	/// <summary>
	/// Añade componentes a la aplicacion
	/// </summary>
	/// <param name="child">Debe ser un componente PUNTERO, NO UNO NORMAL</param>
	void addComponent(ChildWindow* child);
	void deleteComponents();
private:
	std::vector<std::unique_ptr<ChildWindow>> m_components;
};