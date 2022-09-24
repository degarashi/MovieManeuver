#pragma once
#include "inputmgr.hpp"
#include "xi_padstate.hpp"

namespace dg {
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
