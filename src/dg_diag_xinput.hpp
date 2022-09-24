#pragma once
#include <QWidget>

namespace Ui {
	class Diag_XInput;
}
namespace dg {
	struct XI_PadState;

	class Diag_XInput : public QWidget {
		Q_OBJECT
		private:
			std::shared_ptr<Ui::Diag_XInput>	_ui;

		public:
			explicit Diag_XInput(QWidget *parent = nullptr);

		public slots:
			void updateDebugView(const dg::XI_PadState& state);
	};
}
