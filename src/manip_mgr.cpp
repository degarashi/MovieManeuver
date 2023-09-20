#include "manip_mgr.hpp"
#include "manip_udemy.hpp"
#include "manip_youtube.hpp"
#include "manip_box.hpp"
#include "manip_vlc.hpp"
#include "inputmgr_wii.hpp"
#include "inputmgr_xinput.hpp"
#include "input.hpp"
#include "keyinput.hpp"
#include "keyaction.hpp"
#include "keydetect.hpp"
#include "manip_param.hpp"

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

#define MShared(typ, ...) std::make_shared<typ>(__VA_ARGS__)
namespace dg {
	namespace {
		using input::KD_Simul;
		using input::KD_Press;
		using input::KD_Double;
		using input::Act_Method;
		using VK = VirtualKey;
		// とりあえずのキーマップ
		void MakeKeyMap(input::InputMapSet& dst) {
			auto layer0 = std::make_unique<input::InputMapLayer>();
			layer0->addOnPress(0x0000, VK::DLeft, &Manip::backward_few);
			layer0->addOnPress(0x0000, VK::DLeft, &Manip::backward_few);
			layer0->addOnPress(0x0000, VK::DRight, &Manip::forward_few);
			layer0->addOnPress(0x0000, VK::DUp, &Manip::volumeUp);
			layer0->addOnPress(0x0000, VK::DDown, &Manip::volumeDown);

			layer0->addOnPress(0x0000, VK::B, &Manip::startPause);
			layer0->addOnPress(0x0000, VK::Y, &Manip::volumeMute);
			layer0->addOnPress(0x0000, VK::X, &Manip::fullScreen);
			layer0->addOnPress(0x0000, VK::Select, &Manip::captionSwitch);

			layer0->addOnPress(0x0000, VK::L1, &Manip::speedDown);
			layer0->addOnPress(0x0000, VK::R1, &Manip::speedUp);
			layer0->addOnPress(0x0000, VK::L2, &Manip::backward_medium);
			layer0->addOnPress(0x0000, VK::R2, &Manip::forward_medium);

			layer0->addOnPress(0x0000, VK::TL_Left, &Manip::backward_few);
			layer0->addOnPress(0x0000, VK::TL_Right, &Manip::forward_few);
			layer0->addOnPress(0x0000, VK::TL_Up, &Manip::volumeUp);
			layer0->addOnPress(0x0000, VK::TL_Down, &Manip::volumeDown);

			layer0->addOnPress(0x0000, VK::TR_Left, &Manip::backward_medium);
			layer0->addOnPress(0x0000, VK::TR_Right, &Manip::forward_medium);
			layer0->addOnPress(0x0000, VK::TR_Up, &Manip::speedUp);
			layer0->addOnPress(0x0000, VK::TR_Down, &Manip::speedDown);

			dst.addLayer(std::move(layer0));
		}
		void MakeKeyMap_Wii(input::InputMapSet& dst) {
			auto layer0 = std::make_unique<input::InputMapLayer>();
			layer0->addMap(0x0000,
						   MShared(KD_Simul, VK::TL_Right, MShared(KD_Press, VK::DUp)),
						   MShared(Act_Method, &Manip::mediaVolumeUp, false));
			layer0->addMap(0x0000,
						   MShared(KD_Simul, VK::TL_Right, MShared(KD_Press, VK::DDown)),
						   MShared(Act_Method, &Manip::mediaVolumeDown, false));
			layer0->addMap(0x0000,
						   MShared(KD_Double, VK::DDown, 2),
						   MShared(Act_Method, &Manip::mediaVolumeDown, false));
			layer0->addMap(0x0000,
						   MShared(KD_Double, VK::DUp, 2),
						   MShared(Act_Method, &Manip::mediaVolumeUp, false));

			layer0->addOnPress(0x0000, VK::DLeft, &Manip::backward_few);
			layer0->addOnPress(0x0000, VK::DRight, &Manip::forward_few);
			layer0->addOnPress(0x0000, VK::DUp, &Manip::volumeUp);
			layer0->addOnPress(0x0000, VK::DDown, &Manip::volumeDown);

			layer0->addOnPress(0x0000, VK::A, &Manip::startPause);
			layer0->addOnPress(0x0000, VK::B, &Manip::fullScreen);
			layer0->addOnPress(0x0000, VK::L1, &Manip::volumeMute);
			layer0->addOnPress(0x0000, VK::Select, &Manip::speedDown);
			layer0->addOnPress(0x0000, VK::Start, &Manip::speedUp);
			layer0->addOnPress(0x0000, VK::X, &Manip::backward_medium);
			layer0->addOnPress(0x0000, VK::Y, &Manip::forward_medium);
			dst.addLayer(std::move(layer0));
		}
	}
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
		_inputMapSet.reset();
		_inputMapSet = std::make_shared<input::InputMapSet>();
		_ki = std::make_shared<input::KeyInput>();
		_prevTime = input::Clock::now();
		// とりあえず起動時にWiiRemoteが検出されればそれを、なければXInputで初期化する
		auto wiiMgr = std::make_unique<wii::Manager>();
		if(wiiMgr->numRemote() == 0) {
			auto* xiMgr = new xinput::Manager();
			_imgr.reset(xiMgr);
			MakeKeyMap(*_inputMapSet);
		} else {
			_imgr.reset(wiiMgr.release());
			MakeKeyMap_Wii(*_inputMapSet);
		}
		connect(_imgr.get(), &InputMgrBase::onInput,
				this, &Manip_Mgr::onPadUpdate);

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

	void Manip_Mgr::foreground() {
		if(!_restoreFocusTimer->isActive()) {
			_hwRestore = GetForegroundWindow();
			SetForegroundWindow(_hwTarget);
			QThread::msleep(50);
			_manip->setFocus(_hwTarget);
		}
	}

	void Manip_Mgr::callManip(ManipF method, const int param) const {
		ManipParam mp;
		mp.hw = _hwTarget;
		mp.aux0 = param;
		(_manip->*method)(mp);
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

	void Manip_Mgr::onPadUpdate(const VKStateAr& state) {
		const auto curTime = input::Clock::now();
		const auto delta = curTime - _prevTime;
		_prevTime = curTime;
		using namespace std::chrono;

		NDetectAr det;
		std::fill(det.begin(), det.end(), 1);
		_inputMapSet->nDetect(det);
		_ki->update(state, duration_cast<milliseconds>(delta), det);

		if(_hwTarget) {
			input::ProcessedKeys proced;
			if(_inputMapSet->proc(*_ki, proced, *this)) {
				_restoreFocusTimer->start();
			}
		}
	}
}
