#pragma once
#include "inputmgr.hpp"
#include "wii_padstate.hpp"

namespace dg::wii {
	class Manager : public InputMgrBase {
		Q_OBJECT
		private:
			using Remote_v = std::vector<Remote>;
			Remote_v	_remote;

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
