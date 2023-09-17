#pragma once
#include <QMainWindow>
#include "virtual_key_def.hpp"
#include "keymap.hpp"
#include "keydiff.hpp"

namespace dg {
	struct Manip;
	class InputMgrBase;
	using VKMap_V = std::vector<VKMapping>;
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
		SP<QWidget>				_imgrWidget;
		// ウィンドウ検索のインターバルタイマー
        QTimer*							_findWindowTimer;
		// 対象(Youtube/Udemy)の検索や操作方法を定義
		const dg::Manip*				_manip;
		// 操作対象(ブラウザ)のウィンドウハンドル
		HWND							_hwTarget;
		dg::Keymap				_keymap;

        QTimer*					_restoreFocusTimer;
        HWND					_hwRestore;
        void _switchManip(const dg::Manip* manip);

    private slots:
		// XInput / WiiRemoteの初期化
		void _initInputs();
		void onPadUpdate(const dg::KeyDiff_V& inputs);
		void checkTargetWindow();
        void onRestoreFocus();
};
