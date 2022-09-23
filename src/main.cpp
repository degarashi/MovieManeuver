#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
	w.setFixedSize(640, 440);
	w.show();

	//QObject::connect(&test, SLOT())
	// HWND hw = reinterpret_cast<HWND>(w.winId());
	// SetWindowPos(hw, nullptr, 0, 0, 0, 0, SWP_NOSIZE);
    return a.exec();
}
