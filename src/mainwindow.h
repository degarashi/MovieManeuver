#pragma once
#include <QMainWindow>
#include "virtual_key_def.hpp"

namespace dg {
	struct Manip;
	class InputMgrBase;
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
		SP<dg::InputMgrBase>	_imgr;
		// ウィンドウ検索のインターバルタイマー
		QTimer*							_timer;
		// 対象(Youtube/Udemy)の検索や操作方法を定義
		const dg::Manip*				_manip;
		// 操作対象(ブラウザ)のウィンドウハンドル
		HWND							_hwTarget;

		dg::VKMapping			_keyMap;

	private slots:
		void onPadUpdate(const dg::VKInputs& inputs);
		void checkTargetWindow();
};
