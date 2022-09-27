#pragma once
#include <QWidget>

namespace Ui {
	class Diag_XInput;
}
namespace dg::xinput {
	struct PadState;

	class DebugViewWidget : public QWidget {
		Q_OBJECT
		private:
			std::shared_ptr<Ui::Diag_XInput>	_ui;

		public:
			explicit DebugViewWidget(QWidget *parent = nullptr);

		public slots:
			void updateDebugView(const dg::xinput::PadState& state);
	};
}
