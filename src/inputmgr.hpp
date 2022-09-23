#pragma once
#include <QObject>
#include <windows.h>
#include "xi_padstate.hpp"

class QTimer;
namespace dg {
	class InputMgr : public QObject {
		Q_OBJECT
		public:
			InputMgr();
			~InputMgr() override;
		private:
			QThread*	_th;
			QTimer*		_timer;
			DWORD		_pktNum;
			XI_PadState	_state;

		private slots:
			void onTimer();

		signals:
			void onInput(const dg::XI_PadState& state);
	};
}
