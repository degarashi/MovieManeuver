#pragma once
#include "inputmgr_base.hpp"
#include "xi_padstate.hpp"

namespace dg::xinput {
	class Manager : public InputMgrBase {
		Q_OBJECT
		private:
			DWORD		_pktNum = 0;
			PadState	_state;
			using PS = PadState;

			VKInputs _composeInputs() const;

		private slots:
			void onTimer() override;
		signals:
			void onInputXI(const dg::xinput::PadState& state);
		public:
			Manager();
			QWidget* makeDialog() override;
	};
}
