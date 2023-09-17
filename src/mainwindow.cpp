#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "manip_mgr.hpp"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  _ui(new Ui::MainWindow)
{
	_ui->setupUi(this);
	_manipMgr = new dg::Manip_Mgr(this);

	// インプット手段のリセットアクション
	connect(_ui->actionRefresh_Input_r, &QAction::triggered,
			_manipMgr, &dg::Manip_Mgr::initInputs);

	// InputMgrが変更された時、Widgetに反映
	connect(_manipMgr, &dg::Manip_Mgr::onWidgetCreated,
			this, &MainWindow::onInputWidgetCreated);
	// Manipulatorが変更された時、Widgetに伝える
	connect(_manipMgr, &dg::Manip_Mgr::onManipChanged,
			this, &MainWindow::onManipChanged);

	_manipMgr->initInputs();
}

void MainWindow::onInputWidgetCreated(QWidget* widget) {
	_ui->verticalLayout->addWidget(widget, 1);
}
void MainWindow::onManipChanged(const QString& name) {
	_ui->lbWindowName->setText(name + " Running...");
}
