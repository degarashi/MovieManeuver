#pragma once
#include <QObject>
#include <windows.h>
#include "xi_padstate.hpp"
#include "virtual_key_def.hpp"

class QTimer;
namespace dg {
	class InputMgrBase : public QObject {
		Q_OBJECT
		private:
			// キー入力をポーリングで監視するタイマー
			QTimer*		_timer;
		signals:
			void onInput(const dg::VKInputs& inputs);
		private slots:
			virtual void onTimer() = 0;
		public:
			InputMgrBase();
			virtual QWidget* makeDialog() = 0;
	};
	class XInputMgr : public InputMgrBase {
		Q_OBJECT
		private:
			DWORD		_pktNum = 0;
			XI_PadState	_state;
			using PS = XI_PadState;

			VKInputs _composeInputs() const;

		private slots:
			void onTimer() override;
		signals:
			void onInputXI(const dg::XI_PadState& state);
		public:
			XInputMgr();
			QWidget* makeDialog() override;
	};
}
