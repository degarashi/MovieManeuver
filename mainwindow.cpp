#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "inputmgr.hpp"
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
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  _ui(new Ui::MainWindow),
	  _manip(nullptr),
	  _hwTarget(nullptr)
{
	_ui->setupUi(this);
	auto* t = new dg::InputMgr();
	connect(t, &dg::InputMgr::onInput, this, &MainWindow::onPadUpdate);

	_timer = new QTimer(this);
	_timer->start(CHECKTARGET_INTERVAL);
	connect(_timer, &QTimer::timeout, this, &MainWindow::checkTargetWindow);
}
void MainWindow::_updateDebugView(const dg::PadState& state) {
	QCheckBox* ar[] = {
		_ui->cbStart, _ui->cbBack,
		_ui->cbA, _ui->cbB, _ui->cbX, _ui->cbY,
		_ui->cbLThumb, _ui->cbLShoulder,
		_ui->cbRThumb, _ui->cbRShoulder,
		_ui->cbDPad_Left, _ui->cbDPad_Up, _ui->cbDPad_Right, _ui->cbDPad_Down,
	};
	struct Trig {
		PS::E_Button	sourceCB;
		PS::E_Trigger	sourcePB;
		QCheckBox		*cb;
		QProgressBar	*pb;
	};

	Trig arT[] = {
		{PS::E_Button::LeftTrigger, PS::E_Trigger::TriggerLeft,
			_ui->cbLTrigger, _ui->pbLTrigger},
		{PS::E_Button::RightTrigger, PS::E_Trigger::TriggerRight,
			_ui->cbRTrigger, _ui->pbRTrigger},
	};
	struct Thumb {
		PS::E_Thumb		sourceId;
		QProgressBar	*pbX, *pbY;
	};
	Thumb arB[] = {
		{PS::E_Thumb::ThumbLeft, _ui->pbLThumbX, _ui->pbLThumbY},
		{PS::E_Thumb::ThumbRight, _ui->pbRThumbX, _ui->pbRThumbY},
	};
	for(int i=0 ; i<std::size(arB) ; i++) {
		auto& ent = arB[i];
		const auto val = state.getThumb(ent.sourceId);
		ent.pbX->setValue(val.x * 50);
		ent.pbY->setValue(val.y * 50);
	}

	for(int i=0 ; i<std::size(ar) ; i++) {
		ar[i]->setChecked(state.pressing(static_cast<dg::PadState::E_Button>(i)) > 0);
	}
	for(int i=0 ; i<std::size(arT) ; i++) {
		auto& ent = arT[i];
		ent.cb->setChecked(state.pressing(ent.sourceCB));
		ent.pb->setValue(state.getTrigger(ent.sourcePB) * 100);
	}
}

void MainWindow::_manipulate(const dg::PadState& state) {
	if(_hwTarget) {
		struct ManipulateEnt {
			PS::E_Button	buttonId;
			void (dg::Manip::*mproc)(HWND) const;
		};
		const ManipulateEnt manipEnt[] = {
			{PS::E_Button::A, &dg::Manip::startPause},

			{PS::E_Button::DPadLeft, &dg::Manip::backward_5sec},
			{PS::E_Button::DPadRight, &dg::Manip::forward_5sec},
			{PS::E_Button::LeftThumbNX, &dg::Manip::backward_5sec},
			{PS::E_Button::LeftThumbPX, &dg::Manip::forward_5sec},

			{PS::E_Button::LeftTrigger, &dg::Manip::backward_10sec},
			{PS::E_Button::RightTrigger, &dg::Manip::forward_10sec},
			{PS::E_Button::RightThumbNX, &dg::Manip::backward_10sec},
			{PS::E_Button::RightThumbPX, &dg::Manip::forward_10sec},

			{PS::E_Button::LeftShoulder, &dg::Manip::speedDown},
			{PS::E_Button::RightShoulder, &dg::Manip::speedUp},
			{PS::E_Button::RightThumbNY, &dg::Manip::speedDown},
			{PS::E_Button::RightThumbPY, &dg::Manip::speedUp},

			{PS::E_Button::DPadDown, &dg::Manip::volumeDown},
			{PS::E_Button::DPadUp, &dg::Manip::volumeUp},
			{PS::E_Button::LeftThumbNY, &dg::Manip::volumeDown},
			{PS::E_Button::LeftThumbPY, &dg::Manip::volumeUp},

			{PS::E_Button::X, &dg::Manip::volumeMute},
			{PS::E_Button::Y, &dg::Manip::fullScreen},
		};
		for(auto& e: manipEnt) {
			if(state.pressed(e.buttonId)) {
				(_manip->*e.mproc)(_hwTarget);
				break;
			}
		}
	}
}
void MainWindow::onPadUpdate(const dg::PadState& state) {
	_updateDebugView(state);
	_manipulate(state);
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
