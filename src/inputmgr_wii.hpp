#pragma once
#include "inputmgr_base.hpp"
#include "wii_padstate.hpp"

namespace dg::wii {
	class Manager : public InputMgrBase {
		Q_OBJECT
		private:
			using Remote_v = std::vector<Remote>;
			Remote_v	_remote;

			void _updateAll();

		public:
			Manager();
			~Manager();
			QWidget* makeDialog() override;
			int numRemote() const;
		private slots:
			void onTimer() override;
		signals:
			void onInputWii(const dg::wii::Remote& remote);
	};
}
