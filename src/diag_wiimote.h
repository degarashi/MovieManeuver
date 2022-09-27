#pragma once
#include <QWidget>

namespace dg::wii {
	class Remote;
	namespace Ui {
		class DebugViewWidget;
	}
	class DebugViewWidget : public QWidget {
		Q_OBJECT
		private:
			std::shared_ptr<Ui::DebugViewWidget> _ui;

		public:
			explicit DebugViewWidget(QWidget *parent = nullptr);
		public slots:
			void updateDebugView(const dg::wii::Remote& remote);
	};
}
