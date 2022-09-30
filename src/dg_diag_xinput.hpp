#pragma once
#include <QWidget>

namespace dg::xinput {
	struct PadState;
	namespace Ui {
		class DebugViewWidget;
	}

	class DebugViewWidget : public QWidget {
		Q_OBJECT
		private:
			std::shared_ptr<Ui::DebugViewWidget>	_ui;

		public:
			explicit DebugViewWidget(QWidget *parent = nullptr);

		public slots:
			void updateDebugView(const dg::xinput::PadState& state);
	};
}
