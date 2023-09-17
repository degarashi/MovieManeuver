#pragma once
#include <QMainWindow>

namespace dg {
	class Manip_Mgr;
}
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT
	public:
		MainWindow(QWidget* parent=nullptr);

	private:
		template <class T>
		using SP = std::shared_ptr<T>;
		SP<Ui::MainWindow>		_ui;

		dg::Manip_Mgr*			_manipMgr;

	private slots:
		void onInputWidgetCreated(QWidget* widget);
		void onManipChanged(const QString& name);
};
