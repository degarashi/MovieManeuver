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

	// --- KI_Press ---
	KI_Press::KI_Press(const VirtualKey key):
		key(key) {}
	bool KI_Press::check(const VKStateAr& state) {
		return state[static_cast<int>(key)].pressed();
	}
	// --- KI_Release ---
	KI_Release::KI_Release(const VirtualKey key):
		  key(key) {}
	bool KI_Release::check(const VKStateAr& state) {
		return state[static_cast<int>(key)].released();
	}
	// --- KI_Step ---
	KI_Step::KI_Step(const VirtualKey first, const VirtualKey second):
		  key{first, second}
	{}
	bool KI_Step::check(const VKStateAr& state) {
		return state[static_cast<int>(key[0])].pressing()
			   && state[static_cast<int>(key[1])].pressed();
	}

	// --- InputMapLayer ---
	void InputMapLayer::addOnPress(VirtualKey key, ManipF func) {
		addMap(std::make_unique<KI_Press>(key),
			   std::make_shared<Act_Manip>(func));
	}
	bool InputMapLayer::proc(const VKStateAr& state, const Manip* m, HWND hw, const PreProc& pre) {
		for(auto&& a : _actionMap) {
			if(a.first->check(state)) {
				a.second->proc(m, hw, pre);
				return true;
			}
		}
		return false;
	}

	// --- InputMapSet ---
	void InputMapSet::removeSet() {
		if(auto itr = std::prev(_inputLayer.end(), 1);
			itr != _inputLayer.end())
		{
			_inputLayer.erase(itr);
		}
	}
	bool InputMapSet::proc(const VKStateAr& state, const Manip* m, HWND hw, const PreProc& pre) {
		for(auto itr = _inputLayer.rbegin(); itr!=_inputLayer.rend() ; ++itr) {
			if((*itr)->proc(state, m, hw, pre)) {
				return true;
			}
		}
		return false;
	}
}
