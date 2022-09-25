#pragma once
#include <QWidget>

namespace dg {
	namespace wii {
		class Remote;
	}
	namespace Ui {
		class Diag_Wiimote;
	}
	class Diag_Wiimote : public QWidget {
		Q_OBJECT
		private:
			std::shared_ptr<Ui::Diag_Wiimote> _ui;

		public:
			explicit Diag_Wiimote(QWidget *parent = nullptr);
		public slots:
			void updateDebugView(const dg::wii::Remote& remote);
	};
}
