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
		const auto state = remote.getState();
		// 各種ボタンステート
		{
			// dg::wii::Button の定義順に依存
			QCheckBox *const ar[] = {
				_ui->cbA, _ui->cbB,
				_ui->cb1, _ui->cb2,
				_ui->cbMinus, _ui->cbHome, _ui->cbPlus,
				_ui->cbLeft, _ui->cbUp, _ui->cbRight, _ui->cbDown,
			};
			for(int i=0 ; i<Num_Button ; i++) {
				ar[i]->setChecked(state[static_cast<int>(ButtonToVKMap.at(Button(i)))].pressing());
			}
		}
		// 軸のボタン解釈
		{
			std::array<std::pair<VirtualKey, QCheckBox *const>, 6> ar_acc {
				std::make_pair(VirtualKey::TL_Left, _ui->cbAccX_Neg),
				{VirtualKey::TL_Right, _ui->cbAccX_Pos},
				{VirtualKey::TL_Up, _ui->cbAccY_Neg},
				{VirtualKey::TL_Down, _ui->cbAccY_Pos},
				{VirtualKey::TR_Left, _ui->cbAccZ_Neg},
				{VirtualKey::TR_Right, _ui->cbAccZ_Pos},
			};
			for(auto& a : ar_acc) {
				a.second->setChecked(state[static_cast<int>(a.first)].pressing());
			}
		}
		// 軸の値(%)を表示
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
