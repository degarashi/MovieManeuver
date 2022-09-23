#pragma once
#include <QMainWindow>

namespace dg {
	struct PadState;
	struct Manip;
}
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT
	public:
		MainWindow(QWidget* parent=nullptr);

	private:
		using PS = dg::PadState;
		std::shared_ptr<Ui::MainWindow> _ui;
		QTimer*							_timer;
		const dg::Manip*				_manip;
		HWND							_hwTarget;

		// ---- for Debug UI ----
		void _updateDebugView(const dg::PadState& state);
		void _manipulate(const dg::PadState& state);

	private slots:
		void onPadUpdate(const dg::PadState& state);
		void checkTargetWindow();
};
