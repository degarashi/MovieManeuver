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
		const auto pressed = remote.getPressedButton();
		// dg::wii::Button の定義順に依存
		QCheckBox *const ar[] = {
			_ui->cbA, _ui->cbB,
			_ui->cb1, _ui->cb2,
			_ui->cbMinus, _ui->cbHome, _ui->cbPlus,
			_ui->cbLeft, _ui->cbUp, _ui->cbRight, _ui->cbDown,
		};
		for(int i=0 ; i<std::size(ar) ; i++) {
			ar[i]->setChecked(pressed[i]);
		}
	}
}
