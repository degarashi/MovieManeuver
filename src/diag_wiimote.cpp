#include "diag_wiimote.h"
#include "ui_diag_wiimote.h"
#include "wii_padstate.hpp"

namespace dg::wii {
	DebugViewWidget::DebugViewWidget(QWidget *parent) :
		QWidget(parent),
		_ui(new Ui::DebugViewWidget)
	{
		_ui->setupUi(this);
	}
	void DebugViewWidget::updateDebugView(const wii::Remote& remote) {
		// dg::wii::Button の定義順に依存
		QCheckBox *const ar[] = {
			_ui->cbA, _ui->cbB,
			_ui->cb1, _ui->cb2,
			_ui->cbMinus, _ui->cbHome, _ui->cbPlus,
			_ui->cbLeft, _ui->cbUp, _ui->cbRight, _ui->cbDown,
		};
		const auto btn = remote.getButtonDiff();
		for(auto& k : btn) {
			const int idx = static_cast<int>(VKToButtonMap.at(k.key));
			ar[idx]->setChecked(k.pressed);
		}
		{
			const auto acc = remote.getAccelDiff();
			std::array<std::pair<VirtualKey, QCheckBox *const>, 6> ar_acc {
				std::make_pair(VirtualKey::TL_Left, _ui->cbAccX_Neg),
				{VirtualKey::TL_Right, _ui->cbAccX_Pos},
				{VirtualKey::TL_Up, _ui->cbAccY_Neg},
				{VirtualKey::TL_Down, _ui->cbAccY_Pos},
				{VirtualKey::TR_Left, _ui->cbAccZ_Neg},
				{VirtualKey::TR_Right, _ui->cbAccZ_Pos},
			};
			for(auto& a : acc) {
				auto itr = std::find_if(ar_acc.begin(), ar_acc.end(),
									 [&a](auto& p){ return a.key == p.first; });
				if(itr != ar_acc.end()) {
					itr->second->setChecked(a.pressed);
				}
			}
		}
		{
			const auto acc = remote.getAcceleration();
			const auto proc = [&acc](int idx, auto* dst){
				const int val = static_cast<int>(std::floor(acc.axis(idx).axis() * 50.f + 50.f));
				dst->setValue(val);
			};
			proc(0, _ui->pbAccX);
			proc(1, _ui->pbAccY);
			proc(2, _ui->pbAccZ);
		}
	}
}
