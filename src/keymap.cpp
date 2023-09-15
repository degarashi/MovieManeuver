#include "keymap.hpp"

namespace dg {
	Keymap::Keymap():
		_modeSwKey(VirtualKey::Invalid),
		_keyMapIndex(0)
	{}

	void Keymap::setKeymap(const VKMap_V& map) {
		_keyMapV = map;
		_keyMapIndex = 0;
	}
	void Keymap::setKeymapSwitch(const VirtualKey key) {
		_modeSwKey = key;
	}
	bool Keymap::keySwitch(const VirtualKey key) {
		if(key == _modeSwKey) {
			// キーマップモード切り替え
			_keyMapIndex = (++_keyMapIndex) % _keyMapV.size();
			return true;
		}
		return false;
	}
	void Keymap::manipulate(const VirtualKey key, const Manip* m, const HWND hw) {
		const auto& km = _keyMapV[_keyMapIndex];
		auto itr = km.find(key);
		if(itr != km.end()) {
			const auto method = itr->second;
			(m->*method)(hw);
		}
	}
}
