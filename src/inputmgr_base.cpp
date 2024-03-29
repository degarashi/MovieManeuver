#include "inputmgr_base.hpp"
#include <QTimer>

namespace dg {
	namespace {
		constexpr int INTERVAL_MS = 4;
	}
	InputMgrBase::InputMgrBase() {
		_timer = new QTimer;
		_timer->start(INTERVAL_MS);
		connect(_timer, &QTimer::timeout, this, &InputMgrBase::onTimer);
	}
}
