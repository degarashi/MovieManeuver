#include "dg_diag_xinput.hpp"
#include "ui_diag_xinput.h"
#include "xi_padstate.hpp"

namespace dg::xinput {
	DebugViewWidget::DebugViewWidget(QWidget *parent) :
		QWidget(parent),
		_ui(new Ui::DebugViewWidget)
	{
		_ui->setupUi(this);
	}
	void DebugViewWidget::updateDebugView(const dg::xinput::PadState& state) {
		QCheckBox* ar[] = {
			_ui->cbStart, _ui->cbBack,
			_ui->cbA, _ui->cbB, _ui->cbX, _ui->cbY,
			_ui->cbLThumb, _ui->cbLShoulder,
			_ui->cbRThumb, _ui->cbRShoulder,
			_ui->cbDPad_Left, _ui->cbDPad_Up, _ui->cbDPad_Right, _ui->cbDPad_Down,
		};

		struct ThumbP {
			Thumb		sourceId;
			QProgressBar	*pbX, *pbY;
		};
		ThumbP arB[] = {
			{Thumb::ThumbLeft, _ui->pbLThumbX, _ui->pbLThumbY},
			{Thumb::ThumbRight, _ui->pbRThumbX, _ui->pbRThumbY},
		};
		for(int i=0 ; i<std::size(arB) ; i++) {
			auto& ent = arB[i];
			const auto val = state.getThumb(ent.sourceId);
			ent.pbX->setValue(val.x * 50);
			ent.pbY->setValue(val.y * 50);
		}
		for(int i=0 ; i<std::size(ar) ; i++) {
			ar[i]->setChecked(state.pressing(static_cast<dg::xinput::Button>(i)) > 0);
		}

		struct Trig {
			Trigger	sourceId;
			QCheckBox		*cb;
			QProgressBar	*pb;
		};
		const Trig arT[] = {
			{Trigger::TriggerLeft, _ui->cbLTrigger, _ui->pbLTrigger},
			{Trigger::TriggerRight, _ui->cbRTrigger, _ui->pbRTrigger},
		};
		for(auto& ent : arT) {
			auto& t = state.getTrigger(ent.sourceId);
			ent.cb->setChecked(t.buttonState().pressing());
			ent.pb->setValue(t.trigger() * 100);
		}
	}
}
