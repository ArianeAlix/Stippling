#include "window.h"
#include <Windows.h>
#include <QApplication>

int main(int argv, char **args)
{	
	::ShowWindow(::GetConsoleWindow(), SW_HIDE); 
	// Pour ne pas afficher la console

	QApplication app(argv, args);
	app.setApplicationName("Stippling");
	MainWindow window;
	window.show();
	return app.exec();
}
