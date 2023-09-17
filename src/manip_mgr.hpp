#pragma once
#include "keydiff.hpp"
#include "keymap.hpp"

class QTimer;
namespace dg {
	class InputMgrBase;
	struct Manip;
	class Manip_Mgr :
		public QObject
	{
		Q_OBJECT
		private:
			// ウィンドウ検索のインターバルタイマー
			QTimer*						_findWindowTimer;
			// 対象(Youtube/Udemy)の検索や操作方法を定義
			const dg::Manip*			_manip;
			// 操作対象(ブラウザ)のウィンドウハンドル
			HWND						_hwTarget;
			Keymap						_keymap;
			QTimer*						_restoreFocusTimer;
			HWND						_hwRestore;

			template <class T>
			using SP = std::shared_ptr<T>;

			SP<dg::InputMgrBase>	_imgr;
			SP<QWidget>				_imgrWidget;

			void _switchManip(const dg::Manip* manip);

		public:
			Manip_Mgr(QObject* parent = nullptr);

		signals:
			// from InputMgr
			void onInput(const KeyDiff_V& inputs);
			void onWidgetCreated(QWidget* w);
			void onManipChanged(const QString& name);

		public slots:
			// XInput / WiiRemoteの初期化
			void initInputs();

		private slots:
			// call from RestoreFocusTimer
			void onRestoreFocus();
			void onPadUpdate(const dg::KeyDiff_V& inputs);
			void checkTargetWindow();
	};
}
