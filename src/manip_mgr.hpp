#pragma once
#include "input.hpp"
#include <QObject>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "vk_def.hpp"

class QTimer;
namespace dg {
	class InputMapSet;
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
			QTimer*						_restoreFocusTimer;
			HWND						_hwRestore;

			// ---- initInputs()で作成し直すクラス ----
			template <class T>
			using SP = std::shared_ptr<T>;

			SP<dg::InputMgrBase>	_imgr;
			SP<QWidget>				_imgrWidget;
			SP<InputMapSet>			_inputMapSet;
			// ----------------------------------------

			void _switchManip(const dg::Manip* manip);

		public:
			Manip_Mgr(QObject* parent = nullptr);

		signals:
			void onWidgetCreated(QWidget* w);
			void onManipChanged(const QString& name);

		public slots:
			// XInput / WiiRemoteの初期化
			void initInputs();

		private slots:
			// call from RestoreFocusTimer
			void onRestoreFocus();
			void onPadUpdate(const VKStateAr& state);
			void checkTargetWindow();
	};
}
