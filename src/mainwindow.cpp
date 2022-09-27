#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "inputmgr_wii.hpp"
#include "inputmgr_xinput.hpp"
#include "manip_udemy.hpp"
#include "manip_youtube.hpp"

#include <QThread>
#include <QTimer>
#include <QCheckBox>
#include <QProgressBar>

namespace {
	const dg::Manip_YouTube c_manipYT;
	const dg::Manip_Udemy c_manipUD;

	const dg::Manip* c_manipList[] = {
		&c_manipYT, &c_manipUD
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
		map.emplace(VirtualKey::DLeft, &Manip::backward_5sec);
		map.emplace(VirtualKey::DRight, &Manip::forward_5sec);
		map.emplace(VirtualKey::DUp, &Manip::volumeUp);
		map.emplace(VirtualKey::DDown, &Manip::volumeDown);

		map.emplace(VirtualKey::B, &Manip::startPause);
		map.emplace(VirtualKey::Y, &Manip::volumeMute);

		map.emplace(VirtualKey::L1, &Manip::speedDown);
		map.emplace(VirtualKey::R1, &Manip::speedUp);
		map.emplace(VirtualKey::L2, &Manip::backward_10sec);
		map.emplace(VirtualKey::R2, &Manip::forward_10sec);

		map.emplace(VirtualKey::TL_Left, &Manip::backward_5sec);
		map.emplace(VirtualKey::TL_Right, &Manip::forward_5sec);
		map.emplace(VirtualKey::TL_Up, &Manip::volumeUp);
		map.emplace(VirtualKey::TL_Down, &Manip::volumeDown);

		map.emplace(VirtualKey::TR_Left, &Manip::backward_10sec);
		map.emplace(VirtualKey::TR_Right, &Manip::forward_10sec);
		map.emplace(VirtualKey::TR_Up, &Manip::speedUp);
		map.emplace(VirtualKey::TR_Down, &Manip::speedDown);
		return map;
	}
	dg::VKMapping MakeKeyMap_Wii() {
		using dg::VKMapping;
		using dg::VirtualKey;
		using dg::Manip;

		VKMapping map;
		map.emplace(VirtualKey::DLeft, &Manip::backward_5sec);
		map.emplace(VirtualKey::DRight, &Manip::forward_5sec);
		map.emplace(VirtualKey::DUp, &Manip::volumeUp);
		map.emplace(VirtualKey::DDown, &Manip::volumeDown);

		map.emplace(VirtualKey::A, &Manip::startPause);
		map.emplace(VirtualKey::B, &Manip::volumeMute);
		map.emplace(VirtualKey::L1, &Manip::volumeMute);

		map.emplace(VirtualKey::Select, &Manip::speedDown);
		map.emplace(VirtualKey::Start, &Manip::speedUp);
		map.emplace(VirtualKey::X, &Manip::backward_10sec);
		map.emplace(VirtualKey::Y, &Manip::forward_10sec);
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

	// とりあえず起動時にWiiRemoteが検出されればそれを、なければXInputで初期化する
	{
		auto* wiiMgr = new dg::wii::Manager();
		if(wiiMgr->numRemote() == 0) {
			auto* xiMgr = new dg::xinput::XInputMgr();
			_imgr.reset(xiMgr);
			_keyMap = MakeKeyMap();
		} else {
			_imgr.reset(wiiMgr);
			_keyMap = MakeKeyMap_Wii();
		}

		connect(_imgr.get(), &dg::InputMgrBase::onInput, this, &MainWindow::onPadUpdate);
		_ui->verticalLayout->addWidget(_imgr->makeDialog(), 1);
	}

	// ウィンドウ検索のインターバルタイマー初期化
	_timer = new QTimer(this);
	_timer->start(CHECKTARGET_INTERVAL);
	connect(_timer, &QTimer::timeout, this, &MainWindow::checkTargetWindow);
}
void MainWindow::onPadUpdate(const dg::VKInputs& inputs) {
	if(_hwTarget) {
		const auto& m = _keyMap;
		for(auto& inp : inputs) {
			const auto itr = m.find(inp);
			if(itr != m.end()) {
				auto ptr = itr->second;
				(_manip->*ptr)(_hwTarget);
			}
		}
	}
}
void MainWindow::checkTargetWindow() {
	_hwTarget = nullptr;
	_manip = nullptr;
	for(auto* m: c_manipList) {
		if((_hwTarget = m->findTarget())) {
			_manip = m;
			break;
		}
	}
	_ui->lbWindowName->setText(
			_manip ? (_manip->getName() + u8" Running...") : QString("----"));
}
