#include "input.hpp"

namespace dg {
	// --- Act_Inst ---
	Act_Inst::Act_Inst(void (Manip::*ptr)(HWND) const):
		_ptr(ptr) {}
	void Act_Inst::proc(InputMapSet& /*ims*/, const Manip* manip, const HWND hw, const PreProc& pre) const {
		pre();
		(manip->*_ptr)(hw);
	}
	// --- Act_Func ---
	Act_Func::Act_Func(const Proc& proc):
		  _proc(proc) {}
	void Act_Func::proc(InputMapSet& ims, const Manip* /*manip*/, HWND /*hw*/, const PreProc& /*pre*/) const {
		_proc(ims);
	}

	// --- KI_Press ---
	KI_Press::KI_Press(const VirtualKey key):
		key(key) {}
	bool KI_Press::check(const KeyDiff& k) {
		if(k.pressed)
			return key == k.key;
		return false;
	}
	// --- KI_Release ---
	KI_Release::KI_Release(const VirtualKey key):
		  key(key) {}
	bool KI_Release::check(const KeyDiff& k) {
		if(!k.pressed)
			return key == k.key;
		return false;
	}

	// --- InputMapLayer ---
	void InputMapLayer::addOnPress(VirtualKey key, ManipF func) {
		addMap(std::make_unique<KI_Press>(key),
			   std::make_shared<Act_Inst>(func));
	}
	bool InputMapLayer::proc(InputMapSet& ims, const KeyDiff& k, const Manip* m, HWND hw, const PreProc& pre) {
		for(auto&& a : _actionMap) {
			if(a.first->check(k)) {
				a.second->proc(ims, m, hw, pre);
				return true;
			}
		}
		return false;
	}
	void InputMapLayer::reset() {
		for(auto&& a : _actionMap) {
			a.first->reset();
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
	bool InputMapSet::proc(InputMapSet& ims, const KeyDiff& k, const Manip* m, HWND hw, const PreProc& pre) {
		for(auto itr = _inputLayer.rbegin(); itr!=_inputLayer.rend() ; ++itr) {
			if((*itr)->proc(ims, k, m, hw, pre)) {
				reset();
				return true;
			}
		}
		return false;
	}
	void InputMapSet::reset() {
		for(auto&& s : _inputLayer) {
			s->reset();
		}
	}
}
