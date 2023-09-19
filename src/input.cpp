#include "input.hpp"
#include "keyinput.hpp"
#include "keyaction.hpp"
#include "keydetect.hpp"
#include <cassert>

namespace dg::input {
   // --- InputMapLayer ---
	void InputMapLayer::addOnPress(const Priority prio, VirtualKey key, ManipF func) {
		addMap(
			prio,
			std::make_shared<KD_Press>(key),
			std::make_shared<Act_Method>(
				func,
				true
			)
		);
	}
	bool InputMapLayer::proc(const KeyInput& ki, ProcessedKeys& proced, ActionParam& ap) {
		std::sort(_mapEnt.begin(), _mapEnt.end());

		bool ret = false;
		for(auto&& ent : _mapEnt) {
			if(ent.detect->check(ki, proced)) {
				ent.action->action(ap);
				ret = true;
			}
		}
		return ret;
	}
	void InputMapLayer::nDetect(NDetectAr& dst) const {
		for(auto&& ent : _mapEnt) {
			ent.detect->nDetect(dst);
		}
	}

	// --- InputMapSet ---
	void InputMapSet::removeLastLayer() {
		if(auto itr = std::prev(_inputLayer.end(), 1);
			itr != _inputLayer.end())
		{
			_inputLayer.erase(itr);
		}
	}
	bool InputMapSet::proc(const KeyInput& ki, ProcessedKeys& proced, ActionParam& ap) {
		bool ret = false;
		// 優先度の高い順(末端)から処理
		for(auto itr = _inputLayer.rbegin(); itr!=_inputLayer.rend() ; ++itr) {
			ret |= (*itr)->proc(ki, proced, ap);
		}
		return ret;
	}
	void InputMapSet::nDetect(NDetectAr& dst) const {
		for(auto&& l: _inputLayer) {
			l->nDetect(dst);
		}
	}

}
