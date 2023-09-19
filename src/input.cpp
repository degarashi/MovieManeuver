#include "input.hpp"

namespace dg {
	// --- Act_Manip ---
	Act_Manip::Act_Manip(void (Manip::*ptr)(HWND) const):
		_ptr(ptr) {}
	void Act_Manip::proc(const Manip* manip, const HWND hw, const PreProc& pre) const {
		pre();
		(manip->*_ptr)(hw);
	}
	// --- Act_Func ---
	Act_Func::Act_Func(const Proc& proc):
		  _proc(proc) {}
	void Act_Func::proc(const Manip* /*manip*/, HWND /*hw*/, const PreProc& /*pre*/) const {
		_proc();
	}

	// --- KeyInput ---
	KeyInput::KeyInput(const Priority prio):
		priority(prio) {}
	bool KeyInput::needReset() const {
		return false;
	}
	void KeyInput::reset() {}

	// --- KI_Press ---
	KI_Press::KI_Press(const Priority prio, const VirtualKey key):
		KeyInput(prio),
		key(key) {}
	bool KI_Press::check(const VKStateAr& state) {
		return  state[static_cast<int>(key)].pressed();
	}
	// --- KI_Release ---
	KI_Release::KI_Release(const Priority prio, const VirtualKey key):
		  KeyInput(prio),
		  key(key) {}
	bool KI_Release::check(const VKStateAr& state) {
		return state[static_cast<int>(key)].released();
	}
	// --- KI_Step ---
	KI_Step::KI_Step(const Priority prio, const VirtualKey first, const VirtualKey second):
		  KeyInput(prio),
		  key{first, second}
	{}
	bool KI_Step::check(const VKStateAr& state) {
		return state[static_cast<int>(key[0])].pressing()
			   && state[static_cast<int>(key[1])].pressed();
	}

   // --- InputMapLayer ---
	void InputMapLayer::_resort() {
		std::sort(_actionMap.begin(), _actionMap.end(),
			[](const ActionEntry& ent0, const ActionEntry& ent1){
				return ent0.first->priority < ent1.first->priority;
			});
	}
	void InputMapLayer::addOnPress(const Priority prio, VirtualKey key, ManipF func) {
		addMap(std::make_unique<KI_Press>(prio, key),
			   std::make_shared<Act_Manip>(func));
	}
	InputMapAbst::Result InputMapLayer::proc(const VKStateAr& state, const Manip* m, HWND hw, const PreProc& pre) {
		for(auto&& a : _actionMap) {
			if(a.first->check(state)) {
				a.second->proc(m, hw, pre);
				if(a.first->needReset()) {
					return Result::ProceededWithReset;
				}
				return Result::Proceeded;
			}
		}
		return Result::NotProceeded;
	}
	void InputMapLayer::reset() {
		for(auto& ent: _actionMap) {
			ent.first->reset();
		}
	}
	// --- InputMapSet ---
	void InputMapSet::removeSet() {
		if(auto itr = std::prev(_inputLayer.end(), 1);
			itr != _inputLayer.end())
		{
			_inputLayer.erase(itr);
		}
	}
	InputMapAbst::Result InputMapSet::proc(const VKStateAr& state, const Manip* m, HWND hw, const PreProc& pre) {
		for(auto itr = _inputLayer.rbegin(); itr!=_inputLayer.rend() ; ++itr) {
			if(const auto res = (*itr)->proc(state, m, hw, pre);
				res != Result::NotProceeded)
			{
				return res;
			}
		}
		return Result::NotProceeded;
	}
	void InputMapSet::reset() {
		for(auto& layer : _inputLayer) {
			layer->reset();
		}
	}
}
