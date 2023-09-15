#pragma once
#include "virtual_key_def.hpp"
#include <memory>

namespace dg {
	struct Manip;
	using VKMap_V = std::vector<VKMapping>;
	//! キーマップ切り替え & Manip呼び出しクラス
	class Keymap {
		private:
			//! 切り替え仮想キー(サイクル)
			VirtualKey		_modeSwKey;
			//! VirtualKey -> Manip::* 割り当て
			VKMap_V			_keyMapV;
			//! 使用中のキーマップ
			int				_keyMapIndex;

		public:
			Keymap();
			void setKeymap(const VKMap_V& map);
			void setKeymapSwitch(VirtualKey key);
			bool keySwitch(VirtualKey key);
			void manipulate(VirtualKey key, const Manip* m, HWND hw);
	};
}
