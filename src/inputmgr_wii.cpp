#include "inputmgr_wii.hpp"
#include "diag_wiimote.h"
#include "wii_exception.hpp"
#include <wiiremote.h>

namespace dg::wii {
	void Manager::_updateAll() {
		WRMT_UpdateAll();
	}

	Manager::Manager() {
		WRMT_WiiRemote* wiiremotes[WRMT_MAX_DEVICES];
		if(WRMT_Init() != 0)
			throw Init_Fail("WRMT_Init() failed");
		const int num = WRMT_GetNumWiiRemote();
		qDebug() << num << " wii remotes found";

		for(int i=0 ; i<num ; i++) {
			auto* rmt = WRMT_GetWiiRemoteAt(i);
			_remote.emplace_back(rmt);
		}
	}
	Manager::~Manager() {
		WRMT_Quit();
	}
	void Manager::onTimer() {
		_updateAll();
		// とりあえず0番以外は対応しない
		auto& m = _remote[0];
		m.updateState();
		const auto pressed = m.getButtonDiff();
		if(!pressed.empty()) {
			emit onInput(pressed);
		}
		emit onInputWii(m);
	}
	QWidget* Manager::makeDialog() {
		auto* diag = new DebugViewWidget();
		connect(this, &Manager::onInputWii, diag, &DebugViewWidget::updateDebugView);
		return diag;
	}
	int Manager::numRemote() const {
		return WRMT_GetNumWiiRemote();
	}
}
