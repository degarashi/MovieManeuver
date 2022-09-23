#pragma once
#include <QMainWindow>

namespace dg {
	struct XI_PadState;
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
		using PS = dg::XI_PadState;
		std::shared_ptr<Ui::MainWindow> _ui;
		QTimer*							_timer;
		const dg::Manip*				_manip;
		HWND							_hwTarget;

		void _manipulate(const dg::XI_PadState& state);

	private slots:
		void onPadUpdate(const dg::XI_PadState& state);
		void checkTargetWindow();
};
