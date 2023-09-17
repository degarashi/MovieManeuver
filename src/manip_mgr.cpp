#include "manip_mgr.hpp"
#include "manip_udemy.hpp"
#include "manip_youtube.hpp"
#include "manip_box.hpp"
#include "manip_vlc.hpp"
#include "inputmgr_wii.hpp"
#include "inputmgr_xinput.hpp"

#include <QTimer>
#include <QThread>
#include <QWidget>

namespace {
	const dg::Manip_YouTube c_manipYT;
	const dg::Manip_Udemy c_manipUD;
	const dg::Manip_Box c_manipBox;
	const dg::Manip_VLC c_manipVLC;

	const dg::Manip* c_manipList[] = {
		&c_manipYT, &c_manipUD, &c_manipBox, &c_manipVLC,
	};
	constexpr int CHECKTARGET_INTERVAL = 1000;
}

namespace {
	// とりあえずのキーマップ
	std::tuple<dg::VKMap_V, dg::VirtualKey> MakeKeyMap() {
		using dg::VirtualKey;
		using dg::Manip;

		dg::VKMap_V map {
			// Mapping 0
			{
				{VirtualKey::DLeft, &Manip::backward_few},
				{VirtualKey::DRight, &Manip::forward_few},
				{VirtualKey::DUp, &Manip::volumeUp},
				{VirtualKey::DDown, &Manip::volumeDown},

				{VirtualKey::B, &Manip::startPause},
				{VirtualKey::Y, &Manip::volumeMute},
				{VirtualKey::X, &Manip::fullScreen},
				{VirtualKey::Select, &Manip::captionSwitch},

				{VirtualKey::L1, &Manip::speedDown},
				{VirtualKey::R1, &Manip::speedUp},
				{VirtualKey::L2, &Manip::backward_medium},
				{VirtualKey::R2, &Manip::forward_medium},

				{VirtualKey::TL_Left, &Manip::backward_few},
				{VirtualKey::TL_Right, &Manip::forward_few},
				{VirtualKey::TL_Up, &Manip::volumeUp},
				{VirtualKey::TL_Down, &Manip::volumeDown},

				{VirtualKey::TR_Left, &Manip::backward_medium},
				{VirtualKey::TR_Right, &Manip::forward_medium},
				{VirtualKey::TR_Up, &Manip::speedUp},
				{VirtualKey::TR_Down, &Manip::speedDown},
			},
		};
		return {map, VirtualKey::Invalid};
	}
	std::tuple<dg::VKMap_V, dg::VirtualKey> MakeKeyMap_Wii() {
		using dg::VirtualKey;
		using dg::Manip;

		dg::VKMap_V map {
			{
				{VirtualKey::DLeft, &Manip::backward_few},
				{VirtualKey::DRight, &Manip::forward_few},
				{VirtualKey::DUp, &Manip::volumeUp},
				{VirtualKey::DDown, &Manip::volumeDown},

				{VirtualKey::A, &Manip::startPause},
				{VirtualKey::B, &Manip::fullScreen},
				{VirtualKey::L1, &Manip::volumeMute},

				{VirtualKey::Select, &Manip::speedDown},
				{VirtualKey::Start, &Manip::speedUp},
				{VirtualKey::X, &Manip::backward_medium},
				{VirtualKey::Y, &Manip::forward_medium},
			},
			// Mapping 1 (PlaceHolder)
			{}
		};
		// Map0をベースにMap1を定義
		map[1] = map[0];
		map[1][VirtualKey::DUp] = &Manip::mediaVolumeUp;
		map[1][VirtualKey::DDown] = &Manip::mediaVolumeDown;
		map[1][VirtualKey::A] = &Manip::fullScreen;
		return {map, VirtualKey::B};
	}
}

namespace dg {
	Manip_Mgr::Manip_Mgr(QObject* parent):
		  QObject(parent),
		  _manip(nullptr),
		  _hwTarget(nullptr)
	{
		// ウィンドウ検索のインターバルタイマー初期化
		_findWindowTimer = new QTimer(this);
		_findWindowTimer->start(CHECKTARGET_INTERVAL);
		connect(_findWindowTimer, &QTimer::timeout, this, &Manip_Mgr::checkTargetWindow);

		// ウィンドウフォーカス復元用タイマー
		_restoreFocusTimer = new QTimer(this);
		_restoreFocusTimer->setSingleShot(true);
		connect(_restoreFocusTimer, &QTimer::timeout, this, &Manip_Mgr::onRestoreFocus);

	}
	void Manip_Mgr::initInputs() {
		// 終了処理等あるので一旦nullをセット
		_imgr.reset();
		_imgrWidget.reset();
		// とりあえず起動時にWiiRemoteが検出されればそれを、なければXInputで初期化する
		auto wiiMgr = std::make_unique<dg::wii::Manager>();
		dg::VKMap_V keyMap;
		dg::VirtualKey modeSwKey;
		if(wiiMgr->numRemote() == 0) {
			auto* xiMgr = new dg::xinput::Manager();
			_imgr.reset(xiMgr);
			std::tie(keyMap, modeSwKey) = MakeKeyMap();
		} else {
			_imgr.reset(wiiMgr.release());
			std::tie(keyMap, modeSwKey) = MakeKeyMap_Wii();
		}
		connect(_imgr.get(), &dg::InputMgrBase::onInput,
				this, &Manip_Mgr::onPadUpdate);

		_keymap.setKeymapSwitch(modeSwKey);
		_keymap.setKeymap(keyMap);

		auto* dialog = _imgr->makeDialog();
		_imgrWidget.reset(dialog);
		emit onWidgetCreated(_imgrWidget.get());
	}
	void Manip_Mgr::checkTargetWindow() {
		_hwTarget = nullptr;
		for(auto* m: c_manipList) {
			if((_hwTarget = m->findTarget())) {
				_switchManip(m);
				break;
			}
		}
	}
	void Manip_Mgr::onRestoreFocus() {
		SetForegroundWindow(_hwRestore);
	}

	// from (winop.hpp)
	UINT GetDoubleClickTime();
	void Manip_Mgr::_switchManip(const dg::Manip *manip) {
		if(_manip == manip)
			return;

		_manip = manip;
		_restoreFocusTimer->stop();
		_restoreFocusTimer->setInterval(GetDoubleClickTime());

		const QString manipName = (_manip) ? _manip->getName() : "Nothing";
		emit onManipChanged(manipName);
	}
	void Manip_Mgr::onPadUpdate(const dg::KeyDiff_V& inputs) {
		if(_hwTarget) {
			for(auto& inp : inputs) {
				if(!inp.pressed)
					continue;
				if(_keymap.keySwitch(inp.key))
					continue;
				if(!_restoreFocusTimer->isActive()) {
					_hwRestore = GetForegroundWindow();
					SetForegroundWindow(_hwTarget);
					QThread::msleep(50);
					_manip->setFocus(_hwTarget);
				}
				_keymap.manipulate(inp.key, _manip, _hwTarget);
				_restoreFocusTimer->start();
			}
		}
	}
}
