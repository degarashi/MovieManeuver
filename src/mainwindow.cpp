#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "inputmgr_wii.hpp"
#include "inputmgr_xinput.hpp"
#include "manip_udemy.hpp"
#include "manip_youtube.hpp"
#include "manip_box.hpp"
#include "manip_vlc.hpp"

#include <QThread>
#include <QTimer>
#include <QCheckBox>
#include <QProgressBar>

namespace {
	const dg::Manip_YouTube c_manipYT;
	const dg::Manip_Udemy c_manipUD;
	const dg::Manip_Box c_manipBox;
	const dg::Manip_VLC c_manipVLC;

	const dg::Manip* c_manipList[] = {
		&c_manipYT, &c_manipUD, &c_manipBox, &c_manipVLC,
	};
	constexpr int CHECKTARGET_INTERVAL = 1000;
}

namespace {
	// とりあえずのキーマップ
	dg::VKMapping MakeKeyMap() {
		using dg::VKMapping;
		using dg::VirtualKey;
		using dg::Manip;

		VKMapping map;
		map.emplace(VirtualKey::DLeft, &Manip::backward_few);
		map.emplace(VirtualKey::DRight, &Manip::forward_few);
		map.emplace(VirtualKey::DUp, &Manip::volumeUp);
		map.emplace(VirtualKey::DDown, &Manip::volumeDown);

		map.emplace(VirtualKey::B, &Manip::startPause);
		map.emplace(VirtualKey::Y, &Manip::volumeMute);
		map.emplace(VirtualKey::X, &Manip::fullScreen);
		map.emplace(VirtualKey::Select, &Manip::captionSwitch);

		map.emplace(VirtualKey::L1, &Manip::speedDown);
		map.emplace(VirtualKey::R1, &Manip::speedUp);
		map.emplace(VirtualKey::L2, &Manip::backward_medium);
		map.emplace(VirtualKey::R2, &Manip::forward_medium);

		map.emplace(VirtualKey::TL_Left, &Manip::backward_few);
		map.emplace(VirtualKey::TL_Right, &Manip::forward_few);
		map.emplace(VirtualKey::TL_Up, &Manip::volumeUp);
		map.emplace(VirtualKey::TL_Down, &Manip::volumeDown);

		map.emplace(VirtualKey::TR_Left, &Manip::backward_medium);
		map.emplace(VirtualKey::TR_Right, &Manip::forward_medium);
		map.emplace(VirtualKey::TR_Up, &Manip::speedUp);
		map.emplace(VirtualKey::TR_Down, &Manip::speedDown);
		return map;
	}
	dg::VKMapping MakeKeyMap_Wii() {
		using dg::VKMapping;
		using dg::VirtualKey;
		using dg::Manip;

		VKMapping map;
		map.emplace(VirtualKey::DLeft, &Manip::backward_few);
		map.emplace(VirtualKey::DRight, &Manip::forward_few);
		map.emplace(VirtualKey::DUp, &Manip::volumeUp);
		map.emplace(VirtualKey::DDown, &Manip::volumeDown);

		map.emplace(VirtualKey::A, &Manip::startPause);
		map.emplace(VirtualKey::B, &Manip::fullScreen);
		map.emplace(VirtualKey::L1, &Manip::volumeMute);

		map.emplace(VirtualKey::Select, &Manip::speedDown);
		map.emplace(VirtualKey::Start, &Manip::speedUp);
		map.emplace(VirtualKey::X, &Manip::backward_medium);
		map.emplace(VirtualKey::Y, &Manip::forward_medium);
		return map;
	}
}
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  _ui(new Ui::MainWindow),
	  _manip(nullptr),
	  _hwTarget(nullptr)
{
	_ui->setupUi(this);
	_initInputs();

	// ウィンドウ検索のインターバルタイマー初期化
	_findWindowTimer = new QTimer(this);
	_findWindowTimer->start(CHECKTARGET_INTERVAL);
	connect(_findWindowTimer, &QTimer::timeout, this, &MainWindow::checkTargetWindow);

	// ウィンドウフォーカス復元用タイマー
	_restoreFocusTimer = new QTimer(this);
//	  _restoreFocusTimer->setInterval(1000);
	_restoreFocusTimer->setSingleShot(true);
	connect(_restoreFocusTimer, &QTimer::timeout, this, &MainWindow::onRestoreFocus);

	// インプット手段のリセットアクション
	connect(_ui->actionRefresh_Input_r, &QAction::triggered,
			this, &MainWindow::_initInputs);
}

void MainWindow::_initInputs() {
	// 終了処理等あるので一旦nullをセット
	_imgr.reset();
	_imgrWidget.reset();
	// とりあえず起動時にWiiRemoteが検出されればそれを、なければXInputで初期化する
	auto wiiMgr = std::make_unique<dg::wii::Manager>();
	if(wiiMgr->numRemote() == 0) {
		auto* xiMgr = new dg::xinput::Manager();
		_imgr.reset(xiMgr);
		_keyMap = MakeKeyMap();
	} else {
		_imgr.reset(wiiMgr.release());
		_keyMap = MakeKeyMap_Wii();
	}

	connect(_imgr.get(), &dg::InputMgrBase::onInput, this, &MainWindow::onPadUpdate);
	auto* dialog = _imgr->makeDialog();
	_imgrWidget.reset(dialog);
	_ui->verticalLayout->addWidget(dialog, 1);
}
void MainWindow::onPadUpdate(const dg::VKInputs& inputs) {
	if(_hwTarget) {
		const auto& m = _keyMap;
		for(auto& inp : inputs) {
			const auto itr = m.find(inp);
			if(itr != m.end()) {
				const auto ptr = itr->second;
                if(!_restoreFocusTimer->isActive()) {
					_hwRestore = GetForegroundWindow();
					SetForegroundWindow(_hwTarget);
					QThread::msleep(50);
					_manip->setFocus(_hwTarget);
				}
				(_manip->*ptr)(_hwTarget);
				_restoreFocusTimer->start();
			}
		}
	}
}
void MainWindow::_switchManip(const dg::Manip *manip) {
	if(_manip == manip)
		return;

	_manip = manip;
    _restoreFocusTimer->stop();
    _restoreFocusTimer->setInterval(300);
	_ui->lbWindowName->setText(
			_manip ? (_manip->getName() + u8" Running...") : QString("----"));
}
void MainWindow::checkTargetWindow() {
	_hwTarget = nullptr;
	for(auto* m: c_manipList) {
		if((_hwTarget = m->findTarget())) {
			_switchManip(m);
			break;
		}
	}
}

void MainWindow::onRestoreFocus() {
	SetForegroundWindow(_hwRestore);
}
