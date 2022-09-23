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
void MainWindow::_updateDebugView(const dg::XI_PadState& state) {
	QCheckBox* ar[] = {
		_ui->cbStart, _ui->cbBack,
		_ui->cbA, _ui->cbB, _ui->cbX, _ui->cbY,
		_ui->cbLThumb, _ui->cbLShoulder,
		_ui->cbRThumb, _ui->cbRShoulder,
		_ui->cbDPad_Left, _ui->cbDPad_Up, _ui->cbDPad_Right, _ui->cbDPad_Down,
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
		ar[i]->setChecked(state.pressing(static_cast<dg::XI_PadState::E_Button>(i)) > 0);
	}

	struct Trig {
		PS::E_Trigger	sourceId;
		QCheckBox		*cb;
		QProgressBar	*pb;
	};
	const Trig arT[] = {
		{PS::E_Trigger::TriggerLeft, _ui->cbLTrigger, _ui->pbLTrigger},
		{PS::E_Trigger::TriggerRight, _ui->cbRTrigger, _ui->pbRTrigger},
	};
	for(auto& ent : arT) {
		auto& t = state.getTrigger(ent.sourceId);
		ent.cb->setChecked(t.buttonState().pressing());
		ent.pb->setValue(t.trigger() * 100);
	}
}

void MainWindow::_manipulate(const dg::XI_PadState& state) {
	if(_hwTarget) {
		struct ManipulateEnt {
			PS::E_Button	buttonId;
			void (dg::Manip::*mproc)(HWND) const;
		};
		const ManipulateEnt manipEnt[] = {
			{PS::E_Button::A, &dg::Manip::startPause},

			{PS::E_Button::DPadLeft, &dg::Manip::backward_5sec},
			{PS::E_Button::DPadRight, &dg::Manip::forward_5sec},

			{PS::E_Button::LeftShoulder, &dg::Manip::speedDown},
			{PS::E_Button::RightShoulder, &dg::Manip::speedUp},

			{PS::E_Button::DPadDown, &dg::Manip::volumeDown},
			{PS::E_Button::DPadUp, &dg::Manip::volumeUp},

			{PS::E_Button::X, &dg::Manip::volumeMute},
			{PS::E_Button::Y, &dg::Manip::fullScreen},
		};
		for(auto& e: manipEnt) {
			if(state.pressed(e.buttonId)) {
				(_manip->*e.mproc)(_hwTarget);
				break;
			}
		}

		if(state.getTrigger(PS::E_Trigger::TriggerLeft).buttonState().pressed()) {
			_manip->backward_10sec(_hwTarget);
		}
		if(state.getTrigger(PS::E_Trigger::TriggerRight).buttonState().pressed()) {
			_manip->forward_10sec(_hwTarget);
		}

		// TODO: 後でリファクタリング
		if(state.axis(PS::E_Thumb::ThumbLeft)
			.axis(dg::AxisState2D::Horizontal)
			.trigger(dg::AxisState::Negative).buttonState().pressed())
			_manip->backward_5sec(_hwTarget);
		if(state.axis(PS::E_Thumb::ThumbLeft)
			.axis(dg::AxisState2D::Horizontal)
			.trigger(dg::AxisState::Positive).buttonState().pressed())
			_manip->forward_5sec(_hwTarget);
		if(state.axis(PS::E_Thumb::ThumbRight)
			.axis(dg::AxisState2D::Horizontal)
			.trigger(dg::AxisState::Negative).buttonState().pressed())
			_manip->backward_10sec(_hwTarget);
		if(state.axis(PS::E_Thumb::ThumbRight)
			.axis(dg::AxisState2D::Horizontal)
			.trigger(dg::AxisState::Positive).buttonState().pressed())
			_manip->forward_10sec(_hwTarget);

		if(state.axis(PS::E_Thumb::ThumbLeft)
			.axis(dg::AxisState2D::Vertical)
			.trigger(dg::AxisState::Negative).buttonState().pressed())
			_manip->volumeDown(_hwTarget);
		if(state.axis(PS::E_Thumb::ThumbLeft)
			.axis(dg::AxisState2D::Vertical)
			.trigger(dg::AxisState::Positive).buttonState().pressed())
			_manip->volumeUp(_hwTarget);
		if(state.axis(PS::E_Thumb::ThumbRight)
			.axis(dg::AxisState2D::Vertical)
			.trigger(dg::AxisState::Negative).buttonState().pressed())
			_manip->speedDown(_hwTarget);
		if(state.axis(PS::E_Thumb::ThumbRight)
			.axis(dg::AxisState2D::Vertical)
			.trigger(dg::AxisState::Positive).buttonState().pressed())
			_manip->speedUp(_hwTarget);
	}
}
void MainWindow::onPadUpdate(const dg::XI_PadState& state) {
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
