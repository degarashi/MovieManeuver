#pragma once
#include <QObject>
#include "vk_def.hpp"

class QTimer;
namespace dg {
	class InputMgrBase : public QObject {
		Q_OBJECT
		private:
			// キー入力をポーリングで監視するタイマー
			QTimer*		_timer;
		signals:
			void onInput(const VKStateAr& state);
		private slots:
			virtual void onTimer() = 0;
		public:
			InputMgrBase();
			virtual QWidget* makeDialog() = 0;
	};
}
